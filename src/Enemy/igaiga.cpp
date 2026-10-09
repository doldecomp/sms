#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Igaiga.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGMatrix34.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapData.hpp>
#include <Map/MapEventSink.hpp>
#include <Map/MapMirror.hpp>
#include <Map/PollutionManager.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBianco.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Particles.hpp>
#include <System/Params.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static TRollEnemy* gpCurRollEnemy;

f32 TRollEnemy::mTransYOffset;
f32 TRollEnemy::mBoundVal   = 80.0f;
f32 TIgaiga::mReachNodeDist = 300.0f;

TRollEnemySaveLoadParams::TRollEnemySaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLGenerateInterval, 300)
    , PARAM_INIT(mSLExpandRate, 1.0f)
    , PARAM_INIT(mSLExpandMax, 1.5f)
    , PARAM_INIT(mSLBoundVYMax, 15.0f)
    , PARAM_INIT(mSLGroundOffsetY, 150.0f)
{
	load(mPrmPath);
}

TRollEnemy::TRollEnemy(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(0.0f)
    , unk198(0.0f)
    , unk19C(0.0f)
    , unk1A0(0.0f)
    , unk1A4(nullptr)
    , unk1A8(0)
    , unk1AC(0.0f)
    , unk1B0(1.0f)
{
}

void TRollEnemy::reset()
{
	gpCurRollEnemy = this;
	TWalkerEnemy::reset();

	unk194 = TMsRange<f32>(0.0f, 360.0f).rand();
	unk158 = 1.0f;

	JGeometry::TVec3<f32> point;
	getTracer()->getGraph()->getGraphNode(0).getPoint(&point);
	mPosition = point;
	mPosition.y += 10.0f;

	getTracer()->getGraph()->getGraphNode(1).getPoint(&point);

	JGeometry::TVec3<f32> dir;
	dir.sub(point, mPosition);
	mRotation.y = MsWrap(MsGetRotFromZaxisY(dir), 0.0f, 360.0f);

	unk198                = 1.5f * mMarchSpeed;
	unk19C                = mMarchSpeed;
	unk1A0                = 0.0f;
	getTracer()->mCurrIdx = 0;
}

void TRollEnemy::walkBehavior(int param_1, f32 param_2)
{
	if (!unk1A8)
		TWalkerEnemy::walkBehavior(param_1, param_2);

	if (isAirborne() && mPosition.y > mGroundHeight + 20.0f) {
		f32 bound = MsWrap((mPosition.y - mGroundHeight) / mBoundVal, 0.0f,
		                   unk1A4->mSLBoundVYMax.get());
		if (unk1A0 < bound)
			unk1A0 = bound;
	} else {
		if (!mGroundPlane->isWaterSurface()) {
			unk1A8 = false;
			if (unk1A0 > unk1B0) {
				bound();
				mVelocity = JGeometry::TVec3<f32>(0.0f, unk1A0, 0.0f);
				onLiveFlag(LIVE_FLAG_AIRBORNE);
				mPosition.y += 5.0f;
				unk1A0 = 0.0f;
				boundSE();
			}
		}
	}
	if (mPosition.y < mGroundHeight + 30.0f)
		rollSE();
	if (unk128 > 300) {
		onLiveFlag(LIVE_FLAG_UNK10000);
		kill();
	}
}

void TRollEnemy::behaveToWater(THitActor* param_1)
{
	mSprayedByWaterCooldown = 0;
	if (unk158 < unk1A4->mSLExpandMax.get()) {
		f32 rate = unk1A4->mSLExpandRate.get();
		mBodyScale *= rate;
		unk158 *= rate;
		mScaledBodyRadius *= rate;
		mScaling.x = mScaling.y = mScaling.z *= rate;

		f32 attackRadius = getSaveParams()->getSLAttackRadius();
		f32 attackHeight = getSaveParams()->getSLAttackHeight();
		f32 damageRadius = getSaveParams()->getSLDamageRadius();
		f32 damageHeight = getSaveParams()->getSLDamageHeight();
		f32 scale        = mBodyScale / unk154;
		setHitParams(attackRadius * scale, attackHeight * scale,
		             damageRadius * scale, damageHeight * scale);
	}
}

void TRollEnemy::attackToMario()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

void TRollEnemy::flagJump()
{
	JGeometry::TVec3<f32> point;
	getTracer()->getCurrent().getPoint(&point);

	mPosition.y += 30.0f;

	f32 jumpSpeed = getTracer()->unkC;
	JGeometry::TVec3<f32> velocity
	    = calcVelocityToJumpToY(point, jumpSpeed, getGravityY());

	unk1A8 = true;
	setVelocity(velocity);
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

bool TRollEnemy::isCollidMove(THitActor* param_1)
{
	if (param_1->isActorType(ACTOR_TYPE_STOP_ROCK)) {
		kill();
		return true;
	}

	param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
	return false;
}

bool TRollEnemy::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> tmp = getUnk104().getPoint();
	tmp -= mPosition;
	if (!unk1A8)
		tmp.y = 0.0f;
	if (MsVECMag2(&tmp) < 200.0f)
		return true;
	return false;
}

void TRollEnemy::setBehavior()
{
	if (mPosition.y > mGroundHeight + 50.0f)
		return;

	if (mSpine->getTime() % getSaveParams()->getSLPolluteInterval() != 0)
		return;
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;
	if (!mIsPolluter)
		return;

	f32 range = 2.0f;
	if (!checkLiveFlag(LIVE_FLAG_HIDDEN)) {
		if (!mIsAmpPolluter) {
			range = getSaveParams()->getSLPolluteRange();
		} else {
			s32 min   = getSaveParams()->getSLPolluteRMin();
			s32 max   = getSaveParams()->getSLPolluteRMax();
			s32 cycle = getSaveParams()->getSLPolluteCycle();
			f32 angle = 180.0f * (f32)(mSpine->getTime() % cycle) / (f32)cycle;
			range = (f32)min + mBodyScale * ((f32)(max - min) * MsSin(angle));
		}
	}

	gpPollution->stampGround(
	    1, mPosition.x + unk1AC * mPositionDelta.x, mPosition.y,
	    mPosition.z + unk1AC * mPositionDelta.z, 32.0f * range);
}

void TIgaigaPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRGetResource("/scene/igaiga/stamp_igaiga_model1.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (1 << J3DMLF_TevStageNumShift)));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TIgaigaPolluteModel(param_1, 0, modelData);
}

void TIgaigaPolluteModel::setAnm()
{
	unk10->getMActor()->setBckFromIndex(7);
	unk10->getMActor()->getFrameCtrl(0)->setFrame(0.0f);
}

TIgaigaManager::TIgaigaManager(const char* param_1)
    : TSmallEnemyManager(param_1)
    , unk64(nullptr)
    , unk68(nullptr)
{
	gpCurRollEnemy = nullptr;
}

void TIgaigaManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TRollEnemySaveLoadParams("/enemy/igaiga.prm");
	unk68 = new TWaterEmitInfo("/enemy/igaigawater.prm");
}

void TIgaigaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "igaiga_model1.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
		      | J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSmallEnemy* TIgaigaManager::createEnemyInstance() { return new TIgaiga; }

void TIgaigaManager::initSetEnemies()
{
	unk60 = new TIgaigaPolluteModelManager;
	unk60->init((TLiveActor*)unk18[0]);
}

void TIgaigaManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TEnemyManager::perform(param_1, param_2);
	unk60->perform(param_1, param_2);
}

void TIgaigaManager::requestPolluteModel(JGeometry::TVec3<f32>& param_1,
                                         JGeometry::TVec3<f32>& param_2)
{
	unk60->generatePolluteModel(param_1, param_2);
}

static BOOL RollEnemyBodyCallback(J3DNode* param_1, BOOL param_2)
{
	if (param_2 == 0) {
		if (gpCurRollEnemy == nullptr || !gpCurRollEnemy->isRolling())
			return TRUE;

		Mtx rot;
		MtxPtr mtx = gpCurRollEnemy->getModel()->getAnmMtx(
		    ((J3DJoint*)param_1)->getJntNo());
		MsMtxSetRotX(rot, gpCurRollEnemy->unk194);

		mtx[1][3] += TRollEnemy::mTransYOffset;

		MTXConcat(mtx, rot, mtx);
		MTXConcat(J3DSys::mCurrentMtx, rot, J3DSys::mCurrentMtx);
	}
	return TRUE;
}

TIgaiga::TIgaiga(const char* param_1)
    : TRollEnemy(param_1)
    , unk1B4(0)
    , unk1B8(0)
    , unk1BC(true)
    , unk1CC(1.0f)
    , unk1D0(0)
    , unk1E4(1.0f)
    , unk1E8(0)
{
}

void TIgaiga::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = ACTOR_TYPE_IGAIGA;
	unk150     = 0x11;
	offHitFilter(HIT_CATEGORY_BOSS | HIT_CATEGORY_ENEMY);
	onHitFilter(HIT_CATEGORY_MAP_OBJECT);

	mSpine->initWith(&TNerveIgaigaRollOnGraph::theNerve());
	unk1A4 = (TRollEnemySaveLoadParams*)getSaveParam();

	mMActor->setJointCallback(1, RollEnemyBodyCallback);
	mMActor->setBtkFromIndex(0);

	unk124->init(gpConductor->getGraphByName("igaiga"));
}

