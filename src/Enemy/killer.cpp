#include <Enemy/Killer.hpp>
#include <Enemy/KillerNerve.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Camera/CameraShake.hpp>
#include <M3DUtil/MActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DVertex.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <math.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static int KillerBodyCallback(J3DNode*, int);

// fabricated: the ROM calls MsSin, MsCos *and* TVec3::set<f32> out of line
// from the chase nerve's forward step. MsSin/MsCos only stop expanding at
// inline depth 5 and set<f32> at depth 4, so the direction vector is built
// four expansions below the nerve body. TFlyEnemy::flyMove is UNUSED (0x2bc)
// and its body is spelled out in the nerve, so at least one of those four
// levels is flyMove itself; the other three have no symbol anywhere in the
// map, which is why they are TU-local.
//
// Measured ladder on TNerveFlyEnemyChaseFly::execute (levels -> score / the
// deepest callee that still expands): 1 -> 95.00 / JMASSin(short) inlined,
// 2 -> 96.44 / bl JMASin(float), 3 -> 98.16 / bl set<f32>,
// 4 -> 98.94 / bl MsSin, bl MsCos, bl set<f32> -- retail's exact call set.
// TODO: replace the three thin wrappers with the real enclosing functions
// once they are identified; the remaining residue is 48 bytes of frame and
// one `fmr` in the turn clamp.
static inline void MsGetVecFromRotY(JGeometry::TVec3<f32>& dst, f32 rot_y,
                                    f32 length)
{
	dst.set(length * MsSin(rot_y), 0.0f, length * MsCos(rot_y));
}

static inline void MsGetVecFromRotY_L3(JGeometry::TVec3<f32>& dst, f32 rot_y,
                                       f32 length)
{
	MsGetVecFromRotY(dst, rot_y, length);
}

static inline void MsGetVecFromRotY_L2(JGeometry::TVec3<f32>& dst, f32 rot_y,
                                       f32 length)
{
	MsGetVecFromRotY_L3(dst, rot_y, length);
}

static inline void MsGetVecFromRotY_L1(JGeometry::TVec3<f32>& dst, f32 rot_y,
                                       f32 length)
{
	MsGetVecFromRotY_L2(dst, rot_y, length);
}

// fabricated: consume getPosition() inside a by-value fork so the reference
// address does not escape into a callee-saved GPR. Used on X because retail
// already loads that component first.
static inline f32 KillerPosX(const TKiller* k) { return k->getPosition().x; }

static inline f32 FlyPosX(const TFlyEnemy* e) { return e->getPosition().x; }
static inline f32 FlyMarioX() { return SMS_GetMarioPos().x; }

const char* killer_bastable[] = {
	"/scene/killer/bas/downkiller_down1.bas", nullptr, nullptr,
	"/scene/killer/bas/killer_search1.bas",   nullptr,
};

// The joint callback and TKiller::reset/calcRootMatrix hand the currently
// drawn killer to the callback, which has no other way of reaching it.
static TKiller* gpCurKiller;

bool TKiller::mSerialBomb = true;
// TODO: dead-stripped; .sdata placement only tells us the initialiser is
// non-zero. Joint 1 is the one init() attaches KillerBodyCallback to.
u8 TKiller::mSmokeJntNo = 1;
bool TKiller::mTrampleDie = true;
bool TKiller::mRollSw;

f32 TFlyEnemy::mTestSp          = 2.5f;
int TFlyEnemy::mInvalidTime     = 200;
f32 TFlyEnemy::mTestMarioSpMax  = 12.0f;

TFlyEnemyParams::TFlyEnemyParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLNormalFlyGravityY, 0.2f)
    , PARAM_INIT(mSLNormalFlySpeed, 10.0f)
    , PARAM_INIT(mSLChaseFlyGravityY, 0.1f)
    , PARAM_INIT(mSLChaseDist, 2000.0f)
    , PARAM_INIT(mSLForceGravityY, 0.1f)
{
	TParams::load(mPrmPath);
}

void TFlyEnemy::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mFlyParams = (TFlyEnemyParams*)getSaveParam();
}

f32 TFlyEnemy::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
		return mGravityY;
	f32 gravity = mFlyParams->getSLNormalFlyGravityY();
	return gravity;
}

void TFlyEnemy::reset()
{
	TWalkerEnemy::reset();
	mFlyTime   = 0;
	mFlyState  = FLY_STATE_UNK1;
	unk1A4     = false;
	mGravityY  = mFlyParams->mSLNormalFlyGravityY.get();
	unk1A5     = false;
}

