#include <Enemy/Koopa.hpp>
#include <Camera/CameraShake.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/MSSetSound.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>
#include <macros.h>

#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Strategic/Spine.hpp>

// rogue includes needed for matching sinit & bss
#include <Map/MapCollisionEntry.hpp>
#include <MSound/MSoundBGM.hpp>

inline const TNerveKoopaTurnR& TNerveKoopaTurnR::theNerve()
{
	static TNerveKoopaTurnR nerve;
	return nerve;
}

inline const TNerveKoopaTurnL& TNerveKoopaTurnL::theNerve()
{
	static TNerveKoopaTurnL nerve;
	return nerve;
}

inline const TNerveKoopaTumble& TNerveKoopaTumble::theNerve()
{
	static TNerveKoopaTumble nerve;
	return nerve;
}

inline const TNerveKoopaProvoke& TNerveKoopaProvoke::theNerve()
{
	static TNerveKoopaProvoke nerve;
	return nerve;
}

inline const TNerveKoopaWait& TNerveKoopaWait::theNerve()
{
	static TNerveKoopaWait nerve;
	return nerve;
}

inline const TNerveKoopaFlame& TNerveKoopaFlame::theNerve()
{
	static TNerveKoopaFlame nerve;
	return nerve;
}

inline const TNerveKoopaGetDown& TNerveKoopaGetDown::theNerve()
{
	static TNerveKoopaGetDown nerve;
	return nerve;
}

inline const TNerveKoopaStagger& TNerveKoopaStagger::theNerve()
{
	static TNerveKoopaStagger nerve;
	return nerve;
}

inline const TNerveKoopaFall& TNerveKoopaFall::theNerve()
{
	static TNerveKoopaFall nerve;
	return nerve;
}

inline const TNerveKoopaGetShowered& TNerveKoopaGetShowered::theNerve()
{
	static TNerveKoopaGetShowered nerve;
	return nerve;
}

inline BOOL TNerveKoopaTurnR::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	if (koopa->turnToTarget(true))
		return false;
	return true;
}

inline BOOL TNerveKoopaTurnL::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	if (koopa->turnToTarget(false))
		return false;
	return true;
}

enum TKoopaPartType {
	KOOPA_PART_TYPE_FLAME = 0x08000029,
	KOOPA_PART_TYPE_HEAD  = 0x0800002A,
	KOOPA_PART_TYPE_BODY  = 0x0800002A,
	KOOPA_PART_TYPE_HAND  = 0x0800002B,
};

enum TKoopaTargetSide {
	KOOPA_TARGET_SIDE_LEFT  = -1,
	KOOPA_TARGET_SIDE_FRONT = 0,
	KOOPA_TARGET_SIDE_RIGHT = 1,
};

// fabricated
inline int TKoopa::getTargetSide() const
{
	f32 diff = -180.0f
	           + std::fmodf(360.0f + (unk150 - mRotation.y - -180.0f), 360.0f);
	if (diff < -getParam()->getFocusRange())
		return KOOPA_TARGET_SIDE_LEFT;
	if (diff > getParam()->getFocusRange())
		return KOOPA_TARGET_SIDE_RIGHT;
	return KOOPA_TARGET_SIDE_FRONT;
}

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

