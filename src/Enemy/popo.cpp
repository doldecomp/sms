#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Popo.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JGeometry/JGRotation3.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <Player/Mario.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static TPopo* gpCurPopo;

u8 TPopo::mRollSw     = 1;
u8 TPopo::mTriggerSw  = 1;
f32 TPopo::mTestAng_x = 90.0f;
f32 TPopo::mTestAng_y = 90.0f;
f32 TPopo::mTestAng_z;
f32 TPopo::mNozzleOffsetZ = -15.0f;
u8 TPopo::mCenterJntIndex = 1;
u8 TPopo::mMouthJntIndex  = 2;
u8 TPopo::mRLegJntIndex   = 5;
u8 TPopo::mLLegJntIndex   = 11;
u8 TPopo::mRHandJntIndex  = 7;
u8 TPopo::mLHandJntIndex  = 9;
f32 TPopo::mTestBodyScale = 35.0f;
u8 TPopo::mBrkFlag        = 1;
f32 TPopo::mColOffsetY    = 20.0f;
f32 TPopo::mColMinVal     = 0.6f;
u8 TPopo::mLevelShootSw   = 1;
u8 TPopo::mExplosionSw;

static const char* popo_bastable[] = {
	"/scene/popo/bas/popo_chase.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/popo/bas/popo_jump.bas",
	"/scene/popo/bas/popo_wait.bas",
};

TPopoSaveLoadParams::TPopoSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLMoveDist, 100.0f)
    , PARAM_INIT(mSLMoveGravity, 0.1f)
    , PARAM_INIT(mSLMoveJumpSp, 10.0f)
    , PARAM_INIT(mSLAttackDist, 100.0f)
    , PARAM_INIT(mSLAttackGravity, 0.1f)
    , PARAM_INIT(mSLAttackJumpSp, 10.0f)
    , PARAM_INIT(mSLReleaseSpeed, 10.0f)
    , PARAM_INIT(mSLFlyGravity, 0.0f)
    , PARAM_INIT(mSLFlyLimitTime, 300)
    , PARAM_INIT(mSLExplosionEmitTime, 60)
    , PARAM_INIT(mSLWaterScaleMax, 2.0f)
    , PARAM_INIT(mSLThrownGravity, 0.5f)
    , PARAM_INIT(mSLPumpRate, 0.0001f)
    , PARAM_INIT(mSLLevelLimit, 1.2f)
    , PARAM_INIT(mSLScaleRate, 0.99f)
{
	TParams::load(mPrmPath);
}

TPopoManager::TPopoManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
	unk60     = 1;
	unk64     = nullptr;
	unk68     = nullptr;
	gpCurPopo = nullptr;
	unk5C     = 0;
}

void TPopoManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TPopoSaveLoadParams("/enemy/popo.prm");
	unk64 = new TWaterEmitInfo("/enemy/popowater.prm");
	unk68 = new TWaterEmitInfo("/enemy/popoexpwater.prm");
}

TSpineEnemy* TPopoManager::createEnemyInstance() { return new TPopo; }

void TPopoManager::initSetEnemies()
{
	TGraphWeb* graph = getObj(0)->getTracer()->getGraph();
	if (graph && graph->isDummy())
		return;
}

void TPopoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "popoH.bmd", 0x10020000, 0 },
		{ "popoL.bmd", 0x10020000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TPopoManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		for (int i = 0; i < getActiveObjNum(); i++) {
			TPopo* popo = (TPopo*)getObj(i);
			if (popo->unk1A4 && popo->checkLiveFlag(LIVE_FLAG_DEAD))
				popo->reset();
		}
	}

	TEnemyManager::perform(param_1, param_2);
}

BOOL TPopoCollision::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (unk68->isRollJump())
		return unk68->receiveMessage(param_1, param_2);

	return FALSE;
}

void TPopoCollision::checkHit()
{
	for (int i = 0; i < getColNum(); ++i) {
		THitActor* other = getCollision(i);
		if (other->isActorType(ACTOR_TYPE_MARIO))
			unk68->attackToMario();
		else
			unk68->behaveToHitOthers(other);
	}
}

