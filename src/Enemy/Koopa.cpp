#include <Enemy/Koopa.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraShake.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <math.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

// Bowser's nerves keep their theNerve() inline in the class, which leaves weak
// nerve$localstatic0$theNerve__... objects in the map. They live in this file
// rather than the header: MapObjCorona.cpp includes Koopa.hpp and has none of
// those objects.
#define DECLARE_KOOPA_NERVE(Name, Base)                                        \
	class Name : public Base {                                                 \
	public:                                                                    \
		virtual BOOL execute(TSpineBase<TLiveActor>*) const;                   \
		static const Name& theNerve()                                          \
		{                                                                      \
			static Name nerve;                                                 \
			return nerve;                                                      \
		}                                                                      \
	};

// Base of the nerves that can turn Bowser towards mTargetDir.
class TNerveKoopaTurn : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>*) const = 0;

	// fabricated: shared by TNerveKoopaTurnR and TNerveKoopaTurnL
	void turn(TKoopa* koopa, f32 step) const
	{
		if (step > 0.0f)
			koopa->changeAnm(KOOPA_ANM_TURN_R, 0,
			                 step * koopa->getParam()->turnAnim.get());
		else
			koopa->changeAnm(KOOPA_ANM_TURN_L, 0,
			                 -step * koopa->getParam()->turnAnim.get());
		koopa->mRotation.y = MsWrapMod(koopa->mRotation.y + step, -180.0f, 180.0f);
	}
};

DECLARE_KOOPA_NERVE(TNerveKoopaWait, TNerveKoopaTurn);
DECLARE_KOOPA_NERVE(TNerveKoopaTumble, TNerveKoopaTurn);
DECLARE_KOOPA_NERVE(TNerveKoopaFlame, TNerveKoopaTurn);
DECLARE_KOOPA_NERVE(TNerveKoopaFall, TNerveBase<TLiveActor>);
DECLARE_KOOPA_NERVE(TNerveKoopaProvoke, TNerveBase<TLiveActor>);
DECLARE_KOOPA_NERVE(TNerveKoopaStagger, TNerveBase<TLiveActor>);
DECLARE_KOOPA_NERVE(TNerveKoopaGetShowered, TNerveBase<TLiveActor>);
DECLARE_KOOPA_NERVE(TNerveKoopaGetDown, TNerveBase<TLiveActor>);

// Turn by at most turnSpeed degrees per frame towards mTargetDir, playing the
// matching turn animation. Returns true once facing it.
// TODO: nonmatching. The target reads turnSpeed and turnAnim through an
// out-of-line getSaveParam and has a 0x1A0 frame, so the original wraps the
// turn in one more inline level than turn() here.
class TNerveKoopaTurnR : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TKoopa* koopa = (TKoopa*)spine->getBody();
		TEnemyManager* manager = (TEnemyManager*)koopa->mManager;
		f32 diff
		    = MsWrapFmodf(koopa->mTargetDir - koopa->mRotation.y, -180.0f, 180.0f);
		bool turning;
		if (diff > ((const TKoopaParams*)manager->getSaveParam())->turnSpeed.get()) {
			turn(koopa,
			     ((const TKoopaParams*)manager->getSaveParam())->turnSpeed.get());
			turning = true;
		} else if (diff > 0.0f) {
			turn(koopa, diff);
			turning = true;
		} else {
			turning = false;
		}
		return turning ? false : true;
	}
	static const TNerveKoopaTurnR& theNerve()
	{
		static TNerveKoopaTurnR nerve;
		return nerve;
	}
};

class TNerveKoopaTurnL : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const
	{
		TKoopa* koopa = (TKoopa*)spine->getBody();
		TEnemyManager* manager = (TEnemyManager*)koopa->mManager;
		f32 diff
		    = MsWrapFmodf(koopa->mTargetDir - koopa->mRotation.y, -180.0f, 180.0f);
		bool turning;
		if (diff
		    < -((const TKoopaParams*)manager->getSaveParam())->turnSpeed.get()) {
			turn(koopa,
			     -((const TKoopaParams*)manager->getSaveParam())->turnSpeed.get());
			turning = true;
		} else if (diff < 0.0f) {
			turn(koopa, diff);
			turning = true;
		} else {
			turning = false;
		}
		return turning ? false : true;
	}
	static const TNerveKoopaTurnL& theNerve()
	{
		static TNerveKoopaTurnL nerve;
		return nerve;
	}
};

static const char* koopa_bastable[] = {
	"/scene/koopa/bas/koopa_down.bas",
	nullptr,
	"/scene/koopa/bas/koopa_fall.bas",
	"/scene/koopa/bas/koopa_fire_end.bas",
	"/scene/koopa/bas/koopa_fire_loop.bas",
	"/scene/koopa/bas/koopa_fire_start.bas",
	"/scene/koopa/bas/koopa_first.bas",
	"/scene/koopa/bas/koopa_getup.bas",
	"/scene/koopa/bas/koopa_hipdrop.bas",
	"/scene/koopa/bas/koopa_stagger.bas",
	"/scene/koopa/bas/koopa_turn_l.bas",
	"/scene/koopa/bas/koopa_turn_r.bas",
	"/scene/koopa/bas/koopa_wait.bas",
	nullptr,
	"/scene/koopa/bas/koopa_waterhit.bas",
};

