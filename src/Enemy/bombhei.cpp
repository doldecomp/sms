#include <Enemy/BombHei.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Map/MapData.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <Camera/CameraShake.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The two models share one animation list, so the .bck indices run across
// both of them. bombhei_bastable names slots 0, 3 and 6; the rest are guesses
// from the call sites, not from the binary.
// TODO: confirm 1, 2, 4 and 5 against the model data.
enum {
	BOMBHEI_ANM_DOWN1      = 0, // "downnejibomb_down1"
	BOMBHEI_ANM_FREEZE     = 1,
	BOMBHEI_ANM_WIND_UP    = 2,
	BOMBHEI_ANM_LAND1      = 3, // "nejibomb_land1"
	BOMBHEI_ANM_WALK       = 4,
	BOMBHEI_ANM_COUNT_WALK = 5,
	BOMBHEI_ANM_STOP_DOWN1 = 6, // "nejibomb_stop_down1"
};

static const char* bombhei_bastable[] = {
	"/scene/bombhei/bas/downnejibomb_down1.bas",
	nullptr,
	nullptr,
	"/scene/bombhei/bas/nejibomb_land1.bas",
	nullptr,
	nullptr,
	"/scene/bombhei/bas/nejibomb_stop_down1.bas",
};

bool TBombHei::mSerialBomb = true;

TBombHeiSaveLoadParams::TBombHeiSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLBombTime, 1000)
    , PARAM_INIT(mSLBombRange, 300.0f)
    , PARAM_INIT(mSLThrownVY, 50.0f)
    , PARAM_INIT(mSLThrownRateXZ, 0.5f)
    , PARAM_INIT(mSLThrownGravityY, 1.5f)
    , PARAM_INIT(mSLShootVelocity, 12.0f)
{
	TParams::load(mPrmPath);
}

TBombHeiManager::TBombHeiManager(const char* name)
    : TSmallEnemyManager(name)
    , mDeadCoinNum(0)
{
}

void TBombHeiManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TBombHeiSaveLoadParams("/enemy/bombhei.prm");
}

void TBombHeiManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "nejibomb_model1.bmd", 0x10230000, 0 },
		{ "downnejibomb_model1.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TBombHeiManager::createEnemyInstance()
{
	return new TBombHei("ボム兵");
}

// UNUSED, 0x24 in the map: caps how many coins one map's worth of bombs pays
// out.
bool TBombHeiManager::canMakeDeadCoin()
{
	if (mDeadCoinNum < 20) {
		mDeadCoinNum++;
		return true;
	}
	return false;
}

TBombHei::TBombHei(const char* name)
    : TWalkerEnemy(name)
    , mSaveParams(nullptr)
    , mFuseTimer(0)
    , mKeepDistance(true)
    , mThrownByMario(false)
{
}

void TBombHei::init(TLiveManager* live_manager)
{
	TWalkerEnemy::init(live_manager);

	mActorType = 0x1000001E;
	unk150     = 0x11;

	mSaveParams = getSaveParams();
	mSpine->initWith(&TNerveBombHeiGenerate::theNerve());

	if (mInstanceIndex == 0) {
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); i++) {
			// The original walks every joint here and does nothing with it.
		}
	}
}

void TBombHei::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("nejibomb_model1.bmd", 0);
	mMActorKeeper->createMActor("downnejibomb_model1.bmd", 3);
}

void TBombHei::behaveToWater(THitActor* water)
{
	if (isBckAnm(BOMBHEI_ANM_WALK) || isBckAnm(BOMBHEI_ANM_LAND1)) {
		if (getHitPoints() == 0)
			mSpine->pushNerve(&TNerveBombHeiWaitExplosion::theNerve());
		mSprayedByWaterCooldown = 20;
	}
}

// TODO: instruction-exact, frame 0x20 against the ROM's 0x28. Naming the
// MActor result does not add the missing two stack objects.
// TSmallEnemy::changeOut, which is the same code, is 8 bytes short in exactly
// the same way, so whatever the original wrote here it wrote there too.
void TBombHei::changeOut()
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