void TPopoCollision::kill() { onHitFilter(HIT_FILTER_NO_COLLISION); }

static int PopoRollCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurPopo == nullptr)
			return 1;

		MtxPtr mtx = gpCurPopo->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());
		TPosition3f scale;
		scale.setTrans(0.0f, 0.0f, 0.0f);
		f32 bodyScale = gpCurPopo->getBodyScale();
		scale.setScale(bodyScale, bodyScale, bodyScale);

		Mtx rot;
		if (gpCurPopo->isRollJump()) {
			MsMtxSetRotX(rot, gpCurPopo->unk1B8);
		} else {
			MsMtxSetRotY(rot, 180.0f);
		}
		MTXConcat(mtx, rot, mtx);
		MTXConcat(mtx, scale, mtx);
		MTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

static int PopoPossessedCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurPopo == nullptr || !gpCurPopo->isUseScaleCallBack())
			return 1;

		f32 bodyScale = gpCurPopo->unk198;
		if (bodyScale < 1.1f)
			return 1;
		MtxPtr mtx = gpCurPopo->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());
		TPosition3f scale;
		scale.setTrans(0.0f, 0.0f, 0.0f);
		scale.setScale(bodyScale, bodyScale, bodyScale);
		MTXConcat(mtx, scale, mtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
		if (gpCurPopo->unk1BC) {
			MTXCopy(mtx, gpCurPopo->unk1D0);
			Mtx rot;
			MsMtxSetRotRPH(rot, 0.0f, 270.0f, 0.0f);
			MTXConcat(gpCurPopo->unk1D0, rot, gpCurPopo->unk1D0);

			JGeometry::TVec3<f32> axis[3];
			axis[0].x           = mtx[0][0];
			axis[0].y           = mtx[1][0];
			axis[0].z           = mtx[2][0];
			gpCurPopo->unk230.y = axis[0].length();
			axis[1].x           = mtx[0][1];
			axis[1].y           = mtx[1][1];
			axis[1].z           = mtx[2][1];
			gpCurPopo->unk230.z = axis[1].length();
			axis[2].x           = mtx[0][2];
			axis[2].y           = mtx[1][2];
			axis[2].z           = mtx[2][2];
			gpCurPopo->unk230.x = axis[2].length();

			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        PARTICLE_MS_POPO_SHUWA_A, gpCurPopo->unk1D0, 1, gpCurPopo);
			if (emitter)
				emitter->setGlobalScale(gpCurPopo->unk230);
		}
	}
	return 1;
}

static int PopoNonScaleCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurPopo == nullptr || !gpCurPopo->isUseScaleCallBack())
			return 1;

		MtxPtr mtx = gpCurPopo->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());

		TPosition3f scale;
		scale.setTrans(0.0f, 0.0f, 0.0f);
		f32 bodyScale = gpCurPopo->getBodyScale() * 0.9f;
		scale.setScale(bodyScale, bodyScale, bodyScale);

		MTXConcat(mtx, scale, mtx);
		MTXConcat(J3DSys::mCurrentMtx, scale, J3DSys::mCurrentMtx);
	}
	return 1;
}

TPopo::TPopo(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(nullptr)
    , unk198(1.0f)
    , unk19C(0)
    , unk1A0(30.0f)
    , unk1A4(0)
    , unk1B4(0)
    , unk1B8(0.0f)
    , unk1CC(0)
    , unk1CD(0)
    , unk23C(nullptr)
{
}

void TPopo::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	unk1A8 = mPosition;
	unk1A4 = 1;
	reset();
}

