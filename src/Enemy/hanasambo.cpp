#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/HanaSambo.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <Map/Map.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/Application.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Particles.hpp>
#include <System/Params.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSamboFlowerCoinUnit::TSamboFlowerCoinUnit(int param_1) { }

void TSamboFlowerCoinUnit::add(TSamboFlower* param_1) { }

bool TSamboFlowerCoinUnit::checkGenCoin() { return false; }

TSamboLeaf::TSamboLeaf(TSamboFlowerManager* param_1, SDLModelData* param_2,
                       const char* param_3)
    : JDrama::TViewObj(param_3)
{
}

void TSamboLeaf::perform(u32 param_1, JDrama::TGraphics* param_2) { }

void TSamboLeaf::generate(JGeometry::TVec3<f32>& param_1) { }

TSamboFlowerSaveLoadParams::TSamboFlowerSaveLoadParams(const char* param_1)
    : TSpineEnemyParams(param_1)
    , PARAM_INIT(mSLLeafVelocityXZ, 4.0f)
    , PARAM_INIT(mSLLeafVelocityY, 6.0f)
    , PARAM_INIT(mSLLeafGravity, 0.2f)
    , PARAM_INIT(mSLBudDist, 2000.0f)
    , PARAM_INIT(mSLBloomTimer, 300)
    , PARAM_INIT(mSLCoinCircleR, 100.0f)
    , PARAM_INIT(mSLCoinVelocityXZ, 12.0f)
    , PARAM_INIT(mSLCoinVelocityY, 10.0f)
    , PARAM_INIT(mSLSeedShootRange, 500.0f)
    , PARAM_INIT(mSLSeedShootInterval, 200)
    , PARAM_INIT(mSLSeedGravity, 0.1f)
    , PARAM_INIT(mSLSeedSpeedXZ, 10.0f)
    , PARAM_INIT(mSLSeedSpeedY, 10.0f)
{
	TParams::load(mPrmPath);
}

void TSamboFlowerManager::load(JSUMemoryInputStream& param_1)
{
	TEnemyManager::load(param_1);
	unk38 = new TSamboFlowerSaveLoadParams("/enemy/samboflower.prm");
	unk64 = J3DModelLoaderDataBase::loadMaterialTable(
	    JKRFileLoader::getGlbResource("/scene/samboflower/flower_orange.bmt"));
}

void TSamboFlowerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "flower.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TSamboFlowerManager::createEnemyInstance()
{
	return new TSamboFlower("サンボフラワー");
}

void TSamboFlowerManager::loadAfter() { }

void TSamboFlowerManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk58 > 0 && param_1 & 2) {
		for (int i = 0; i < unk58; ++i)
			unk54[i]->checkGenCoin();
	}

	TEnemyManager::perform(param_1, param_2);

	for (int i = 0; i < 0x12; ++i)
		unk60[i]->perform(param_1, param_2);
}

void TSamboFlowerManager::dropLeaf(JGeometry::TVec3<f32>& param_1,
                                   JGeometry::TVec3<f32>& param_2)
{
}

void TSamboFlower::load(JSUMemoryInputStream& param_1)
{
	TSpineEnemy::load(param_1);
	param_1.read(&unk15C, 4);
	param_1.read(&unk158, 4);
	unk160 = true;
	reset();
	offLiveFlag(LIVE_FLAG_UNK800);
}

void TSamboFlower::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	if (unk15C < 0)
		return;

	TMapObjBase* obj = TMapObjBaseManager::newAndRegisterObj(
	    "coin", JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	    JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f),
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	if (obj)
		unk168 = obj;
}

