#include <Enemy/Igaiga.hpp>

#include <Enemy/Conductor.hpp>

#include <Enemy/Graph.hpp>

#include <Strategic/LiveActor.hpp>

#include <Strategic/Spine.hpp>

#include <Strategic/ObjModel.hpp>

#include <Strategic/SharedParts.hpp>

#include <M3DUtil/MActor.hpp>

#include <M3DUtil/SDLModel.hpp>

#include <MarioUtil/MathUtil.hpp>

#include <MarioUtil/RandomUtil.hpp>

#include <Player/MarioAccess.hpp>

#include <Player/ModelWaterManager.hpp>

#include <Map/Map.hpp>

#include <Map/MapData.hpp>

#include <JSystem/JKernel/JKRFileLoader.hpp>

#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>

#include <JSystem/JDrama/JDRNameRefGen.hpp>

#include <Map/MapEventSink.hpp>

#include <Enemy/AreaCylinder.hpp>

#include <Map/PollutionManager.hpp>

#include <MarioUtil/RumbleMgr.hpp>

#include <MarioUtil/TexUtil.hpp>

#include <MarioUtil/PacketUtil.hpp>

#include <MoveBG/ItemManager.hpp>

#include <MoveBG/MapObjBianco.hpp>

#include <Map/MapCollisionData.hpp>

#include <Map/MapMirror.hpp>

#include <Strategic/MirrorActor.hpp>

#include <System/Particles.hpp>

#include <JSystem/JParticle/JPAEmitter.hpp>

#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

#include <MSound/MSound.hpp>

#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>

#include <MSound/MSSetSound.hpp>

#include <MSound/MSoundBGM.hpp>

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

static const char* gorogoro_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr,
};

f32 TRollEnemy::mBoundVal     = 80.0f;

f32 TRollEnemy::mTransYOffset;

f32 TIgaiga::mReachNodeDist   = 300.0f;

// UNUSED in the map; the values are not recoverable from the binary.
f32 TIgaiga::mTremblePow      = 1.0f;

f32 TIgaiga::mTrembleAcc      = 1.0f;

f32 TIgaiga::mTrembleBrk      = 1.0f;

static TRollEnemy* gpCurRollEnemy;

static int RollEnemyBodyCallback(J3DNode* node, int param);

// UNUSED, 0x160 in the map: only ever constructed inline by the managers.
TRollEnemySaveLoadParams::TRollEnemySaveLoadParams(const char* prm)
    : TWalkerEnemyParams(prm)
    , PARAM_INIT(mSLGenerateInterval, 300)
    , PARAM_INIT(mSLExpandRate, 1.0f)
    , PARAM_INIT(mSLExpandMax, 1.5f)
    , PARAM_INIT(mSLBoundVYMax, 15.0f)
    , PARAM_INIT(mSLGroundOffsetY, 150.0f)
{
	TParams::load(mPrmPath);
}

// UNUSED, 0xf8 in the map: the rolling-along-the-graph step that the three
// igaiga nerves share.
void TIgaiga::rollMove()
{
	if (isReachedToGoalXZ()) {
		if (jumpToNextGraphNode() >= 0)
			flagJump();
		if (!(unk124->getCurrent().getRailNode()->mFlags & 0x40))
			goToRandomNextGraphNode();
	} else {
		walkBehavior(2, 1.0f);
	}
}

// UNUSED, 0x48 in the map: pop after too many sprays.
void TIgaiga::waterExplosion()
{
	onLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
	mSpine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
}

// UNUSED, 0x94 in the map: only ever constructed inline by the manager.
TGorogoro::TGorogoro(const char* name)
    : TRollEnemy(name)
{
	unk1E4            = 0;
	mGenerateGraphIdx = 0;
	unk1EC            = 0;
	gpCurRollEnemy    = nullptr;
}

// TODO: 36%. The original carries TRollEnemy::behaveToWater's whole body
// inline here (that function itself matches to 99.8%) where our build
// calls it. Either MWCC inlined the call or the original pasted the body;

// UNUSED, 0x1e8 in the map.
// TODO: only the field it sets is known; the body is a guess.
void TGorogoro::setGenerateGraphIdx(int idx) { mGenerateGraphIdx = idx; }

// UNUSED, 0x24 in the map.
void TIgaigaManager::requestPolluteModel(JGeometry::TVec3<f32>& pos,
                                         JGeometry::TVec3<f32>& scale)
{
	unk60->generatePolluteModel(pos, scale);
}

// UNUSED, 0x24 in the map.
void TGorogoroManager::requestPolluteModel(JGeometry::TVec3<f32>& pos,
                                           JGeometry::TVec3<f32>& scale)
{
	unk6C->generatePolluteModel(pos, scale);
}

// UNUSED, 0x34 in the map.
bool TGorogoroManager::inArea(const JGeometry::TVec3<f32>& pos)
{
	return unk70 ? unk70->contain(pos) : true;
}

TRollEnemy::TRollEnemy(const char* name)
    : TWalkerEnemy(name)
{
	mRollAngle = 0.0f;
	unk198     = 0.0f;
	unk19C     = 0.0f;
	unk1A0     = 0.0f;
	unk1A4     = nullptr;
	unk1A8     = 0;
	unk1AC     = 0.0f;
	unk1B0     = 1.0f;
}

