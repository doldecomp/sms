#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/TobiPuku.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/MarioFlags.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Particles.hpp>
#include <System/Params.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static TTobiPuku* gpCurTobiPuku;

static BOOL TobiPukuRollCallback(J3DNode* param_1, BOOL param_2)
{
	if (param_2 == 0) {
		TTobiPuku* puku = gpCurTobiPuku;
		if (puku != nullptr) {
			bool roll;
			if (puku->mSpine->getCurrentNerve()
			        == &TNerveTobiPukuLand::theNerve()
			    || puku->mSpine->getCurrentNerve()
			           == &TNerveTobiPukuPrepareFly::theNerve()
			    || puku->mSpine->getCurrentNerve()
			           == &TNerveTobiPukuReturnLaunch::theNerve())
				roll = true;
			else
				roll = false;
			if (!roll)
				return true;
			MtxPtr mA = gpCurTobiPuku->getMActor()->getModel()->getAnmMtx(
			    ((J3DJoint*)param_1)->getJntNo());

			Mtx mtx;
			MsMtxSetRotZ(mtx, gpCurTobiPuku->unk1EC);

			MTXConcat(mA, mtx, mA);
			MTXConcat(J3DSys::mCurrentMtx, mtx, J3DSys::mCurrentMtx);
		}
	}
	return true;
}

f32 TTobiPuku::mLandAngle       = 90.0f;
bool TTobiPuku::mBoundSw        = true;
f32 TTobiPuku::mBoundVelocityY  = 0.8f;
bool TTobiPuku::mReturnLaunchSw = true;

TTobiPukuLaunchPadSaveLoadParams::TTobiPukuLaunchPadSaveLoadParams(
    const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mSLLaunchInterval, 300)
    , PARAM_INIT(mSLLaunchVelocityY, 12.0f)
    , PARAM_INIT(mSLFlyDist, 1000.0f)
    , PARAM_INIT(mSLFlySpeed, 30.0f)
    , PARAM_INIT(mSLLaunchAngle, 45.0f)
{
	TParams::load(mPrmPath);
}

TTobiPukuSaveLoadParams::TTobiPukuSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLBoundNum, 3)
    , PARAM_INIT(mSLBoundVal, 0.8f)
    , PARAM_INIT(mSLLifeTimer, 200)
    , PARAM_INIT(mSLFlyGravityY, 0.2f)
    , PARAM_INIT(mSLPowerFromWater, 1.0f)
{
	TParams::load(mPrmPath);
}

TTobiPukuLaunchPadManager::TTobiPukuLaunchPadManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
	unk60 = 0;
}

void TTobiPukuLaunchPadManager::load(JSUMemoryInputStream& param_1)
{
	unk38 = new TTobiPukuLaunchPadSaveLoadParams("/enemy/tobipukulaunch.prm");
	TSmallEnemyManager::load(param_1);
}

TSpineEnemy* TTobiPukuLaunchPadManager::createEnemyInstance()
{
	return new TTobiPukuLaunchPad("とびプク発射台");
}

void TTobiPukuLaunchPadManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	for (int i = 0; i < getActiveObjNum(); ++i)
		TLiveManager::getObj(i)->perform(param_1, param_2);
}

TSpineEnemy* TMoePukuLaunchPadManager::createEnemyInstance()
{
	return new TMoePukuLaunchPad();
}

TTobiPukuManager::TTobiPukuManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void TTobiPukuManager::load(JSUMemoryInputStream& param_1)
{
	unk38 = new TTobiPukuSaveLoadParams("/enemy/tobipuku.prm");
	TSmallEnemyManager::load(param_1);
}

TSpineEnemy* TTobiPukuManager::createEnemyInstance()
{
	return new TTobiPuku("とびプク");
}

TSpineEnemy* TMoePukuManager::createEnemyInstance() { return new TMoePuku(); }

TTobiPukuLaunchPad::TTobiPukuLaunchPad(const char* param_1)
    : TSmallEnemy(param_1)
    , unk194(0)
    , unk19C(0.0f)
    , unk1A8(0)
{
}