void TSamboFlower::init(TLiveManager* param_1)
{
	mManager = param_1;
	param_1->manageActor(this);
	setMActorAndKeeper();
	unk130 = 1;
	unk16C = getSaveParam();
	if (unk16C) {
		mBodyRadius       = unk16C->mSLBodyRadius.get();
		mWallRadius       = unk16C->mSLWallRadius.get();
		mHeadHeight       = unk16C->mSLHeadHeight.get();
		mScaledBodyRadius = mBodyScale * mBodyRadius;
	}

	initHitActor(0x10000027, 1, 0x80000000, mBodyRadius, mHeadHeight,
	             mBodyRadius, mHeadHeight);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK400);
	initAnmSound();
	mActorType = 0x10000027;
	unk150     = false;
	onLiveFlag(LIVE_FLAG_DEAD);
}

void TSamboFlower::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("flower.bmd", 3);

	MActor* actor            = mMActor;
	J3DMaterialTable* table  = ((TSamboFlowerManager*)mManager)->unk64;
	J3DModelData* model_data = actor->getModel()->getModelData();
	model_data->setMaterialTable(table, J3DMatCopyFlag_All);
	actor->initDL();
	actor->getModel()->lock();
}

BOOL TSamboFlower::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (!unk150) {
			unk150 = true;
			unk154 = 0;
			gpMarioParticleManager->emit(0xB2, &mPosition, 0, nullptr);
			mMActor->setBck("flower_hit");
			if (unk160 && unk164) {
				unk164->unk0 = unk164->unk0 - 1;
				u32 id       = MSD_SE_OBJ_FLOWER_OPEN_0 + unk164->unk0;
				if (gpMSound->gateCheck(id))
					MSoundSESystem::MSoundSE::startSoundActor(id, &mPosition, 0,
					                                          nullptr, 0, 4);
			}
		}
		return 1;
	}

	return 0;
}

void TSamboFlower::reset()
{
	TSpineEnemy::reset();
	mMActor->setBck("flower_wait");
	offHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK800);
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_UNK10);
}

void TSamboFlower::moveObject()
{
	if (unk150) {
		if (checkCurAnmEnd(0) && getMActor()->checkCurAnm("flower_hit", 0))
			getMActor()->setBck("flower_fwait");

		if (unk160) {
			unk154++;
			if (unk154
			    > ((TSamboFlowerSaveLoadParams*)unk16C)->mSLBloomTimer.get()) {
				if (getMActor()->checkCurAnm("flower_fwait", 0)) {
					unk150 = false;
					if (unk164)
						unk164->unk0 = unk164->unk0 + 1;

					getMActor()->setBck("flower_hit");
					getMActor()
					    ->getFrameCtrl(ANM_TYPE_BCK)
					    ->setFrame(
					        getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getEnd());
					getMActor()->setFrameRate(-SMSGetAnmFrameRate(),
					                          ANM_TYPE_BCK);
				} else if (getMActor()->getFrameCtrl(ANM_TYPE_BCK)->getFrame()
				           < 1.0f) {
					unk154 = 0;
					getMActor()->setBck("flower_wait");
				}
			}
		}
	}

	if (!checkLiveFlag(LIVE_FLAG_UNK10))
		mPosition.y = gpMap->checkGround(mPosition.x, mPosition.y + 100.0f,
		                                 mPosition.z, &mGroundPlane);
}

void TSamboFlower::drawObject(JDrama::TGraphics* param_1)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	TCircleShadowRequest request;
	request.mPosition = mPosition;
	request.mRadiusX = request.mRadiusZ = 120.0f;
	gpBindShadowManager->request(request, getActorType() & ACTOR_TYPE_MASK);
	mMActor->setLightData(mGroundPlane, mPosition);
	mMActor->entry();
}

void TSamboFlower::bloom() { }

bool TSamboFlower::isBloomEnd() { return false; }

void TSamboFlower::hide() { }

// TODO: default values are placeholders; names and offsets are from the object.
THanaSamboSaveLoadParams::THanaSamboSaveLoadParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mSLAttackDist, 0.0f)
    , PARAM_INIT(mSLAttackInterval, 0)
    , PARAM_INIT(mSLHideDist, 0.0f)
    , PARAM_INIT(mSLAppearDist, 0.0f)
    , PARAM_INIT(mSLAttackingTime, 0)
    , PARAM_INIT(mSLHeadAttackRadius, 0.0f)
    , PARAM_INIT(mSLHeadAttackHeight, 0.0f)
    , PARAM_INIT(mSLHeadDamageRadius, 0.0f)
    , PARAM_INIT(mSLHeadDamageHeight, 0.0f)
{
	TParams::load(mPrmPath);
}

