#include <Enemy/HanaSamboObj.hpp>
#include <Enemy/HanaSambo.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/PathNode.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/Map.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/Item.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JMath.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <Camera/cameralib.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* sambo_bastable[] = {
	"/scene/sambo/bas/sambo_down.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/sambo/bas/sambo_Fhide.bas",
	nullptr,
	"/scene/sambo/bas/sambo_Fset.bas",
	"/scene/sambo/bas/sambo_Fwait.bas",
	"/scene/sambo/bas/sambo_hit.bas",
	nullptr,
	"/scene/sambo/bas/sambo_Ydown.bas",
};

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
	nullptr,
	"/scene/sambohead/bas/samboHead_set.bas",
	"/scene/sambohead/bas/samboHead_turn.bas",
	nullptr,
};

static int SamboHeadRollCallback(J3DNode*, int);

u8 THanaSambo::mHeadJntIndex   = 3;
u8 THanaSambo::mPollenJntIndex = 6;
u8 TSamboHead::mBodyJntIndex   = 0;
TSamboHead* gpCurSamboHead;

// Emitters scaled the same on both axes, as the die/hit effects are.
static inline void setEmitterScale(JPABaseEmitter* emitter, f32 x, f32 y,
                                   f32 z)
{
	emitter->mGlobalDynamicsScale.set(x, y, z);
	emitter->mGlobalParticleScale.set(x, y, z);
}

// ---------------------------------------------------------------------------
// TSamboFlowerCoinUnit
// ---------------------------------------------------------------------------

// UNUSED, 0x50 in the map: inlined into TSamboFlowerManager::loadAfter.
TSamboFlowerCoinUnit::TSamboFlowerCoinUnit(int capacity)
{
	mFlowers     = nullptr;
	mNum         = 0;
	mCapacity    = capacity;
	mCoinItem    = nullptr;
	mClosedCount = capacity;
	mFlowers     = new TSamboFlower*[capacity];
}

// UNUSED, 0x34 in the map: inlined into TSamboFlowerManager::loadAfter.
void TSamboFlowerCoinUnit::add(TSamboFlower* flower)
{
	if (mNum < mCapacity) {
		mFlowers[mNum]         = flower;
		flower->mBloomCounter  = &mClosedCount;
		++mNum;
	}
}

// Once every flower in the ring is open, the coins fly out of the centre.
void TSamboFlowerCoinUnit::checkGenCoin()
{
	if (mCoinItem == nullptr)
		return;

	bool allOpen = true;
	for (int i = 0; i < mNum; ++i) {
		bool open = mFlowers[i]->getMActor()->checkCurAnm("flower_fwait", 0);
		if (!open)
			allOpen = false;
	}
	if (!allOpen)
		return;

	int coinNum = 0;
	for (int i = 0; i < mNum; ++i) {
		TSamboFlower* flower = mFlowers[i];
		if (flower->mCoin)
			++coinNum;
		flower->mCanClose = false;
	}

	if (coinNum > 0) {
		int k = 0;
		for (int i = 0; i < mNum; ++i) {
			TSamboFlower* flower = mFlowers[i];
			if (flower->mCoin == nullptr)
				continue;

			JGeometry::TVec3<f32> offset;
			offset.x = 0.0f;
			offset.y = 0.0f;
			f32 rate = (f32)k / (f32)coinNum;
			offset.z = flower->mSaveParams->mSLCoinCircleR.get();
			s16 angle = DEG2SHORTANGLE(360.0f * rate);
			f32 s     = JMASSin(angle);
			f32 c     = JMASCos(angle);
			Mtx rot;
			rot[0][0] = c;
			rot[0][1] = 0.0f;
			rot[0][2] = s;
			rot[0][3] = 0.0f;
			rot[1][0] = 0.0f;
			rot[1][1] = 1.0f;
			rot[1][2] = 0.0f;
			rot[1][3] = 0.0f;
			rot[2][0] = -s;
			rot[2][1] = 0.0f;
			rot[2][2] = c;
			rot[2][3] = 0.0f;
			MTXMultVec(rot, &offset, &offset);

			TMapObjBase* coin = mFlowers[i]->mCoin;
			if (coin->isActorType(0x2000000E))
				coin = gpItemManager->makeObjAppear(0x2000000E);
			if (coin) {
				coin->appear();
				// `a = b + c` reaches the map's out-of-line
				// TVec3::add: operator= is one inline level and the
				// sum nested in its argument two more.
				coin->mPosition = mCenter + offset;
				MsVECNormalize(&offset, &offset);
				++k;
				TSamboFlowerSaveLoadParams* params = mFlowers[i]->mSaveParams;
				f32 speed = params->mSLCoinVelocityXZ.get();
				coin->mVelocity.set(offset.x * speed,
				                    8.0f * rate
				                        + params->mSLCoinVelocityY.get(),
				                    offset.z * speed);
				coin->offLiveFlag(LIVE_FLAG_UNK10);
			}
		}
		if (gpMSound->gateCheck(0x4813))
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4813, 0, nullptr,
			                                             0);
	}
	mCoinItem = nullptr;
}

// ---------------------------------------------------------------------------
// TSamboLeaf
// ---------------------------------------------------------------------------

// UNUSED, 0xb8 in the map: inlined into TSamboFlowerManager::loadAfter.
TSamboLeaf::TSamboLeaf(TSamboFlowerManager* manager, SDLModelData* data,
                       const char* name)
    : JDrama::TViewObj(name)
{
	mModel    = nullptr;
	mIsActive = false;
	mManager  = manager;
	mModel    = new SDLModel(data, 3, 1);
}

void TSamboLeaf::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!mIsActive)
		return;

	if (cue & 1) {
		mPosition.add(mVelocity);
		if (mVelocity.y > -20.0f)
			mVelocity.y -= mManager->getSaveParams()->mSLLeafGravity.get();
		const TBGCheckData* ground;
		if (mPosition.y
		    < gpMap->checkGround(mPosition.x, 20.0f + mPosition.y,
		                         mPosition.z, &ground))
			mIsActive = false;
	}
	if (cue & 2) {
		Mtx mtx;
		MsMtxSetXYZRPH(mtx, mPosition.x, mPosition.y, mPosition.z, 0,
		               (s16)DEG2SHORTANGLE(mRotation.y), 0);
		s16 angle = DEG2SHORTANGLE(-MsGetRotFromZaxis(mVelocity).x);
		f32 s     = JMASSin(angle);
		f32 c     = JMASCos(angle);
		Mtx roll;
		roll[0][0] = c;
		roll[0][1] = -s;
		roll[0][2] = 0.0f;
		roll[0][3] = 0.0f;
		roll[1][0] = s;
		roll[1][1] = c;
		roll[1][2] = 0.0f;
		roll[1][3] = 0.0f;
		roll[2][0] = 0.0f;
		roll[2][1] = 0.0f;
		roll[2][2] = 1.0f;
		roll[2][3] = 0.0f;
		MTXConcat(mtx, roll, mtx);
		MTXCopy(mtx, mModel->getBaseTRMtx());
		mModel->setBaseScale(mScaling);
		mModel->calc();
	}
	if (cue & 4)
		mModel->viewCalc();
	if (cue & 0x200)
		mModel->entry();
}