DEFINE_NERVE_EXECUTE(TNerveKoopaWait, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	if (koopa->ignoresMario()) {
		koopa->changeAnm(KOOPA_ANM_WAIT, 1, koopa->getParam()->getWaitSpeed());
		return false;
	}
	bool nearGrip = koopa->updateTarget();
	int side      = koopa->getTargetSide();
	if (nearGrip) {
		switch (side) {
		case KOOPA_TARGET_SIDE_LEFT:
			spine->pushNerve(&TNerveKoopaTurnL::theNerve());
			break;
		case KOOPA_TARGET_SIDE_RIGHT:
			spine->pushNerve(&TNerveKoopaTurnR::theNerve());
			break;
		default:
		case KOOPA_TARGET_SIDE_FRONT:
			koopa->changeAnm(KOOPA_ANM_WAIT, 1,
			                 koopa->getParam()->getWaitSpeed());
			if (koopa->canTumble()
			    && ((TBathtub*)JDrama::TNameRefGen::search("バスタブ"))
			           ->allowsTumble())
				spine->pushNerve(&TNerveKoopaTumble::theNerve());
			break;
		}
		return false;
	}
	switch (side) {
	case KOOPA_TARGET_SIDE_LEFT:
		spine->pushNerve(&TNerveKoopaTurnL::theNerve());
		break;
	case KOOPA_TARGET_SIDE_RIGHT:
		spine->pushNerve(&TNerveKoopaTurnR::theNerve());
		break;
	default:
	case KOOPA_TARGET_SIDE_FRONT:
		koopa->checkMarioWhichSide();
		spine->setNext(&TNerveKoopaFlame::theNerve());
		break;
	}
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaTumble, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	koopa->changeAnm(KOOPA_ANM_HIPDROP, 0, koopa->getParam()->getTumbleSpeed());
	koopa->getAnmFrame();
	if (spine->getTime() == 190) {
		gpCameraShake->startShake((EnumCamShakeMode)0x27, 1.0f);
		TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
		gpMarioParticleManager->emitAndBindToMtx(
		    0xF5, (MtxPtr)bathtub->getRootJointMtx(), 0, this);
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
	koopa->changeAnm(KOOPA_ANM_FALL, 0, koopa->getParam()->getFallSpeed());
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaFlame, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	switch (koopa->getAnmIndex()) {
	case KOOPA_ANM_FIRE_START:
		if (koopa->endsAnm()) {
			koopa->changeAnm(KOOPA_ANM_FIRE_LOOP, 0, 2.0f);
			spine->setNext(&TNerveKoopaFlame::theNerve());
		} else {
			koopa->setUnk155(false);
		}
		break;
	case KOOPA_ANM_FIRE_END:
		if (koopa->endsAnm()) {
			if (koopa->getUnk155()) {
				koopa->laugh();
				koopa->setUnk155(false);
			}
			if (koopa->ignoresMario()) {
				spine->setNext(&TNerveKoopaWait::theNerve());
				return false;
			}
			bool nearGrip = koopa->updateTarget();
			int side      = koopa->getTargetSide();
			if (nearGrip) {
				spine->setNext(&TNerveKoopaWait::theNerve());
			} else {
				switch (side) {
				case KOOPA_TARGET_SIDE_LEFT:
					spine->pushNerve(&TNerveKoopaTurnL::theNerve());
					break;
				case KOOPA_TARGET_SIDE_RIGHT:
					spine->pushNerve(&TNerveKoopaTurnR::theNerve());
					break;
				case KOOPA_TARGET_SIDE_FRONT:
					koopa->checkMarioWhichSide();
					koopa->changeAnm(KOOPA_ANM_FIRE_START, 0,
					                 koopa->getParam()->getFireSpeed());
					spine->setNext(&TNerveKoopaFlame::theNerve());
					break;
				}
			}
		}
		break;
	case KOOPA_ANM_FIRE_LOOP:
		if (spine->getTime() >= koopa->getParam()->getFlameCount()) {
			if (!(spine->getTime() & 7)) {
				bool nearGrip = koopa->updateTarget();
				int side      = koopa->getTargetSide();
				if (nearGrip || side != KOOPA_TARGET_SIDE_FRONT)
					koopa->changeAnm(KOOPA_ANM_FIRE_END, 0,
					                 koopa->getParam()->getFireSpeed());
			} else if (koopa->endsAnm()) {
				if (spine->getTime()
				    >= koopa->getParam()->getFlameFocusEndStep())
					koopa->changeAnm(KOOPA_ANM_FIRE_END, 0,
					                 koopa->getParam()->getFireSpeed());
			}
		}
		break;
	default:
		koopa->changeAnm(KOOPA_ANM_FIRE_START, 0,
		                 koopa->getParam()->getFireSpeed());
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
	koopa->changeAnm(KOOPA_ANM_STAGGER, 0,
	                 koopa->getParam()->getStaggerSpeed());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaGetShowered, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	koopa->changeAnm(KOOPA_ANM_WATERHIT, 0,
	                 koopa->getParam()->getWaterhitSpeed());
	if (koopa->endsAnm())
		return true;
	return false;
}

DEFINE_NERVE_EXECUTE(TNerveKoopaGetDown, TLiveActor)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	switch (koopa->getAnmIndex()) {
	case KOOPA_ANM_DOWN:
		if (koopa->endsAnm())
			koopa->changeAnm(KOOPA_ANM_DOWN_LOOP, 0,
			                 koopa->getParam()->getDownSpeed());
		break;
	case KOOPA_ANM_DOWN_LOOP: {
		TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
		f32 downStep      = koopa->getParam()->getDownStep();
		if (!(spine->getTime() * (bathtub->getNumGripsDead() + 2) < downStep)
		    && koopa->endsAnm())
			koopa->changeAnm(KOOPA_ANM_GETUP, 0,
			                 koopa->getParam()->getDownSpeed());
		break;
	}
	case KOOPA_ANM_GETUP:
		if (koopa->endsAnm())
			return true;
		break;
	default:
		koopa->changeAnm(KOOPA_ANM_DOWN, 0, koopa->getParam()->getDownSpeed());
		TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
		gpMarioParticleManager->emitAndBindToMtx(
		    0xF5, (MtxPtr)bathtub->getRootJointMtx(), 0, this);
		break;
	}
	return false;
}

TKoopaParts::TKoopaParts(const char* name, u32 actorType, TKoopa* koopa,
                         f32 radius)
    : THitActor(name)
    , unk68(koopa)
{
	TIdxGroupObj* groupObj
	    = (TIdxGroupObj*)JDrama::TNameRefGen::search("敵グループ");
	groupObj->getChildren().push_back(this);
	initHitActor(actorType, 5, HIT_CATEGORY_PLAYER | HIT_CATEGORY_BOSS, radius,
	             radius, radius, radius);
	onHitFilter(HIT_FILTER_NO_ATTACK);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_CATEGORY_ENEMY);
	onHitFilter(HIT_CATEGORY_BOSS);
}

