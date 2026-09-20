#include <Enemy/TinKoopa.hpp>
#include <Enemy/CoasterKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorData.hpp>
#include <Camera/CameraShake.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <GC2D/GCConsole2.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

#include <Map/MapCollisionEntry.hpp>
#include <Map/MapCollisionManager.hpp>

// The body model's animation list runs break1..4 (0..3), damage1..4 (5..8) and
// wait1..5 (12..16). tinkoopa_bastable only names the slots that have a .bas
// file; the three gaps (4, 9, 10, 11) are animations without sound.
enum {
	TINKOOPA_ANM_BREAK1  = 0,
	TINKOOPA_ANM_DAMAGE1 = 5,
	TINKOOPA_ANM_WAIT1   = 12,
	// TTinKoopa_getWaitAnimationIndex's last slot; also the "nothing to play"
	// entry of the break and damage tables.
	TINKOOPA_ANM_WAIT5 = 16,
};

// Plate numbers. They index every one of the ten TTinKoopa_get* tables.
enum {
	TINKOOPA_PARTS_HEAD    = 0,
	TINKOOPA_PARTS_BREAST  = 1,
	TINKOOPA_PARTS_STOMACH = 2,
	TINKOOPA_PARTS_RARM    = 3,
	TINKOOPA_PARTS_LARM    = 4,
	TINKOOPA_PARTS_LEG     = 5,
	TINKOOPA_PARTS_NUM     = 6,
};

// Slots of TTinKoopa_jointNameTable, so a joint index is one lookup away.
enum {
	TINKOOPA_JOINT_HEAD         = 0,
	TINKOOPA_JOINT_BREAST       = 1,
	TINKOOPA_JOINT_STOMACH      = 2,
	TINKOOPA_JOINT_RARM         = 3,
	TINKOOPA_JOINT_LARM         = 4,
	TINKOOPA_JOINT_LEG          = 5,
	TINKOOPA_JOINT_LEYE         = 6,
	TINKOOPA_JOINT_REYE         = 7,
	TINKOOPA_JOINT_FIRE         = 8,
	TINKOOPA_JOINT_FIRE_COL     = 9,
	TINKOOPA_JOINT_FEMUR        = 10,
	TINKOOPA_JOINT_KILLER_FIRST = 11,
	TINKOOPA_JOINT_NUM          = 15,
};

static const char* tinkoopa_bastable[] = {
	"/scene/tinkoopa/bas/tinkoopa_break1.bas",
	"/scene/tinkoopa/bas/tinkoopa_break2.bas",
	"/scene/tinkoopa/bas/tinkoopa_break3.bas",
	"/scene/tinkoopa/bas/tinkoopa_break4.bas",
	nullptr,
	"/scene/tinkoopa/bas/tinkoopa_damage1.bas",
	"/scene/tinkoopa/bas/tinkoopa_damage2.bas",
	"/scene/tinkoopa/bas/tinkoopa_damage3.bas",
	"/scene/tinkoopa/bas/tinkoopa_damage4.bas",
	nullptr,
	nullptr,
	nullptr,
	"/scene/tinkoopa/bas/tinkoopa_wait1.bas",
	"/scene/tinkoopa/bas/tinkoopa_wait2.bas",
	"/scene/tinkoopa/bas/tinkoopa_wait3.bas",
	"/scene/tinkoopa/bas/tinkoopa_wait4.bas",
	"/scene/tinkoopa/bas/tinkoopa_wait5.bas",
};

static const char* TTinKoopa_jointNameTable[] = {
	"jnt_head",      "jnt_breast",    "jnt_stomach",  "jnt_rarm",
	"jnt_larm",      "jnt_leg",       "jnt_leye",     "jnt_reye",
	"fire_null",     "fire_col_null", "jnt_femur",    "killer_null1",
	"killer_null2",  "killer_null3",  "killer_null4",
};

static int TTinKoopa_jointIndexTable[TINKOOPA_JOINT_NUM];

TTinKoopaParams::TTinKoopaParams(const char* prm)
    : TSpineEnemyParams(prm)
    , PARAM_INIT(mSLPartsHP, 2)
    , PARAM_INIT(mSLFlameHP, 10)
    , PARAM_INIT(mSLFlameRevivalTime, 10)
    , PARAM_INIT(mSLFlameDamageRadius0, 100.0f)
    , PARAM_INIT(mSLFlameDamageHeight0, 400.0f)
    , PARAM_INIT(mSLFlameDamageRadius1, 100.0f)
    , PARAM_INIT(mSLFlameDamageHeight1, 400.0f)
    , PARAM_INIT(mSLDamageRadius, 1000.0f)
    , PARAM_INIT(mSLDamageHeight0, 4000.0f)
    , PARAM_INIT(mSLDamageHeight1, 4000.0f)
    , PARAM_INIT(mSLKillerInterval, 30)
    , PARAM_INIT(mSLDefeatWaitTime, 600)
    , PARAM_INIT(mSLKillerApproachingDistance, 6000.0f)
{
	TParams::load(mPrmPath);

	// Every one of the thirteen values is overwritten right after the load,
	// so /enemy/tinkoopa.prm has no say in the fight at all. The stores are
	// there in the retail object, in this order, reading the literals out of
	// .sdata through TParamRT::set's reference parameter.
	mSLPartsHP.set(1);
	mSLFlameHP.set(50);
	mSLFlameRevivalTime.set(1200);
	mSLFlameDamageRadius0.set(500.0f);
	mSLFlameDamageHeight0.set(300.0f);
	mSLFlameDamageRadius1.set(700.0f);
	mSLFlameDamageHeight1.set(400.0f);
	mSLKillerInterval.set(60);
	mSLDamageRadius.set(800.0f);
	mSLDamageHeight0.set(4000.0f);
	mSLDamageHeight1.set(3400.0f);
	mSLDefeatWaitTime.set(160);
	mSLKillerApproachingDistance.set(2000.0f);
}

// The ten tables below are indexed by plate number, except the animation ones
// which are indexed by damage stage. All ten helpers are UNUSED at 0x18 bytes:
// a table lookup is cheaper than a call, so every site got the body inlined.

static int TTinKoopa_getJointIndex(int parts)
{
	return TTinKoopa_jointIndexTable[parts];
}

static const char* TTinKoopa_getCollisionFileName(int parts)
{
	static const char* table[TINKOOPA_PARTS_NUM] = {
		"/scene/tinkoopa/head_col.col",    "/scene/tinkoopa/breast_col.col",
		"/scene/tinkoopa/stomach_col.col", "/scene/tinkoopa/rarm_col.col",
		"/scene/tinkoopa/larm_col.col",    "/scene/tinkoopa/leg_col.col",
	};
	return table[parts];
}

static const char* TTinKoopa_getPartsFileName(int parts)
{
	// The head and the leg never break off, so they have no debris model.
	static const char* table[TINKOOPA_PARTS_NUM] = {
		nullptr,               "tinkoopa_breast.bmd", "tinkoopa_stomach.bmd",
		"tinkoopa_rarm.bmd",   "tinkoopa_larm.bmd",   nullptr,
	};
	return table[parts];
}

