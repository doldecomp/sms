#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/HanaSambo.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
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
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
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
#include <System/Application.hpp>
#include <System/BaseParam.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/ParamInst.hpp>
#include <System/Particles.hpp>
#include <System/Params.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TSamboFlowerCoinUnit::TSamboFlowerCoinUnit(int param_1)
    : unk0(nullptr)
    , unk10(0)
    , unk14(param_1)
    , unk18(nullptr)
    , unk1C(param_1)
{
	unk0 = new TSamboFlower*[param_1];
}

void TSamboFlowerCoinUnit::add(TSamboFlower* param_1)
{
	if (unk10 < unk14) {
		unk0[unk10]     = param_1;
		param_1->unk164 = &unk1C;
		unk10++;
	}
}

void TSamboFlowerCoinUnit::checkGenCoin()
{
	if (!unk18)
		return;

	bool bloomed = true;
	for (int i = 0; i < unk10; ++i) {
		bool bloomEnd = unk0[i]->isBloomEnd();
		if (!bloomEnd)
			bloomed = false;
	}

	if (!bloomed)
		return;

	int total = 0;
	for (int i = 0; i < unk10; ++i) {
		TSamboFlower* flower = unk0[i];
		if (flower->unk168)
			total++;

		flower->unk160 = false;
	}

	if (total > 0) {
		Mtx mtx;
		int spawned = 0;
		for (int i = 0; i < unk10; ++i) {
			if (!unk0[i]->unk168)
				continue;

			f32 rate = (f32)spawned / (f32)total;

			JGeometry::TVec3<f32> offset(
			    0.0f, 0.0f,
			    ((TSamboFlowerSaveLoadParams*)unk0[i]->unk16C)
			        ->mSLCoinCircleR.get());

			MsMtxSetRotY(mtx, 360.0f * rate);
			MTXMultVec(mtx, &offset, &offset);

			TMapObjBase* coin = unk0[i]->unk168;
			if (coin->isActorType(0x2000000E))
				coin = gpItemManager->makeObjAppear(0x2000000E);

			if (coin) {
				coin->appear();

				JGeometry::TVec3<f32> pos = unk4;
				pos += offset;
				coin->mPosition = pos;

				MsVECNormalize(&offset, &offset);

				TSamboFlowerSaveLoadParams* prm
				    = (TSamboFlowerSaveLoadParams*)unk0[i]->unk16C;
				coin->mVelocity.set(offset.x * prm->mSLCoinVelocityXZ.get(),
				                    8.0f * rate + prm->mSLCoinVelocityY.get(),
				                    offset.z * prm->mSLCoinVelocityXZ.get());
				coin->offLiveFlag(LIVE_FLAG_UNK10);
				spawned++;
			}
		}

		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_COIN_APPEAR, spawned,
		                                   nullptr, 0);
	}

	unk18 = nullptr;
}

TSamboLeaf::TSamboLeaf(TSamboFlowerManager* param_1, SDLModelData* param_2,
                       const char* param_3)
    : JDrama::TViewObj(param_3)
    , unk10(nullptr)
    , unk44(0)
    , unk48(param_1)
{
	unk10 = new SDLModel(param_2, 3, 1);
}

void TSamboLeaf::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (!unk44)
		return;

	if (param_1 & CUE_MOVE) {
		unk14 += unk38;

		if (unk38.y > -20.0f)
			unk38.y -= ((TSamboFlowerSaveLoadParams*)unk48->unk38)
			               ->mSLLeafGravity.get();

		const TBGCheckData* ground;
		if (unk14.y
		    < gpMap->checkGround(unk14.x, 20.0f + unk14.y, unk14.z, &ground))
			unk44 = false;
	}

	if (param_1 & CUE_CALC_ANIM) {
		Mtx mtx;
		MsMtxSetXYZRPH(mtx, unk14.x, unk14.y, unk14.z, 0.0f, unk20.y, 0.0f);

		const JGeometry::TVec3<f32>& rot = MsGetRotFromZaxis(unk38);
		Mtx rotMtx;
		MsMtxSetRotZ(rotMtx, -rot.x);
		MTXConcat(mtx, rotMtx, mtx);

		unk10->setBaseTRMtx(mtx);
		unk10->setBaseScale(unk2C);
		unk10->calc();
	}

	if (param_1 & CUE_CALC_VIEW)
		unk10->viewCalc();

	if (param_1 & CUE_ENTRY)
		unk10->entry();
}

