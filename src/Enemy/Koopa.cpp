#include <Enemy/KoopaNerve.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MAnmSound.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/MapCollisionManager.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <math.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The .bas per .bck slot of koopa_model.bmd. Slots 1 (koopa_down_wait) and 13
// have no sound table of their own. This lands in .data rather than .rodata,
// so the original declaration is an unqualified `static`.
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

namespace {
int KoopaNeckCallBack(J3DNode*, int);
} // namespace

// Wraps `angle` into [-180, 180) through the helper pair in KoopaNerve.hpp;
// the wrap in turnBody uses JGeometry::TUtil<f32>::mod instead of std::fmodf,
// so the two cannot share a helper (same split as TDirectionCalc in
// koopajr.cpp).
#define KOOPA_WRAP_DEGREES(angle) KoopaWrapDegrees(angle)

// ---------------------------------------------------------------------------
// Nerves
// ---------------------------------------------------------------------------

static inline TKoopa* KoopaGetBody(TSpineBase<TLiveActor>* spine)
{
	TKoopa* koopa = (TKoopa*)spine->getBody();
	return koopa;
}

// Where Mario will be after the given estimation time. Retail calls
// TVec3::set<f> and TEnemyManager::getSaveParam out of line in every nerve
// that picks a grip, which puts the speed vector and the parameter read two
// levels below the nerve: KoopaFindGrip, then these.
static inline void KoopaEstimateMarioWait(TKoopa* koopa,
                                          JGeometry::TVec3<f32>& out)
{
	JGeometry::TVec3<f32> speed(*gpMarioSpeedX, *gpMarioSpeedY,
	                            *gpMarioSpeedZ);
	out.scale(koopa->getParam()->marioEstimationWait.get(), speed);
}

static inline void KoopaEstimateMarioFire(TKoopa* koopa,
                                          JGeometry::TVec3<f32>& out)
{
	JGeometry::TVec3<f32> speed(*gpMarioSpeedX, *gpMarioSpeedY,
	                            *gpMarioSpeedZ);
	out.scale(koopa->getParam()->marioEstimationFire.get(), speed);
}

// Retail calls TKoopa::getTargetDir out of line in both nerves that use this
// but expands it in TKoopa::init and checkMarioWhichSide: one inline level
// between the nerve and the call is what keeps it out of line there, and
// only while getTargetDir names its matan result (one statement fewer and
// it inlines through this helper too).
static inline bool KoopaIsMarioLeft(TKoopa* koopa)
{
	f32 toMario = koopa->getTargetDir(SMS_GetMarioPos());
	return KOOPA_WRAP_DEGREES(toMario - koopa->mTargetDir) < 0.0f;
}

static inline u8 KoopaFindGrip(TKoopa* koopa, TBathtub* bathtub)
{
	JGeometry::TVec3<f32> estimated;
	KoopaEstimateMarioWait(koopa, estimated);
	u8 onGrip = bathtub->getNextGrip(SMS_GetMarioPos(), estimated,
	                                 koopa->getParam()->waitRange.get(),
	                                 &koopa->mTargetDir);
	if (!onGrip) {
		KoopaEstimateMarioFire(koopa, estimated);
		koopa->mTargetDir
		    = bathtub->getNextJuncture(SMS_GetMarioPos(), estimated);
	}
	return onGrip;
}

// TODO: 99.4%. Frame 0x88 short (retail 0x260) and one scheduling swap of
// the spine load at entry; instructions otherwise match.
BOOL TNerveKoopaWait::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	if (koopa->mWaitTimer > 0) {
		koopa->changeAnm(KOOPA_ANM_WAIT, 1,
		                 koopa->getParam()->waitSpeed.get());
		return FALSE;
	}

	TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
	u8 onGrip = KoopaFindGrip(koopa, bathtub);

	f32 diff = KOOPA_WRAP_DEGREES(koopa->mTargetDir - koopa->mRotation.y);
	f32 focusRange = koopa->getParam()->focusRange.get();
	int side;
	if (diff < -focusRange)
		side = -1;
	else if (diff > focusRange)
		side = 1;
	else
		side = 0;

	if (onGrip) {
		switch (side) {
		case -1:
			spine->pushNerve(&TNerveKoopaTurnL::theNerve());
			break;
		case 1:
			spine->pushNerve(&TNerveKoopaTurnR::theNerve());
			break;
		default:
		case 0:
			koopa->changeAnm(KOOPA_ANM_WAIT, 1,
			                 koopa->getParam()->waitSpeed.get());
			if (koopa->canTumble()) {
				if (((TBathtub*)JDrama::TNameRefGen::search2("バスタブ"))
				        ->allowsTumble())
					spine->pushNerve(&TNerveKoopaTumble::theNerve());
			}
			break;
		}
		return FALSE;
	}

	switch (side) {
	case -1:
		spine->pushNerve(&TNerveKoopaTurnL::theNerve());
		break;
	case 1:
		spine->pushNerve(&TNerveKoopaTurnR::theNerve());
		break;
	default:
	case 0: {
		koopa->mTurnsLeft = KoopaIsMarioLeft(koopa);
		spine->setNext(&TNerveKoopaFlame::theNerve());
		break;
	}
	}

	return FALSE;
}

BOOL TNerveKoopaTumble::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	koopa->changeAnm(KOOPA_ANM_HIPDROP, 0,
	                 koopa->getParam()->tumbleSpeed.get());
	koopa->getMActor()->getFrameCtrl(ANM_TYPE_BCK);

	if (spine->getTime() == 190) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_KOOPA_HIPDROP, 1.0f);

		static TBathtub* bathtub
		    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
		gpMarioParticleManager->emitAndBindToMtx(
		    KOOPA_JPA_MS_KP_HIPDROP, *bathtub->getRootJointMtx(), 0, this);
		if (SMS_IsMarioTouchGround4cm())
			SMSRumbleMgr->start(1, (f32*)nullptr);
	}

	if (koopa->getAnmEnd())
		return TRUE;
	return FALSE;
}