static int TTinKoopa_getBreakingAnimationIndex(int parts)
{
	static int table[TINKOOPA_PARTS_NUM] = { 0, 4, 11, 9, 10, 0 };
	return table[parts];
}

static u32 TTinKoopa_getActorType(int parts)
{
	static u32 table[TINKOOPA_PARTS_NUM] = {
		0x08000019, 0x0800001A, 0x0800001B, 0x0800001D, 0x0800001C, 0x0800001E,
	};
	return table[parts];
}

static int TTinKoopa_getWaitAnimationIndex(int stage)
{
	static int table[5] = { 12, 13, 14, 15, 16 };
	return table[stage];
}

static int TTinKoopa_getBreakAnimationIndex(int stage)
{
	static int table[5] = { 0, 1, 2, 3, 16 };
	return table[stage];
}

static int TTinKoopa_getDamageAnimationIndex(int stage)
{
	static int table[5] = { 5, 6, 7, 8, 16 };
	return table[stage];
}

static int TTinKoopa_getPartsVisibleFrame(int stage)
{
	static int table[5] = { 100, 134, 134, 100, 0 };
	return table[stage];
}

static int TTinKoopa_getBreakingPartsIndex(int stage)
{
	static int table[5] = { 2, 3, 4, 1, 2 };
	return table[stage];
}

// The joints of each debris model that puff smoke on the way down and blow up
// when it disappears. The head and leg plates have none, so the two-argument
// effect helpers are only ever reached for plates 1 to 4.
static const char* breastTrackJointNameTable[] = {
	"breast_1", "breast_2", "breast_3", "breast_4", "breast_5", "breast_6",
};

static const char* bellyTrackJointNameTable[] = {
	"stomach_1", "stomach_2", "stomach_3",
	"stomach_4", "stomach_5", "stomach_6",
};

static const char* rightArmTrackJointNameTable[]
    = { "rarm_1", "rarm_2", "rarm_3", "rarm_4" };

static const char* leftArmTrackJointNameTable[]
    = { "larm_1", "larm_2", "larm_3", "larm_4" };

void TTinKoopa::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);

	mSpine->initWith(&TNerveTinKoopaWait::theNerve());

	mMActorKeeper = new TMActorKeeper(mManager, 7);
	mMActor       = mMActorKeeper->createMActor("tinkoopa_body.bmd", 0);

	JUTNameTab* jointNames = getModel()->getModelData()->getJointName();
	for (int i = 0; i < TINKOOPA_JOINT_NUM; i++)
		TTinKoopa_jointIndexTable[i]
		    = jointNames->getIndex(TTinKoopa_jointNameTable[i]);

	mFlame = new TTinKoopaFlame("flame", this);

	for (int i = 0; i < TINKOOPA_PARTS_NUM; i++) {
		mParts[i] = new TTinKoopaPartsBase(TTinKoopa_getCollisionFileName(i),
		                                   i, this);
		mParts[i]->initTinKoopaPartsBase();
	}

	initHitActor(0x08000018, 1, 0, 0.0f, 0.0f,
	             getSaveParams()->getSLDamageRadius(),
	             getSaveParams()->getSLDamageHeight0());
	offHitFlag(HIT_FLAG_NO_COLLISION);

	getMActor()->setLightType(1);

	J3DModel* model = getMActor()->getModel();
	if (model->getSkinDeform() == nullptr)
		model->setSkinDeform(new J3DSkinDeform, (J3DDeformAttachFlag)1);

	calcRootMatrix();
	getMActor()->calc();

	mKillerGraph = gpConductor->getGraphByName("killer");
	makeCoasterDistanceTable();

	mLaunchSchedule = new TTinKoopaLaunchSchedule(11, this);
	makeLaunchSchedule();

	initAnmSound();
	resetTinKoopa();
}

// The whole fight on paper: lap, animation frame, how many killers and which
// side of the body they come out of. A count of -1 means "let the damage stage
// decide", which is what TTinKoopaLaunchOrder::checkOrder works out.
void TTinKoopa::makeLaunchSchedule()
{
	int i = 0;
	TTinKoopaLaunchOrder* order;
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(0, 600, -1, 0);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(0, 1230, -1, 1);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(0, 2450, -1, 1);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(1, 240, -1, 0);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(1, 280, -1, 1);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(1, 630, -1, 0);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(1, 900, -1, 0);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(1, 1200, -1, 1);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(2, 565, -1, 0);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(2, 700, -1, 0);
	order = mLaunchSchedule->mOrders[i++];
	order->makeOrder(2, 2220, -1, 1);
}

TTinKoopaFlame::TTinKoopaFlame(const char* name, TTinKoopa* tin_koopa)
    : THitActor(name)
    , mTinKoopa(tin_koopa)
{
	initHitActor(0x08000027, 0, 0, 0.0f, 0.0f,
	             mTinKoopa->getSaveParams()->getSLFlameDamageRadius0(),
	             mTinKoopa->getSaveParams()->getSLFlameDamageHeight0());
	offHitFlag(HIT_FLAG_NO_COLLISION);

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	resetTinKoopaFlame();
}

void TTinKoopaFlame::makeHitCollision()
{
	if (mTinKoopa->getDamageStage() == 0)
		setHitParams(
		    0.0f, 0.0f,
		    mTinKoopa->getSaveParams()->mSLFlameDamageRadius0.get(),
		    mTinKoopa->getSaveParams()->mSLFlameDamageHeight0.get());
	else if (mTinKoopa->getDamageStage() == 1)
		setHitParams(
		    0.0f, 0.0f,
		    mTinKoopa->getSaveParams()->mSLFlameDamageRadius1.get(),
		    mTinKoopa->getSaveParams()->mSLFlameDamageHeight1.get());
}

void TTinKoopaFlame::resetTinKoopaFlame()
{
	mHitPoints = (s16)mTinKoopa->getSaveParams()->getSLFlameHP();
	mScale     = 1.0f;
	mSprayed   = false;
}

BOOL TTinKoopaFlame::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		hitWater();
		return TRUE;
	}
	return FALSE;
}

void TTinKoopaFlame::hitWater()
{
	if (mTinKoopa->mFlameStopTimer > 0)
		return;

	if (mHitPoints > 0)
		mHitPoints--;

	if (mHitPoints <= 0) {
		mHitPoints
		    = (s16)mTinKoopa->getSaveParams()->getSLFlameHP();
		mTinKoopa->mFlameStopTimer
		    = (s16)mTinKoopa->getSaveParams()->getSLFlameRevivalTime();
		onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (!mSprayed) {
		mSprayed = true;
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_FLAME_YUGE,
		                                            &mPosition, 0, this);
	}
}

void TTinKoopaFlame::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);

	if (cue & 1)
		checkMario();

	if (cue & 2) {
		MtxPtr mtx = mTinKoopa->getModel()->getAnmMtx(
		    TTinKoopa_getJointIndex(TINKOOPA_JOINT_FIRE_COL));
		mPosition.x = mtx[0][3];
		mPosition.y = mtx[1][3];
		mPosition.z = mtx[2][3];

		emitFlameEffects();

		if (mTinKoopa->mFlameStopTimer <= 0)
			offHitFlag(HIT_FLAG_NO_COLLISION);

		mSprayed = false;
	}
}