THanaSamboManager::THanaSamboManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void THanaSamboManager::load(JSUMemoryInputStream& param_1) { }

TSpineEnemy* THanaSamboManager::createEnemyInstance()
{
	return new THanaSambo("ハナサンボ");
}

void THanaSamboManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "sambo.bmd", 0x10220000, 0 },
		{ "samboD.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

BOOL THanaSamboHead::receiveMessage(THitActor* param_1, u32 param_2)
{
	if (param_2 == HIT_MESSAGE_TRAMPLE || param_2 == HIT_MESSAGE_HIP_DROP) {
		unk68->kill();
		return 1;
	}

	if (param_2 == HIT_MESSAGE_SPRAYED_BY_WATER) {
		unk68->waterDamage();
		return 1;
	}

	return 0;
}

bool THanaSamboHead::checkHit() { return false; }

void THanaSamboHead::kill() { }

THanaSambo::THanaSambo(const char* param_1)
    : TSmallEnemy(param_1)
    , unk194(nullptr)
    , unk198(0)
    , unk1A8(0)
    , unk1AC(0)
{
}

void THanaSambo::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	reset();
	setGoalPathMario();
}

void THanaSambo::init(TLiveManager* param_1) { }

void THanaSambo::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("sambo.bmd", 3);
	mMActorKeeper->createMActor("samboD.bmd", 3);
}

u8 THanaSambo::mHeadJntIndex   = 3;
u8 THanaSambo::mPollenJntIndex = 6;

void THanaSambo::moveObject()
{
	TSmallEnemy::moveObject();

	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		unk194->mPosition = mPosition;
	} else {
		MtxPtr mtx          = getModel()->getAnmMtx(mHeadJntIndex);
		unk194->mPosition.x = mtx[0][3];
		unk194->mPosition.y = mtx[1][3];
		unk194->mPosition.z = mtx[2][3];
	}

	THitActor* head = unk194;
	for (int i = 0; i < head->getColNum(); ++i) {
		if (head->getCollision(i)->isActorType(0x80000001))
			SMS_SendMessageToMario(head, HIT_MESSAGE_ATTACK);
	}

	mPosition.x = unk19C.x;
	mPosition.z = unk19C.z;
}

void THanaSambo::drawObject(JDrama::TGraphics* param_1)
{
	TLiveActor::drawObject(param_1);
	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8))
		unk1AC->entryDrawShadow();
}

void THanaSambo::kill()
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;

	mHitPoints = 1;
	if (isBckAnm(7))
		unk18C = 3;

	if (mSpine->getCurrentNerve() != &TNerveHanaSamboDie::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveHanaSamboDie::theNerve());
		mSpine->pushAfterCurrent(&TNerveHanaSamboDie::theNerve());
	}

	unk194->onHitFlag(HIT_FLAG_NO_COLLISION);
	onLiveFlag(LIVE_FLAG_UNK40);
}

void THanaSambo::reset()
{
	TSmallEnemy::reset();
	unk165 = false;
	unk19C = mPosition;
}

void THanaSambo::setWaitAnm()
{
	setBckAnm(7);
	unk1B0 = 0;
}

void THanaSambo::setDeadAnm()
{
	mMActor = getActorKeeper()->getMActor("samboD.bmd");
	if (unk1B0)
		setBckAnm(10);
	else
		setBckAnm(0);

	unk194->onHitFlag(HIT_FLAG_NO_COLLISION);
	unk1B0 = 0;
	onLiveFlag(LIVE_FLAG_UNK8);
}

void THanaSambo::setAttackAnm()
{
	setBckAnm(3);
	unk1B0 = 1;
}

