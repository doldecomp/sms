#include <Enemy/ElecNokonoko.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAttach.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <Camera/Camera.hpp>
#include <Enemy/Conductor.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <System/Particles.hpp>

void createNokonokoThunder(JGeometry::TVec3<f32> param_1) { }

TElecNokonokoSaveLoadParams::TElecNokonokoSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLReadyTime, 300)
    , PARAM_INIT(mSLCarapaceGravity, 0.01f)
    , PARAM_INIT(mSLCarapaceSpeed, 5.0f)
    , PARAM_INIT(mSLCarapaceTurnSpeed, 2.0f)
    , PARAM_INIT(mSLCarapaceSpinSpeed, 2.0f)
    , PARAM_INIT(mSLCarapaceShootRange, 500.0f)
    , PARAM_INIT(mSLCarapaceFlyDist, 100.0f)
{
	TParams::load(mPrmPath);
}

bool TElecNokonoko::mReflectSw = true;
u8 TElecNokonoko::mCarapaceJntIndex;

static const char* dennoko_bastable[] = {
	"/scene/dennoko/bas/dennoko_catch1.bas",
	"/scene/dennoko/bas/dennoko_down1.bas",
	"/scene/dennoko/bas/dennoko_elec_down1.bas",
	"/scene/dennoko/bas/dennoko_hit1.bas",
	nullptr,
	nullptr,
	"/scene/dennoko/bas/dennoko_mogaki1_loop.bas",
	"/scene/dennoko/bas/dennoko_mogaki1_start.bas",
	nullptr,
	nullptr,
	"/scene/dennoko/bas/dennoko_run1_loop.bas",
	nullptr,
	"/scene/dennoko/bas/dennoko_shoot1.bas",
	"/scene/dennoko/bas/dennoko_supply1.bas",
	nullptr,
	"/scene/dennoko/bas/dennoko_turn1_loop.bas",
	nullptr,
	nullptr,
};

TElecNokonokoManager::TElecNokonokoManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void TElecNokonokoManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TElecNokonokoSaveLoadParams("/enemy/elecNokonoko.prm");
	unk60 = J3DModelLoaderDataBase::loadMaterialTable(
	    JKRGetResource("/scene/dennoko/dennoko_model1.bmt"));
}

void TElecNokonokoManager::initSetEnemies() { }

TSpineEnemy* TElecNokonokoManager::createEnemyInstance()
{
	return new TElecNokonoko;
}

void TElecNokonokoManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "dennoko_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (2 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TElecNokonokoManager::clipEnemies(JDrama::TGraphics* param_1)
{
	f32 radius;
	f32 far;
	if (unk38 == nullptr) {
		far    = gpConductor->getCondParams().mEnemyFarClip.get();
		radius = 300.0f;
	} else {
		far    = unk38->mSLFarClip.get();
		radius = unk38->mSLClipRadius.get();
	}

	SetViewFrustumClipCheckPerspective(
	    gpCamera->getFovy(), gpCamera->getAspect(), param_1->mNearPlane, far);

	for (int i = 0; i < mObjNum; ++i) {
		TElecNokonoko* nokonoko = (TElecNokonoko*)getObj(i);

		if (ViewFrustumClipCheck(param_1, &nokonoko->mPosition, radius))
			nokonoko->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		else
			nokonoko->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);

		if (!nokonoko->unk194->isState(0)) {
			if (ViewFrustumClipCheck(param_1, &nokonoko->unk194->mPosition,
			                         radius))
				nokonoko->unk194->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
			else
				nokonoko->unk194->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TElecNokonokoManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager::perform(param_1, param_2);
	for (int i = 0; i < getActiveObjNum(); ++i) {
		((TElecNokonoko*)getObj(i))->unk194->perform(param_1, param_2);
	}
}

TElecNokonoko::TElecNokonoko(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(nullptr)
    , unk198(0)
    , unk1A4(0)
{
}

void TElecNokonoko::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = ACTOR_TYPE_ELEC_NOKONOKO;
	unk150     = 17;
	unk1A0     = (TElecNokonokoSaveLoadParams*)getSaveParam();
	unk194     = new TElecCarapace;
	mSpine->initWith(&TNerveWalkerGraphWander::theNerve());
	unk194->loadInit(this, "koura_model1.bmd");

	J3DMaterialTable* table  = ((TElecNokonokoManager*)mManager)->unk60;
	MActor* actor            = unk194->getMActor();
	J3DModelData* model_data = actor->getModel()->getModelData();
	model_data->setMaterialTable(table, J3DMatCopyFlag_All);
	actor->initDL();
	actor->getModel()->lock();
	unk19C = TMsRange<s32>(0, 300).rand();
	offHitFilter(HIT_FILTER_NO_COLLISION);
}

void TElecNokonoko::rest()
{
	TWalkerEnemy::reset();
	mScaledBodyRadius = 140.0f;
}

void TElecNokonoko::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	reset();
}

