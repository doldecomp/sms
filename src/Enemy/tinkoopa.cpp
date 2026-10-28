#include <Enemy/TinKoopa.hpp>
#include <Enemy/CoasterKiller.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/Graph.hpp>
#include <Camera/CameraShake.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSModBgm.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>

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
	"jnt_head",     "jnt_breast",    "jnt_stomach",  "jnt_rarm",
	"jnt_larm",     "jnt_leg",       "jnt_leye",     "jnt_reye",
	"fire_null",    "fire_col_null", "jnt_femur",    "killer_null1",
	"killer_null2", "killer_null3",  "killer_null4",
};

static int TTinKoopa_jointIndexTable[15];

TTinKoopaParams::TTinKoopaParams(const char* path)
    : TSpineEnemyParams(path)
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

static int TTinKoopa_getJointIndex(int index)
{
	return TTinKoopa_jointIndexTable[index];
}

static const char* TTinKoopa_getCollisionFileName(int index)
{
	static const char* table[] = {
		"/scene/tinkoopa/head_col.col",    "/scene/tinkoopa/breast_col.col",
		"/scene/tinkoopa/stomach_col.col", "/scene/tinkoopa/rarm_col.col",
		"/scene/tinkoopa/larm_col.col",    "/scene/tinkoopa/leg_col.col",
	};
	return table[index];
}

static const char* TTinKoopa_getPartsFileName(int index)
{
	static const char* table[] = {
		nullptr,
		"tinkoopa_breast.bmd",
		"tinkoopa_stomach.bmd",
		"tinkoopa_rarm.bmd",
		"tinkoopa_larm.bmd",
		nullptr,
	};
	return table[index];
}

static int TTinKoopa_getBreakingAnimationIndex(int index)
{
	static int table[] = { 0, 4, 11, 9, 10, 0 };
	return table[index];
}

static u32 TTinKoopa_getActorType(int index)
{
	static u32 table[] = {
		ACTOR_TYPE_TIN_KOOPA_HEAD,    ACTOR_TYPE_TIN_KOOPA_BREAST,
		ACTOR_TYPE_TIN_KOOPA_STOMACH, ACTOR_TYPE_TIN_KOOPA_RARM,
		ACTOR_TYPE_TIN_KOOPA_LARM,    ACTOR_TYPE_TIN_KOOPA_LEG,
	};
	return table[index];
}

static int TTinKoopa_getWaitAnimationIndex(int index)
{
	static int table[] = { 12, 13, 14, 15, 16 };
	return table[index];
}

static int TTinKoopa_getBreakAnimationIndex(int index)
{
	static int table[] = { 0, 1, 2, 3, 16 };
	return table[index];
}

static int TTinKoopa_getDamageAnimationIndex(int index)
{
	static int table[] = { 5, 6, 7, 8, 16 };
	return table[index];
}

static int TTinKoopa_getPartsVisibleFrame(int index)
{
	static int table[] = { 100, 134, 134, 100, 0 };
	return table[index];
}

static int TTinKoopa_getBreakingPartsIndex(int index)
{
	static int table[] = { 2, 3, 4, 1, 2 };
	return table[index];
}

static const char* breastTrackJointNameTable[] = {
	"breast_1", "breast_2", "breast_3", "breast_4", "breast_5", "breast_6",
};

static const char* bellyTrackJointNameTable[] = {
	"stomach_1", "stomach_2", "stomach_3",
	"stomach_4", "stomach_5", "stomach_6",
};

static const char* rightArmTrackJointNameTable[] = {
	"rarm_1",
	"rarm_2",
	"rarm_3",
	"rarm_4",
};

static const char* leftArmTrackJointNameTable[] = {
	"larm_1",
	"larm_2",
	"larm_3",
	"larm_4",
};

void TTinKoopa::init(TLiveManager* manager)
{
	mManager = manager;
	mManager->manageActor(this);
	mSpine->initWith(&TNerveTinKoopaWait::theNerve());
	mMActorKeeper = new TMActorKeeper(mManager, 7);
	mMActor       = mMActorKeeper->createMActor("tinkoopa_body.bmd", 0);

	JUTNameTab* names = getModel()->getModelData()->getJointName();
	for (int i = 0; i < 15; ++i)
		TTinKoopa_jointIndexTable[i]
		    = names->getIndex(TTinKoopa_jointNameTable[i]);

	mFlame = new TTinKoopaFlame("flame", this);

	for (int i = 0; i < 6; ++i) {
		mParts[i] = new TTinKoopaPartsBase(TTinKoopa_getCollisionFileName(i), i,
		                                   this);
		mParts[i]->initTinKoopaPartsBase();
	}

	initHitActor(ACTOR_TYPE_TIN_KOOPA, 1, 0, 0.0f, 0.0f,
	             getSaveParam2()->mSLDamageRadius.get(),
	             getSaveParam2()->mSLDamageHeight0.get());
	offHitFilter(HIT_FILTER_NO_COLLISION);

	mMActor->setLightType(1);
	J3DModel* model = mMActor->getModel();
	if (!model->getSkinDeform())
		model->setSkinDeform(new J3DSkinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);

	calcRootMatrix();
	mMActor->calc();

	unk1EC = gpConductor->getGraphByName("killer");
	makeCoasterDistanceTable();

	mLaunchSchedule = new TTinKoopaLaunchSchedule(11, this);
	makeLaunchSchedule();
	initAnmSound();
	resetTinKoopa();
}