void TPopo::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = ACTOR_TYPE_POPO;

	if (mInstanceIndex == 0) {
		// TODO: what this loop did is unknown
		for (u8 i = 0; i < getModel()->getModelData()->getJointNum(); ++i)
			;
	}

	unk150 = 0x11;
	unk194 = (TPopoSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
	onHitFilter(HIT_CATEGORY_BOSS);
	mMActor->setJointCallback(mCenterJntIndex, PopoRollCallback);
	mMActorKeeper->getMActor("popoL.bmd")
	    ->setJointCallback(mCenterJntIndex, PopoRollCallback);
	mMActor->setJointCallback(mMouthJntIndex, PopoPossessedCallback);
	mMActor->setJointCallback(mRLegJntIndex, PopoNonScaleCallback);
	mMActor->setJointCallback(mLLegJntIndex, PopoNonScaleCallback);
	mMActor->setJointCallback(mRHandJntIndex, PopoNonScaleCallback);
	mMActor->setJointCallback(mLHandJntIndex, PopoNonScaleCallback);
	unk188 = 0.0f;

	unk23C = new TPopoCollision;
	static_cast<JDrama::TViewObjPtrListT<TPopoCollision>*>(
	    JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(unk23C);
	unk23C->initHitActor(ACTOR_TYPE_POPO, 2,
	                     HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY
	                         | HIT_CATEGORY_BOSS,
	                     80.0f, 80.0f, 80.0f, 80.0f);
	unk23C->kill();
	unk23C->unk68 = this;
}

void TPopo::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	unk23C->THitActor::perform(param_1, param_2);
}

void TPopo::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("popoH.bmd", 3);
	mMActorKeeper->createMActor("popoL.bmd", 3);
}

void TPopo::reset()
{
	gpCurPopo = this;
	TWalkerEnemy::reset();
	unk165            = false;
	unk1B4            = 0;
	unk198            = 1.0f;
	unk1B8            = 0.0f;
	unk19C            = 0;
	mScaledBodyRadius = 15.0f * getBodyRadius();
	unk190            = 0.2f;
	expandCollision();
	mMActor = mMActorKeeper->getMActor("popoL.bmd");

	if (unk1A4) {
		onLiveFlag(LIVE_FLAG_UNK10);
		mSpine->initWith(&TNervePopoWait::theNerve());
		mPosition = unk1A8;
		offLiveFlag(LIVE_FLAG_UNK800);
	}

	unk23C->kill();
	unk18C = 0;
}

bool TPopo::checkTrigger()
{
	unk1BC = 0;
	if (gpMarioOriginal->onYoshi()
	    || (int)SMS_GetMarioWaterGun()->mCurrentNozzle != 0) {
		kill();
		return false;
	}

	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACH);
	f32 maxScale = unk194->mSLWaterScaleMax.get();
	u8 pressure  = gpMarioOriginal->mGamePad->mCompSPos[3];
	if (pressure > 20) {
		unk1BC = 1;
		SMSGetMSound()->startSoundActorWithInfo(MSD_SE_EN_POPO_GROW, &mPosition,
		                                        nullptr, unk198, 0, 0, nullptr,
		                                        0, 4);
		mSprayedByWaterCooldown = 0;
		unk165                  = true;
		f32 pumpAdd             = pressure * unk194->mSLPumpRate.get();
		unk198 += pumpAdd;
		if (unk198 > maxScale) {
			unk198 = maxScale;
			if (!mBrkFlag)
				getMActor()->setFrameRate(SMSGetAnmFrameRate(), 5);
		}
		f32 brkMaxScale = unk194->mSLWaterScaleMax.get();
		if (mBrkFlag)
			mMActor->getFrameCtrl(5)->setFrame(unk1A0 * unk198 / brkMaxScale);
	}

	if ((gpMarioOriginal->mGamePad->mEnabledFrameMeaning
	     & TMarioGamePad::MEANING_R)
	    || !mTriggerSw) {
		if (mLevelShootSw)
			unk1CC = 1;
		else if (unk198 >= maxScale)
			unk1CC = 1;
	}
	if (mLevelShootSw && unk198 < maxScale - 0.1f && unk198 > 1.0f)
		unk198 *= unk194->mSLScaleRate.get();

	f32 levelLimit = unk194->mSLLevelLimit.get();
	if (pressure < 20 && (unk1CC || unk198 > levelLimit)) {
		SMSGetMSound()->startSoundActor(MSD_SE_EN_POPO_POP, &mPosition, 0,
		                                nullptr, 0, 4);
		onHitFilter(HIT_FILTER_NO_COLLISION);
		unk23C->offHitFilter(HIT_FILTER_NO_COLLISION);
		return true;
	}

	mScaledBodyRadius = (8.0f * unk198 + 8.0f) * getBodyRadius();
	if (unk198 >= maxScale)
		mMActor->getFrameCtrl(3)->setFrame(5.0f);
	return false;
}