BOOL TNerveKoopaFall::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = KoopaGetBody(spine);

	koopa->changeAnm(KOOPA_ANM_FALL, 0, koopa->getParam()->fallSpeed.get());
	return FALSE;
}

// TODO: 98.5%. Frame 0x148 short (retail 0x368), a few register swaps and a
// handful of missing/extra instructions remain.
BOOL TNerveKoopaFlame::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	switch (koopa->getAnmIndex()) {
	case KOOPA_ANM_FIRE_START:
		if (koopa->getAnmEnd()) {
			koopa->changeAnm(KOOPA_ANM_FIRE_LOOP, 0, 2.0f);
			spine->setNext(&TNerveKoopaFlame::theNerve());
		} else {
			koopa->mLaughPending = false;
		}
		break;

	case KOOPA_ANM_FIRE_END:
		if (koopa->getAnmEnd()) {
			koopa->laugh();

			if (koopa->mWaitTimer > 0) {
				spine->setNext(&TNerveKoopaWait::theNerve());
				return FALSE;
			}

			TBathtub* bathtub
			    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
			u8 onGrip = KoopaFindGrip(koopa, bathtub);

			f32 diff
			    = KOOPA_WRAP_DEGREES(koopa->mTargetDir - koopa->mRotation.y);
			f32 focusRange = koopa->getParam()->focusRange.get();
			int side;
			if (diff < -focusRange)
				side = -1;
			else if (diff > focusRange)
				side = 1;
			else
				side = 0;

			if (onGrip) {
				spine->setNext(&TNerveKoopaWait::theNerve());
			} else {
				switch (side) {
				case -1:
					spine->pushNerve(&TNerveKoopaTurnL::theNerve());
					break;
				case 1:
					spine->pushNerve(&TNerveKoopaTurnR::theNerve());
					break;
				case 0: {
					koopa->mTurnsLeft = KoopaIsMarioLeft(koopa);
					koopa->changeAnm(KOOPA_ANM_FIRE_START, 0,
					                 koopa->getParam()->fireSpeed.get());
					spine->setNext(&TNerveKoopaFlame::theNerve());
					break;
				}
				}
			}
		}
		break;

	case KOOPA_ANM_FIRE_LOOP: {
		int time = spine->getTime();
		if (time >= koopa->getParam()->flameFocusEndStep.get()) {
			if (!(time & 7)) {
				TBathtub* bathtub
				    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
				u8 onGrip = KoopaFindGrip(koopa, bathtub);

				TKoopaParams* params = koopa->getParam();
				f32 diff = KOOPA_WRAP_DEGREES(koopa->mTargetDir
				                              - koopa->mRotation.y);
				f32 focusRange = params->focusRange.get();
				int side;
				if (diff < -focusRange)
					side = -1;
				else if (diff > focusRange)
					side = 1;
				else
					side = 0;

				if (onGrip || side != 0)
					koopa->changeAnm(KOOPA_ANM_FIRE_END, 0,
					                 params->fireSpeed.get());
			} else if (koopa->getAnmEnd()) {
				TKoopaParams* params = koopa->getParam();
				if (spine->getTime() >= params->flameFocusEndStep.get())
					koopa->changeAnm(KOOPA_ANM_FIRE_END, 0,
					                 params->fireSpeed.get());
			}
		}
		break;
	}

	default:
		koopa->changeAnm(KOOPA_ANM_FIRE_START, 0,
		                 koopa->getParam()->fireSpeed.get());
		break;
	}

	return FALSE;
}

BOOL TNerveKoopaProvoke::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	koopa->changeAnm(KOOPA_ANM_FIRST, 0, 2.0f);
	if (koopa->getAnmEnd()) {
		spine->setNext(&TNerveKoopaWait::theNerve());
		return FALSE;
	}

	return FALSE;
}

BOOL TNerveKoopaStagger::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = KoopaGetBody(spine);

	koopa->changeAnm(KOOPA_ANM_STAGGER, 0,
	                 koopa->getParam()->staggerSpeed.get());
	if (koopa->getAnmEnd())
		return TRUE;
	return FALSE;
}

BOOL TNerveKoopaGetShowered::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = KoopaGetBody(spine);

	koopa->changeAnm(KOOPA_ANM_WATERHIT, 0,
	                 koopa->getParam()->waterhitSpeed.get());
	if (koopa->getAnmEnd())
		return TRUE;
	return FALSE;
}

static inline TKoopaParams* KoopaGetDownParam(TKoopa* koopa)
{
	TKoopaParams* param = koopa->getParam();
	return param;
}

BOOL TNerveKoopaGetDown::execute(TSpineBase<TLiveActor>* spine) const
{
	TKoopa* koopa = (TKoopa*)spine->getBody();

	switch (koopa->getAnmIndex()) {
	case KOOPA_ANM_DOWN:
		if (koopa->getAnmEnd())
			koopa->changeAnm(KOOPA_ANM_DOWN_WAIT, 0,
			                 KoopaGetDownParam(koopa)->downSpeed.get());
		break;

	case KOOPA_ANM_DOWN_WAIT: {
		TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
		int time          = spine->getTime();
		f32 downStep      = KoopaGetDownParam(koopa)->downStep.get();
		if (!((f32)(time * (bathtub->getNumGripsDead() + 2)) < downStep)
		    && koopa->getAnmEnd())
			koopa->changeAnm(KOOPA_ANM_GETUP, 0,
			                 KoopaGetDownParam(koopa)->downSpeed.get());
		break;
	}

	case KOOPA_ANM_GETUP:
		if (koopa->getAnmEnd())
			return TRUE;
		break;

	default: {
		koopa->changeAnm(KOOPA_ANM_DOWN, 0,
		                 koopa->getParam()->downSpeed.get());
		static TBathtub* bathtub
		    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
		gpMarioParticleManager->emitAndBindToMtx(
		    0xF5, *bathtub->getRootJointMtx(), 0, this);
		break;
	}
	}

	return FALSE;
}