void TTobiPukuLaunchPad::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!checkLiveFlag(LIVE_FLAG_UNK200) && !checkLiveFlag(LIVE_FLAG_DEAD)
	    && (param_1 & 1)) {
		if (TTobiPuku::mReturnLaunchSw) {
			if (!unk1A8)
				launch();
			else if (unk1A8->checkLiveFlag(LIVE_FLAG_DEAD))
				launch();
		} else {
			unk194++;
			if (unk194 > unk198->mSLLaunchInterval.get()) {
				unk194 = 0;
				launch();
			}
		}
	}
}

void TTobiPukuLaunchPad::init(TLiveManager* param_1)
{
	TSmallEnemy::init(param_1);
	mActorType = 0x10000012;
	unk198     = (TTobiPukuLaunchPadSaveLoadParams*)getSaveParam();
}

void TTobiPukuLaunchPad::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	s32 launchPower;
	param_1 >> launchPower;
	unk19C = launchPower;
	reset();
}

void TTobiPukuLaunchPad::reset()
{
	TSmallEnemy::reset();
	unk194 = 0;
	unk1A8 = 0;
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

void TTobiPukuLaunchPad::forceLaunch(TTobiPuku* param_1)
{
	JGeometry::TVec3<f32> pos = mPosition;
	f32 sinY                  = MsSin(mRotation.y);
	f32 cosY                  = MsCos(mRotation.y);

	JGeometry::TVec3<f32> velocity;
	if (((TTobiPukuLaunchPadManager*)mManager)->unk60) {
		f32 dist = unk198->mSLFlyDist.get();
		pos.x += sinY * dist;
		pos.z += cosY * dist;
		f32 launchVelocityY = unk198->mSLLaunchVelocityY.get();
		f32 flyGravityY     = param_1->unk19C->mSLFlyGravityY.get();
		velocity = calcVelocityToJumpToY(pos, launchVelocityY, flyGravityY);
	} else {
		velocity.set(sinY * unk19C * MsCos(mRotation.x),
		             1.0f * unk19C * MsSin(mRotation.x),
		             cosY * unk19C * MsCos(mRotation.x));
	}

	param_1->reset();
	param_1->mPosition                = mPosition;
	param_1->mRotation                = mRotation;
	param_1->unk1D0                   = velocity;
	param_1->unk1B0                   = mPosition.y;
	param_1->unk1DC                   = this;
	JGeometry::TVec3<f32> padVelocity = mVelocity;
	param_1->unk1B4                   = MsGetRotFromZaxis(padVelocity).x;
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

TTobiPuku::TTobiPuku(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(0)
    , unk198(0)
    , unk19C(0)
    , unk1AC(1)
    , unk1AD(1)
    , unk1AE(0)
    , unk1B0(0.0f)
    , unk1B4(0.0f)
    , unk1E0(0.0f)
    , unk1E4(0.0f)
    , unk1E8(0.0f)
    , unk1EC(0.0f)
{
	gpCurTobiPuku = nullptr;
}

void TTobiPuku::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000012;
	unk150     = 0x31;
	unk19C     = (TTobiPukuSaveLoadParams*)getSaveParam();
	mMActor->setJointCallback(1, TobiPukuRollCallback);
}

void TTobiPuku::reset()
{
	gpCurTobiPuku = this;
	TWalkerEnemy::reset();
	mSpine->initWith(&TNerveTobiPukuGenerate::theNerve());
	unk1AD    = 1;
	unk194    = 0;
	unk1B8[1] = getPosition();
	unk1B8[0] = unk1B8[1];
	unk1E0    = getPosition().y;
}

void TTobiPuku::moveObject()
{
	char trash[8];
	mTurnSpeed = unk19C->mSLTurnSpeedLow.get();
	if (mBoundSw && TTobiPuku::isInhibitedForceMove())
		hitWall();
	TWalkerEnemy::moveObject();
}

void TTobiPuku::hitWall()
{
	TBGWallCheckRecord record(mPosition.x, mPosition.y + mHeadHeight,
	                          mPosition.z, 1.1f * (mBodyScale * mWallRadius), 1,
	                          0);

	if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
		f32 dot = mVelocity.x * record.mResultWalls[0]->mNormal.x
		          + mVelocity.y * record.mResultWalls[0]->mNormal.y
		          + mVelocity.z * record.mResultWalls[0]->mNormal.z;
		f32 bounce = -(2.0f * dot);

		mVelocity.x += bounce * record.mResultWalls[0]->mNormal.x;
		mVelocity.y *= 0.5f;
		mVelocity.z += bounce * record.mResultWalls[0]->mNormal.z;

		unk1D0 = mVelocity;
		unk1B0 = mPosition.y;
	} else {
		const TBGCheckData* roof;
		gpMap->checkRoof(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
		                 &roof);
		if (roof && roof->mActor) {
			if (mVelocity.y > 0.0f)
				mVelocity.y = 0.0f;
		}
	}
}

void TTobiPuku::bound() { }

void TTobiPuku::calcRootMatrix()
{
	char trash[0x10];
	gpCurTobiPuku = this;
	TSpineEnemy::calcRootMatrix();
	if (mRotation.x != 0.0f) {
		if (isEaten())
			return;
		MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y + 70.0f * mRotation.x / mLandAngle,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	}
	if (isPichiEffect()) {
		unk1A0.set(mMActor->getModel()->getAnmMtx(1)[0][3],
		           mMActor->getModel()->getAnmMtx(1)[1][3],
		           mMActor->getModel()->getAnmMtx(1)[2][3]);
		gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_PUKU_PICHI,
		                                            &unk1A0, 1, this);
	}
}