void TTinKoopa::makeLaunchSchedule()
{
	TTinKoopaLaunchOrder* order;
	int i = 0;
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

TTinKoopaFlame::TTinKoopaFlame(const char* name, TTinKoopa* owner)
    : THitActor(name)
    , mOwner(owner)
{
	initHitActor(ACTOR_TYPE_TIN_KOOPA_FLAME, 0, 0, 0.0f, 0.0f,
	             mOwner->getSaveParam2()->mSLFlameDamageRadius0.get(),
	             mOwner->getSaveParam2()->mSLFlameDamageHeight0.get());
	offHitFilter(HIT_FILTER_NO_COLLISION);
	static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"))
	    ->getChildren()
	    .push_back(this);
	resetTinKoopaFlame();
}

void TTinKoopaFlame::makeHitCollision()
{
	if (mOwner->mBreakCount == 0) {
		setHitParams(0.0f, 0.0f,
		             mOwner->getSaveParam2()->mSLFlameDamageRadius0.get(),
		             mOwner->getSaveParam2()->mSLFlameDamageHeight0.get());
	} else if (mOwner->mBreakCount == 1) {
		setHitParams(0.0f, 0.0f,
		             mOwner->getSaveParam2()->mSLFlameDamageRadius1.get(),
		             mOwner->getSaveParam2()->mSLFlameDamageHeight1.get());
	}
}

void TTinKoopaFlame::resetTinKoopaFlame()
{
	unk70 = mOwner->getSaveParam2()->mSLFlameHP.get();
	unk6C = 1.0f;
	unk72 = false;
}

BOOL TTinKoopaFlame::receiveMessage(THitActor* sender, u32 message)
{
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		hitWater();
		return true;
	}
	return false;
}

void TTinKoopaFlame::hitWater()
{
	if (mOwner->mTimers[TTinKoopa::TIMER_FLAME_REVIVAL] > 0)
		return;

	if (unk70 > 0)
		--unk70;

	if (unk70 <= 0) {
		unk70 = mOwner->getSaveParam2()->mSLFlameHP.get();
		mOwner->mTimers[TTinKoopa::TIMER_FLAME_REVIVAL]
		    = (s16)mOwner->getSaveParam2()->mSLFlameRevivalTime.get();
		onHitFilter(HIT_FILTER_NO_COLLISION);
	}

	if (!unk72) {
		unk72 = true;
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_FLAME_YUGE, &mPosition, 0, this);
	}
}

void TTinKoopaFlame::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);

	if (cue & CUE_MOVE) {
		if (mOwner->mTimers[TTinKoopa::TIMER_FLAME_REVIVAL] <= 0
		    && mOwner->mBreakCount != 4) {
			int frame = mOwner->mBreakCount == 0 ? 2750 : 3400;
			if (mOwner->checkTruckAnimationPass(frame))
				SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		}
	}

	if (cue & CUE_CALC_ANIM) {
		MtxPtr mtx  = mOwner->getModel()->getAnmMtx(TTinKoopa_getJointIndex(9));
		mPosition.x = mtx[0][3];
		mPosition.y = mtx[1][3];
		mPosition.z = mtx[2][3];
		emitFlameEffects();
		if (mOwner->mTimers[TTinKoopa::TIMER_FLAME_REVIVAL] <= 0)
			offHitFilter(HIT_FILTER_NO_COLLISION);
		unk72 = false;
	}
}

void TTinKoopaFlame::checkMario() { }

bool TTinKoopaFlame::isHighPosition() { }