void TRollEnemy::reset()
{
	gpCurRollEnemy = this;
	TWalkerEnemy::reset();

	TMsRange<f32> angle(0.0f, 360.0f);
	mRollAngle = angle.rand();
	unk158     = 1.0f;

	// Start on the first graph node, 10 up, facing the second.
	JGeometry::TVec3<f32> point;
	unk124->unk0->unk0[0].getPoint((Vec*)&point);
	mPosition = point;
	mPosition.y += 10.0f;

	unk124->unk0->unk0[1].getPoint((Vec*)&point);
	JGeometry::TVec3<f32> dir(point.x - mPosition.x, 0.0f,
	                          point.z - mPosition.z);
	mRotation.y = MsWrap(MsGetRotFromZaxisY(dir), 0.0f, 360.0f);

	unk198          = 1.5f * mMarchSpeed;
	unk19C          = mMarchSpeed;
	unk1A0          = 0.0f;
	unk124->mCurrIdx = 0;
}

void TRollEnemy::walkBehavior(int param_1, f32 param_2)
{
	if (!unk1A8)
		TWalkerEnemy::walkBehavior(param_1, param_2);

	if (isAirborne() && mPosition.y > 20.0f + mGroundHeight) {
		// Remember the highest point of the arc, in bounce units.
		f32 height = MsWrap((mPosition.y - mGroundHeight) / mBoundVal, 0.0f,
		                    unk1A4->mSLBoundVYMax.get());
		if (unk1A0 < height)
			unk1A0 = height;
	} else if (!mGroundPlane->isWaterSurface()) {
		unk1A8 = 0;
		if (unk1A0 > unk1B0) {
			// Landed hard enough to bounce.
			bound();
			JGeometry::TVec3<f32> vel(0.0f, unk1A0, 0.0f);
			mVelocity = vel;
			onLiveFlag(LIVE_FLAG_AIRBORNE);
			mPosition.y += 5.0f;
			unk1A0 = 0.0f;
			boundSE();
		}
	}

	if (mPosition.y < 30.0f + mGroundHeight)
		rollSE();

	if (unk128 > 300) {
		onLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		kill();
	}
}

void TRollEnemy::behaveToWater(THitActor* param_1)
{
	mSprayedByWaterCooldown = 0;

	// Each spray swells the body, and the hit radii with it.
	if (unk158 < unk1A4->mSLExpandMax.get()) {
		f32 rate = unk1A4->mSLExpandRate.get();
		mBodyScale *= rate;
		unk158 *= rate;
		mScaledBodyRadius *= rate;
		mScaling.x = mScaling.y = mScaling.z = mScaling.z * rate;

		f32 attackRadius = getSaveParams()->getSLAttackRadius();
		f32 attackHeight = getSaveParams()->getSLAttackHeight();
		f32 damageRadius = getSaveParams()->getSLDamageRadius();
		f32 damageHeight = getSaveParams()->getSLDamageHeight();
		f32 ratio        = mBodyScale / unk154;
		mAttackRadius    = attackRadius * ratio;
		mAttackHeight    = attackHeight * ratio;
		mDamageRadius    = damageRadius * ratio;
		mDamageHeight    = damageHeight * ratio;
		calcEntryRadius();
	}
}

void TRollEnemy::attackToMario() { SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK); }

void TRollEnemy::flagJump()
{
	// Hop toward the current graph node, 30 up, at the tracer's speed.
	JGeometry::TVec3<f32> target;
	unk124->getCurrent().getPoint((Vec*)&target);
	mPosition.y += 30.0f;

	f32 speed = unk124->unkC;
	JGeometry::TVec3<f32> vel
	    = calcVelocityToJumpToY(target, speed, getGravityY());
	unk1A8    = 1;
	mVelocity = vel;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

bool TRollEnemy::isCollidMove(THitActor* param_1)
{
	// Running into a cannon shell kills it; anything else it just hits.
	if (param_1->isActorType(0x4000022B)) {
		kill();
		return true;
	}
	param_1->receiveMessage(this, HIT_MESSAGE_ATTACK);
	return false;
}

bool TRollEnemy::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> d(unk104.getPoint());
	d.x -= mPosition.x;
	d.y -= mPosition.y;
	d.z -= mPosition.z;
	if (!unk1A8)
		d.y = 0.0f;
	if (MsVECMag2(d) < 200.0f)
		return true;
	return false;
}

void TRollEnemy::setBehavior()
{
	if (getPosition().y > 50.0f + mGroundHeight)
		return;

	if (mSpine->getTime() % getSaveParams()->mSLPolluteInterval.get() != 0)
		return;
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return;
	if (mSpine->getCurrentNerve() == &TNerveSmallEnemyDie::theNerve())
		return;
	if (!TSmallEnemy::mIsPolluter)
		return;

	// Stamp goop just ahead of the roll; the amplitude version pulses the
	// stamp radius with the body scale.
	f32 range = 2.0f;
	if (!checkLiveFlag(LIVE_FLAG_HIDDEN)) {
		if (!TSmallEnemy::mIsAmpPolluter) {
			range = getSaveParams()->mSLPolluteRange.get();
		} else {
			s32 rmin  = getSaveParams()->mSLPolluteRMin.get();
			s32 rmax  = getSaveParams()->mSLPolluteRMax.get();
			s32 cycle = getSaveParams()->mSLPolluteCycle.get();
			range     = rmin
			    + mBodyScale
			        * (JMASin(180.0f * (mSpine->getTime() % cycle) / cycle)
			           * (rmax - rmin));
		}
	}

	SMSGetPollution()->stampGround(
	    1, unk1AC * mLinearVelocity.x + mPosition.x, getPosition().y,
	    unk1AC * mLinearVelocity.z + mPosition.z, 32.0f * range);
}

void TIgaigaPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/igaiga/stamp_igaiga_model1.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (2 << J3DMLF_TevStageNumShift)));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TIgaigaPolluteModel(param_1, modelData);
}