bool TBombHei::isHitValid(u32 message)
{
	if (message == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
		genEventCoin();
		return false;
	}
	return false;
}

void TBombHei::kill()
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		mHitPoints = 1;
		if (mSpine->getCurrentNerve()
		    != &TNerveBombHeiExplosion::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveBombHeiExplosion::theNerve());
			mSpine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
		}
		onLiveFlag(LIVE_FLAG_UNK40);
	}
}

// TODO: 99.7%. Instruction-exact since the intermediate `toMario` vector went
// away -- retail copy-initialises the one named vector straight from `a - b`
// and normalises it in place, so the second declaration was six extra
// instructions. What is left is the frame: 0x48 against the ROM's 0x50, made of
// eight bytes of pool too many (the `sub` receiver sits at 0x24, retail's at
// 0x1c) plus a sixteen-byte dead hole between that receiver and the named
// vector at 0x38 that we do not reserve -- the size of a TPathNode or a
// TQuat4 declared and never used.
// Batch 158 pinned the arithmetic: retail's pool below `dir` is 16 bytes of
// low region + the 12-byte `operator-` by-value parameter copy at 0x1c + a
// 16-byte hole, 44 bytes in all, against our 24 + 12 = 36, so it is two
// separate errors that happen to net to +8 -- we generate one 8-byte pool
// item too many *before* the parameter copy and none of the 16 after it. The
// hole sits between the parameter copy and `dir`, i.e. in expansion order it
// belongs to `operator-`'s own body or to a second *named* local declared
// after `dir` (a dead 12-byte TVec3 plus 4 of alignment fits it exactly), not
// to anything expanded later such as setVelocityAndFlag10.
void TBombHei::genEventCoin()
{
	TBombHeiManager* manager = (TBombHeiManager*)mManager;
	if (mThrownByMario && manager->canMakeDeadCoin()) {
		TMapObjBase* coin = gpItemManager->makeObjAppear(
		    mPosition.x, mPosition.y, mPosition.z, 0x2000000E, true);
		if (coin) {
			coin->mPosition.y = mPosition.y;

			// Copy-initialising from `a - b` reaches the map's
			// out-of-line TVec3::sub: the copy constructor is one
			// inline level and the difference nested in its argument
			// two more.
			JGeometry::TVec3<f32> dir = *gpMarioPos - mPosition;
			MsVECNormalize((Vec*)&dir, (Vec*)&dir);
			coin->setVelocityAndFlag10(20.0f * dir.x, 20.0f, 20.0f * dir.z);
		}
	}
}

void TBombHei::setWalkAnm() { setBckAnm(BOMBHEI_ANM_WALK); }

void TBombHei::setFreezeAnm() { setBckAnm(BOMBHEI_ANM_FREEZE); }

void TBombHei::setDeadAnm()
{
	mMActor = getActorKeeper()->getMActor("downnejibomb_model1.bmd");

	TMsRange<f32> yawRange(0.0f, 360.0f);
	mRotation.y   = yawRange.rand();
	mKeepDistance = false;
	setBckAnm(BOMBHEI_ANM_DOWN1);

	gpCameraShake->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);
	SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
}

// Two stacked binding levels over the address of mPosition, worth +16 of low
// region in TBombHei::calcRootMatrix -- one level alone moves nothing there
// (batch 130).
static inline const JGeometry::TVec3<f32>* BombheiPositionL0(const TBombHei* p)
{
	const JGeometry::TVec3<f32>* position = &p->mPosition;
	return position;
}

static inline const JGeometry::TVec3<f32>* BombheiPosition(const TBombHei* p)
{
	const JGeometry::TVec3<f32>* position = BombheiPositionL0(p);
	return position;
}