void TTinKoopaFlame::emitFlameEffects()
{
	if (mOwner->mSpine->getCurrentNerve() != &TNerveTinKoopaWait::theNerve())
		return;

	MtxPtr mtx = mOwner->getModel()->getAnmMtx(TTinKoopa_getJointIndex(8));

	f32 rate = 1.0f;
	if (mOwner->mBreakCount != 0)
		rate = 1.6f;

	if (mOwner->mTimers[TTinKoopa::TIMER_FLAME_REVIVAL] > 0) {
		unk6C -= 0.05f;
		if (unk6C < 0.3f)
			unk6C = 0.3f;
	} else {
		unk6C += 0.05f;
		if (unk6C > 1.0f)
			unk6C = 1.0f;
	}

	f32 scale = unk6C * rate;
	JGeometry::TVec3<f32> scaleVec(scale, scale, scale);
	if (mOwner->mTimers[TTinKoopa::TIMER_FLAME_REVIVAL] > 0)
		scaleVec.y *= 0.5f;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_FIRE_A, mtx, 1, this);
	if (emitter)
		emitter->setGlobalScale(scaleVec);

	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_FIRE_B, mtx, 1, this);
	if (emitter)
		emitter->setGlobalScale(scaleVec);

	emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_FIRE_C, mtx, 3, this);
	if (emitter)
		emitter->setGlobalScale(scaleVec);

	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_MKP_FIRE, &mPosition, nullptr, scale, 0, 0, nullptr, 0, 4);
}

TTinKoopaLaunchOrder::TTinKoopaLaunchOrder(TTinKoopa* owner)
    : mOwner(owner)
    , mLap(0)
    , mFrame(0)
    , mKillerNum(0)
    , mKillerDir(0)
{
}

void TTinKoopaLaunchOrder::makeOrder(s8 lap, s32 frame, s8 num, s8 dir)
{
	mLap       = lap;
	mFrame     = frame;
	mKillerNum = num;
	mKillerDir = dir;
}

void TTinKoopaLaunchOrder::checkOrder()
{
	if (mOwner->mLap != mLap)
		return;

	if (!mOwner->checkTruckAnimationPass(mFrame))
		return;

	int num = 1;
	if (mKillerNum == -1) {
		if (mOwner->mBreakCount == 0)
			num = 1;
		else if (mOwner->mBreakCount == 1)
			num = 1;
		else if (mOwner->mBreakCount == 2)
			num = 2;
		else if (mOwner->mBreakCount == 3)
			num = 3;
	} else {
		num = mKillerNum;
	}

	if (mKillerDir == 1)
		num = num <= 2 ? num : 2;

	mOwner->makeKillerQueue(num, mKillerDir);
}

TTinKoopaLaunchSchedule::TTinKoopaLaunchSchedule(u8 num, TTinKoopa* owner)
    : mOrderNum(num)
    , mOwner(owner)
{
	mOrders = new TTinKoopaLaunchOrder*[mOrderNum];
	for (int i = 0; i < mOrderNum; ++i)
		mOrders[i] = new TTinKoopaLaunchOrder(mOwner);
}

void TTinKoopaLaunchSchedule::checkOrder()
{
	for (int i = 0; i < mOrderNum; ++i)
		mOrders[i]->checkOrder();
}

TTinKoopaPartsBase::TTinKoopaPartsBase(const char* name, int index,
                                       TTinKoopa* owner)
    : TLiveActor(name)
    , mIsBreaking(false)
    , mIndex(index)
    , mOwner(owner)
    , unk104(nullptr)
{
}

void TTinKoopaPartsBase::initTinKoopaPartsBase()
{
	initHitActor(TTinKoopa_getActorType(mIndex), 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);

	mCollision          = new TMapCollisionMove;
	const char* colName = TTinKoopa_getCollisionFileName(mIndex);
	mCollision->init(colName, 0, this);
	mCollision->setUpTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));

	const char* fileName = TTinKoopa_getPartsFileName(mIndex);
	if (fileName) {
		unk104 = mOwner->mMActorKeeper->createMActor(fileName, 0);
		unk104->setLightType(1);
	}

	resetTinKoopaPartsBase();
}

void TTinKoopaPartsBase::reset() { resetTinKoopaPartsBase(); }