// fabricated
static inline TBathtub* getBathtub()
{
	return (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
}

// The angle from Bowser to where Mario is going, used by the wait and flame
// nerves to decide whether to turn first.
// fabricated
// TODO: the target calls getSaveParam and TVec3::set out of line in here, so
// the original has one more inline level around the Mario estimation. The
// UNUSED TKoopa::checkMarioWhichSide (0x1A0 bytes) is probably related.
static inline int checkTurnDir(TKoopa* koopa, bool* grip)
{
	TBathtub* bathtub = getBathtub();
	JGeometry::TVec3<f32> marioVel(SMS_GetMarioSpeedX(), SMS_GetMarioSpeedY(),
	                               SMS_GetMarioSpeedZ());
	JGeometry::TVec3<f32> estimated;
	estimated.scale(koopa->getParam()->marioEstimationWait.get(), marioVel);
	*grip = bathtub->getNextGrip(SMS_GetMarioPos(), estimated,
	                             koopa->getParam()->waitRange.get(),
	                             &koopa->mTargetDir);
	if (!*grip) {
		JGeometry::TVec3<f32> marioVel2(SMS_GetMarioSpeedX(),
		                                SMS_GetMarioSpeedY(),
		                                SMS_GetMarioSpeedZ());
		JGeometry::TVec3<f32> estimated2;
		estimated2.scale(koopa->getParam()->marioEstimationFire.get(),
		                 marioVel2);
		koopa->mTargetDir
		    = bathtub->getNextJuncture(SMS_GetMarioPos(), estimated2);
	}

	f32 diff = MsWrapFmodf(koopa->mTargetDir - koopa->mRotation.y, -180.0f,
	                       180.0f);
	f32 range = koopa->getParam()->focusRange.get();
	if (diff < -range)
		return -1;
	if (diff > range)
		return 1;
	return 0;
}

// fabricated
static inline bool isMarioOnLeft(TKoopa* koopa)
{
	return MsWrapFmodf(koopa->getTargetDir(SMS_GetMarioPos())
	                       - koopa->mTargetDir,
	                   -180.0f, 180.0f)
	       < 0.0f;
}

// TODO: getTargetDir and some nerve constructors stay out of line in the
// target here; the original runs out of inlining budget earlier than this
// version does.
DEFINE_NERVE_EXECUTE(TNerveKoopaWait, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	if (koopa->ignoresMario()) {
		koopa->changeAnm(KOOPA_ANM_WAIT, 1, koopa->getParam()->waitSpeed.get());
		return false;
	}

	bool grip;
	int dir = checkTurnDir(koopa, &grip);
	if (grip) {
		switch (dir) {
		case -1:
			spine->pushNerve(&TNerveKoopaTurnL::theNerve());
			break;
		case 1:
			spine->pushNerve(&TNerveKoopaTurnR::theNerve());
			break;
		default:
		case 0: {
			koopa->changeAnm(KOOPA_ANM_WAIT, 1,
			                 koopa->getParam()->waitSpeed.get());
			if (koopa->canTumble() && getBathtub()->allowsTumble())
				spine->pushNerve(&TNerveKoopaTumble::theNerve());
			break;
		}
		}
		return false;
	}

	switch (dir) {
	case -1:
		spine->pushNerve(&TNerveKoopaTurnL::theNerve());
		break;
	case 1:
		spine->pushNerve(&TNerveKoopaTurnR::theNerve());
		break;
	default:
	case 0:
		koopa->mFlameFlipped = isMarioOnLeft(koopa);
		spine->setNext(&TNerveKoopaFlame::theNerve());
		break;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaTumble, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	koopa->changeAnm(KOOPA_ANM_HIPDROP, 0, koopa->getParam()->tumbleSpeed.get());
	koopa->getMActor()->getFrameCtrl(ANM_TYPE_BCK);

	if (spine->getTime() == 190) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK27, 1.0f);
		static TBathtub* bathtub = getBathtub();
		gpMarioParticleManager->emitAndBindToMtx(
		    SCENE_KOOPA_JPA_MS_KP_HIPDROP, *bathtub->getRootJointMtx(), 0,
		    this);
		if (SMS_IsMarioTouchGround4cm())
			SMSRumbleMgr->start(1, (f32*)nullptr);
	}

	if (koopa->endsAnm())
		return true;

	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaFall, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	koopa->changeAnm(KOOPA_ANM_FALL, 0, koopa->getParam()->fallSpeed.get());
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaFlame, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	switch (koopa->getMActor()->getCurAnmIdx(ANM_TYPE_BCK)) {
	case KOOPA_ANM_FIRE_START:
		if (koopa->endsAnm()) {
			koopa->changeAnm(KOOPA_ANM_FIRE_LOOP, 0, 2.0f);
			spine->setNext(&TNerveKoopaFlame::theNerve());
		} else {
			koopa->mWillLaugh = false;
		}
		break;

	case KOOPA_ANM_FIRE_END:
		if (koopa->endsAnm()) {
			if (koopa->mWillLaugh)
				koopa->laugh();

			if (koopa->ignoresMario()) {
				spine->setNext(&TNerveKoopaWait::theNerve());
				return false;
			}

			bool grip;
			int dir = checkTurnDir(koopa, &grip);
			if (grip) {
				spine->setNext(&TNerveKoopaWait::theNerve());
			} else {
				switch (dir) {
				case -1:
					spine->pushNerve(&TNerveKoopaTurnL::theNerve());
					break;
				case 1:
					spine->pushNerve(&TNerveKoopaTurnR::theNerve());
					break;
				case 0:
					koopa->mFlameFlipped
					    = isMarioOnLeft(koopa);
					koopa->changeAnm(KOOPA_ANM_FIRE_START, 0,
					                 koopa->getParam()->fireSpeed.get());
					spine->setNext(&TNerveKoopaFlame::theNerve());
					break;
				}
			}
		}
		break;

	case KOOPA_ANM_FIRE_LOOP:
		if (spine->getTime() >= koopa->getParam()->flameCount.get()) {
			if ((spine->getTime() & 7) == 0) {
				bool grip;
				int dir = checkTurnDir(koopa, &grip);
				if (grip || dir != 0)
					koopa->changeAnm(KOOPA_ANM_FIRE_END, 0,
					                 koopa->getParam()->fireSpeed.get());
			} else if (koopa->getMActor()->curAnmEndsNext(ANM_TYPE_BCK,
			                                              nullptr)) {
				if (spine->getTime()
				    >= koopa->getParam()->flameFocusEndStep.get())
					koopa->changeAnm(KOOPA_ANM_FIRE_END, 0,
					                 koopa->getParam()->fireSpeed.get());
			}
		}
		break;

	default:
		koopa->changeAnm(KOOPA_ANM_FIRE_START, 0,
		                 koopa->getParam()->fireSpeed.get());
		break;
	}

	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaProvoke, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	koopa->changeAnm(KOOPA_ANM_FIRST, 0, 2.0f);
	if (koopa->endsAnm()) {
		spine->setNext(&TNerveKoopaWait::theNerve());
		return false;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaStagger, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	koopa->changeAnm(KOOPA_ANM_STAGGER, 0, koopa->getParam()->staggerSpeed.get());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaGetShowered, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	koopa->changeAnm(KOOPA_ANM_WATERHIT, 0,
	                 koopa->getParam()->waterhitSpeed.get());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaGetDown, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	switch (koopa->getMActor()->getCurAnmIdx(ANM_TYPE_BCK)) {
	case KOOPA_ANM_DOWN:
		if (koopa->endsAnm())
			koopa->changeAnm(KOOPA_ANM_DOWN_LOOP, 0,
			                 koopa->getParam()->downSpeed.get());
		break;

	case KOOPA_ANM_DOWN_LOOP:
		if (!(spine->getTime() * (getBathtub()->getNumGripsDead() + 2)
		      < koopa->getParam()->downStep.get())
		    && koopa->endsAnm())
			koopa->changeAnm(KOOPA_ANM_GETUP, 0,
			                 koopa->getParam()->downSpeed.get());
		break;

	case KOOPA_ANM_GETUP:
		if (koopa->endsAnm())
			return true;
		break;

	default: {
		koopa->changeAnm(KOOPA_ANM_DOWN, 0, koopa->getParam()->downSpeed.get());
		static TBathtub* bathtub = getBathtub();
		gpMarioParticleManager->emitAndBindToMtx(
		    SCENE_KOOPA_JPA_MS_KP_HIPDROP, *bathtub->getRootJointMtx(), 0,
		    this);
		break;
	}
	}

	return false;
}

TKoopaParts::TKoopaParts(const char* name, u32 actor_type, TKoopa* koopa,
                         f32 radius)
    : THitActor(name)
    , mKoopa(koopa)
{
	static_cast<JDrama::TViewObjPtrListT<THitActor>*>(
	    JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(this);
	initHitActor(actor_type, 5, 0x88000000, radius, radius, radius, radius);
	onHitFilter(HIT_FILTER_NO_ATTACK);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_CATEGORY_ENEMY);
	onHitFilter(HIT_CATEGORY_BOSS);
}

void TKoopaParts::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & 1) {
		control();
		for (int i = 0; i < mColCount; i++)
			attack_(mCollisions[i]);
	}
}