// Binding level over a raw member read, worth +16 of low region in
// TIgaigaPolluteModel::setAnm (batch 127).
static inline TSharedParts* IgaigaUnk10(const TIgaigaPolluteModel* p)
{
	TSharedParts* v10 = p->unk10;
	return v10;
}

void TIgaigaPolluteModel::setAnm()
{
	IgaigaUnk10(this)->unk18->setBckFromIndex(7);
	IgaigaUnk10(this)->unk18->getFrameCtrl(0)->setFrame(0.0f);
}

TIgaigaManager::TIgaigaManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk64          = 0;
	unk68          = nullptr;
	gpCurRollEnemy = nullptr;
}

void TIgaigaManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TRollEnemySaveLoadParams("/enemy/igaiga.prm");
	unk68 = new TWaterEmitInfo("/enemy/igaigawater.prm");
}

void TIgaigaManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "igaiga_model1.bmd", 0x11240000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

TSpineEnemy* TIgaigaManager::createEnemyInstance()
{
	return new TIgaiga("イガイガ");
}

void TIgaigaManager::initSetEnemies()
{
	unk60 = new TIgaigaPolluteModelManager;
	unk60->init((TLiveActor*)unk18[0]);
}

void TIgaigaManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TEnemyManager::perform(cue, graphics);
	unk60->perform(cue, graphics);
}

// Rolls the body joint about Z by the accumulated roll angle, and lifts it
// by mTransYOffset.
// TODO: 82%. The original keeps the roll matrix's address in a callee-saved
// register across both concats; ours recomputes it from the stack pointer.
static int RollEnemyBodyCallback(J3DNode* node, int param)
{
	if (param == 0) {
		if (gpCurRollEnemy == nullptr || !gpCurRollEnemy->isRolling())
			return true;

		J3DJoint* joint = (J3DJoint*)node;
		MtxPtr anmMtx
		    = gpCurRollEnemy->getModel()->getAnmMtx(joint->getJntNo());

		s16 angle = DEG2SHORTANGLE(gpCurRollEnemy->mRollAngle);
		f32 s     = JMASSin(angle);
		f32 c     = JMASCos(angle);

		Mtx roll;
		roll[0][0] = 1.0f;
		roll[0][1] = 0.0f;
		roll[0][2] = 0.0f;
		roll[0][3] = 0.0f;

		roll[1][0] = 0.0f;
		roll[1][1] = c;
		roll[1][2] = -s;
		roll[1][3] = 0.0f;

		roll[2][0] = 0.0f;
		roll[2][1] = s;
		roll[2][2] = c;
		roll[2][3] = 0.0f;

		anmMtx[1][3] += TRollEnemy::mTransYOffset;
		MTXConcat(anmMtx, roll, anmMtx);
		MTXConcat(J3DSys::mCurrentMtx, roll, J3DSys::mCurrentMtx);
	}
	return true;
}

TIgaiga::TIgaiga(const char* name)
    : TRollEnemy(name)
{
	unk1B4 = 0;
	unk1B8 = 0;
	unk1BC = 1;
	unk1CC = 1.0f;
	unk1D0 = 0;
	unk1E4 = 1.0f;
	unk1E8 = 0;
}

void TIgaiga::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000017;
	unk150     = 0x11;
	offHitFlag(HIT_FLAG_UNK8000000 | HIT_FLAG_UNK10000000);
	onHitFlag(HIT_FLAG_UNK40000000);
	mSpine->initWith(&TNerveIgaigaRollOnGraph::theNerve());
	unk1A4 = (TRollEnemySaveLoadParams*)getSaveParam();
	mMActor->setJointCallback(1, RollEnemyBodyCallback);
	mMActor->setBtkFromIndex(0);
	unk124->setGraph(gpConductor->getGraphByName("igaiga"));
}

void TIgaiga::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("igaiga_model1.bmd", 0);
}

void TIgaiga::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);
}

void TIgaiga::calcRootMatrix()
{
	gpCurRollEnemy = this;
	TSpineEnemy::calcRootMatrix();
}

bool TIgaiga::isRolling()
{
	if (mSpine->getCurrentNerve() == &TNerveIgaigaRollOnGraph::theNerve())
		return true;
	if (mSpine->getCurrentNerve() == &TNerveIgaigaShootFromCannon::theNerve())
		return true;
	if (mSpine->getCurrentNerve() == &TNerveIgaigaWaterHit::theNerve())
		return true;
	return false;
}

void TIgaiga::behaveToWater(THitActor* param_1)
{
	mSprayedByWaterCooldown = 0;

	// Every hit swells it by mSLExpandRate, up to mSLExpandMax.
	if (unk1E4 < unk1A4->mSLExpandMax.get())
		unk1E4 *= unk1A4->mSLExpandRate.get();

	unk165 = true;
	if (getSpine()->getCurrentNerve() != &TNerveIgaigaWaterHit::theNerve())
		mSpine->pushNerve(&TNerveIgaigaWaterHit::theNerve());
}

void TIgaiga::reset()
{
	TRollEnemy::reset();
	initialGraphNode();
	offLiveFlag(LIVE_FLAG_UNK10);

	// Tremble timer: fires every 50..100 seconds.
	unk1B4 = 0;
	TMsRange<int> seconds(50, 100);
	unk1B8 = seconds.rand() * 120;
	unk1BC = 1;

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
	mRollAngle = 0.0f;
	TSmallEnemy::kill();
}