void TKoopaParts::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_MOVE) {
		control();
		for (int i = 0; i < mColCount; ++i)
			attack_(mCollisions[i]);
	}
}

// Unused
void TKoopaParts::set(const JGeometry::TVec3<f32>& pos, f32 radius, f32 height)
{
	if (height <= 0.0f)
		height = 2.0f * radius;
	mPosition.set(pos);
	offHitFilter(HIT_FILTER_NO_ATTACK);
	offHitFilter(HIT_FILTER_NO_DAMAGE);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	setHitParams(radius, height, radius, height);
}

// Unused
void TKoopaParts::remove()
{
	onHitFilter(HIT_FILTER_NO_ATTACK);
	onHitFilter(HIT_FILTER_NO_DAMAGE);
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

// Unused
TKoopaFlame::TKoopaFlame(TKoopa* koopa)
    : TKoopaParts("クッパの吐く炎", KOOPA_PART_TYPE_FLAME, koopa, 100.0f)
{
	resetFlame();
}

// Unused
bool TKoopaFlame::isAlive() const { return unk8C < unk88; }

// Unused
f32 TKoopaFlame::getLength() const { return unk8C; }

// Unused
void TKoopaFlame::resetFlame()
{
	unk88 = 0.0f;
	unk8C = 1.0f;
}

// Unused
void TKoopaFlame::fire(const JGeometry::TVec3<f32>& pos,
                       const JGeometry::TVec3<f32>& dir, f32 speed, f32 length,
                       f32 radius, f32 height)
{
	mPosition.set(pos);
	unk78.set(dir);
	unk6C.set(pos);
	unk84 = speed;
	unk88 = length;
	unk8C = 0.0f;
	unk90 = radius;
	unk94 = height;
}

void TKoopaFlame::control()
{
	if (!isAlive()) {
		remove();
		return;
	}
	unk8C += unk84;
	JGeometry::TVec3<f32> pos;
	pos.scaleAdd(unk8C, unk78, unk6C);
	set(pos, unk90, unk94);
}

BOOL TKoopaFlame::receiveMessage(THitActor*, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		return false;
	default:
		return true;
	}
}

void TKoopaFlame::attack_(THitActor* target)
{
	if (target->receiveMessage(this, HIT_MESSAGE_BURN)
	    && target == gpMarioAddress) {
		f32 jump = unk68->getParam()->getFlameJump();
		JGeometry::TVec3<f32> dir(0.0f, 1.0f, 0.0f);
		SMS_ThrowMario(dir, jump);
		unk68->setUnk155(true);
		unk68->stopFlame();
		unk68->setIgnoreMario(240);
	}
}

// Unused
TKoopaHand::TKoopaHand(TKoopa* koopa)
    : TKoopaParts("クッパの手", KOOPA_PART_TYPE_HAND, koopa, 100.0f)
{
}

BOOL TKoopaHand::receiveMessage(THitActor*, u32) { return true; }