void TElecNokonoko::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("dennoko_model1.bmd", 3);

	MActor* actor            = mMActor;
	J3DMaterialTable* table  = ((TElecNokonokoManager*)mManager)->unk60;
	J3DModelData* model_data = actor->getModel()->getModelData();
	model_data->setMaterialTable(table, J3DMatCopyFlag_All);
	actor->initDL();
	actor->getModel()->lock();
}

void TElecNokonoko::moveObject()
{
	TWalkerEnemy::moveObject();
	if (isBckAnm(0xB) && checkCurAnmEnd(0))
		setBckAnm(10);
}

void TElecNokonoko::attackToMario()
{
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	TSmallEnemy::attackToMario();

	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoAttack::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoCollect::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoShoot::theNerve())
		return;

	if (!hasCarapace())
		return;

	mSpine->pushNerve(&TNerveElecNokonokoAttack::theNerve());
}

void TElecNokonoko::calcRootMatrix()
{
	TSpineEnemy::calcRootMatrix();
	if (hasCarapace()) {
		if (mSpine->getCurrentNerve() != &TNerveElecNokonokoFreeze::theNerve()
		    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
		    && mSpine->getCurrentNerve()
		           != &TNerveElecNokonokoCollect::theNerve()) {
			SMSGetMSound()->startSoundActor(MSD_SE_EN_DENNOKO_SPARK1,
			                                &mPosition, 0, nullptr, 0, 4);
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        PARTICLE_MS_DNK_BIRI, mMActor->getModel()->getAnmMtx(7), 1,
			        this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_DNK_SPARK_L, mMActor->getModel()->getAnmMtx(7), 1,
			    this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
			emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_DNK_SPARK_R, mMActor->getModel()->getAnmMtx(7), 1,
			    this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
		}
	}
	if (mCurrentBckAnm == 2) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    PARTICLE_MS_DNK_SHIBIRE_A, mMActor->getModel()->getAnmMtx(0), 1,
		    this);
		if (emitter)
			emitter->setGlobalScale(mScaling);
		MtxPtr mtx = mMActor->getModel()->getAnmMtx(8);
		unk1A8.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_DNK_HIBANA, &unk1A8, 1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);
		if (mMActor->getFrameCtrl(0)->checkPass(72.0f)) {
			emitter = gpMarioParticleManager->emitAndBindToPosPtr(
			    PARTICLE_MS_BOMB_LIMIT, &unk1A8, 1, this);
			if (emitter)
				emitter->setGlobalScale(mScaling);
		}
	}
}

void TElecNokonoko::sendAttackMsgToMario()
{
	if (unk1A4 == 0)
		SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
	else
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

BOOL TElecNokonoko::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_UNKD || param_2 == HIT_MESSAGE_UNKB) {
		onLiveFlag(LIVE_FLAG_DEAD);
		kill();
	}

	if (param_2 == HIT_MESSAGE_TAKE && mHolder == nullptr) {
		onHitFilter(HIT_FILTER_NO_COLLISION);
		mHolder = (TTakeActor*)param_1;
		return true;
	}

	if ((param_2 == HIT_MESSAGE_PUT || param_2 == HIT_MESSAGE_THROWN)
	    && mHolder == param_1) {
		mHolder = nullptr;
		return true;
	}

	if (param_2 == HIT_MESSAGE_TRAMPLE) {
		if (unk1A4 == 1) {
			mHitPoints = 1;
			kill();
			return true;
		}
		SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);
		return false;
	}

	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (!changeByJuice())
			behaveToWater(param_1);
		else
			unk194->kill();
		return true;
	}

	return false;
}