// TODO: 90.2%. Instruction-identical apart from the final
// JGeometry::TVec3<f32>::sub, which the ROM calls out of line and our build
// expands -- the per-call-site TVec3 inlining divergence recorded in
// docs/catalog/codegen-tells.md. The 40-byte frame gap follows from it.
void TFlyEnemy::fly()
{
	JGeometry::TVec3<f32> nextPos = getPosition();
	nextPos.add(mLinearVelocity);

	JGeometry::TVec3<f32> drift = mVelocity;
	drift.x += SMS_GetMarioSpeedX() / TFlyEnemy::mTestMarioSpMax;
	drift.z += SMS_GetMarioSpeedZ() / TFlyEnemy::mTestMarioSpMax;
	nextPos.add(drift);
	nextPos.y += mGravityY;

	f32 nextY     = nextPos.y;
	mGroundHeight = gpMap->checkGround(nextPos.x, nextY + mHeadHeight,
	                                   nextPos.z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (nextY <= mGroundHeight) {
		if (mFlyTime > TFlyEnemy::mInvalidTime) {
			offLiveFlag(LIVE_FLAG_AIRBORNE);
			mVelocity.set(0.0f, 0.0f, 0.0f);
			nextPos.y = mGroundHeight;
		}

		const TLiveActor* rider = getGroundPlane()->getActor();
		if (rider) {
			if (rider->isActorType(0x4000000A))
				((TLiveActor*)rider)->kill();
		}

		if (getGroundPlane()->isIllegalData())
			kill();
	} else {
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	// `a = b - c` reaches the map's out-of-line TVec3::sub: operator= is one
	// inline level and the difference nested in its argument two more.
	mLinearVelocity = nextPos - mPosition;
}

// TODO: 99.9%. FlyMarioX + FlyPosX (by-value getPosition forks) land the
// 0x90 frame and every stack slot (99.5 -> 99.9). Residue is the params
// pointer after MsVECNormalize in r3 against retail's r4.
// Tried: params named or not, getSLNormalFlySpeed()/.get()/.value (and the
// same for the force gravity), speed-first products, velocity.set(), and
// every MsVECNormalize argument spelling: none moves the register.
void TFlyEnemy::calcChaseParam()
{
	JGeometry::TVec3<f32> toMario(FlyMarioX() - FlyPosX(this),
	                              SMS_GetMarioPos().y - mPosition.y,
	                              SMS_GetMarioPos().z - mPosition.z);
	toMario.x *= 1.1f;
	toMario.z *= 1.1f;

	JGeometry::TVec3<f32> goal;
	goal.x = mPosition.x + toMario.x;
	goal.y = mPosition.y + toMario.y;
	goal.z = mPosition.z + toMario.z;

	if (unk1A5)
		setGoalPath(TPathNode(goal));

	if (toMario.y > 100.0f || fabsf(toMario.y) < 100.0f) {
		if (unk1A5) {
			if (mFlyState != FLY_STATE_CHASE && mSprayedByWaterCooldown == 0)
				mGravityY = mFlyParams->mSLChaseFlyGravityY.get();
			mFlyState = FLY_STATE_CHASE;
		} else {
			mPosition.y -= 1.0f;
		}
	} else {
		mGravityY = mFlyParams->mSLNormalFlyGravityY.get();

		JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 0.0f);
		if (mFlyState != FLY_STATE_CHASE || toMario.y > 150.0f) {
			mFlyState = FLY_STATE_NORMAL;
			MsVECNormalize((Vec*)&toMario, (Vec*)&toMario);
			TFlyEnemyParams* params = mFlyParams;
			f32 speed               = params->getSLNormalFlySpeed();
			velocity.x              = toMario.x * speed;
			velocity.z              = toMario.z * speed;
			mGravityY               = params->mSLForceGravityY.get();
		} else {
			mPosition.y -= 3.0f;
		}

		// TODO: this throws away the velocity the branch above just computed,
		// leaving only the (unused) Y component. Present in the ROM.
		velocity.z = 0.0f;
		velocity.x = 0.0f;
		mVelocity  = velocity;
	}
}

void TFlyEnemy::bind()
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve()
	    || mFlyTime < TFlyEnemy::mInvalidTime)
		fly();
	else
		TLiveActor::bind();
}