void TPopo::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNervePopoExplosion::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNervePopoPossessedNozzle::theNerve()) {
		mSprayedByWaterCooldown = 0;
		return;
	}

	if (isAirborne()) {
		JGeometry::TVec3<f32> vel = getVelocity();
		JGeometry::TVec3<f32> dir;
		const JGeometry::TVec3<f32>& marioPos = SMS_GetMarioPos();
		dir.set(mPosition.x - marioPos.x, 0.0f, mPosition.z - marioPos.z);
		MsVECNormalize(&dir, &dir);
		dir *= 12.0f;
		dir.y = -1.0f;
		dir += vel;
		mVelocity = dir;
	} else if (mSpine->getCurrentNerve()
	           != &TNerveSmallEnemyFreeze::theNerve()) {
		mSpine->pushNerve(&TNerveSmallEnemyFreeze::theNerve());
	}
}

f32 TPopo::getGravityY() const
{
	f32 gravity = mGravity;
	if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerEscape::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveWalkerAttack::theNerve())
		return unk194->mSLMoveGravity.get();
	if (mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve())
		gravity = unk194->mSLAttackGravity.get();
	else if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve())
		gravity = unk194->mSLFlyGravity.get();
	else if (mSpine->getCurrentNerve() == &TNervePopoThrown::theNerve())
		gravity = unk194->mSLThrownGravity.get();
	return gravity;
}

void TPopo::behaveToFindMario()
{
	TPopoManager* manager = (TPopoManager*)mManager;
	if (SMS_CheckMarioFlag(MARIO_FLAG_HAS_FLUDD) && manager->unk60
	    && (int)SMS_GetMarioWaterGun()->mCurrentNozzle == 0
	    && !gpMarioOriginal->onYoshi()) {
		setGoalPath(TPathNode((THitActor*)gpMarioAddress));
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushAfterCurrent(&TNervePopoAttack::theNerve());
	} else {
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
	}
}

void TPopo::walkBehavior(int param_1, f32 param_2)
{
	if (!isAirborne()) {
		JGeometry::TVec3<f32> goal = unk104.getPoint();
		goal.set(unk104.getPoint().x - mPosition.x, 0.0f,
		         unk104.getPoint().z - mPosition.z);
		if (goal.x == 0.0f && goal.y == 0.0f && goal.z == 0.0f)
			goal.x += 1.0f;
		MsVECNormalize(&goal, &goal);

		f32 jumpDistance = unk194->mSLMoveDist.get();
		f32 jumpSpeed    = unk194->mSLMoveJumpSp.get();
		TMsRange<f32> range(-20.0f, 20.0f);
		f32 rangeScale = 1.0f;
		if (mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve()) {
			jumpSpeed    = unk194->mSLAttackJumpSp.get();
			jumpDistance = unk194->mSLAttackDist.get();
			rangeScale   = 10.0f;
			setBckAnm(0);
		}
		goal.x
		    = mPosition.x + goal.x * jumpDistance + rangeScale * range.rand();
		goal.z
		    = mPosition.z + goal.z * jumpDistance + rangeScale * range.rand();
		goal.y = mPosition.y;

		f32 jumpScale = 1.0f;
		if (mSpine->getCurrentNerve() == &TNerveWalkerEscape::theNerve()) {
			jumpScale = 1.2f;
			setBckAnm(5);
		}
		mVelocity
		    = calcVelocityToJumpToY(goal, jumpSpeed * jumpScale, getGravityY());
		mPosition.y += 2.0f;
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		if (mSpine->getCurrentNerve() == &TNerveWalkerGraphWander::theNerve()) {
			setGoalPath(TPathNode(goal));
			setBckAnm(5);
		}
	} else {
		if (mVelocity.y > 1.5f)
			mPosition.y += 0.5f * mVelocity.y;
		if (mVelocity.y < -1.0f)
			mPosition.y += 0.2f * mVelocity.y;
	}
	unk1B8 += 1.0f;
	if (mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve())
		unk1B8 += 2.0f;
	if (unk1B8 > 360.0f)
		unk1B8 -= 360.0f;
	if (!mRollSw)
		unk1B8 = 0.0f;
	if (JGeometry::TVec3<f32>(mVelocity).y > 0.0f)
		walkToCurPathNode(0.0f, getTurnSpeed(), 0.0f);
}

