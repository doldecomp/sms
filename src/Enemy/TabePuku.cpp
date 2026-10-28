#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/TabePuku.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JGeometry/JGRotation3.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JGeometry/JGVec4.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/BaseParam.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <System/Particles.hpp>

namespace {
const f32 cAngleMax = JGeometry::TUtil<f32>::PI() / 8.0f;
}

TTabePukuParams::TTabePukuParams(const char* param_1)
    : TSmallEnemyParams(param_1)
    , PARAM_INIT(mMarchSpeed, 0.15f)
    , PARAM_INIT(mAttackSpeed, 0.22f)
    , PARAM_INIT(mDiveSpeed, 0.4f)
    , PARAM_INIT(mWaterFric, 0.95f)
    , PARAM_INIT(mTurnSlerpRate, 0.05f)
    , PARAM_INIT(mApartHeight, 500.0f)
    , PARAM_INIT(mCorrectY, -40.0f)
    , PARAM_INIT(mCorrectZ, 150.0f)
    , PARAM_INIT(mTerritoryRange, 1000.0f)
    , PARAM_INIT(mDragLength, 2500.0f)
{
	TParams::load(mPrmPath);
}

TTPHitActor::TTPHitActor(TTabePuku& param_1)
    : THitActor("タベプク用当たり")
    , mOwner(&param_1)
{
}

void TTPHitActor::init()
{
	initHitActor(0x10000035, 1, HIT_CATEGORY_PLAYER, 10.0f, 10.0f, 10.0f,
	             10.0f);
	offHitFilter(HIT_FILTER_NO_COLLISION);
	onHitFilter(HIT_FILTER_NO_DAMAGE);

	TIdxGroupObj* group
	    = static_cast<TIdxGroupObj*>(JDrama::TNameRefGen::search("敵グループ"));
	group->getChildren().push_back(this);
}

BOOL TTPHitActor::receiveMessage(THitActor* param_1, u32 param_2)
{
	return mOwner->receiveMessage(param_1, param_2);
}

void TTPHitActor::updateObjCollision()
{
	setHitParams(mOwner->getSaveParams()->getSLAttackRadius(),
	             mOwner->getSaveParams()->getSLAttackHeight(),
	             mOwner->getSaveParams()->getSLDamageRadius(),
	             mOwner->getSaveParams()->getSLDamageHeight());
}

void TTPHitActor::updateTerrainCollsion()
{
	JGeometry::TQuat4<f32> quat = mOwner->mQuat;
	JGeometry::TVec3<f32> up;
	quat.getYDir(up);

	f32 height   = getAttackHeight();
	mCheckHeight = height;
	mCheckRadius = getAttackRadius();

	f32 sink = (2.0f / 3.0f) * height;
	if (mOwner->isHolding()) {
		sink += mOwner->getHeldObject()->getDamageHeight();
		mCheckHeight += mOwner->getHeldObject()->getDamageHeight();
		mCheckRadius += mOwner->getHeldObject()->getDamageRadius();
	}

	f32 lift = 0.5f * getAttackHeight();
	JGeometry::TVec3<f32> down(0.0f, -1.0f, 0.0f);

	JGeometry::TVec3<f32> pos;
	JGeometry::TVec3<f32>& ownerPos = mOwner->mPosition;
	pos.x                           = up.x * lift + ownerPos.x + down.x * sink;
	pos.y                           = up.y * lift + ownerPos.y + down.y * sink;
	pos.z                           = up.z * lift + ownerPos.z + down.z * sink;

	JGeometry::TVec3<f32> moved(pos);
	moved -= mPosition;
	mVelocity = moved;
	mPosition = pos;
}

void TTPHitActor::bind()
{
	JGeometry::TVec3<f32> pos(mPosition);
	pos.add(mVelocity);

	TTabePuku* owner = mOwner;
	pos.add(owner->getVelocity());
	pos.add(owner->mPositionDelta);

	f32 y = pos.y;
	f32 z = pos.z;

	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
	    pos.x, y + mCheckHeight, z, &mGroundPlane);
	mGroundHeight += 1.0f;

	if (y <= 0.05f + mGroundHeight) {
		mAirborne = false;

		JGeometry::TVec3<f32> ground(pos.x, mGroundHeight, z);
		f32 push = 1.0f
		           - (mGroundPlane->getNormal().dot(
		                  JGeometry::TVec3<f32>(pos.x, y, z))
		              - mGroundPlane->getNormal().dot(ground));
		if (push > 0.0f)
			pos.scaleAdd(push, mGroundPlane->getNormal(), pos);
		pos.y = ground.y;
	} else {
		mAirborne = true;
	}

	if (0.0f <= pos.y + mCheckHeight)
		pos.y = -mCheckHeight;

	TBGWallCheckRecord record(pos.x, pos.y, pos.z, mCheckRadius, 1, 0);
	bool touchedWall = gpMap->isTouchedWallsAndMoveXZ(&record);
	pos.x            = record.mCenter.x;
	pos.z            = record.mCenter.z;

	JGeometry::TVec3<f32> moved = pos;
	moved -= mPosition;
	mVelocity    = moved;
	mTouchedWall = touchedWall;
	mPosition    = pos;
}