void TSamboLeaf::generate(JGeometry::TVec3<f32>& param_1)
{
	unk14 = param_1;
	unk14.y += 10.0f;
	unk44 = true;
}

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
	return new TSamboFlower;
}

void TSamboFlowerManager::loadAfter()
{
	void* leafRes
	    = JKRFileLoader::getGlbResource("/scene/samboflower/leaf.bmd");
	SDLModelData* leafData
	    = new SDLModelData(J3DModelLoaderDataBase::load(leafRes, 0x10210000));

	unk60 = new TSamboLeaf*[0x12];
	for (int i = 0; i < 0x12; i++)
		unk60[i] = new TSamboLeaf(this, leafData);

	unk58 = 0;
	for (int i = 0; i < gpItemManager->getObjNum(); i++) {
		if (strstr(gpItemManager->getObj(i)->getName(), "コイン（フラワー用）"))
			unk58++;
	}

	unk54 = new TSamboFlowerCoinUnit*[unk58];

	int* counts = new int[unk58];
	for (int i = 0; i < unk58; i++)
		counts[i] = 0;

	for (int i = 0; i < getObjNum(); i++) {
		if (strstr(getObj(i)->getName(), "フラワー（コイン用）")) {
			TSamboFlower* flower = (TSamboFlower*)getObj(i);
			if (flower->unk158 < unk58)
				counts[flower->unk158]++;
		}
	}

	for (int i = 0; i < unk58; i++)
		unk54[i] = new TSamboFlowerCoinUnit(counts[i]);

	for (int i = 0; i < gpItemManager->getObjNum(); i++) {
		if (strstr(gpItemManager->getObj(i)->getName(),
		           "コイン（フラワー用）")) {
			TFlowerCoin* coin = (TFlowerCoin*)gpItemManager->getObj(i);
			int idx           = coin->unk158;
			if (idx < unk58) {
				unk54[idx]->unk18 = coin;
				unk54[idx]->unk4  = coin->mPosition;
				coin->kill();
			}
		}
	}

	for (int i = 0; i < getObjNum(); i++) {
		if (strstr(getObj(i)->getName(), "フラワー（コイン用）")) {
			TSamboFlower* flower = (TSamboFlower*)getObj(i);
			if (flower->unk158 < unk58)
				unk54[flower->unk158]->add(flower);
		}
	}

	JDrama::TNameRef::loadAfter();
}

void TSamboFlowerManager::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (unk58 > 0 && param_1 & 2) {
		for (int i = 0; i < unk58; i++)
			unk54[i]->checkGenCoin();
	}

	TEnemyManager::perform(param_1, param_2);

	for (int i = 0; i < 0x12; ++i)
		unk60[i]->perform(param_1, param_2);
}

void TSamboFlowerManager::dropLeaf(JGeometry::TVec3<f32>& param_1,
                                   JGeometry::TVec3<f32>& param_2)
{
	f32 rotY[] = { 0.0f, 120.0f, 240.0f };

	int dropped = 0;
	for (int i = 0; i < 0x12; i++) {
		TSamboLeaf* leaf = unk60[i];
		if (!leaf->unk44) {
			leaf->generate(param_1);

			TSamboFlowerSaveLoadParams* prm
			    = (TSamboFlowerSaveLoadParams*)unk38;
			f32 velXZ = prm->mSLLeafVelocityXZ.get();
			f32 velY  = prm->mSLLeafVelocityY.get();
			TMsRange<f32> speedXZ(velXZ, 1.2f * velXZ);
			TMsRange<f32> speedY(velY, 1.2f * velY);

			JGeometry::TVec3<f32> velocity(0.0f, speedY.rand(), speedXZ.rand());

			Mtx mtx;
			MsMtxSetRotRPH(mtx, 0.0f, rotY[i], 0.0f);
			MTXMultVec(mtx, &velocity, &velocity);

			unk60[i]->unk20.set(0.0f, rotY[i] - 90.0f, 0.0f);
			unk60[i]->unk38 = velocity;
			unk60[i]->unk2C = param_2;
			dropped++;
		}

		if (dropped >= 3)
			break;
	}
}

