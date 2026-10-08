#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Rocket.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Walker.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry/JGRotation3.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <System/Particles.hpp>

TRocketSaveLoadParams::TRocketSaveLoadParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mSLReleaseSpeed, 10.0f)
    , PARAM_INIT(mSLFlyGravity, 0.0f)
    , PARAM_INIT(mSLFlyLimitTime, 300)
{
	TParams::load(mPrmPath);
}

static const char* rocket_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

f32 TRocket::mTestAng_x     = 0.0f;
f32 TRocket::mTestAng_y     = 90.0f;
f32 TRocket::mTestAng_z     = 0.0f;
f32 TRocket::mNozzleOffsetZ = 25.0f;
f32 TRocket::mColOffsetY    = 20.0f;

TRocketManager::TRocketManager(const char* param_1)
    : TSmallEnemyManager(param_1)
    , unk60(1)
    , unk64(0)
    , unk68(nullptr)
{
}

void TRocketManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TRocketSaveLoadParams("/enemy/rocket.prm");
	unk68 = new TWaterEmitInfo("/enemy/rocketexpwater.prm");
}

void TRocketManager::loadAfter() { JDrama::TNameRef::loadAfter(); }

TSpineEnemy* TRocketManager::createEnemyInstance() { return new TRocket; }

void TRocketManager::initSetEnemies()
{
	for (int i = 0; i < mObjNum; ++i) {
		TGraphWeb* graph = gpConductor->getGraphByName("main");
		TRocket* rocket  = (TRocket*)getObj(i);
		if (rocket->checkLiveFlag(LIVE_FLAG_DEAD) && !graph->isDummy()) {
			JGeometry::TVec3<f32> position;
			graph->getGraphNode(TMsRange<s32>(0, graph->getNodeNum()).rand())
			    .getPoint(&position);
			rocket->mPosition = position;
			rocket->mPosition.y += 5.0f;
			rocket->onLiveFlag(LIVE_FLAG_AIRBORNE);
			rocket->reset();
		}
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

void TRocketManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & CUE_MOVE) {
		for (int i = 0; i < getActiveObjNum(); ++i) {
			TRocket* rocket = (TRocket*)getObj(i);
			if (rocket->checkLiveFlag(LIVE_FLAG_DEAD))
				rocket->reset();
		}
	}
	TSmallEnemyManager::perform(param_1, param_2);
}

TRocket::TRocket(const char* param_1)
    : TSmallEnemy(param_1)
    , unk1A0(0)
    , unk1A1(0)
{
}

void TRocket::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	unk194 = mPosition;
	unk1A1 = 1;
	reset();
}

void TRocket::init(TLiveManager* param_1)
{
	TSmallEnemy::init(param_1);
	mActorType = ACTOR_TYPE_ROCKET;
	unk150     = 0x11;
	unk1A4     = (TRocketSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveRocketWait::theNerve());
	onHitFilter(HIT_CATEGORY_BOSS);
}

void TRocket::calcRootMatrix()
{
	if (unk1A0) {
		getModel()->setBaseScale(mScaling);
		TPosition3f matrix;
		if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()) {
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
			offset.translation(mNozzleOffsetZ, 0.0f, 0.0f);
			MTXConcat(matrix, offset, matrix);
			mPosition.x = matrix.at(0, 3);
			mPosition.y = matrix.at(1, 3) - mColOffsetY;
			mPosition.z = matrix.at(2, 3);
		}
		Mtx rotation;
		MsMtxSetRotRPH(rotation, mTestAng_x, mTestAng_y, mTestAng_z);
		MTXConcat(matrix, rotation, matrix);
		getModel()->setBaseTRMtx(matrix);
	} else {
		TSpineEnemy::calcRootMatrix();
	}
	if (isBckAnm(1))
		SMSGetMSound()->startSoundActor(MSD_SE_PO_PETBOTTLE_FLY, &mPosition, 0,
		                                nullptr, 0, 4);
}

void TRocket::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("rocket.bmd", 3);
}

void TRocket::reset()
{
	unk1A0 = 0;
	TSmallEnemy::reset();
	if (unk1A1)
		mPosition = unk194;
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_UNK800);
	onLiveFlag(LIVE_FLAG_UNK8);
	mSpine->initWith(&TNerveRocketWait::theNerve());
}

void TRocket::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveRocketWait::theNerve()
	    && ((TRocketManager*)mManager)->unk60)
		mSpine->pushNerve(&TNerveRocketPossessedNozzle::theNerve());
}

void TRocket::behaveToWater(THitActor* param_1) { attackToMario(); }

void TRocket::bind()
{
	if (checkLiveFlag(LIVE_FLAG_UNK10))
		return;
	if (mSpine->getCurrentNerve() == &TNerveRocketPossessedNozzle::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()) {
		TBGWallCheckRecord record(mPosition.x, mPosition.y, mPosition.z,
		                          getWallRadius(), 1, 0);
		if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
			const TLiveActor* actor = record.mResultWalls[0]->getActor();
			if (actor)
				((TLiveActor*)actor)->receiveMessage(this, HIT_MESSAGE_ATTACK);
			kill();
			return;
		}
		if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()) {
			TLiveActor::bind();
			if (!isAirborne()) {
				const TLiveActor* actor = getGroundPlane()->getActor();
				if (actor)
					((TLiveActor*)actor)
					    ->receiveMessage(this, HIT_MESSAGE_ATTACK);
				kill();
			}
		}
	} else {
		TLiveActor::bind();
	}
}