bool TElecNokonoko::isResignationAttack()
{
	f32 shootRange = unk1A0->mSLCarapaceShootRange.get();
	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && (unk104.getPoint() - mPosition).length() < shootRange) {
		mSpine->pushAfterCurrent(&TNerveElecNokonokoShoot::theNerve());
		return true;
	}
	return false;
}

void TElecNokonoko::behaveToFindMario()
{
	mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
	mSpine->pushAfterCurrent(&TNerveWalkerAttack::theNerve());
	mSpine->pushAfterCurrent(&TNerveElecNokonokoTurn::theNerve());
	setGoalPath((THitActor*)gpMarioAddress);
}

void TElecNokonoko::behaveToWater(THitActor* param_1)
{
	if (isBckAnm(12) && getCurAnmFrameNo(0) > 58.0f)
		return;

	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve()
	    || (isBckAnm(0) && unk1A4 == 0))
		return;

	unk165                  = 1;
	mSprayedByWaterCooldown = 0;

	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoFreeze::theNerve())
		return;

	mSpine->pushNerve(&TNerveElecNokonokoFreeze::theNerve());
}

void TElecNokonoko::catchIn()
{
	if (isBckAnm(8))
		setBckAnm(0);
}

void TElecNokonoko::shootIn()
{
	unk194->shoot();
	unk1A4 = 1;
}

const char** TElecNokonoko::getBasNameTable() const { return dennoko_bastable; }

void TElecNokonoko::setWaitAnm()
{
	unk198 = 0;
	setBckAnm(0x11);
}

void TElecNokonoko::setWalkAnm()
{
	if (!isBckAnm(10))
		setBckAnm(11);
}

void TElecNokonoko::setRunAnm()
{
	if (!isBckAnm(10))
		setBckAnm(11);
}

void TElecNokonoko::setDeadAnm() { setBckAnm(1); }

void TElecNokonoko::setMeltAnm()
{
	setBckAnm(2);
	onLiveFlag(LIVE_FLAG_UNK8);

	JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
	unk18C            = 3;
	unk194->mVelocity = zero;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_SHIBIRE_B, mMActor->getModel()->getAnmMtx(0), 0,
	    nullptr);
	if (emitter != nullptr)
		emitter->setGlobalScale(mScaling);
}

void TElecNokonoko::genRandomItem()
{
	unk194->kill();
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
	                                            &mPosition, 0, nullptr);
	TSmallEnemy::genRandomItem();
	if (checkLiveFlag(LIVE_FLAG_MELT_ON_DEATH)) {
		TMapObjBase* item = gpItemManager->makeObjAppear(
		    unk194->mPosition.x, unk194->mPosition.y, unk194->mPosition.z,
		    ACTOR_TYPE_COIN, true);
		if (item != nullptr) {
			item->mVelocity.set(0.0f, 20.0f, 0.0f);
			item->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}
}

bool TElecNokonoko::isShootReady() { return false; }

bool TElecNokonoko::isCatchReady()
{
	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoCollect::theNerve())
		return true;
	return false;
}

void TElecNokonoko::forceCatchReady()
{
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveElecNokonokoFreeze::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveElecNokonokoCollect::theNerve())
		mSpine->setNext(&TNerveElecNokonokoCollect::theNerve());
}

bool TElecNokonoko::isDeadByThunder()
{
	if (mSpine->getCurrentNerve() == &TNerveElecNokonokoFreeze::theNerve()) {
		JGeometry::TVec3<f32> toCarapace = mPosition - unk194->mPosition;
		if (VECMag(&toCarapace) < 200.0f)
			return true;
	}
	return false;
}

void TElecNokonoko::recoverCarapace() { }

TElecCarapace::TElecCarapace(const char* param_1)
    : TEnemyAttachment(param_1)
    , unk16C(nullptr)
    , unk170(nullptr)
    , unk174(1)
    , unk175(0)
    , unk176(0)
    , unk178(0.0f)
    , unk17C(0.0f)
    , unk180(0)
    , unk184(0)
    , unk188(0.0f)
    , unk198(0.0f)
{
}

void TElecCarapace::loadInit(TSpineEnemy* param_1, const char* param_2)
{
	TEnemyAttachment::loadInit(param_1, param_2);
	unk16C = (TElecNokonoko*)unk160;
	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);
	initHitActor(ACTOR_TYPE_ELEC_CARAPACE, 3,
	             HIT_CATEGORY_PLAYER | HIT_CATEGORY_ENEMY | HIT_CATEGORY_BOSS,
	             80.0f, 80.0f, 60.0f, 60.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	unk150 = 0;
	mSpine->initWith(&TNerveElecCarapaceMove::theNerve());
	if (TMsRange<s32>(0, 300).rand() < 150)
		unk174 = 0;
	mHeadHeight = 80.0f;
}