void TIgaiga::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("igaiga_model1.bmd", 0);
}

void TIgaiga::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
}

void TIgaiga::calcRootMatrix()
{
	gpCurRollEnemy = this;
	TSpineEnemy::calcRootMatrix();
}

bool TIgaiga::isRolling()
{
	if (mSpine->getCurrentNerve() == &TNerveIgaigaRollOnGraph::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveIgaigaShootFromCannon::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveIgaigaWaterHit::theNerve())
		return true;

	return false;
}

void TIgaiga::behaveToWater(THitActor* param_1)
{
	mSprayedByWaterCooldown = 0;
	if (unk1E4 < unk1A4->mSLExpandMax.get())
		unk1E4 *= unk1A4->mSLExpandRate.get();
	unk165 = true;
	if (mSpine->getCurrentNerve() != &TNerveIgaigaWaterHit::theNerve())
		mSpine->pushNerve(&TNerveIgaigaWaterHit::theNerve());
}

void TIgaiga::reset()
{
	TRollEnemy::reset();
	initialGraphNode();
	offLiveFlag(LIVE_FLAG_UNK10);

	unk1B4 = 0;
	unk1B8 = TMsRange<s32>(50, 100).rand() * 120;
	unk1BC = true;

	mPosition.y += 20.0f;
	gpMap->checkGround(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
	                   &mGroundPlane);
	onLiveFlag(LIVE_FLAG_AIRBORNE);

	unk1E4 = 1.0f;
	unk1CC = 1.0f;
	unk1AC = -30.0f;
	unk1B0 = 2.0f;
	unk1E8 = 0;
}

void TIgaiga::kill()
{
	unk194 = 0.0f;
	TSmallEnemy::kill();
}

void TIgaiga::moveObject()
{
	TWalkerEnemy::moveObject();
	unk1CC = MsClamp(unk1CC - 0.0002f, 0.5f, 1.0f);

	f32 attackRadius = getSaveParams()->getSLAttackRadius();
	f32 attackHeight = getSaveParams()->getSLAttackHeight();
	f32 damageRadius = getSaveParams()->getSLDamageRadius();
	f32 damageHeight = getSaveParams()->getSLDamageHeight();
	f32 baseScale    = unk154 * unk1CC;
	mBodyScale = MsClamp(unk1E4 * baseScale, baseScale, 3.0f * mBodyScale);
	f32 scale  = mBodyScale / unk154;
	mScaledBodyRadius
	    = 8.0f * getBodyRadius() * MsClamp(unk1CC * unk1E4, 1.0f, 1.2f);
	mScaling.x = mScaling.y = mScaling.z = mBodyScale;
	setHitParams(attackRadius * scale, attackHeight * scale,
	             damageRadius * scale, damageHeight * scale);
	mMarchSpeed = unk1A4->mSLMarchSpeedLow.get();
	mTurnSpeed  = unk1A4->mSLTurnSpeedLow.get();
	if (getTracer()->getCurrent().checkFlag(0x40)) {
		JGeometry::TVec3<f32> point;
		getTracer()->getCurrent().getPoint(&point);
		if (mPosition.y < point.y + 50.0f) {
			kill();
			unk1BC = true;
		}
	}
}

void TIgaiga::rollSE()
{
	SMSGetMSound()->startSoundActorSpecial(MSD_SE_EN_IGAIGA_ROLL, &mPosition,
	                                       mScaling.x, mMarchSpeed, 0, nullptr,
	                                       0, 4);
}