void TBombHei::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();

	if (SMSGetMarDirector()->checkUnk4CFlag(0xF)) {
		onLiveFlag(LIVE_FLAG_DEAD);
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (isBckAnm(BOMBHEI_ANM_DOWN1)
	    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(2.0f)) {
		TEffectExplosion* smoke
		    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
		        mPosition, "エフェクト爆発マネージャー", 1);
		if (smoke)
			smoke->generate(*BombheiPosition(this), mScaling);
	}
}

void TBombHei::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiExplosion::theNerve())
		SMS_SendMessageToMario(this, HIT_MESSAGE_UNKA);
}

void TBombHei::behaveToTaken(THitActor* taker)
{
	if (mSpine->getCurrentNerve() != &TNerveBombHeiPickUp::theNerve()) {
		if (taker->isActorType(0x80000001))
			mThrownByMario = true;
		mSpine->pushNerve(&TNerveBombHeiPickUp::theNerve());
	}
}

void TBombHei::behaveToRelease()
{
	if (unk164
	    || mSpine->getCurrentNerve()
	           == &TNerveBombHeiWalkExplosion::theNerve()) {
		if (mSpine->getCurrentNerve() != &TNerveBombHeiThrown::theNerve())
			mSpine->pushNerve(&TNerveBombHeiThrown::theNerve());
	}
}

// UNUSED, 0xc4 in the map: hands the bomb over to the thrown nerve. The only
// place that could have called it is behaveToRelease, but calling it from
// there puts the inlined theNerve()'s TNerveBase constructor one level too
// deep and it becomes a real `bl` the ROM does not have, so the original must
// have spelled the push out and left this helper dead. Size is exact.
void TBombHei::bombIn()
{
	mSpine->pushNerve(&TNerveBombHeiThrown::theNerve());
}

void TBombHei::reset()
{
	TWalkerEnemy::reset();

	mFuseTimer     = 0;
	mThrownByMario = false;
	unk164         = 0;
	mKeepDistance  = true;
	mMActor        = getActorKeeper()->getMActor("nejibomb_model1.bmd");
}

f32 TBombHei::getGravityY() const
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve()) {
		f32 gravity = mSaveParams->mSLThrownGravityY.get();
		return gravity;
	}
	return mGravity;
}

// TODO: instruction-exact, frame 0x28 against the ROM's 0x30. Naming the
// MSound result and taking &getPosition() were both no help (the latter is
// worse), so the missing two stack objects are still unexplained.
void TBombHei::walkBehavior(int param_1, f32 speed)
{
	SMSGetMSound()->startSoundActor(MSD_SE_EN_BOMBHEI_ZENMAI, &mPosition);
	TWalkerEnemy::walkBehavior(param_1, speed);
}

void TBombHei::moveObject()
{
	TWalkerEnemy::moveObject();

	if (mSpine->getCurrentNerve() != &TNerveBombHeiThrown::theNerve()
	    && mSpine->getCurrentNerve()
	           != &TNerveBombHeiExplosion::theNerve()) {
		if (mSpine->getCurrentNerve() != &TNerveBombHeiGenerate::theNerve()
		    && mSpine->getCurrentNerve()
		           != &TNerveSmallEnemyChange::theNerve()) {
			mFuseTimer++;
			if (mFuseTimer > mSaveParams->getSLBombTime()) {
				unk164     = 0;
				mFuseTimer = 0;
				if (mSpine->getCurrentNerve()
				    != &TNerveBombHeiWaitExplosion::theNerve())
					mSpine->pushNerve(
					    &TNerveBombHeiWalkExplosion::theNerve());
			}
		}
	}
}