// UNUSED, 0x34 in the map.
void TSamboLeaf::generate(JGeometry::TVec3<f32>& position)
{
	mPosition = position;
	mPosition.y += 10.0f;
	mIsActive = true;
}

// ---------------------------------------------------------------------------
// TSamboFlowerManager / TSamboFlower
// ---------------------------------------------------------------------------

TSamboFlowerSaveLoadParams::TSamboFlowerSaveLoadParams(const char* prm)
    : TSpineEnemyParams(prm)
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

void TSamboFlowerManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TSamboFlowerSaveLoadParams("/enemy/samboflower.prm");
	mMaterialTable = J3DModelLoaderDataBase::loadMaterialTable(
	    JKRFileLoader::getGlbResource(
	        "/scene/samboflower/flower_orange.bmt"));
}

void TSamboFlowerManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "flower.bmd", 0x10220000, 0 },
		{ nullptr },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TSamboFlowerManager::createEnemyInstance()
{
	return new TSamboFlower;
}

// Builds the leaves and groups the flowers into coin units by the index
// each one loaded, matched against the flower coins by name.
void TSamboFlowerManager::loadAfter()
{
	void* resource = JKRFileLoader::getGlbResource("/scene/samboflower/leaf.bmd");
	SDLModelData* modelData = new SDLModelData(
	    J3DModelLoaderDataBase::load(resource, 0x10210000));

	mLeaves = new TSamboLeaf*[18];
	for (int i = 0; i < 18; ++i)
		mLeaves[i] = new TSamboLeaf(this, modelData, "サンボリーフ");

	mCoinUnitNum = 0;
	for (int i = 0; i < gpItemManager->getObjNum(); ++i) {
		if (strstr(gpItemManager->getObj(i)->getName(), "コイン（フラワー用）"))
			++mCoinUnitNum;
	}

	mCoinUnits  = new TSamboFlowerCoinUnit*[mCoinUnitNum];
	int* counts = new int[mCoinUnitNum];
	for (int i = 0; i < mCoinUnitNum; ++i)
		counts[i] = 0;

	for (int i = 0; i < getObjNum(); ++i) {
		if (strstr(getObj(i)->getName(), "フラワー（コイン用）")) {
			int index = ((TSamboFlower*)getObj(i))->mCoinUnitIndex;
			if (index < mCoinUnitNum)
				++counts[index];
		}
	}

	for (int i = 0; i < mCoinUnitNum; ++i)
		mCoinUnits[i] = new TSamboFlowerCoinUnit(counts[i]);

	for (int i = 0; i < gpItemManager->getObjNum(); ++i) {
		if (strstr(gpItemManager->getObj(i)->getName(), "コイン（フラワー用）")) {
			TFlowerCoin* coin = (TFlowerCoin*)gpItemManager->getObj(i);
			int index         = coin->unk158;
			if (index < mCoinUnitNum) {
				mCoinUnits[index]->mCoinItem = coin;
				mCoinUnits[index]->mCenter   = coin->mPosition;
				coin->kill();
			}
		}
	}

	for (int i = 0; i < getObjNum(); ++i) {
		if (strstr(getObj(i)->getName(), "フラワー（コイン用）")) {
			TSamboFlower* flower = (TSamboFlower*)getObj(i);
			int index            = flower->mCoinUnitIndex;
			if (index < mCoinUnitNum)
				mCoinUnits[index]->add(flower);
		}
	}

	JDrama::TNameRef::loadAfter();
}

void TSamboFlowerManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mCoinUnitNum > 0 && (cue & 2)) {
		for (int i = 0; i < mCoinUnitNum; ++i)
			mCoinUnits[i]->checkGenCoin();
	}
	TEnemyManager::perform(cue, graphics);
	for (int i = 0; i < 18; ++i)
		mLeaves[i]->perform(cue, graphics);
}

// Throws up to three free leaves, 120 degrees apart.
void TSamboFlowerManager::dropLeaf(JGeometry::TVec3<f32>& position,
                                   JGeometry::TVec3<f32>& scale)
{
	int dropped     = 0;
	f32 angles[3]   = { 0.0f, 120.0f, 240.0f };
	for (int i = 0; i < 18; ++i) {
		TSamboLeaf* leaf = mLeaves[i];
		if (!leaf->mIsActive) {
			leaf->generate(position);
			TSamboFlowerSaveLoadParams* params = getSaveParams();
			TMsRange<f32> rangeXZ(params->mSLLeafVelocityXZ.get(),
			                      1.2f * params->mSLLeafVelocityXZ.get());
			TMsRange<f32> rangeY(params->mSLLeafVelocityY.get(),
			                     1.2f * params->mSLLeafVelocityY.get());
			JGeometry::TVec3<f32> velocity(0.0f, rangeY.rand(),
			                               rangeXZ.rand());
			f32 angle = angles[i];
			Mtx rot;
			MsMtxSetRotRPH(rot, 0.0f, angle, 0.0f);
			MTXMultVec(rot, &velocity, &velocity);
			++dropped;
			mLeaves[i]->mRotation.set(0.0f, angle - 90.0f, 0.0f);
			mLeaves[i]->mVelocity = velocity;
			mLeaves[i]->mScaling  = scale;
		}
		if (dropped >= 3)
			break;
	}
}

void TSamboFlower::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	stream.read(&mCoinIndex, 4);
	stream.read(&mCoinUnitIndex, 4);
	mCanClose = true;
	reset();
	offLiveFlag(LIVE_FLAG_UNK800);
}

void TSamboFlower::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	if (mCoinIndex >= 0) {
		TMapObjBase* coin = TMapObjBaseManager::newAndRegisterObj("coin");
		if (coin)
			mCoin = coin;
	}
}

void TSamboFlower::init(TLiveManager* manager)
{
	mManager = manager;
	manager->manageActor(this);
	setMActorAndKeeper();
	unk130      = 1;
	mSaveParams = getSaveParams();
	if (mSaveParams) {
		mBodyRadius       = mSaveParams->mSLBodyRadius.get();
		mWallRadius       = mSaveParams->mSLWallRadius.get();
		mHeadHeight       = mSaveParams->mSLHeadHeight.get();
		mScaledBodyRadius = mBodyScale * mBodyRadius;
	}
	initHitActor(0x10000027, 1, 0x80000000, mBodyRadius, mHeadHeight,
	             mBodyRadius, mHeadHeight);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK400);
	initAnmSound();
	mActorType = 0x10000027;
	mIsBloomed = false;
	onLiveFlag(LIVE_FLAG_DEAD);
}