void TKoopaHand::attack_(THitActor* target)
{
	target->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

// Unused
TKoopaHead::TKoopaHead(TKoopa* koopa)
    : TKoopaParts("クッパの頭", KOOPA_PART_TYPE_HEAD, koopa, 100.0f)
{
}

BOOL TKoopaHead::receiveMessage(THitActor* actor, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		if (unk68->getShowered()) {
			gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT,
			                             &actor->mPosition, 0, nullptr);
			gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
			                        &unk68->mPosition, 0, 0.0f, 0, 0, 4);
		}
		break;
	case HIT_MESSAGE_ATTACK:
		if (actor->getActorType() == ACTOR_TYPE_BATHTUB_KILLER)
			unk68->stagger(false);
		break;
	}
	return true;
}

void TKoopaHead::attack_(THitActor* target)
{
	if (target->receiveMessage(this, HIT_MESSAGE_ATTACK)
	    && target == SMS_GetMarioLiveActor())
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
}

// Unused
TKoopaBody::TKoopaBody(TKoopa* koopa)
    : TKoopaParts("クッパの体", KOOPA_PART_TYPE_BODY, koopa, 100.0f)
{
}

BOOL TKoopaBody::receiveMessage(THitActor* actor, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		break;
	case HIT_MESSAGE_ATTACK:
		if (actor->getActorType() == ACTOR_TYPE_BATHTUB_KILLER)
			unk68->stagger(false);
		break;
	}
	return true;
}

void TKoopaBody::attack_(THitActor* target)
{
	if (target->receiveMessage(this, HIT_MESSAGE_ATTACK)
	    && target == SMS_GetMarioLiveActor())
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
}

f32 TKoopa::getFlameDirDegree() const
{
	f32 angle = getFlameDirRate() * getParam()->getFlameNeckRange();
	return mRotation.y + (unk154 ? -angle : angle);
}

// Idk why this would be in a unnamed namespace...
// This sh also needs cleanup
namespace {
BOOL KoopaNeckCallBack(J3DNode* node, int timing)
{
	if (timing != 0)
		return true;
	TKoopa* koopa     = (TKoopa*)node->getCallBackUserData();
	TPosition3f* head = (TPosition3f*)j3dSys.getModel()->getAnmMtx(
	    ((J3DJoint*)node)->getJntNo());
	JGeometry::TVec3<f32> target = *gpMarioPos;
	target.y += 85.0f;
	JGeometry::TVec3<f32> headPos;
	head->getTrans(headPos);
	target.sub(headPos);
	if (koopa->isFlaming()) {
		f32 rate = koopa->getFlameDirRate();
		f32 yaw  = rate * (2.0f * JGeometry::TUtil<f32>::PI())
		          * koopa->getParam()->getFlameNeckRange() / 360.0f;
		f32 pitch = yaw * koopa->getParam()->getFlameNeckDownRate();
		if (pitch > 0.0f)
			pitch = -pitch;
		if (koopa->unk154)
			yaw = -yaw;
		TPosition3f rotation;
		rotation.setEularY(yaw);
		rotation.setTrans(0.0f, 0.0f, 0.0f);
		head->concat(rotation, *head);
		rotation.setEularZ(pitch);
		rotation.setTrans(0.0f, 0.0f, 0.0f);
		head->concat(*head, rotation);
	}
	f32 focus = koopa->getNeckFocus();
	JGeometry::TVec3<f32> up;
	head->getYDir(up);
	JGeometry::TVec3<f32> forward;
	head->getXDir(forward);
	JGeometry::TVec3<f32> projected;
	projected.scaleAdd(-up.dot(target), up, target);
	projected.normalize();
	target.normalize();
	f32 dot = projected.dot(forward);
	if (dot < 0.5)
		focus *= (1.0f + dot) / 1.5f;
	JGeometry::TQuat4<f32> turn;
	turn.setRotate(forward, projected, focus);
	turn.normalize();
	if (!koopa->isFlaming()) {
		f32 angle = MsAngleBetween(projected, target);
		if (target.dot(up) < 0.0f)
			angle = -angle;
		JGeometry::TVec3<f32> axis;
		turn.getZDir(axis);
		JGeometry::TQuat4<f32> pitch;
		pitch.setRotate(axis, angle * focus);
		pitch.normalize();
		turn.mul(pitch, turn);
	}
	TPosition3f rotation;
	rotation.setQuat(turn);
	rotation.setTrans(0.0f, 0.0f, 0.0f);
	head->concat(*head, rotation);
	MTXCopy(*head, J3DSys::mCurrentMtx);
	return true;
}
} // namespace

