#include <Enemy/TobiPuku.hpp>

#include <Strategic/Spine.hpp>

#include <M3DUtil/MActor.hpp>

#include <MarioUtil/MathUtil.hpp>

#include <Map/MapData.hpp>

#include <Map/Map.hpp>

#include <Map/MapCollisionData.hpp>

#include <Enemy/SmallEnemy.hpp>

#include <Player/MarioAccess.hpp>

#include <Enemy/PathNode.hpp>

#include <Enemy/Conductor.hpp>

#include <MoveBG/MapObjBlock.hpp>

#include <Enemy/EffectObj.hpp>

#include <MSound/MSound.hpp>

#include <MSound/MSoundSE.hpp>

#include <System/Particles.hpp>

#include <JSystem/JParticle/JPAEmitter.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

#include <MSound/MSSetSound.hpp>

#include <MSound/MSoundBGM.hpp>

static const char* pukupuku_bastable[] = {
	nullptr,
	"/scene/pukupuku/bas/pukupuku_death.bas",
	"/scene/pukupuku/bas/pukupuku_down_air.bas",
	"/scene/pukupuku/bas/pukupuku_down_land.bas",
	nullptr,
	"/scene/pukupuku/bas/pukupuku_fall_end_land.bas",
	nullptr,
	nullptr,
	"/scene/pukupuku/bas/pukupuku_pitipiti.bas",
	"/scene/pukupuku/bas/pukupuku_swim.bas",
	nullptr,
};

static const char* moepuku_bastable[] = {
	nullptr,
	nullptr,
	"/scene/moepuku/bas/moepuku_down_air.bas",
	"/scene/moepuku/bas/moepuku_down_land.bas",
	nullptr,
	"/scene/moepuku/bas/moepuku_fall_end_land.bas",
	nullptr,
	nullptr,
	"/scene/moepuku/bas/moepuku_pitipiti.bas",
	nullptr,
	nullptr,
};

// Animation slots shared by both variants.
enum {
	PUKU_ANM_ATTACK        = 0,
	PUKU_ANM_DEAD          = 1,
	PUKU_ANM_DOWN_AIR      = 2,
	PUKU_ANM_DOWN_LAND     = 3,
	PUKU_ANM_FALL          = 4,
	PUKU_ANM_FALL_END_LAND = 5,
	PUKU_ANM_JUMP          = 6,
	PUKU_ANM_JUMP_START    = 7,
	PUKU_ANM_PICHI         = 8,
	PUKU_ANM_SWIM          = 9,
};

f32 TTobiPuku::mLandAngle = 90.0f;

u8 TTobiPuku::mBoundSw = 1;

f32 TTobiPuku::mBoundVelocityY = 0.8f;

u8 TTobiPuku::mReturnLaunchSw = 1;

static TMoePuku* gpCurTobiPuku;

static int TobiPukuRollCallback(J3DNode* node, int param);

// Rolls the whole model about Z while the puku is being flung, so it tumbles
// instead of gliding flat. Only the three launch-related nerves want it.
static int TobiPukuRollCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		TMoePuku* puku = gpCurTobiPuku;
		if (puku) {
			if (puku->isRoll()) {
				J3DJoint* joint = (J3DJoint*)param_1;
				MtxPtr anmMtx   = gpCurTobiPuku->getMActor()
				                    ->getModel()
				                    ->getAnmMtx(joint->getJntNo());

				Mtx local_44;
				MsMtxSetRotZ(local_44, gpCurTobiPuku->unk1EC);

				MTXConcat(anmMtx, local_44, anmMtx);
				MTXConcat(J3DSys::mCurrentMtx, local_44,
				          J3DSys::mCurrentMtx);
			}
		}
	}
	return true;
}

// The map emits both params constructors (UNUSED) immediately before
// TobiPukuRollCallback, so in reverse source order they follow it.
TTobiPukuLaunchPadSaveLoadParams::TTobiPukuLaunchPadSaveLoadParams(
    const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mSLLaunchInterval, 300)
    , PARAM_INIT(mSLLaunchVelocityY, 12.0f)
    , PARAM_INIT(mSLFlyDist, 1000.0f)
    , PARAM_INIT(mSLFlySpeed, 30.0f)
    , PARAM_INIT(mSLLaunchAngle, 45.0f)
{
	TParams::load(mPrmPath);
}

TTobiPukuSaveLoadParams::TTobiPukuSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLBoundNum, 3)
    , PARAM_INIT(mSLBoundVal, 0.8f)
    , PARAM_INIT(mSLLifeTimer, 200)
    , PARAM_INIT(mSLFlyGravityY, 0.2f)
    , PARAM_INIT(mSLPowerFromWater, 1.0f)
{
	TParams::load(mPrmPath);
}

TTobiPukuLaunchPadManager::TTobiPukuLaunchPadManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk60 = 0;
}

void TTobiPukuLaunchPadManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TTobiPukuLaunchPadSaveLoadParams("/enemy/tobipukulaunch.prm");
	TSmallEnemyManager::load(stream);
}

TSpineEnemy* TTobiPukuLaunchPadManager::createEnemyInstance()
{
	return new TTobiPukuLaunchPad("とびプク発射台");
}

void TTobiPukuLaunchPadManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	for (int i = 0; i < getActiveObjNum(); ++i)
		((TTobiPukuLaunchPad*)TLiveManager::getObj(i))
		    ->perform(cue, graphics);
}

TSpineEnemy* TMoePukuLaunchPadManager::createEnemyInstance()
{
	return new TMoePukuLaunchPad("モエプク発射台");
}

TTobiPukuManager::TTobiPukuManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TTobiPukuManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TTobiPukuSaveLoadParams("/enemy/tobipuku.prm");
	TSmallEnemyManager::load(stream);
}