void TTPHitActor::checkHitActors()
{
	THitActor** end = mCollisions + mColCount;
	for (THitActor** it = mCollisions; it != end; ++it) {
		switch ((*it)->mActorType) {
		case ACTOR_TYPE_MARIO:
			mOwner->attackToMario();
			break;
		}
	}
}

TTabePuku::TTabePuku(const char* param_1)
    : TSmallEnemy(param_1)
{
	onLiveFlag(LIVE_FLAG_UNK1000);
}

void TTabePuku::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	setMActorAndKeeper();
	mSpine->initWith(&TNerveTabePukuGraphWander::theNerve());
	initCollision();
	initParams();
	initAnmSound();
}

void TTabePuku::reset() { mScaledBodyRadius = 130.0f; }

void TTabePuku::initCollision()
{
	initHitActor(0x10000035, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	unk194 = new TTPHitActor(*this);
	unk194->init();
	unk194->mPosition = mPosition;
}

void TTabePuku::initParams()
{
	mQuat.set(SMS_Eular2Quat(mRotation));
	u16 jointIndex
	    = getModel()->getModelData()->getJointName()->getIndex("jnt_mouth_up");
	mMouthJointIndex = jointIndex;
}

void TTabePuku::perform(u32 cue, JDrama::TGraphics* param_2)
{
	unk194->perform(cue, param_2);
	TSmallEnemy::perform(cue, param_2);
}

void TTabePuku::control()
{
	TLiveActor::control();

	unk194->checkHitActors();
	updateSound();
}

void TTabePuku::updateSound()
{
	if (isBiting())
		SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_CHEW, &mPosition, 0,
		                                nullptr, 0, 4);
}

void TTabePuku::bind()
{
	unk194->updateObjCollision();
	unk194->updateTerrainCollsion();
	unk194->bind();
	mPositionDelta = unk194->mVelocity;
	mTouchedWall   = unk194->mTouchedWall;
	if (unk194->mAirborne)
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	else
		offLiveFlag(LIVE_FLAG_AIRBORNE);
	mGroundPlane  = unk194->mGroundPlane;
	mGroundHeight = unk194->mGroundHeight;
}

void TTabePuku::calcRootMatrix()
{
	if (isTaken()) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	TPosition3f mtx;
	mtx.setQT(mQuat, mPosition);
	getModel()->setBaseScale(mScaling);
	getModel()->setBaseTRMtx(mtx);
	emitEffects();
}

void TTabePuku::emitEffects()
{
	JPABaseEmitter* emitter = SMS_EasyEmitParticle(
	    PARTICLE_MS_PUKU_AWA, getModel()->getAnmMtx(mMouthJointIndex), this,
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	if (emitter) {
		f32 depth = -mPosition.y / 100.0f;
		if (depth <= 0.0f)
			depth = 0.0f;
		int lifetime = (int)depth * 20 + 2;
		if (lifetime > 200)
			lifetime = 200;
		emitter->setLifeTime(lifetime);
		if (isAttacking())
			emitter->setRate(0.1f);
	}
}

BOOL TTabePuku::receiveMessage(THitActor* param_1, u32 param_2)
{
	switch (param_2) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		return FALSE;
	default:
		return TSmallEnemy::receiveMessage(param_1, param_2);
	}
}

MtxPtr TTabePuku::getTakingMtx()
{
	mTakeMtx.setQuat(mQuat);
	JGeometry::TVec3<f32> forward;
	JGeometry::TVec3<f32> up;
	mTakeMtx.getZDir(forward);
	mTakeMtx.getYDir(up);
	forward.scaleAdd(((TTabePukuParams*)getSaveParam())->mCorrectZ.value,
	                 forward, mPosition);
	up.scaleAdd(((TTabePukuParams*)getSaveParam())->mCorrectY.value, up,
	            forward);
	mTakeMtx.setTrans(up);
	return mTakeMtx;
}

static const char* tabepuku_bastable[] = {
	"/scene/tabepuku/bas/pukupuku_chase.bas",
	"/scene/tabepuku/bas/pukupuku_search.bas",
	"/scene/tabepuku/bas/pukupuku_swim.bas",
};