// UNUSED (0x2bc): inlined into TNerveFlyEnemyChaseFly::execute's chase case.
void TFlyEnemy::flyMove()
{
	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(0.9f);
	mVelocity = velocity;

	JGeometry::TVec3<f32> toGoal = getUnkF4().getPoint();
	toGoal.sub(mPosition);
	// TODO: the magnitude is computed and discarded; debug leftover.
	VECMag((Vec*)&toGoal);

	f32 goalYaw = MsAngleWrap(MsGetRotFromZaxisY(toGoal));
	f32 diff    = MsAngleDiff(goalYaw, mRotation.y);
	f32 turn;
	if (diff > 0.0f) {
		turn = diff > mTurnSpeed ? mTurnSpeed : diff;
	} else {
		diff = diff > -mTurnSpeed ? diff : -mTurnSpeed;
		turn = diff;
	}
	mRotation.y = MsAngleWrap(mRotation.y + turn);

	JGeometry::TVec3<f32> linear = mLinearVelocity;
	f32 yaw                      = mRotation.y;
	f32 speed                    = mMarchSpeed;
	JGeometry::TVec3<f32> forward;
	MsGetVecFromRotY(forward, yaw, speed);
	linear.add(forward);
	mLinearVelocity = linear;
}

// The normal-fly nerve's two chase tests share this squared range. The
// `*=` puts the product in the range's own FPR (retail's f1 against the
// distance in f0), and the direct-return params fork nested in it is the
// pool rung that, with the raw mBodyScale read below, lands the 0x90 frame.
static inline TFlyEnemyParams* FlyP(TFlyEnemy* e) { return e->mFlyParams; }
static inline f32 FlyChaseDistSq(TFlyEnemy* e)
{
	f32 chaseDist = FlyP(e)->getSLChaseDist();
	chaseDist *= chaseDist;
	return chaseDist;
}

DEFINE_NERVE(TNerveFlyEnemyNormalFly, TLiveActor)
{
	TFlyEnemy* flyEnemy = (TFlyEnemy*)spine->getBody();

	if (spine->getTime() == 0)
		flyEnemy->setNormalFlyAnm();

	if (flyEnemy->unk1A4 && flyEnemy->mFlyTime > 500) {
		flyEnemy->updateSquareToMario();
		f32 chaseDist = FlyChaseDistSq(flyEnemy);
		if (flyEnemy->getDistToMarioSquared() < chaseDist) {
			spine->pushAfterCurrent(&TNerveFlyEnemyChaseFly::theNerve());
			return TRUE;
		}
	} else if (!flyEnemy->mIsGold && flyEnemy->isFindMario(1.0f)
	           && flyEnemy->mFlyTime > 100) {
		flyEnemy->updateSquareToMario();
		f32 chaseDist = FlyChaseDistSq(flyEnemy);
		if (flyEnemy->getDistToMarioSquared() < chaseDist) {
			spine->pushAfterCurrent(&TNerveFlyEnemyChaseFly::theNerve());
			return TRUE;
		}
	}

	flyEnemy->mRotation.x
	    = MsGetRotFromZaxis(JGeometry::TVec3<f32>(flyEnemy->mVelocity)).x;

	flyEnemy->mScaling.x = flyEnemy->mScaling.y = flyEnemy->mScaling.z
	    = MsClamp(1.05f * flyEnemy->mScaling.x, 0.0f,
	              flyEnemy->mBodyScale);
	return FALSE;
}