bool TBombHei::isCollidMove(THitActor* other)
{
	if (mSerialBomb) {
		if (other->isActorType(0x1000001E)) {
			TBombHei* bomb = (TBombHei*)other;
			if (bomb->isExplosion()) {
				if (mSpine->getCurrentNerve()
				    != &TNerveBombHeiExplosion::theNerve())
					mSpine->pushNerve(
					    &TNerveBombHeiExplosion::theNerve());
			}
		}
	}

	if (other->isActorType(0x8000013)) {
		if (mSpine->getCurrentNerve()
		    == &TNerveBombHeiExplosion::theNerve())
			other->receiveMessage(this, HIT_MESSAGE_TRAMPLE);

		if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve())
			mSpine->pushNerve(&TNerveBombHeiExplosion::theNerve());
	}

	return true;
}

// How far MWCC folds the consecutive BG types of the inlined
// isPool()/isWaterSurface() chains is decided by the **receiver expression**,
// not by the predicate bodies: with the ground plane held in a named local it
// folds every run (0x104..0x105 and 0x100..0x105), through `getGroundPlane()`
// it peels one value off the water range, and through the raw member it peels
// two and leaves 0x104/0x105 apart -- which is the ROM's shape here and the
// shape TSmallEnemy::forceKill and TEffectEnemy::forceKill already match with
// the same raw read. Respelling the MapData.hpp predicates as
// `if (type == A) return true;` chains is refuted: it suppresses the fold
// everywhere and costs TEffectEnemy::forceKill, TPakkunSeed::forceKill and
// TSmallEnemy::forceKill, all byte-exact today.
void TBombHei::forceKill()
{
	if (mGroundPlane->isIllegalData())
		return;

	if (mGroundPlane->isDeathPlane() || mGroundPlane->isPool()
	    || mGroundPlane->isWaterSurface()) {
		if (!isAirborne() && !checkLiveFlag(LIVE_FLAG_UNK10)) {
			if (mSpine->getCurrentNerve()
			    != &TNerveBombHeiExplosion::theNerve()) {
				mSpine->reset();
				mSpine->setNext(&TNerveBombHeiExplosion::theNerve());
				mSpine->pushAfterCurrent(mSpine->getDefault());
				onLiveFlag(LIVE_FLAG_UNK20000);
				mHitPoints = 1;
			}
		}
	}
}

// UNUSED, 0x8c in the map: inlined into isCollidMove, which is the only place
// that asks another bomb whether it is already going off.
bool TBombHei::isExplosion()
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiExplosion::theNerve())
		return true;
	return false;
}

bool TBombHei::isDamageToCannon()
{
	if (mSpine->getCurrentNerve() == &TNerveBombHeiThrown::theNerve()
	    || mSpine->getCurrentNerve()
	           == &TNerveBombHeiExplosion::theNerve()) {
		onHitFlag(HIT_FLAG_NO_COLLISION);
		return true;
	}
	return false;
}

const char** TBombHei::getBasNameTable() const { return bombhei_bastable; }

// TODO: instruction-exact, frame 0x38 against the ROM's 0x58. The ROM has
// ~28 more bytes of inline-expansion temporaries than we generate; the two
// identical four-statement "switch back to the wind-up model" blocks look
// like an inline helper, but the map lists no UNUSED symbol for one.
DEFINE_NERVE(TNerveBombHeiGenerate, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0) {
		bombHei->mMActor
		    = bombHei->getActorKeeper()->getMActor("nejibomb_model1.bmd");
		bombHei->setBckAnm(BOMBHEI_ANM_WIND_UP);
		bombHei->getMActor()->setBtpFromIndex(1);
		bombHei->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
	}

	if (bombHei->getHolder())
		bombHei->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);

	if (!bombHei->isAirborne() && !bombHei->getHolder()) {
		if (bombHei->isBckAnm(BOMBHEI_ANM_WIND_UP)) {
			bombHei->setBckAnm(BOMBHEI_ANM_LAND1);
		} else if (bombHei->checkCurAnmEnd(BOMBHEI_ANM_DOWN1)) {
			spine->pushAfterCurrent(&TNerveBombHeiAttack::theNerve());
			return TRUE;
		}
	} else {
		JGeometry::TVec3<f32> velocity = bombHei->mVelocity;
		if (velocity.y > 0.0f) {
			if (!bombHei->isBckAnm(BOMBHEI_ANM_WIND_UP)) {
				bombHei->mMActor = bombHei->getActorKeeper()->getMActor(
				    "nejibomb_model1.bmd");
				bombHei->setBckAnm(BOMBHEI_ANM_WIND_UP);
				bombHei->getMActor()->setBtpFromIndex(1);
				bombHei->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
			}
		}
	}

	return FALSE;
}