// Unused
void TKoopa::stopFlame()
{
	changeAnm(KOOPA_ANM_FIRE_END, 0, getParam()->getFireSpeed());
}

// Unused
void TKoopa::breathFlame()
{
	getMActor()->calc();
	JGeometry::TVec3<f32> scale(getParam()->getFlameScale());
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(),
	    TMarioParticleManager::EFFECT_LOOP_INDIRECT, this);
	if (emitter)
		emitter->setGlobalScale(scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_D, getHeadMtx(), scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_C, getHeadMtx(), scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_B, getHeadMtx(), scale);
	emitMtxParticle(KOOPA_JPA_MS_KP_FIRE_A, getHeadMtx(), scale);
}

// Unused
void TKoopa::resetFlame_()
{
	for (int i = 0; i < ARRAY_COUNT(unk164); ++i)
		unk164[i]->resetFlame();
}

void TKoopa::setUpHitActors()
{
	if (isBreathing()) {
		int freeFlame  = -1;
		bool nearFlame = false;
		for (int i = 0; i < ARRAY_COUNT(unk164); ++i) {
			if (!unk164[i]->isAlive())
				freeFlame = i;
			else if (unk164[i]->getLength()
			         < 2.0f * getParam()->getFlameRadius())
				nearFlame = true;
		}
		if (!nearFlame && freeFlame >= 0) {
			MtxPtr head = getHeadMtx();
			JGeometry::TVec3<f32> pos(head[0][3], head[1][3] - 500.0f,
			                          head[2][3]);
			JGeometry::TVec3<f32> dir(head[0][0], 0.0f, head[2][0]);
			dir.normalize();
			unk164[freeFlame]->fire(pos, dir, getParam()->getFlameVelocity(),
			                        4000.0f, getParam()->getFlameRadius(),
			                        getParam()->getFlameHeight());
		}
	} else {
		resetFlame_();
	}
	TPosition3f* jaw = (TPosition3f*)getMActor()->getModel()->getAnmMtx(unk1A8);
	JGeometry::TVec3<f32> headPos;
	jaw->getTrans(headPos);
	headPos.y -= 200.0f;
	unk194->set(headPos, getParam()->getHeadRadius(), 0.0f);
	unk198->set(mPosition, 800.0f, 2000.0f);
}

void TKoopa::changeAnm(int bckIndex, int btpIndex, f32 speed)
{
	if (!getMActor()->checkCurBckFromIndex(bckIndex)) {
		getMActor()->setBckFromIndex(bckIndex);
		setAnmSound(getBas(bckIndex));
	}
	if (btpIndex != getMActor()->getCurAnmIdx(ANM_TYPE_BTP))
		getMActor()->setBtpFromIndex(btpIndex);
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 rate           = speed * SMSGetAnmFrameRate();
	rate               = 0.5f * rate;
	ctrl->setRate(rate);
}

// Unused
int TKoopa::getAnmIndex() const
{
	return getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
}

// Unused
bool TKoopa::endsAnm() const
{
	return getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr);
}

// Unused
void TKoopa::laugh()
{
	gpMSound->startSoundActor(MSD_SE_BS_KOOPA_VO_LAUGH, &unk158, 0, nullptr, 0,
	                          4);
}

// Unused
bool TKoopa::isBreathing() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP)
		return true;
	else if (getAnmIndex() == KOOPA_ANM_FIRE_START && getAnmFrame() >= 85.0f)
		return true;
	else
		return false;
}

// Unused
bool TKoopa::ignoresMario() const { return unk19C > 0; }

// Unused
void TKoopa::setIgnoreMario(long time) { unk19C = time; }