void TElecCarapace::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyAttachment::perform(param_1, param_2);

	if (param_1 & 1) {
		if (unk180 != 0) {
			unk180++;
			if (unk180 > 5)
				unk180 = 0;
		}
	}

	if (param_1 & 0x200) {
		if (isState(0) || unk16C->checkLiveFlag(LIVE_FLAG_DEAD)
		    || checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN))
			return;

		TCircleShadowRequest request;

		request.mPosition = mPosition;

		if (!isAirborne()) {
			request.mPosition.y       = mGroundHeight;
			request.mNeedsGroundCheck = false;
		}

		request.mRadiusX = request.mRadiusZ = unk16C->mScaledBodyRadius;
		request.mRotationY                  = mRotation.y;

		gpBindShadowManager->request(request, getActorType());
	}
}

void TElecCarapace::setBehavior()
{
	if (unk16C->checkLiveFlag(LIVE_FLAG_DEAD)) {
		kill();
	}
	if (unk168) {
		mPosition.y = mGroundHeight;
	}
	unk168 = 0;
}

void TElecCarapace::behaveToHitGround()
{
	if (unk176 != 0)
		unk184 = 1;
	if (mGroundPlane->isWaterSurface())
		kill();
	unk176 = 0;
	unk168 = 1;
	offLiveFlag(LIVE_FLAG_AIRBORNE);
	mVelocity.set(0.0f, 0.0f, 0.0f);
}

void TElecCarapace::kill() { TEnemyAttachment::kill(); }

void TElecCarapace::behaveToHitWall(const TBGCheckData* param_1)
{
	if (unk180 <= 0) {
		if (TElecNokonoko::mReflectSw) {
			unk180      = 1;
			unk184      = 0;
			unk176      = 1;
			unk175      = 1;
			f32 speed   = mPositionDelta.dot(param_1->getNormal());
			mVelocity.x = -1.5f * speed * param_1->getNormal().x;
			mVelocity.y = 3.0f;
			mVelocity.z = -1.5f * speed * param_1->getNormal().z;
			mPosition.y = mGroundHeight + 2.0f;
			setGoalPath(unk16C->mPosition);
		}
	}
}

f32 TElecCarapace::getNowGravity()
{
	return ((TElecNokonokoSaveLoadParams*)unk16C->getSaveParam())
	    ->mSLCarapaceGravity.get();
}

void TElecCarapace::appear()
{
	if (unk150 != 0)
		return;
	unk150     = 1;
	mPosition  = unk16C->mPosition;
	f32 scale  = unk16C->mScaling.x;
	unk164     = scale;
	mScaling.x = mScaling.y = mScaling.z = scale;
	mBodyRadius                          = 50.0f;
	unk170                               = 0;
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TElecCarapace::shoot()
{
	unk180 = 0;
	if (unk150 == 2)
		return;
	JGeometry::TVec3<f32> goal = SMS_GetMarioPos() - mPosition;
	MsVECNormalize(&goal, &goal);
	f32 distance = unk16C->unk1A0->mSLCarapaceFlyDist.get();
	goal.x *= distance;
	goal.y *= mPosition.y;
	goal.z *= distance;
	goal += mPosition;
	unk174 = !unk174;
	unk188 = 0.0f;
	unk150 = 2;
	unk176 = 0;
	unk168 = 0;
	unk184 = 0;
	unk175 = 0;
	offHitFilter(HIT_FILTER_NO_COLLISION);
	mSpine->initWith(&TNerveElecCarapaceMove::theNerve());
	setGoalPath(goal);
	setZigParameter();
}

void TElecCarapace::setZigParameter()
{
	f32 factor = TMsRange<f32>(3.0f, 5.0f).rand();
	unk178     = factor * (unk104.getPoint() - mPosition).length();
	unk17C     = TMsRange<f32>(20.0f, 30.0f).rand();
}

void TElecCarapace::bind()
{
	control();
	TEnemyAttachment::bind();
}

void TElecCarapace::calcRootMatrix()
{
	MsMtxSetXYZRPH(mMActor->getModel()->getBaseTRMtx(), mPosition.x,
	               mPosition.y, mPosition.z, mRotation.x, mRotation.y + unk188,
	               mRotation.z);
	mMActor->getModel()->setBaseScale(mScaling);
	SMSGetMSound()->startSoundActor(MSD_SE_EN_DENNOKO_SPARK2, &mPosition, 0,
	                                nullptr, 0, 4);

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_BIRI, mMActor->getModel()->getAnmMtx(2), 1, this);
	if (emitter)
		emitter->setGlobalScale(unk16C->mScaling);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_SPARK_L, mMActor->getModel()->getAnmMtx(2), 1, this);
	if (emitter)
		emitter->setGlobalScale(unk16C->mScaling);
	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    PARTICLE_MS_DNK_SPARK_R, mMActor->getModel()->getAnmMtx(2), 1, this);
	if (emitter)
		emitter->setGlobalScale(unk16C->mScaling);
}