// TODO: instruction-exact; the frame is the right size but every stack object
// sits 4 bytes low, so the ROM has one inline-expansion temporary more than we
// do before the TPathNode that setGoalPathMario() builds.
DEFINE_NERVE(TNerveBombHeiAttack, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0) {
		bombHei->setWalkAnm();
		bombHei->unk164 = 0;
		bombHei->setGoalPathMario();
	}

	bombHei->walkBehavior(2, 1.0f);
	return FALSE;
}

// Binding level worth +8 of low region, landing
// TNerveBombHeiWalkExplosion::execute's frame at 0x60 (batch 121).
static inline MActor* BombheiGetMActor(const TBombHei* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

// TODO: instruction-exact, frame 0x58 against the ROM's 0x60.
DEFINE_NERVE(TNerveBombHeiWalkExplosion, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0) {
		bombHei->setBckAnm(BOMBHEI_ANM_COUNT_WALK);
		BombheiGetMActor(bombHei)->setBtpFromIndex(0);
	} else if (bombHei->checkCurAnmEnd(BOMBHEI_ANM_DOWN1)) {
		spine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
		return TRUE;
	}

	// TODO: 8 bytes of frame short of the ROM with an otherwise exact body.
	// Naming this int recovered 8 of the missing 16; the last two stack
	// objects are unaccounted for.
	int btpFrame
	    = (int)bombHei->getMActor()->getFrameCtrl(ANM_TYPE_BTP)->getFrame();
	if (btpFrame % 40 == 0)
		gpMSound->startSoundActor(MSD_SE_EN_BOMBHEI_COUNT,
		                          &bombHei->mPosition, 0, nullptr, 0, 4);

	bombHei->walkBehavior(2, 0.6f);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_BOMB_LIMIT,
	    bombHei->getMActor()->getModel()->getAnmMtx(1), 1, bombHei);

	return FALSE;
}

// Binding level over a raw member read, worth +16 of low region in
// TNerveBombHeiWaitExplosion::execute (batch 127).
static inline MSound* BombheiGetMSound()
{
	MSound* mSound = gpMSound;
	return mSound;
}

// TODO: the frame is 0x68 against the ROM's 0x78.
DEFINE_NERVE(TNerveBombHeiWaitExplosion, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0) {
		bombHei->setBckAnm(BOMBHEI_ANM_STOP_DOWN1);
		bombHei->unk164 = 1;
	}

	if (bombHei->unk164) {
		if (bombHei->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(60.0f))
			bombHei->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);

		if (bombHei->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(
		        10.0f)) {
			MActor* actor = bombHei->getMActor();
			actor->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BTP);
			if (bombHei->getCurAnmFrameNo(ANM_TYPE_BTP) >= 1.0f)
				bombHei->getMActor()->setFrameRate(0.0f, ANM_TYPE_BTP);
		}
	} else {
		MActor* actor = bombHei->getMActor();
		actor->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);

		if (!bombHei->getMActor()->checkCurAnmFromIndex(0, ANM_TYPE_BTP))
			bombHei->getMActor()->setBtpFromIndex(0);

		if (bombHei->checkCurAnmEnd(BOMBHEI_ANM_DOWN1)
		    && spine->getTime() > 150) {
			spine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
			return TRUE;
		}

		int btpFrame = (int)bombHei->getMActor()
		                   ->getFrameCtrl(ANM_TYPE_BTP)
		                   ->getFrame();
		if (btpFrame % 40 == 0)
			BombheiGetMSound()->startSoundActor(MSD_SE_EN_BOMBHEI_COUNT,
			                          &bombHei->mPosition, 0, nullptr, 0, 4);

		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_BOMB_LIMIT,
		    bombHei->getMActor()->getModel()->getAnmMtx(1), 1, bombHei);
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBombHeiPickUp, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0 && bombHei->unk164 == 0)
		return TRUE;

	return FALSE;
}