void TKoopaParts::set(const JGeometry::TVec3<f32>& pos, f32 radius, f32 height)
{
	mPosition.set(pos);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	offHitFilter(HIT_FILTER_NO_DAMAGE);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mAttackRadius = radius;
	mAttackHeight = height;
	mDamageRadius = radius;
	mDamageHeight = height;
	calcEntryRadius();
}

void TKoopaParts::remove()
{
	onHitFilter(HIT_FILTER_NO_ATTACK);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

TKoopaFlame::TKoopaFlame(TKoopa* koopa)
    : TKoopaParts("クッパの吐く炎", ACTOR_TYPE_BOSS_UNK29, koopa, 100.0f)
    , mMaxLength(0.0f)
    , mLength(1.0f)
{
}

bool TKoopaFlame::isAlive() const { return mLength < mMaxLength; }

f32 TKoopaFlame::getLength() const { return mLength; }

void TKoopaFlame::resetFlame()
{
	mMaxLength = 0.0f;
	mLength    = 1.0f;
}

void TKoopaFlame::fire(const JGeometry::TVec3<f32>& origin,
                       const JGeometry::TVec3<f32>& dir, f32 speed, f32 length,
                       f32 radius, f32 height)
{
	mPosition.set(origin);
	mDir.set(dir);
	mOrigin.set(origin);
	mSpeed     = speed;
	mMaxLength = length;
	mLength    = 0.0f;
	mRadius    = radius;
	mHeight    = height;
}

void TKoopaFlame::control()
{
	if (!isAlive()) {
		remove();
		return;
	}

	mLength += mSpeed;
	f32 height = mHeight;
	f32 radius = mRadius;
	JGeometry::TVec3<f32> pos;
	pos.scaleAdd(mLength, mDir, mOrigin);
	if (height <= 0.0f)
		height = 2.0f * radius;
	set(pos, radius, height);
}

BOOL TKoopaFlame::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		return false;
	}
	return true;
}