void TTinKoopaFlame::checkMario()
{
	if (mTinKoopa->mFlameStopTimer > 0)
		return;

	if (mTinKoopa->getDamageStage() == 4)
		return;

	int frame;
	if (mTinKoopa->getDamageStage() == 0)
		frame = 2750;
	else
		frame = 3400;

	if (mTinKoopa->checkTruckAnimationPass(frame))
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
}

// UNUSED, 0x18 in the map: six instructions, so the body can only be the bare
// stage test. Nothing calls it; emitFlameEffects compares inline.
bool TTinKoopaFlame::isHighPosition()
{
	return mTinKoopa->getDamageStage() != 0;
}

void TTinKoopaFlame::emitFlameEffects()
{
	if (mTinKoopa->getSpine()->getCurrentNerve()
	    != &TNerveTinKoopaWait::theNerve())
		return;

	MtxPtr mtx = mTinKoopa->getModel()->getAnmMtx(
	    TTinKoopa_getJointIndex(TINKOOPA_JOINT_FIRE));

	f32 height = 1.0f;
	if (mTinKoopa->getDamageStage() != 0)
		height = 1.6f;

	if (mTinKoopa->mFlameStopTimer > 0) {
		mScale -= 0.05f;
		if (mScale < 0.3f)
			mScale = 0.3f;
	} else {
		mScale += 0.05f;
		if (mScale > 1.0f)
			mScale = 1.0f;
	}

	f32 scaleY, scale;
	scale  = mScale * height;
	scaleY = scale;
	if (mTinKoopa->mFlameStopTimer > 0)
		scaleY *= 0.5f;

	JGeometry::TVec3<f32> flameScale(scale, scaleY, scale);

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    TINKOOPA_JPA_MS_MKP_FIRE_A, mtx, 1, this);
	if (emitter)
		emitter->setGlobalScale(flameScale);

	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(TINKOOPA_JPA_MS_MKP_FIRE_B,
	                                                      mtx, 1, this);
	if (emitter)
		emitter->setGlobalScale(flameScale);

	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(TINKOOPA_JPA_MS_MKP_FIRE_C,
	                                                      mtx, 3, this);
	if (emitter)
		emitter->setGlobalScale(flameScale);

	gpMSound->startSoundActorWithInfo(MSD_SE_BS_MKP_FIRE, &mPosition, nullptr,
	                                  scale, 0, 0, nullptr, 0, 4);
}

// UNUSED, 0x1c in the map.
TTinKoopaLaunchOrder::TTinKoopaLaunchOrder(TTinKoopa* tin_koopa)
    : mTinKoopa(tin_koopa)
    , mLap(0)
    , mFrame(0)
    , mCount(0)
    , mDirection(0)
{
}

// UNUSED, 0x14 in the map: four stores.
void TTinKoopaLaunchOrder::makeOrder(s8 lap, long frame, s8 count,
                                     s8 direction)
{
	mLap       = lap;
	mFrame     = frame;
	mCount     = count;
	mDirection = direction;
}

void TTinKoopaLaunchOrder::checkOrder()
{
	if (mTinKoopa->getLap() != mLap)
		return;

	if (!mTinKoopa->checkTruckAnimationPass(mFrame))
		return;

	int count = 1;
	if (mCount == -1) {
		if (mTinKoopa->getDamageStage() == 0)
			count = 1;
		else if (mTinKoopa->getDamageStage() == 1)
			count = 1;
		else if (mTinKoopa->getDamageStage() == 2)
			count = 2;
		else if (mTinKoopa->getDamageStage() == 3)
			count = 3;
	} else {
		count = mCount;
	}

	// The right-hand launchers are the two the arms cover, so once an arm is
	// gone only two of them are left.
	if ((int)mDirection == 1)
		count = count <= 2 ? count : 2;

	int num = count;
	if (num > 4)
		num = 4;
	mTinKoopa->makeKillerQueue(num, mDirection);
}

// UNUSED, 0xa4 in the map: inlined into TTinKoopa::init.
TTinKoopaLaunchSchedule::TTinKoopaLaunchSchedule(u8 num, TTinKoopa* tin_koopa)
    : mOrderNum(num)
    , mTinKoopa(tin_koopa)
{
	mOrders = new TTinKoopaLaunchOrder*[mOrderNum];
	for (int i = 0; i < mOrderNum; i++)
		mOrders[i] = new TTinKoopaLaunchOrder(mTinKoopa);
}

// UNUSED, 0x64 in the map: inlined into TTinKoopa::perform.
void TTinKoopaLaunchSchedule::checkOrder()
{
	for (int i = 0; i < mOrderNum; i++)
		mOrders[i]->checkOrder();
}

// UNUSED, 0x94 in the map: inlined into TTinKoopa::init.
TTinKoopaPartsBase::TTinKoopaPartsBase(const char* name, int index,
                                       TTinKoopa* tin_koopa)
    : TLiveActor(name)
    , mBreaking(false)
    , mPartsIndex(index)
    , mTinKoopa(tin_koopa)
    , mPartsMActor(nullptr)
{
}