f32 TKoopa::getFlameDirRate() const
{
	f32 frame     = getAnmFrame();
	f32 end       = getAnmEnd();
	f32 over      = getParam()->getFlameOverStart();
	int startStep = getParam()->getFlameFocusStartStep();
	int endStep   = getParam()->getFlameFocusEndStep();
	long time     = mSpine->getTime();
	int anm       = getAnmIndex();
	switch (anm) {
	case KOOPA_ANM_FIRE_START:
		return -(frame * over / end);
	case KOOPA_ANM_FIRE_LOOP:
	case KOOPA_ANM_FIRE_END:
		f32 rate;
		if (mSpine->getTime() <= startStep)
			rate = -over;
		else if (time <= endStep)
			rate = (1.0f + over) * (time - startStep)
			           / (f32)(endStep - startStep)
			       - over;
		else
			rate = 1.0f;
		if (anm == KOOPA_ANM_FIRE_END)
			rate *= 1.0f - frame / end;
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

// Unused
bool TKoopa::isProvoking() const
{
	if (getMActor()->getCurAnmIdx(ANM_TYPE_BCK) == KOOPA_ANM_FIRST) {
		f32 frame = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
		if (68.0f <= frame && frame <= 164.0f)
			return true;
	}
	return false;
}

f32 TKoopa::getNeckFocus() const
{
	int anm            = getAnmIndex();
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 end            = ctrl->getEnd();
	f32 frame          = ctrl->getFrame();
	f32 focus          = 1.0f;
	switch (anm) {
	case KOOPA_ANM_FIRST:
		focus = 0.0f;
		if (frame >= 164.0f)
			focus = (frame - 164.0f) / (end - 164.0f);
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
	if (&TNerveKoopaFall::theNerve() != mSpine->getCurrentNerve())
		if (&TNerveKoopaProvoke::theNerve() != mSpine->getCurrentNerve())
			if (&TNerveKoopaTumble::theNerve() != mSpine->getCurrentNerve()) {
				if (&TNerveKoopaStagger::theNerve()
				    == mSpine->getCurrentNerve())
					mSpine->setNext(&TNerveKoopaGetDown::theNerve());
				if (&TNerveKoopaGetShowered::theNerve()
				    == mSpine->getCurrentNerve())
					mSpine->setNext(&TNerveKoopaGetDown::theNerve());
				mSpine->pushNerve(&TNerveKoopaGetDown::theNerve());
			}
}

bool TKoopa::effectsTumble() const
{
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve()) {
		if (mSpine->getTime() < 900 && mSpine->getTime() > 190)
			return true;
	}
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
	if (!force && mSpine->getCurrentNerve() == &TNerveKoopaFlame::theNerve())
		return;
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		return;
	mSpine->pushNerve(&TNerveKoopaStagger::theNerve());
}

// Unused
bool TKoopa::isTumbling() const
{
	f32 frame = getAnmFrame();
	return (mSpine->getCurrentNerve() == &TNerveKoopaTumble::theNerve()
	        && frame >= getParam()->getTumbleStartFrame())
	       && frame <= getParam()->getTumbleEndFrame();
}

f32 TKoopa::getTargetDir(const JGeometry::TVec3<f32>& target) const
{
	TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");

	// I guess this is completely fine?
	TPosition3f* mtx = (TPosition3f*)bathtub->getRootJointMtx();
	JGeometry::TVec3<f32> position;
	mtx->getTrans(position);
	JGeometry::TVec3<f32> offset;
	JGeometry::TVec3<f32> xDir;
	mtx->getXDir(xDir);
	JGeometry::TVec3<f32> zDir;
	mtx->getZDir(zDir);
	offset.sub(target, position);
	f32 z = zDir.dot(offset);
	f32 x = xDir.dot(offset);
	return SHORTANGLE2DEG(matan(z, x));
}

// Unused
f32 TKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

// Unused
f32 TKoopa::getAnmFrameNext() const
{
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	return 0.005f + (ctrl->getFrame() + ctrl->getRate());
}

// Unused
bool TKoopa::passesAnmFrame(f32 frame) const
{
	return getAnmFrame() <= frame && frame <= getAnmFrameNext();
}

// Unused
bool TKoopa::canTumble() const
{
	if (getAnmIndex() != KOOPA_ANM_WAIT)
		return false;
	if (endsAnm())
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
	if (getAnmIndex() == KOOPA_ANM_HIPDROP)
		unk158.set(mPosition);
	else {
		TPosition3f* head = (TPosition3f*)getHeadMtx();
		head->getTrans(unk158);
	}
	if (mAnmSound && mAnmSoundPath) {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		mAnmSound->animeLoop(&unk158, ctrl->getFrame(), ctrl->getRate(), 0, 4);
	}
}

// Unused
f32 TKoopa::getAnmEnd() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
}

TKoopa::TKoopa(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	onLiveFlag(LIVE_FLAG_NO_BIND);
}

void TKoopa::load(JSUMemoryInputStream& stream) { TSpineEnemy::load(stream); }

void TKoopa::loadAfter()
{
	TSpineEnemy::loadAfter();
	for (int i = 0; i < ARRAY_COUNT(unk164); ++i)
		unk164[i] = new TKoopaFlame(this);
	for (int i = 0; i < ARRAY_COUNT(unk18C); ++i)
		unk18C[i] = new TKoopaHand(this);
	unk194 = new TKoopaHead(this);
	unk198 = new TKoopaBody(this);
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
	unk150 = getTargetDir(SMS_GetMarioPos());
	initAnmSound();
	reset();

	JUTNameTab* names = getModel()->getModelData()->getJointName();
	for (u16 i = 0; i < names->getNameNum(); ++i) {
		// Important to loop through nothing...
		// Debug code?
	}

	unk1A8                  = names->getIndex("ago");
	unk1A0                  = names->getIndex("head");
	unk1A4                  = names->getIndex("neck");
	J3DModelData* modelData = getModel()->getModelData();
	J3DJoint* head          = modelData->getJointNodePointer(unk1A0);
	head->setCallBack(KoopaNeckCallBack);
	head->setCallBackUserData(this);
	unk1B8 = 1.0f;
	unk155 = false;
}

const char** TKoopa::getBasNameTable() const { return koopa_bastable; }

// Unused
MtxPtr TKoopa::getHeadMtx() const
{
	J3DModel* model = getMActor()->getModel();
	return model->getAnmMtx(unk1A0);
}

void TKoopa::reset()
{
	TSpineEnemy::reset();
	changeAnm(KOOPA_ANM_WAIT, 1, getParam()->getWaitSpeed());
	setIgnoreMario(600);
}

void TKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		unk1B8 = getNeckFocus();
		if (unk19C > 0)
			--unk19C;
	}
	TSpineEnemy::perform(cue, graphics);
	for (int i = 0; i < ARRAY_COUNT(unk164); ++i)
		unk164[i]->perform(cue, graphics);
	unk194->perform(cue, graphics);
	unk18C[0]->perform(cue, graphics);
	unk18C[1]->perform(cue, graphics);
	unk198->perform(cue, graphics);
	if (cue & CUE_MOVE) {
		if (isTumbling()) {
			TBathtub* bathtub
			    = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
			bathtub->tumble(mRotation.y, getParam()->getTumbleWeight());
		}
		setUpHitActors();
	}
	if (cue & CUE_CALC_ANIM) {
		if (!isBreathing() && !isProvoking()) {
			return;
		}

		breathFlame();
	}
}