void TKoopaFlame::attack_(THitActor* actor)
{
	if (actor->receiveMessage(this, HIT_MESSAGE_BURN)
	    && actor == gpMarioAddress) {
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f),
		               mKoopa->getParam()->flameJump.get());
		mKoopa->mWillLaugh = true;
		mKoopa->stopFlame();
		mKoopa->setIgnoreMario(240);
	}
}

TKoopaHand::TKoopaHand(TKoopa* koopa)
    : TKoopaParts("クッパの手", HIT_CATEGORY_BOSS | 0x2B, koopa, 100.0f)
{
}

BOOL TKoopaHand::receiveMessage(THitActor* sender, u32 message) { return true; }

void TKoopaHand::attack_(THitActor* actor)
{
	actor->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

TKoopaHead::TKoopaHead(TKoopa* koopa)
    : TKoopaParts("クッパの頭", ACTOR_TYPE_BOSS_UNK2A, koopa, 100.0f)
{
}

BOOL TKoopaHead::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		if (mKoopa->getShowered()) {
			gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
			                             &sender->mPosition, 0, nullptr);
			SMSGetMSound()->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
			                              &mKoopa->mPosition, 0, 0.0f, 0, 0,
			                              4);
		}
		break;
	case HIT_MESSAGE_ATTACK:
		if (sender->mActorType == ACTOR_TYPE_BATHTUB_KILLER)
			mKoopa->stagger(false);
		break;
	}
	return true;
}

void TKoopaHead::attack_(THitActor* actor)
{
	if (actor->receiveMessage(this, HIT_MESSAGE_ATTACK)
	    && actor == SMS_GetMarioLiveActor())
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
}

TKoopaBody::TKoopaBody(TKoopa* koopa)
    : TKoopaParts("クッパの体", ACTOR_TYPE_BOSS_UNK2A, koopa, 100.0f)
{
}

BOOL TKoopaBody::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		break;
	case HIT_MESSAGE_ATTACK:
		if (sender->mActorType == ACTOR_TYPE_BATHTUB_KILLER)
			mKoopa->stagger(false);
		break;
	}
	return true;
}

void TKoopaBody::attack_(THitActor* actor)
{
	if (actor->receiveMessage(this, HIT_MESSAGE_ATTACK)
	    && actor == SMS_GetMarioLiveActor())
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
}

f32 TKoopa::getFlameDirDegree() const
{
	f32 rate = getFlameDirRate() * getParam()->flameNeckRange.get();
	return mRotation.y + (mFlameFlipped ? -rate : rate);
}