void TSamboFlower::load(JSUMemoryInputStream& param_1)
{
	TSpineEnemy::load(param_1);
	param_1 >> unk15C;
	param_1 >> unk158;
	unk160 = true;
	reset();
	offLiveFlag(LIVE_FLAG_UNK800);
}

void TSamboFlower::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	if (unk15C < 0)
		return;

	TMapObjBase* obj = TMapObjBaseManager::newAndRegisterObj("coin");
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
		if (!unk150)
			bloom();
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
						*unk164 += 1;

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

void TSamboFlower::bloom()
{
	unk150 = true;
	unk154 = 0;
	gpMarioParticleManager->emit(PARTICLE_MS_SMB_KAFUN_OW, &mPosition, 0,
	                             nullptr);
	mMActor->setBck("flower_hit");
	if (unk160 && unk164) {
		*unk164 -= 1;
		u32 id = MSD_SE_OBJ_FLOWER_OPEN_0 + *unk164;
		SMSGetMSound()->startSoundActor(id, &mPosition, 0, nullptr, 0, 4);
	}
}

bool TSamboFlower::isBloomEnd()
{
	if (getMActor()->checkCurAnm("flower_fwait", 0))
		return true;

	return false;
}

void TSamboFlower::hide()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mMActor->setBck("flower_fwait");
	onLiveFlag(LIVE_FLAG_DEAD);
}

THanaSamboSaveLoadParams::THanaSamboSaveLoadParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mSLAttackDist, 200.0f)
    , PARAM_INIT(mSLAttackInterval, 200)
    , PARAM_INIT(mSLHideDist, 800.0f)
    , PARAM_INIT(mSLAppearDist, 1000.0f)
    , PARAM_INIT(mSLAttackingTime, 30)
    , PARAM_INIT(mSLHeadAttackRadius, 60.0f)
    , PARAM_INIT(mSLHeadAttackHeight, 20.0f)
    , PARAM_INIT(mSLHeadDamageRadius, 80.0f)
    , PARAM_INIT(mSLHeadDamageHeight, 40.0f)
{
	TParams::load(mPrmPath);
}

THanaSamboManager::THanaSamboManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void THanaSamboManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new THanaSamboSaveLoadParams("/enemy/hanasambo.prm");
}

TSpineEnemy* THanaSamboManager::createEnemyInstance() { return new THanaSambo; }

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

void THanaSamboHead::checkHit()
{
	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	}
}

void THanaSamboHead::kill() { onHitFlag(HIT_FLAG_NO_COLLISION); }

THanaSambo::THanaSambo(const char* param_1)
    : TSmallEnemy(param_1)
    , unk194(nullptr)
    , unk198(0)
    , unk1A8(nullptr)
    , unk1AC(0)
{
}

void THanaSambo::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemy::load(param_1);
	reset();
	setGoalPathMario();
}