// ---------------------------------------------------------------------------
// TKoopaParts and its four hit boxes
// ---------------------------------------------------------------------------

TKoopaParts::TKoopaParts(const char* name, u32 actor_type, TKoopa* owner,
                         f32 radius)
    : THitActor(name)
    , mOwner(owner)
{
	JDrama::TNameRef* group = JDrama::TNameRefGen::search2("敵グループ");
	((TIdxGroupObj*)group)
	    ->getChildren()
	    .push_back(this);
	initHitActor(actor_type, 5, 0x88000000, radius, radius, radius, radius);
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_UNK10000000);
	onHitFlag(HIT_FLAG_UNK8000000);
}

void TKoopaParts::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);

	if (cue & CUE_MOVE) {
		control();
		for (int i = 0; i < mColCount; i++)
			attack_(mCollisions[i]);
	}
}

// UNUSED (0x84). Inlined into TKoopa::setUpHitActors for the head and the
// body boxes.
void TKoopaParts::set(const JGeometry::TVec3<f32>& position, f32 radius,
                      f32 height)
{
	mPosition.set(position);
	offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	offHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mAttackRadius = radius;
	mAttackHeight = height;
	mDamageRadius = radius;
	mDamageHeight = height;
	calcEntryRadius();
}

// UNUSED (0x28) -- exactly the three flag sets and the return.
void TKoopaParts::remove()
{
	onHitFlag(HIT_FLAG_CANNOT_ATTACK);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	onHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED (0x18c). loadAfter allocates the ten flames through the base
// constructor and then writes the vtable and the two reset floats itself, so
// this body is what it inlined.
TKoopaFlame::TKoopaFlame(TKoopa* owner)
    : TKoopaParts("クッパの吐く炎", 0x08000029, owner, 100.0f)
{
	resetFlame();
}

// UNUSED (0x18).
bool TKoopaFlame::isAlive() const { return mLength < mLengthMax; }

// UNUSED (0x8).
f32 TKoopaFlame::getLength() const { return mLength; }

// UNUSED (0x14).
void TKoopaFlame::resetFlame()
{
	mLengthMax = 0.0f;
	mLength    = 1.0f;
}

// UNUSED (0x64). Inlined into TKoopa::breathFlame.
void TKoopaFlame::fire(const JGeometry::TVec3<f32>& position,
                       const JGeometry::TVec3<f32>& direction, f32 speed,
                       f32 length_max, f32 radius, f32 height)
{
	mPosition.set(position);
	mDirection.set(direction);
	mStartPos.set(position);
	mSpeed     = speed;
	mLengthMax = length_max;
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

	f32 length = mLength;
	JGeometry::TVec3<f32> position;
	position.x = mDirection.x * length + mStartPos.x;
	position.y = mDirection.y * length + mStartPos.y;
	position.z = mDirection.z * length + mStartPos.z;
	f32 radius = mRadius;
	f32 height = mHeight;
	if (height <= 0.0f)
		height = 2.0f * radius;

	set(position, radius, height);
}

BOOL TKoopaFlame::receiveMessage(THitActor*, u32 message)
{
	// A switch is what gives the ROM's signed compare; a plain `==` on the
	// u32 parameter emits cmplwi.
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		return FALSE;
	}
	return TRUE;
}

// TODO: 18.3%. Retail `bl`s TKoopa::changeAnm from here while MWCC expands it,
// and this is a *per-site* refusal, not a budget: seven extra zero-codegen
// statements in changeAnm take this function from 18.4% to 99.8% and leave
// changeAnm itself byte-exact, but they also turn changeAnm into a call in
// every other site in the TU and cost eleven functions their match (Wait,
// Tumble, Fall, Flame, Provoke, Stagger, GetShowered, GetDown, init, reset:
// the unit drops 85.1 -> 76.1). So retail inlines the same 8-statement body
// everywhere else and calls it only here -- the emergent per-expansion class
// of docs/catalog/codegen-tells.md "round 38" (SMS_getShineID in
// TSelectMenu::perform), not a statement count. The r30/r31 inversion below
// (retail ranks the `other` parameter above `this`) is a second, independent
// residue.
void TKoopaFlame::attack_(THitActor* other)
{
	if (other->receiveMessage(this, HIT_MESSAGE_UNKA)
	    && other == (THitActor*)gpMarioAddress) {
		f32 jump = mOwner->getParam()->flameJump.get();
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), jump);
		mOwner->mLaughPending = true;
		mOwner->changeAnm(KOOPA_ANM_FIRE_END, 0,
		                  mOwner->getParam()->fireSpeed.get());
		mOwner->mWaitTimer = 240;
	}
}

// UNUSED (0x17c).
TKoopaHand::TKoopaHand(TKoopa* owner)
    : TKoopaParts("クッパの手", 0x0800002B, owner, 100.0f)
{
}

BOOL TKoopaHand::receiveMessage(THitActor*, u32) { return TRUE; }

void TKoopaHand::attack_(THitActor* other)
{
	other->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

// UNUSED (0x17c).
TKoopaHead::TKoopaHead(TKoopa* owner)
    : TKoopaParts("クッパの頭", 0x0800002A, owner, 100.0f)
{
}

BOOL TKoopaHead::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		if (mOwner->getShowered()) {
			gpMarioParticleManager->emit(
			    PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
			    0, nullptr);
			gpMSound->startSoundSet(MSD_SE_EN_COMMON_W_HIT_OK,
			                        (Vec*)&mOwner->mPosition, 0, 0.0f, 0, 0, 4);
		}
		break;
	case HIT_MESSAGE_ATTACK:
		if (sender->getActorType() == 0x08000024)
			mOwner->stagger(false);
		break;
	}
	return TRUE;
}

void TKoopaHead::attack_(THitActor* other)
{
	if (other->receiveMessage(this, HIT_MESSAGE_ATTACK)
	    && other == SMS_GetMarioHitActor())
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
}