namespace {

// Turns Bowser's neck joint towards Mario, and sideways while he breathes
// fire.
BOOL KoopaNeckCallBack(J3DNode* node, int calc)
{
	if (calc != 0)
		return TRUE;

	TKoopa* koopa = (TKoopa*)node->getCallBackUserData();
	TMtx34f& mtx  = *(TMtx34f*)j3dSys.getModel()->getAnmMtx(
        ((J3DJoint*)node)->getJntNo());

	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario.y += 85.0f;
	toMario.x -= mtx.at(0, 3);
	toMario.y -= mtx.at(1, 3);
	toMario.z -= mtx.at(2, 3);

	if (koopa->isFlaming()) {
		f32 yaw = koopa->getFlameDirRate() * 6.2831855f
		          * koopa->getParam()->flameNeckRange.get() / 360.0f;
		f32 pitch = yaw * koopa->getParam()->flameNeckDownRate.get();
		if (pitch > 0.0f)
			pitch = -pitch;
		if (koopa->mFlameFlipped)
			yaw = -yaw;

		f32 s = sinf(yaw);
		f32 c = cosf(yaw);
		TMtx34f rotY;
		rotY.set(c, 0.0f, s, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -s, 0.0f, c, 0.0f);
		mtx.concat(rotY, mtx);

		s = sinf(pitch);
		c = cosf(pitch);
		TMtx34f rotZ;
		rotZ.set(c, -s, 0.0f, 0.0f, s, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		mtx.concat(mtx, rotZ);
	}

	f32 focus = koopa->getNeckFocus();

	JGeometry::TVec3<f32> xAxis(mtx.at(0, 0), mtx.at(1, 0), mtx.at(2, 0));
	JGeometry::TVec3<f32> yAxis(mtx.at(0, 1), mtx.at(1, 1), mtx.at(2, 1));

	// Mario's direction, flattened onto the neck's turning plane
	JGeometry::TVec3<f32> flat;
	flat.scaleAdd(-yAxis.dot(toMario), yAxis, toMario);
	flat.normalize();
	toMario.normalize();

	f32 dot = xAxis.dot(flat);
	if (dot < 0.5)
		focus *= (1.0f + dot) / 1.5f;

	JGeometry::TQuat4<f32> rot;
	rot.setRotate(xAxis, flat, focus);
	rot.normalize();

	if (!koopa->isFlaming()) {
		JGeometry::TVec3<f32> axis;
		axis.cross(flat, toMario);
		f32 angle = fabsf(atan2f(axis.length(), flat.dot(toMario)));
		if (toMario.dot(yAxis) < 0.0f)
			angle = -angle;

		JGeometry::TVec3<f32> zDir;
		rot.getZDir(zDir);
		JGeometry::TQuat4<f32> tilt;
		tilt.setRotate(zDir, angle * focus);
		tilt.normalize();
		rot.mul(tilt, rot);
	}

	TPosition3f rotMtx;
	rotMtx.setQuat(rot);
	rotMtx.zeroTrans();
	mtx.concat(mtx, rotMtx);
	MTXCopy(mtx.mMtx, J3DSys::mCurrentMtx);
	return TRUE;
}

} // namespace

void TKoopa::stopFlame()
{
	changeAnm(KOOPA_ANM_FIRE_END, 0, getParam()->fireSpeed.get());
}

// Spawn the next flame hit actor from Bowser's mouth, unless the last one is
// still too close to it.
void TKoopa::breathFlame()
{
	int free     = -1;
	bool tooNear = false;
	for (int i = 0; i < ARRAY_COUNT(mFlames); i++) {
		if (!mFlames[i]->isAlive())
			free = i;
		else if (mFlames[i]->getLength() < 2.0f * getParam()->flameRadius.get())
			tooNear = true;
	}

	if (!tooNear && free >= 0) {
		MtxPtr head = getHeadMtx();
		JGeometry::TVec3<f32> origin(head[0][3], head[1][3] - 500.0f,
		                             head[2][3]);
		JGeometry::TVec3<f32> dir(head[0][0], 0.0f, head[2][0]);
		dir.normalize();
		mFlames[free]->fire(origin, dir, getParam()->flameVelocity.get(),
		                    4000.0f, getParam()->flameRadius.get(),
		                    getParam()->flameHeight.get());
	}
}

void TKoopa::resetFlame_()
{
	for (int i = 0; i < ARRAY_COUNT(mFlames); i++)
		mFlames[i]->resetFlame();
}

void TKoopa::setUpHitActors()
{
	// TODO: this block is probably breathFlame(), but that doesn't get inlined
	// here (it compiles to 0x2B4 bytes on its own instead of the map's 0x1C0).
	if (isBreathing()) {
		int free     = -1;
		bool tooNear = false;
		for (int i = 0; i < ARRAY_COUNT(mFlames); i++) {
			if (!mFlames[i]->isAlive())
				free = i;
			else if (mFlames[i]->getLength()
			         < 2.0f * getParam()->flameRadius.get())
				tooNear = true;
		}

		if (!tooNear && free >= 0) {
			MtxPtr head = getHeadMtx();
			JGeometry::TVec3<f32> origin(head[0][3], head[1][3] - 500.0f,
			                             head[2][3]);
			JGeometry::TVec3<f32> dir(head[0][0], 0.0f, head[2][0]);
			dir.normalize();
			mFlames[free]->fire(origin, dir, getParam()->flameVelocity.get(),
			                    4000.0f, getParam()->flameRadius.get(),
			                    getParam()->flameHeight.get());
		}
	} else {
		resetFlame_();
	}

	MtxPtr jaw = getMActor()->getModel()->getAnmMtx(mJawJoint);
	f32 radius = getParam()->headRadius.get();
	mHead->set(JGeometry::TVec3<f32>(jaw[0][3], jaw[1][3] - 200.0f, jaw[2][3]),
	           radius, 2.0f * radius);
	mBody->set(mPosition, 800.0f, 2000.0f);
}

void TKoopa::changeAnm(int bck, int btp, f32 speed)
{
	if (!getMActor()->checkCurBckFromIndex(bck)) {
		getMActor()->setBckFromIndex(bck);
		setAnmSound(getBas(bck));
	}

	if (btp != getMActor()->getCurAnmIdx(ANM_TYPE_BTP))
		getMActor()->setBtpFromIndex(btp);

	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 rate = 0.5f * (speed * SMSGetAnmFrameRate());
	ctrl->setRate(rate);
}

int TKoopa::getAnmIndex() const
{
	return getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
}

bool TKoopa::endsAnm() const
{
	return getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr);
}