void TRocket::setDeadAnm()
{
	TRocketManager* manager    = (TRocketManager*)mManager;
	manager->unk68->mPos.value = mPosition;
	gpModelWaterManager->emitRequest(*manager->unk68);
	if (unk1A0)
		releaseNozzle();
	onLiveFlag(LIVE_FLAG_UNK20000);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_PETBTL_BOMB_A, mMActor->getModel()->getBaseTRMtx(), 0,
	    nullptr);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_PETBTL_BOMB_B, mMActor->getModel()->getBaseTRMtx(), 0,
	    nullptr);
}

f32 TRocket::getGravityY() const
{
	f32 gravity = mGravity;
	if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve())
		gravity = unk1A4->mSLFlyGravity.get();
	return gravity;
}

bool TRocket::isCollidMove(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve()
	    && param_1->receiveMessage(this, HIT_MESSAGE_TRAMPLE))
		kill();
	return false;
}

void TRocket::possessedNozzle()
{
	((TRocketManager*)mManager)->unk60 = 0;
	offLiveFlag(LIVE_FLAG_UNK10);
	unk1A0 = 1;
}

void TRocket::releaseNozzle()
{
	((TRocketManager*)mManager)->unk60 = 1;
	unk1A0                             = 0;
}

bool TRocket::checkTrigger()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACH);
	if ((u8)SMSGetMarDirector()->getGamePad()->mCompSPos[3] > 20
	    && getHitPoints() > 1)
		--mHitPoints;
	if (!isBckAnm(2)) {
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_PO_WATER_FULL, 0, nullptr,
		                                   0);
		setBckAnm(2);
	}
	if (SMSGetMarDirector()->getGamePad()->mEnabledFrameMeaning
	    & TMarioGamePad::MEANING_R) {
		unk190 = 2.0f;
		expandCollision();
		SMSGetMSound()->startSoundActor(MSD_SE_PO_PETBOTTLE_FLY, &mPosition, 0,
		                                nullptr, 0, 4);
		SMSRumbleMgr->start(0x15, 5, (f32*)nullptr);
		return true;
	}
	return false;
}

void TRocket::flyBehavior()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_PETBTL_SMOKE,
	                                            &mPosition, 1, this);
	if (mSpine->getTime() > unk1A4->mSLFlyLimitTime.get())
		kill();
	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT))
		getModel();
}

bool TRocket::isAttack()
{
	if (mSpine->getCurrentNerve() == &TNerveRocketFly::theNerve())
		return true;
	return false;
}

const char** TRocket::getBasNameTable() const { return rocket_bastable; }

DEFINE_NERVE(TNerveRocketPossessedNozzle, TLiveActor)
{
	TRocket* self = (TRocket*)spine->getBody();
	if (spine->getTime() == 0) {
		SMSRumbleMgr->start(0x15, 10, (f32*)nullptr);
		SMSGetMSound()->startSoundActor(MSD_SE_MA_GET_ITEM, &self->mPosition, 0,
		                                nullptr, 0, 4);
		SMSGetMSound()->startSoundActor(MSD_SE_PO_GET_PETBOTTLE,
		                                &self->mPosition, 0, nullptr, 0, 4);
		self->possessedNozzle();
		self->setBckAnm(0);
	}
	if (self->checkTrigger()) {
		spine->pushAfterCurrent(&TNerveRocketFly::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveRocketFly, TLiveActor)
{
	TRocket* self = (TRocket*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(1);

		MtxPtr emitMtx = SMS_GetMarioWaterGun()->getEmitMtx(0);
		f32 speed      = self->unk1A4->mSLReleaseSpeed.get();
		JGeometry::TVec3<f32> velocity;
		velocity.x      = speed * emitMtx[0][0];
		velocity.y      = speed * emitMtx[1][0];
		velocity.z      = speed * emitMtx[2][0];
		self->mVelocity = velocity;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->releaseNozzle();

		f32 angle = MsGetRotFromZaxisY(velocity);
		self->mRotation.set(0.0f, MsAngleWrap(angle), 0.0f);
		self->offHitFilter(HIT_FILTER_NO_COLLISION);
	}

	if (!self->isBckAnm(1))
		self->setBckAnm(1);

	self->mRotation.x = MsGetRotFromZaxis(self->getVelocity()).x;
	self->flyBehavior();

	return false;
}

DEFINE_NERVE(TNerveRocketWait, TLiveActor)
{
	TRocket* self = (TRocket*)spine->getBody();
	if (spine->getTime() == 0) {
		self->onLiveFlag(LIVE_FLAG_UNK10);
		self->setBckAnm(3);
	}
	return false;
}