// UNUSED (0x17c).
TKoopaBody::TKoopaBody(TKoopa* owner)
    : TKoopaParts("クッパの体", 0x0800002A, owner, 100.0f)
{
}

BOOL TKoopaBody::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_SPRAYED_BY_WATER:
		break;
	case HIT_MESSAGE_ATTACK:
		if (sender->getActorType() == 0x08000024)
			mOwner->stagger(false);
		break;
	}
	return TRUE;
}

void TKoopaBody::attack_(THitActor* other)
{
	if (other->receiveMessage(this, HIT_MESSAGE_ATTACK)
	    && other == SMS_GetMarioHitActor())
		SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 60.0f);
}

// ---------------------------------------------------------------------------
// TKoopa
// ---------------------------------------------------------------------------

// Binding level over a raw member read, worth +8 of low region in
// TKoopa::getFlameDirDegree (batch 127).
static inline bool KoopaTurnsLeft(const TKoopa* p)
{
	bool turnsLeft = p->mTurnsLeft;
	return turnsLeft;
}

f32 TKoopa::getFlameDirDegree() const
{
	f32 offset = getFlameDirRate() * getParam()->flameNeckRange.get();
	f32 signed_;
	if (KoopaTurnsLeft(this))
		signed_ = -offset;
	else
		signed_ = offset;
	return mRotation.y + signed_;
}

namespace {
// The neck joint callback: it aims Bowser's head at Mario, by an amount
// TKoopa::getNeckFocus() scales, and while he is breathing fire it first
// swings the head sideways and down by getFlameDirRate().
//
// TODO: only the head of the function is reconstructed. What is missing, in
// the ROM's order:
//   * inside the isFlaming() guard, two hand-written 3x4 concatenations,
//     `mtx = Ry(yaw) * mtx` and then `mtx = mtx * Rz(pitch)`, both spelled
//     out with literal 0.0f/1.0f factors (which is why those products
//     survive);
//   * the aim itself: the up axis is column 1 of mtx, `focus` is projected
//     onto the plane through it and normalised, and if the dot product with
//     column 0 is below 0.5 the focus factor is scaled by
//     (1 + dot) / 1.5;
//   * a quaternion from the axis/angle between column 0 and that direction,
//     scaled by the focus factor with a refinement-free sqrt, multiplied by
//     a second quaternion around the up axis when not flaming, and finally
//     turned into a rotation matrix concatenated onto mtx;
//   * PSMTXCopy(mtx, J3DSys::mCurrentMtx).
int KoopaNeckCallBack(J3DNode* node, int flag)
{
	if (flag != 0)
		return 1;

	TKoopa* koopa = (TKoopa*)node->getCallBackUserData();
	MtxPtr mtx
	    = j3dSys.getModel()->getAnmMtx(((J3DJoint*)node)->getJntNo());

	// The integer copy is the ROM's: the whole vector is moved with
	// lwz/stw before the first component is touched.
	JGeometry::TVec3<f32> focus = SMS_GetMarioPos();
	focus.y += 85.0f;
	focus.x -= mtx[0][3];
	focus.y -= mtx[1][3];
	focus.z -= mtx[2][3];

	if (koopa->isFlaming()) {
		f32 yaw = (koopa->getFlameDirRate() * 6.2831855f
		           * koopa->getParam()->flameNeckRange.get())
		          / 360.0f;
		f32 pitch = yaw * koopa->getParam()->flameNeckDownRate.get();
		if (pitch > 0.0f)
			pitch = -pitch;
		if (koopa->mTurnsLeft)
			yaw = -yaw;

		f32 sinYaw = sinf(yaw);
		f32 cosYaw = cosf(yaw);
		// TODO: mtx = Ry(yaw) * mtx, written out with the literal factors.
		// TODO: mtx = mtx * Rz(pitch), likewise.
	}

	// TODO: the aim below is an outline only. The ROM normalises `focus` in
	// place, builds the quaternion from the axis/angle between column 0 and
	// `flat`, multiplies in a second quaternion around the up axis when not
	// flaming, and concatenates the resulting rotation onto mtx.
	f32 neckFocus = koopa->getNeckFocus();

	JGeometry::TVec3<f32> up(mtx[0][1], mtx[1][1], mtx[2][1]);
	JGeometry::TVec3<f32> flat;
	flat.scaleAdd(-up.dot(focus), focus, up);
	flat.normalize();
	focus.normalize();

	JGeometry::TVec3<f32> front(mtx[0][0], mtx[1][0], mtx[2][0]);
	// The ROM's threshold is a `double` literal, so the comparison promotes.
	if (front.dot(flat) < 0.5)
		neckFocus *= (1.0f + front.dot(flat)) / 1.5f;

	PSMTXCopy(mtx, J3DSys::mCurrentMtx);
	return 1;
}
} // namespace

// UNUSED (0xd0).
void TKoopa::stopFlame()
{
	for (int i = 0; i < 10; i++) {
		mFlames[i]->resetFlame();
		mFlames[i]->remove();
	}
}

// UNUSED (0x1c0). This is the whole "breathe" arm of setUpHitActors: find a
// spent flame, and if none of the live ones is still close to the mouth, fire
// it along the head joint's forward axis.
void TKoopa::breathFlame()
{
	int spent    = -1;
	bool tooNear = false;
	for (int i = 0; i < 10; i++) {
		TKoopaFlame* flame = mFlames[i];
		if (!flame->isAlive())
			spent = i;
		else if (flame->getLength() < 2.0f * getParam()->flameRadius.get())
			tooNear = true;
	}

	if (!tooNear && spent >= 0) {
		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(mHeadJntIndex);
		JGeometry::TVec3<f32> position(mtx[0][3], mtx[1][3] - 500.0f,
		                               mtx[2][3]);
		JGeometry::TVec3<f32> direction(mtx[0][0], 0.0f, mtx[2][0]);
		direction.normalize();

		TKoopaParams* params = getParam();
		mFlames[spent]->fire(position, direction, params->flameVelocity.get(),
		                     4000.0f, params->flameRadius.get(),
		                     params->flameHeight.get());
	}
}