void TTinKoopaPartsBase::resetTinKoopaPartsBase()
{
	mIsBreaking = false;
	mCollision->setUpTrans(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
}

void TTinKoopaPartsBase::startBreaking()
{
	mIsBreaking = true;

	MtxPtr mtx = mOwner->getModel()->getAnmMtx(TTinKoopa_getJointIndex(mIndex));
	mPosition.x = mtx[0][3];
	mPosition.y = mtx[1][3];
	mPosition.z = mtx[2][3];

	if (unk104) {
		unk104->setBckFromIndex(TTinKoopa_getBreakingAnimationIndex(mIndex));
		J3DModel* model = unk104->getModel();
		MtxPtr baseMtx  = model->getBaseTRMtx();
		baseMtx[0][3]   = mPosition.x;
		baseMtx[1][3]   = mPosition.y;
		baseMtx[2][3]   = mPosition.z;
		unk104->getModel()->setBaseTRMtx(baseMtx);
		emitPartsTrackEffects();
	}
}

void TTinKoopaPartsBase::emitPartsTrackEffects()
{
	if (!unk104)
		return;

	if (mIndex == 1)
		emitPartsTrackEffects(breastTrackJointNameTable, 6);
	else if (mIndex == 2)
		emitPartsTrackEffects(bellyTrackJointNameTable, 6);
	else if (mIndex == 3)
		emitPartsTrackEffects(rightArmTrackJointNameTable, 4);
	else if (mIndex == 4)
		emitPartsTrackEffects(leftArmTrackJointNameTable, 4);
}

void TTinKoopaPartsBase::emitPartsTrackEffects(const char** joint_names,
                                               int num)
{
	unk104->getModel()->calc();
	JUTNameTab* names = unk104->getModel()->getModelData()->getJointName();
	for (int i = 0; i < num; ++i) {
		int index = names->getIndex(joint_names[i]);
		if (index < 0)
			break;

		MtxPtr mtx = unk104->getModel()->getAnmMtx(index);
		unk108[i].set(mtx[0][3], mtx[1][3], mtx[2][3]);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_PARTS, &unk108[i], 0, mOwner);
	}
}

void TTinKoopaPartsBase::emitPartsDisappearEffects()
{
	if (!unk104)
		return;

	if (!unk104->checkCurBckFromIndex(
	        TTinKoopa_getBreakingAnimationIndex(mIndex)))
		return;

	if (!unk104->getFrameCtrl(0)->checkPass(60))
		return;

	if (mIndex == 1)
		emitPartsDisappearEffects(breastTrackJointNameTable, 6, 4.0f);
	else if (mIndex == 2)
		emitPartsDisappearEffects(bellyTrackJointNameTable, 6, 4.0f);
	else if (mIndex == 3)
		emitPartsDisappearEffects(rightArmTrackJointNameTable, 4, 3.0f);
	else if (mIndex == 4)
		emitPartsDisappearEffects(leftArmTrackJointNameTable, 4, 3.0f);

	mOwner->unk1E4 = nullptr;
}

void TTinKoopaPartsBase::emitPartsDisappearEffects(const char** joint_names,
                                                   int num, f32 scale)
{
	JGeometry::TVec3<f32> scaling = mScaling;
	scaling.scale(scale);

	JUTNameTab* names = unk104->getModel()->getModelData()->getJointName();
	for (int i = 0; i < num; ++i) {
		int index = names->getIndex(joint_names[i]);
		if (index < 0)
			break;

		MtxPtr mtx = unk104->getModel()->getAnmMtx(index);
		unk108[i].set(mtx[0][3], mtx[1][3], mtx[2][3]);

		TEffectExplosion* explosion
		    = (TEffectExplosion*)gpConductor->makeOneEnemyAppear(
		        unk108[i], "エフェクト爆発マネージャー", 1);
		if (!explosion)
			break;

		explosion->generate(unk108[i], scaling);
	}
}

BOOL TTinKoopaPartsBase::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == ACTOR_TYPE_ROCKET) {
		mOwner->hitParts();
		return true;
	}
	return false;
}

void TTinKoopaPartsBase::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TLiveActor::perform(cue, graphics);

	if (cue & CUE_MOVE)
		mCollision->moveMtx(
		    mOwner->getModel()->getAnmMtx(TTinKoopa_getJointIndex(mIndex)));

	if ((cue & CUE_CALC_ANIM) && mIsBreaking) {
		if (unk104 && unk104->curAnmEndsNext(0, nullptr))
			mIsBreaking = false;
	}

	if (mIsBreaking && unk104)
		unk104->perform(cue, graphics);
}

TTinKoopaMtxCalc::TTinKoopaMtxCalc(TTinKoopa* owner) { }

void TTinKoopaMtxCalc::joinAnm(int) { }

void TTinKoopaMtxCalc::calc(u16 param_1)
{
	M3UMtxCalcSIAnmBlendQuat::calc(param_1);
}

TTinKoopa::TTinKoopa(const char* name)
    : TSpineEnemy(name)
    , unk1F0(nullptr)
    , unk1F8(0)
{
	onLiveFlag(LIVE_FLAG_UNK10);
	offLiveFlag(LIVE_FLAG_ENABLE_CLIPPING);
	mScaledBodyRadius = 2000.0f;
}