void THanaSambo::init(TLiveManager* param_1)
{
	TSmallEnemy::init(param_1);
	mActorType = 0x1000001A;
	unk150     = 0x11;
	unk198     = (THanaSamboSaveLoadParams*)getSaveParam();
	mSpine->initWith(&TNerveHanaSamboHide::theNerve());
	unk1AC = new TMBindShadowBody(this, getModel(), 1.5f);
	setGoalPathMario();

	if (mInstanceIndex == 0) {
		// TODO: what this loop did is unknown
		for (u8 i = 0; i < getActorKeeper()
		                       ->getMActor("samboD.bmd")
		                       ->getModel()
		                       ->getModelData()
		                       ->getJointNum();
		     ++i) { }
	}

	unk194 = new THanaSamboHead;
	((TIdxGroupObj*)JDrama::TNameRefGen::search("敵グループ"))->add(unk194);
	unk194->initHitActor(0x1000001B, 2, 0x80000000,
	                     unk198->mSLHeadAttackRadius.get() * mBodyScale,
	                     unk198->mSLHeadAttackHeight.get() * mBodyScale,
	                     unk198->mSLHeadDamageRadius.get() * mBodyScale,
	                     unk198->mSLHeadDamageHeight.get() * mBodyScale);
	unk194->unk68 = this;
}

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

	unk194->checkHit();

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

	unk194->kill();
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

	unk194->kill();
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
		emitter = gpMarioParticleManager->emit(PARTICLE_MS_SMB_KAFUN_OW, &pos,
		                                       0, nullptr);
		mtx[1][3] += 200.0f;
	} else {
		emitter = gpMarioParticleManager->emit(PARTICLE_MS_SMB_KAFUN_OA, &pos,
		                                       0, nullptr);

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

void THanaSambo::initFlower()
{
	if (!unk1A8) {
		unk1A8 = (TSamboFlower*)gpConductor->makeOneEnemyAppear(
		    mPosition, "サンボフラワーマネージャー", 1);
		unk1A8->reset();
	}

	unk1A8->onLiveFlag(LIVE_FLAG_UNK10);
	unk1A8->offLiveFlag(LIVE_FLAG_DEAD);
	unk1A8->mPosition   = mPosition;
	unk1A8->mPosition.y = mGroundHeight;
}

DEFINE_NERVE(TNerveHanaSamboAppear, TLiveActor)
{
	THanaSambo* self = (THanaSambo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
		self->setBckAnm(6);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_ROCK, &self->mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_SMOKE, &self->mPosition,
		                             0, nullptr);
		self->unk1A8->hide();
	}

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveHanaSamboWait::theNerve());
		self->setWaitAnm();
		if (gpMarioPos->y < 100.0f + self->mPosition.y) {
			self->updateSquareToMario();
			if (self->mDistToMarioSquared
			    < self->unk198->mSLAttackDist.get()
			          * self->unk198->mSLAttackDist.get())
				spine->pushAfterCurrent(&TNerveHanaSamboAttack::theNerve());
		}
		return true;
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed() * 3.0f, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveHanaSamboWait, TLiveActor)
{
	THanaSambo* self = (THanaSambo*)spine->getBody();

	if (spine->getTime() == 0)
		self->setWaitAnm();

	if (!self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && self->getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(16.0f))
		self->createPollen();

	if (spine->getTime() > self->unk198->mSLAttackInterval.get()
	    && gpMarioPos->y < 100.0f + self->mPosition.y) {
		self->updateSquareToMario();
		if (self->mDistToMarioSquared
		    < self->unk198->mSLAttackDist.get()
		          * self->unk198->mSLAttackDist.get()) {
			spine->pushAfterCurrent(&TNerveHanaSamboAttack::theNerve());
			return true;
		}
	}

	self->updateSquareToMario();
	if (self->mDistToMarioSquared > self->unk198->mSLAppearDist.get()
	                                    * self->unk198->mSLAppearDist.get()) {
		spine->pushAfterCurrent(&TNerveHanaSamboHide::theNerve());
		return true;
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	return false;
}

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

DEFINE_NERVE(TNerveHanaSamboHide, TLiveActor)
{
	THanaSambo* self = (THanaSambo*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(4);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_HD_ROCK, &self->mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_HD_SMOKE, &self->mPosition,
		                             0, nullptr);
	}

	if (spine->getTime() == 75)
		self->initFlower();

	if (self->checkCurAnmEnd(0)) {
		self->initFlower();
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		self->setBckAnm(6);
		self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);
		self->onLiveFlag(LIVE_FLAG_HIDDEN);
	}

	self->updateSquareToMario();
	if (self->mDistToMarioSquared
	    < self->unk198->mSLHideDist.get() * self->unk198->mSLHideDist.get()) {
		spine->pushAfterCurrent(&TNerveHanaSamboAppear::theNerve());
		return true;
	}

	return false;
}

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