// UNUSED (0x9c) -- the "not breathing" arm of setUpHitActors.
void TKoopa::resetFlame_()
{
	for (int i = 0; i < 10; i++)
		mFlames[i]->resetFlame();
}

void TKoopa::setUpHitActors()
{
	// breathFlame() and resetFlame_() are both UNUSED and both spelled out
	// here: behind a call MWCC refuses to expand a 0x1c0-byte body, while the
	// ROM has them inline (same pasted-UNUSED pattern as TFlyEnemy::flyMove
	// and TAnimalBird::doWalk).
	if (isBreathing()) {
		int spent    = -1;
		bool tooNear = false;
		for (int i = 0; i < 10; i++) {
			TKoopaFlame* flame = mFlames[i];
			if (!flame->isAlive())
				spent = i;
			else if (flame->getLength()
			         < 2.0f * getParam()->flameRadius.get())
				tooNear = true;
		}

		if (!tooNear && spent >= 0) {
			MtxPtr mtx = getMActor()->getModel()->getAnmMtx(mHeadJntIndex);
			JGeometry::TVec3<f32> position(mtx[0][3], mtx[1][3] - 500.0f,
			                               mtx[2][3]);
			JGeometry::TVec3<f32> direction(mtx[0][0], 0.0f, mtx[2][0]);
			direction.normalize();

			TKoopaParams* params = getParam();
			mFlames[spent]->fire(position, direction,
			                     params->flameVelocity.get(), 4000.0f,
			                     params->flameRadius.get(),
			                     params->flameHeight.get());
		}
	} else {
		for (int i = 0; i < 10; i++)
			mFlames[i]->resetFlame();
	}

	MtxPtr agoMtx = getMActor()->getModel()->getAnmMtx(mAgoJntIndex);
	f32 headRadius = getParam()->headRadius.get();
	mHead->set(JGeometry::TVec3<f32>(agoMtx[0][3], agoMtx[1][3] - 200.0f,
	                                 agoMtx[2][3]),
	           headRadius, 2.0f * headRadius);
	mBody->set(mPosition, 800.0f, 2000.0f);
}

void TKoopa::changeAnm(int bck_index, int btp_index, f32 rate)
{
	if (!getMActor()->checkCurBckFromIndex(bck_index)) {
		getMActor()->setBckFromIndex(bck_index);
		const char** table = getBasNameTable();
		setAnmSound(table == nullptr ? nullptr : table[bck_index]);
	}
	if (btp_index != getMActor()->getCurAnmIdx(ANM_TYPE_BTP))
		getMActor()->setBtpFromIndex(btp_index);
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	ctrl->setRate(rate * SMSGetAnmFrameRate() * 0.5f);
}

// UNUSED (0x28).
int TKoopa::getAnmIndex() const
{
	return getMActor()->getCurAnmIdx(ANM_TYPE_BCK);
}

// TODO: UNUSED (0x3c), body not reconstructed.
BOOL TKoopa::endsAnm() const { return FALSE; }

// UNUSED (0x68). Inlined at the top of TNerveKoopaFlame's fire-end case.
void TKoopa::laugh()
{
	if (mLaughPending) {
		gpMSound->startSoundActor(MSD_SE_BS_KOOPA_VO_LAUGH,
		                          (Vec*)&mAnmSoundPos, 0, nullptr, 0, 4);
		mLaughPending = false;
	}
}

// UNUSED (0x84). Flames come out for the whole of the loop animation and for
// the tail of the start animation. setUpHitActors and perform both expand it.
bool TKoopa::isBreathing() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRE_LOOP)
		return true;
	if (getAnmIndex() == KOOPA_ANM_FIRE_START && getAnmFrame() >= 85.0f)
		return true;
	return false;
}

// TODO: UNUSED (0x20). setIgnoreMario's two instructions and this accessor's
// eight are all the evidence for the flag's offset.
BOOL TKoopa::ignoresMario() const
{
	if (mIgnoreMario)
		return TRUE;
	return FALSE;
}

// UNUSED (0x8) -- one store and a return.
void TKoopa::setIgnoreMario(long ignore) { mIgnoreMario = ignore; }

f32 TKoopa::getFlameDirRate() const
{
	f32 frame = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
	f32 end   = (f32)getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd();
	TKoopaParams* params = getParam();
	f32 overStart        = params->flameOverStart.get();
	int startStep        = params->flameFocusStartStep.get();
	int endStep          = params->flameFocusEndStep.get();
	int time             = mSpine->getTime();
	int index            = getMActor()->getCurAnmIdx(ANM_TYPE_BCK);

	switch (index) {
	case KOOPA_ANM_FIRE_START:
		return -((frame * overStart) / end);
	case KOOPA_ANM_FIRE_END:
	case KOOPA_ANM_FIRE_LOOP: {
		f32 rate;
		if (mSpine->getTime() <= startStep)
			rate = -overStart;
		else if (time <= endStep)
			rate = ((1.0f + overStart) * (f32)(time - startStep))
			           / (f32)(endStep - startStep)
			       - overStart;
		else
			rate = 1.0f;

		if (index == KOOPA_ANM_FIRE_END)
			rate *= 1.0f - frame / end;
		return rate;
	}
	}

	return 0.0f;
}

bool TKoopa::isFlaming() const
{
	// Written as a switch: the `||` chain folds into the subi/cmplwi range
	// test, while the ROM keeps the switch's two-compare tree.
	switch (getMActor()->getCurAnmIdx(ANM_TYPE_BCK)) {
	case KOOPA_ANM_FIRE_END:
	case KOOPA_ANM_FIRE_LOOP:
	case KOOPA_ANM_FIRE_START:
		return true;
	}
	return false;
}