void TPopo::attackToMario()
{
	TPopoManager* manager = (TPopoManager*)mManager;
	if ((mSpine->getCurrentNerve() == &TNervePopoAttack::theNerve()
	     || mSpine->getCurrentNerve() == &TNervePopoWait::theNerve())
	    && manager->unk60) {
		mSpine->pushNerve(&TNervePopoPossessedNozzle::theNerve());
	} else if (mSpine->getCurrentNerve() == &TNerveWalkerEscape::theNerve()
	           || mSpine->getCurrentNerve()
	                  == &TNerveWalkerGraphWander::theNerve()) {
		sendAttackMsgToMario();
		JGeometry::TVec3<f32> positionDelta(0.0f, 0.0f, 0.0f);
		JGeometry::TVec3<f32> direction;
		const JGeometry::TVec3<f32>& marioPos = SMS_GetMarioPos();
		direction.set(mPosition.x - marioPos.x, mPosition.y - marioPos.y,
		              mPosition.z - marioPos.z);
		MsVECNormalize(&direction, &direction);
		mVelocity.x = direction.x;
		mVelocity.z = direction.z;
		direction *= getBodyRadius();
		positionDelta += direction;
		mPositionDelta = positionDelta;
	}
}

void TPopo::calcRootMatrix()
{
	gpCurPopo  = this;
	MtxPtr mtx = getModel()->getAnmMtx(mCenterJntIndex);
	unk23C->mPosition.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	if (unk1B4) {
		unk190 = 0.8f * unk198 / unk194->mSLWaterScaleMax.get();
		if (unk190 < mColMinVal)
			unk190 = mColMinVal;
		expandCollision();
		getModel()->setBaseScale(mScaling);

		TPosition3f matrix;
		if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()) {
			matrix.translation(mPosition.x, mPosition.y, mPosition.z);
		} else {
			MTXCopy(SMS_GetMarioWaterGun()->getEmitMtx(0), matrix);
			JGeometry::TVec3<f32> axes[3];
			axes[0].x   = matrix.at(0, 0);
			axes[0].y   = matrix.at(1, 0);
			axes[0].z   = matrix.at(2, 0);
			f32 xLength = axes[0].length();
			axes[1].x   = matrix.at(0, 1);
			axes[1].y   = matrix.at(1, 1);
			axes[1].z   = matrix.at(2, 1);
			f32 yLength = axes[1].length();
			axes[2].x   = matrix.at(0, 2);
			axes[2].y   = matrix.at(1, 2);
			axes[2].z   = matrix.at(2, 2);
			f32 zLength = axes[2].length();
			if (zLength != 0.0f) {
				matrix.ref(0, 0) /= xLength;
				matrix.ref(1, 0) /= xLength;
				matrix.ref(2, 0) /= xLength;
			}
			if (xLength != 0.0f) {
				matrix.ref(0, 1) /= yLength;
				matrix.ref(1, 1) /= yLength;
				matrix.ref(2, 1) /= yLength;
			}
			if (yLength != 0.0f) {
				matrix.ref(0, 2) /= zLength;
				matrix.ref(1, 2) /= zLength;
				matrix.ref(2, 2) /= zLength;
			}
			TPosition3f offset;
			offset.translation(7.0f * unk198 + mNozzleOffsetZ, 0.0f, 0.0f);
			MTXConcat(matrix, offset, matrix);
			TPosition3f body;
			body.translation(mTestBodyScale * unk198, 0.0f, 0.0f);
			MTXConcat(matrix, body, body);
			mPosition.x = body.at(0, 3);
			mPosition.y = body.at(1, 3) - mColOffsetY * unk198;
			mPosition.z = body.at(2, 3);

			if (unk1BC) {
				MtxPtr emitMtx = unk200;
				MTXCopy(mMActor->getModel()->getAnmMtx(mCenterJntIndex),
				        emitMtx);
				emitMtx[0][3] = body.at(0, 3);
				emitMtx[1][3] = body.at(1, 3);
				emitMtx[2][3] = body.at(2, 3);
				JPABaseEmitter* emitter
				    = gpMarioParticleManager->emitAndBindToMtxPtr(
				        PARTICLE_MS_POPO_SHUWA_B, emitMtx, 1, this);
				if (emitter)
					emitter->setGlobalScale(unk230);
			}
		}
		Mtx rotation;
		MsMtxSetRotRPH(rotation, mTestAng_x, mTestAng_y, mTestAng_z);
		MTXConcat(matrix, rotation, matrix);
		getModel()->setBaseTRMtx(matrix);
	} else {
		TSpineEnemy::calcRootMatrix();
	}
}