u8 TSamboHead::mBodyJntIndex;

static TSamboHead* gpCurSamboHead;

static int SamboHeadRollCallback(J3DNode* param_1, int param_2)
{
	if (param_2 == 0) {
		if (gpCurSamboHead == nullptr || !gpCurSamboHead->isUseCallBack())
			return true;

		J3DJoint* joint = (J3DJoint*)param_1;
		MtxPtr anmMtx
		    = gpCurSamboHead->getModel()->getAnmMtx(joint->getJntNo());

		JGeometry::TVec3<f32> velocity = gpCurSamboHead->mVelocity;
		if (0.0f == velocity.x && 0.0f == velocity.z)
			velocity.x = 0.001f;

		JGeometry::TVec3<f32> side;
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		VECCrossProduct(&up, &velocity, &side);

		JGeometry::TVec3<f32> zAxis(anmMtx[0][2], anmMtx[1][2], anmMtx[2][2]);
		JGeometry::TVec3<f32> yAxis(anmMtx[0][1], anmMtx[1][1], anmMtx[2][1]);
		JGeometry::TVec3<f32> xAxis(anmMtx[0][0], anmMtx[1][0], anmMtx[2][0]);

		f32 rateZ = 0.0f;
		if (0.0f != zAxis.squared())
			rateZ = side.dot(zAxis) / zAxis.squared();

		f32 rateY = 0.0f;
		if (0.0f != yAxis.squared())
			rateY = side.dot(yAxis) / yAxis.squared();

		f32 rateX = 0.0f;
		if (0.0f != xAxis.squared())
			rateX = side.dot(xAxis) / xAxis.squared();

		JGeometry::TVec3<f32> axis(rateX, rateY, rateZ);

		Mtx local_100;
		MTXRotAxisRad(local_100, &axis, 0.017453292f * gpCurSamboHead->unk1AC);
		MTXConcat(anmMtx, local_100, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, local_100, J3DSys::mCurrentMtx);
	}
	return true;
}

TSamboHeadSaveLoadParams::TSamboHeadSaveLoadParams(const char* param_1)
    : TWalkerEnemyParams(param_1)
    , PARAM_INIT(mSLAppearDist, 800.0f)
    , PARAM_INIT(mSLHideDist, 1000.0f)
    , PARAM_INIT(mSLMoveDist, 100.0f)
    , PARAM_INIT(mSLMoveGravity, 0.1f)
    , PARAM_INIT(mSLJumpSp, 10.0f)
    , PARAM_INIT(mSLJumpPrepareTime, 20)
    , PARAM_INIT(mSLHitJumpSpXZ, 12.0f)
    , PARAM_INIT(mSLHitJumpSpY, 10.0f)
    , PARAM_INIT(mSLHitJumpGravity, 1.0f)
    , PARAM_INIT(mSLHitJumpSpRateXZ, 0.3f)
    , PARAM_INIT(mSLHitJumpSpRateY, 0.3f)
    , PARAM_INIT(mSLJumpAngY, 30.0f)
{
	TParams::load(mPrmPath);
}

TSamboHeadManager::TSamboHeadManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
	gpCurSamboHead = nullptr;
}

void TSamboHeadManager::load(JSUMemoryInputStream& param_1)
{
	TSmallEnemyManager::load(param_1);
	unk38 = new TSamboHeadSaveLoadParams("/enemy/sambohead.prm");
}

void TSamboHeadManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "samboHead.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TSamboHeadManager::createEnemyInstance() { return new TSamboHead; }