void TSamboFlower::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("flower.bmd", 3);
	MActor* actor = mMActor;
	actor->getModel()->getModelData()->setMaterialTable(
	    ((TSamboFlowerManager*)mManager)->mMaterialTable,
	    (J3DMaterialCopyFlag)3);
	actor->initDL();
	actor->getModel()->lock();
}

BOOL TSamboFlower::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (!mIsBloomed)
			bloom();
		return TRUE;
	}
	return FALSE;
}

void TSamboFlower::reset()
{
	TSpineEnemy::reset();
	getMActor()->setBck("flower_wait");
	offHitFlag(HIT_FLAG_NO_COLLISION);
	offLiveFlag(LIVE_FLAG_UNK800);
	offLiveFlag(LIVE_FLAG_DEAD);
	offLiveFlag(LIVE_FLAG_UNK10);
}

// An open flower waits, then closes again by playing the hit anim backwards.
void TSamboFlower::moveObject()
{
	if (mIsBloomed) {
		if (checkCurAnmEnd(0) && getMActor()->checkCurAnm("flower_hit", 0))
			getMActor()->setBck("flower_fwait");
		if (mCanClose) {
			++mBloomTimer;
			if (mBloomTimer > mSaveParams->mSLBloomTimer.get()) {
				if (getMActor()->checkCurAnm("flower_fwait", 0)) {
					hide();
				} else if (isBloomEnd()) {
					mBloomTimer = 0;
					getMActor()->setBck("flower_wait");
				}
			}
		}
	}
	if (!checkLiveFlag(LIVE_FLAG_UNK10))
		mPosition.y = gpMap->checkGround(mPosition.x, 100.0f + mPosition.y,
		                                 mPosition.z, &mGroundPlane);
}

void TSamboFlower::drawObject(JDrama::TGraphics*)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	TCircleShadowRequest request;
	request.mPosition = mPosition;
	request.mRadiusX = request.mRadiusZ = 120.0f;
	gpBindShadowManager->request(request, getActorType() & 0xFFFF0000);
	getMActor()->setLightData(mGroundPlane, mPosition);
	getMActor()->entry();
}

// UNUSED, 0xcc in the map: inlined into receiveMessage and
// TNerveSamboHeadAppear.
void TSamboFlower::bloom()
{
	mIsBloomed  = true;
	mBloomTimer = 0;
	gpMarioParticleManager->emit(0xB2, &mPosition, 0, nullptr);
	getMActor()->setBck("flower_hit");
	if (mCanClose && mBloomCounter) {
		--*mBloomCounter;
		u32 sound = *mBloomCounter + 0x89B9;
		if (gpMSound->gateCheck(sound))
			MSoundSESystem::MSoundSE::startSoundActor(sound, &mPosition, 0,
			                                          nullptr, 0, 4);
	}
}

// UNUSED, 0x44 in the map: inlined into moveObject.
bool TSamboFlower::isBloomEnd()
{
	return getMActor()->getFrameCtrl(0)->getFrame() < 1.0f;
}

// UNUSED, 0x50 in the map: inlined into moveObject.
void TSamboFlower::hide()
{
	mIsBloomed = false;
	if (mBloomCounter)
		++*mBloomCounter;
	getMActor()->setBck("flower_hit");
	s16 end = getMActor()->getFrameCtrl(0)->getEnd();
	getMActor()->getFrameCtrl(0)->setFrame(end);
	getMActor()->setFrameRate(-SMSGetAnmFrameRate(), 0);
}

// ---------------------------------------------------------------------------
// THanaSamboManager / THanaSamboHead
// ---------------------------------------------------------------------------

// UNUSED, 0x1f8 in the map: inlined into THanaSamboManager::load.
THanaSamboSaveLoadParams::THanaSamboSaveLoadParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mSLAttackDist, 200.0f)
    , PARAM_INIT(mSLAttackInterval, 200)
    , PARAM_INIT(mSLAppearDist, 800.0f)
    , PARAM_INIT(mSLHideDist, 1000.0f)
    , PARAM_INIT(mSLAttackingTime, 30)
    , PARAM_INIT(mSLHeadAttackRadius, 60.0f)
    , PARAM_INIT(mSLHeadAttackHeight, 20.0f)
    , PARAM_INIT(mSLHeadDamageRadius, 80.0f)
    , PARAM_INIT(mSLHeadDamageHeight, 40.0f)
{
	TParams::load(mPrmPath);
}

THanaSamboManager::THanaSamboManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void THanaSamboManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new THanaSamboSaveLoadParams("/enemy/hanasambo.prm");
}

TSpineEnemy* THanaSamboManager::createEnemyInstance()
{
	return new THanaSambo("ハナサンボ");
}

void THanaSamboManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "sambo.bmd", 0x10220000, 0 },
		{ "samboD.bmd", 0x10220000, 0 },
		{ nullptr },
	};
	createModelDataArray(entry);
}

BOOL THanaSamboHead::receiveMessage(THitActor* sender, u32 message)
{
	if (message <= HIT_MESSAGE_HIP_DROP) {
		mOwner->kill();
		return TRUE;
	}
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		mOwner->waterDamage();
		return TRUE;
	}
	return FALSE;
}

// UNUSED, 0x90 in the map.
void THanaSamboHead::checkHit()
{
	for (int i = 0; i < mColCount; ++i) {
		if (mCollisions[i]->isActorType(0x80000001))
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	}
}

// UNUSED, 0x10 in the map: inlined into THanaSambo::kill and setDeadAnm.
void THanaSamboHead::kill() { onHitFlag(HIT_FLAG_NO_COLLISION); }

// ---------------------------------------------------------------------------
// THanaSambo
// ---------------------------------------------------------------------------

THanaSambo::THanaSambo(const char* name)
    : TSmallEnemy(name)
    , mHead(nullptr)
    , mSaveParams(nullptr)
    , mFlower(nullptr)
    , mShadow(nullptr)
{
}

static inline THitActor* SamboMarioAddress()
{
	return (THitActor*)gpMarioAddress;
}

void THanaSambo::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	TPathNode node(SamboMarioAddress());
	unkF4  = node;
	unk104 = node;
	unk114.clear();
}