void THanaSambo::waterDamage()
{
	unk165 = true;

	if (changeByJuice())
		return;

	if (mSpine->getCurrentNerve() != &TNerveHanaSamboWait::theNerve())
		return;

	mSpine->pushNerve(&TNerveHanaSamboFreeze::theNerve());
}

static const char* sambo_bastable[] = {
	"/scene/sambo/bas/sambo_down.bas",
	0,
	0,
	0,
	"/scene/sambo/bas/sambo_Fhide.bas",
	0,
	"/scene/sambo/bas/sambo_Fset.bas",
	"/scene/sambo/bas/sambo_Fwait.bas",
	"/scene/sambo/bas/sambo_hit.bas",
	0,
	"/scene/sambo/bas/sambo_Ydown.bas",
};

const char** THanaSambo::getBasNameTable() const { return sambo_bastable; }

void THanaSambo::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSmallEnemy::perform(param_1, param_2);
	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                   | LIVE_FLAG_CLIPPED_OUT))
		unk194->THitActor::perform(param_1, param_2);
}

void THanaSambo::createPollen()
{
	MtxPtr jointMtx = getMActor()->getModel()->getAnmMtx(mPollenJntIndex);
	JGeometry::TVec3<f32> pos;
	pos.x = jointMtx[0][3];
	pos.y = jointMtx[1][3];
	pos.z = jointMtx[2][3];

	Mtx mtx;
	MsMtxSetRotRPH(mtx, 0.0f, 0.0f, -90.0f);
	MTXConcat(jointMtx, mtx, mtx);

	JPABaseEmitter* emitter;
	if (mSpine->getCurrentNerve() == &TNerveHanaSamboWait::theNerve()) {
		emitter = gpMarioParticleManager->emit(0xB2, &pos, 0, nullptr);
		mtx[1][3] += 200.0f;
	} else {
		emitter = gpMarioParticleManager->emit(0xB3, &pos, 0, nullptr);

		JGeometry::TVec3<f32> offset(0.0f, 0.0f, 200.0f);
		Mtx rot;
		MsMtxSetRotRPH(rot, mRotation.x, mRotation.y, mRotation.z);
		MTXMultVec(rot, &offset, &offset);
		mtx[0][3] += offset.x;
		mtx[1][3] += offset.y;
		mtx[2][3] += offset.z;
	}

	if (emitter)
		emitter->setGlobalSRTMatrix(mtx);
}

void THanaSambo::initFlower() { }

DEFINE_NERVE(TNerveHanaSamboAppear, TLiveActor) { return false; }

DEFINE_NERVE(TNerveHanaSamboWait, TLiveActor) { return false; }

DEFINE_NERVE(TNerveHanaSamboAttack, TLiveActor)
{
	THanaSambo* self = (THanaSambo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setAttackAnm();
	} else if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(3)) {
			self->createPollen();
			SMSGetMSound()->startSoundActor(MSD_SE_EN_SAMBO_ATTACK,
			                                &self->mPosition, 0, nullptr, 0, 4);

			self->setBckAnm(1);
		} else if (self->isBckAnm(1)) {
			if (spine->getTime() > self->unk198->mSLAttackingTime.get()
			    && !self->unsetUnk165())
				self->setBckAnm(2);
			else
				self->setBckAnm(1);
		} else {
			spine->pushAfterCurrent(&TNerveHanaSamboWait::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveHanaSamboHide, TLiveActor) { return false; }

DEFINE_NERVE(TNerveHanaSamboDie, TLiveActor)
{
	THanaSambo* self = (THanaSambo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->setDeadAnm();
	} else if (self->checkCurAnmEnd(0) || spine->getTime() > 300) {
		static int jIndexTable[] = { 1, 3, 4, 5 };

		for (int i = 0; i < 4; ++i) {
			MtxPtr mtx
			    = self->getMActor()->getModel()->getAnmMtx(jIndexTable[i]);
			self->unk1B4[i].set(mtx[0][3], mtx[1][3], mtx[2][3]);

			if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			        PARTICLE_MS_ENM_DISAP_A, &self->unk1B4[i], 0, nullptr))
				emitter->setGlobalScale(self->mScaling);

			if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
			        PARTICLE_MS_ENM_DISAP_B, &self->unk1B4[i], 0, nullptr))
				emitter->setGlobalScale(self->mScaling);
		}

		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		self->mHolder = nullptr;
		self->stopAnmSound();
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		self->genRandomItem();
	}

	return false;
}