// UNUSED (0x78). The stretch of the intro roar (koopa_first, the animation
// TNerveKoopaProvoke plays) during which the mouth is open; perform() spits
// flames for it just as it does while breathing.
bool TKoopa::isProvoking() const
{
	if (getAnmIndex() == KOOPA_ANM_FIRST) {
		f32 frame = getAnmFrame();
		if (68.0f <= frame) {
			if (frame <= 164.0f)
				return true;
		}
	}
	return false;
}

// Binding level worth +8 of low region, landing TKoopa::getNeckFocus's frame
// at 0x30 (batch 121).
static inline MActor* KoopaGetMActor(const TKoopa* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

// How far the head has turned towards Mario over the course of the current
// animation, in [0, 1]: 1 at rest, 0 while the neck is busy.
f32 TKoopa::getNeckFocus() const
{
	int index          = KoopaGetMActor(this)->getCurAnmIdx(ANM_TYPE_BCK);
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	f32 end            = (f32)ctrl->getEnd();
	f32 frame          = ctrl->getFrame();
	f32 focus          = 1.0f;

	switch (index) {
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
	case KOOPA_ANM_DOWN_WAIT:
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
		// A chain of `if (...) break;` tests: the two that leave the head
		// fully free have no body at all, which is what gives the ROM's
		// unfused `bne +8; b end` there.
		if (frame <= 200.0f) {
			focus = 1.0f;
			break;
		}
		if (frame <= 255.0f) {
			focus = 1.0f - (frame - 200.0f) / 55.0f;
			break;
		}
		if (frame <= 330.0f) {
			focus = 0.0f;
			break;
		}
		if (frame <= 390.0f) {
			focus = (frame - 330.0f) / 60.0f;
			break;
		}
		if (frame <= 440.0f) {
			focus = 1.0f;
			break;
		}
		if (frame <= 480.0f) {
			focus = 1.0f - (frame - 440.0f) / 40.0f;
			break;
		}
		if (frame <= 555.0f) {
			focus = 0.0f;
			break;
		}
		if (frame <= 615.0f)
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

// TODO: retail keeps the .bss base in r31 and `this` in r30 (ours swapped);
// the nerve guard objects' .bss offsets differ too (TU static order).
void TKoopa::getDown()
{
	if (&TNerveKoopaFall::theNerve() == getSpine()->getCurrentNerve())
		return;
	if (&TNerveKoopaProvoke::theNerve() == getSpine()->getCurrentNerve())
		return;
	if (&TNerveKoopaTumble::theNerve() == getSpine()->getCurrentNerve())
		return;

	if (&TNerveKoopaStagger::theNerve() == getSpine()->getCurrentNerve())
		mSpine->setNext(&TNerveKoopaGetDown::theNerve());
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		mSpine->setNext(&TNerveKoopaGetDown::theNerve());
	getSpine()->pushNerve(&TNerveKoopaGetDown::theNerve());
}

bool TKoopa::effectsTumble() const
{
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve()) {
		int time = mSpine->getTime();
		if (time < 900 && time > 190)
			return true;
	}
	return false;
}

// TODO: retail keeps `this` in r31 and the .bss base in r30 (ours swapped),
// and its nerve guard objects sit at other .bss offsets (TU static order).
bool TKoopa::getShowered()
{
	if (&TNerveKoopaFall::theNerve() == getSpine()->getCurrentNerve())
		return false;
	if (&TNerveKoopaProvoke::theNerve() == getSpine()->getCurrentNerve())
		return false;
	if (&TNerveKoopaTumble::theNerve() == getSpine()->getCurrentNerve())
		return false;
	if (&TNerveKoopaGetDown::theNerve() == getSpine()->getCurrentNerve())
		return false;
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		return true;
	if (&TNerveKoopaStagger::theNerve() == mSpine->getCurrentNerve()) {
		getSpine()->setNext(&TNerveKoopaGetShowered::theNerve());
		return true;
	}
	if (&TNerveKoopaFlame::theNerve() == mSpine->getCurrentNerve()) {
		getSpine()->setNext(&TNerveKoopaWait::theNerve());
		return false;
	}
	getSpine()->pushNerve(&TNerveKoopaGetShowered::theNerve());
	return true;
}

// TODO: only the nerve guard objects' .bss offsets differ (TU static order).
void TKoopa::stagger(bool force)
{
	if (&TNerveKoopaFall::theNerve() == getSpine()->getCurrentNerve())
		return;
	if (&TNerveKoopaProvoke::theNerve() == getSpine()->getCurrentNerve())
		return;
	if (!force
	    && getSpine()->getCurrentNerve() == &TNerveKoopaFlame::theNerve())
		return;
	if (&TNerveKoopaTumble::theNerve() == getSpine()->getCurrentNerve())
		return;
	if (&TNerveKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return;
	if (&TNerveKoopaGetShowered::theNerve() == mSpine->getCurrentNerve())
		return;
	getSpine()->pushNerve(&TNerveKoopaStagger::theNerve());
}

// TODO: UNUSED (0xec), body not reconstructed. Two inlined theNerve() guards
// would be about this size.
BOOL TKoopa::isTumbling() const
{
	if (&TNerveKoopaTumble::theNerve() == mSpine->getCurrentNerve())
		return TRUE;
	if (&TNerveKoopaGetDown::theNerve() == mSpine->getCurrentNerve())
		return TRUE;
	return FALSE;
}

// TODO: frame 0x28 short in the low (inline temporary) region, and 0x3c
// short where TKoopa::init inlines this; every instruction else matches.
f32 TKoopa::getTargetDir(const JGeometry::TVec3<f32>& target) const
{
	TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
	MtxPtr mtx        = *bathtub->getRootJointMtx();
	JGeometry::TVec3<f32> origin(mtx[0][3], mtx[1][3], mtx[2][3]);
	JGeometry::TVec3<f32> toTarget;
	JGeometry::TVec3<f32> xDir(mtx[0][0], mtx[1][0], mtx[2][0]);
	JGeometry::TVec3<f32> zDir(mtx[0][2], mtx[1][2], mtx[2][2]);
	toTarget.sub(target, origin);
	f32 z = zDir.dot(toTarget);
	f32 x = xDir.dot(toTarget);
	f32 angle = matan(z, x);
	return (360.0f / 65536.0f) * angle;
}

// UNUSED (0x2c).
f32 TKoopa::getAnmFrame() const
{
	return getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
}

// UNUSED (0x3c).
f32 TKoopa::getAnmFrameNext() const
{
	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	return ctrl->getFrame() + ctrl->getRate();
}

// UNUSED (0x90). True on the frame the animation steps past `frame`.
bool TKoopa::passesAnmFrame(f32 frame) const
{
	bool passes = false;
	if (getAnmFrame() <= frame && frame <= 0.005f + getAnmFrameNext())
		passes = true;
	return passes;
}

// UNUSED (0x1a8). Bowser only agrees to hip-drop on the wait animation, and
// only as it crosses one of its three rest points.
bool TKoopa::canTumble() const
{
	if (getAnmIndex() != KOOPA_ANM_WAIT)
		return false;
	if (getAnmEnd())
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
	if (getMActor()->getCurAnmIdx(ANM_TYPE_BCK) == KOOPA_ANM_HIPDROP) {
		mAnmSoundPos.x = mPosition.x;
		mAnmSoundPos.y = mPosition.y;
		mAnmSoundPos.z = mPosition.z;
	} else {
		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(mHeadJntIndex);
		mAnmSoundPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}

	if (mAnmSound && mAnmSoundPath) {
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		mAnmSound->animeLoop((Vec*)&mAnmSoundPos, ctrl->getFrame(),
		                     ctrl->getRate(), 0, 4);
	}
}

// UNUSED (0x48).
BOOL TKoopa::getAnmEnd() const
{
	return getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr);
}

TKoopa::TKoopa(const char* name)
    : TSpineEnemy(name)
{
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_UNK100);
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TKoopa::load(JSUMemoryInputStream& stream) { TSpineEnemy::load(stream); }

void TKoopa::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	for (int i = 0; i < 10; i++)
		mFlames[i] = new TKoopaFlame(this);

	for (int i = 0; i < 2; i++)
		mHands[i] = new TKoopaHand(this);

	mHead = new TKoopaHead(this);
	mBody = new TKoopaBody(this);
}

// TODO: 95.8%. Frame 0x38 short (retail 0x150) in the inlined getTargetDir
// temporaries; a few loads are scheduled differently.
void TKoopa::init(TLiveManager* manager)
{
	mBodyRadius = 800.0f;
	mHeadHeight = 2000.0f;
	TSpineEnemy::init(manager);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);
	offHitFlag(HIT_FLAG_CANNOT_ATTACK);
	mSpine->initWith(&TNerveKoopaProvoke::theNerve());

	changeAnm(KOOPA_ANM_WAIT, 1, 2.0f);
	changeAnm(KOOPA_ANM_FIRST, 0, 2.0f);

	MActorAnmBck* bck = getMActor()->getAnmBck();
	if (bck)
		bck->initSimpleMotionBlend(0x10);

	mTargetDir = getTargetDir(SMS_GetMarioPos());

	initAnmSound();
	reset();

	JUTNameTab* joints = getModel()->getModelData()->getJointName();
	// TODO: the ROM walks every joint name here and does nothing with them.
	for (u16 i = 0; i < joints->getResNameTable()->mEntryNum; i++) { }

	mAgoJntIndex  = joints->getIndex("ago");
	mHeadJntIndex = joints->getIndex("head");
	mNeckJntIndex = joints->getIndex("neck");

	J3DJoint* head
	    = getModel()->getModelData()->getJointNodePointer(mHeadJntIndex);
	head->setCallBack(&KoopaNeckCallBack);
	head->setCallBackUserData(this);

	mNeckFocus    = 1.0f;
	mLaughPending = false;
}

