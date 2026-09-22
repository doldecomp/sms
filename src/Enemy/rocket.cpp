#include <Enemy/Rocket.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <System/Particles.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The rocket's orientation while Mario carries it: the model is authored
// facing +X, so the root matrix gets a fixed 90 degree yaw.
f32 TRocket::mTestAng_x     = 0.0f;
f32 TRocket::mTestAng_y     = 90.0f;
f32 TRocket::mTestAng_z     = 0.0f;
f32 TRocket::mNozzleOffsetZ = 25.0f;
f32 TRocket::mColOffsetY    = 20.0f;

// TODO: the model's four .bck slots. rocket_bastable is all-null in the ROM,
// so the .bas rule gives no names; the indices are the setBckAnm arguments.
enum {
	ROCKET_ANM_POSSESSED = 0,
	ROCKET_ANM_FLY       = 1,
	ROCKET_ANM_FULL      = 2,
	ROCKET_ANM_WAIT      = 3,
};

// TODO: these particle ids have no names in include/System/Particles.hpp yet.
// This batch is only allowed to touch rocket's own two files, so they live
// here; the names are guesses from the call sites, not from the binary.
enum {
	ROCKET_JPA_MS_ROCKET_BREAK_A = 0xC1,
	ROCKET_JPA_MS_ROCKET_BREAK_B = 0xC2,
	ROCKET_JPA_MS_ROCKET_SMOKE   = 0x179,
};

static const char* rocket_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

TRocketSaveLoadParams::TRocketSaveLoadParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mSLReleaseSpeed, 10.0f)
    , PARAM_INIT(mSLFlyGravity, 0.0f)
    , PARAM_INIT(mSLFlyLimitTime, 300)
{
	TParams::load(mPrmPath);
}

TRocketManager::TRocketManager(const char* name)
    : TSmallEnemyManager(name)
    , mIsNozzleFree(1)
    , mFlyWater(nullptr)
    , mExplosionWater(nullptr)
{
}

void TRocketManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38           = new TRocketSaveLoadParams("/enemy/rocket.prm");
	mExplosionWater = new TWaterEmitInfo("/enemy/rocketexpwater.prm");
}

// Deliberately skips TSmallEnemyManager::loadAfter: the rockets are placed by
// initSetEnemies, not by the usual enemy spawn pass.
void TRocketManager::loadAfter() { JDrama::TNameRef::loadAfter(); }

TSpineEnemy* TRocketManager::createEnemyInstance() { return new TRocket(); }

// TODO: instruction-identical and the frame total is right, but the
// TMsRange lands 4 bytes low: the ROM leaves a 4-byte hole between it and
// `point`, i.e. one more 4-byte local was declared between the two.
void TRocketManager::initSetEnemies()
{
	TGraphWeb* web;
	for (int i = 0; i < mObjNum; ++i) {
		web             = gpConductor->getGraphByName("main");
		TRocket* rocket = (TRocket*)getObj(i);
		if (!rocket->checkLiveFlag(LIVE_FLAG_DEAD))
			continue;
		if (web->isDummy())
			continue;

		TMsRange<int> node(0, web->unk8);
		JGeometry::TVec3<f32> point;
		web->getGraphNode(node.rand()).getPoint((Vec*)&point);
		rocket->mPosition = point;
		rocket->mPosition.y += 5.0f;
		rocket->onLiveFlag(LIVE_FLAG_AIRBORNE);
		rocket->reset();
	}
}

void TRocketManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "rocket.bmd", 0x10040000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TRocketManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		for (int i = 0; i < getActiveObjNum(); ++i) {
			TRocket* rocket = (TRocket*)getObj(i);
			if (rocket->checkLiveFlag(LIVE_FLAG_DEAD))
				rocket->reset();
		}
	}
	TEnemyManager::perform(cue, graphics);
}

TRocket::TRocket(const char* name)
    : TSmallEnemy(name)
    , mIsPossessed(0)
    , mIsLoaded(0)
{
}

void TRocket::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	mInitialPos = mPosition;
	mIsLoaded   = 1;
	reset();
}

void TRocket::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType  = 0x1000002B;
	unk150      = 0x11;
	mSaveParams = (TRocketSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveRocketWait::theNerve());
	onHitFlag(HIT_FLAG_UNK8000000);
}