void TIgaiga::moveObject()
{
	TWalkerEnemy::moveObject();

	// It slowly deflates back toward half size after being swollen.
	unk1CC = MsClamp(unk1CC - 0.0002f, 0.5f, 1.0f);

	f32 attackRadius = getSaveParams()->getSLAttackRadius();
	f32 attackHeight = getSaveParams()->getSLAttackHeight();
	f32 damageRadius = getSaveParams()->getSLDamageRadius();
	f32 damageHeight = getSaveParams()->getSLDamageHeight();

	f32 base   = unk154 * unk1CC;
	mBodyScale = MsClamp(unk1E4 * base, base, 3.0f * mBodyScale);

	f32 ratio         = mBodyScale / unk154;
	mScaledBodyRadius = 8.0f * (mBodyScale * mBodyRadius)
	    * MsClamp(unk1CC * unk1E4, 1.0f, 1.2f);
	mScaling.x = mScaling.y = mScaling.z = mBodyScale;
	mAttackRadius = attackRadius * ratio;
	mAttackHeight = attackHeight * ratio;
	mDamageRadius = damageRadius * ratio;
	mDamageHeight = damageHeight * ratio;
	calcEntryRadius();

	mMarchSpeed = unk1A4->mSLMarchSpeedLow.get();
	mTurnSpeed  = unk1A4->mSLTurnSpeedLow.get();

	// Fallen below the node it is heading for: dead, and not by Mario.
	JGeometry::TVec3<f32> node;
	unk124->getCurrent().getPoint((Vec*)&node);
	if (mPosition.y < 50.0f + node.y) {
		kill();
		unk1BC = 1;
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
	    abs(mGroundPlane->mNormal.y), 0, 0, nullptr, 0, 4);
}

void TIgaiga::walkBehavior(int param_1, f32 param_2)
{
	TRollEnemy::walkBehavior(param_1, param_2);

	// Spin the body by how far it moved this frame over its radius.
	f32 dx = mLinearVelocity.x;
	f32 dz = mLinearVelocity.z;
	if (unk1A8) {
		JGeometry::TVec3<f32> vel(mVelocity);
		dx = JGeometry::TVec3<f32>(vel).x;
		dz = JGeometry::TVec3<f32>(vel).z;
	}
	mRollAngle += 4.0f
	    * (JGeometry::TUtil<f32>::sqrt(dx * dx + dz * dz)
	       / (unk1E4 * (mBodyRadius * unk1CC)));

	if (unk1B4 != 0) {
		unk1B4++;
		if (unk1B4 > unk1B8)
			unk1B4 = 0;
	}

	// Whatever it is resting on or leaning against gets hit.
	if (!isAirborne()) {
		if (mGroundPlane && mGroundPlane->getActor())
			((THitActor*)mGroundPlane->getActor())
			    ->receiveMessage(this, HIT_MESSAGE_ATTACK);
	}
	if (unk138 && unk138->getActor())
		((THitActor*)unk138->getActor())->receiveMessage(this, HIT_MESSAGE_ATTACK);
}

bool TIgaiga::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> d(unk104.getPoint());
	d.x -= mPosition.x;
	d.y -= mPosition.y;
	d.z -= mPosition.z;
	if (!unk1A8)
		d.y = 0.0f;
	d.y = 0.0f;
	if (MsVECMag2(d) < mReachNodeDist)
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

	gpMarioParticleManager->emit(0xCB, &unk1C0, 0, nullptr);
	if (unk1BC)
		setBckAnm(0);
	else
		setBckAnm(1);

	// The goop it leaves is the body's scale, clamped.
	JGeometry::TVec3<f32> stamp(mScaling);
	stamp.scale(unk1CC * unk1E4);
	mPosition.y = mGroundHeight;
	stamp.x     = MsClamp(stamp.x, 0.8f, 1.5f);
	stamp.z     = stamp.x;
	stamp.y     = stamp.x;
	((TIgaigaManager*)mManager)->unk60->generatePolluteModel(mPosition, stamp);
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
			SMSRumbleMgr->start(0x15, 10, (Vec*)&mPosition);
		else
			SMSRumbleMgr->start(0x14, 10, (Vec*)&mPosition);
	}

	// Burst into water, then a puff of smoke.
	TWaterEmitInfo* info = ((TIgaigaManager*)mManager)->unk68;
	info->mPos.value     = mPosition;
	gpModelWaterManager->emitRequest(*info);

	// The assignment is the level that reaches the map's out-of-line
	// TVec3::scale: operator= is depth 1, operator* (nested in its
	// argument) 2, operator*= 3 and scale 4.
	JGeometry::TVec3<f32> scale;
	scale = mScaling * 0.5f;

	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emit(0xA1, &unk1C0, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(scale);
	emitter = gpMarioParticleManager->emit(0xA2, &unk1C0, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(scale);

	setBckAnm(5);

	// One time in five it leaves a coin behind.
	TMsRange<f32> chance(0.0f, 1.0f);
	if (chance.rand() < 0.2f) {
		gpItemManager->makeObjAppear(mPosition.x, 20.0f + mPosition.y,
		                             mPosition.z, 0x20000002, true);
	}
}

const char** TIgaiga::getBasNameTable() const { return igaiga_bastable; }

bool TIgaiga::isHitValid(u32 param_1)
{
	// A hip drop is the one hit that does not count as a proper kill.
	unk1BC = 1;
	if (param_1 == HIT_MESSAGE_HIP_DROP)
		unk1BC = 0;
	return true;
}

void TIgaiga::bound()
{
	if (unk1A0 > 5.0f) {
		setBckAnm(2);
		if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT) && SMS_IsMarioTouchGround4cm()) {
			if (mScaling.x > mBodyScale)
				SMSRumbleMgr->start(0x15, 10, (Vec*)&mPosition);
			else
				SMSRumbleMgr->start(0x14, 10, (Vec*)&mPosition);
		}
	}
}