// TODO: 98.9%. The forward step now uses the MsGetVecFromRotY_L1 ladder
// above, which is the only spelling that reproduces retail's call set here
// (bl MsSin, bl MsCos, bl set<f32>) and it also recovers the third
// callee-saved FPR. This supersedes the earlier note that a second wrapper
// was not justified: the ladder is measured, not guessed, and a
// statement-bearing helper does not substitute for it (a three-statement
// `MsAddVecFromRotY` doing the add itself only buys one level, 96.4%).
// Remaining: the frame is 48 bytes short and one `fmr` is missing from the
// negative turn clamp.
// The ROM's toGoal sits low (0x88, a temporary) and the velocity copy high
// (0xf4); ours are adjacent named locals. A by-value TVec3 helper doing the
// sub/VECMag/MsGetRotFromZaxisY (with or without the wrap) adds 42
// instructions, so the enclosing shape is still unknown.
DEFINE_NERVE(TNerveFlyEnemyChaseFly, TLiveActor)
{
	TFlyEnemy* flyEnemy = (TFlyEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		flyEnemy->unk1A8 = flyEnemy->mVelocity;
		flyEnemy->calcChaseParam();
		flyEnemy->setChaseFlyAnm();
	}

	f32 rate = 1.0f;
	if (!flyEnemy->unk1A5) {
		f32 marchSpeedSq
		    = flyEnemy->getMarchSpeed() * flyEnemy->getMarchSpeed();
		flyEnemy->unk1A8.y = 0.1f;
		rate = flyEnemy->unk1A8.length() / marchSpeedSq * TFlyEnemy::mTestSp;
	} else if (flyEnemy->mFlyState == TFlyEnemy::FLY_STATE_NORMAL) {
		rate = 2.0f;
	}
	flyEnemy->walkBehavior(2, rate);

	if ((flyEnemy->unk1A5 && flyEnemy->isReachedToGoalXZ())
	    || (flyEnemy->mFlyState == TFlyEnemy::FLY_STATE_NORMAL
	        && flyEnemy->mPosition.y < 100.0f + flyEnemy->getGroundHeight()))
		flyEnemy->calcChaseParam();

	switch (flyEnemy->mFlyState) {
	case TFlyEnemy::FLY_STATE_NORMAL:
	case TFlyEnemy::FLY_STATE_UNK1:
		flyEnemy->walkBehavior(3, 1.0f);
		break;
	case TFlyEnemy::FLY_STATE_CHASE: {
		// The ROM has no call here: TFlyEnemy::flyMove is UNUSED and its body
		// is spelled out in the nerve, the same shape as
		// TFruitsBoat::rowToCurPathNode and TYumbo::lookatMario.
		JGeometry::TVec3<f32> velocity = flyEnemy->mVelocity;
		velocity.scale(0.9f);
		flyEnemy->mVelocity = velocity;

		JGeometry::TVec3<f32> toGoal = flyEnemy->getUnkF4().getPoint();
		toGoal.sub(flyEnemy->mPosition);
		VECMag((Vec*)&toGoal);

		f32 goalYaw = MsAngleWrap(MsGetRotFromZaxisY(toGoal));
		f32 diff    = MsAngleDiff(goalYaw, flyEnemy->mRotation.y);
		f32 turn;
		if (diff > 0.0f) {
			turn = diff > flyEnemy->getTurnSpeed()
			           ? flyEnemy->getTurnSpeed()
			           : diff;
		} else {
			diff = diff > -flyEnemy->getTurnSpeed()
			           ? diff
			           : -flyEnemy->getTurnSpeed();
			turn = diff;
		}
		flyEnemy->mRotation.y = MsAngleWrap(flyEnemy->mRotation.y + turn);

		JGeometry::TVec3<f32> linear = flyEnemy->mLinearVelocity;
		f32 yaw                      = flyEnemy->mRotation.y;
		f32 speed                    = flyEnemy->getMarchSpeed();
		JGeometry::TVec3<f32> forward;
		MsGetVecFromRotY_L1(forward, yaw, speed);
		linear.add(forward);
		flyEnemy->mLinearVelocity = linear;
		break;
	}
	}

	if (flyEnemy->mFlyState != TFlyEnemy::FLY_STATE_CHASE
	    && flyEnemy->mPosition.y > 200.0f + flyEnemy->getGroundHeight()) {
		f32 marchSpeed = flyEnemy->getMarchSpeed();
		JGeometry::TVec3<f32> dive(flyEnemy->getMarchSpeed(),
		                           -40.0f * flyEnemy->getGravityY(),
		                           marchSpeed);
		flyEnemy->mRotation.x = MsGetRotFromZaxis(dive).x;

		JGeometry::TVec3<f32> linear(flyEnemy->mLinearVelocity);
		linear.y              = flyEnemy->getGravityY();
		flyEnemy->mRotation.x = MsGetRotFromZaxis(linear).x;
	} else {
		flyEnemy->mRotation.x *= 0.99f;
	}

	flyEnemy->flyBehavior();

	flyEnemy->mScaling.x = flyEnemy->mScaling.y = flyEnemy->mScaling.z
	    = MsClamp(1.1f * flyEnemy->mScaling.x, 0.0f,
	              flyEnemy->getBodyScale());
	return FALSE;
}

TKillerSaveLoadParams::TKillerSaveLoadParams(const char* prm)
    : TFlyEnemyParams(prm)
    , PARAM_INIT(mSLWaterAddGravityY, 1.0f)
    , PARAM_INIT(mSLChaseTimer, 1000)
    , PARAM_INIT(mSLBombRange, 300.0f)
{
	TParams::load(mPrmPath);
}

TKillerManager::TKillerManager(const char* name)
    : TSmallEnemyManager(name)
{
	gpCurKiller = nullptr;
}

void TKillerManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TKillerSaveLoadParams("/enemy/killer.prm");
}

void TKillerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "killer_model1.bmd", 0x10220000, 0 },
		{ "downkiller_model1.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};

	createModelDataArray(entry);
}

TSpineEnemy* TKillerManager::createEnemyInstance() { return new TKiller; }