bool TTobiPuku::isPichiEffect()
{
	if (isBckAnm(8))
		return true;
	return false;
}

bool TTobiPuku::isJumpBck()
{
	if (isBckAnm(6))
		return true;
	return false;
}

bool TTobiPuku::isDeadBck()
{
	if (isBckAnm(1))
		return true;
	return false;
}

bool TTobiPuku::isAttackBck()
{
	if (isBckAnm(0))
		return true;
	return false;
}

bool TTobiPuku::isFallEndLandBck()
{
	if (isBckAnm(5))
		return true;
	return false;
}

bool TTobiPuku::isJumpStartBck()
{
	if (isBckAnm(7))
		return true;
	return false;
}

void TTobiPuku::setJumpAnm() { setBckAnm(6); }

void TTobiPuku::setSwimAnm() { setBckAnm(9); }

void TTobiPuku::setAttackAnm() { setBckAnm(0); }

void TTobiPuku::setPichiAnm() { setBckAnm(8); }

void TTobiPuku::setFallAnm() { setBckAnm(4); }

void TTobiPuku::setDownAirAnm() { setBckAnm(2); }

void TTobiPuku::setDownLandAnm() { setBckAnm(3); }

void TTobiPuku::setDeadAnm() { setBckAnm(1); }

void TTobiPuku::setFallEndLandAnm() { setBckAnm(5); }

void TTobiPuku::setJumpStartAnm()
{
	if (isBckAnm(7))
		setBckAnm(7);
}

bool TTobiPuku::canBound() { return false; }

bool TTobiPuku::isRoll() { return false; }

void TTobiPuku::behaveToWater(THitActor* param_1)
{
	char trash[8];
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuHitWater::theNerve()) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_COMMON_FLY, &mPosition, 0,
		                                nullptr, 0, 4);
		mSpine->pushNerve(&TNerveTobiPukuHitWater::theNerve());
	}
}

void TTobiPuku::walkBehavior(int param_1, f32 param_2)
{
	TWalkerEnemy::walkBehavior(param_1, param_2);

	f32 prevY   = mPosition.y;
	mPosition.y = unk1E0 + 10.0f * MsSin(2.0f * (f32)mSpine->getTime());

	JGeometry::TVec3<f32> velocity = mLinearVelocity;
	velocity.y                     = prevY - mPosition.y;
	mRotation.x                    = MsGetRotFromZaxis(velocity).x;
}

void TTobiPuku::swimEffect()
{
	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_PUKU_AWA, getMActor()->getModel()->getAnmMtx(6), 1,
		    this);
		if (emitter) {
			s16 lifetime = (s16)(mGroundHeight - mPosition.y) * 16 / 100 + 20;
			if (lifetime > 200)
				lifetime = 200;
			emitter->setLifeTime(lifetime);
		}
	}
}