void TTinKoopa::makeCoasterDistanceTable()
{
	unk1E8 = new f32[unk1EC->getNodeNum() - 1];

	JGeometry::TVec3<f32> point = unk1EC->indexToPoint(0);
	f32 total                   = 0.0f;
	for (int i = 0; i < unk1EC->getNodeNum() - 1; ++i) {
		JGeometry::TVec3<f32> prev = point;
		point                      = unk1EC->indexToPoint(i + 1);
		unk1E8[i]                  = prev.distance(point);
		total += unk1E8[i];
	}
}

f32 TTinKoopa::calcCoasterDistance(int from, int to)
{
	f32 distance = 0.0f;
	for (int i = from; i < to; ++i)
		distance += unk1E8[i];
	return distance;
}

f32 TTinKoopa::calcCoasterDistanceInOrder(int from, int to)
{
	if (to >= from)
		return calcCoasterDistance(from, to);

	return calcCoasterDistance(0, to)
	       + calcCoasterDistance(from, unk1EC->getNodeNum() - 1);
}

bool TTinKoopa::checkKillerApproachingFromBack(TCoasterKiller* killer,
                                               JGeometry::TVec3<f32> pos,
                                               f32 distance)
{
	if (killer->checkLiveFlag(LIVE_FLAG_DEAD))
		return false;

	if (killer->getPathDir() != 0)
		return false;

	int index = unk1EC->findNearestNodeIndex(pos, -1);
	return calcCoasterDistanceInOrder(killer->getPathIdx(), index) <= distance;
}

void TTinKoopa::reset()
{
	TSpineEnemy::reset();
	mFlame->resetTinKoopaFlame();
	for (int i = 0; i < 6; ++i)
		mParts[i]->reset();
	makeHitCollision();
	resetTinKoopa();
	changeBck(TTinKoopa_getWaitAnimationIndex(mBreakCount));
}

void TTinKoopa::resetTinKoopa()
{
	if (!unk1F0)
		unk1F0 = (TCoasterKillerManager*)JDrama::TNameRefGen::search(
		    "コースターキラーマネージャー");
	unk1F0->getActiveObjNum();

	mTruckActor               = gpMarioOriginal->mKoopaRail;
	mBreakCount               = 0;
	unk154                    = 0;
	unk158                    = 0;
	mLap                      = 0;
	mFirstFlameMessageShown   = false;
	mPartsHp                  = getSaveParam2()->mSLPartsHP.get();
	unk1E4                    = nullptr;
	mQueuedKillerNum          = 0;
	mQueuedKillersLaunchedNum = 0;
	for (int i = 0; i < 4; ++i)
		mQueuedKillerDirs[i] = 0;
	for (int i = 0; i < 3; ++i)
		mTimers[i] = 0;
	mTimers[TIMER_KILLER_INTERVAL] = 0;
	unk1B4                         = 0.0f;
	unk1B8                         = 30.0f;
	unk1BC                         = 15.0f;
	unk1C0                         = 45.0f;
}

void TTinKoopa::makeHitCollision()
{
	if (mBreakCount == 0) {
		setHitParams(0.0f, 0.0f, getSaveParam2()->mSLDamageRadius.get(),
		             getSaveParam2()->mSLDamageHeight0.get());
	} else if (mBreakCount == 1) {
		setHitParams(0.0f, 0.0f, getSaveParam2()->mSLDamageRadius.get(),
		             getSaveParam2()->mSLDamageHeight1.get());
	}
	mFlame->makeHitCollision();
}

void TTinKoopa::makeKillerQueue(int num, s8 dir)
{
	if (num > 4)
		num = 4;
	mQueuedKillersLaunchedNum = 0;
	mQueuedKillerNum          = num;
	for (int i = 0; i < mQueuedKillerNum; ++i)
		mQueuedKillerDirs[i] = dir;
	mTimers[TIMER_KILLER_INTERVAL] = 0;
}

void TTinKoopa::checkLap()
{
	if (!mTruckActor)
		return;

	J3DFrameCtrl* ctrl = mTruckActor->getFrameCtrl(0);
	if (ctrl->checkPass(ctrl->getEnd() - 1)) {
		++mLap;
		if (mLap > 2)
			mLap = 0;
	}
}

bool TTinKoopa::checkTruckAnimationPass(int frame)
{
	if (!mTruckActor)
		return false;

	if (mTruckActor->getFrameCtrl(0)->checkPass(frame))
		return true;

	return false;
}

static void printTinKoopaDebugInfo(TTinKoopa*) { }

void TTinKoopa::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		emitTinKoopaEffects();
		checkKillerLaunch();
	}

	if (cue & CUE_MOVE) {
		updateTimers();
		checkLap();
		mLaunchSchedule->checkOrder();
		checkTinKoopaMessage();
	}

	TSpineEnemy::perform(cue, graphics);
	mFlame->perform(cue, graphics);
	for (int i = 0; i < 6; ++i)
		mParts[i]->perform(cue, graphics);
}