void TTinKoopaPartsBase::initTinKoopaPartsBase()
{
	initHitActor(TTinKoopa_getActorType(mPartsIndex), 0, 0, 0.0f, 0.0f, 0.0f,
	             0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	TIdxGroupObj* group
	    = JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ");
	group->getChildren().push_back(this);

	mCollision = new TMapCollisionMove();
	mCollision->init(TTinKoopa_getCollisionFileName(mPartsIndex), 0, this);

	JGeometry::TVec3<f32> origin(0.0f, 0.0f, 0.0f);
	mCollision->setUpTrans(origin);

	const char* modelName = TTinKoopa_getPartsFileName(mPartsIndex);
	if (modelName) {
		mPartsMActor
		    = mTinKoopa->getActorKeeper()->createMActor(modelName, 0);
		mPartsMActor->setLightType(1);
	}

	resetTinKoopaPartsBase();
}

void TTinKoopaPartsBase::reset() { resetTinKoopaPartsBase(); }

// UNUSED, 0x4c in the map: the same size as reset(), which is nothing but a
// call to it.
void TTinKoopaPartsBase::resetTinKoopaPartsBase()
{
	mBreaking = false;

	JGeometry::TVec3<f32> origin(0.0f, 0.0f, 0.0f);
	mCollision->setUpTrans(origin);
}

void TTinKoopaPartsBase::startBreaking()
{
	mBreaking = true;

	int joint  = TTinKoopa_getJointIndex(mPartsIndex);
	MtxPtr mtx = mTinKoopa->getModel()->getAnmMtx(joint);
	mPosition.x = mtx[0][3];
	mPosition.y = mtx[1][3];
	mPosition.z = mtx[2][3];

	if (mPartsMActor) {
		mPartsMActor->setBckFromIndex(
		    TTinKoopa_getBreakingAnimationIndex(mPartsIndex));

		MtxPtr base = mPartsMActor->getModel()->getBaseTRMtx();
		base[0][3]  = getPosition().x;
		base[1][3]  = getPosition().y;
		base[2][3]  = mPosition.z;
		mPartsMActor->getModel()->setBaseTRMtx(base);

		if (mPartsMActor)
			emitPartsTrackEffects();
	}
}

// UNUSED, 0x2fc in the map. It must exist as a function even though the retail
// object has no copy of it: startBreaking's four calls to the two-argument
// overload are this switch inlined, and the out-of-line copy is big because
// there the overload inlines too.
void TTinKoopaPartsBase::emitPartsTrackEffects()
{
	if (mPartsIndex == TINKOOPA_PARTS_BREAST)
		emitPartsTrackEffects(breastTrackJointNameTable, 6);
	else if (mPartsIndex == TINKOOPA_PARTS_STOMACH)
		emitPartsTrackEffects(bellyTrackJointNameTable, 6);
	else if (mPartsIndex == TINKOOPA_PARTS_RARM)
		emitPartsTrackEffects(rightArmTrackJointNameTable, 4);
	else if (mPartsIndex == TINKOOPA_PARTS_LARM)
		emitPartsTrackEffects(leftArmTrackJointNameTable, 4);
}

void TTinKoopaPartsBase::emitPartsTrackEffects(const char** joint_names,
                                               int num)
{
	mPartsMActor->getModel()->calc();

	JUTNameTab* jointNames
	    = mPartsMActor->getModel()->getModelData()->getJointName();

	for (int i = 0; i < num; i++) {
		int joint = jointNames->getIndex(joint_names[i]);
		if (joint < 0)
			return;

		MtxPtr mtx = mPartsMActor->getModel()->getAnmMtx(joint);
		mEffectPos[i].set(mtx[0][3], mtx[1][3], mtx[2][3]);

		gpMarioParticleManager->emitAndBindToPosPtr(
		    TINKOOPA_JPA_MS_MKP_KEMU_PARTS, &mEffectPos[i], 0, mTinKoopa);
	}
}

void TTinKoopaPartsBase::emitPartsDisappearEffects()
{
	if (!mPartsMActor)
		return;

	if (!mPartsMActor->checkCurBckFromIndex(
	        TTinKoopa_getBreakingAnimationIndex(mPartsIndex)))
		return;

	// The frame stays an int all the way into checkPass: the retail object
	// converts it at run time instead of loading a float constant.
	J3DFrameCtrl* ctrl = mPartsMActor->getFrameCtrl(ANM_TYPE_BCK);
	int disappearFrame = 60;
	if (!ctrl->checkPass((f32)disappearFrame))
		return;

	if (mPartsIndex == TINKOOPA_PARTS_BREAST)
		emitPartsDisappearEffects(breastTrackJointNameTable, 6, 4.0f);
	else if (mPartsIndex == TINKOOPA_PARTS_STOMACH)
		emitPartsDisappearEffects(bellyTrackJointNameTable, 6, 4.0f);
	else if (mPartsIndex == TINKOOPA_PARTS_RARM)
		emitPartsDisappearEffects(rightArmTrackJointNameTable, 4, 3.0f);
	else if (mPartsIndex == TINKOOPA_PARTS_LARM)
		emitPartsDisappearEffects(leftArmTrackJointNameTable, 4, 3.0f);

	mTinKoopa->mBreakingParts = nullptr;
}

void TTinKoopaPartsBase::emitPartsDisappearEffects(const char** joint_names,
                                                   int num, f32 scale)
{
	JGeometry::TVec3<f32> effectScale(mScaling);
	effectScale.scale(scale);

	JUTNameTab* jointNames
	    = mPartsMActor->getModel()->getModelData()->getJointName();

	for (int i = 0; i < num; i++) {
		int joint = jointNames->getIndex(joint_names[i]);
		if (joint < 0)
			return;

		MtxPtr mtx = mPartsMActor->getModel()->getAnmMtx(joint);
		mEffectPos[i].set(mtx[0][3], mtx[1][3], mtx[2][3]);

		TEffectExplosion* explosion
		    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
		        mEffectPos[i], "エフェクト爆発マネージャー", 1);
		if (!explosion)
			return;

		explosion->generate(mEffectPos[i], effectScale);
	}
}

BOOL TTinKoopaPartsBase::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000002B) {
		mTinKoopa->hitParts();
		return TRUE;
	}
	return FALSE;
}

// TODO: the TMapCollisionMove::moveMtx this reaches is 32.8%: the retail body
// is MTXCopy(mtx, unk20) followed by move(), and the inline in
// include/Map/MapCollisionEntry.hpp only does the copy. That header is outside
// this batch's scope; adding the move() call there is the fix.
void TTinKoopaPartsBase::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TLiveActor::perform(cue, graphics);

	if (cue & 1)
		mCollision->moveMtx(mTinKoopa->getModel()->getAnmMtx(
		    TTinKoopa_getJointIndex(mPartsIndex)));

	if ((cue & 2) && mBreaking) {
		if (mPartsMActor && mPartsMActor->curAnmEndsNext(ANM_TYPE_BCK, nullptr))
			mBreaking = false;
	}

	if (mBreaking) {
		if (mPartsMActor)
			mPartsMActor->perform(cue, graphics);
	}
}

// UNUSED, 0x98 in the map: nothing in the retail object ever makes one.
TTinKoopaMtxCalc::TTinKoopaMtxCalc(TTinKoopa* tin_koopa)
    : mTinKoopa(tin_koopa)
{
}

// UNUSED, 0x38 in the map.
void TTinKoopaMtxCalc::joinAnm(int index)
{
	M3UMtxCalcSIAnmBlendQuat::joinAnm(
	    mTinKoopa->getMActor()->getCurBckAnmPtr());
}

void TTinKoopaMtxCalc::calc(u16 joint)
{
	M3UMtxCalcSIAnmBlendQuat::calc(joint);
}

TTinKoopa::TTinKoopa(const char* name)
    : TSpineEnemy(name)
    , mKillerManager(nullptr)
    , unk1F8(0)
{
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_UNK100);
	mScaledBodyRadius = 2000.0f;
}

// UNUSED, 0x16c in the map: inlined into init.
void TTinKoopa::makeCoasterDistanceTable()
{
	mCoasterDistances = new f32[mKillerGraph->getNodeNum() - 1];

	// The running total is never used afterwards, but the retail object adds
	// it up all the same, so the original kept the lap length around.
	f32 total = 0.0f;

	JGeometry::TVec3<f32> next = mKillerGraph->indexToPoint(0);
	for (int i = 0; i < mKillerGraph->getNodeNum() - 1; i++) {
		JGeometry::TVec3<f32> cur = next;
		next                      = mKillerGraph->indexToPoint(i + 1);
		mCoasterDistances[i]      = cur.distance(next);
		total += mCoasterDistances[i];
	}
}

f32 TTinKoopa::calcCoasterDistance(int from, int to)
{
	f32 distance = 0.0f;
	for (int i = from; i < to; i++)
		distance += mCoasterDistances[i];
	return distance;
}

// UNUSED, 0x220 in the map. checkKillerApproachingFromBack wants exactly this
// and spells the two branches out instead of calling it, which is why its own
// copy is this big: there calcCoasterDistance inlines three times over.
f32 TTinKoopa::calcCoasterDistanceInOrder(int from, int to)
{
	if (to >= from)
		return calcCoasterDistance(from, to);

	return calcCoasterDistance(from, mKillerGraph->getNodeNum() - 1)
	    + calcCoasterDistance(0, to);
}