bool TTobiPuku::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> tmp;
	tmp = getUnk104().getPoint();
	tmp -= mPosition;
	tmp.y = 0.0f;

	if (tmp.x == 0.0f && tmp.z == 0.0f)
		return true;

	if (MsVECMag2(&tmp) < 200.0f)
		return true;
	else
		return false;
}

void TTobiPuku::generateEffectColumWater()
{
	char trash[8];
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	TEffectColumWater* eff
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);
	if (eff)
		eff->generate(mPosition, mScaling);

	if (mSpine->getCurrentNerve() != &TNerveTobiPukuGenerate::theNerve()) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_TOWATER, &mPosition,
		                                0, nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_FRWATER, &mPosition,
		                                0, nullptr, 0, 4);
	}
}

void TTobiPuku::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	if (mSpine->getCurrentNerve() != &TNerveTobiPukuAttack::theNerve()
	    && !unk1AE && !SMS_CheckMarioFlag(MARIO_FLAG_IN_WATER)) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		unk1D0 = zero;
		mSpine->pushNerve(&TNerveTobiPukuAttack::theNerve());
	}
}

f32 TTobiPuku::getGravityY() const
{
	if (unk194)
		return unk19C->mSLFlyGravityY.get();
	return mGravity;
}

void TTobiPuku::flyStart() { }

void TTobiPuku::fallStart() { }

void TTobiPuku::hitWater()
{
	JGeometry::TVec3<f32> velocity = mVelocity;

	JGeometry::TVec3<f32> dir;
	dir.set(mPosition.x - gpMarioPos->x, mPosition.y - gpMarioPos->y,
	        mPosition.z - gpMarioPos->z);
	if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
		dir.x += 1.0f;
	MsVECNormalize(&dir, &dir);

	f32 power  = unk19C->mSLPowerFromWater.get();
	velocity.x = dir.x * power;
	velocity.y = 2.0f * (dir.y * power);
	velocity.z = dir.z * power;

	mVelocity   = velocity;
	unk1D0      = velocity;
	unk1B0      = mPosition.y;
	mRotation.y = 180.0f - 0.005493164f * *gpMarioAngleY;
}

void TTobiPuku::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD) || mGroundPlane->isIllegalData())
		return;

	mHitPoints = 1;
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuDie::theNerve() && unk1AD)
		return;

	unk1AD = 1;
	mSpine->reset();
	mSpine->setNext(&TNerveTobiPukuDie::theNerve());
	mSpine->pushAfterCurrent(mSpine->getDefault());
}

// TODO: nothing follows the guard in the object.
void TTobiPuku::forceKill()
{
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuDie::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTobiPukuPrepareFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTobiPukuFly::theNerve()
	    || checkLiveFlag(LIVE_FLAG_UNK10) || !isJumpBck())
		return;
}

void TTobiPuku::genEventCoin() { isDeadBck(); }

void TTobiPuku::changeOut()
{
	char trash[8];
	offLiveFlag(LIVE_FLAG_HIDDEN);
	mPosition = mJuiceBlock->mPosition;
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
	                                            &mPosition, 0, nullptr);
	getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	mJuiceBlock->kill();
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
	f32 xzScale           = MsClamp(mJuiceBlock->unk140.x + 0.02f, 0.0f, 3.0f);
	mJuiceBlock->unk140.x = mJuiceBlock->unk140.z = xzScale;
	mJuiceBlock->mScaling.x = mJuiceBlock->mScaling.z = xzScale;

	f32 yScale              = MsClamp(mJuiceBlock->unk140.y + 0.01f, 0.0f,
	                                  TSmallEnemyManager::mBlockYScale);
	mJuiceBlock->unk140.y   = yScale;
	mJuiceBlock->mScaling.y = yScale;
}

static const char* pukupuku_bastable[] = {
	0,
	"/scene/pukupuku/bas/pukupuku_death.bas",
	"/scene/pukupuku/bas/pukupuku_down_air.bas",
	"/scene/pukupuku/bas/pukupuku_down_land.bas",
	0,
	"/scene/pukupuku/bas/pukupuku_fall_end_land.bas",
	0,
	0,
	"/scene/pukupuku/bas/pukupuku_pitipiti.bas",
	"/scene/pukupuku/bas/pukupuku_swim.bas",
	0,
};