// The rolling killer spins its body joint around Z on top of whatever the
// animation produced, then re-applies the body scale.
// TODO: 96.9%. Frame, slots and r31 (the roll matrix) now match; the ROM
// loads roll[2][2]'s 1.0f just before its store where ours hoists it to the
// top, which shifts every FPR by one. Tried: the roll build as a TU-local
// helper (-8 frame), a chained zero row, a named angle, killer-> reads, an
// s16 angle through JMASSin, the scale build as a helper, a 1.0f-returning
// helper and a named `one` local.
static int KillerBodyCallback(J3DNode* node, int param)
{
	if (param == 0) {
		TKiller* killer = gpCurKiller;
		if (killer == nullptr || !TKiller::mRollSw || !killer->isRollFly())
			return 1;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx = gpCurKiller->getModel()->getAnmMtx(joint->getJntNo());

		Mtx scale;
		Mtx roll;
		scale[0][3] = 0.0f;
		scale[1][3] = 0.0f;
		scale[2][3] = 0.0f;
		f32 s       = gpCurKiller->getBodyScale();
		scale[0][0] = s;
		scale[0][1] = 0.0f;
		scale[0][2] = 0.0f;
		scale[1][0] = 0.0f;
		scale[1][1] = s;
		scale[1][2] = 0.0f;
		scale[2][0] = 0.0f;
		scale[2][1] = 0.0f;
		scale[2][2] = s;

		f32 rs      = JMASin(gpCurKiller->mRollAngle);
		f32 rc      = JMACos(gpCurKiller->mRollAngle);
		roll[0][0]  = rc;
		roll[0][1]  = -rs;
		roll[0][2]  = 0.0f;
		roll[0][3]  = 0.0f;
		roll[1][0]  = rs;
		roll[1][1]  = rc;
		roll[1][2]  = 0.0f;
		roll[1][3]  = 0.0f;
		roll[2][0]  = 0.0f;
		roll[2][1]  = 0.0f;
		roll[2][2]  = 1.0f;
		roll[2][3]  = 0.0f;

		MtxPtr rollMtx = roll;
		MTXConcat(anmMtx, rollMtx, anmMtx);
		MTXConcat(anmMtx, scale, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, rollMtx, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

TKiller::TKiller(const char* name)
    : TFlyEnemy(name)
{
}

void TKiller::init(TLiveManager* manager)
{
	TFlyEnemy::init(manager);
	mActorType    = 0x1000001F;
	unk150        = 17;
	mKillerParams = (TKillerSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveFlyEnemyNormalFly::theNerve());
	onLiveFlag(LIVE_FLAG_UNK400);
	offLiveFlag(LIVE_FLAG_UNK800);
	onHitFlag(HIT_FLAG_UNK40000000);

	J3DModel* model = getMActor()->getModel();
	if (!model->getSkinDeform()) {
		J3DSkinDeform* deform = new J3DSkinDeform;
		model->setSkinDeform(deform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	}

	getMActor()->resetDL();

	// TODO: the ROM walks every joint of the first instance and does nothing
	// with them; whatever the body was has been optimised away.
	if (getInstanceIndex() == 0) {
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); i++) { }
	}

	getMActor()->setJointCallback(1, KillerBodyCallback);
	unk188 = 0.0f;
}

void TKiller::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("killer_model1.bmd", 3);
	mMActorKeeper->createMActor("downkiller_model1.bmd", 3);

	s32 noseMatIdx = getActorKeeper()
	                     ->getMActor("killer_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_nosemat1");
	s32 eyesMatIdx = getActorKeeper()
	                     ->getMActor("killer_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_eyesmat1");
	s32 bodyMatIdx = getActorKeeper()
	                     ->getMActor("killer_model1.bmd")
	                     ->getModel()
	                     ->getModelData()
	                     ->getMaterialName()
	                     ->getIndex("_body1");

	SMS_InitPacket_OneTevColor(getMActor()->getModel(), noseMatIdx, GX_TEVREG0,
	                           &mNoseColor);
	SMS_InitPacket_OneTevColor(getMActor()->getModel(), eyesMatIdx, GX_TEVREG0,
	                           &mEyesColor);
	SMS_InitPacket_OneTevColor(getMActor()->getModel(), bodyMatIdx, GX_TEVREG0,
	                           &mBodyColor);
	SMS_InitPacket_OneTevColor(
	    getActorKeeper()->getMActor("downkiller_model1.bmd")->getModel(),
	    bodyMatIdx, GX_TEVREG0, &mBaseColor);
}

void TKiller::behaveToWater(THitActor* water)
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyNormalFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
		genEventCoin();

	if (mSpine->getCurrentNerve() != &TNerveKillerExplosion::theNerve()) {
		mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
		onHitFlag(HIT_FLAG_NO_COLLISION);

		JGeometry::TVec3<f32> velocity(0.0f, 0.0f, 0.0f);
		mVelocity = velocity;
	}
}