// UNUSED, 0xdc in the map: inlined into
// checkTinKoopaKillerApproachingMessage.
bool TTinKoopa::checkKillerApproachingFromBack(TCoasterKiller* killer,
                                               JGeometry::TVec3<f32> pos,
                                               f32 limit)
{
	if (killer->checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	if (killer->getPathDir() != 0)
		return false;

	int marioNode  = mKillerGraph->findNearestNodeIndex(pos, -1);
	int killerNode = killer->getPathIdx();

	return calcCoasterDistanceInOrder(killerNode, marioNode) <= limit;
}

void TTinKoopa::reset()
{
	TSpineEnemy::reset();

	mFlame->resetTinKoopaFlame();

	for (int i = 0; i < TINKOOPA_PARTS_NUM; i++)
		mParts[i]->reset();

	makeHitCollision();
	resetTinKoopa();

	changeBck(TTinKoopa_getWaitAnimationIndex(mDamageStage));
}

// Instruction-exact; the frame is 0x70 against the ROM's 0x88.
static inline TMario* TinKoopaResetMarioRef()
{
	return gpMarioOriginal;
}

static inline TMario* TinKoopaResetMario()
{
	TMario* mario = TinKoopaResetMarioRef();
	return mario;
}

void TTinKoopa::resetTinKoopa()
{
	if (!mKillerManager) {
		TCoasterKillerManager* manager
		    = (TCoasterKillerManager*)JDrama::TNameRefGen::search<
		        TCoasterKillerManager>("コースターキラーマネージャー");
		mKillerManager = manager;
	}

	// TODO: the retail object reads mKillerManager->unk38 here and throws the
	// value away, so something the original wrote between the search and the
	// rail fetch went through the manager's save params. One dead `lwz`.
	// The retail object asks the killer manager for its active count here and
	// throws the answer away: all that is left of the statement is the two
	// loads and the null test getActiveObjNum() starts with. Dropping it
	// loses exactly those two instructions.
	mKillerManager->getActiveObjNum();

	mTruckMActor = TinKoopaResetMario()->mKoopaRail;

	mDamageStage = 0;
	unk154       = 0;
	unk158       = 0;
	mLap         = 0;

	mFirstFlameMessageDone = false;

	mPartsHitPoints = getSaveParams()->getSLPartsHP();
	mBreakingParts  = nullptr;

	mKillerNum   = 0;
	mKillerIndex = 0;
	for (int i = 0; i < 4; i++)
		mKillerDirs[i] = 0;

	mKillerIntervalTimer = 0;
	mFlameStopTimer      = 0;
	mDefeatWaitTimer     = 0;
	mKillerIntervalTimer = 0;

	unk1B4 = 0.0f;
	unk1B8 = 30.0f;
	unk1BC = 15.0f;
	unk1C0 = 45.0f;
}

// UNUSED, 0x1a0 in the map: the damage-stage switch plus the flame's own
// collision, inlined into both reset() and the break nerve.
void TTinKoopa::makeHitCollision()
{
	// TODO: the ROM keeps the third arm's `cmpwi r0, 2` (and the `b` over it
	// that arm 1 needs), which MWCC only emits when the arm holds a statement
	// that generates no code -- an empty `{ }` body is deleted compare and
	// all.  Which statement is not recoverable: a placeholder `a = a;` lands
	// TTinKoopa::reset at 99.9% and TNerveTinKoopaBreak::execute at 100%, but
	// it is a mechanism, not a plausible source, so it is not committed.  A
	// `switch` with an empty `case 2:` is refuted (reset 98.6% -> 94.2%: it
	// builds a range tree).
	if (mDamageStage == 0)
		setHitParams(0.0f, 0.0f, getSaveParams()->mSLDamageRadius.get(),
		             getSaveParams()->mSLDamageHeight0.get());
	else if (mDamageStage == 1)
		setHitParams(0.0f, 0.0f, getSaveParams()->mSLDamageRadius.get(),
		             getSaveParams()->mSLDamageHeight1.get());
	else if (mDamageStage == 2) { }

	mFlame->makeHitCollision();
}

// UNUSED, 0x44 in the map: inlined into TTinKoopaLaunchOrder::checkOrder.
void TTinKoopa::makeKillerQueue(int num, s8 direction)
{
	mKillerIndex = 0;
	mKillerNum   = num;

	for (int i = 0; i < mKillerNum; i++)
		mKillerDirs[i] = direction;

	mKillerIntervalTimer = 0;
}

// UNUSED, 0x8c in the map: inlined into perform.
void TTinKoopa::checkLap()
{
	if (!mTruckMActor)
		return;

	J3DFrameCtrl* ctrl = mTruckMActor->getFrameCtrl(ANM_TYPE_BCK);
	if (ctrl->checkPass((f32)(ctrl->getEnd() - 1))) {
		mLap++;
		if (mLap > 2)
			mLap = 0;
	}
}

// UNUSED, 0x78 in the map: every scripted moment of the fight goes through
// this, so it is inlined at each of them.
bool TTinKoopa::checkTruckAnimationPass(int frame)
{
	if (!mTruckMActor)
		return false;

	if (mTruckMActor->getFrameCtrl(ANM_TYPE_BCK)->checkPass((f32)frame))
		return true;

	return false;
}

// UNUSED, 0x24c in the map, and nothing references it: a debug dump of the
// fight's state. Whatever it printed was dead-stripped along with it, so the
// format strings are not in the retail object and cannot be recovered -- and
// inventing any would push this TU's whole string pool along. The body is
// therefore left empty on purpose.
// TODO: incorrect size.
static void printTinKoopaDebugInfo(TTinKoopa* tin_koopa) { }

void TTinKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & 2) {
		emitTinKoopaEffects();
		checkKillerLaunch();
	}

	if (cue & 1) {
		updateTimers();
		checkLap();
		mLaunchSchedule->checkOrder();
		checkTinKoopaMessage();
	}

	TSpineEnemy::perform(cue, graphics);

	mFlame->perform(cue, graphics);

	for (int i = 0; i < TINKOOPA_PARTS_NUM; i++)
		mParts[i]->perform(cue, graphics);
}

// UNUSED, 0x3b4 in the map, and nothing references it: the eye beam never made
// it into the fight. Only the four floats resetTinKoopa initialises say what it
// would have swept, so the body below is a reconstruction from those and from
// the map's weak JGeometry::TRotation3::setEular, which is the one header
// inline this TU pulls in and nothing else here uses.
// TODO: incorrect size, and setEular itself is missing from our object: MWCC
// expands it here while the retail compile kept a weak out-of-line copy (0x100,
// dead-stripped). A minimal body, a second call and a following mult33 all
// still inline, and TMapObjBase::rotateVecByAxisY gets the call with the very
// same shape, so the lever is not the call site.
void TTinKoopa::makeEyeBeamEffect()
{
	unk1B4 += unk1BC;
	if (unk1B4 > unk1C0)
		unk1B4 = unk1C0;

	JGeometry::TVec3<f32> leftEye;
	getJointTransByIndex(TTinKoopa_getJointIndex(TINKOOPA_JOINT_LEYE),
	                     &leftEye);

	JGeometry::TRotation3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    leftBeam;
	leftBeam.setEular(mRotation.y, unk1B4, 0.0f);

	JGeometry::TVec3<f32> rightEye;
	getJointTransByIndex(TTinKoopa_getJointIndex(TINKOOPA_JOINT_REYE),
	                     &rightEye);

	JGeometry::TRotation3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    rightBeam;
	rightBeam.setEular(mRotation.y, -unk1B4, 0.0f);

	unk1C4 = unk1B8;
}