const char** TTobiPuku::getBasNameTable() const { return pukupuku_bastable; }

TPukuPuku::TPukuPuku(const char* param_1)
    : TTobiPuku(param_1)
{
}

void TPukuPuku::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	reset();
	unk1AC = 0;
}

void TPukuPuku::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000012;
	unk150     = 0x31;
	unk19C     = (TTobiPukuSaveLoadParams*)getSaveParam();
	mMActor->setJointCallback(1, TobiPukuRollCallback);
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
	gpCurTobiPuku = this;
	TSpineEnemy::calcRootMatrix();
	if (mRotation.x != 0.0f) {
		if (isEaten())
			return;
		MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y + 70.0f * mRotation.x / mLandAngle,
		               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	}
	if (isPichiEffect()) {
		unk1A0.set(mMActor->getModel()->getAnmMtx(1)[0][3],
		           mMActor->getModel()->getAnmMtx(1)[1][3],
		           mMActor->getModel()->getAnmMtx(1)[2][3]);
		gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_PUKU_PICHI,
		                                            &unk1A0, 1, this);
	}
	if (mSpine->getCurrentNerve() == &TNerveTobiPukuFly::theNerve()) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_MOEKURI_FLAME, &mPosition, 0,
		                                nullptr, 0, 4);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MPK_FIRE_A, mMActor->getModel()->getAnmMtx(1), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MPK_FIRE_B, mMActor->getModel()->getAnmMtx(1), 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_MPK_FIRE_C, mMActor->getModel()->getAnmMtx(1), 3, this);
	}
}

void TMoePuku::hitWater()
{
	char trash[8];
	TTobiPuku::hitWater();

	MtxPtr mtx = mMActor->getModel()->getAnmMtx(1);
	unk1A0.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
	        PARTICLE_MS_MOE_FIRE_OFF, &mPosition, 0, nullptr))
		emitter->setGlobalScale(JGeometry::TVec3<f32>(2.0f, 2.0f, 2.0f));

	SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition, 0,
	                                nullptr, 0, 4);
}

bool TMoePuku::isPichiEffect()
{
	if (isBckAnm(8))
		return true;
	return false;
}

bool TMoePuku::isJumpBck()
{
	if (isBckAnm(6))
		return true;
	return false;
}

bool TMoePuku::isDeadBck()
{
	if (isBckAnm(1))
		return true;
	return false;
}

bool TMoePuku::isAttackBck()
{
	if (isBckAnm(0))
		return true;
	return false;
}

bool TMoePuku::isFallEndLandBck()
{
	if (isBckAnm(5))
		return true;
	return false;
}

bool TMoePuku::isJumpStartBck()
{
	if (isBckAnm(7))
		return true;
	return false;
}

void TMoePuku::setJumpAnm() { setBckAnm(6); }

void TMoePuku::setSwimAnm() { setBckAnm(9); }

void TMoePuku::setAttackAnm() { setBckAnm(0); }

void TMoePuku::setPichiAnm() { setBckAnm(8); }

void TMoePuku::setFallAnm() { setBckAnm(4); }

void TMoePuku::setDownAirAnm() { setBckAnm(2); }

void TMoePuku::setDownLandAnm() { setBckAnm(3); }

void TMoePuku::setDeadAnm() { setBckAnm(1); }

void TMoePuku::setFallEndLandAnm() { setBckAnm(5); }

void TMoePuku::setJumpStartAnm()
{
	if (isBckAnm(7))
		setBckAnm(7);
}

void TMoePuku::generateEffectColumWater()
{
	char trash[8];
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		return;

	TEffectColumWater* eff
	    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
	        mPosition, "エフェクト水柱マネージャー", 0);
	if (eff)
		eff->generate(mPosition, mScaling);

	if (mSpine->getCurrentNerve() != &TNerveTobiPukuGenerate::theNerve()) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_MOEPUKU_TOWATER, &mPosition,
		                                0, nullptr, 0, 4);
	} else {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_PAKKUN_SHOOT_IMI, &mPosition,
		                                0, nullptr, 0, 4);
	}

	JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	    PARTICLE_MS_M_TOBIKOMI_C, &mPosition, 2, nullptr);
	if (emitter)
		emitter->setGlobalScale(mScaling);
}