// TODO: instruction-identical; the frame is 0x110 against the ROM's 0x120,
// so 16 bytes of locals are still missing below `rot`.
void TRocket::calcRootMatrix()
{
	if (mIsPossessed) {
		getModel()->setBaseScale(mScaling);

		TPosition3f mtx;
		if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()) {
			mtx.translation(getPosition().x, getPosition().y, getPosition().z);
		} else {
			MTXCopy(SMS_GetMarioWaterGun()->getEmitMtx(0), (MtxPtr)mtx);

			JGeometry::TVec3<f32> dir[3];
			dir[0].x = mtx.ref(0, 0);
			dir[0].y = mtx.ref(1, 0);
			dir[0].z = mtx.ref(2, 0);
			f32 lenX = dir[0].length();
			dir[1].x = mtx.ref(0, 1);
			dir[1].y = mtx.ref(1, 1);
			dir[1].z = mtx.ref(2, 1);
			f32 lenY = dir[1].length();
			dir[2].x = mtx.ref(0, 2);
			dir[2].y = mtx.ref(1, 2);
			dir[2].z = mtx.ref(2, 2);
			f32 lenZ = dir[2].length();

			// The guards really are shifted by one against the divisors in
			// the ROM (popo.cpp has the identical codegen), so the original
			// mixed the three length locals up when copy-pasting the blocks.
			if (lenZ != 0.0f) {
				mtx.ref(0, 0) /= lenX;
				mtx.ref(1, 0) /= lenX;
				mtx.ref(2, 0) /= lenX;
			}
			if (lenX != 0.0f) {
				mtx.ref(0, 1) /= lenY;
				mtx.ref(1, 1) /= lenY;
				mtx.ref(2, 1) /= lenY;
			}
			if (lenY != 0.0f) {
				mtx.ref(0, 2) /= lenZ;
				mtx.ref(1, 2) /= lenZ;
				mtx.ref(2, 2) /= lenZ;
			}

			TPosition3f nozzle;
			nozzle.translation(mNozzleOffsetZ, 0.0f, 0.0f);
			MTXConcat((MtxPtr)mtx, (MtxPtr)nozzle, (MtxPtr)mtx);
			mPosition.x = mtx.ref(0, 3);
			mPosition.y = mtx.ref(1, 3) - mColOffsetY;
			mPosition.z = mtx.ref(2, 3);
		}

		Mtx rot;
		MsMtxSetRotRPH(rot, mTestAng_x, mTestAng_y, mTestAng_z);
		MTXConcat((MtxPtr)mtx, rot, (MtxPtr)mtx);
		MtxPtr rootMtx = (MtxPtr)mtx;
		getModel()->setBaseTRMtx(rootMtx);
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	if (isBckAnm(ROCKET_ANM_FLY)) {
		if (gpMSound->gateCheck(MSD_SE_PO_PETBOTTLE_FLY))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_PO_PETBOTTLE_FLY, &mPosition, 0, nullptr, 0, 4);
	}
}

void TRocket::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("rocket.bmd", 3);
}

void TRocket::reset()
{
	mIsPossessed = 0;
	TSmallEnemy::reset();
	if (mIsLoaded)
		mPosition = mInitialPos;
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_UNK800);
	onLiveFlag(LIVE_FLAG_UNK8);
	mSpine->initWith(&TNerveRocketWait::theNerve());
}

// Binding level worth +8 of low region, landing TRocket::attackToMario's
// frame at 0x40 (batch 121).
static inline TLiveManager* RocketGetManager(TRocket* p)
{
	TLiveManager* manager = p->getManager();
	return manager;
}

// TODO: instruction-identical; 8 bytes of frame short (0x38 vs 0x40).
// Naming the manager costs three instructions, so it is not that.
void TRocket::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveRocketWait::theNerve()
	    && ((TRocketManager*)RocketGetManager(this))->mIsNozzleFree)
		mSpine->pushNerve(&TNerveRocketPossessedNozzle::theNerve());
}

void TRocket::behaveToWater(THitActor* param_1) { attackToMario(); }