// UNUSED, 0x4c in the map: the three countdowns of perform's movement cue.
// TODO: the retail object takes the address of each timer before storing it
// back (`addi r4, this, 0x178` then `stw r0, 0(r4)`), which is what an inlined
// helper taking a pointer looks like; writing one (`countDownTimer(int*)`,
// inline so it leaves no symbol) gives byte-identical code to the plain form,
// so the address-of has another cause.
void TTinKoopa::updateTimers()
{
	if (mKillerIntervalTimer > 0)
		mKillerIntervalTimer--;

	if (mFlameStopTimer > 0)
		mFlameStopTimer--;

	if (mDefeatWaitTimer > 0)
		mDefeatWaitTimer--;
}

const char** TTinKoopa::getBasNameTable() const { return tinkoopa_bastable; }

// UNUSED, 0x70 in the map: the animation switch every nerve does.
void TTinKoopa::changeBck(int index)
{
	getMActor()->setBckFromIndex(index);

	const char** table = getBasNameTable();
	setAnmSound(table == nullptr ? nullptr : table[index]);
}

// TODO: 0%. The retail object calls hitParts() here; ours expands it, so the
// whole 596-byte body lands inside these 60. hitParts sits exactly three
// statements under MWCC's per-callee inline budget -- three dead `if`s in it
// flip this function to 100% -- but no plausible spelling of its body (early
// returns instead of nested ifs, a spelled-out console call, a named local)
// finds those three statements. Same story for
// TTinKoopaPartsBase::receiveMessage, which calls the same helper.
BOOL TTinKoopa::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000002B) {
		hitParts();
		return TRUE;
	}
	return FALSE;
}

// The six statements this body was missing: the three guards are early returns
// rather than two nested `if`s (+3), each pushNerve names its nerve (+2, and
// that is also what fixes retail's r5/r6 assignment in both inlined pushNerve
// expansions -- retail creates the argument temporary before the receiver),
// and the decremented hit points are read back into a named local (+1, which
// also brings the frame from 0x60 to 0x68). At 15 statements the body is over
// MWCC's depth-1 inline budget, so both TTinKoopa::receiveMessage and
// TTinKoopaPartsBase::receiveMessage `bl` it as retail does; both 0 -> 100.
//
// TODO: instruction-exact; 8 bytes of frame left (0x68 against 0x70), i.e. one
// more named local with a stack slot. Naming mDamageStage costs a statement
// but no frame, so it is not the one.
void TTinKoopa::hitParts()
{
	if (mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		return;
	if (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve())
		return;
	if (mDamageStage == 4)
		return;

	startTinKoopaMessage(BALLOON_MSG_TINKOOPA_PARTS_HIT);

	mPartsHitPoints--;

	int hitPoints = mPartsHitPoints;
	if (hitPoints <= 0) {
		TNerveBase<TLiveActor>* nerve = &TNerveTinKoopaBreak::theNerve();
		mSpine->pushNerve(nerve);
		return;
	}

	TNerveBase<TLiveActor>* nerve = &TNerveTinKoopaDamage::theNerve();
	mSpine->pushNerve(nerve);
}

// UNUSED, 0x4c in the map: inlined into the break nerve.
void TTinKoopa::startBreakingParts()
{
	mBreakingParts = mParts[TTinKoopa_getBreakingPartsIndex(mDamageStage)];
	mBreakingParts->startBreaking();
}

// Binding level over a raw member read, worth +8 of low region in
// TTinKoopa::launchKiller (batch 127).
static inline int TinkoopaKillerIndex(const TTinKoopa* p)
{
	int killerIndex = p->mKillerIndex;
	return killerIndex;
}

void TTinKoopa::launchKiller(int direction)
{
	TCoasterKiller* killer
	    = (TCoasterKiller*)mKillerManager->getDeadEnemy();
	if (!killer)
		return;

	killer->reset();

	int slot;
	if (direction == 1)
		slot = TINKOOPA_JOINT_KILLER_FIRST + TinkoopaKillerIndex(this);
	else
		slot = (TINKOOPA_JOINT_KILLER_FIRST + 3) - mKillerIndex;

	int joint = TTinKoopa_getJointIndex(slot);
	getJointTransByIndex(joint, &killer->mPosition);
	killer->mPathDir = direction;

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    TINKOOPA_JPA_MS_MKP_KILLER, getModel()->getAnmMtx(joint), 0, this);

	gpMSound->startSoundActor(MSD_SE_EN_KILLER_FIRE, &killer->mPosition, 0,
	                          nullptr, 0, 4);
}

// UNUSED, 0xd8 in the map: inlined into perform's draw cue.
void TTinKoopa::checkKillerLaunch()
{
	if (mSpine->getCurrentNerve() != &TNerveTinKoopaWait::theNerve())
		return;

	if (mKillerIndex >= mKillerNum)
		return;

	if (mKillerIntervalTimer > 0)
		return;

	launchKiller(mKillerDirs[mKillerIndex]);
	mKillerIndex++;
	mKillerIntervalTimer = getSaveParams()->getSLKillerInterval();
}

// UNUSED, 0x1e0 in the map: the three message checks of perform's movement
// cue, which the retail object has inlined together.
void TTinKoopa::checkTinKoopaMessage()
{
	checkTinKoopaKillerApproachingMessage();

	if (mTruckMActor) {
		J3DFrameCtrl* ctrl = mTruckMActor->getFrameCtrl(ANM_TYPE_BCK);
		if (mLap == 0 && ctrl->checkPass(300.0f))
			startTinKoopaMessage(BALLOON_MSG_TINKOOPA_LAP);
	}

	checkTinKoopaFirstFlameMessage();
}

void TTinKoopa::checkTinKoopaKillerApproachingMessage()
{
	for (int i = 0; i < mKillerManager->getActiveObjNum(); i++) {
		TCoasterKiller* killer = (TCoasterKiller*)mKillerManager->getObj(i);
		if (killer->checkLiveFlag(LIVE_FLAG_DEAD))
			continue;

		if (checkKillerApproachingFromBack(
		        killer, *gpMarioPos,
		        getSaveParams()->getSLKillerApproachingDistance()))
			startTinKoopaMessage(BALLOON_MSG_TINKOOPA_KILLER_APPROACHING);
	}
}