static const char* moepuku_bastable[] = {
	0,
	0,
	"/scene/moepuku/bas/moepuku_down_air.bas",
	"/scene/moepuku/bas/moepuku_down_land.bas",
	0,
	"/scene/moepuku/bas/moepuku_fall_end_land.bas",
	0,
	0,
	"/scene/moepuku/bas/moepuku_pitipiti.bas",
	0,
	0,
};

const char** TMoePuku::getBasNameTable() const { return moepuku_bastable; }

DEFINE_NERVE(TNerveTobiPukuGenerate, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->onLiveFlag(LIVE_FLAG_UNK10);
		self->mPosition.y -= 300.0f;
		JGeometry::TVec3<f32> velocity = self->mVelocity;
		self->mRotation.x              = MsGetRotFromZaxis(velocity).x;
		self->setJumpAnm();
	}

	self->mPosition.y += self->unk1D0.y;
	if (self->mPosition.y > self->unk1B0) {
		self->unk198    = 0;
		self->unk194    = 1;
		self->mVelocity = self->unk1D0;
		self->unk1B4    = MsGetRotFromZaxis(self->unk1D0).x;
		self->generateEffectColumWater();
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->offLiveFlag(LIVE_FLAG_UNK10);
		spine->pushAfterCurrent(&TNerveTobiPukuFly::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuFly, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setJumpStartAnm();
		self->offLiveFlag(LIVE_FLAG_UNK10);
	}

	if (self->checkCurAnmEnd(0) && self->isJumpStartBck())
		self->setJumpAnm();

	if (!self->isAirborne()) {
		spine->pushAfterCurrent(&TNerveTobiPukuLand::theNerve());
		return true;
	}

	JGeometry::TVec3<f32> velocity  = self->mVelocity;
	self->unk1E4                    = velocity.y;
	JGeometry::TVec3<f32> velocity2 = self->mVelocity;
	self->mRotation.x               = MsGetRotFromZaxis(velocity2).x;
	return false;
}

DEFINE_NERVE(TNerveTobiPukuAttack, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0)
		self->setAttackAnm();

	if (self->isAirborne()) {
		if (self->getCurAnmFrameNo(0) >= 6.0f) {
			self->unk194                   = 0;
			JGeometry::TVec3<f32> velocity = self->mVelocity;
			JGeometry::TVec3<f32> newVelocity;
			newVelocity.x   = 0.0f;
			newVelocity.y   = velocity.y;
			newVelocity.z   = 0.0f;
			self->mVelocity = newVelocity;
			self->mPosition.y += 2.0f;
			self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		if (self->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveTobiPukuFall::theNerve());
			return true;
		}
		return false;
	}
	return true;
}