void TPopo::kill()
{
	releaseNozzle();
	TSmallEnemy::kill();
}

void TPopo::forceKill()
{
	if (!(mGroundPlane->isIllegalData()
	      || (!mGroundPlane->isDeathPlane() && !mGroundPlane->isPool()
	          && !mGroundPlane->isWaterSurface())
	      || isAirborne() || checkLiveFlag(LIVE_FLAG_UNK10))
	    || !gpMap->isInArea(mPosition.x, mPosition.z)) {

		if (mSpine->getCurrentNerve() != &TNervePopoExplosion::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNervePopoExplosion::theNerve());
			mSpine->pushAfterCurrent(mSpine->getDefault());

			onLiveFlag(LIVE_FLAG_UNK20000);
			mHitPoints = 1;
		}
	}
}

void TPopo::bind()
{
	unk23C->checkHit();
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;

	if ((mSpine->getCurrentNerve() == &TNervePopoPossessedNozzle::theNerve()
	     && unk198 > 1.2f && mExplosionSw)
	    || mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()) {
		mGroundHeight = gpMap->checkGround(
		    mPosition.x, mPosition.y + mHeadHeight, mPosition.z, &mGroundPlane);
		if (mPosition.y <= mGroundHeight + 30.0f
		    || (fabsf(JGeometry::TVec3<f32>(mVelocity).x) < 1.0f
		        && fabsf(JGeometry::TVec3<f32>(mVelocity).z) < 1.0f))
			mSpine->pushNerve(&TNervePopoExplosion::theNerve());
		TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z,
		                          unk198 * getWallRadius(), 1, 0);
		if (gpMap->isTouchedWallsAndMoveXZ(&record))
			mSpine->pushNerve(&TNervePopoExplosion::theNerve());
		else if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve())
			TLiveActor::bind();
	} else {
		TLiveActor::bind();
	}
}

bool TPopo::isHitValid(u32 param_1)
{
	if (param_1 == HIT_MESSAGE_UNKB)
		return true;

	if (param_1 == HIT_MESSAGE_TRAMPLE || param_1 == HIT_MESSAGE_HIP_DROP)
		mSpine->pushNerve(&TNervePopoExplosion::theNerve());

	return false;
}

bool TPopo::isRollJump()
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve())
		return false;
	return true;
}

bool TPopo::isUseScaleCallBack()
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()
	    || mSpine->getCurrentNerve() == &TNervePopoExplosion::theNerve()
	    || unk1B4)
		return true;
	return false;
}