void TIgaiga::boundSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_EN_IGAIGA_BOUND, &mPosition, nullptr,
	    abs(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

void TIgaiga::walkBehavior(int param_1, f32 param_2)
{
	TRollEnemy::walkBehavior(param_1, param_2);

	f32 x = mPositionDelta.x;
	f32 z = mPositionDelta.z;
	if (unk1A8) {
		x = getVelocity().x;
		z = getVelocity().z;
	}
	f32 speed = JGeometry::TVec2<f32>(x, z).length();
	unk194 += 4.0f * (speed / (mBodyRadius * unk1CC * unk1E4));

	if (unk1B4) {
		++unk1B4;
		if (unk1B4 > unk1B8)
			unk1B4 = 0;
	}

	if (!isAirborne() && mGroundPlane && mGroundPlane->getActor())
		((TLiveActor*)mGroundPlane->getActor())
		    ->receiveMessage(this, HIT_MESSAGE_ATTACK);
	if (unk138 && unk138->getActor())
		((TLiveActor*)unk138->getActor())
		    ->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

bool TIgaiga::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> tmp = getUnk104().getPoint();
	tmp -= mPosition;
	if (!unk1A8)
		tmp.y = 0.0f;
	tmp.y = 0.0f;
	if (MsVECMag2(&tmp) < mReachNodeDist)
		return true;
	return false;
}

void TIgaiga::setWalkAnm() { setBckAnm(3); }

void TIgaiga::setDeadAnm()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk1C0 = mPosition;
	} else {
		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(0);
		unk1C0.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}

	gpMarioParticleManager->emit(PARTICLE_MS_IGA_FUMI_AIR, &unk1C0, 0, nullptr);

	if (unk1BC)
		setBckAnm(0);
	else
		setBckAnm(1);

	TIgaigaManager* manager     = (TIgaigaManager*)getManager();
	JGeometry::TVec3<f32> scale = getScaling();
	scale.scale(unk1CC * unk1E4);

	mPosition.y = getGroundHeight();

	scale.x = MsClamp(scale.x, 0.8f, 1.5f);
	scale.y = scale.z = scale.x;

	manager->requestPolluteModel(mPosition, scale);
}

void TIgaiga::setMeltAnm()
{
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk1C0 = mPosition;
	} else {
		MtxPtr mtx = getMActor()->getModel()->getAnmMtx(0);
		unk1C0.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	}

	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT) && SMS_IsMarioTouchGround4cm()) {
		if (mScaling.x > mBodyScale)
			SMSRumbleMgr->start(0x15, 10, &mPosition);
		else
			SMSRumbleMgr->start(0x14, 10, &mPosition);
	}

	waterExplosion();

	JGeometry::TVec3<f32> scale = getScaling() * 0.5f;
	if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	        PARTICLE_MS_POPO_BOMB_A, &unk1C0, 0, nullptr))
		emitter->setGlobalScale(scale);
	if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	        PARTICLE_MS_POPO_BOMB_B, &unk1C0, 0, nullptr))
		emitter->setGlobalScale(scale);

	setBckAnm(5);
	if (TMsRange<f32>(0.0f, 1.0f).rand() < 0.2f)
		gpItemManager->makeObjAppear(mPosition.x, mPosition.y + 20.0f,
		                             mPosition.z, ACTOR_TYPE_BOTTLE_LARGE,
		                             true);
}

static const char* igaiga_bastable[] = {
	"/scene/igaiga/bas/igaiga_down1.bas",
	"/scene/igaiga/bas/igaiga_down2.bas",
	nullptr,
	nullptr,
	"/scene/igaiga/bas/igaiga_shoot1.bas",
	"/scene/igaiga/bas/igaiga_waterdown1.bas",
	"/scene/igaiga/bas/igaiga_waterhit1.bas",
	nullptr,
};

const char** TIgaiga::getBasNameTable() const { return igaiga_bastable; }

bool TIgaiga::isHitValid(u32 param_1)
{
	unk1BC = true;
	if (param_1 == HIT_MESSAGE_HIP_DROP)
		unk1BC = false;
	return true;
}

void TIgaiga::rollMove()
{
	if (checkCurAnmEnd(0) && isBckAnm(2))
		setBckAnm(3);

	if (isReachedToGoalXZ()) {
		if (jumpToNextGraphNode() >= 0)
			flagJump();
		if (getTracer()->getCurrent().checkFlag(0x40))
			return;
		goToRandomNextGraphNode();
	}
	walkBehavior(2, 1.0f);
}

void TIgaiga::bound()
{
	if (unk1A0 > 5.0f) {
		setBckAnm(2);

		if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
		    && SMS_IsMarioTouchGround4cm()) {
			if (mScaling.x > mBodyScale)
				SMSRumbleMgr->start(0x15, 10, &mPosition);
			else
				SMSRumbleMgr->start(0x14, 10, &mPosition);
		}
	}
}

void TIgaiga::waterExplosion()
{
	TIgaigaManager* manager    = (TIgaigaManager*)getManager();
	manager->unk68->mPos.value = mPosition;
	gpModelWaterManager->emitRequest(*manager->unk68);
}

void TIgaiga::shoot(JGeometry::TVec3<f32>& param_1)
{
	mSpine->setNext(&TNerveIgaigaShootFromCannon::theNerve());
	unk1D8 = param_1;
	offLiveFlag(LIVE_FLAG_UNK10);
	unk1A8 = true;
}

DEFINE_NERVE(TNerveIgaigaRollOnGraph, TLiveActor)
{
	TIgaiga* self = (TIgaiga*)spine->getBody();
	if (spine->getTime() == 0)
		self->setWalkAnm();
	self->rollMove();
	return FALSE;
}