TSpineEnemy* TTobiPukuManager::createEnemyInstance()
{
	return new TTobiPuku("とびプク");
}

TSpineEnemy* TMoePukuManager::createEnemyInstance()
{
	return new TMoePuku("モエプク");
}

TTobiPukuLaunchPad::TTobiPukuLaunchPad(const char* name)
    : TSmallEnemy(name)
{
	unk194 = 0;
	unk19C = 0.0f;
	unk1A8 = nullptr;
}

void TTobiPukuLaunchPad::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (checkLiveFlag(LIVE_FLAG_UNK200))
		return;
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	if (!(cue & CUE_MOVE))
		return;

	if (TTobiPuku::mReturnLaunchSw) {
		// Hold the next launch until the one already in flight is gone.
		if (!unk1A8) {
			launch();
			return;
		}
		if (unk1A8->checkLiveFlag(LIVE_FLAG_DEAD))
			launch();
	} else {
		unk194++;
		if (unk194 > unk198->mSLLaunchInterval.get()) {
			unk194 = 0;
			launch();
		}
	}
}

void TTobiPukuLaunchPad::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType = 0x10000012;
	unk198     = (TTobiPukuLaunchPadSaveLoadParams*)getSaveParam();
}

void TTobiPukuLaunchPad::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);

	int angle;
	stream.read(&angle, 4);
	unk19C = angle;

	reset();
}

void TTobiPukuLaunchPad::reset()
{
	TSmallEnemy::reset();
	unk194 = 0;
	unk1A8 = nullptr;
}

void TTobiPukuLaunchPad::launch()
{
	TTobiPuku* puku = (TTobiPuku*)gpConductor->makeOneEnemyAppear(
	    mPosition, "とびプクマネージャー", 1);
	if (puku) {
		forceLaunch(puku);
		unk1A8 = puku;
	}
}

void TTobiPukuLaunchPad::forceLaunch(TTobiPuku* puku)
{
	JGeometry::TVec3<f32> target(mPosition);

	// 16384/90 is 65536/360 written the long way; the original spells the
	// conversion out here rather than going through DEG2SHORTANGLE.
	s16 yaw   = (s16)(16384.0f * mRotation.y / 90.0f);
	f32 sinYaw = JMASSin(yaw);
	f32 cosYaw = JMASCos(yaw);

	JGeometry::TVec3<f32> velocity;
	if (((TTobiPukuLaunchPadManager*)getManager())->unk60) {
		// Aim at a point mSLFlyDist ahead and solve for the arc.
		f32 dist = unk198->mSLFlyDist.get();
		target.x += sinYaw * dist;
		target.z += cosYaw * dist;
		velocity = calcVelocityToJumpToY(
		    target, unk198->mSLLaunchVelocityY.get(),
		    puku->unk19C->mSLFlyGravityY.get());
	} else {
		// Otherwise just fire along the pad's own facing at its own speed.
		f32 speed    = unk19C;
		s16 pitch    = (s16)(16384.0f * mRotation.x / 90.0f);
		f32 cosPitch = JMASCos(pitch);
		velocity.x   = sinYaw * speed * cosPitch;
		velocity.y   = 1.0f * speed * JMASSin(pitch);
		velocity.z   = cosYaw * speed * cosPitch;
	}

	puku->reset();
	puku->mPosition       = mPosition;
	puku->mRotation       = mRotation;
	puku->mLaunchVelocity = velocity;
	puku->unk1B0          = mPosition.y;
	puku->mLaunchPad      = this;

	JGeometry::TVec3<f32> vel(mVelocity);
	puku->mLaunchAngle = MsGetRotFromZaxis(vel).x;
}

void TMoePukuLaunchPad::launch()
{
	TTobiPuku* puku = (TTobiPuku*)gpConductor->makeOneEnemyAppear(
	    mPosition, "モエプクマネージャー", 1);
	if (puku) {
		forceLaunch(puku);
		unk1A8 = puku;
	}
}

// TODO: 0% of 180 bytes despite initialising the right fields. The original
// default-constructs mLand[0] and mLand[1] through __construct_array with a
// count of 2, and keeps `this` in a stack slot across the base call; ours
// inlines both TVec3 constructors instead. The field set and their values are
// confirmed by the assembly, so only the construction form is wrong.
TTobiPuku::TTobiPuku(const char* name)
    : TWalkerEnemy(name)
    , unk194(0)
    , mBoundCount(0)
    , unk19C(nullptr)
    , unk1AC(1)
    , unk1AD(1)
    , unk1AE(0)
    , unk1B0(0.0f)
    , mLaunchAngle(0.0f)
    , mSwimBaseY(0.0f)
    , mFlyVelocityY(0.0f)
    , mReturnPitchStep(0.0f)
    , unk1EC(0.0f)
{
	gpCurTobiPuku = nullptr;
}

void TTobiPuku::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000012;
	unk150     = 0x31;
	unk19C     = (TTobiPukuSaveLoadParams*)getSaveParam();
	mMActor->setJointCallback(1, TobiPukuRollCallback);
}

void TTobiPuku::reset()
{
	gpCurTobiPuku = (TMoePuku*)this;
	TWalkerEnemy::reset();
	mSpine->initWith(&TNerveTobiPukuGenerate::theNerve());
	unk1AD          = 1;
	unk194          = 0;
	mLand[1]      = getPosition();
	mLand[0]        = mLand[1];
	mSwimBaseY      = getPosition().y;
}

void TTobiPuku::moveObject()
{
	mTurnSpeed = unk19C->mSLTurnSpeedLow.get();

	if (TTobiPuku::mBoundSw && isAirborne())
		hitWall();

	TWalkerEnemy::moveObject();
}