DEFINE_NERVE(TNerveHanaSamboFreeze, TLiveActor)
{
	THanaSambo* self = (THanaSambo*)spine->getBody();

	if (spine->getTime() == 0)
		self->setBckAnm(5);

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(5)) {
			if (self->unsetUnk165())
				self->setBckAnm(8);
		} else if (self->unsetUnk165()) {
			self->setBckAnm(8);
		} else {
			return true;
		}
	}

	return false;
}

static int SamboHeadRollCallback(J3DNode* param_1, int param_2) { return 0; }

// TODO: default values are placeholders; names and offsets are from the object.
TSamboHeadSaveLoadParams::TSamboHeadSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLAppearDist, 0.0f)
    , PARAM_INIT(mSLHideDist, 0.0f)
    , PARAM_INIT(mSLMoveDist, 0.0f)
    , PARAM_INIT(mSLMoveGravity, 0.0f)
    , PARAM_INIT(mSLJumpSp, 0.0f)
    , PARAM_INIT(mSLJumpPrepareTime, 0)
    , PARAM_INIT(mSLHitJumpSpXZ, 0.0f)
    , PARAM_INIT(mSLHitJumpSpY, 0.0f)
    , PARAM_INIT(mSLHitJumpGravity, 0.0f)
    , PARAM_INIT(mSLHitJumpSpRateXZ, 0.0f)
    , PARAM_INIT(mSLHitJumpSpRateY, 0.0f)
    , PARAM_INIT(mSLJumpAngY, 0.0f)
{
	TParams::load(mPrmPath);
}

u8 TSamboHead::mBodyJntIndex;

static TSamboHead* gpCurSamboHead;

TSamboHeadManager::TSamboHeadManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
	gpCurSamboHead = nullptr;
}

void TSamboHeadManager::load(JSUMemoryInputStream& param_1) { }

void TSamboHeadManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "samboHead.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TSamboHeadManager::createEnemyInstance()
{
	return new TSamboHead("サンボヘッド");
}

TSamboHead::TSamboHead(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(nullptr)
    , unk198(0)
    , unk19C(0)
    , unk1AC(0.0f)
    , unk1B0(0)
{
}

void TSamboHead::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	reset();
	setGoalPathMario();
}

void TSamboHead::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x1000001B;
	unk150     = 0x11;
	unk194     = getSaveParam();
	mSpine->initWith(&TNerveSamboHeadHide::theNerve());
	setGoalPathMario();
	mMActor->setJointCallback(mBodyJntIndex, SamboHeadRollCallback);
}

void TSamboHead::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("samboHead.bmd", 3);
}

void TSamboHead::reset()
{
	gpCurSamboHead = this;
	TWalkerEnemy::reset();
	unk165 = false;
	unk1B0 = 0;
	unk19C = 0;
	unk1AC = 0.0f;
	offLiveFlag(LIVE_FLAG_UNK8);
}

void TSamboHead::kill()
{
	mHitPoints = 1;
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()) {
		mSpine->reset();
		mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
		mSpine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
	}
	onLiveFlag(LIVE_FLAG_UNK40);
}

void TSamboHead::behaveToWater(THitActor* param_1) { }