DEFINE_NERVE(TNerveIgaigaWaterHit, TLiveActor)
{
	TIgaiga* self = (TIgaiga*)spine->getBody();
	if (spine->getTime() == 0)
		self->setBckAnm(6);

	if (self->unk1E4 >= self->unk1A4->mSLExpandMax.get()) {
		if (self->unk1E8 > 20) {
			self->onLiveFlag(LIVE_FLAG_UNK10000);
			spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
			return TRUE;
		}
		++self->unk1E8;
	} else if (self->checkCurAnmEnd(0) && !self->unsetUnk165()) {
		self->setBckAnm(3);
		spine->pushAfterCurrent(&TNerveIgaigaRollOnGraph::theNerve());
		return TRUE;
	}

	self->rollMove();
	return FALSE;
}

DEFINE_NERVE(TNerveIgaigaShootFromCannon, TLiveActor)
{
	TIgaiga* self = (TIgaiga*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(4);
		self->mPosition.y += 10.0f;
		self->mVelocity = self->unk1D8;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
	} else if (self->checkCurAnmEnd(0) && !self->isAirborne()) {
		self->bound();
		spine->pushAfterCurrent(&TNerveIgaigaRollOnGraph::theNerve());
		return TRUE;
	}

	self->walkBehavior(2, 1.0f);
	return FALSE;
}

void TGorogoroPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRGetResource("/scene/gorogoro/bosspaku_head_stamp.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (1 << J3DMLF_TevStageNumShift)));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TGorogoroPolluteModel(param_1, 0, modelData);
}

void TGorogoroPolluteModel::setAnm()
{
	unk10->getMActor()->setBckFromIndex(3);
	unk10->getMActor()->getFrameCtrl(0)->setFrame(0.0f);
}

TGorogoroManager::TGorogoroManager(const char* param_1)
    : TSmallEnemyManager(param_1)
    , unk60(0)
    , unk64(nullptr)
    , unk68(1)
    , unk6C(nullptr)
    , unk70(nullptr)
{
}

void TGorogoroManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TRollEnemySaveLoadParams("/enemy/gorogoro.prm");
	static const char* anmlist[] = { "bosspaku_head_move", nullptr };
	createSharedMActorSet(anmlist);
}

void TGorogoroManager::loadAfter()
{
	unk64 = (TMapEventSink*)JDrama::TNameRefGen::getInstance()
	            ->getRootNameRef()
	            ->search("イベント（地形沈むビアンコ）");
	unk70 = (TAreaCylinderManager*)gpConductor->search(
	    "ゴロゴロ発生マネージャー");
}

TSmallEnemy* TGorogoroManager::createEnemyInstance() { return new TGorogoro; }

void TGorogoroManager::initSetEnemies()
{
	unk6C = new TGorogoroPolluteModelManager;
	unk6C->init((TLiveActor*)unk18[0]);

	static const char* graphlist[] = { "gorogoro0", "gorogoro1" };
	for (int i = 0; i < mObjNum; ++i) {
		TGraphWeb* graph = gpConductor->getGraphByName(graphlist[i % 2]);
		if (graph->isDummy())
			graph = gpConductor->getGraphByName(graphlist[0]);

		if (!graph->isDummy()) {
			TGorogoro* gorogoro = (TGorogoro*)unk18[i];
			JGeometry::TVec3<f32> point;
			graph->getGraphNode(0).getPoint(&point);
			gorogoro->getTracer()->init(graph);
			gorogoro->mPosition = point;
			gorogoro->unk1E8    = graph->unk8 - 1;
		}
	}
}

void TGorogoroManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "bosspaku_head.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
		      | (16 << J3DMLF_TevStageNumShift),
		  0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

BOOL TGorogoroManager::inArea(const JGeometry::TVec3<f32>& param_1)
{
	return unk70 ? unk70->contain(param_1) : TRUE;
}

void TGorogoroManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & CUE_MOVE) {
		++unk60;
		s32 interval
		    = ((TRollEnemySaveLoadParams*)unk38)->mSLGenerateInterval.get();
		if (unk60 > interval) {
			unk60 = 0;
			if (inArea(SMS_GetMarioPos())) {
				for (int i = 0; i < getActiveObjNum(); ++i) {
					TGorogoro* gorogoro = (TGorogoro*)getObj(i);
					if (!gorogoro->checkLiveFlag(LIVE_FLAG_DEAD))
						continue;

					if (unk64) {
						if (!unk64->isBuried(1)) {
							if (unk68) {
								unk68 = false;
								gorogoro->reset();
								gorogoro->setGenerateGraphIdx(10);
								TGorogoro* other = (TGorogoro*)getObj(1);
								other->reset();
								other->setGenerateGraphIdx(16);
							} else {
								gorogoro->reset();
							}
						} else {
							unk60 = interval;
						}
					} else {
						gorogoro->reset();
					}
					break;
				}
			}
		}
	}

	TEnemyManager::perform(param_1, param_2);
	unk6C->perform(param_1, param_2);
}