void TIgaiga::shoot(JGeometry::TVec3<f32>& velocity)
{
	mSpine->setNext(&TNerveIgaigaShootFromCannon::theNerve());
	mShootVelocity = velocity;
	offLiveFlag(LIVE_FLAG_UNK10);
	unk1A8 = 1;
}

DEFINE_NERVE(TNerveIgaigaRollOnGraph, TLiveActor)
{
	TIgaiga* igaiga = (TIgaiga*)spine->getBody();

	if (spine->getTime() == 0)
		igaiga->setWalkAnm();

	if (igaiga->checkCurAnmEnd(0) && igaiga->isBckAnm(2))
		igaiga->setBckAnm(3);

	igaiga->rollMove();
	return FALSE;
}

DEFINE_NERVE(TNerveIgaigaWaterHit, TLiveActor)
{
	TIgaiga* igaiga = (TIgaiga*)spine->getBody();

	if (spine->getTime() == 0)
		igaiga->setBckAnm(6);

	// Fully swollen: twenty more hits and it bursts.
	if (igaiga->unk1E4 >= igaiga->unk1A4->mSLExpandMax.get()) {
		if (igaiga->unk1E8 > 20)
			igaiga->waterExplosion();
		igaiga->unk1E8++;
	}

	if (igaiga->checkCurAnmEnd(0)) {
		bool sprayed  = igaiga->unk165;
		igaiga->unk165 = false;
		if (!sprayed) {
			igaiga->setBckAnm(3);
			spine->pushAfterCurrent(&TNerveIgaigaRollOnGraph::theNerve());
			return TRUE;
		}
	}

	if (igaiga->checkCurAnmEnd(0) && igaiga->isBckAnm(2))
		igaiga->setBckAnm(3);

	igaiga->rollMove();
	return FALSE;
}

DEFINE_NERVE(TNerveIgaigaShootFromCannon, TLiveActor)
{
	TIgaiga* igaiga = (TIgaiga*)spine->getBody();

	if (spine->getTime() == 0) {
		igaiga->setBckAnm(4);
		igaiga->mPosition.y += 10.0f;
		igaiga->mVelocity = igaiga->mShootVelocity;
		igaiga->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (igaiga->checkCurAnmEnd(0) && !igaiga->isAirborne()) {
		igaiga->bound();
		spine->pushAfterCurrent(&TNerveIgaigaRollOnGraph::theNerve());
		return TRUE;
	}

	igaiga->walkBehavior(2, 1.0f);
	return FALSE;
}

void TGorogoroPolluteModelManager::init(TLiveActor* param_1)
{
	TEnemyPolluteModelManager::init(param_1);

	void* res = JKRFileLoader::getGlbResource(
	    "/scene/gorogoro/bosspaku_head_stamp.bmd");
	SDLModelData* modelData = new SDLModelData(J3DModelLoaderDataBase::load(
	    res, J3DMLF_MaterialPEFull | J3DMLF_UseUniqueMaterials
	             | (2 << J3DMLF_TevStageNumShift)));

	for (int i = 0; i < unk14; ++i)
		unk18[i] = new TGorogoroPolluteModel(param_1, modelData);
}

// Binding level over a raw member read, worth +16 of low region in
// TGorogoroPolluteModel::setAnm (batch 127).
static inline TSharedParts* IgaigaUnk10(const TGorogoroPolluteModel* p)
{
	TSharedParts* v10 = p->unk10;
	return v10;
}

void TGorogoroPolluteModel::setAnm()
{
	IgaigaUnk10(this)->unk18->setBckFromIndex(3);
	IgaigaUnk10(this)->unk18->getFrameCtrl(0)->setFrame(0.0f);
}

TGorogoroManager::TGorogoroManager(const char* name)
    : TSmallEnemyManager(name)
{
	unk60 = 0;
	unk64 = nullptr;
	unk68 = 1;
	unk6C = nullptr;
	unk70 = nullptr;
}

void TGorogoroManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TRollEnemySaveLoadParams("/enemy/gorogoro.prm");

	static const char* anmlist[] = { "bosspaku_head_move", nullptr };
	createSharedMActorSet(anmlist);
}

void TGorogoroManager::loadAfter()
{
	unk64 = JDrama::TNameRefGen::search<TMapEventSink>(
	    "イベント（地形沈むビアンコ）");
	unk70 = (TAreaCylinderManager*)gpConductor->search("ゴロゴロ発生マネージャー");
}

TSpineEnemy* TGorogoroManager::createEnemyInstance()
{
	return new TGorogoro("ゴロゴロ");
}

void TGorogoroManager::initSetEnemies()
{
	unk6C = new TGorogoroPolluteModelManager;
	unk6C->init((TLiveActor*)unk18[0]);

	static const char* graphlist[] = { "gorogoro0", "gorogoro1" };

	// Alternate the two graphs; fall back to the first if one is missing.
	for (int i = 0; i < mObjNum; ++i) {
		TGraphWeb* web = gpConductor->getGraphByName(graphlist[i % 2]);
		if (web->isDummy())
			web = gpConductor->getGraphByName(graphlist[0]);
		if (web->isDummy())
			continue;

		TGorogoro* goro = (TGorogoro*)unk18[i];
		JGeometry::TVec3<f32> point;
		web->unk0[0].getPoint((Vec*)&point);
		goro->unk124->setGraph(web);
		goro->mPosition         = point;
		goro->mGenerateGraphIdx = web->unk8 - 1;
	}
}