// TODO: 97.1%. Two residuals: the frame is 24 bytes short (0xa8 vs 0xc0),
// and the ROM copies the wall's actor into r3 inside the guarded block while
// we hoist the `mr` above the branch.
void TRocket::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if (mSpine->getCurrentNerve() == &TNerveRocketPossessedNozzle::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()) {
		TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z,
		                          getWallRadius(), 1, 0);
		if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
			// Whatever it hit gets hit back, then the bottle bursts.
			const TLiveActor* wallActor
			    = record.mResultWalls[0]->getActor();
			if (wallActor)
				((THitActor*)wallActor)
				    ->receiveMessage(this, HIT_MESSAGE_ATTACK);
			kill();
		} else if (mSpine->getCurrentNerve()
		           == &TNerveRocketFly::theNerve()) {
			TLiveActor::bind();
			if (!isAirborne()) {
				const TLiveActor* groundActor = getGroundPlane()->getActor();
				if (groundActor)
					((THitActor*)groundActor)
					    ->receiveMessage(this, HIT_MESSAGE_ATTACK);
				kill();
			}
		}
	} else {
		TLiveActor::bind();
	}
}

// TODO: 91.7%. The frame is 48 bytes short, and the dead mDir read below
// keeps its stores in our build where the ROM has the three loads alone.
void TRocket::setDeadAnm()
{
	TRocketManager* manager = (TRocketManager*)getManager();
	JGeometry::TVec3<f32> pos = mPosition;
	// TODO: the ROM reads mDir.value here and throws it away, exactly like
	// the dead half of TPopo::explosion's direction flip. Reconstructed as
	// the leftover copy; the flip itself is gone.
	JGeometry::TVec3<f32> dir(manager->mExplosionWater->mDir.value);
	manager->mExplosionWater->mPos.value = pos;
	gpModelWaterManager->emitRequest(*manager->mExplosionWater);

	if (mIsPossessed)
		releaseNozzle();

	onLiveFlag(LIVE_FLAG_UNK20000);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    ROCKET_JPA_MS_ROCKET_BREAK_A, getMActor()->getModel()->getBaseTRMtx(),
	    0, nullptr);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    ROCKET_JPA_MS_ROCKET_BREAK_B, getMActor()->getModel()->getBaseTRMtx(),
	    0, nullptr);
}

f32 TRocket::getGravityY() const
{
	f32 gravity = mGravity;
	if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve())
		gravity = mSaveParams->getSLFlyGravity();
	return gravity;
}

bool TRocket::isCollidMove(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()
	    && (param_1->checkActorType(ACTOR_TYPE_BOSS)
	        || param_1->isActorType(0x1000001F))
	    && param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE))
		kill();
	return false;
}

// UNUSED, 0x24 in the map: takes the nozzle.
void TRocket::possessedNozzle()
{
	((TRocketManager*)mManager)->mIsNozzleFree = 0;
	offLiveFlag(LIVE_FLAG_UNK10);
	mIsPossessed = 1;
}

// UNUSED, 0x18 in the map: hands the nozzle back.
void TRocket::releaseNozzle()
{
	((TRocketManager*)mManager)->mIsNozzleFree = 1;
	mIsPossessed                               = 0;
}

// UNUSED, 0x148 in the map: pumping while Mario holds the bottle. Returns
// true on the frame the trigger launches it.
bool TRocket::checkTrigger()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_UNK5);

	u8 analogR = gpMarDirector->unk18[0]->mCompSPos[3];
	if (analogR > 20) {
		if (getHitPoints() > 1)
			mHitPoints -= 1;
	}

	if (!isBckAnm(ROCKET_ANM_FULL)) {
		gpMSound->startSoundSystemSE(MSD_SE_SY_PO_WATER_FULL, 0, nullptr, 0);
		setBckAnm(ROCKET_ANM_FULL);
	}

	if (gpMarDirector->unk18[0]->checkFrameMeaning(
	        TMarioGamePad::MEANING_0x400)) {
		unk190 = 2.0f;
		expandCollision();
		gpMSound->startSoundActor(MSD_SE_PO_PETBOTTLE_FLY, &mPosition, 0,
		                          nullptr, 0, 4);
		SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
		return true;
	}
	return false;
}