void TGorogoroManager::requestPolluteModel(JGeometry::TVec3<f32>& param_1,
                                           JGeometry::TVec3<f32>& param_2)
{
	unk6C->generatePolluteModel(param_1, param_2);
}

TGorogoro::TGorogoro(const char* param_1)
    : TRollEnemy(param_1)
    , unk1E4(0)
    , unk1E8(0)
    , unk1EC(0)
{
	gpCurRollEnemy = nullptr;
}

void TGorogoro::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = ACTOR_TYPE_GOROGORO;
	unk150     = 0x31;
	offHitFilter(HIT_CATEGORY_BOSS | HIT_CATEGORY_ENEMY
	             | HIT_CATEGORY_MAP_OBJECT);
	mSpine->initWith(&TNerveGorogoroRollOnGraph::theNerve());
	unk1A4 = (TRollEnemySaveLoadParams*)getSaveParam();

	TMirrorActor* mirror = new TMirrorActor("ゴロゴロin鏡");
	mirror->init(getMActor()->getModel(), 0x18);
	mTevKColor.a = 0xFF;

	ResTIMG* img
	    = (ResTIMG*)JKRGetResource("/scene/map/pollution/H_ma_rak.bti");
	if (img) {
		SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(), "M_dummy",
		                     *img);
		SMS_ChangeTextureAll(mirror->unk14->getModelData(), "M_dummy", *img);
	}

	for (u16 i = 0;
	     i < getMActor()->getModel()->getModelData()->getMaterialNum(); ++i) {
		SMS_InitPacket_OneTevKColor(getMActor()->getModel(), i, GX_KCOLOR0,
		                            &mTevKColor);
		SMS_InitPacket_OneTevKColor(mirror->unk14, i, GX_KCOLOR0, &mTevKColor);
	}

	mMActor->setJointCallback(1, RollEnemyBodyCallback);
	unk130 = 1;
}

void TGorogoro::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		if (gpMirrorModelManager->isInMirror(mPosition)) {
			if (param_1 & CUE_CALC_ANIM) {
				calcRootMatrix();
				mMActor->calc();
			}
			if (param_1 & CUE_CALC_VIEW)
				mMActor->viewCalc();
		}
	}
}

void TGorogoro::calcRootMatrix()
{
	gpCurRollEnemy = this;
	if (mSpine->getCurrentNerve() == &TNerveGorogoroDie::theNerve()) {
		TSpineEnemy::calcRootMatrix();
		if (checkLiveFlag(LIVE_FLAG_UNK10000)) {
			unk1B4.ref(0, 3) = mPosition.x;
			unk1B4.ref(2, 3) = mPosition.z;
		}
	} else if (!isEaten()) {
		f32 groundOffset = unk1A4->mSLGroundOffsetY.get();
		if (mPosition.y < mGroundHeight + 30.0f) {
			if (JPABaseEmitter* emitter
			    = gpMarioParticleManager->emitAndBindToMtxPtr(
			        PARTICLE_MS_PKH_DORO, getMActor()->getModel()->getAnmMtx(0),
			        1, this))
				emitter->setGlobalScale(mScaling);
		}
		MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
		               mPosition.y + groundOffset * unk158, mPosition.z,
		               mRotation.x, mRotation.y, mRotation.z);
		getModel()->setBaseScale(mScaling);
	}
}

void TGorogoro::reset()
{
	unk130 = 1;
	TRollEnemy::reset();
	offLiveFlag(LIVE_FLAG_UNK1000);
	mTevKColor.a = 0xFF;
	unk1AC       = -10.0f;
	unk1B0       = 1.0f;
}

void TGorogoro::kill()
{
	unk194 = 0.0f;

	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveGorogoroDie::theNerve())
		return;

	mSpine->reset();
	mSpine->setNext(&TNerveGorogoroDie::theNerve());
	mSpine->pushAfterCurrent(mSpine->getDefault());
	onLiveFlag(LIVE_FLAG_UNK8);
}