DEFINE_NERVE(TNerveBombHeiThrown, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0) {
		// TODO: 96.1%. The instruction stream is right but r5/r6 are
		// swapped (the ROM allocates the cos table index before the
		// throw-power pointer) and the frame is 8 bytes too big. Naming
		// the angle in an s16 local, or assigning the three components
		// separately, are both worse (92.0% / 84.7%).
		TBombHeiSaveLoadParams* params = bombHei->getSaveParams();
		JGeometry::TVec3<f32> velocity(
		    params->getSLThrownRateXZ()
		        * (*gpMarioThrowPower * JMASSin(SMS_GetMarioAngleY())),
		    params->getSLThrownVY(),
		    params->getSLThrownRateXZ()
		        * (*gpMarioThrowPower * JMASCos(SMS_GetMarioAngleY())));
		bombHei->mVelocity = velocity;
		bombHei->mPosition.y += 2.0f;
		bombHei->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (spine->getTime() == 120)
		bombHei->offHitFlag(HIT_FLAG_NO_COLLISION);

	if (!bombHei->isAirborne()) {
		bombHei->genEventCoin();
		spine->pushAfterCurrent(&TNerveBombHeiExplosion::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveBombHeiExplosion, TLiveActor)
{
	TBombHei* bombHei = (TBombHei*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 bombRange = bombHei->getSaveParams()->getSLBombRange();
		f32 bombScale = bombRange * bombHei->getBodyScale();
		bombHei->mExplosionScaleMax = bombScale / bombHei->mAttackRadius;
		bombHei->setDeadAnm();
		bombHei->onLiveFlag(LIVE_FLAG_UNK8);

		if (bombHei->mHolder == gpMarioAddress)
			bombHei->sendAttackMsgToMario();

		bombHei->unk164 = 0;

		if (bombHei->getGroundPlane()->isWaterSurface()) {
			TEffectBombColumWater* column
			    = (TEffectBombColumWater*)gpConductor->makeOneEnemyAppear(
			        bombHei->mPosition, "エフェクト爆発水柱マネージャー", 1);
			if (column) {
				JGeometry::TVec3<f32> scaling(2.0f, 2.0f, 2.0f);
				column->generate(bombHei->mPosition, scaling);
			}
		}

		if (bombHei->getGroundPlane()->isSand()) {
			TEffectColumSand* column
			    = (TEffectColumSand*)gpConductor->makeOneEnemyAppear(
			        bombHei->mPosition, "エフェクト砂柱マネージャー", 1);
			if (column) {
				JGeometry::TVec3<f32> scaling(0.7f, 0.7f, 0.7f);
				column->generate(bombHei->mPosition, scaling);
			}
		}
	}

	if (bombHei->unk190 < bombHei->mExplosionScaleMax) {
		bombHei->unk190 *= 1.2f;
	} else if (bombHei->checkCurAnmEnd(BOMBHEI_ANM_DOWN1)) {
		bombHei->onHitFlag(HIT_FLAG_NO_COLLISION);
		bombHei->onLiveFlag(LIVE_FLAG_DEAD);
		bombHei->onLiveFlag(LIVE_FLAG_UNK8);
		bombHei->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		bombHei->mHolder = nullptr;
		bombHei->stopAnmSound();
		spine->reset();
		spine->setDefaultNext();
		spine->pushAfterCurrent(spine->getDefault());
		return TRUE;
	}

	bombHei->expandCollision();
	return FALSE;
}