const char** TTabePuku::getBasNameTable() const { return tabepuku_bastable; }

void TTabePuku::attackToMario()
{
	if (isBiting())
		return;

	if (isGraphWander())
		return;

	if (SMS_SendMessageToMario(this, HIT_MESSAGE_TAKE)) {
		mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
		mSpine->reset();
		mSpine->setNext(&TNerveTabePukuBite::theNerve());
	}
}

void TTabePuku::behaveToWater(THitActor* param_1) { }

void TTabePuku::forceKill() { }

bool TTabePuku::isFindMario(f32 param_1)
{
	return isFindMarioFromParam(param_1);
}

bool TTabePuku::doKeepDistance() { return !(isAttacking() || isBiting()); }

bool TTabePuku::isMissMario() const { return false; }

bool TTabePuku::isTouchedPlane() const { return !isAirborne() || mTouchedWall; }

bool TTabePuku::isGraphWander() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveTabePukuGraphWander::theNerve()
	       || nerve == &TNerveTabePukuRecoverGraph::theNerve();
}

bool TTabePuku::isAttacking() const
{
	return mSpine->getLatestNerve() == &TNerveTabePukuAttack::theNerve();
}

bool TTabePuku::isBiting() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveTabePukuBite::theNerve()
	       || nerve == &TNerveTabePukuDive::theNerve()
	       || nerve == &TNerveTabePukuDrag::theNerve();
}

void TTabePuku::swimToCurPathNode(const JGeometry::TVec3<f32>& offset)
{
	JGeometry::TVec3<f32> dir(getUnk104().getPoint());
	dir -= mPosition;
	dir += offset;
	swimTo(dir);
}

void TTabePuku::doBite() { }

void TTabePuku::prepareDive()
{
	mDiveStartY = mPosition.y;
	setBckAnm(2);
	getMActor()->getFrameCtrl(0)->setRate(2.0f * SMSGetAnmFrameRate());
}

bool TTabePuku::doDive()
{
	swimTo(JGeometry::TVec3<f32>(0.0f, mGroundHeight - mPosition.y, 0.0f));

	if (mPosition.y - mDiveStartY
	        < -((TTabePukuParams*)getSaveParam())->mApartHeight.get()
	    || mPosition.y - mGroundHeight < 200.0f || !isAirborne())
		return true;

	return false;
}

void TTabePuku::prepareDrag()
{
	mDragDir.set(0.0f, 0.0f, 1.0f);
	JGeometry::TQuat4<f32> quat;
	quat.setEulerY(MsRandF() * (2.0f * JGeometry::TUtil<f32>::PI()));
	quat.rotate(mDragDir);
	setGoalPath(mPosition);
}

bool TTabePuku::doDrag() { return false; }

void TTabePuku::swimTo(const JGeometry::TVec3<f32>& dir)
{
	JGeometry::TVec3<f32> d(dir);

	if (JGeometry::TUtil<f32>::epsilonEquals(
	        0.0f, dir.squared(), JGeometry::TUtil<f32>::epsilon())) {
		JGeometry::TVec3<f32> forward;
		mQuat.getZDir(forward);
		forward.scale(mMarchSpeed);

		JGeometry::TVec3<f32> velocity = mVelocity;
		velocity.scale(((TTabePukuParams*)getSaveParam())->mWaterFric.get());
		velocity.add(forward);
		mVelocity = velocity;

		mRotation.y = MsGetRotFromZaxisY(velocity);
		return;
	}

	d.normalize();

	JGeometry::TVec3<f32> zaxis(0.0f, 0.0f, 1.0f);
	JGeometry::TQuat4<f32> target;
	if (JGeometry::TUtil<f32>::epsilonEquals(-1.0f, d.dot(zaxis),
	                                         JGeometry::TUtil<f32>::epsilon()))
		target.setEulerY(JGeometry::TUtil<f32>::PI());
	else
		target.setRotate(zaxis, d, 1.0f);

	mQuat.slerp(target,
	            ((TTabePukuParams*)getSaveParam())->mTurnSlerpRate.value);
	mQuat.normalize();

	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	forward.scale(mMarchSpeed);

	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(((TTabePukuParams*)getSaveParam())->mWaterFric.get());
	velocity.add(forward);
	mVelocity = velocity;

	mRotation.y = MsGetRotFromZaxisY(velocity);
}

void TTabePuku::setMomentumFromQuat() { }

void TTabePuku::detach()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_DETACH);
	mHeldObject = nullptr;
}

TTabePukuManager::TTabePukuManager(const char* param_1)
    : TSmallEnemyManager(param_1)
{
}