// TODO: 99.9%. Function-scope offset + KillerPosX (getPosition().x consumed
// inside a by-value fork) lands the 0xd8 frame and the 0x20/0x50 slots;
// declaring spreadMtx first and assigning it later gives it r31 over coinNum.
// Residue: the coin X argument's fadds operands (pos.x f3 first in retail).
// Tried: swapping the operands, offset.get().x, a const-ref read, an
// add-returning fork (+frame), getPosition().x raw (-8 frame).
void TKiller::genEventCoin()
{
	MtxPtr spreadMtx;
	int coinNum = 2;
	if (mIsGold)
		coinNum = 8;

	JGeometry::TVec3<f32> offset;
	Mtx spread;
	spreadMtx = spread;
	for (int i = 0; i < coinNum; i++) {
		offset.set(0.0f, 0.0f, 30.0f);
		f32 yaw = 360.0f * (1.0f / coinNum) * (i + 1);

		f32 s        = JMASin(yaw);
		f32 c        = JMACos(yaw);
		spread[0][0] = c;
		spread[0][1] = 0.0f;
		spread[0][2] = s;
		spread[0][3] = 0.0f;
		spread[1][0] = 0.0f;
		spread[1][1] = 1.0f;
		spread[1][2] = 0.0f;
		spread[1][3] = 0.0f;
		spread[2][0] = -s;
		spread[2][1] = 0.0f;
		spread[2][2] = c;
		spread[2][3] = 0.0f;
		MTXMultVec(spreadMtx, (Vec*)&offset, (Vec*)&offset);

		TMapObjBase* coin = gpItemManager->makeObjAppear(
		    KillerPosX(this) + offset.x, mPosition.y, mPosition.z + offset.z,
		    0x2000000E, true);
		if (coin) {
			coin->mPosition.y = mPosition.y;
			MsVECNormalize((Vec*)&offset, (Vec*)&offset);
			coin->mVelocity.set(3.0f * offset.x, 20.0f, 3.0f * offset.z);
			coin->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}
}

bool TKiller::isHitValid(u32 message)
{
	if (message == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		genEventCoin();
		return false;
	}

	if (TKiller::mTrampleDie)
		mGravityY -= 12.0f;

	return false;
}

void TKiller::setDeadAnm()
{
	mMActor = getActorKeeper()->getMActor("downkiller_model1.bmd");
	setBckAnm(KILLER_ANM_DOWN1);

	TEffectExplosion* explosion
	    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト爆発マネージャー", 1);
	if (explosion)
		explosion->generate(mPosition, mScaling);

	gpCameraShake->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);
	SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
}

void TKiller::attackToMario()
{
	if (SMS_GetMarioPos().y < mPosition.y) {
		if (mSpine->getCurrentNerve() != &TNerveKillerExplosion::theNerve()) {
			mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
			sendAttackMsgToMario();
		} else {
			SMS_SendMessageToMario(this, HIT_MESSAGE_UNKA);
		}
	}
}

const char** TKiller::getBasNameTable() const { return killer_bastable; }

void TKiller::setNormalFlyAnm()
{
	mMActor = getActorKeeper()->getMActor("killer_model1.bmd");
	setBckAnm(KILLER_ANM_FLY);
	mRollAngle = 0.0f;
	mFlyTime   = 0;
}

void TKiller::setChaseFlyAnm() { setBckAnm(KILLER_ANM_SEARCH1); }

bool TKiller::isRollFly()
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
		if (isBckAnm(KILLER_ANM_UNK1))
			return true;
	return false;
}

bool TKiller::isCollidMove(THitActor* other)
{
	if (other->isActorType(0x4000000A))
		((TLiveActor*)other)->kill();

	if (TKiller::mSerialBomb) {
		if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve())
			mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
	}

	return true;
}

void TKiller::flyBehavior()
{
	mTurnSpeed = mKillerParams->getSLTurnSpeedLow();

	if (getSpine()->getTime() > mKillerParams->mSLChaseTimer.get())
		mGravityY -= mKillerParams->mSLWaterAddGravityY.get();

	if (checkCurAnmEnd(KILLER_ANM_DOWN1)) {
		if (isBckAnm(KILLER_ANM_SEARCH1))
			setBckAnm(KILLER_ANM_UNK1);
	}

	mRollAngle += 2.5f;
}

// TODO: 99.9%, frame 0x20 against the ROM's 0x28. TBombHei::changeOut and
// TSmallEnemy::changeOut are the same code and are 8 bytes short in exactly
// the same way; see docs/catalog/frame-gaps.md.
void TKiller::changeOut()
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_TELSA_RECOVER, &mPosition);

	onLiveFlag(LIVE_FLAG_DEAD);
	genEventCoin();
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mPosition = mJuiceBlock->mPosition;

	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
	                                            &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	mJuiceBlock->kill();
	mJuiceBlock = nullptr;
}