void TElecCarapace::sendMessage()
{
	for (int i = 0; i < getColNum(); ++i) {
		THitActor* actor = mCollisions[i];
		if (actor->isActorType(ACTOR_TYPE_MARIO)) {
			if (SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK)) {
				onHitFilter(HIT_FILTER_NO_COLLISION);
				if (mSpine->getCurrentNerve()
				    != &TNerveElecCarapaceWait::theNerve())
					mSpine->pushNerve(&TNerveElecCarapaceWait::theNerve());
			}
		} else if (actor == unk16C) {
			offHitFilter(HIT_FILTER_NO_COLLISION);
		} else if (actor->isActorType(ACTOR_TYPE_WATER)) {
			// TODO: the three random angles are never used
			TMsRange<s32> range(0, 360);
			for (int j = 0; j < 5; ++j) {
				s32 x = range.rand();
				s32 y = range.rand();
				s32 z = range.rand();
			}
		} else if (TElecNokonoko::mReflectSw) {
			reflect(actor);
		}
	}
}

BOOL TElecCarapace::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_UNKD) {
		gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_TLS_CHANGE,
		                                            &mPosition, 0, nullptr);
		kill();
	}

	if (param_2 == HIT_MESSAGE_TRAMPLE)
		SMS_SendMessageToMario(this, HIT_MESSAGE_ELECTRIC_SHOCK);

	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER)
		return true;
	return false;
}

void TElecCarapace::reflect(THitActor* param_1)
{
	if (unk170 == param_1)
		return;
	unk184 = 0;
	unk170 = param_1;
	unk176 = 1;
	unk175 = 0;
	JGeometry::TVec3<f32> direction(param_1->mPosition.x - mPosition.x, 0.0f,
	                                param_1->mPosition.z - mPosition.z);
	if (0.0f == direction.x && 0.0f == direction.y && 0.0f == direction.z)
		direction.x += 1.0f;
	MsVECNormalize(&direction, &direction);
	JGeometry::TVec3<f32> normal(0.0f, 0.0f, 0.0f);
	if (fabsf(direction.z / direction.x) > 1.0f) {
		if (param_1->mPosition.z > mPosition.z)
			normal.z = 1.0f;
		else
			normal.z = -1.0f;
	} else {
		if (param_1->mPosition.x > mPosition.x)
			normal.x = 1.0f;
		else
			normal.x = -1.0f;
	}
	f32 speed   = -7.0f * direction.dot(normal);
	mVelocity.x = direction.x * speed;
	mVelocity.y = 2.0f;
	mVelocity.z = direction.z * speed;
	mPosition.y = 2.0f + mGroundHeight;
	unk174      = 0;
	if ((mVelocity.x > 0.0f && mVelocity.z > 0.0f)
	    || (mVelocity.x < 0.0f && mVelocity.z < 0.0f))
		unk174 = 1;
	setGoalPath(unk16C->mPosition);
}

void TElecCarapace::move()
{
	TElecNokonokoSaveLoadParams* params = unk16C->unk1A0;
	f32 spin                            = params->mSLCarapaceSpinSpeed.get();
	f32 speed                           = params->mSLCarapaceSpeed.get();
	f32 turn                            = params->mSLCarapaceTurnSpeed.get();
	if (unk175 != 0)
		walkToCurPathNode(speed, turn, 0.0f);
	else
		zigzagToCurPathNode(speed, turn, unk178, unk17C);
	unk188 += spin;
	if (unk188 > 360.0f)
		unk188 -= 360.0f;
}