void TTabePukuManager::load(JSUMemoryInputStream& param_1)
{
	unk38 = new TTabePukuParams("/enemy/tabepuku.prm");
	TSmallEnemyManager::load(param_1);
}

void TTabePukuManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "tabepuku.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

DEFINE_NERVE(TNerveTabePukuGraphWander, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->getTracer()->mPrevIdx = -1;
		self->goToShortestNextGraphNode();
		self->setBckAnm(2);
		self->mMarchSpeed
		    = ((TTabePukuParams*)self->getSaveParam())->mMarchSpeed.get();
	}
	if (self->isReachedToGoal())
		self->goToRandomNextGraphNode();
	if (self->isFindMario(1.0f)) {
		spine->pushAfterCurrent(&TNerveTabePukuFound::theNerve());
		return true;
	}
	self->swimToCurPathNode(JGeometry::TVec3<f32>(0.0f));
	return false;
}

DEFINE_NERVE(TNerveTabePukuFound, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(1);
		self->mMarchSpeed = 0.0f;
	}

	JGeometry::TVec3<f32> forward;
	self->mQuat.getZDir(forward);
	forward.scale(self->mMarchSpeed);
	JGeometry::TVec3<f32> velocity = self->mVelocity;
	velocity.scale(((TTabePukuParams*)self->getSaveParam())->mWaterFric.get());
	velocity.add(forward);
	self->mVelocity   = velocity;
	self->mRotation.y = MsGetRotFromZaxisY(velocity);

	if (self->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveTabePukuAttack::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->getTracer()->mPrevIdx = -1;
		self->getTracer()->mCurrIdx = -1;
		self->goToShortestNextGraphNode();
		self->mMarchSpeed
		    = ((TTabePukuParams*)self->getSaveParam())->mMarchSpeed.get();
	}
	if (self->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveTabePukuGraphWander::theNerve());
		return true;
	}
	JGeometry::TVec3<f32> offset;
	if (self->isTouchedPlane()) {
		offset.set(0.0f, 10000.0f, 0.0f);
	} else {
		offset.zero();
	}
	self->swimToCurPathNode(offset);
	return false;
}

DEFINE_NERVE(TNerveTabePukuAttack, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setBckAnm(0);
		self->setGoalPath((THitActor*)gpMarioAddress);
		self->mMarchSpeed
		    = ((TTabePukuParams*)self->getSaveParam())->mAttackSpeed.get();
	}
	bool lost = fabsf(SMS_GetMarioY() - self->mPosition.y)
	            > self->getSaveParams()->getSLGiveUpHeight();
	if (!lost) {
		f32 giveUpLength = self->getSaveParams()->getSLGiveUpLength();
		JGeometry::TVec3<f32> toGoal = self->getUnk104().getPoint();
		toGoal -= self->mPosition;
		if (toGoal.length() > giveUpLength) {
			lost = true;
		} else {
			JGeometry::TVec3<f32> nearest
			    = self->getTracer()->getGraph()->getNearestPosOnGraphLink(
			        self->mPosition);
			nearest.sub(self->mPosition);
			f32 radius = ((TTabePukuParams*)self->getSaveParam())
			                 ->mTerritoryRange.get();
			lost = radius * radius <= nearest.squared();
		}
	}
	if (lost || self->mTouchedWall) {
		spine->pushAfterCurrent(&TNerveTabePukuRecoverGraph::theNerve());
		return true;
	}
	self->swimToCurPathNode(JGeometry::TVec3<f32>(0.0f, 150.0f, 0.0f));
	return false;
}

DEFINE_NERVE(TNerveTabePukuBite, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();

	self->setBckAnm(2);
	SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_BITE, &self->mPosition,
	                                0, nullptr, 0, 4);

	spine->pushAfterCurrent(&TNerveTabePukuDive::theNerve());
	return true;
}

DEFINE_NERVE(TNerveTabePukuDive, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareDive();
		self->mMarchSpeed
		    = ((TTabePukuParams*)self->getSaveParam())->mDiveSpeed.get();
	}
	if (self->doDive()) {
		spine->pushAfterCurrent(&TNerveTabePukuDrag::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTabePukuDrag, TLiveActor)
{
	TTabePuku* self = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		self->prepareDrag();
		self->mMarchSpeed
		    = ((TTabePukuParams*)self->getSaveParam())->mDiveSpeed.get();
	}

	self->swimTo(self->mDragDir);
	if (self->mTouchedWall || !self->isAirborne()
	    || self->getUnk104().getPoint().distance(self->mPosition)
	           > ((TTabePukuParams*)self->getSaveParam())->mDragLength.get()) {
		self->detach();
		spine->pushAfterCurrent(&TNerveTabePukuRecoverGraph::theNerve());
		return true;
	}
	return false;
}