void TTobiPuku::hitWall()
{
	TBGWallCheckRecord record(mPosition.x, mPosition.y + mHeadHeight,
	                          mPosition.z, 1.1f * (mBodyScale * mWallRadius),
	                          1, 0);

	if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
		// TODO: 97.1%. One load short -- the original re-reads mNormal.x
		// for the x bounce -- and its frame is 0x30 larger: accessor pool
		// inside the inlined TBGWallCheckRecord constructor.
		f32 dot = getVelocity().x * record.mResultWalls[0]->mNormal.x
		          + getVelocity().y * record.mResultWalls[0]->mNormal.y
		          + getVelocity().z * record.mResultWalls[0]->mNormal.z;
		f32 bounce = -(2.0f * dot);
		mVelocity.x += bounce * record.mResultWalls[0]->mNormal.x;
		mVelocity.y *= 0.5f;
		mVelocity.z += bounce * record.mResultWalls[0]->mNormal.z;
		mLaunchVelocity = mVelocity;
		unk1B0          = mPosition.y;
		return;
	}

	const TBGCheckData* roof;
	gpMap->checkRoof(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
	                 &roof);
	if (roof && roof->getActor() && mVelocity.y > 0.0f)
		mVelocity.y = 0.0f;
}

// UNUSED, 0xc8 in the map and size-exact: the body TNerveTobiPukuBound::execute
// pastes in once canBound() has passed. Damp the stored launch velocity,
// rebuild the vertical component from the drop height, go airborne, and reset
// the drop reference while still rising.
void TTobiPuku::bound()
{
	mBoundCount = mBoundCount + 1;

	f32 damp = unk19C->mSLBoundVal.get();
	JGeometry::TVec3<f32> vel(mLaunchVelocity);
	vel.x *= damp;
	vel.z *= damp;
	vel.y = (TTobiPuku::mBoundVelocityY * damp * (unk1B0 - mGroundHeight))
	        / 30.0f;

	mLaunchVelocity = vel;
	mVelocity       = vel;
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	if (vel.y > 0.0f)
		unk1B0 = mPosition.y;
}

void TTobiPuku::calcRootMatrix()
{
	gpCurTobiPuku = (TMoePuku*)this;
	TSpineEnemy::calcRootMatrix();

	if (mRotation.x != 0.0f) {
		if (isEaten())
			return;

		// Tipping over on landing lifts the body so it pivots on its edge
		// rather than sinking into the ground.
		MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), getPosition().x,
		               mPosition.y
		                   + 70.0f * mRotation.x / TTobiPuku::mLandAngle,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	}

	if (isPichiEffect()) {
		mFlamePos.set(getMActor()->getModel()->getAnmMtx(1)[0][3],
		              getMActor()->getModel()->getAnmMtx(1)[1][3],
		              getMActor()->getModel()->getAnmMtx(1)[2][3]);
		gpMarioParticleManager->emitAndBindToPosPtr(0x177, &mFlamePos, 1,
		                                            this);
	}
}

bool TTobiPuku::isPichiEffect() { return isBckAnm(PUKU_ANM_PICHI) ? true : false; }

bool TTobiPuku::isJumpBck() { return isBckAnm(PUKU_ANM_JUMP) ? true : false; }

BOOL TTobiPuku::isDeadBck() { return isBckAnm(PUKU_ANM_DEAD) ? TRUE : FALSE; }

bool TTobiPuku::isAttackBck() { return isBckAnm(PUKU_ANM_ATTACK) ? true : false; }

BOOL TTobiPuku::isFallEndLandBck()
{
	return isBckAnm(PUKU_ANM_FALL_END_LAND) ? TRUE : FALSE;
}

bool TTobiPuku::isJumpStartBck()
{
	return isBckAnm(PUKU_ANM_JUMP_START) ? true : false;
}

void TTobiPuku::setJumpAnm() { setBckAnm(PUKU_ANM_JUMP); }

void TTobiPuku::setSwimAnm() { setBckAnm(PUKU_ANM_SWIM); }

void TTobiPuku::setAttackAnm() { setBckAnm(PUKU_ANM_ATTACK); }

void TTobiPuku::setPichiAnm() { setBckAnm(PUKU_ANM_PICHI); }

void TTobiPuku::setFallAnm() { setBckAnm(PUKU_ANM_FALL); }

void TTobiPuku::setDownAirAnm() { setBckAnm(PUKU_ANM_DOWN_AIR); }

void TTobiPuku::setDownLandAnm() { setBckAnm(PUKU_ANM_DOWN_LAND); }

void TTobiPuku::setDeadAnm() { setBckAnm(PUKU_ANM_DEAD); }

void TTobiPuku::setFallEndLandAnm() { setBckAnm(PUKU_ANM_FALL_END_LAND); }

void TTobiPuku::setJumpStartAnm()
{
	if (isBckAnm(PUKU_ANM_JUMP_START))
		setBckAnm(PUKU_ANM_JUMP_START);
}

// UNUSED, 0x2c in the map and size-exact: the guard
// TNerveTobiPukuBound::execute tests before bound(). The `unk1AE = 1` store is
// what the last two instructions of the map's 0x2c are; without it the body is
// 0x24.
bool TTobiPuku::canBound()
{
	unk1AE = 1;
	if (mBoundCount < unk19C->mSLBoundNum.get())
		return true;
	return false;
}