void TGorogoroManager::createModelData()
{
	static TModelDataLoadEntry entry[] = {
		{ "bosspaku_head.bmd", 0x10300000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

// TODO: 88%. The original calls a local out-of-line MsWrap<f> twice here
// (the map's MsWrap<f>__Ffff, 72 bytes, our one missing symbol); ours
// inlines the header template at both sites.
void TGorogoroManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		unk60++;
		s32 interval = ((TRollEnemySaveLoadParams*)unk38)
		                   ->mSLGenerateInterval.get();
		if (unk60 > interval) {
			unk60 = 0;

			// Only spawn while Mario is inside the spawning area.
			if (inArea(SMS_GetMarioPos())) {
				for (int i = 0; i < getActiveObjNum(); ++i) {
					TGorogoro* goro = (TGorogoro*)unk18[i];
					if (!goro->checkLiveFlag(LIVE_FLAG_DEAD))
						continue;

					if (unk64 && !unk64->isBuried(1)) {
						if (unk68) {
							// First spawn after the ground sinks: seat the
							// first two on fixed nodes, facing along the
							// rail, and aim them at the next node.
							unk68 = 0;

							goro->reset();
							JGeometry::TVec3<f32> point;
							goro->unk124->unk0->unk0[10].getPoint((Vec*)&point);
							goro->mPosition        = point;
							goro->unk124->mCurrIdx = 10;
							goro->unk124->mPrevIdx = 9;
							goro->unk124->unk0->unk0[11].getPoint((Vec*)&point);
							JGeometry::TVec3<f32> dir(point.x - goro->mPosition.x,
							                          0.0f,
							                          point.z - goro->mPosition.z);
							goro->mRotation.y
							    = MsWrap(MsGetRotFromZaxisY(dir), 0.0f, 360.0f);
							TPathNode goal(point);
							goro->unkF4  = goal;
							goro->unk104 = goal;
							goro->unk114.clear();

							TGorogoro* second = (TGorogoro*)unk18[1];
							second->reset();
							second->unk124->unk0->unk0[16].getPoint((Vec*)&point);
							second->mPosition        = point;
							second->unk124->mCurrIdx = 16;
							second->unk124->mPrevIdx = 15;
							second->unk124->unk0->unk0[17].getPoint((Vec*)&point);
							JGeometry::TVec3<f32> dir2(
							    point.x - second->mPosition.x, 0.0f,
							    point.z - second->mPosition.z);
							second->mRotation.y
							    = MsWrap(MsGetRotFromZaxisY(dir2), 0.0f, 360.0f);
							TPathNode goal2(point);
							second->unkF4  = goal2;
							second->unk104 = goal2;
							second->unk114.clear();
						} else {
							goro->reset();
						}
					} else if (unk64) {
						// Still buried: try again next frame.
						unk60 = interval;
					} else {
						goro->reset();
					}
					break;
				}
			}
		}
	}

	TEnemyManager::perform(cue, graphics);
	unk6C->perform(cue, graphics);
}

void TGorogoro::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000019;
	unk150     = 0x31;
	offHitFlag(HIT_FLAG_UNK8000000 | HIT_FLAG_UNK10000000 | HIT_FLAG_UNK40000000);
	mSpine->initWith(&TNerveGorogoroRollOnGraph::theNerve());
	unk1A4 = (TRollEnemySaveLoadParams*)getSaveParam();

	TMirrorActor* mirror = new TMirrorActor("ゴロゴロ in鏡");
	mirror->init(getMActor()->getModel(), 0x18);

	mTevKColor.a = 0xFF;

	// Both the live model and its reflection get the goop texture.
	const ResTIMG* timg = (const ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (timg) {
		SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(), "M_dummy",
		                     *timg);
		SMS_ChangeTextureAll(mirror->unk14->getModelData(), "M_dummy", *timg);
	}

	for (u16 i = 0;
	     i < getMActor()->getModel()->getModelData()->getMaterialNum(); ++i) {
		SMS_InitPacket_OneTevKColor(getMActor()->getModel(), i, GX_KCOLOR0,
		                            &mTevKColor);
		SMS_InitPacket_OneTevKColor(mirror->unk14, i, GX_KCOLOR0,
		                            &mTevKColor);
	}

	getMActor()->setJointCallback(1, RollEnemyBodyCallback);
	unk130 = 1;
}

// Binding level over a raw member read, worth +16 of low region in
// TGorogoro::perform (batch 127).
static inline MActor* IgaigaMActor(const TGorogoro* p)
{
	MActor* mActor = p->mMActor;
	return mActor;
}

void TGorogoro::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	// Keep animating a clipped-out one whose reflection is still visible.
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
	    && gpMirrorModelManager->isInMirror(mPosition)) {
		if (cue & CUE_CALC_ANIM) {
			calcRootMatrix();
			IgaigaMActor(this)->calc();
		}
		if (cue & CUE_CALC_VIEW)
			IgaigaMActor(this)->viewCalc();
	}
}

void TGorogoro::calcRootMatrix()
{
	gpCurRollEnemy = this;

	if (mSpine->getCurrentNerve() == &TNerveGorogoroDie::theNerve()) {
		TSpineEnemy::calcRootMatrix();
		if (checkLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH)) {
			unk1B4.ref(0, 3) = mPosition.x;
			unk1B4.ref(2, 3) = mPosition.z;
		}
		return;
	}

	if (isEaten())
		return;

	// Kick up dust while it is on the ground.
	f32 offsetY = unk1A4->mSLGroundOffsetY.get();
	if (mPosition.y < 30.0f + mGroundHeight) {
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x175, getMActor()->getModel()->getAnmMtx(0), 1, this);
		if (emitter)
			emitter->setGlobalScale(mScaling);
	}

	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x,
	               offsetY * unk158 + mPosition.y, mPosition.z, mRotation.x,
	               mRotation.y, mRotation.z);
	getModel()->setBaseScale(*(Vec*)&mScaling);
}