bool TPopo::isFindMario(f32 param_1)
{
	if (mSpine->getTime() > 100
	    && !gpMarioOriginal->checkFlag(MARIO_FLAG_VISIBLE)) {
		TSmallEnemyParams* params = getSaveParams();
		JGeometry::TVec3<f32> marioPos(SMS_GetMarioPos().x, SMS_GetMarioPos().y,
		                               SMS_GetMarioPos().z);
		f32 searchLength = params->mSLSearchLength.get();
		f32 searchAngle  = params->mSLSearchAngle.get();
		f32 searchAware  = params->mSLSearchAware.get();
		searchLength *= param_1;
		searchAngle *= param_1;
		searchAware *= param_1;
		if (isInSight(marioPos, searchLength, searchAngle, searchAware))
			return true;
	}

	return false;
}

bool TPopo::isCollidMove(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNervePopoFly::theNerve()
	    && param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE))
		mSpine->pushNerve(&TNervePopoExplosion::theNerve());

	return false;
}

void TPopo::releaseNozzle()
{
	if (unk1B4) {
		((TPopoManager*)mManager)->unk60 = 1;
		unk1B4                           = 0;
	}
}

void TPopo::flyBehavior()
{
	unk19C++;
	if (unk19C > unk194->mSLFlyLimitTime.get()) {
		unk19C = 0;
		mSpine->pushNerve(&TNervePopoExplosion::theNerve());
	}

	if (unk198 > 1.0f)
		unk198 *= 0.999f;

	JGeometry::TVec3<f32> pos;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		pos = mPosition;
	} else {
		MtxPtr mtx = getModel()->getAnmMtx(mMouthJntIndex);
		pos.x      = mtx[0][3];
		pos.y      = mtx[1][3];
		pos.z      = mtx[2][3];
	}

	TPopoManager* manager      = (TPopoManager*)mManager;
	manager->unk64->mPos.value = pos;
	gpModelWaterManager->emitRequest(*manager->unk64);

	SMSGetMSound()->startSoundActor(MSD_SE_EN_POPO_FLY, &mPosition, 0, nullptr,
	                                0, 4);
}

void TPopo::explosion()
{
	if (unk198 > 1.0f)
		unk198 *= 0.9f;
	TPopoManager* manager         = (TPopoManager*)mManager;
	JGeometry::TVec3<f32> emitPos = mPosition;
	emitPos.y += 100.0f;
	if (mSpine->getTime() % 2 == 0) {
		JGeometry::TVec3<f32>& dir       = manager->unk68->mDir.value;
		JGeometry::TVec3<f32> flippedDir = dir;
		flippedDir.y *= -1.0f;
		dir = flippedDir;
	}
	f32 num = manager->unk68->mNum.value
	          * (unk198 / unk194->mSLWaterScaleMax.get());
	if (num < 2.0f)
		num = 2.0f;
	manager->unk68->mNum.value = (s32)num;
	manager->unk68->mPos.value = emitPos;
	gpModelWaterManager->emitRequest(*manager->unk68);
}

void TPopo::possessedIn()
{
	mMActor = mMActorKeeper->getMActor("popoH.bmd");
	setBckAnm(3);
	mMActor->setBtpFromIndex(0);
	mMActor->setFrameRate(0.0f, 3);

	if (!mExplosionSw)
		onHitFilter(HIT_FILTER_NO_COLLISION);

	mMActor->setBrkFromIndex(0);
	mMActor->getFrameCtrl(5)->setFrame(0.0f);
	unk1A0 = 30.0f;
	mMActor->setFrameRate(0.0f, 5);
	offLiveFlag(LIVE_FLAG_UNK10);
	unk1B8 = 90.0f;
	unk1B4 = 1;

	SMSGetMSound()->startSoundActor(MSD_SE_EN_POPO_STUCK, &mPosition, 0,
	                                nullptr, 0, 4);

	unk1CC = 0;
	unk1CD = 0;
}

void TPopo::explosionEffect()
{
	MtxPtr mtx = mMActor->getModel()->getAnmMtx(mCenterJntIndex);
	unk1C0.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	gpMarioParticleManager->emit(PARTICLE_MS_POPO_BOMB_A, &unk1C0, 0, nullptr);
	gpMarioParticleManager->emit(PARTICLE_MS_POPO_BOMB_B, &unk1C0, 0, nullptr);
}

void TPopo::thrownByChorobei()
{
	mSpine->initWith(&TNervePopoThrown::theNerve());
}