const char** TKoopa::getBasNameTable() const { return koopa_bastable; }

// UNUSED (0x1c).
MtxPtr TKoopa::getHeadMtx() const
{
	return getMActor()->getModel()->getAnmMtx(mHeadJntIndex);
}

static inline TKoopaParams* KoopaGetParam(const TKoopa* koopa)
{
	TKoopaParams* param = koopa->getParam();
	return param;
}

void TKoopa::reset()
{
	TSpineEnemy::reset();
	changeAnm(KOOPA_ANM_WAIT, 1, KoopaGetParam(this)->waitSpeed.get());
	mWaitTimer = 600;
}

void TKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		mNeckFocus = getNeckFocus();
		if (mWaitTimer > 0)
			mWaitTimer--;
	}

	TSpineEnemy::perform(cue, graphics);

	for (int i = 0; i < 10; i++)
		mFlames[i]->perform(cue, graphics);
	mHead->perform(cue, graphics);
	mHands[0]->perform(cue, graphics);
	mHands[1]->perform(cue, graphics);
	mBody->perform(cue, graphics);

	if (cue & CUE_MOVE) {
		f32 frame = getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame();
		bool inTumbleWindow = false;
		bool tumbles        = false;
		if (mSpine->getCurrentNerve() == &TNerveKoopaTumble::theNerve()
		    && frame >= getParam()->tumbleStartFrame.get())
			inTumbleWindow = true;
		if (inTumbleWindow && frame <= getParam()->tumbleEndFrame.get())
			tumbles = true;
		if (tumbles) {
			TBathtub* bathtub
			    = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
			bathtub->tumble(mRotation.y, getParam()->tumbleWeight.get());
		}
		setUpHitActors();
	}

	if (cue & CUE_CALC_ANIM) {
		if (isBreathing() || isProvoking()) {
			getMActor()->calc();

			f32 scale = getParam()->flameScale.get();
			JGeometry::TVec3<f32> flameScale(scale, scale, scale);

			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        KOOPA_JPA_MS_KP_FIRE_E, getHeadMtx(), 3, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_D, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_C, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_B, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);

			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    KOOPA_JPA_MS_KP_FIRE_A, getHeadMtx(), 1, this);
			if (emitter)
				emitter->setGlobalScale(flameScale);
		}
	}
}