void THanaSambo::init(TLiveManager* manager)
{
	TSmallEnemy::init(manager);
	mActorType  = 0x1000001A;
	unk150      = 0x11;
	mSaveParams = getSaveParams();
	mSpine->initWith(&TNerveHanaSamboHide::theNerve());
	mShadow = new TMBindShadowBody(this, getModel(), 1.5f);

	TPathNode node(SamboMarioAddress());
	unkF4  = node;
	unk104 = node;
	unk114.clear();

	if (mInstanceIndex == 0) {
		for (int i = 0; i < mMActorKeeper->getMActor("samboD.bmd")
		                        ->getModel()
		                        ->getModelData()
		                        ->getJointNum();
		     ++i) { }
	}

	mHead = new THanaSamboHead("ハナサンボ頭あたり");
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(mHead);
	THanaSamboSaveLoadParams* params = mSaveParams;
	f32 scale                        = mBodyScale;
	mHead->initHitActor(0x1000001B, 2, 0x80000000,
	                    params->mSLHeadAttackRadius.get() * scale,
	                    params->mSLHeadAttackHeight.get() * scale,
	                    params->mSLHeadDamageRadius.get() * scale,
	                    params->mSLHeadDamageHeight.get() * scale);
	mHead->mOwner = this;
}

void THanaSambo::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 2);
	mMActor       = mMActorKeeper->createMActor("sambo.bmd", 3);
	mMActorKeeper->createMActor("samboD.bmd", 3);
}

// The head hit box rides the head joint and attacks whatever it touches.
void THanaSambo::moveObject()
{
	TSmallEnemy::moveObject();
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		mHead->mPosition = getPosition();
	} else {
		MtxPtr mtx         = getModel()->getAnmMtx(mHeadJntIndex);
		mHead->mPosition.x = mtx[0][3];
		mHead->mPosition.y = mtx[1][3];
		mHead->mPosition.z = mtx[2][3];
	}

	mHead->checkHit();

	if (!checkLiveFlag(LIVE_FLAG_HIDDEN)
	    || mSpine->getCurrentNerve() == &TNerveHanaSamboHide::theNerve()) {
		mPosition.x = mInitialPos.x;
		mPosition.z = mInitialPos.z;
	}
}

void THanaSambo::drawObject(JDrama::TGraphics* graphics)
{
	TLiveActor::drawObject(graphics);
	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN | LIVE_FLAG_UNK8))
		mShadow->entryDrawShadow();
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
	mHead->kill();
	onLiveFlag(LIVE_FLAG_UNK40);
}

void THanaSambo::reset()
{
	TSmallEnemy::reset();
	unk165      = false;
	mInitialPos = mPosition;
}

void THanaSambo::setWaitAnm()
{
	setBckAnm(7);
	mIsAttacking = false;
}

// The death model is a separate bmd.
void THanaSambo::setDeadAnm()
{
	mMActor = mMActorKeeper->getMActor("samboD.bmd");
	if (mIsAttacking)
		setBckAnm(0xA);
	else
		setBckAnm(0);
	mHead->kill();
	mIsAttacking = false;
	onLiveFlag(LIVE_FLAG_UNK8);
}

// UNUSED, 0x44 in the map.
void THanaSambo::setAttackAnm()
{
	setBckAnm(3);
	mIsAttacking = true;
}

// UNUSED, 0x140 in the map.
void THanaSambo::waterDamage()
{
	unk165 = true;
	if (!changeByJuice()) {
		if (mSpine->getCurrentNerve() == &TNerveHanaSamboWait::theNerve())
			mSpine->pushNerve(&TNerveHanaSamboFreeze::theNerve());
	}
}

const char** THanaSambo::getBasNameTable() const { return sambo_bastable; }

void THanaSambo::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
	if (!checkLiveFlag(LIVE_FLAG_DEAD | LIVE_FLAG_HIDDEN
	                   | LIVE_FLAG_CLIPPED_OUT))
		mHead->THitActor::perform(cue, graphics);
}

// Pollen puffs out of the mouth joint; while waiting it drifts up, while
// attacking it is thrown forward.
void THanaSambo::createPollen()
{
	MtxPtr joint = getMActor()->getModel()->getAnmMtx(mPollenJntIndex);
	JGeometry::TVec3<f32> pos;
	pos.x = joint[0][3];
	pos.y = joint[1][3];
	pos.z = joint[2][3];
	Mtx mtx;
	MsMtxSetRotRPH(mtx, 0.0f, 0.0f, -90.0f);
	MTXConcat(joint, mtx, mtx);

	JPABaseEmitter* emitter;
	if (mSpine->getCurrentNerve() == &TNerveHanaSamboWait::theNerve()) {
		emitter = gpMarioParticleManager->emit(0xB2, &pos, 0, nullptr);
		mtx[1][3] += 200.0f;
	} else {
		emitter = gpMarioParticleManager->emit(0xB3, &pos, 0, nullptr);
		JGeometry::TVec3<f32> forward(0.0f, 0.0f, 200.0f);
		Mtx rot;
		MsMtxSetRotRPH(rot, mRotation.x, mRotation.y, mRotation.z);
		MTXMultVec(rot, &forward, &forward);
		mtx[0][3] += forward.x;
		mtx[1][3] += forward.y;
		mtx[2][3] += forward.z;
	}
	if (emitter)
		emitter->setGlobalSRTMatrix(mtx);
}

// UNUSED, 0xac in the map: inlined twice into TNerveHanaSamboHide.
void THanaSambo::initFlower()
{
	if (mFlower == nullptr) {
		mFlower = (TSamboFlower*)gpConductor->makeOneEnemyAppear(
		    mPosition, "サンボフラワーマネージャー", 1);
		mFlower->reset();
	}
	mFlower->onLiveFlag(LIVE_FLAG_UNK10);
	mFlower->offLiveFlag(LIVE_FLAG_DEAD);
	mFlower->mPosition   = mPosition;
	mFlower->mPosition.y = mGroundHeight;
}

static inline THanaSamboSaveLoadParams* HanasamboWaitParams(const THanaSambo* p)
{
	THanaSamboSaveLoadParams* params = p->mSaveParams;
	return params;
}