// Laugh once the flame that hit Mario has died down.
void TKoopa::laugh()
{
	SMSGetMSound()->startSoundActor(MSD_SE_BS_KOOPA_VO_LAUGH, &mSoundPos, 0,
	                                nullptr, 0, 4);
	mWillLaugh = false;
}

bool TKoopa::isBreathing() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP)
		return true;
	if (getAnmIndex() == KOOPA_ANM_FIRE_START && getAnmFrame() >= 85.0f)
		return true;
	return false;
}

bool TKoopa::ignoresMario() const { return mIgnoreMarioTimer > 0; }

void TKoopa::setIgnoreMario(long time) { mIgnoreMarioTimer = time; }

f32 TKoopa::getFlameDirRate() const
{
	f32 frame  = getAnmFrame();
	f32 end    = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
	f32 over   = getParam()->flameOverStart.get();
	int start  = getParam()->flameFocusStartStep.get();
	int finish = getParam()->flameFocusEndStep.get();
	int time   = mSpine->getTime();

	f32 rate;
	int anm = getAnmIndex();
	switch (anm) {
	case KOOPA_ANM_FIRE_START:
		return -(frame * over / end);

	case KOOPA_ANM_FIRE_END:
	case KOOPA_ANM_FIRE_LOOP:
		if (mSpine->getTime() <= start)
			rate = -over;
		else if (time <= finish)
			rate = (1.0f + over) * (time - start) / (finish - start) - over;
		else
			rate = 1.0f;

		if (anm == KOOPA_ANM_FIRE_END)
			return rate * (1.0f - frame / end);
		return rate;

	default:
		return 0.0f;
	}
}

bool TKoopa::isFlaming() const
{
	switch (getAnmIndex()) {
	case KOOPA_ANM_FIRE_END:
	case KOOPA_ANM_FIRE_LOOP:
	case KOOPA_ANM_FIRE_START:
		return true;
	default:
		return false;
	}
}

bool TKoopa::isProvoking() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRST) {
		f32 frame = getAnmFrame();
		if (68.0f <= frame && frame <= 164.0f)
			return true;
	}
	return false;
}

// How strongly the neck follows Mario (0 = not at all, 1 = fully), as a
// curve over each animation.
f32 TKoopa::getNeckFocus() const
{
	int anm            = getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 end            = ctrl->getEnd();
	f32 frame          = ctrl->getFrame();

	f32 focus = 1.0f;
	switch (anm) {
	case KOOPA_ANM_FIRST:
		if (frame >= 164.0f)
			focus = (frame - 164.0f) / (end - 164.0f);
		else
			focus = 0.0f;
		break;
	case KOOPA_ANM_FALL:
		focus = 0.0f;
		break;
	case KOOPA_ANM_DOWN:
		if (frame <= 40.0f)
			focus = 1.0f - frame / 40.0f;
		else
			focus = 0.0f;
		break;
	case KOOPA_ANM_DOWN_LOOP:
		focus = 0.0f;
		break;
	case KOOPA_ANM_GETUP:
		if (frame <= 125.0f)
			focus = 0.0f;
		else
			focus = (frame - 125.0f) / (end - 125.0f);
		break;
	case KOOPA_ANM_STAGGER:
		if (frame <= 30.0f)
			focus = 1.0f - frame / 30.0f;
		else if (frame <= 65.0f)
			focus = 0.0f;
		else
			focus = (frame - 65.0f) / (end - 65.0f);
		break;
	case KOOPA_ANM_HIPDROP:
		if (frame <= 30.0f)
			focus = 1.0f - frame / 30.0f;
		else if (frame <= 170.0f)
			focus = 0.0f;
		else
			focus = (frame - 170.0f) / (end - 170.0f);
		break;
	case KOOPA_ANM_WATERHIT:
		if (frame <= 20.0f)
			focus = 1.0f - frame / 20.0f;
		else if (frame <= 40.0f)
			focus = 0.0f;
		else
			focus = (frame - 40.0f) / (end - 40.0f);
		break;
	case KOOPA_ANM_FIRE_LOOP:
		focus = 0.0f;
		break;
	case KOOPA_ANM_FIRE_START:
		if (frame <= 103.0f)
			focus = 1.0f - frame / 103.0f;
		else
			focus = 0.0f;
		break;
	case KOOPA_ANM_FIRE_END:
		focus = frame / end;
		break;
	case KOOPA_ANM_WAIT:
		// looks away from Mario twice during the idle loop
		if (frame <= 200.0f)
			focus = 1.0f;
		else if (frame <= 255.0f)
			focus = 1.0f - (frame - 200.0f) / 55.0f;
		else if (frame <= 330.0f)
			focus = 0.0f;
		else if (frame <= 390.0f)
			focus = (frame - 330.0f) / 60.0f;
		else if (frame <= 440.0f)
			focus = 1.0f;
		else if (frame <= 480.0f)
			focus = 1.0f - (frame - 440.0f) / 40.0f;
		else if (frame <= 555.0f)
			focus = 0.0f;
		else if (frame <= 615.0f)
			focus = (frame - 555.0f) / 60.0f;
		break;
	case KOOPA_ANM_TURN_L:
	case KOOPA_ANM_TURN_R:
	case KOOPA_ANM_UNK13:
	default:
		break;
	}
	return focus;
}