void TTinKoopa::makeEyeBeamEffect() { }

void TTinKoopa::updateTimers()
{
	for (int i = 0; i < 3; ++i)
		if (mTimers[i] > 0)
			--mTimers[i];
}

const char** TTinKoopa::getBasNameTable() const { return tinkoopa_bastable; }

void TTinKoopa::changeBck(int index)
{
	mMActor->setBckFromIndex(index);
	setAnmSound(getBas(index));
}

BOOL TTinKoopa::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == ACTOR_TYPE_ROCKET) {
		hitParts();
		return true;
	}
	return false;
}

void TTinKoopa::hitParts()
{
	// TODO: uuuh, make this not inline properly somehow.
	(void)0;
	(void)0;
	(void)0;
	if (mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		return;

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve())
		return;

	if (mBreakCount == 4)
		return;

	startTinKoopaMessage(0xE0024);
	--mPartsHp;
	if (mPartsHp <= 0)
		mSpine->pushNerve(&TNerveTinKoopaBreak::theNerve());
	else
		mSpine->pushNerve(&TNerveTinKoopaDamage::theNerve());
}

void TTinKoopa::startBreakingParts()
{
	unk1E4 = mParts[TTinKoopa_getBreakingPartsIndex(mBreakCount)];
	unk1E4->startBreaking();
}

void TTinKoopa::launchKiller(int dir)
{
	TCoasterKiller* killer = (TCoasterKiller*)unk1F0->getDeadEnemy();
	if (!killer)
		return;

	killer->reset();

	int joint
	    = TTinKoopa_getJointIndex(dir == 1 ? mQueuedKillersLaunchedNum + 11
	                                       : 14 - mQueuedKillersLaunchedNum);

	getJointTransByIndex(joint, &killer->mPosition);
	killer->mPathDir = dir;
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_KILLER, getModel()->getAnmMtx(joint), 0,
	    this);
	SMSGetMSound()->startSoundActor(MSD_SE_EN_KILLER_FIRE, &killer->mPosition,
	                                0, nullptr, 0, 4);
}

void TTinKoopa::checkKillerLaunch()
{
	if (mSpine->getCurrentNerve() != &TNerveTinKoopaWait::theNerve())
		return;

	if (mQueuedKillersLaunchedNum < mQueuedKillerNum
	    && mTimers[TIMER_KILLER_INTERVAL] <= 0) {
		launchKiller(mQueuedKillerDirs[mQueuedKillersLaunchedNum]);
		++mQueuedKillersLaunchedNum;
		mTimers[TIMER_KILLER_INTERVAL]
		    = getSaveParam2()->mSLKillerInterval.get();
	}
}

void TTinKoopa::checkTinKoopaMessage()
{
	checkTinKoopaKillerApproachingMessage();
	checkTinKoopaFirstRocketMessage();
	checkTinKoopaFirstFlameMessage();
}

void TTinKoopa::checkTinKoopaKillerApproachingMessage()
{
	u32 msgId = 0xE0009;
	for (int i = 0; i < unk1F0->getActiveObjNum(); ++i) {
		TCoasterKiller* killer = (TCoasterKiller*)unk1F0->getObj(i);
		if (killer->checkLiveFlag(LIVE_FLAG_DEAD))
			continue;

		if (checkKillerApproachingFromBack(
		        killer, *gpMarioPos,
		        getSaveParam2()->mSLKillerApproachingDistance.get()))
			startTinKoopaMessage(msgId);
	}
}

void TTinKoopa::checkTinKoopaFirstRocketMessage()
{
	if (mTruckActor) {
		J3DFrameCtrl* ctrl = mTruckActor->getFrameCtrl(0);
		if (mLap == 0 && ctrl->checkPass(300.0f))
			startTinKoopaMessage(0xE000A);
	}
}

void TTinKoopa::checkTinKoopaFirstFlameMessage()
{
	if (!mTruckActor)
		return;

	if (mFirstFlameMessageShown)
		return;

	if (mSpine->getCurrentNerve() != &TNerveTinKoopaWait::theNerve())
		return;

	J3DFrameCtrl* ctrl = mTruckActor->getFrameCtrl(0);
	if (mBreakCount == 0) {
		if (ctrl->checkPass(2600.0f)) {
			startTinKoopaMessage(0xE000B);
			mFirstFlameMessageShown = true;
		}
	} else {
		if (ctrl->checkPass(3100.0f)) {
			startTinKoopaMessage(0xE000B);
			mFirstFlameMessageShown = true;
		}
	}
}