DEFINE_NERVE(TNerveHanaSamboAppear, TLiveActor)
{
	THanaSambo* sambo = (THanaSambo*)spine->getBody();
	if (spine->getTime() == 0) {
		sambo->offLiveFlag(LIVE_FLAG_HIDDEN);
		sambo->offHitFlag(HIT_FLAG_NO_COLLISION);
		sambo->setBckAnm(6);
		gpMarioParticleManager->emit(0xB6, &sambo->mPosition, 0, nullptr);
		gpMarioParticleManager->emit(0xB7, &sambo->mPosition, 0, nullptr);
		TSamboFlower* flower = sambo->mFlower;
		flower->onHitFlag(HIT_FLAG_NO_COLLISION);
		MActor* flowerActor = flower->getMActor();
		flowerActor->setBck("flower_fwait");
		flower->onLiveFlag(LIVE_FLAG_DEAD);
	}
	if (sambo->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveHanaSamboWait::theNerve());
		sambo->setWaitAnm();
		if (gpMarioPos->y < 100.0f + sambo->mPosition.y) {
			sambo->updateSquareToMario();
			f32 dist = HanasamboWaitParams(sambo)->mSLAttackDist.get();
			dist *= dist;
			if (sambo->mDistToMarioSquared < dist)
				spine->pushAfterCurrent(&TNerveHanaSamboAttack::theNerve());
		}
		return true;
	}
	f32 turnSpeed = sambo->mTurnSpeed;
	sambo->walkToCurPathNode(0.0f, 3.0f * turnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveHanaSamboWait, TLiveActor)
{
	THanaSambo* sambo = (THanaSambo*)spine->getBody();
	if (spine->getTime() == 0)
		sambo->setWaitAnm();
	if (!sambo->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && sambo->getMActor()->getFrameCtrl(0)->checkPass(16.0f))
		sambo->createPollen();

	if (spine->getTime() > HanasamboWaitParams(sambo)->mSLAttackInterval.get()
	    && gpMarioPos->y < 100.0f + sambo->mPosition.y) {
		sambo->updateSquareToMario();
		f32 dist = HanasamboWaitParams(sambo)->mSLAttackDist.get();
		dist *= dist;
		if (sambo->mDistToMarioSquared < dist) {
			spine->pushAfterCurrent(&TNerveHanaSamboAttack::theNerve());
			return true;
		}
	}
	sambo->updateSquareToMario();
	f32 dist = HanasamboWaitParams(sambo)->mSLHideDist.get();
	dist *= dist;
	if (sambo->mDistToMarioSquared > dist) {
		spine->pushAfterCurrent(&TNerveHanaSamboHide::theNerve());
		return true;
	}
	sambo->walkToCurPathNode(0.0f, sambo->mTurnSpeed, 0.0f);
	return false;
}

static inline MSound* HanasamboAttackGetMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

DEFINE_NERVE(TNerveHanaSamboAttack, TLiveActor)
{
	THanaSambo* sambo = (THanaSambo*)spine->getBody();
	if (spine->getTime() == 0) {
		sambo->setAttackAnm();
	} else if (sambo->checkCurAnmEnd(0)) {
		if (sambo->isBckAnm(3)) {
			sambo->createPollen();
			if (HanasamboAttackGetMSound()->gateCheck(0x291B))
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x291B, &sambo->mPosition, 0, nullptr, 0, 4);
			sambo->setBckAnm(1);
		} else if (sambo->isBckAnm(1)) {
			s32 attackingTime = sambo->mSaveParams->mSLAttackingTime.get();
			if (spine->getTime() > attackingTime && !sambo->unsetUnk165())
				sambo->setBckAnm(2);
			else
				sambo->setBckAnm(1);
		} else {
			spine->pushAfterCurrent(&TNerveHanaSamboWait::theNerve());
			return true;
		}
	}
	return false;
}

// Binding level worth +16 of low region, landing
// TNerveHanaSamboHide::execute's frame at 0x60 (batch 121).
static inline MActor* HanasamboGetMActor(const THanaSambo* p)
{
	MActor* mActor = p->getMActor();
	return mActor;
}