// UNUSED, 0x140 in the map. Inlined into TobiPukuRollCallback, where the
// materialised bool it returns is what shapes that function's branches.
bool TTobiPuku::isRoll()
{
	// TODO: 0x150 against the map's 0x140, and when inlined the original's
	// three tests converge on one `li r0, 1` where ours each get their own.
	// Writing the body as a single `a || b || c` return gets the size down
	// to 0xd4 but makes the inlined form much worse, so this stays.
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuLand::theNerve())
		return true;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuPrepareFly::theNerve())
		return true;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuReturnLaunch::theNerve())
		return true;
	return false;
}

void TTobiPuku::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuHitWater::theNerve())
		return;

	SMSGetMSound()->startSoundActor(MSD_SE_EN_COMMON_FLY, &mPosition);
	mSpine->pushNerve(&TNerveTobiPukuHitWater::theNerve());
}

// Binding level worth +8 of low region, landing TTobiPuku::walkBehavior's
// frame at 0x68 (batch 124).
static inline int TobiPukuGetTime(const TSpineBase<TLiveActor>* p)
{
	int time = p->getTime();
	return time;
}

void TTobiPuku::walkBehavior(int param_1, f32 param_2)
{
	TWalkerEnemy::walkBehavior(param_1, param_2);

	f32 prevY   = mPosition.y;
	mPosition.y = mSwimBaseY + 10.0f * JMASin(2.0f * TobiPukuGetTime(mSpine));

	// Only the pitch is taken: the bob is vertical, so yaw and roll are
	// left to whatever TWalkerEnemy::walkBehavior set.
	JGeometry::TVec3<f32> vel(mLinearVelocity);
	vel.y         = prevY - mPosition.y;
	mRotation.x = MsGetRotFromZaxis(vel).x;
}

void TTobiPuku::swimEffect()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_PUKU_AWA, getMActor()->getModel()->getAnmMtx(6), 1, this);
	if (!emitter)
		return;

	// Deeper water gives the bubble trail a longer life, up to a cap.
	s16 life = (s16)(mGroundHeight - mPosition.y) * 16 / 100 + 20;
	if (life > 200)
		life = 200;
	emitter->mBaseLifetime = life;
}

// PathNode.hpp's getPoint() reaches the node's actor through getPosition();
// reading mPosition raw instead is low region here. Header round 27 measured
// the same change made in the header as a tree-wide wash, so it stays parked
// TU-locally.
static inline const JGeometry::TVec3<f32>& TobiPukuGetPoint(const TPathNode& node)
{
	if (node.unk0 != 0)
		return node.unk0->mPosition;

	return node.unk4;
}

bool TTobiPuku::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> d(TobiPukuGetPoint(getUnk104()));
	d.x -= mPosition.x;
	d.y -= mPosition.y;
	d.z -= mPosition.z;
	d.y = 0.0f;

	if (d.x == 0.0f && d.z == 0.0f)
		return true;

	if (MsVECMag2(d) < 200.0f)
		return true;

	return false;
}

void TTobiPuku::generateEffectColumWater()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	TEffectColumWater* enemy
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);

	if (enemy)
		enemy->generate(mPosition, mScaling);

	// Coming up out of the water while the Generate nerve is still running
	// is the "from water" cue; anything else is an entry splash.
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuGenerate::theNerve())
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_TOWATER,
		                                &mPosition);
	else
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_FRWATER,
		                                &mPosition, 0, nullptr, 0, 4);
}

// TODO: 97.3%. One instruction differs: the original computes &unk104 into a
// register before the inlined getPoint, which neither the direct expression nor
// binding the result to a reference reproduces.
void TTobiPuku::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);

	if (mSpine->getCurrentNerve() == &TNerveTobiPukuAttack::theNerve())
		return;
	if (unk1AE)
		return;
	if (SMS_CheckMarioFlag(MARIO_FLAG_IN_WATER))
		return;

	JGeometry::TVec3<f32> stop(0.0f, 0.0f, 0.0f);
	mLaunchVelocity = stop;
	mSpine->pushNerve(&TNerveTobiPukuAttack::theNerve());
}

f32 TTobiPuku::getGravityY() const
{
	if (unk194)
		return unk19C->mSLFlyGravityY.get();
	return mGravity;
}

// UNUSED, 0x90 in the map and size-exact: the launch block
// TNerveTobiPukuGenerate::execute pastes in before pushing the Fly nerve.
// Rejected alternative: the Fly nerve's per-frame tail, which compiles to 0x74,
// and a `pushNerve(&TNerveTobiPukuFly::theNerve())` wrapper, which is 0xcc
// because the singleton expands inside it.
void TTobiPuku::flyStart()
{
	mBoundCount  = 0;
	unk194       = 1;
	mVelocity    = mLaunchVelocity;
	mLaunchAngle = MsGetRotFromZaxis(mVelocity).x;
	generateEffectColumWater();
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	offLiveFlag(LIVE_FLAG_UNK10);
}

// UNUSED, 0x74 in the map and size-exact: the block
// TNerveTobiPukuAttack::execute pastes in before pushing the Fall nerve --
// stop the horizontal velocity, lift clear of the ground and go airborne.
// Rejected alternative: the Fall nerve's own `getTime() == 0` head, 0x34.
void TTobiPuku::fallStart()
{
	unk194 = 0;
	JGeometry::TVec3<f32> vel(mVelocity);
	JGeometry::TVec3<f32> stop(0.0f, vel.y, 0.0f);
	mVelocity = stop;
	mPosition.y += 2.0f;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TTobiPuku::hitWater()
{
	JGeometry::TVec3<f32> vel(getVelocity());
	JGeometry::TVec3<f32> away(getPosition().x - SMS_GetMarioPos().x,
	                           getPosition().y - SMS_GetMarioPos().y,
	                           mPosition.z - SMS_GetMarioPos().z);
	if (away.x == 0.0f && away.y == 0.0f && away.z == 0.0f)
		away.x += 1.0f;

	MsVECNormalize(away, away);

	f32 push = unk19C->mSLPowerFromWater.get();
	vel.x    = away.x * push;
	vel.y    = 2.0f * (away.y * push);
	vel.z     = away.z * push;
	mVelocity = vel;

	mLaunchVelocity = vel;
	unk1B0          = mPosition.y;
	mRotation.y     = -((*gpMarioAngleY * (360.0f / 65536.0f)) - 180.0f);
}

void TTobiPuku::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD) || mGroundPlane->isIllegalData())
		return;

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuDie::theNerve()
	    || !unk1AD) {
		unk1AD = 1;
		mSpine->reset();
		mSpine->setNext(&TNerveTobiPukuDie::theNerve());
		mSpine->pushAfterCurrent(mSpine->getDefault());
	}
}