void TTinKoopa::startTinKoopaMessage(u32 message)
{
	SMSGetMarDirector()->getConsole()->startAppearBalloon(message, true);
}

void TTinKoopa::emitTinKoopaEffects()
{
	MtxPtr mtx = getModel()->getAnmMtx(TTinKoopa_getJointIndex(0));
	unk184.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	mtx = getModel()->getAnmMtx(TTinKoopa_getJointIndex(1));
	unk190.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	mtx = getModel()->getAnmMtx(TTinKoopa_getJointIndex(3));
	unk19C.set(mtx[0][3], mtx[1][3], mtx[2][3]);
	mtx = getModel()->getAnmMtx(TTinKoopa_getJointIndex(4));
	unk1A8.set(mtx[0][3], mtx[1][3], mtx[2][3]);

	gpMarioParticleManager->emitAndBindToPosPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_HIBANA_W1BR, &unk190, 1, this);

	if (mBreakCount > 1)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_HIBANA_W3AR,
		    getModel()->getAnmMtx(TTinKoopa_getJointIndex(3)), 1, this);

	if (mBreakCount > 2)
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_HIBANA_W4AR,
		    getModel()->getAnmMtx(TTinKoopa_getJointIndex(4)), 1, this);

	if (mBreakCount <= 0
	    && mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve())
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_W1ST,
		    getModel()->getAnmMtx(TTinKoopa_getJointIndex(2)), 1, this);

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_W1AR,
	    getModel()->getAnmMtx(TTinKoopa_getJointIndex(3)), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_W1AR,
	    getModel()->getAnmMtx(TTinKoopa_getJointIndex(4)), 1, this + 1);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_W1FE,
	    getModel()->getAnmMtx(TTinKoopa_getJointIndex(10)), 1, this);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_W1HE,
	    getModel()->getAnmMtx(TTinKoopa_getJointIndex(0)), 1, this);

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()) {
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_D1BR_A, &unk190, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_D1BR_A, &unk190, 1, this + 1);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_D1BR_B, &unk190, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_BIRI_D1BR_B, &unk190, 1, this + 1);
	}

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mBreakCount > 1)
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_B1AR, &unk19C, 1, this);

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mBreakCount > 2)
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_B1AR, &unk1A8, 1, this + 1);

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mBreakCount > 0)
	    || (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	        && mBreakCount > 0)
	    || (mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()
	        && (mBreakCount == 1 || mBreakCount == 2))) {
		MtxPtr breastMtx = getModel()->getAnmMtx(TTinKoopa_getJointIndex(1));
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_W2BR_A, breastMtx, 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_W2BR_B, breastMtx, 1, this);
	}

	if ((mSpine->getCurrentNerve() == &TNerveTinKoopaWait::theNerve()
	     && mBreakCount > 2)
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve())
		gpMarioParticleManager->emitAndBindToPosPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_B1HE, &unk184, 1, this);

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaDamage::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()) {
		MtxPtr femurMtx = getModel()->getAnmMtx(TTinKoopa_getJointIndex(10));
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_B1FE_R, femurMtx, 1, this);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_KEMU_B1FE_L, femurMtx, 1, this);
	}

	if (mSpine->getCurrentNerve() == &TNerveTinKoopaBreak::theNerve()) {
		if ((mBreakCount == 0 || mBreakCount == 3)
		    && mMActor->getFrameCtrl(0)->checkPass(100.0f))
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);

		if ((mBreakCount == 1 || mBreakCount == 2)
		    && mMActor->getFrameCtrl(0)->checkPass(104.0f))
			gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);

		if ((mBreakCount == 0 || mBreakCount == 3)
		    && mMActor->getFrameCtrl(0)->checkPass(108.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_TINKOOPA_JPA_MS_MKP_SMOKE1,
			    getModel()->getAnmMtx(TTinKoopa_getJointIndex(2)), 0, this);

		if (mBreakCount == 0 && mMActor->getFrameCtrl(0)->checkPass(100.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_TINKOOPA_JPA_MS_MKP_PARGE_B14,
			    getModel()->getAnmMtx(TTinKoopa_getJointIndex(2)), 0, this);

		if (mBreakCount == 3 && mMActor->getFrameCtrl(0)->checkPass(100.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_TINKOOPA_JPA_MS_MKP_PARGE_B14,
			    getModel()->getAnmMtx(TTinKoopa_getJointIndex(1)), 0, this);

		if (mBreakCount == 1 && mMActor->getFrameCtrl(0)->checkPass(106.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_TINKOOPA_JPA_MS_MKP_PARGE_B23,
			    getModel()->getAnmMtx(TTinKoopa_getJointIndex(3)), 0, this);

		if (mBreakCount == 2 && mMActor->getFrameCtrl(0)->checkPass(106.0f))
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    SCENE_TINKOOPA_JPA_MS_MKP_PARGE_B23,
			    getModel()->getAnmMtx(TTinKoopa_getJointIndex(4)), 0, this);
	}

	if (unk1E4)
		unk1E4->emitPartsDisappearEffects();
}

TTinKoopaManager::TTinKoopaManager(const char* name)
    : TEnemyManager(name)
{
}

void TTinKoopaManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "tinkoopa_body.bmd",
		  J3DMLF_MaterialPEFull | J3DMLF_MaterialTexGenFull
		      | J3DMLF_UseUniqueMaterials | (4 << J3DMLF_TevStageNumShift),
		  0 },
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
	static const char* onetimeFilenames[] = {
		"/scene/tinkoopa/jpa/ms_mkp_hibana_d1he.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_killer.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_smoke1.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_parge_b14.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_parge_b23.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_flame_yuge.jpa",
		"/scene/tinkoopa/jpa/ms_mkp_kemu_parts.jpa",
	};
	for (int i = 0; i < 7; ++i)
		SMS_LoadParticle(onetimeFilenames[i], 0xEE + i);

	static const char* loopFilenames[] = {
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
	for (int i = 0; i < 17; ++i)
		SMS_LoadParticle(loopFilenames[i], 0x1AC + i);

	static const char* loopIndirectFilenames[1] = {
		"/scene/tinkoopa/jpa/ms_mkp_fire_c.jpa",
	};
	for (int i = 0; i < 1; ++i)
		SMS_LoadParticle(loopIndirectFilenames[i], 0x1F2 + i);
}