// UNUSED, 0x80 in the map. These four statements are exactly 0x80 bytes, and
// TNerveRocketFly::execute runs the same sequence with the flight-time check
// in the middle of it -- so the ROM's nerve spells the body out itself and
// this copy (inherited from TPopo::flyBehavior) is dead. No contiguous slice
// of the nerve's tail is 0x80 bytes long, which is what rules out a call.
void TRocket::flyBehavior()
{
	JGeometry::TVec3<f32> vel = getVelocity();
	mRotation.x               = MsGetRotFromZaxis(vel).x;
	gpMarioParticleManager->emitAndBindToPosPtr(ROCKET_JPA_MS_ROCKET_SMOKE,
	                                            &mPosition, 1, this);

	// TODO: the ROM calls getModel() here and throws the result away; the
	// statement that used it is not recovered.
	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		getModel();
}

bool TRocket::isAttack()
{
	return mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve() ? true
	                                                                 : false;
}

const char** TRocket::getBasNameTable() const { return rocket_bastable; }

// TODO: instruction-identical; the frame is 0x40 against the ROM's 0xa0. The
// missing 96 bytes are locals of the inlined checkTrigger (which is size-exact
// against the map), so they are declarations inside it that the optimiser
// never touches.
DEFINE_NERVE(TNerveRocketPossessedNozzle, TLiveActor)
{
	TRocket* rocket = (TRocket*)spine->getBody();

	if (spine->getTime() == 0) {
		SMSRumbleMgr->start(0x15, 10, (f32*)nullptr);
		if (gpMSound->gateCheck(MSD_SE_MA_GET_ITEM))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_MA_GET_ITEM, &rocket->mPosition, 0, nullptr, 0, 4);
		if (gpMSound->gateCheck(MSD_SE_PO_GET_PETBOTTLE))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_PO_GET_PETBOTTLE, &rocket->mPosition, 0, nullptr, 0,
			    4);
		rocket->possessedNozzle();
		rocket->setBckAnm(ROCKET_ANM_POSSESSED);
	}

	if (rocket->checkTrigger()) {
		spine->pushAfterCurrent(&TNerveRocketFly::theNerve());
		return TRUE;
	}
	return FALSE;
}

// TODO: 98.9%. Every instruction matches; the frame is 0x88 against the ROM's
// 0xb8 and the MsGetRotFromZaxisY result needs an extra `fmr` into f1. Feeding
// the result straight to MsWrap removes the `fmr` but pushes
// MsGetRotFromZaxisY past its inline depth (70.2%), so the local stays.
DEFINE_NERVE(TNerveRocketFly, TLiveActor)
{
	TRocket* rocket = (TRocket*)spine->getBody();

	if (spine->getTime() == 0) {
		rocket->setBckAnm(ROCKET_ANM_FLY);
		MtxPtr emitMtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
		f32 speed      = rocket->getSaveParams()->getSLReleaseSpeed();
		JGeometry::TVec3<f32> vel;
		vel.x           = speed * emitMtx[0][0];
		vel.y           = speed * emitMtx[1][0];
		vel.z           = speed * emitMtx[2][0];
		rocket->mVelocity = vel;
		rocket->onLiveFlag(LIVE_FLAG_AIRBORNE);
		rocket->releaseNozzle();
		f32 yaw = MsGetRotFromZaxisY(vel);
		rocket->mRotation.set(0.0f, MsWrap(yaw, 0.0f, 360.0f), 0.0f);
		rocket->offHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (!rocket->isBckAnm(ROCKET_ANM_FLY))
		rocket->setBckAnm(ROCKET_ANM_FLY);

	JGeometry::TVec3<f32> vel = rocket->getVelocity();
	rocket->mRotation.x       = MsGetRotFromZaxis(vel).x;
	gpMarioParticleManager->emitAndBindToPosPtr(ROCKET_JPA_MS_ROCKET_SMOKE,
	                                            &rocket->mPosition, 1, rocket);

	if (rocket->mSpine->getTime()
	    > rocket->getSaveParams()->getSLFlyLimitTime())
		rocket->kill();

	// TODO: the ROM calls getModel() here and throws the result away; the
	// statement that used it is not recovered.
	if (!rocket->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		rocket->getModel();
	return FALSE;
}

DEFINE_NERVE(TNerveRocketWait, TLiveActor)
{
	TRocket* rocket = (TRocket*)spine->getBody();

	if (spine->getTime() == 0) {
		rocket->onLiveFlag(LIVE_FLAG_UNK10);
		rocket->setBckAnm(ROCKET_ANM_WAIT);
	}
	return FALSE;
}