void TTobiPuku::forceKill()
{
	// TODO: the trailing isJumpBck() is evaluated and tested but its result
	// goes nowhere, like genEventCoin() above. Whatever consumed it was
	// compiled out of the retail build.
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuDie::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuPrepareFly::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuFly::theNerve())
		return;
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if (!isJumpBck())
		return;
}

void TTobiPuku::genEventCoin() { isDeadBck(); }

void TTobiPuku::changeOut()
{
	offLiveFlag(LIVE_FLAG_HIDDEN);

	// Note the direction: the puku takes the block's position when it pops
	// out, where TSmallEnemy::changeOut moves the block to the enemy.
	mPosition = mJuiceBlock->mPosition;

	gpMarioParticleManager->emitAndBindToPosPtr(0xCD, &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	getJuiceBlock()->kill();
	mJuiceBlock = nullptr;
}

void TTobiPuku::initAttacker(THitActor* param_1)
{
	mRotation = param_1->mRotation;
	mSpine->pushNerve(&TNerveTobiPukuFly::theNerve());
	unk184 = 1;
}

void TTobiPuku::scalingChangeActor()
{
	// The flying variant caps its XZ growth at a fixed 3.0 rather than at
	// TSmallEnemyManager::mBlockXZScale, which is what TSmallEnemy uses.
	f32 xzScale = MsClamp(mJuiceBlock->unk140.x + 0.02f, 0.0f, 3.0f);

	mJuiceBlock->unk140.x   = mJuiceBlock->unk140.z = xzScale;
	mJuiceBlock->mScaling.x = mJuiceBlock->mScaling.z = xzScale;

	f32 yScale              = MsClamp(mJuiceBlock->unk140.y + 0.01f, 0.0f,
	                                  TSmallEnemyManager::mBlockYScale);
	mJuiceBlock->unk140.y   = yScale;
	mJuiceBlock->mScaling.y = yScale;
}

const char** TTobiPuku::getBasNameTable() const { return pukupuku_bastable; }

TPukuPuku::TPukuPuku(const char* name)
    : TTobiPuku(name)
{
}

void TPukuPuku::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	unk1AC = 0;
}

void TPukuPuku::init(TLiveManager* manager)
{
	TTobiPuku::init(manager);
	mSpine->initWith(&TNerveTobiPukuSwimWander::theNerve());
	gpCurTobiPuku = nullptr;
}

void TPukuPuku::reset()
{
	TTobiPuku::reset();
	mSpine->initWith(&TNerveTobiPukuSwimWander::theNerve());
}

void TMoePuku::calcRootMatrix()
{
	TTobiPuku::calcRootMatrix();

	// The flame only burns while the puku is airborne on the Fly nerve.
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuFly::theNerve())
		return;

	SMSGetMSound()->startSoundActor(MSD_SE_EN_MOEKURI_FLAME, &mPosition, 0,
	                                nullptr, 0, 4);

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x1D1, getMActor()->getModel()->getAnmMtx(1), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x1D2, getMActor()->getModel()->getAnmMtx(1), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x1F8, getMActor()->getModel()->getAnmMtx(1), 3, this);
}

void TMoePuku::hitWater()
{
	TTobiPuku::hitWater();

	// The flame trails the head joint rather than the actor origin.
	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(1);
	mFlamePos.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	    0x8B, &mPosition, 0, nullptr);
	if (emitter) {
		JGeometry::TVec3<f32> scale(2.0f, 2.0f, 2.0f);
		emitter->setGlobalScale(scale);
	}

	// The burning pukupuku hitting water reuses the wanwan sizzle.
	SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition);
}

bool TMoePuku::isPichiEffect() { return isBckAnm(PUKU_ANM_PICHI) ? true : false; }

bool TMoePuku::isJumpBck() { return isBckAnm(PUKU_ANM_JUMP) ? true : false; }

BOOL TMoePuku::isDeadBck() { return isBckAnm(PUKU_ANM_DEAD) ? TRUE : FALSE; }

bool TMoePuku::isAttackBck() { return isBckAnm(PUKU_ANM_ATTACK) ? true : false; }

BOOL TMoePuku::isFallEndLandBck()
{
	return isBckAnm(PUKU_ANM_FALL_END_LAND) ? TRUE : FALSE;
}

bool TMoePuku::isJumpStartBck()
{
	return isBckAnm(PUKU_ANM_JUMP_START) ? true : false;
}

void TMoePuku::setJumpAnm() { setBckAnm(PUKU_ANM_JUMP); }

void TMoePuku::setSwimAnm() { setBckAnm(PUKU_ANM_SWIM); }

void TMoePuku::setAttackAnm() { setBckAnm(PUKU_ANM_ATTACK); }

void TMoePuku::setPichiAnm() { setBckAnm(PUKU_ANM_PICHI); }

void TMoePuku::setFallAnm() { setBckAnm(PUKU_ANM_FALL); }