DEFINE_NERVE(TNerveHanaSamboHide, TLiveActor)
{
	THanaSambo* sambo = (THanaSambo*)spine->getBody();
	if (spine->getTime() == 0) {
		sambo->setBckAnm(4);
		gpMarioParticleManager->emit(0xB8, &sambo->mPosition, 0, nullptr);
		gpMarioParticleManager->emit(0xB9, &sambo->mPosition, 0, nullptr);
	}
	if (spine->getTime() == 75)
		sambo->initFlower();
	if (sambo->checkCurAnmEnd(0)) {
		sambo->initFlower();
		sambo->onHitFlag(HIT_FLAG_NO_COLLISION);
		sambo->setBckAnm(6);
		HanasamboGetMActor(sambo)->setFrameRate(0.0f, 0);
		sambo->onLiveFlag(LIVE_FLAG_HIDDEN);
	}
	sambo->updateSquareToMario();
	f32 dist = sambo->mSaveParams->mSLAppearDist.get();
	dist *= dist;
	if (sambo->mDistToMarioSquared < dist) {
		spine->pushAfterCurrent(&TNerveHanaSamboAppear::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveHanaSamboDie, TLiveActor)
{
	// The map names this jIndexTable$3048, the mangling of a function-local
	// static, so it lives inside the nerve body rather than at file scope.
	// It is not const: the map puts it in .data, between entry$2896 and
	// entry$3090, and a const copy lands in .rodata and pushes every string
	// after it 0x10 high.
	static int jIndexTable[4] = { 1, 3, 4, 5 };

	THanaSambo* sambo = (THanaSambo*)spine->getBody();
	if (spine->getTime() == 0) {
		sambo->onHitFlag(HIT_FLAG_NO_COLLISION);
		sambo->setDeadAnm();
	} else if (sambo->checkCurAnmEnd(0) || spine->getTime() > 300) {
		for (int i = 0; i < 4; ++i) {
			MtxPtr mtx = sambo->getMActor()->getModel()->getAnmMtx(
			    jIndexTable[i]);
			sambo->mDieEffectPos[i].set(mtx[0][3], mtx[1][3],
			                            mtx[2][3]);
			JPABaseEmitter* emitter
			    = gpMarioParticleManager->emit(0xE4, &sambo->mDieEffectPos[i], 0, nullptr);
			if (emitter) {
				emitter->mGlobalDynamicsScale.set(sambo->mScaling);
				emitter->mGlobalParticleScale.set(sambo->mScaling);
			}
			emitter = gpMarioParticleManager->emit(0xE6, &sambo->mDieEffectPos[i], 0, nullptr);
			if (emitter) {
				emitter->mGlobalDynamicsScale.set(sambo->mScaling);
				emitter->mGlobalParticleScale.set(sambo->mScaling);
			}
		}
		sambo->onLiveFlag(LIVE_FLAG_DEAD);
		sambo->onLiveFlag(LIVE_FLAG_UNK8);
		sambo->offLiveFlag(LIVE_FLAG_HIDDEN);
		sambo->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		sambo->mHolder = nullptr;
		sambo->stopAnmSound();
		sambo->onHitFlag(HIT_FLAG_NO_COLLISION);
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		sambo->genRandomItem();
	}
	return false;
}

DEFINE_NERVE(TNerveHanaSamboFreeze, TLiveActor)
{
	THanaSambo* sambo = (THanaSambo*)spine->getBody();
	if (spine->getTime() == 0)
		sambo->setBckAnm(5);
	if (sambo->checkCurAnmEnd(0)) {
		if (sambo->isBckAnm(5)) {
			if (sambo->unsetUnk165())
				sambo->setBckAnm(8);
		} else if (sambo->unsetUnk165()) {
			sambo->setBckAnm(8);
		} else {
			return true;
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
// TSamboHead
// ---------------------------------------------------------------------------

// Rolls the body joint about the axis perpendicular to the velocity while
// the head is airborne.
static int SamboHeadRollCallback(J3DNode* node, int param)
{
	if (param == 0) {
		TSamboHead* head = gpCurSamboHead;
		if (head == nullptr || !head->isUseCallBack())
			return true;
		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx   = gpCurSamboHead->getModel()->getAnmMtx(
                    joint->getJntNo());

		JGeometry::TVec3<f32> velocity(gpCurSamboHead->mVelocity);
		if (velocity.x == 0.0f && velocity.z == 0.0f)
			velocity.x = 0.001f;
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> axis;
		VECCrossProduct(&up, &velocity, &axis);

		JGeometry::TVec3<f32> colZ(anmMtx[0][2], anmMtx[1][2],
		                           anmMtx[2][2]);
		JGeometry::TVec3<f32> colX(anmMtx[0][0], anmMtx[1][0],
		                           anmMtx[2][0]);
		JGeometry::TVec3<f32> colY(anmMtx[0][1], anmMtx[1][1],
		                           anmMtx[2][1]);
		f32 angle = gpCurSamboHead->mRollAngle;

		f32 pz   = 0.0f;
		f32 lenZ = colZ.dot(colZ);
		if (0.0f != lenZ)
			pz = axis.dot(colZ) / lenZ;
		f32 py   = 0.0f;
		f32 lenY = colY.dot(colY);
		if (0.0f != lenY)
			py = axis.dot(colY) / lenY;
		f32 px   = 0.0f;
		f32 lenX = colX.dot(colX);
		if (0.0f != lenX)
			px = axis.dot(colX) / lenX;
		JGeometry::TVec3<f32> localAxis(px, py, pz);

		Mtx roll;
		MTXRotAxisRad(roll, &localAxis, 0.017453292f * angle);
		MTXConcat(anmMtx, roll, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, roll, J3DSys::mCurrentMtx);
	}
	return true;
}

// UNUSED, 0x294 in the map: inlined into TSamboHeadManager::load.
TSamboHeadSaveLoadParams::TSamboHeadSaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
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

TSamboHeadManager::TSamboHeadManager(const char* name)
    : TSmallEnemyManager(name)
{
	gpCurSamboHead = nullptr;
}

void TSamboHeadManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TSamboHeadSaveLoadParams("/enemy/sambohead.prm");
}

void TSamboHeadManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "samboHead.bmd", 0x10220000, 0 },
		{ nullptr },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TSamboHeadManager::createEnemyInstance()
{
	return new TSamboHead("サンボヘッド");
}

TSamboHead::TSamboHead(const char* name)
    : TWalkerEnemy(name)
    , mSaveParams(nullptr)
    , mFlower(nullptr)
    , mJumpTimer(0)
    , mRollAngle(0.0f)
    , mIsTurnedBack(false)
{
}

void TSamboHead::load(JSUMemoryInputStream& stream)
{
	TSmallEnemy::load(stream);
	reset();
	TPathNode node(SamboMarioAddress());
	unkF4  = node;
	unk104 = node;
	unk114.clear();
}

void TSamboHead::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType  = 0x1000001B;
	unk150      = 0x11;
	mSaveParams = getSaveParams();
	getSpine()->initWith(&TNerveSamboHeadHide::theNerve());
	TPathNode node((THitActor*)gpMarioAddress);
	unkF4  = node;
	unk104 = node;
	unk114.clear();
	getMActor()->setJointCallback(mBodyJntIndex, SamboHeadRollCallback);
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
	unk165        = false;
	mIsTurnedBack = false;
	mJumpTimer    = 0;
	mRollAngle    = 0.0f;
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

// Water knocks the head back up into the air, away from Mario.
void TSamboHead::behaveToWater(THitActor*)
{
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadHide::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAppear::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadHitWall::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;

	JGeometry::TVec3<f32> velocity(getVelocity());
	velocity.y = 0.0f;
	JGeometry::TVec3<f32> away(getPosition().x - SMS_GetMarioPos().x, 0.0f,
	                           getPosition().z - SMS_GetMarioPos().z);
	MsVECNormalize(&away, &away);
	away.scale(mSaveParams->mSLHitJumpSpXZ.get());
	away.y = mSaveParams->mSLHitJumpSpY.get();
	if (mSpine->getCurrentNerve() != &TNerveSamboHeadHitWater::theNerve())
		mSpine->pushNerve(&TNerveSamboHeadHitWater::theNerve());
	else
		away.add(velocity);
	mVelocity = away;
	mPosition.y += 2.0f;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TSamboHead::attackToMario()
{
	sendAttackMsgToMario();
	if (checkLiveFlag2(LIVE_FLAG_AIRBORNE)) {
		JGeometry::TVec3<f32> away(mPosition.x - gpMarioPos->x, 10.0f,
		                           mPosition.z - gpMarioPos->z);
		MsVECNormalize(&away, &away);
		away.scale(8.0f);
		mVelocity = away;
		return;
	}
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAttack::theNerve())
		mSpine->pushNerve(&TNerveSmallEnemyFreeze::theNerve());
}

f32 TSamboHead::getGravityY() const
{
	f32 gravity = mGravity;
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAttack::theNerve())
		gravity = mSaveParams->mSLMoveGravity.get();
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadHitWater::theNerve())
		gravity = mSaveParams->mSLHitJumpGravity.get();
	return gravity;
}

// UNUSED, 0xbc in the map: inlined into TNerveSamboHeadHide.
void TSamboHead::initFlower()
{
	if (mFlower == nullptr) {
		mFlower = (TSamboFlower*)gpConductor->makeOneEnemyAppear(
		    mPosition, "サンボフラワーマネージャー", 1);
		mFlower->reset();
	}
	mFlower->offHitFlag(HIT_FLAG_NO_COLLISION);
	mFlower->onLiveFlag(LIVE_FLAG_UNK10);
	mFlower->offLiveFlag(LIVE_FLAG_DEAD);
	mFlower->mPosition.y = mGroundHeight;
	mFlower->mPosition   = mPosition;
}

void TSamboHead::setDeadAnm() { setBckAnm(3); }

static inline TMarioParticleManager* SamboHeadGetMarioParticleManager()
{
	TMarioParticleManager* marioParticleManager = gpMarioParticleManager;
	return marioParticleManager;
}

static inline MSound* SamboHeadGetMSound()
{
	MSound* sound = gpMSound;
	return sound;
}

void TSamboHead::setAfterDeadEffect()
{
	JPABaseEmitter* emitter;
	if (isBckAnm(1)) {
		emitter = SamboHeadGetMarioParticleManager()->emit(0xE5, &mPosition, 0,
		                                                   nullptr);
		if (emitter)
			setEmitterScale(emitter, 1.5f, 1.5f, 1.5f);
	} else {
		emitter = SamboHeadGetMarioParticleManager()->emit(0xE4, &mPosition, 0,
		                                                   nullptr);
		if (emitter)
			setEmitterScale(emitter, 1.5f, 1.5f, 1.5f);
	}
	emitter = SamboHeadGetMarioParticleManager()->emit(0xE6, &mPosition, 0,
	                                                   nullptr);
	if (emitter)
		setEmitterScale(emitter, 1.5f, 1.5f, 1.5f);
	if (SamboHeadGetMSound()->gateCheck(0x295F))
		MSoundSESystem::MSoundSE::startSoundActor(0x295F, &mPosition, 0,
		                                          nullptr, 0, 4);
}

// UNUSED, 0xfc in the map.
void TSamboHead::setCrashAnm()
{
	setBckAnm(1);
	JPABaseEmitter* emitter = gpMarioParticleManager->emitWithRotate(
	    0xE2, &mPosition, 0, (s16)DEG2SHORTANGLE(mRotation.y), 0, 0, nullptr);
	if (emitter)
		setEmitterScale(emitter, 1.5f, 1.5f, 1.5f);
	emitter = gpMarioParticleManager->emitWithRotate(
	    0xE3, &mPosition, 0, (s16)DEG2SHORTANGLE(mRotation.y), 0, 0, nullptr);
	if (emitter) {
		setEmitterScale(emitter, 1.5f, 1.5f, 1.5f);
		SMSSetEmitterPolColor(emitter, 6);
	}
}

void TSamboHead::calcRootMatrix()
{
	gpCurSamboHead = this;
	TSpineEnemy::calcRootMatrix();
}

// A crashed head scatters three coins in an arc in front of it.
void TSamboHead::genEventCoin()
{
	if (!isBckAnm(1)) {
		TSmallEnemy::genEventCoin();
		return;
	}
	for (int i = 0; i < 3; ++i) {
		s16 angle = DEG2SHORTANGLE(60.0f * (f32)i + (mRotation.y - 60.0f));
		f32 s     = JMASSin(angle);
		f32 c     = JMASCos(angle);
		Mtx rot;
		rot[0][0] = c;
		rot[0][1] = 0.0f;
		rot[0][2] = s;
		rot[0][3] = 0.0f;
		rot[1][0] = 0.0f;
		rot[1][1] = 1.0f;
		rot[1][2] = 0.0f;
		rot[1][3] = 0.0f;
		rot[2][0] = -s;
		rot[2][1] = 0.0f;
		rot[2][2] = c;
		rot[2][3] = 0.0f;
		JGeometry::TVec3<f32> offset(0.0f, 0.0f, 100.0f);
		MTXMultVec(rot, &offset, &offset);

		TMapObjBase* coin;
		if (i == 1 && mCoin != nullptr) {
			coin = mCoin;
			if (coin->isActorType(0x2000000E))
				coin = gpItemManager->makeObjAppear(0x2000000E);
			if (coin) {
				coin->appear();
				coin->mPosition = mPosition;
			}
		} else {
			coin = gpItemManager->makeObjAppear(mPosition.x + offset.x,
			                                    mPosition.y,
			                                    mPosition.z + offset.z,
			                                    0x2000000E, true);
		}
		if (coin) {
			coin->mPosition.y = mPosition.y;
			MsVECNormalize(&offset, &offset);
			TMsRange<f32> range(8.0f, 16.0f);
			coin->mVelocity.set(4.0f * offset.x, range.rand(),
			                    4.0f * offset.z);
			coin->offLiveFlag(LIVE_FLAG_UNK10);
		}
	}
}

// UNUSED, 0x140 in the map: inlined into SamboHeadRollCallback.
bool TSamboHead::isUseCallBack()
{
	if (mSpine->getCurrentNerve() == &TNerveSamboHeadAttack::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveSamboHeadHitWater::theNerve()
	    || mSpine->getCurrentNerve()
	           == &TNerveSamboHeadRecoverWater::theNerve())
		return true;
	return false;
}

const char** TSamboHead::getBasNameTable() const
{
	return sambohead_bastable;
}

// TNerveSamboHeadHide::execute (batch 127).
static inline TMarioParticleManager* HanasamboGetMarioParticleManager()
{
	TMarioParticleManager* marioParticleManager = gpMarioParticleManager;
	return marioParticleManager;
}

static inline TSamboFlower* SamboHeadFlower(const TSamboHead* p)
{
	TSamboFlower* flower = p->mFlower;
	return flower;
}

DEFINE_NERVE(TNerveSamboHeadAppear, TLiveActor)
{
	TSamboHead* head = (TSamboHead*)spine->getBody();
	if (spine->getTime() == 0) {
		head->offLiveFlag(LIVE_FLAG_HIDDEN);
		head->offHitFlag(HIT_FLAG_NO_COLLISION);
		TSamboFlower* flower = head->mFlower;
		if (flower->mIsBloomed) {
			flower->bloom();
			head->setBckAnm(0xA);
		} else {
			head->setBckAnm(0xA);
		}
		HanasamboGetMarioParticleManager()->emit(0xB6, &head->mPosition, 0, nullptr);
		HanasamboGetMarioParticleManager()->emit(0xB7, &head->mPosition, 0, nullptr);
		flower = SamboHeadFlower(head);
		flower->onHitFlag(HIT_FLAG_NO_COLLISION);
		flower->getMActor()->setBck("flower_fwait");
		flower->onLiveFlag(LIVE_FLAG_DEAD);
	}
	if (spine->getTime() == 20)
		((TSamboFlowerManager*)SamboHeadFlower(head)->mManager)
		    ->dropLeaf(head->mPosition, head->mScaling);
	if (head->checkCurAnmEnd(0)) {
		head->setBckAnm(0xC);
		spine->pushAfterCurrent(&TNerveSamboHeadAttack::theNerve());
		return true;
	}
	return false;
}

// Hops toward Mario, one jump every mSLJumpPrepareTime frames.
DEFINE_NERVE(TNerveSamboHeadAttack, TLiveActor)
{
	TSamboHead* head = (TSamboHead*)spine->getBody();
	bool airborne    = head->isAirborne();
	if (!airborne) {
		if (head->mJumpTimer > head->mSaveParams->mSLJumpPrepareTime.get()
		    && head->checkCurAnmEnd(0)) {
			head->mJumpTimer = 0;
			head->updateSquareToMario();
			JGeometry::TVec3<f32> goal(head->unk104.getPoint());
			goal.set(gpMarioPos->x - head->mPosition.x, 0.0f,
			         gpMarioPos->z - head->mPosition.z);
			if (goal.x == 0.0f && goal.y == 0.0f && goal.z == 0.0f)
				goal.x += 1.0f;
			MsVECNormalize(&goal, &goal);
			f32 dist = head->mSaveParams->mSLMoveDist.get();
			goal.x   = goal.x * dist + head->mPosition.x;
			goal.z   = goal.z * dist + head->mPosition.z;
			goal.y   = head->mPosition.y;
			f32 jumpSp      = head->mSaveParams->mSLJumpSp.get();
			head->mVelocity = head->calcVelocityToJumpToY(
			    goal, jumpSp, head->getGravityY());
			head->mPosition.y += 2.0f;
			head->onLiveFlag(LIVE_FLAG_AIRBORNE);
			head->setBckAnm(8);
		} else {
			++head->mJumpTimer;
		}
		if (head->checkCurAnmEnd(0)) {
			BOOL landed = head->isBckAnm(7);
			if (landed)
				head->setBckAnm(0xC);
		}
		head->getMActor()->setFrameRate(SMSGetAnmFrameRate(), 0);
	} else {
		JGeometry::TVec3<f32> velocity(head->mVelocity);
		if (velocity.y < 0.0f) {
			if (head->isBckAnm(8)) {
				head->setBckAnm(7);
				head->getMActor()->setFrameRate(0.0f, 0);
			}
		}
	}

	if (head->mPosition.y > 30.0f + head->mGroundHeight) {
		f32 angMax = head->mSaveParams->mSLJumpAngY.get();
		JGeometry::TVec3<f32> velocity(head->mVelocity);
		head->mRollAngle
		    = MsClamp(MsGetRotFromZaxis(velocity).x, -angMax, angMax);
	} else {
		head->mRollAngle *= 0.8f;
	}

	f32 turnSpeed = head->mTurnSpeed;
	if (head->isAirborne())
		turnSpeed = 5.0f;
	head->walkToCurPathNode(0.0f, turnSpeed, 0.0f);
	return false;
}

// Binding level over a raw member read, worth +8 of low region in
DEFINE_NERVE(TNerveSamboHeadHide, TLiveActor)
{
	TSamboHead* head = (TSamboHead*)spine->getBody();
	if (spine->getTime() == 0) {
		head->setBckAnm(4);
		head->onHitFlag(HIT_FLAG_NO_COLLISION);
		HanasamboGetMarioParticleManager()->emit(0xB8, &head->mPosition, 0, nullptr);
		gpMarioParticleManager->emit(0xB9, &head->mPosition, 0, nullptr);
	} else if (head->checkCurAnmEnd(0)) {
		head->onLiveFlag(LIVE_FLAG_HIDDEN);
		head->setBckAnm(0xC);
		head->initFlower();
	} else if (head->isFindMario(1.0f)) {
		head->updateSquareToMario();
		f32 dist = head->mSaveParams->mSLAppearDist.get();
		dist *= dist;
		if (head->mDistToMarioSquared < dist) {
			spine->pushAfterCurrent(&TNerveSamboHeadAppear::theNerve());
			return true;
		}
	}
	head->walkToCurPathNode(0.0f, head->mTurnSpeed, 0.0f);
	return false;
}

static inline TSamboHeadSaveLoadParams* SamboHeadWaterParamsRaw(const TSamboHead* p)
{
	return p->mSaveParams;
}

static inline TSamboHeadSaveLoadParams* SamboHeadWaterParams(const TSamboHead* p)
{
	TSamboHeadSaveLoadParams* params = SamboHeadWaterParamsRaw(p);
	return params;
}

// Flung back by water: flies, lands, then bounces once more before it can
// attack again.
DEFINE_NERVE(TNerveSamboHeadHitWater, TLiveActor)
{
	TSamboHead* head = (TSamboHead*)spine->getBody();
	if (spine->getTime() == 0) {
		head->setBckAnm(5);
		head->mHitVelocity = head->getVelocity();
	}
	if (head->isHitWallInBound()) {
		head->mRollAngle = 0.0f;
		if (head->mIsTurnedBack)
			head->mRotation.y += 180.0f;
		spine->reset();
		spine->setNext(&TNerveSamboHeadHitWall::theNerve());
		spine->pushAfterCurrent(&TNerveSamboHeadHitWall::theNerve());
		return true;
	}

	f32 angMax = SamboHeadWaterParams(head)->mSLJumpAngY.get();
	if (head->isBckAnm(6)) {
		if (spine->getTime() < 100)
			head->mRollAngle
			    = MsClamp(head->mRollAngle - 3.0f, -angMax, angMax);
		else
			head->mRollAngle = MsClamp(3.0f + head->mRollAngle, -angMax, 0.0f);
	} else {
		head->mRollAngle = MsClamp(3.0f + head->mRollAngle, -angMax, angMax);
	}

	if (head->isBckAnm(5)) {
		if (!head->isAirborne())
			head->setBckAnm(7);
	}
	if (head->isBckAnm(6)) {
		head->onLiveFlag(LIVE_FLAG_AIRBORNE);
		head->mPosition.y = 1.0f + head->getGroundHeight();
		head->mVelocity   = head->mHitVelocity;
	}
	if (!head->isAirborne())
		head->setBckAnm(6);

	if (head->checkCurAnmEnd(0)) {
		if (head->isBckAnm(6)) {
			// TODO: retail schedules the vtable load ahead of the receiver
			// copy and the params read here; every spelling of these two
			// statements tried keeps our order.
			f32 rate = SamboHeadWaterParams(head)->mSLHitJumpSpRateXZ.get();
			head->setBckAnm(6);
			head->mHitVelocity.x *= rate;
			head->mHitVelocity.z *= rate;
			head->mHitVelocity.y = 0.0f;
			head->mVelocity      = head->mHitVelocity;
			head->mPosition.y    = head->mGroundHeight;
			head->offLiveFlag(LIVE_FLAG_AIRBORNE);
			head->setBckAnm(0xC);
			spine->setNext(&TNerveSamboHeadAttack::theNerve());
			spine->pushAfterCurrent(&TNerveSamboHeadRecoverWater::theNerve());
			return true;
		}
	}
	head->walkToCurPathNode(0.0f, head->mTurnSpeed, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveSamboHeadRecoverWater, TLiveActor)
{
	TSamboHead* head = (TSamboHead*)spine->getBody();
	if (spine->getTime() == 0)
		head->setBckAnm(0xC);
	head->mRollAngle *= 0.99f;
	if (head->checkCurAnmEnd(0) && head->mRollAngle < 1.0f)
		return true;
	return false;
}

DEFINE_NERVE(TNerveSamboHeadHitWall, TLiveActor)
{
	TSamboHead* head = (TSamboHead*)spine->getBody();
	if (spine->getTime() == 0)
		head->setCrashAnm();
	int wait = ((TSmallEnemyManager*)head->mManager)->unk5C;
	if (head->checkCurAnmEnd(0)
	    && spine->getTime()
	           > wait + head->getMActor()->getFrameCtrl(0)->getEnd()) {
		head->onLiveFlag(LIVE_FLAG_DEAD);
		head->onLiveFlag(LIVE_FLAG_UNK8);
		head->onLiveFlag(LIVE_FLAG_UNK20000);
		head->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		head->mHolder = nullptr;
		head->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
		head->genRandomItem();
		return true;
	}
	return false;
}