BOOL TKoopa::receiveMessage(THitActor* sender, u32 message)
{
	return TSpineEnemy::receiveMessage(sender, message);
}

// Bowser rides the bathtub: his world matrix is the tub's matrix times his own
// Y rotation, with the origin pushed 1500 units down the tub's up axis.
//
// JGeometry::TMatrix34<T>::concat(a, b) now carries exactly these twelve terms,
// but it takes both operands as const T&, and here the left one is the
// bathtub's joint matrix, an MtxPtr with no JGeometry wrapper to bind to. Hence
// the terms stay written out; nothing but a cast would let the call be used.
void TKoopa::calcRootMatrix()
{
	TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search2("バスタブ");
	MtxPtr tub        = *bathtub->getRootJointMtx();
	f32 downX         = -1500.0f * tub[0][1];
	f32 downY         = -1500.0f * tub[1][1];
	f32 downZ         = -1500.0f * tub[2][1];

	JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > mtx;
	MsMtxSetRotRPH(mtx, 0.0f, mRotation.y, 0.0f);
	mtx.ref(0, 3) = 0.0f;
	mtx.ref(1, 3) = 0.0f;
	mtx.ref(2, 3) = 0.0f;

	mtx.set(tub[0][0] * mtx.at(0, 0) + tub[0][1] * mtx.at(1, 0)
	            + tub[0][2] * mtx.at(2, 0),
	        tub[0][0] * mtx.at(0, 1) + tub[0][1] * mtx.at(1, 1)
	            + tub[0][2] * mtx.at(2, 1),
	        tub[0][0] * mtx.at(0, 2) + tub[0][1] * mtx.at(1, 2)
	            + tub[0][2] * mtx.at(2, 2),
	        tub[0][3]
	            + (tub[0][0] * mtx.at(0, 3) + tub[0][1] * mtx.at(1, 3)
	               + tub[0][2] * mtx.at(2, 3)),

	        tub[1][0] * mtx.at(0, 0) + tub[1][1] * mtx.at(1, 0)
	            + tub[1][2] * mtx.at(2, 0),
	        tub[1][0] * mtx.at(0, 1) + tub[1][1] * mtx.at(1, 1)
	            + tub[1][2] * mtx.at(2, 1),
	        tub[1][0] * mtx.at(0, 2) + tub[1][1] * mtx.at(1, 2)
	            + tub[1][2] * mtx.at(2, 2),
	        tub[1][3]
	            + (tub[1][0] * mtx.at(0, 3) + tub[1][1] * mtx.at(1, 3)
	               + tub[1][2] * mtx.at(2, 3)),

	        tub[2][0] * mtx.at(0, 0) + tub[2][1] * mtx.at(1, 0)
	            + tub[2][2] * mtx.at(2, 0),
	        tub[2][0] * mtx.at(0, 1) + tub[2][1] * mtx.at(1, 1)
	            + tub[2][2] * mtx.at(2, 1),
	        tub[2][0] * mtx.at(0, 2) + tub[2][1] * mtx.at(1, 2)
	            + tub[2][2] * mtx.at(2, 2),
	        tub[2][3]
	            + (tub[2][0] * mtx.at(0, 3) + tub[2][1] * mtx.at(1, 3)
	               + tub[2][2] * mtx.at(2, 3)));

	mPosition.x = mtx.at(0, 3);
	mPosition.y = mtx.at(1, 3);
	mPosition.z = mtx.at(2, 3);
	mPosition.x += downX;
	mPosition.y += downY;
	mPosition.z += downZ;
	mtx.ref(0, 3) = mPosition.x;
	mtx.ref(1, 3) = mPosition.y;
	mtx.ref(2, 3) = mPosition.z;

	MTXCopy(mtx, getModel()->getBaseTRMtx());

	JGeometry::TVec3<f32> scale(1.0f, 1.0f, 1.0f);
	getModel()->setBaseScale(scale);
	mScaling.x = 1.0f;
	mScaling.y = 1.0f;
	mScaling.z = 1.0f;
}

// UNUSED (0x1a0). Says which side of Bowser Mario is on. The body is gone,
// but its .sdata2 claims survive: between TKoopaManager::load's pool and
// calcRootMatrix's -1500 retail holds 360/65536, 360, -180, 0.0f and the
// int-to-float double, i.e. getTargetDir expanded here, a degree wrap and a
// comparison with zero.
// TODO: ours is 0x1b0 against the map's 0x1a0 and claims -180 before 360.
int TKoopa::checkMarioWhichSide()
{
	f32 toMario = getTargetDir(SMS_GetMarioPos());
	f32 diff    = KOOPA_WRAP_DEGREES(toMario - mRotation.y);
	if (diff < 0.0f)
		return -1;
	if (diff > 0.0f)
		return 1;
	return 0;
}

// UNUSED (0xc).
TKoopaParams* TKoopa::getParam() const
{
	return (TKoopaParams*)((TEnemyManager*)mManager)->getSaveParam();
}

// ---------------------------------------------------------------------------
// TKoopaManager
// ---------------------------------------------------------------------------

TKoopaManager::TKoopaManager(const char* name)
    : TEnemyManager(name)
{
}

// The bathtub Bowser is placed by the scene, never spawned.
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
	                 KOOPA_JPA_MS_KP_FIRE_A);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_b.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_B);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_c.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_C);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_d.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_D);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_hipdrop.jpa",
	                 KOOPA_JPA_MS_KP_HIPDROP);
	SMS_LoadParticle("/scene/koopa/jpa/ms_kp_fire_e.jpa",
	                 KOOPA_JPA_MS_KP_FIRE_E);
}