BOOL TKoopa::receiveMessage(THitActor* actor, u32 message)
{
	return TSpineEnemy::receiveMessage(actor, message);
}

void TKoopa::calcRootMatrix()
{
	TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
	TPosition3f* bathtubMtx = (TPosition3f*)bathtub->getRootJointMtx();

	JGeometry::TVec3<f32> offset(bathtubMtx->at(0, 1), bathtubMtx->at(1, 1),
	                             bathtubMtx->at(2, 1));
	offset.scale(-1500.0f);
	TPosition3f mtx;
	MsMtxSetRotRPH(mtx, 0.0f, mRotation.y, 0.0f);
	mtx.setTrans(0.0f, 0.0f, 0.0f);
	mtx.concat(*bathtubMtx, mtx);
	mtx.getTrans(mPosition);
	mPosition.add(offset);
	mtx.setTrans(mPosition);
	getModel()->setBaseTRMtx(mtx);
	getModel()->setBaseScale(JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	mScaling.set(1.0f, 1.0f, 1.0f);
}

// Unused
void TKoopa::checkMarioWhichSide()
{
	unk154
	    = -180.0f
	          + std::fmodf(
	              360.0f + (getTargetDir(SMS_GetMarioPos()) - unk150 - -180.0f),
	              360.0f)
	      < 0.0f;
}

// Unused
TKoopaParams* TKoopa::getParam() const
{
	return ((TKoopaManager*)mManager)->getSaveParam();
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
	TEnemyManager::loadAfter();
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_a.jpa", 0x1C0);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_b.jpa", 0x1C1);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_c.jpa", 0x1C2);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_d.jpa", 0x1C3);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_hipdrop.jpa", 0xF5);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_e.jpa", 0x1F3);
}