void TKiller::reset()
{
	gpCurKiller = this;
	TFlyEnemy::reset();

	TMsRange<f32> goldRate(0.0f, 1.0f);

	mBaseColor.r = mBaseColor.g = mBaseColor.b = 0;
	mBodyColor.r = mBodyColor.g = mBodyColor.b = 0;

	mIsGold = false;
	if (goldRate.rand() < 0.05f) {
		mIsGold      = true;
		mBodyColor.r = 200;
		mBodyColor.g = 185;
		mBodyColor.b = 0;
		mBaseColor.r = 255;
		mBaseColor.g = 225;
		mBaseColor.b = 70;
	}
}

void TKiller::setColorType()
{
	if (mIsGold)
		unk1A5 = false;

	if (unk1A5) {
		mBodyColor.r = 70;
		mBodyColor.g = 20;
		mBodyColor.b = 70;
		mBaseColor.r = 70;
		mBaseColor.g = 20;
		mBaseColor.b = 70;
	}
}

// TODO: 99.9%, instruction-identical with a 16-byte frame gap.
void TKiller::bind()
{
	if (mSpine->getCurrentNerve() == &TNerveFlyEnemyChaseFly::theNerve()
	    || mFlyTime < TFlyEnemy::mInvalidTime)
		fly();
	else
		TLiveActor::bind();

	mFlyTime++;

	if (mSpine->getCurrentNerve() != &TNerveKillerExplosion::theNerve()) {
		if (!isAirborne() && mFlyTime > TFlyEnemy::mInvalidTime) {
			mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
		} else if (mFlyTime > TFlyEnemy::mInvalidTime) {
			TBGWallCheckRecord record(getPosition().x,
			                          mPosition.y + mHeadHeight,
			                          getPosition().z, 2.0f * mBodyRadius, 1,
			                          0);
			if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
				const TLiveActor* rider
				    = record.mResultWalls[0]->getActor();
				if (rider) {
					if (rider->isActorType(0x4000000A))
						((TLiveActor*)rider)->kill();
				}
				mSpine->pushNerve(&TNerveKillerExplosion::theNerve());
			}
		}
	}

	if (isAirborne()) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_KILLER_FLY, &mPosition);

		mRotation.x = MsClamp(mRotation.x, -25.0f, 90.0f);
		MsMtxSetXYZRPH(mParticleMtx, mPosition.x, mPosition.y, mPosition.z,
		               mRotation.x, mRotation.y, mRotation.z);
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_KIL_SMOKE,
		                                            mParticleMtx, 1, this);
	}
}

// Binding level worth +8 of low region, landing TKiller::calcRootMatrix's
// frame at 0x90 (batch 121).
static inline TSpineBase<TLiveActor>* KillerGetSpine(const TKiller* p)
{
	TSpineBase<TLiveActor>* spine = p->getSpine();
	return spine;
}