void TSamboHead::attackToMario()
{
	sendAttackMsgToMario();

	if (isAirborne()) {
		JGeometry::TVec3<f32> velocity(mPosition.x - gpMarioPos->x, 10.0f,
		                               mPosition.z - gpMarioPos->z);
		MsVECNormalize(&velocity, &velocity);
		velocity.scale(8.0f);
		setVelocity(velocity);
	} else if (mSpine->getCurrentNerve()
	           == &TNerveSamboHeadAttack::theNerve()) {
		mSpine->pushNerve(&TNerveSmallEnemyFreeze::theNerve());
	}
}

f32 TSamboHead::getGravityY() const
{
	f32 gravity = mGravity;

	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAttack::theNerve())
		gravity = ((TSamboHeadSaveLoadParams*)unk194)->mSLMoveGravity.get();

	if (mSpine->getCurrentNerve() == &TNerveSamboHeadHitWater::theNerve())
		gravity = ((TSamboHeadSaveLoadParams*)unk194)->mSLHitJumpGravity.get();

	return gravity;
}

void TSamboHead::initFlower() { }

void TSamboHead::setDeadAnm() { setBckAnm(3); }

void TSamboHead::setAfterDeadEffect()
{
	JGeometry::TVec3<f32> scale(1.5f);

	if (isBckAnm(1)) {
		if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		        PARTICLE_MS_ENM_DISAP_A_W, &mPosition, 0, nullptr))
			emitter->setGlobalScale(scale);
	} else {
		if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
		        PARTICLE_MS_ENM_DISAP_A, &mPosition, 0, nullptr))
			emitter->setGlobalScale(scale);
	}

	if (JPABaseEmitter* emitter = gpMarioParticleManager->emit(
	        PARTICLE_MS_ENM_DISAP_B, &mPosition, 0, nullptr))
		emitter->setGlobalScale(scale);

	if (gpMSound->gateCheck(MSD_SE_EN_COMMON_SMOKE))
		MSoundSESystem::MSoundSE::startSoundActor(MSD_SE_EN_COMMON_SMOKE,
		                                          &mPosition, 0, nullptr, 0, 4);
}

void TSamboHead::setCrashAnm() { }

void TSamboHead::calcRootMatrix()
{
	gpCurSamboHead = this;
	TSpineEnemy::calcRootMatrix();
}

void TSamboHead::genEventCoin() { }

bool TSamboHead::isUseCallBack() { return false; }

static const char* sambohead_bastable[] = {
	"/scene/sambohead/bas/flower_shoot.bas",
	"/scene/sambohead/bas/samboHead_crash.bas",
	"/scene/sambohead/bas/samboHead_dance.bas",
	"/scene/sambohead/bas/samboHead_down.bas",
	"/scene/sambohead/bas/samboHead_Fhide.bas",
	"/scene/sambohead/bas/samboHead_hit.bas",
	"/scene/sambohead/bas/samboHead_hit_end.bas",
	"/scene/sambohead/bas/samboHead_jump_end.bas",
	"/scene/sambohead/bas/samboHead_jump_start.bas",
	0,
	"/scene/sambohead/bas/samboHead_set.bas",
	"/scene/sambohead/bas/samboHead_turn.bas",
	0,
};

const char** TSamboHead::getBasNameTable() const { return sambohead_bastable; }

DEFINE_NERVE(TNerveSamboHeadAppear, TLiveActor) { return false; }

DEFINE_NERVE(TNerveSamboHeadAttack, TLiveActor) { return false; }

DEFINE_NERVE(TNerveSamboHeadHide, TLiveActor) { return false; }

DEFINE_NERVE(TNerveSamboHeadHitWater, TLiveActor) { return false; }

DEFINE_NERVE(TNerveSamboHeadRecoverWater, TLiveActor)
{
	TSamboHead* self = static_cast<TSamboHead*>(spine->getBody());
	if (spine->getTime() == 0)
		self->setBckAnm(12);

	self->unk1AC *= 0.99f;
	if (self->checkCurAnmEnd(0) && self->unk1AC < 1.0f)
		return true;
	return false;
}

DEFINE_NERVE(TNerveSamboHeadHitWall, TLiveActor) { return false; }