bool TKoopa::allowsLaunch() const
{
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return false;
	return true;
}

void TKoopa::getDown()
{
	if (&TNerveKoopaFall::theNerve() == mSpine->getCurrentNerve()
	    || &TNerveKoopaProvoke::theNerve() == mSpine->getCurrentNerve()
	    || &TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;

	if (&TNerveKoopaStagger::theNerve() == mSpine->getCurrentNerve())
		mSpine->setNext(&TNerveKoopaGetDown::theNerve());
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		mSpine->setNext(&TNerveKoopaGetDown::theNerve());
	mSpine->pushNerve(&TNerveKoopaGetDown::theNerve());
}

bool TKoopa::effectsTumble() const
{
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve()
	    && mSpine->getTime() < 900 && mSpine->getTime() > 190)
		return true;
	return false;
}

bool TKoopa::getShowered()
{
	if (&TNerveKoopaFall::theNerve() == mSpine->getCurrentNerve())
		return false;
	if (&TNerveKoopaProvoke::theNerve() == mSpine->getCurrentNerve())
		return false;
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return false;
	if (&TNerveKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return false;
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		return true;

	if (&TNerveKoopaStagger::theNerve() == mSpine->getCurrentNerve()) {
		mSpine->setNext(&TNerveKoopaGetShowered::theNerve());
		return true;
	}

	if (&TNerveKoopaFlame::theNerve() == mSpine->getCurrentNerve()) {
		mSpine->setNext(&TNerveKoopaWait::theNerve());
		return false;
	}

	mSpine->pushNerve(&TNerveKoopaGetShowered::theNerve());
	return true;
}

void TKoopa::stagger(bool force)
{
	if (&TNerveKoopaFall::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaProvoke::theNerve() == mSpine->getCurrentNerve())
		return;
	if (!force && &TNerveKoopaFlame::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		return;

	mSpine->pushNerve(&TNerveKoopaStagger::theNerve());
}

// Whether Bowser is in the part of the hip drop that rocks the bathtub.
bool TKoopa::isTumbling() const
{
	f32 frame = getAnmFrame();
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve()
	    && frame >= getParam()->tumbleStartFrame.get()
	    && frame <= getParam()->tumbleEndFrame.get())
		return true;
	return false;
}

// The direction of @p pos around the bathtub, in degrees.
f32 TKoopa::getTargetDir(const JGeometry::TVec3<f32>& pos) const
{
	MtxPtr mtx = *getBathtub()->getRootJointMtx();
	JGeometry::TVec3<f32> trans(mtx[0][3], mtx[1][3], mtx[2][3]);
	JGeometry::TVec3<f32> xAxis(mtx[0][0], mtx[1][0], mtx[2][0]);
	JGeometry::TVec3<f32> zAxis(mtx[0][2], mtx[1][2], mtx[2][2]);
	JGeometry::TVec3<f32> d(pos.x - trans.x, pos.y - trans.y, pos.z - trans.z);
	return (360.0f / 65536.0f) * matan(zAxis.dot(d), xAxis.dot(d));
}

f32 TKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

f32 TKoopa::getAnmFrameNext() const
{
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	return ctrl->getFrame() + ctrl->getRate();
}

// Whether the animation crosses @p frame during this update.
bool TKoopa::passesAnmFrame(f32 frame) const
{
	if (getAnmFrame() <= frame && frame <= 0.005f + getAnmFrameNext())
		return true;
	return false;
}

// Bowser only starts a hip drop at a few points of his idle animation.
bool TKoopa::canTumble() const
{
	if (getAnmIndex() != KOOPA_ANM_WAIT)
		return false;
	if (getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
		return true;
	if (passesAnmFrame(2.0f))
		return true;
	if (passesAnmFrame(400.0f))
		return true;
	if (passesAnmFrame(700.0f))
		return true;
	return false;
}

void TKoopa::fall()
{
	mRotation.y = 180.0f;
	mSpine->setNext(&TNerveKoopaFall::theNerve());
}

void TKoopa::updateAnmSound()
{
	if (getAnmIndex() == KOOPA_ANM_HIPDROP) {
		mSoundPos.set(mPosition);
	} else {
		MtxPtr head = getHeadMtx();
		JGeometry::TVec3<f32> headPos(head[0][3], head[1][3], head[2][3]);
		mSoundPos.set(headPos);
	}

	if (mAnmSound != nullptr && mAnmSoundPath != nullptr) {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		mAnmSound->animeLoop(&mSoundPos, ctrl->getFrame(), ctrl->getRate(), 0,
		                     4);
	}
}

f32 TKoopa::getAnmEnd() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
}

TKoopa::TKoopa(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TKoopa::load(JSUMemoryInputStream& stream) { TSpineEnemy::load(stream); }

void TKoopa::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	for (int i = 0; i < ARRAY_COUNT(mFlames); i++)
		mFlames[i] = new TKoopaFlame(this);
	for (int i = 0; i < ARRAY_COUNT(mHands); i++)
		mHands[i] = new TKoopaHand(this);
	mHead = new TKoopaHead(this);
	mBody = new TKoopaBody(this);
}

void TKoopa::init(TLiveManager* manager)
{
	mBodyRadius = 800.0f;
	mHeadHeight = 2000.0f;
	TSpineEnemy::init(manager);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	mSpine->initWith(&TNerveKoopaProvoke::theNerve());

	changeAnm(KOOPA_ANM_WAIT, 1, 2.0f);
	changeAnm(KOOPA_ANM_FIRST, 0, 2.0f);
	getMActor()->initBckSimpleMotionBlend(16);

	mTargetDir = getTargetDir(SMS_GetMarioPos());

	initAnmSound();
	reset();

	JUTNameTab* names = getModel()->getModelData()->getJointName();
	for (u16 i = 0; i < names->getNameNum(); i++)
		;
	mJawJoint  = names->getIndex("ago");
	mHeadJoint = names->getIndex("head");
	mNeckJoint = names->getIndex("neck");

	J3DJoint* neck = getModel()->getModelData()->getJointNodePointer(
	    mHeadJoint);
	neck->setCallBack(KoopaNeckCallBack);
	neck->setCallBackUserData(this);

	mNeckFocus = 1.0f;
	mWillLaugh = false;
}

const char** TKoopa::getBasNameTable() const { return koopa_bastable; }

MtxPtr TKoopa::getHeadMtx() const
{
	return getMActor()->getModel()->getAnmMtx(mHeadJoint);
}

void TKoopa::reset()
{
	TSpineEnemy::reset();
	changeAnm(KOOPA_ANM_WAIT, 1, getParam()->waitSpeed.get());
	mIgnoreMarioTimer = 600;
}

void TKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 1) {
		mNeckFocus = getNeckFocus();
		if (mIgnoreMarioTimer > 0)
			mIgnoreMarioTimer--;
	}

	TSpineEnemy::perform(cue, graphics);

	for (int i = 0; i < ARRAY_COUNT(mFlames); i++)
		mFlames[i]->perform(cue, graphics);
	mHead->perform(cue, graphics);
	mHands[0]->perform(cue, graphics);
	mHands[1]->perform(cue, graphics);
	mBody->perform(cue, graphics);

	if (cue & 1) {
		if (isTumbling())
			getBathtub()->tumble(mRotation.y, getParam()->tumbleWeight.get());
		setUpHitActors();
	}

	if (cue & 2) {
		if (isBreathing() || isProvoking()) {
			getMActor()->calc();
			JGeometry::TVec3<f32> scale(getParam()->flameScale.get());

			JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(), 3, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_D, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_C, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_B, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_KOOPA_JPA_MS_KP_FIRE_A, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(scale);
		}
	}
}