// TODO: 99.9%, instruction-identical with a 32-byte frame gap.
void TKiller::calcRootMatrix()
{
	if (SMSGetMarDirector()->checkUnk4CFlag(0xF)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (isBckAnm(KILLER_ANM_FLY)) {
		if (mIsGold) {
			mNoseColor.r = mEyesColor.r = 170;
			mNoseColor.g = mEyesColor.g = 140;
			mNoseColor.b = mEyesColor.b = 0;
		} else {
			mEyesColor.r = mEyesColor.g = mEyesColor.b = 0;
			mNoseColor.r = mNoseColor.g = mNoseColor.b = 0;
		}
	}

	if (isBckAnm(KILLER_ANM_SEARCH1)) {
		mEyesColor.r = mNoseColor.g = mNoseColor.b = 0;
		if (KillerGetSpine(this)->getTime() % 10 < 5)
			mEyesColor.g = mEyesColor.b = mNoseColor.r = 0;

		if (mIsGold) {
			if (getSpine()->getTime() % 10 < 5) {
				mNoseColor.r = 170;
				mNoseColor.g = 140;
				mNoseColor.b = 0;
			} else {
				mNoseColor.r = 180;
				mNoseColor.g = 140;
				mNoseColor.b = 150;
			}
		}

		if (unk1A5) {
			if (getSpine()->getTime() % 10 < 5) {
				mNoseColor.r = mEyesColor.r = 200;
				mNoseColor.g                = 0;
				mNoseColor.b                = 0;
			} else {
				mNoseColor.r = 70;
				mNoseColor.g = 20;
				mNoseColor.b = 70;
			}
		}
	}

	if (isBckAnm(KILLER_ANM_UNK1)) {
		mEyesColor.r = 0;
		mEyesColor.g = mEyesColor.b = 0;
		mNoseColor.g = mNoseColor.b = 0;

		f32 pulse = fabsf(JMASin(360.0f * getSpine()->getTime() / 120.0f));

		if (mIsGold) {
			mBodyColor.r = 170;
			mBodyColor.g = 140;
			mBodyColor.b = 0;
			mNoseColor.r = (u8)(10.0f * pulse + 160.0f);
			mNoseColor.g = (u8)(30.0f * pulse + 140.0f);
			mNoseColor.b = (u8)(150.0f * pulse);
		}

		if (unk1A5) {
			mBodyColor.r = 70;
			mBodyColor.g = 20;
			mBodyColor.b = 70;
			mBaseColor.r = 70;
			mBaseColor.g = 20;
			mBaseColor.b = 70;
			mNoseColor.r = (u8)(130.0f * pulse + 70.0f);
			mNoseColor.g = (u8)(20.0f - 20.0f * pulse);
			mNoseColor.b = (u8)(70.0f - 70.0f * pulse);
		}
	}

	gpCurKiller = this;
	mRotation.x = MsClamp(mRotation.x, -25.0f, 90.0f);
	TSpineEnemy::calcRootMatrix();
}

// Same body as TSmallEnemy::isFindMarioFromParam: *= into the named
// search-length local loads it straight into f1 (the first isInSight
// argument) instead of f0-then-fmuls.
bool TKiller::isFindMario(f32 rate)
{
	TSmallEnemyParams* params = getSaveParams();

	f32 searchHeight = params->mSLSearchHeight.get();

	if (abs(SMS_GetMarioPos().y - mPosition.y) < searchHeight) {
		JGeometry::TVec3<f32> marioPos(SMS_GetMarioPos().x,
		                               SMS_GetMarioPos().y,
		                               SMS_GetMarioPos().z);

		f32 searchLength = params->mSLSearchLength.get();
		f32 searchAngle  = params->mSLSearchAngle.get();
		f32 searchAware  = params->mSLSearchAware.get();

		searchLength *= rate;
		searchAngle *= rate;
		searchAware *= rate;

		if (isInSight(marioPos, searchLength, searchAngle, searchAware))
			return true;
		else
			return false;
	}

	return false;
}

DEFINE_NERVE(TNerveKillerExplosion, TLiveActor)
{
	TKiller* killer = (TKiller*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 bombRange = killer->getSaveParam3()->getSLBombRange();
		killer->mExplosionScaleMax
		    = bombRange * killer->getBodyScale() / killer->mAttackRadius;
		killer->mRotation.x        = 0.0f;
		killer->setDeadAnm();

		if (!killer->isAirborne()) {
			if (killer->getGroundPlane()->isWaterSurface()) {
				TEffectBombColumWater* column
				    = (TEffectBombColumWater*)gpConductor->makeOneEnemyAppear(
				        killer->mPosition, "エフェクト爆発水柱マネージャー",
				        1);
				if (column) {
					JGeometry::TVec3<f32> scaling(2.0f, 2.0f, 2.0f);
					column->generate(killer->mPosition, scaling);
				}
			}

			if (killer->mGroundPlane->isSand()) {
				TEffectColumSand* column
				    = (TEffectColumSand*)gpConductor->makeOneEnemyAppear(
				        killer->mPosition, "エフェクト砂柱マネージャー", 1);
				if (column) {
					JGeometry::TVec3<f32> scaling(0.6f, 0.9f, 0.6f);
					column->generate(killer->mPosition, scaling);
				}
			}
		}

		SMSRumbleMgr->start(0x13, (Vec*)&killer->mPosition);
	}

	if (killer->unk190 < killer->mExplosionScaleMax) {
		killer->unk190 *= 1.3f;
	} else {
		killer->onHitFlag(HIT_FLAG_NO_COLLISION);
		if (killer->checkCurAnmEnd(TKiller::KILLER_ANM_DOWN1)) {
			killer->onLiveFlag(LIVE_FLAG_DEAD);
			killer->onLiveFlag(LIVE_FLAG_UNK8);
			killer->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
			killer->mHolder = nullptr;
			killer->stopAnmSound();
			spine->reset();
			spine->setNext(&TNerveSmallEnemyDie::theNerve());
			spine->pushAfterCurrent(spine->getDefault());
			killer->mPosition.y -= 200.0f;
			return TRUE;
		}
	}

	killer->expandCollision();
	return FALSE;
}