void TGorogoro::forceKill()
{
	if (mGroundPlane->isIllegalData())
		return;

	if (!mGroundPlane->isPool() && !mGroundPlane->isWaterSurface())
		return;

	if (isAirborne())
		return;

	if (mSpine->getCurrentNerve() == &TNerveGorogoroDie::theNerve())
		return;

	mSpine->reset();
	mSpine->setNext(&TNerveGorogoroDie::theNerve());
	mSpine->pushAfterCurrent(mSpine->getDefault());
	onLiveFlag(LIVE_FLAG_UNK20000);
	onLiveFlag(LIVE_FLAG_MELT_ON_DEATH);
}

void TGorogoro::behaveToWater(THitActor* param_1)
{
	TRollEnemy::behaveToWater(param_1);

	u8 maxHitPoints = getMaxHitPoints();
	mTevKColor.a    = mHitPoints * 255 / maxHitPoints;
	if (mHitPoints < 2)
		mHitPoints = 1;

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	        PARTICLE_MS_PKH_WASH, getMActor()->getModel()->getAnmMtx(0), 1,
	        this))
		emitter->setGlobalScale(mScaling);
}

void TGorogoro::rollSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_KRPAKU_ROLL, &mPosition, nullptr,
	    abs(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

void TGorogoro::boundSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_KRPAKU_GND, &mPosition, nullptr,
	    abs(getGroundPlane()->getNormal().y), 0, 0, nullptr, 0, 4);
}

void TGorogoro::walkBehavior(int param_1, f32 param_2)
{
	if (mPosition.y > mGroundHeight)
		onLiveFlag(LIVE_FLAG_AIRBORNE);

	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		if (mGroundPlane && mGroundPlane->getActor()
		    && mGroundPlane->getActor()->mActorType
		           == ACTOR_TYPE_BIA_WATERMILL01) {
			((TBiancoWatermill*)mGroundPlane->getActor())
			    ->turnByEnemy(this, mGroundPlane);
			TRollEnemy::walkBehavior(param_1, 0.2f * param_2);
			return;
		}

		const TBGCheckData* roof;
		gpMap->checkRoof(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
		                 &roof);
		if (roof && roof->getActor()
		    && roof->getActor()->mActorType == ACTOR_TYPE_BIA_WATERMILL01) {
			((TBiancoWatermill*)roof->getActor())->turnByEnemy(this, roof);
			TRollEnemy::walkBehavior(param_1, 0.3f * param_2);
			return;
		}

		if (unk138 && unk138->getActor()) {
			TBGWallCheckRecord record(mPosition.x, mPosition.y + mHeadHeight,
			                          mPosition.z, getWallRadius(), 4, 0);
			if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
				for (int i = 0; i < record.mResultWallsNum; ++i) {
					const TLiveActor* actor
					    = record.mResultWalls[i]->getActor();
					if (actor
					    && actor->mActorType == ACTOR_TYPE_BIA_WATERMILL01) {
						((TBiancoWatermill*)actor)
						    ->turnByEnemy(this, record.mResultWalls[i]);
					}
				}
				TRollEnemy::walkBehavior(param_1, 0.2f * param_2);
				return;
			}
		}
	}

	mTurnSpeed = unk1A4->mSLTurnSpeedLow.get();
	TRollEnemy::walkBehavior(param_1, param_2);
	unk194 += 0.4f * mMarchSpeed;

	if (mSpine->getCurrentNerve() != &TNerveGorogoroDie::theNerve()
	    && mPosition.y < mGroundHeight + 10.0f
	    && getGroundPlane()->isWaterSurface()) {
		onLiveFlag(LIVE_FLAG_UNK10000);
		onLiveFlag(LIVE_FLAG_UNK20000);
		kill();
	}
}

void TGorogoro::flagJump() { TRollEnemy::flagJump(); }

void TGorogoro::setDeadAnm()
{
	MtxPtr mtx = getMActor()->getModel()->getAnmMtx(1);
	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	        PARTICLE_MS_PKH_FUMI, mtx, 0, nullptr))
		emitter->setGlobalScale(mScaling);

	setBckAnm(0);

	SMSGetMSound()->startSoundActor(MSD_SE_BS_KRPAKU_DOWN, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TGorogoro::setMeltAnm()
{
	setBckAnm(1);
	unk130 = 0;
	onLiveFlag(LIVE_FLAG_UNK1000);

	MTXCopy(getMActor()->getModel()->getBaseTRMtx(), unk1B4);
	unk1B4.ref(1, 3) = mGroundHeight;

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	        PARTICLE_MS_PKH_TOKE, unk1B4, 0, nullptr))
		emitter->setGlobalScale(mScaling);

	SMSGetMSound()->startSoundActor(MSD_SE_BS_KRPAKU_SINK, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TGorogoro::bound()
{
	((TGorogoroManager*)mManager)->requestPolluteModel(mPosition, mScaling);

	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT) && SMS_IsMarioTouchGround4cm())
		SMSRumbleMgr->start(0x15, 10, &mPosition);
}