// UNUSED, 0x6c in the map, and nothing references it: the rocket nozzle hint
// the fight does not use. Modelled on the flame one, which is the same shape
// without the stage split.
// TODO: incorrect size.
void TTinKoopa::checkTinKoopaFirstRocketMessage()
{
	if (!mTruckMActor)
		return;

	if (mFirstFlameMessageDone)
		return;

	if (mSpine->getCurrentNerve() != &TNerveTinKoopaWait::theNerve())
		return;

	if (mTruckMActor->getFrameCtrl(ANM_TYPE_BCK)->checkPass(2600.0f)) {
		startTinKoopaMessage(BALLOON_MSG_TINKOOPA_FIRST_FLAME);
		mFirstFlameMessageDone = true;
	}
}

// Binding level over a raw member read, worth +16 of low region in
// TTinKoopa::checkTinKoopaFirstFlameMessage (batch 127).
static inline MActor* TinkoopaTruckMActor(const TTinKoopa* p)
{
	MActor* truckMActor = p->mTruckMActor;
	return truckMActor;
}

void TTinKoopa::checkTinKoopaFirstFlameMessage()
{
	if (!TinkoopaTruckMActor(this))
		return;

	if (mFirstFlameMessageDone)
		return;

	if (mSpine->getCurrentNerve() != &TNerveTinKoopaWait::theNerve())
		return;

	J3DFrameCtrl* ctrl = TinkoopaTruckMActor(this)->getFrameCtrl(ANM_TYPE_BCK);
	if (mDamageStage == 0) {
		if (ctrl->checkPass(2600.0f)) {
			startTinKoopaMessage(BALLOON_MSG_TINKOOPA_FIRST_FLAME);
			mFirstFlameMessageDone = true;
		}
	} else if (ctrl->checkPass(3100.0f)) {
		startTinKoopaMessage(BALLOON_MSG_TINKOOPA_FIRST_FLAME);
		mFirstFlameMessageDone = true;
	}
}

// UNUSED, 0x2c in the map.
void TTinKoopa::startTinKoopaMessage(u32 id)
{
	SMSGetMarDirector()->getConsole()->startAppearBalloon(id, true);
}

// The ten leading joint-matrix reads bind the model into a named local; the
// nerve-tested tail below reads it straight through the accessor.
static inline J3DModel* TinkoopaEffectModel(const TTinKoopa* p)
{
	J3DModel* model = p->getModel();
	return model;
}