void TMoePuku::setDownAirAnm() { setBckAnm(PUKU_ANM_DOWN_AIR); }

void TMoePuku::setDownLandAnm() { setBckAnm(PUKU_ANM_DOWN_LAND); }

void TMoePuku::setDeadAnm() { setBckAnm(PUKU_ANM_DEAD); }

void TMoePuku::setFallEndLandAnm() { setBckAnm(PUKU_ANM_FALL_END_LAND); }

void TMoePuku::setJumpStartAnm()
{
	if (isBckAnm(PUKU_ANM_JUMP_START))
		setBckAnm(PUKU_ANM_JUMP_START);
}

void TMoePuku::generateEffectColumWater()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	TEffectColumWater* enemy
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);

	if (enemy)
		enemy->generate(getPosition(), mScaling);

	if (mSpine->getCurrentNerve() != &TNerveTobiPukuGenerate::theNerve())
		SMSGetMSound()->startSoundActor(MSD_SE_EN_MOEPUKU_TOWATER, &mPosition,
		                                0, nullptr, 0, 4);
	else
		SMSGetMSound()->startSoundActor(MSD_SE_EN_PAKKUN_SHOOT_IMI, &mPosition,
		                                0, nullptr, 0, 4);

	// The flame gets doused: a steam puff scaled to the body.
	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0x1D4, &mPosition, 2, nullptr);
	if (emitter)
		emitter->setGlobalScale(getScaling());
}

const char** TMoePuku::getBasNameTable() const { return moepuku_bastable; }

// Binding level over spine->getBody(), worth +4 of low region and landing
// TNerveTobiPukuAttack::execute exactly (ladder 341).
static inline TTobiPuku* TobiPukuBody(TSpineBase<TLiveActor>* spine)
{
	TTobiPuku* body = (TTobiPuku*)spine->getBody();
	return body;
}

// TODO: incorrect size. Map records 0x1ac (428 bytes).
// TODO: 86.9% of 428 bytes. The structure and call order are right; what
// differs is how the two MsGetRotFromZaxis results are stored. The original
// keeps its returned vector in a stack temporary and copies a single float to
// mRotation, where this assigns the whole vector.
DEFINE_NERVE(TNerveTobiPukuGenerate, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->onLiveFlag(LIVE_FLAG_UNK10);
		puku->mPosition.y -= 300.0f;
		JGeometry::TVec3<f32> dir(puku->mVelocity);
		puku->mRotation.x = MsGetRotFromZaxis(dir).x;
		puku->setJumpAnm();
	}

	puku->mPosition.y += puku->mLaunchVelocity.y;

	if (puku->mPosition.y > puku->unk1B0) {
		puku->mBoundCount = 0;
		puku->unk194      = 1;
		puku->mVelocity   = puku->mLaunchVelocity;
		puku->mLaunchAngle = MsGetRotFromZaxis(puku->mVelocity).x;
		puku->generateEffectColumWater();
		puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		puku->offLiveFlag(LIVE_FLAG_UNK10);
		spine->pushAfterCurrent(&TNerveTobiPukuFly::theNerve());
		return TRUE;
	}
	return FALSE;
}

// Setter level around a scalar member assignment, worth +4 of low region in
// TNerveTobiPukuFly::execute (ladder 341).
static inline void TobiPukuSetFlyVelocityY(TTobiPuku* p, f32 y)
{
	p->mFlyVelocityY = y;
}

// TODO: incorrect size. Map records 0x194 (404 bytes).
DEFINE_NERVE(TNerveTobiPukuFly, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->setJumpStartAnm();
		puku->offLiveFlag(LIVE_FLAG_UNK10);
	}

	if (puku->checkCurAnmEnd(0) && puku->isJumpStartBck())
		puku->setJumpAnm();

	if (!puku->isAirborne()) {
		spine->pushAfterCurrent(&TNerveTobiPukuLand::theNerve());
		return TRUE;
	}

	JGeometry::TVec3<f32> vel(puku->getVelocity());
	TobiPukuSetFlyVelocityY(puku, vel.y);

	puku->mRotation.x
	    = MsGetRotFromZaxis(JGeometry::TVec3<f32>(puku->mVelocity)).x;
	return FALSE;
}