void TGorogoro::reset()
{
	unk130 = 1;
	TRollEnemy::reset();
	offLiveFlag(LIVE_FLAG_UNK1000);
	mTevKColor.a = 0xFF;
	unk1AC       = -10.0f;
	unk1B0 = 1.0f;
}

void TGorogoro::kill()
{
	mRollAngle = 0.0f;
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
	// Standing on water or bad ground kills it outright.
	BOOL illegal = mGroundPlane->isIllegalData();
	if (!illegal && !mGroundPlane->isPool() && !mGroundPlane->isWaterSurface())
		return;

	if (isAirborne())
		return;
	if (mSpine->getCurrentNerve() == &TNerveGorogoroDie::theNerve())
		return;

	mSpine->reset();
	mSpine->setNext(&TNerveGorogoroDie::theNerve());
	mSpine->pushAfterCurrent(mSpine->getDefault());
	onLiveFlag(LIVE_FLAG_UNK20000);
	onLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
}

// duplicating it to find out would be a fakematch, so it stays a call.
void TGorogoro::behaveToWater(THitActor* param_1)
{
	TRollEnemy::behaveToWater(param_1);

	// Fade the goop tint with the hit points that are left.
	s32 maxHp = getSaveParams() ? getSaveParams()->mSLHitPointMax.get() : 1;
	mTevKColor.a = (s8)((mHitPoints * 255) / maxHp);
	if (mHitPoints < 2)
		mHitPoints = 1;

	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    0x176, getMActor()->getModel()->getAnmMtx(0), 1, this);
	if (emitter)
		emitter->setGlobalScale(mScaling);
}

void TGorogoro::rollSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_KRPAKU_ROLL, &mPosition, nullptr,
	    abs(mGroundPlane->mNormal.y), 0, 0, nullptr, 0, 4);
}

// Binding level over the address of a struct member, worth +16 of low region
// in TGorogoro::boundSE (batch 130).
static inline const JGeometry::TVec3<f32>* IgaigaNormal(const TBGCheckData* p)
{
	const JGeometry::TVec3<f32>* normal = &p->mNormal;
	return normal;
}

void TGorogoro::boundSE()
{
	SMSGetMSound()->startSoundActorWithInfo(
	    MSD_SE_BS_KRPAKU_GND, &mPosition, nullptr,
	    abs(IgaigaNormal(mGroundPlane)->y), 0, 0, nullptr, 0, 4);
}

void TGorogoro::walkBehavior(int param_1, f32 param_2)
{
	if (mPosition.y > mGroundHeight)
		onLiveFlag(LIVE_FLAG_AIRBORNE);

	// Rolling into a watermill turns the wheel and slows the roll; the
	// ground, the roof and every touched wall are all checked for one.
	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		const TBGCheckData* ground = mGroundPlane;
		if (ground) {
			const TLiveActor* actor = ground->getActor();
			if (actor && actor->mActorType == 0x4000009A) {
				((TBiancoWatermill*)actor)->turnByEnemy(this, ground);
				TRollEnemy::walkBehavior(param_1, 0.2f * param_2);
			}
		}

		const TBGCheckData* roof;
		gpMap->checkRoof(mPosition.x, mPosition.y + mHeadHeight, mPosition.z,
		                 &roof);
		if (roof) {
			const TLiveActor* actor = roof->getActor();
			if (actor && actor->mActorType == 0x4000009A) {
				((TBiancoWatermill*)actor)->turnByEnemy(this, roof);
				TRollEnemy::walkBehavior(param_1, 0.3f * param_2);
			}
		}

		if (unk138 && unk138->getActor()) {
			TBGWallCheckRecord record(mPosition.x, mPosition.y + mHeadHeight,
			                          mPosition.z, mBodyScale * mWallRadius, 4,
			                          0);
			if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
				for (int i = 0; i < record.mResultWallsNum; ++i) {
					const TBGCheckData* wall = record.mResultWalls[i];
					const TLiveActor* actor  = wall->getActor();
					if (actor && actor->mActorType == 0x4000009A)
						((TBiancoWatermill*)actor)->turnByEnemy(this, wall);
				}
				TRollEnemy::walkBehavior(param_1, 0.2f * param_2);
			}
		}
	}

	mTurnSpeed = unk1A4->mSLTurnSpeedLow.get();
	TRollEnemy::walkBehavior(param_1, param_2);
	mRollAngle += 0.4f * mMarchSpeed;

	// Rolling onto water while alive drowns it.
	if (mSpine->getCurrentNerve() != &TNerveGorogoroDie::theNerve()
	    && mPosition.y < 10.0f + mGroundHeight
	    && mGroundPlane->isWaterSurface()) {
		onLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		onLiveFlag(LIVE_FLAG_UNK20000);
		kill();
	}
}

void TGorogoro::flagJump()
{
	JGeometry::TVec3<f32> target;
	unk124->getCurrent().getPoint((Vec*)&target);
	mPosition.y += 30.0f;

	f32 speed = unk124->unkC;
	JGeometry::TVec3<f32> vel
	    = calcVelocityToJumpToY(target, speed, getGravityY());
	unk1A8    = 1;
	mVelocity = vel;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
}

void TGorogoro::setDeadAnm()
{
	JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
	    0xBF, getMActor()->getModel()->getAnmMtx(1), 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(mScaling);

	setBckAnm(0);
	SMSGetMSound()->startSoundActor(MSD_SE_BS_KRPAKU_DOWN, &mPosition);
}