bool TElecCarapace::isMove()
{
	if (mSpine->getCurrentNerve() == &TNerveElecCarapaceWait::theNerve())
		return false;
	return true;
}

DEFINE_NERVE(TNerveElecNokonokoShoot, TLiveActor)
{
	TElecNokonoko* self = (TElecNokonoko*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(9);
	if (self->isBckAnm(9)) {
		if (self->checkCurAnmEnd(0)) {
			self->setBckAnm(12);
			self->unk198 = 0;
		}
	} else if (self->isBckAnm(12)) {
		if (self->getMActor()->getFrameCtrl(0)->checkPass(60.0f))
			self->unk194->appear();
		if (self->getCurAnmFrameNo(0) < 62.0f)
			self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
		if (self->getMActor()->getFrameCtrl(0)->checkPass(62.0f))
			self->shootIn();
		if (self->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveElecNokonokoCollect::theNerve());
			return true;
		}
	}
	return false;
}

DEFINE_NERVE(TNerveElecNokonokoCollect, TLiveActor)
{
	TElecNokonoko* self = (TElecNokonoko*)spine->getBody();
	if (spine->getTime() == 0) {
		if (!self->isBckAnm(0))
			self->setBckAnm(8);
		self->setGoalPath(self->unk194);
	}
	if (self->unk194->isMove())
		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	else
		self->getMActor()->setFrameRate(0.0f, 0);
	if (self->isBckAnm(0)) {
		int frame = self->getCurAnmFrameNo(0);
		if (frame > 20)
			self->unk194->onHitFilter(HIT_FILTER_NO_COLLISION);
		if (frame > 32) {
			self->unk1A4 = 0;
			self->unk194->kill();
		}
		if (self->checkCurAnmEnd(0))
			return true;
	} else if (spine->getTime() > 800
	           || self->unk194->checkLiveFlag(LIVE_FLAG_DEAD)) {
		spine->pushAfterCurrent(&TNerveElecNokonokoRebirth::theNerve());
		return true;
	}
	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	return false;
}

DEFINE_NERVE(TNerveElecNokonokoTurn, TLiveActor)
{
	TElecNokonoko* self = (TElecNokonoko*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(16);
		self->setGoalPath((THitActor*)gpMarioAddress);
	}
	if (self->isBckAnm(15)
	    && MsIsInSight(self->mPosition, self->mRotation.y, SMS_GetMarioPos(),
	                   self->getSaveParams()->getSLSearchLength(), 60.0f, 0.0f))
		self->setBckAnm(14);
	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(16))
			self->setBckAnm(15);
		else if (self->isBckAnm(14))
			return true;
	}
	if (0.0f == self->mPosition.x - self->unk194->mPosition.x
	    && 0.0f == self->mPosition.z - self->unk194->mPosition.z)
		self->mPosition.x += 1.0f;
	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	if (spine->getTime() > 500)
		return true;
	return false;
}

DEFINE_NERVE(TNerveElecNokonokoFreeze, TLiveActor)
{
	TElecNokonoko* self = (TElecNokonoko*)spine->getBody();
	if (spine->getTime() == 0) {
		if (self->hasCarapace()) {
			self->setBckAnm(3);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_DNK_SHIBIRE_B,
			    self->getMActor()->getModel()->getAnmMtx(0), 0, nullptr);
		} else {
			self->setBckAnm(7);
		}
	}
	if (self->isBckAnm(3) && self->getCurAnmFrameNo(0) < 25.0f) {
		MtxPtr jointMtx = self->getMActor()->getModel()->getAnmMtx(8);
		self->unk1A8.set(jointMtx[0][3], jointMtx[1][3], jointMtx[2][3]);
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_DNK_HIBANA, &self->unk1A8, 1, self);
		if (emitter != nullptr)
			emitter->setGlobalScale(self->mScaling);
	}
	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(7)) {
			self->setBckAnm(6);
		} else if (self->isBckAnm(6)) {
			if (!self->unsetUnk165() && self->hasCarapace())
				self->setBckAnm(5);
			else
				self->setBckAnm(6);
		} else {
			return true;
		}
	}
	if (spine->getTime() > 400) {
		spine->reset();
		spine->setDefaultNext();
		spine->pushAfterCurrent(&TNerveElecNokonokoRebirth::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveElecNokonokoRebirth, TLiveActor)
{
	TElecNokonoko* self = (TElecNokonoko*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(13);
		self->unk1A4 = 0;
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_TLS_CHANGE, &self->unk194->mPosition, 0, nullptr);
		self->unk194->kill();
	}

	if (self->getMActor()->getFrameCtrl(0)->checkPass(88.0f))
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_TLS_CHANGE, &self->mPosition, 0, nullptr);

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveElecNokonokoAttack, TLiveActor)
{
	TElecNokonoko* self = (TElecNokonoko*)spine->getBody();
	if (spine->getTime() == 0 || !self->isBckAnm(3))
		self->setBckAnm(3);
	if (self->checkCurAnmEnd(0))
		return true;
	return false;
}