TSpineEnemy* TTinKoopaManager::createEnemyInstance() { return nullptr; }

DEFINE_NERVE(TNerveTinKoopaWait, TLiveActor)
{
	TTinKoopa* self = (TTinKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(TTinKoopa_getWaitAnimationIndex(self->mBreakCount));
		self->mTimers[TTinKoopa::TIMER_DEFEAT_WAIT]
		    = self->getSaveParam2()->mSLDefeatWaitTime.get();
	}

	if (self->mBreakCount == 4
	    && self->mTimers[TTinKoopa::TIMER_DEFEAT_WAIT] <= 0)
		TFlagManager::getInstance()->setBool(true, MSF_MECHA_BOWSER_DEFEATED);

	return false;
}

DEFINE_NERVE(TNerveTinKoopaDamage, TLiveActor)
{
	TTinKoopa* self = (TTinKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(TTinKoopa_getDamageAnimationIndex(self->mBreakCount));
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_HIBANA_D1HE,
		    self->getModel()->getAnmMtx(TTinKoopa_getJointIndex(0)), 0, this);
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);
	}

	if (self->getMActor()->checkCurBckFromIndex(
	        TTinKoopa_getDamageAnimationIndex(self->mBreakCount))
	    && self->getMActor()->curAnmEndsNext(0, nullptr)) {
		self->changeBck(TTinKoopa_getWaitAnimationIndex(self->mBreakCount));
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveTinKoopaBreak, TLiveActor)
{
	TTinKoopa* self = (TTinKoopa*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(TTinKoopa_getBreakAnimationIndex(self->mBreakCount));
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    SCENE_TINKOOPA_JPA_MS_MKP_HIBANA_D1HE,
		    self->getModel()->getAnmMtx(TTinKoopa_getJointIndex(0)), 0, this);
		gpCameraShake->startShake(CAM_SHAKE_MODE_UNK6, 1.0f);
		if (self->mBreakCount == 3) {
			self->unk1F0->unk60 = 1;
			MSBgm::stopTrackBGMs(7, 10);
		}
	}

	if (self->getMActor()->checkCurBckFromIndex(
	        TTinKoopa_getBreakAnimationIndex(self->mBreakCount))) {
		TTinKoopaPartsBase* parts
		    = self->mParts[TTinKoopa_getBreakingPartsIndex(self->mBreakCount)];

		if (self->getMActor()->curAnmEndsNext(0, nullptr)) {
			parts->mCollision->remove();
			++self->mBreakCount;
			self->makeHitCollision();
			self->changeBck(TTinKoopa_getWaitAnimationIndex(self->mBreakCount));
			self->mPartsHp = self->getSaveParam2()->mSLPartsHP.get();
			return true;
		}

		if (!parts->mIsBreaking
		    && self->getMActor()->getFrameCtrl(0)->checkPass(
		        TTinKoopa_getPartsVisibleFrame(self->mBreakCount)))
			self->startBreakingParts();
	}

	return false;
}