// TODO: incorrect size. Map records 0x198 (408 bytes).
DEFINE_NERVE(TNerveTobiPukuAttack, TLiveActor)
{
	TTobiPuku* puku = TobiPukuBody(spine);

	if (spine->getTime() == 0)
		puku->setAttackAnm();

	if (puku->isAirborne()) {
		if (puku->getCurAnmFrameNo(0) >= 6.0f) {
			puku->unk194 = 0;
			JGeometry::TVec3<f32> vel(puku->mVelocity);
			JGeometry::TVec3<f32> stop(0.0f, vel.y, 0.0f);
			puku->mVelocity = stop;
			puku->mPosition.y += 2.0f;
			puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		if (puku->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveTobiPukuFall::theNerve());
			return TRUE;
		}
	} else {
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x250 (592 bytes).
DEFINE_NERVE(TNerveTobiPukuHitWater, TLiveActor)
{
	TTobiPuku* puku = TobiPukuBody(spine);

	if (spine->getTime() == 0) {
		if (puku->isAirborne()) {
			if (puku->mPosition.y - puku->mGroundHeight > 50.0f) {
				puku->setAttackAnm();
				puku->hitWater();
			}
		} else if (puku->unk1AE != 0) {
			puku->setPichiAnm();
		}
	}

	if (!puku->isAirborne()) {
		JGeometry::TVec3<f32> away(puku->mPosition.x - SMS_GetMarioPos().x,
		                           0.0f,
		                           puku->mPosition.z - SMS_GetMarioPos().z);
		if (away.x == 0.0f && away.y == 0.0f && away.z == 0.0f)
			away.x += 1.0f;

		MsVECNormalize(away, away);
		away.y = 5.0f;
		away.x *= 5.0f;
		away.z *= 5.0f;
		puku->mVelocity = away;
		puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		puku->mPosition.y += 5.0f;
	}

	if (puku->checkCurAnmEnd(0)) {
		if (puku->isAttackBck())
			spine->pushAfterCurrent(&TNerveTobiPukuFall::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1e4 (484 bytes).
// TODO: 98.3%. The only difference is how the water-type test is grouped:
// TBGCheckData::isWaterSurface compiles to == 0x100, == 0x101, 0x102..0x105,
// == 0x4104, while the original groups 0x101..0x105 into one range. Same set of
// types, so the original's source lists them in a different order. Fixing it
// means editing the shared predicate in Map/MapData.hpp, which other matched
// callers depend on.
DEFINE_NERVE(TNerveTobiPukuFall, TLiveActor)
{
	TTobiPuku* puku = TobiPukuBody(spine);

	if (spine->getTime() == 0) {
		puku->mRotation.x = 0.0f;
		puku->setFallAnm();
	}

	if (!puku->isAirborne()) {
		if (puku->mGroundPlane->isWaterSurface()) {
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			puku->generateEffectColumWater();
			puku->onLiveFlag(LIVE_FLAG_UNK20000);
		} else {
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			puku->onLiveFlag(LIVE_FLAG_UNK20000);
		}
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x118 (280 bytes).
DEFINE_NERVE(TNerveTobiPukuPitiPiti, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0)
		puku->setPichiAnm();

	if (puku->checkCurAnmEnd(0)
	    && spine->getTime() > puku->getSaveParam2()->mSLLifeTimer.get()) {
		puku->unk1AD = 0;
		spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1fc (508 bytes).
DEFINE_NERVE(TNerveTobiPukuDie, TLiveActor)
{
	TTobiPuku* puku = TobiPukuBody(spine);

	if (spine->getTime() == 0) {
		if (puku->isAirborne()) {
			puku->mHitFlags |= HIT_FLAG_NO_COLLISION;
			JGeometry::TVec3<f32> stop(0.0f, 0.0f, 0.0f);
			JGeometry::TVec3<f32> vel(puku->mVelocity);
			stop.y          = vel.y;
			puku->mVelocity = stop;
			puku->setDownAirAnm();
		} else if (puku->unk1AD != 0) {
			puku->mHitFlags |= HIT_FLAG_NO_COLLISION;
			puku->setDownLandAnm();
		} else {
			puku->onLiveFlag(LIVE_FLAG_UNK20000);
			puku->setDeadAnm();
		}
	}

	if (puku->checkCurAnmEnd(0)) {
		puku->onLiveFlag(LIVE_FLAG_DEAD);
		puku->onLiveFlag(LIVE_FLAG_UNK8);
		puku->offLiveFlag(LIVE_FLAG_HIDDEN);
		puku->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		puku->mHolder = nullptr;
		puku->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		puku->mHitFlags |= HIT_FLAG_NO_COLLISION;
		puku->genRandomItem();
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x5a8 (1448 bytes).
// TODO: partial, 54.5% of 1448 bytes. Three of the four branches are written:
// the water landing, the bound hand-off, and the fall-end-land interpolation
// over twenty frames. The fourth, taken when TTobiPuku::mReturnLaunchSw is set,
// is **not** written. It halves the launch velocity's x and z, recomputes y
// from mFlyVelocityY against the 600-unit drop, advances mRotation.x by
// mReturnPitchStep clamped to 0..180, scales the horizontal velocity by
// cos(pitch) through the jma table, and hands off once the drop exceeds 600.
// m2c's rendering of that block is ambiguous about which component receives
// which product, so it is left out rather than guessed.
DEFINE_NERVE(TNerveTobiPukuLand, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() < 2) {
		if (puku->mGroundPlane->isWaterSurface()) {
			f32 y             = puku->mPosition.y;
			puku->mPosition.y = y - 10.0f;
			puku->onLiveFlag(LIVE_FLAG_UNK10);
			puku->generateEffectColumWater();
			if (TTobiPuku::mReturnLaunchSw) {
				puku->mFlyVelocityY *= 0.8f;
				puku->mReturnPitchStep
				    = (180.0f - puku->mRotation.x)
				      / fabsf(600.0f / puku->mFlyVelocityY);
			}
			return FALSE;
		}

		if (TTobiPuku::mBoundSw) {
			if (puku->mBoundCount < puku->unk19C->mSLBoundNum.get()) {
				spine->pushAfterCurrent(&TNerveTobiPukuBound::theNerve());
				return TRUE;
			}
			puku->unk1AE = 0;
		}

		puku->mLand[0] = puku->mPosition;
		puku->setFallEndLandAnm();
		puku->mRotation.x = 0.0f;
		return FALSE;
	}

	if (puku->isFallEndLandBck()) {
		if (spine->getTime() == 1) {
			puku->mLand[1].x = puku->mPosition.x - puku->mLand[0].x;
			puku->mLand[1].y = puku->mPosition.y - puku->mLand[0].y;
			puku->mLand[1].z = puku->mPosition.z - puku->mLand[0].z;
		}

		int time = spine->getTime();
		if (time < 20) {
			f32 t             = 0.05f * (f32)time;
			puku->mPosition.x = puku->mLand[0].x;
			puku->mPosition.y = puku->mLand[0].y;
			puku->mPosition.z = puku->mLand[0].z;
			puku->mPosition.x += puku->mLand[1].x * t;
			puku->mPosition.y += puku->mLand[1].y * t;
			puku->mPosition.z += puku->mLand[1].z * t;
		}

		if (puku->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveTobiPukuPitiPiti::theNerve());
			return TRUE;
		}
		return FALSE;
	}

	f32 y             = puku->mPosition.y;
	puku->mPosition.y = y - 12.0f;

	if (puku->isJumpBck()) {
		f32 pitch = puku->mRotation.x;
		if (pitch < TTobiPuku::mLandAngle)
			puku->mRotation.x = pitch + 1.2f;
	}

	if (spine->getTime() > 100) {
		puku->onLiveFlag(LIVE_FLAG_DEAD);
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1c8 (456 bytes).
DEFINE_NERVE(TNerveTobiPukuBound, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->unk1AE = 1;
		int count    = puku->mBoundCount;
		if (count < puku->unk19C->mSLBoundNum.get()) {
			puku->mBoundCount = count + 1;

			f32 damp = puku->unk19C->mSLBoundVal.get();
			JGeometry::TVec3<f32> vel(puku->mLaunchVelocity);
			vel.x *= damp;
			vel.z *= damp;
			vel.y = (TTobiPuku::mBoundVelocityY * damp
			         * (puku->unk1B0 - puku->mGroundHeight))
			        / 30.0f;

			puku->mLaunchVelocity = vel;
			puku->mVelocity       = vel;
			puku->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}

	JGeometry::TVec3<f32> vel(puku->mVelocity);
	if (vel.y > 0.0f)
		puku->unk1B0 = puku->mPosition.y;

	if (!puku->isAirborne()) {
		spine->pushAfterCurrent(&TNerveTobiPukuLand::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x1a8 (424 bytes).
DEFINE_NERVE(TNerveTobiPukuPrepareFly, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 angle = puku->mLaunchAngle;
		while (angle >= 360.0f)
			angle -= 360.0f;
		while (angle < 0.0f)
			angle += 360.0f;
		puku->mRotStep = (angle - puku->mRotation.x) / 60.0f;
	}

	// TODO: 99.4%. Only the volatile FPR pair is left: retail puts the
	// component in f1 and the 1/60 literal in f2, we have them the other
	// way round in all three statements.
	f32 x = puku->getPosition().x;
	puku->mPosition.x
	    = (1.0f / 60.0f) * (puku->mLaunchPad->mPosition.x - x) + x;
	f32 y = puku->getPosition().y;
	puku->mPosition.y
	    = (1.0f / 60.0f) * (puku->mLaunchPad->mPosition.y - y) + y;
	f32 z = puku->getPosition().z;
	puku->mPosition.z
	    = (1.0f / 60.0f) * (puku->mLaunchPad->mPosition.z - z) + z;

	f32 spread = puku->unk1EC - 3.0f;
	if (spread > 180.0f)
		spread = 180.0f;
	else if (spread < 0.0f)
		spread = 0.0f;
	puku->unk1EC = spread;

	puku->mRotation.x += puku->mRotStep;

	if (spine->getTime() == 50.0f)
		puku->setJumpStartAnm();

	if (spine->getTime() > 60.0f) {
		puku->mLaunchPad->forceLaunch(puku);
		puku->reset();
	}
	return FALSE;
}

// TODO: incorrect size. Map records 0x254 (596 bytes).
// TODO: 88.2% of 596 bytes. Structure and call order match. The remaining
// difference is that the original *calls* JGeometry::TVec3<f>::sub out of line
// where ours inlines its three fsubs; both use the same header inline, so what
// makes MWCC emit the call here has not been identified.
DEFINE_NERVE(TNerveTobiPukuReturnLaunch, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		TTobiPukuLaunchPad* pad = puku->mLaunchPad;
		puku->setGoalPath(pad->mPosition);
		puku->setSwimAnm();
		puku->mSwimBaseY = puku->mPosition.y;
	}

	puku->swimEffect();

	if (puku->isReachedToGoalXZ()) {
		spine->pushAfterCurrent(&TNerveTobiPukuPrepareFly::theNerve());
		return TRUE;
	}

	// Copy-initialising from `a - b` is what reaches the map's out-of-line
	// TVec3::sub: the copy constructor is one inline level and the
	// difference nested in its argument two more.
	JGeometry::TVec3<f32> toPad = puku->mLaunchPad->mPosition
	                              - puku->mPosition;

	JGeometry::TVec3<f32> dir(toPad);
	dir.y = 0.0f;
	MsVECNormalize(dir, dir);

	f32 speed = puku->mMarchSpeed;
	puku->mLaunchVelocity.x *= 0.99f;
	puku->mLaunchVelocity.z *= 0.99f;
	puku->mPosition.x += dir.x * speed - puku->mLaunchVelocity.x;
	puku->mPosition.z += dir.z * speed - puku->mLaunchVelocity.z;

	f32 spread = 1.0f + puku->unk1EC;
	if (spread > 180.0f)
		spread = 180.0f;
	else if (spread < 0.0f)
		spread = 0.0f;
	puku->unk1EC = spread;

	return FALSE;
}

// TODO: incorrect size. Map records 0xb4 (180 bytes).
DEFINE_NERVE(TNerveTobiPukuSwimWander, TLiveActor)
{
	TTobiPuku* puku = (TTobiPuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->mSwimBaseY = puku->mPosition.y;
		puku->setSwimAnm();
		puku->initialGraphNode();
		puku->mLiveFlag |= LIVE_FLAG_UNK10;
	}

	if (puku->isReachedToGoalXZ())
		puku->goToRandomNextGraphNode();

	puku->walkBehavior(0, 1.5f);
	return FALSE;
}