const char** TPopo::getBasNameTable() const { return &popo_bastable[0]; }

DEFINE_NERVE(TNervePopoPossessedNozzle, TLiveActor)
{
	TPopo* self = (TPopo*)spine->getBody();
	if (spine->getTime() == 0) {
		TPopoManager* manager = (TPopoManager*)self->mManager;
		if (manager->unk60 == 0) {
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return true;
		}
		manager->unk60 = 0;
		self->possessedIn();
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->unsetUnk165()) {
			self->setBckAnm(3);
			self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 3);
		} else {
			self->setBckAnm(4);
			self->getMActor()->getFrameCtrl(3)->setFrame(0.0f);
			self->getMActor()->setFrameRate(0.0f, 3);
		}
	}

	if (self->checkTrigger()) {
		spine->pushAfterCurrent(&TNervePopoFly::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNervePopoAttack, TLiveActor)
{
	TPopo* self = (TPopo*)spine->getBody();
	if (spine->getTime() == 0)
		self->setGoalPath(TPathNode((THitActor*)gpMarioAddress));
	if (!self->isAirborne()) {
		if (((TPopoManager*)self->mManager)->unk60 == 0)
			return true;
		if (gpMarioOriginal->checkFlag(MARIO_FLAG_VISIBLE))
			return true;
		f32 giveUpHeight = self->getSaveParam2()->mSLGiveUpHeight.get();
		if (abs(SMS_GetMarioPos().y - self->mPosition.y) > giveUpHeight)
			return true;
		if (self->isResignationAttack())
			return true;
	}
	self->walkBehavior(0, 1.0f);
	return false;
}

DEFINE_NERVE(TNervePopoFly, TLiveActor)
{
	TPopo* self = (TPopo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(2);
		MtxPtr mtx                  = SMS_GetMarioWaterGun()->getEmitMtx(0);
		TPopoSaveLoadParams* params = self->unk194;
		f32 scale                   = params->mSLReleaseSpeed.get();
		scale *= self->unk198 / params->mSLWaterScaleMax.get();
		JGeometry::TVec3<f32> vel;
		vel.x           = scale * mtx[0][0];
		vel.y           = scale * mtx[1][0];
		vel.z           = scale * mtx[2][0];
		self->mVelocity = vel;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->releaseNozzle();
		f32 angle = MsGetRotFromZaxisY(vel);
		self->mRotation.set(0.0f, MsWrap(angle, 0.0f, 360.0f), 0.0f);
		if (TPopo::mExplosionSw)
			self->offHitFilter(HIT_FILTER_NO_COLLISION);
	} else if (!self->isAirborne()) {
		spine->pushAfterCurrent(&TNervePopoExplosion::theNerve());
		return true;
	}

	if (spine->getTime() > 5) {
		self->offHitFilter(HIT_FILTER_NO_COLLISION);
		self->unk23C->offHitFilter(HIT_FILTER_NO_COLLISION);
	}
	self->flyBehavior();
	return false;
}

DEFINE_NERVE(TNervePopoExplosion, TLiveActor)
{
	TPopo* self = (TPopo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
		self->getMActor()->setFrameRate(0.0f, 0);
		self->releaseNozzle();
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->explosionEffect();
	}
	if (spine->getTime() > self->unk194->mSLExplosionEmitTime.get()) {
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		self->mHolder = nullptr;
		self->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		return true;
	}
	self->explosion();
	return false;
}

DEFINE_NERVE(TNervePopoWait, TLiveActor)
{
	TPopo* self = (TPopo*)spine->getBody();
	if (spine->getTime() == 0) {
		self->onLiveFlag(LIVE_FLAG_UNK10);
		self->setBckAnm(6);
		self->setGoalPath(TPathNode((THitActor*)gpMarioAddress));
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	return false;
}

DEFINE_NERVE(TNervePopoThrown, TLiveActor)
{
	TPopo* self = (TPopo*)spine->getBody();
	if (spine->getTime() > 30 && !self->isAirborne()) {
		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		return true;
	}
	return false;
}