void TGorogoro::setMeltAnm()
{
	setBckAnm(1);
	unk130 = 0;
	onLiveFlag(LIVE_FLAG_UNK1000);

	// Freeze where it died, on the ground, and sink there.
	MTXCopy(getMActor()->getModel()->getBaseTRMtx(), unk1B4);
	unk1B4.ref(1, 3) = mGroundHeight;

	JPABaseEmitter* emitter
	    = gpMarioParticleManager->emitAndBindToMtxPtr(0xBE, unk1B4, 0, nullptr);
	if (emitter)
		emitter->setGlobalScale(mScaling);

	SMSGetMSound()->startSoundActor(MSD_SE_BS_KRPAKU_SINK, &mPosition, 0,
	                                nullptr, 0, 4);
}

void TGorogoro::bound()
{
	((TGorogoroManager*)mManager)->unk6C->generatePolluteModel(mPosition, mScaling);
	if (!checkLiveFlag(LIVE_FLAG_CLIPPED_OUT) && SMS_IsMarioTouchGround4cm())
		SMSRumbleMgr->start(0x15, 10, (Vec*)&mPosition);
}

const char** TGorogoro::getBasNameTable() const { return gorogoro_bastable; }

bool TGorogoro::isRolling()
{
	if (mSpine->getCurrentNerve() == &TNerveGorogoroRollOnGraph::theNerve())
		return true;
	if (isBckAnm(1))
		return true;
	return false;
}

void TGorogoro::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("bosspaku_head.bmd", 3);
}

void TGorogoro::generateByGateKeeper(const JGeometry::TVec3<f32>& pos,
                                     const JGeometry::TVec3<f32>& dir)
{
	reset();

	// Snap the tracer to the nearest node, then either charge at Mario if
	// he is in sight or head for that node.
	TGraphWeb* web    = unk124->unk0;
	int nearest       = web->findNearestNodeIndex(pos, 0xffffffff);
	unk124->mCurrIdx  = nearest;
	unk124->mPrevIdx  = nearest - 1;
	TGraphNode* node  = &web->unk0[nearest];

	bool sawMario;
	JGeometry::TVec3<f32> target;
	if (MsIsInSight(pos, dir.y, SMS_GetMarioPos(), 2000.0f, 360.0f, -1.0f)) {
		sawMario = true;
		target   = SMS_GetMarioPos();
	} else {
		node->getPoint((Vec*)&target);
		sawMario = false;
	}

	target.x -= pos.x;
	target.y -= pos.y;
	target.z -= pos.z;
	mPosition = pos;

	if (!target.isZero()) {
		// Aim up to 15 degrees either side, then jump 0..1500 that way.
		VECNormalize(&target, &target);
		s16 angle = DEG2SHORTANGLE(30.0f * MsRandF() - 15.0f);
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
		MTXMultVec(rot, &target, &target);

		mRotation.y = MsWrap(MsGetRotFromZaxisY(target), 0.0f, 360.0f);

		target.scale(1500.0f * MsRandF());
		target.x += pos.x;
		target.y += pos.y;
		target.z += pos.z;
		mVelocity = calcVelocityToJumpToY(target, 15.0f, getGravityY());
	} else {
		mVelocity   = calcVelocityToJumpToY(target, 15.0f, getGravityY());
		mRotation.y = MsWrap(MsGetRotFromZaxisY(target), 0.0f, 360.0f);
	}

	// Chasing Mario: push the current goal and make him the next one.
	if (sawMario) {
		TPathNode goal(SMS_GetMarioPos());
		unk114.push(unkF4);
		unkF4 = goal;
	}

	unk1A8 = 1;
}

DEFINE_NERVE(TNerveGorogoroRollOnGraph, TLiveActor)
{
	TGorogoro* goro = (TGorogoro*)spine->getBody();

	if (spine->getTime() == 0) {
		goro->goToShortestNextGraphNode();
		goro->setBckAnm(2);
	}

	if (goro->isReachedToGoalXZ()) {
		if (goro->jumpToNextGraphNode() >= 0)
			goro->flagJump();
		else
			goro->goToShortestNextGraphNode();
	}

	goro->walkBehavior(2, 1.0f);
	return FALSE;
}

DEFINE_NERVE(TNerveGorogoroDie, TLiveActor)
{
	TGorogoro* goro = (TGorogoro*)spine->getBody();

	if (spine->getTime() < 2) {
		goro->onHitFlag(HIT_FLAG_NO_COLLISION);

		// Dying over water splashes; dying on land stamps goop.
		BOOL onWater = goro->mGroundPlane->isWaterSurface();
		if (onWater && !goro->isAirborne())
			goro->generateEffectColumWater();

		if (goro->checkLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH)) {
			goro->setMeltAnm();
		} else {
			((TGorogoroManager*)goro->mManager)
			    ->unk6C->generatePolluteModel(goro->mPosition, goro->mScaling);
			goro->setDeadAnm();
			goro->setDeadEffect();
		}
	}

	if (goro->checkCurAnmEnd(0) || spine->getTime() > 360) {
		goro->onHitFlag(HIT_FLAG_NO_COLLISION);
		goro->onLiveFlag(LIVE_FLAG_DEAD);
		goro->onLiveFlag(LIVE_FLAG_UNK8);
		goro->offLiveFlag(LIVE_FLAG_HIDDEN);
		goro->offLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH);
		goro->mHolder = nullptr;
		goro->stopAnmSound();
		spine->reset();
		spine->setNext(&TNerveSmallEnemyDie::theNerve());
		spine->pushAfterCurrent(spine->getDefault());
		goro->genRandomItem();
		return FALSE;
	}

	if (goro->checkLiveFlag(TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH))
		goro->walkBehavior(2, 0.5f);
	return FALSE;
}