DEFINE_NERVE(TNerveElecCarapaceMove, TLiveActor)
{
	TElecCarapace* self = (TElecCarapace*)spine->getBody();
	self->move();
	if (self->unk184 != 0) {
		f32 range = 64.0f * self->unk16C->unk1A0->mSLCarapaceSpeed.get();
		if ((self->unk104.getPoint() - self->mPosition).length() < range) {
			if (!self->unk16C->isCatchReady()) {
				self->unk16C->forceCatchReady();
			}
			spine->pushAfterCurrent(&TNerveElecCarapaceReturn::theNerve());
			return true;
		}
	}
	JGeometry::TVec3<f32> diff = self->unk104.getPoint();
	diff -= self->mPosition;
	diff.y = 0.0f;
	if (self->unk176 == 0 && MsVECMag2(&diff) < 100.0f) {
		if (self->unk184 != 0) {
			spine->pushAfterCurrent(&TNerveElecCarapaceReturn::theNerve());
			return true;
		}
		self->unk184 = 1;
		self->setGoalPath(self->unk16C->mPosition);
	}
	return false;
}

DEFINE_NERVE(TNerveElecCarapaceWait, TLiveActor)
{
	if (spine->getTime() > 60)
		return true;
	return false;
}

DEFINE_NERVE(TNerveElecCarapaceReturn, TLiveActor)
{
	TElecCarapace* self = (TElecCarapace*)spine->getBody();
	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> hostPos = self->unk16C->mPosition;
		self->unk18C.set((hostPos.x - self->mPosition.x) * 0.015625f,
		                 (hostPos.y - self->mPosition.y) * 0.015625f,
		                 (hostPos.z - self->mPosition.z) * 0.015625f);
		self->unk16C->catchIn();
	}
	if (spine->getTime() < 20) {
		self->unk16C->catchIn();
	}
	if (self->unk16C->isDeadByThunder()) {
		self->unk16C->onLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		self->unk16C->kill();
		gpMarioParticleManager->emitAndBindToPosPtr(
		    PARTICLE_MS_TLS_CHANGE, &self->mPosition, 0, nullptr);
	}
	self->unk188 += self->unk16C->unk1A0->mSLCarapaceSpinSpeed.get();
	if (self->unk188 > 360.0f)
		self->unk188 -= 360.0f;
	self->mPosition += self->unk18C;
	JGeometry::TVec3<f32> hostPos = self->unk16C->mPosition;
	if (self->unk18C.x > 0.0f) {
		if (self->mPosition.x > hostPos.x)
			self->mPosition.x = hostPos.x;
	} else {
		if (self->mPosition.x < hostPos.x)
			self->mPosition.x = hostPos.x;
	}
	if (self->unk18C.y > 0.0f) {
		if (self->mPosition.y > hostPos.y)
			self->mPosition.y = hostPos.y;
	} else {
		if (self->mPosition.y < hostPos.y)
			self->mPosition.y = hostPos.y;
	}
	if (self->unk18C.z > 0.0f) {
		if (self->mPosition.z > hostPos.z)
			self->mPosition.z = hostPos.z;
	} else {
		if (self->mPosition.z < hostPos.z)
			self->mPosition.z = hostPos.z;
	}
	if (self->unk16C->checkLiveFlag(LIVE_FLAG_DEAD)) {
		self->mScaling.y *= 0.8f;
		if (self->mScaling.y < 0.01f)
			self->kill();
	}
	return false;
}