BOOL TKoopa::receiveMessage(THitActor* sender, u32 message)
{
	return TSpineEnemy::receiveMessage(sender, message);
}

// Bowser stands on the bathtub: follow its root joint 1500 units down its up
// axis.
void TKoopa::calcRootMatrix()
{
	Mtx* tub = getBathtub()->getRootJointMtx();
	JGeometry::TVec3<f32> offset((*tub)[0][1], (*tub)[1][1], (*tub)[2][1]);
	offset.scale(-1500.0f);

	TPosition3f mtx;
	MsMtxSetRotRPH(mtx, 0.0f, mRotation.y, 0.0f);
	mtx.zeroTrans();
	mtx.concat(*(TPosition3f*)tub, mtx);

	mtx.getTrans(mPosition);
	mPosition += offset;
	mtx.setTrans(mPosition);

	getModel()->setBaseTRMtx(mtx);
	getModel()->setBaseScale(JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	mScaling.set(1.0f, 1.0f, 1.0f);
}

// TODO: UNUSED, 0x1A0 bytes; see checkTurnDir.
void TKoopa::checkMarioWhichSide() { }

const TKoopaParams* TKoopa::getParam() const
{
	return (const TKoopaParams*)((TEnemyManager*)mManager)->getSaveParam();
}

TKoopaManager::TKoopaManager(const char* name)
    : TEnemyManager(name)
{
}

TSpineEnemy* TKoopaManager::createEnemyInstance() { return nullptr; }

void TKoopaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "koopa_model.bmd", 0x14240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TKoopaManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TKoopaParams("/enemy/koopa.prm");
}

void TKoopaManager::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_a.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_A);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_b.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_B);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_c.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_C);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_d.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_D);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_hipdrop.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_HIPDROP);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_e.jpa",
	                 SCENE_KOOPA_JPA_MS_KP_FIRE_E);
}