static const char* gorogoro_bastable[] = { nullptr, nullptr, nullptr, nullptr };

const char** TGorogoro::getBasNameTable() const { return gorogoro_bastable; }

bool TGorogoro::isRolling()
{
	if (mSpine->getCurrentNerve() == &TNerveGorogoroRollOnGraph::theNerve()
	    || isBckAnm(1))
		return true;

	return false;
}

void TGorogoro::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("bosspaku_head.bmd", 3);
}

void TGorogoro::setGenerateGraphIdx(int param_1)
{
	JGeometry::TVec3<f32> point;
	getTracer()->getGraph()->getGraphNode(param_1).getPoint(&point);
	mPosition = point;

	getTracer()->mCurrIdx = param_1;
	getTracer()->mPrevIdx = param_1 - 1;

	getTracer()->getGraph()->getGraphNode(param_1 + 1).getPoint(&point);

	JGeometry::TVec3<f32> dir;
	dir.sub(point, mPosition);
	mRotation.y = MsWrap(MsGetRotFromZaxisY(dir), 0.0f, 360.0f);

	setGoalPath(TPathNode(point));
}

void TGorogoro::generateByGateKeeper(const JGeometry::TVec3<f32>& param_1,
                                     const JGeometry::TVec3<f32>& param_2)
{
	reset();

	TGraphWeb* graph       = getTracer()->getGraph();
	int index              = graph->findNearestNodeIndex(param_1, 0xFFFFFFFF);
	getTracer()->mCurrIdx  = index;
	getTracer()->mPrevIdx  = index - 1;
	const TGraphNode& node = graph->getGraphNode(index);

	BOOL toMario;
	JGeometry::TVec3<f32> dir;
	if (MsIsInSight(param_1, param_2.y, SMS_GetMarioPos(), 2000.0f, 360.0f,
	                -1.0f)) {
		toMario = TRUE;
		dir     = SMS_GetMarioPos();
	} else {
		node.getPoint(&dir);
		toMario = FALSE;
	}

	dir -= param_1;
	mPosition = param_1;

	if (!dir.isZero()) {
		VECNormalize(&dir, &dir);

		Mtx mtx;
		MsMtxSetRotY(mtx, MsRandF() * 30.0f - 15.0f);
		MTXMultVec(mtx, &dir, &dir);
		mRotation.y = MsWrap(MsGetRotFromZaxisY(dir), 0.0f, 360.0f);

		dir.scale(1500.0f * MsRandF());
		dir += param_1;
		mVelocity = calcVelocityToJumpToY(dir, 15.0f, getGravityY());
	} else {
		mVelocity   = calcVelocityToJumpToY(param_1, 15.0f, getGravityY());
		mRotation.y = MsWrap(MsGetRotFromZaxisY(dir), 0.0f, 360.0f);
	}

	if (toMario) {
		TPathNode goal(SMS_GetMarioPos());
		unk114.push(unkF4);
		unkF4 = goal;
	}

	unk1A8 = true;
}

DEFINE_NERVE(TNerveGorogoroRollOnGraph, TLiveActor)
{
	TGorogoro* self = (TGorogoro*)spine->getBody();

	if (spine->getTime() == 0) {
		self->goToShortestNextGraphNode();
		self->setBckAnm(2);
	}

	if (self->isReachedToGoalXZ()) {
		if (self->jumpToNextGraphNode() >= 0)
			self->flagJump();
		else
			self->goToShortestNextGraphNode();
	}

	self->walkBehavior(2, 1.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveGorogoroDie, TLiveActor)
{
	TGorogoro* self = (TGorogoro*)spine->getBody();

	if (spine->getTime() < 2) {
		self->onHitFilter(HIT_FILTER_NO_COLLISION);

		if (self->getGroundPlane()->isWaterSurface() && !self->isAirborne())
			self->generateEffectColumWater();

		if (self->checkLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH)) {
			self->setMeltAnm();
		} else {
			((TGorogoroManager*)self->mManager)
			    ->requestPolluteModel(self->mPosition, self->mScaling);
			self->setDeadAnm();
			self->setDeadEffect();
		}
	} else if (self->checkCurAnmEnd(0) || spine->getTime() > 360) {
		self->onHitFilter(HIT_FILTER_NO_COLLISION);
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		self->mHolder = nullptr;
		self->stopAnmSound();

		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());

		self->genRandomItem();
		return TRUE;
	}

	if (self->checkLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH))
		self->walkBehavior(2, 0.5f);

	return FALSE;
}