void TTinKoopa::emitTinKoopaEffects()
{
	MtxPtr mtx;

	mtx = TinkoopaEffectModel(this)->getAnmMtx(
	    TTinKoopa_getJointIndex(TINKOOPA_JOINT_HEAD));
	mHeadPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	mtx = TinkoopaEffectModel(this)->getAnmMtx(
	    TTinKoopa_getJointIndex(TINKOOPA_JOINT_BREAST));
	mBreastPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	mtx = TinkoopaEffectModel(this)->getAnmMtx(
	    TTinKoopa_getJointIndex(TINKOOPA_JOINT_RARM));
	mRightArmPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	mtx = TinkoopaEffectModel(this)->getAnmMtx(
	    TTinKoopa_getJointIndex(TINKOOPA_JOINT_LARM));
	mLeftArmPos.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_HIBANA_W1BR,
	                                            &mBreastPos, 1, this);

	if (mDamageStage > 1)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    TINKOOPA_JPA_MS_MKP_HIBANA_W3AR,
		    TinkoopaEffectModel(this)->getAnmMtx(
		        TTinKoopa_getJointIndex(TINKOOPA_JOINT_RARM)),
		    1, this);

	if (mDamageStage > 2)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    TINKOOPA_JPA_MS_MKP_HIBANA_W4AR,
		    TinkoopaEffectModel(this)->getAnmMtx(
		        TTinKoopa_getJointIndex(TINKOOPA_JOINT_LARM)),
		    1, this);

	if (mDamageStage <= 0) {
		if (mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve())
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    TINKOOPA_JPA_MS_MKP_BIRI_W1ST,
			    TinkoopaEffectModel(this)->getAnmMtx(
			        TTinKoopa_getJointIndex(TINKOOPA_JOINT_STOMACH)),
			    1, this);
	}

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    TINKOOPA_JPA_MS_MKP_BIRI_W1AR,
	    TinkoopaEffectModel(this)->getAnmMtx(TTinKoopa_getJointIndex(TINKOOPA_JOINT_RARM)), 1,
	    this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    TINKOOPA_JPA_MS_MKP_BIRI_W1AR,
	    TinkoopaEffectModel(this)->getAnmMtx(TTinKoopa_getJointIndex(TINKOOPA_JOINT_LARM)), 1,
	    this + 1);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    TINKOOPA_JPA_MS_MKP_BIRI_W1FE,
	    TinkoopaEffectModel(this)->getAnmMtx(TTinKoopa_getJointIndex(TINKOOPA_JOINT_FEMUR)),
	    1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    TINKOOPA_JPA_MS_MKP_BIRI_W1HE,
	    getModel()->getAnmMtx(TTinKoopa_getJointIndex(TINKOOPA_JOINT_HEAD)), 1,
	    this);

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()) {
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_BIRI_D1BR_A,
		                                            &mBreastPos, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_BIRI_D1BR_A,
		                                            &mBreastPos, 1, this + 1);
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_BIRI_D1BR_B,
		                                            &mBreastPos, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_BIRI_D1BR_B,
		                                            &mBreastPos, 1, this + 1);
	}

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mDamageStage > 1)
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_KEMU_B1AR,
		                                            &mRightArmPos, 1, this);

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mDamageStage > 2)
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		gpMarioParticleManager->emitAndBindToPosPtr(
		    TINKOOPA_JPA_MS_MKP_KEMU_B1AR, &mLeftArmPos, 1, this + 1);

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mDamageStage > 0)
	    || (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	        && mDamageStage > 0)
	    || (mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()
	        && (mDamageStage == 1 || mDamageStage == 2))) {
		MtxPtr breast = getModel()->getAnmMtx(
		    TTinKoopa_getJointIndex(TINKOOPA_JOINT_BREAST));
		gpMarioParticleManager->emitAndBindToMtxPtr(TINKOOPA_JPA_MS_MKP_KEMU_W2BR_A,
		                                            breast, 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(TINKOOPA_JPA_MS_MKP_KEMU_W2BR_B,
		                                            breast, 1, this);
	}

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mDamageStage > 2)
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		gpMarioParticleManager->emitAndBindToPosPtr(TINKOOPA_JPA_MS_MKP_KEMU_B1HE,
		                                            &mHeadPos, 1, this);

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()) {
		MtxPtr femur = getModel()->getAnmMtx(
		    TTinKoopa_getJointIndex(TINKOOPA_JOINT_FEMUR));
		gpMarioParticleManager->emitAndBindToMtxPtr(TINKOOPA_JPA_MS_MKP_KEMU_B1FE_R,
		                                            femur, 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(TINKOOPA_JPA_MS_MKP_KEMU_B1FE_L,
		                                            femur, 1, this);
	}

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()) {
		if ((mDamageStage == 0 || mDamageStage == 3)
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(100.0f))
			gpCameraShake->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);

		if ((mDamageStage == 1 || mDamageStage == 2)
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(104.0f))
			gpCameraShake->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);

		if ((mDamageStage == 0 || mDamageStage == 3)
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(108.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    TINKOOPA_JPA_MS_MKP_SMOKE1,
			    getModel()->getAnmMtx(
			        TTinKoopa_getJointIndex(TINKOOPA_JOINT_STOMACH)),
			    0, this);

		if (mDamageStage == 0
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(100.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    TINKOOPA_JPA_MS_MKP_PARGE_B14,
			    getModel()->getAnmMtx(
			        TTinKoopa_getJointIndex(TINKOOPA_JOINT_STOMACH)),
			    0, this);

		if (mDamageStage == 3
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(100.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    TINKOOPA_JPA_MS_MKP_PARGE_B14,
			    getModel()->getAnmMtx(
			        TTinKoopa_getJointIndex(TINKOOPA_JOINT_BREAST)),
			    0, this);

		if (mDamageStage == 1
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(106.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    TINKOOPA_JPA_MS_MKP_PARGE_B23,
			    getModel()->getAnmMtx(
			        TTinKoopa_getJointIndex(TINKOOPA_JOINT_RARM)),
			    0, this);

		if (mDamageStage == 2
		    && getMActor()->getFrameCtrl(ANM_TYPE_BCK)->checkPass(106.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    TINKOOPA_JPA_MS_MKP_PARGE_B23,
			    getModel()->getAnmMtx(
			        TTinKoopa_getJointIndex(TINKOOPA_JOINT_LARM)),
			    0, this);
	}

	if (mBreakingParts)
		mBreakingParts->emitPartsDisappearEffects();
}

TTinKoopaManager::TTinKoopaManager(const char* name)
    : TEnemyManager(name)
{
}

void TTinKoopaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "tinkoopa_body.bmd", 0x14240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TTinKoopaManager::load(JSUMemoryInputStream& stream)
{
	TEnemyManager::load(stream);
	unk38 = new TTinKoopaParams("/enemy/tinkoopa.prm");
}

void TTinKoopaManager::loadAfter()
{
	static const char* onetimeFilenames[7] = {
		"/scene/tinkoopa/jpa/ms_mkp_hibana_d1he.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_killer.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_smoke1.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_parge_b14.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_parge_b23.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_flame_yuge.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_parts.jpa",
	};
	for (int i = 0; i < 7; i++)
		SMS_LoadParticle(onetimeFilenames[i], TINKOOPA_JPA_MS_MKP_HIBANA_D1HE + i);

	static const char* loopFilenames[17] = {
		"/scene/tinkoopa/jpa/ms_mkp_hibana_w1br.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_hibana_w3ar.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_hibana_w4ar.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_biri_w1st.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_biri_w1ar.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_biri_w1fe.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_biri_w1he.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_biri_d1br_a.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_biri_d1br_b.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_b1ar.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_w2br_a.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_w2br_b.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_b1he.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_b1fe_l.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_b1fe_r.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_fire_a.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_fire_b.jpa",
	};
	for (int i = 0; i < 17; i++)
		SMS_LoadParticle(loopFilenames[i], TINKOOPA_JPA_MS_MKP_HIBANA_W1BR + i);

	static const char* loopIndirectFilenames[1] = {
		"/scene/tinkoopa/jpa/ms_mkp_fire_c.jpa",
	};
	for (int i = 0; i < 1; i++)
		SMS_LoadParticle(loopIndirectFilenames[i], TINKOOPA_JPA_MS_MKP_FIRE_C + i);
}

// Mecha-Bowser is placed by hand, so the manager never makes one.
TSpineEnemy* TTinKoopaManager::createEnemyInstance() { return nullptr; }

DEFINE_NERVE(TNerveTinKoopaWait, TLiveActor)
{
	TTinKoopa* tinKoopa = (TTinKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		tinKoopa->changeBck(
		    TTinKoopa_getWaitAnimationIndex(tinKoopa->mDamageStage));
		tinKoopa->mDefeatWaitTimer
		    = tinKoopa->getSaveParams()->getSLDefeatWaitTime();
	}

	if (tinKoopa->mDamageStage == 4 && tinKoopa->mDefeatWaitTimer <= 0)
		TFlagManager::smInstance->setBool(true, 0x5000A);

	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveTinKoopaDamage::execute (batch 127).
static inline TCameraShake* TinkoopaGetCameraShake()
{
	TCameraShake* cameraShake = gpCameraShake;
	return cameraShake;
}

DEFINE_NERVE(TNerveTinKoopaDamage, TLiveActor)
{
	TTinKoopa* tinKoopa = (TTinKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		tinKoopa->changeBck(
		    TTinKoopa_getDamageAnimationIndex(tinKoopa->mDamageStage));
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    TINKOOPA_JPA_MS_MKP_HIBANA_D1HE,
		    tinKoopa->getModel()->getAnmMtx(
		        TTinKoopa_getJointIndex(TINKOOPA_JOINT_HEAD)),
		    0, this);
		TinkoopaGetCameraShake()->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);
	}

	if (tinKoopa->getMActor()->checkCurBckFromIndex(
	        TTinKoopa_getDamageAnimationIndex(tinKoopa->mDamageStage))) {
		if (tinKoopa->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			tinKoopa->changeBck(
			    TTinKoopa_getWaitAnimationIndex(tinKoopa->mDamageStage));
			return TRUE;
		}
	}

	return FALSE;
}

DEFINE_NERVE(TNerveTinKoopaBreak, TLiveActor)
{
	TTinKoopa* tinKoopa = (TTinKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		tinKoopa->changeBck(
		    TTinKoopa_getBreakAnimationIndex(tinKoopa->mDamageStage));
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    TINKOOPA_JPA_MS_MKP_HIBANA_D1HE,
		    tinKoopa->getModel()->getAnmMtx(
		        TTinKoopa_getJointIndex(TINKOOPA_JOINT_HEAD)),
		    0, this);
		gpCameraShake->startShake(CAM_SHAKE_MODE_KILLER, 1.0f);

		// The last plate is the cue for the killers to stop coming and for
		// the boss music to fade out.
		if (tinKoopa->mDamageStage == 3) {
			tinKoopa->mKillerManager->unk60 = 1;
			MSBgm::stopTrackBGMs(7, 10);
		}
	}

	if (tinKoopa->getMActor()->checkCurBckFromIndex(
	        TTinKoopa_getBreakAnimationIndex(tinKoopa->mDamageStage))) {
		TTinKoopaPartsBase* parts = tinKoopa->getParts(
		    TTinKoopa_getBreakingPartsIndex(tinKoopa->mDamageStage));

		if (tinKoopa->getMActor()->curAnmEndsNext(ANM_TYPE_BCK, nullptr)) {
			parts->mCollision->remove();
			tinKoopa->mDamageStage++;
			tinKoopa->makeHitCollision();
			tinKoopa->changeBck(
			    TTinKoopa_getWaitAnimationIndex(tinKoopa->mDamageStage));
			tinKoopa->mPartsHitPoints
			    = tinKoopa->getSaveParams()->getSLPartsHP();
			return TRUE;
		}

		if (!parts->isBreaking()) {
			if (tinKoopa->getMActor()
			        ->getFrameCtrl(ANM_TYPE_BCK)
			        ->checkPass((f32)TTinKoopa_getPartsVisibleFrame(
			            tinKoopa->mDamageStage)))
				tinKoopa->startBreakingParts();
		}
	}

	return FALSE;
}