TSamboHead::TSamboHead(const char* param_1)
    : TWalkerEnemy(param_1)
    , unk194(nullptr)
    , unk198(nullptr)
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

void TSamboHead::behaveToWater(THitActor* param_1)
{
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadHide::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAppear::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveSamboHeadHitWall::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	JGeometry::TVec3<f32> velocity = mLinearVelocity;
	velocity.y                     = 0.0f;

	JGeometry::TVec3<f32> jump(mPosition.x - gpMarioPos->x, 0.0f,
	                           mPosition.z - gpMarioPos->z);
	MsVECNormalize(&jump, &jump);
	jump.scale(((TSamboHeadSaveLoadParams*)unk194)->mSLHitJumpSpXZ.get());
	jump.y = ((TSamboHeadSaveLoadParams*)unk194)->mSLHitJumpSpY.get();

	if (mSpine->getCurrentNerve() != &TNerveSamboHeadHitWater::theNerve())
		mSpine->pushNerve(&TNerveSamboHeadHitWater::theNerve());
	else
		jump.add(velocity);

	setVelocity(jump);
	mPosition.y += 2.0f;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

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

void TSamboHead::initFlower()
{
	if (!unk198) {
		unk198 = (TSamboFlower*)gpConductor->makeOneEnemyAppear(
		    mPosition, "サンボフラワーマネージャー", 1);
		unk198->reset();
	}

	unk198->offHitFlag(HIT_FLAG_NO_COLLISION);
	unk198->onLiveFlag(LIVE_FLAG_UNK10);
	unk198->offLiveFlag(LIVE_FLAG_DEAD);
	unk198->mPosition.y = mGroundHeight;
	unk198->mPosition   = mPosition;
}

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

	SMSGetMSound()->startSoundActor(MSD_SE_EN_COMMON_SMOKE, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TSamboHead::setCrashAnm()
{
	if (unk198->unk150)
		unk198->bloom();

	setBckAnm(10);
}

void TSamboHead::calcRootMatrix()
{
	gpCurSamboHead = this;
	TSpineEnemy::calcRootMatrix();
}

void TSamboHead::genEventCoin()
{
	if (isBckAnm(1)) {
		for (int i = 0; i < 3; ++i) {
			Mtx local_38;
			Vec local_2c;
			f32 angle = 60.0f * i + (mRotation.y - 60.0f);
			MsMtxSetRotY(local_38, angle);

			local_2c.x = 0.0f;
			local_2c.y = 0.0f;
			local_2c.z = 100.0f;
			MTXMultVec(local_38, &local_2c, &local_2c);

			TCoin* coin;
			if (i == 1 && mCoin) {
				coin = mCoin;
				if (coin->isActorType(0x2000000E))
					coin = (TCoin*)gpItemManager->makeObjAppear(0x2000000E);

				if (coin) {
					coin->appear();
					coin->mPosition = mPosition;
				}
			} else {
				coin = (TCoin*)gpItemManager->makeObjAppear(
				    mPosition.x + local_2c.x, mPosition.y,
				    mPosition.z + local_2c.z, 0x2000000E, true);
			}

			if (coin) {
				coin->mPosition.y = mPosition.y;
				MsVECNormalize(&local_2c, &local_2c);
				coin->mVelocity.set(local_2c.x * 4,
				                    TMsRange<f32>(8.0f, 16.0f).rand(),
				                    local_2c.z * 4);
				coin->offLiveFlag(LIVE_FLAG_UNK10);
			}
		}
	} else {
		TSmallEnemy::genEventCoin();
	}
}

bool TSamboHead::isUseCallBack()
{
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSamboHeadHitWater::theNerve()
	    || mSpine->getCurrentNerve()
	           == &TNerveSamboHeadRecoverWater::theNerve())
		return true;

	return false;
}

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

DEFINE_NERVE(TNerveSamboHeadAppear, TLiveActor)
{
	TSamboHead* self = (TSamboHead*)spine->getBody();

	if (spine->getTime() == 0) {
		self->offLiveFlag(LIVE_FLAG_HIDDEN);
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
		if (self->unk198->unk150) {
			self->unk198->bloom();
			self->setBckAnm(10);
		} else {
			self->setBckAnm(10);
		}
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_ROCK, &self->mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_AP_SMOKE, &self->mPosition,
		                             0, nullptr);
		self->unk198->hide();
	}

	if (spine->getTime() == 20)
		((TSamboFlowerManager*)self->unk198->mManager)
		    ->dropLeaf(self->mPosition, self->mScaling);

	if (self->checkCurAnmEnd(0)) {
		self->setBckAnm(12);
		spine->pushAfterCurrent(&TNerveSamboHeadAttack::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveSamboHeadAttack, TLiveActor)
{
	TSamboHead* self = (TSamboHead*)spine->getBody();

	if (!self->isAirborne()) {
		if (self->unk19C > ((TSamboHeadSaveLoadParams*)self->unk194)
		                       ->mSLJumpPrepareTime.get()
		    && self->checkCurAnmEnd(0)) {
			self->unk19C = 0;
			self->updateSquareToMario();

			JGeometry::TVec3<f32> goal = self->getUnk104().getPoint();
			goal.set(gpMarioPos->x - self->mPosition.x, 0.0f,
			         gpMarioPos->z - self->mPosition.z);
			if (goal.x == 0.0f && goal.y == 0.0f && goal.z == 0.0f)
				goal.x += 1.0f;

			MsVECNormalize(&goal, &goal);

			f32 moveDist
			    = ((TSamboHeadSaveLoadParams*)self->unk194)->mSLMoveDist.get();
			goal.x = goal.x * moveDist + self->mPosition.x;
			goal.z = goal.z * moveDist + self->mPosition.z;
			goal.y = self->mPosition.y;

			f32 jumpSp
			    = ((TSamboHeadSaveLoadParams*)self->unk194)->mSLJumpSp.get();
			self->setVelocity(
			    self->calcVelocityToJumpToY(goal, jumpSp, self->getGravityY()));
			self->mPosition.y += 2.0f;
			self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			self->setBckAnm(8);
		} else {
			self->unk19C++;
		}

		if (self->checkCurAnmEnd(0) && self->isBckAnm(7))
			self->setBckAnm(12);

		self->getMActor()->setFrameRate(SMSGetAnmFrameRate(), ANM_TYPE_BCK);
	} else {
		JGeometry::TVec3<f32> velocity = self->mLinearVelocity;
		if (velocity.y < 0.0f && self->isBckAnm(8)) {
			self->setBckAnm(7);
			self->getMActor()->setFrameRate(0.0f, ANM_TYPE_BCK);
		}
	}

	if (self->mPosition.y > 30.0f + self->mGroundHeight) {
		f32 limit
		    = ((TSamboHeadSaveLoadParams*)self->unk194)->mSLJumpAngY.get();
		JGeometry::TVec3<f32> velocity = self->mLinearVelocity;
		self->unk1AC = MsClamp(MsGetRotFromZaxis(velocity).x, -limit, limit);
	} else {
		self->unk1AC *= 0.8f;
	}

	f32 turnSpeed = self->getTurnSpeed();
	if (self->isAirborne())
		turnSpeed = 5.0f;

	self->walkToCurPathNode(0.0f, turnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveSamboHeadHide, TLiveActor)
{
	TSamboHead* self = (TSamboHead*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(4);
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_HD_ROCK, &self->mPosition,
		                             0, nullptr);
		gpMarioParticleManager->emit(PARTICLE_MS_SMB_HD_SMOKE, &self->mPosition,
		                             0, nullptr);
	} else if (self->checkCurAnmEnd(0)) {
		self->onLiveFlag(LIVE_FLAG_HIDDEN);
		self->setBckAnm(12);
		self->initFlower();
	} else if (self->isFindMario(1.0f)) {
		self->updateSquareToMario();
		if (self->mDistToMarioSquared
		    < ((TSamboHeadSaveLoadParams*)self->unk194)->mSLAppearDist.get()
		          * ((TSamboHeadSaveLoadParams*)self->unk194)
		                ->mSLAppearDist.get()) {
			spine->pushAfterCurrent(&TNerveSamboHeadAppear::theNerve());
			return true;
		}
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	return false;
}

DEFINE_NERVE(TNerveSamboHeadHitWater, TLiveActor)
{
	TSamboHead* self = (TSamboHead*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(5);
		self->unk1A0 = self->mLinearVelocity;
	}

	if (self->isHitWallInBound()) {
		self->unk1AC = 0.0f;
		if (self->unk1B0)
			self->mRotation.y += 180.0f;

		spine->reset();
		spine->setNext(&TNerveSamboHeadHitWall::theNerve());
		spine->pushAfterCurrent(&TNerveSamboHeadHitWall::theNerve());
		return true;
	}

	f32 limit = ((TSamboHeadSaveLoadParams*)self->unk194)->mSLJumpAngY.get();
	if (self->isBckAnm(6)) {
		if (spine->getTime() < 100)
			self->unk1AC = MsClamp(self->unk1AC - 3.0f, -limit, limit);
		else
			self->unk1AC = MsClamp(3.0f + self->unk1AC, -limit, 0.0f);
	} else {
		self->unk1AC = MsClamp(3.0f + self->unk1AC, -limit, limit);
	}

	if (self->isBckAnm(5) && !self->isAirborne())
		self->setBckAnm(7);

	if (self->isBckAnm(6)) {
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		self->mPosition.y = 1.0f + self->mGroundHeight;
		self->setVelocity(self->unk1A0);
	}

	if (!self->isAirborne())
		self->setBckAnm(6);

	if (self->checkCurAnmEnd(0) && self->isBckAnm(6)) {
		f32 rate = ((TSamboHeadSaveLoadParams*)self->unk194)
		               ->mSLHitJumpSpRateXZ.get();
		self->setBckAnm(6);
		self->unk1A0.x *= rate;
		self->unk1A0.z *= rate;
		self->unk1A0.y = 0.0f;
		self->setVelocity(self->unk1A0);
		self->mPosition.y = self->mGroundHeight;
		self->offLiveFlag(LIVE_FLAG_AIRBORNE);
		self->setBckAnm(12);
		spine->setNext(&TNerveSamboHeadAttack::theNerve());
		spine->pushAfterCurrent(&TNerveSamboHeadRecoverWater::theNerve());
		return true;
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	return false;
}

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

DEFINE_NERVE(TNerveSamboHeadHitWall, TLiveActor)
{
	TSamboHead* self = (TSamboHead*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setBckAnm(1);

		JGeometry::TVec3<f32> scale(1.5f);
		if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		        PARTICLE_MS_ENM_WALLHIT, &self->mPosition, 0,
		        DEG2SHORTANGLE(self->mRotation.y), 0, 0, nullptr))
			emitter->setGlobalScale(scale);

		if (JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
		        PARTICLE_MS_ENM_WALLHIT_O, &self->mPosition, 0,
		        DEG2SHORTANGLE(self->mRotation.y), 0, 0, nullptr)) {
			emitter->setGlobalScale(scale);
			SMSSetEmitterPolColor(emitter, 6);
		}
	}

	int waitTime = ((TSmallEnemyManager*)self->getManager())->unk5C;
	if (self->checkCurAnmEnd(0)
	    && spine->getTime() > waitTime
	                              + self->getMActor()
	                                    ->getFrameCtrl(ANM_TYPE_BCK)
	                                    ->getEnd()) {
		self->onLiveFlag(LIVE_FLAG_DEAD);
		self->onLiveFlag(LIVE_FLAG_UNK8);
		self->onLiveFlag(LIVE_FLAG_UNK20000);
		self->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		self->mHolder = nullptr;
		self->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
		self->genRandomItem();
		return true;
	}

	return false;
}