DEFINE_NERVE(TNerveTobiPukuHitWater, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		if (self->isAirborne()) {
			if (self->mPosition.y - self->mGroundHeight > 50.0f) {
				self->setAttackAnm();
				self->hitWater();
			}
		} else if (self->unk1AE) {
			self->setPichiAnm();
		}
	}

	if (!self->isAirborne()) {
		JGeometry::TVec3<f32> dir;
		dir.set(self->mPosition.x - gpMarioPos->x, 0.0f,
		        self->mPosition.z - gpMarioPos->z);
		if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
			dir.x += 1.0f;
		MsVECNormalize(&dir, &dir);
		dir.y = 5.0f;
		dir.x *= 5.0f;
		dir.z *= 5.0f;
		self->mVelocity = dir;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->mPosition.y += 5.0f;
	}

	if (self->checkCurAnmEnd(0) && self->isAttackBck()) {
		spine->pushAfterCurrent(&TNerveTobiPukuFall::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuFall, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mRotation.x = 0.0f;
		self->setFallAnm();
	}

	if (!self->isAirborne()) {
		if (self->getGroundPlane()->isWaterSurface()) {
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			self->generateEffectColumWater();
			self->onLiveFlag(LIVE_FLAG_UNK20000);
		} else {
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			self->onLiveFlag(LIVE_FLAG_UNK20000);
		}
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuPitiPiti, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0)
		self->setPichiAnm();

	if (self->checkCurAnmEnd(0)) {
		TTobiPukuSaveLoadParams* params
		    = (TTobiPukuSaveLoadParams*)self->getSaveParams();
		if (spine->getTime() > params->mSLLifeTimer.get()) {
			self->unk1AD = 0;
			spine->pushAfterCurrent(&TNerveTobiPukuDie::theNerve());
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuDie, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		if (self->isAirborne()) {
			self->onHitFlag(HIT_FLAG_NO_COLLISION);
			JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
			JGeometry::TVec3<f32> velocity = self->mVelocity;
			zero.y                         = velocity.y;
			self->mVelocity                = zero;
			self->setDownAirAnm();
		} else if (self->unk1AD) {
			self->onHitFlag(HIT_FLAG_NO_COLLISION);
			self->setDownLandAnm();
		} else {
			self->onLiveFlag(LIVE_FLAG_UNK20000);
			self->setDeadAnm();
		}
	}

	if (self->checkCurAnmEnd(0)) {
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->offLiveFlag(LIVE_FLAG_UNK10000);
		self->mHolder = nullptr;
		self->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->genRandomItem();
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuLand, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() < 2) {
		if (self->getGroundPlane()->isWaterSurface()) {
			self->mPosition.y -= 10.0f;
			self->onLiveFlag(LIVE_FLAG_UNK10);
			self->generateEffectColumWater();
			if (TTobiPuku::mReturnLaunchSw) {
				self->unk1E4 *= 0.8f;
				self->unk1E8 = (180.0f - self->mRotation.x)
				               / fabsf(600.0f / self->unk1E4);
			}
			return false;
		} else {
			if (TTobiPuku::mBoundSw) {
				if (self->unk198 < self->unk19C->mSLBoundNum.get())
					self->unk1AE = 1;
				else
					self->unk1AE = 0;
				if (self->unk1AE) {
					spine->pushAfterCurrent(&TNerveTobiPukuBound::theNerve());
					return true;
				}
			}

			self->unk1B8[0] = self->mPosition;
			self->setFallEndLandAnm();
			self->mRotation.x = 0.0f;
		}
	} else if (self->isFallEndLandBck()) {
		if (spine->getTime() == 1) {
			f32 dx            = self->mPosition.x - self->unk1B8[0].x;
			f32 dy            = self->mPosition.y - self->unk1B8[0].y;
			f32 dz            = self->mPosition.z - self->unk1B8[0].z;
			self->unk1B8[1].x = dx;
			self->unk1B8[1].y = dy;
			self->unk1B8[1].z = dz;
		}

		if (spine->getTime() < 20) {
			f32 rate        = 0.05f * (f32)spine->getTime();
			self->mPosition = self->unk1B8[0];
			self->mPosition.x += self->unk1B8[1].x * rate;
			self->mPosition.y += self->unk1B8[1].y * rate;
			self->mPosition.z += self->unk1B8[1].z * rate;
		}

		if (self->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveTobiPukuPitiPiti::theNerve());
			return true;
		}
	} else if (TTobiPuku::mReturnLaunchSw) {
		f32 dy                         = self->unk1E0 - self->mPosition.y;
		JGeometry::TVec3<f32> velocity = self->unk1D0;
		velocity.y                     = self->unk1D0.y *= 0.5f;
		velocity.z                     = self->unk1D0.z *= 0.5f;
		// The rate below is overwritten by unk1E4 before it is read;
		// the object keeps both stores.
		velocity.y = self->unk1E4 * (600.0f - dy) / 600.0f;
		self->mRotation.x
		    = MsClamp(self->mRotation.x + self->unk1E8, 0.0f, 180.0f);
		f32 cos = MsCos(self->mRotation.x);
		velocity.x *= cos;
		velocity.z *= cos;
		velocity.y = self->unk1E4;
		self->mPosition.x += velocity.x;
		self->mPosition.y += velocity.y;
		self->mPosition.z += velocity.z;
		if (fabsf(dy) > 120.0f)
			self->unk1EC = MsClamp(self->unk1EC + 3.0f, 0.0f, 180.0f);
		if (fabsf(dy) > 600.0f) {
			spine->pushAfterCurrent(&TNerveTobiPukuReturnLaunch::theNerve());
			return true;
		}
	} else {
		self->mPosition.y -= 12.0f;
		if (self->isJumpBck() && self->mRotation.x < TTobiPuku::mLandAngle)
			self->mRotation.x += 1.2f;
		if (spine->getTime() > 100) {
			self->onLiveFlag(LIVE_FLAG_DEAD);
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuBound, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	JGeometry::TVec3<f32> velocity2;
	if (spine->getTime() == 0) {
		self->unk1AE = 1;
		if (self->unk198 < self->unk19C->mSLBoundNum.get()) {
			self->unk198++;
			f32 boundVal                   = self->unk19C->mSLBoundVal.get();
			JGeometry::TVec3<f32> velocity = self->unk1D0;
			velocity.x *= boundVal;
			velocity.z *= boundVal;
			velocity.y = TTobiPuku::mBoundVelocityY * boundVal
			             * (self->unk1B0 - self->mGroundHeight) / 30.0f;
			self->unk1D0    = velocity;
			self->mVelocity = velocity;
			self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}
	}
	char trash[0xC];

	velocity2 = self->mVelocity;
	if (velocity2.y > 0.0f)
		self->unk1B0 = self->mPosition.y;

	if (!self->isAirborne()) {
		spine->pushAfterCurrent(&TNerveTobiPukuLand::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuPrepareFly, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		f32 angle    = MsWrap(self->unk1B4, 0.0f, 360.0f);
		self->unk1F0 = (angle - self->mRotation.x) / 60.0f;
	}

	self->mPosition.x
	    += 0.016666668f * (self->unk1DC->mPosition.x - self->mPosition.x);
	self->mPosition.y
	    += 0.016666668f * (self->unk1DC->mPosition.y - self->mPosition.y);
	self->mPosition.z
	    += 0.016666668f * (self->unk1DC->mPosition.z - self->mPosition.z);

	self->unk1EC = MsClamp(self->unk1EC - 3.0f, 0.0f, 180.0f);

	self->mRotation.x += self->unk1F0;

	if ((f32)spine->getTime() == 50.0f)
		self->setJumpStartAnm();

	if ((f32)spine->getTime() > 60.0f) {
		self->unk1DC->forceLaunch(self);
		self->reset();
	}
	return false;
}

DEFINE_NERVE(TNerveTobiPukuReturnLaunch, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setGoalPath(TPathNode(self->unk1DC->mPosition));
		self->setSwimAnm();
		self->unk1E0 = self->mPosition.y;
	}

	self->swimEffect();
	if (self->isReachedToGoalXZ()) {
		spine->pushAfterCurrent(&TNerveTobiPukuPrepareFly::theNerve());
		return true;
	}

	JGeometry::TVec3<f32> diff = self->unk1DC->mPosition;
	diff.sub(self->mPosition);
	JGeometry::TVec3<f32> dir = diff;
	dir.y                     = 0.0f;
	MsVECNormalize(&dir, &dir);
	self->unk1D0.x *= 0.99f;
	self->unk1D0.z *= 0.99f;
	self->mPosition.x += dir.x * self->mMarchSpeed - self->unk1D0.x;
	self->mPosition.z += dir.z * self->mMarchSpeed - self->unk1D0.z;
	self->unk1EC = MsClamp(self->unk1EC + 1.0f, 0.0f, 180.0f);
	return false;
}

DEFINE_NERVE(TNerveTobiPukuSwimWander, TLiveActor)
{
	TTobiPuku* self = (TTobiPuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->unk1E0 = self->mPosition.y;
		self->setSwimAnm();
		self->initialGraphNode();
		self->onLiveFlag(LIVE_FLAG_UNK10);
	}

	if (self->isReachedToGoalXZ())
		self->goToRandomNextGraphNode();

	self->walkBehavior(0, 1.5f);
	return false;
}
