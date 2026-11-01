#include <Enemy/TabePuku.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <M3DUtil/MActor.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Particles.hpp>
#include <stdlib.h>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/MActorAnm.hpp>

namespace {

// TODO: only set here; whatever read it is UNUSED.
f32 cAngleMax = JGeometry::TUtil<f32>::PI() / 8;

} // namespace

static const char* tabepuku_bastable[] = {
	"/scene/tabepuku/bas/pukupuku_chase.bas",
	"/scene/tabepuku/bas/pukupuku_search.bas",
	"/scene/tabepuku/bas/pukupuku_swim.bas",
};

TTabePukuParams::TTabePukuParams(const char* path)
    : TSmallEnemyParams(path)
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

TTPHitActor::TTPHitActor(TTabePuku& owner)
    : THitActor("タベプク用当たり")
    , mOwner(&owner)
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

BOOL TTPHitActor::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

// Size the collision after the fish's parameters.
void TTPHitActor::updateObjCollision()
{
	f32 damageHeight = mOwner->getParam()->mSLDamageHeight.get();
	f32 damageRadius = mOwner->getParam()->mSLDamageRadius.get();
	f32 attackHeight = mOwner->getParam()->mSLAttackHeight.get();
	mAttackRadius    = mOwner->getParam()->mSLAttackRadius.get();
	mAttackHeight    = attackHeight;
	mDamageRadius    = damageRadius;
	mDamageHeight    = damageHeight;
	calcEntryRadius();
}

// Hang the collision below the fish, and below Mario too while it carries
// him.
void TTPHitActor::updateTerrainCollsion()
{
	JGeometry::TQuat4<f32> rot = mOwner->mQuat;
	JGeometry::TVec3<f32> up;
	rot.getYDir(up);
	f32 depth = 0.6666667f * mAttackHeight;
	mHeight   = mAttackHeight;
	mRadius   = mAttackRadius;
	if (mOwner->isHolding()) {
		depth += mOwner->getHeldObject()->mDamageHeight;
		mHeight += mOwner->getHeldObject()->mDamageHeight;
		mRadius += mOwner->getHeldObject()->mDamageRadius;
	}

	JGeometry::TVec3<f32> center;
	center.scaleAdd(0.5f * mAttackHeight, up, mOwner->mPosition);
	JGeometry::TVec3<f32> pos;
	pos.scaleAdd(depth, JGeometry::TVec3<f32>(0.0f, -1.0f, 0.0f), center);

	JGeometry::TVec3<f32> delta = pos;
	delta -= mPosition;
	mPositionDelta = delta;
	mPosition      = pos;
}

void TTPHitActor::bind()
{
	JGeometry::TVec3<f32> next = mPosition;
	next += mPositionDelta;
	JGeometry::TVec3<f32> velocity = mOwner->mVelocity;
	next += velocity;
	next += mOwner->mPositionDelta;

	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
	    next.x, next.y + mHeight, next.z, &mGroundPlane);
	mGroundHeight += 1.0f;
	if (next.y <= 0.05f + mGroundHeight) {
		mAirborne = false;
		// push off steep ground along its normal
		JGeometry::TVec3<f32> ground(next.x, mGroundHeight, next.z);
		f32 push = 1.0f
		           - (mGroundPlane->getNormal().dot(next)
		              - mGroundPlane->getNormal().dot(ground));
		if (push > 0.0f)
			next.scaleAdd(push, mGroundPlane->getNormal(), next);

		next.y = mGroundHeight;
	} else {
		mAirborne = true;
	}

	// stay under the water surface
	if (0.0f <= next.y + mHeight)
		next.y = -mHeight;

	TBGWallCheckRecord record(next.x, next.y, next.z, mRadius, 1, 0);
	bool touched = gpMap->isTouchedWallsAndMoveXZ(&record);
	next.x       = record.mCenter.x;
	next.z       = record.mCenter.z;

	mPositionDelta = next - mPosition;
	mTouchedWall   = touched;
	mPosition      = next;
}

void TTPHitActor::checkHitActors()
{
	for (THitActor **it = mCollisions, **end = mCollisions + mColCount;
	     it != end; it++) {
		switch ((*it)->getActorType()) {
		case ACTOR_TYPE_MARIO:
			mOwner->attackToMario();
			break;
		}
	}
}

TTabePuku::TTabePuku(const char* name)
    : TSmallEnemy(name)
{
	onLiveFlag(LIVE_FLAG_UNK1000);
}

void TTabePuku::init(TLiveManager* manager)
{
	mManager = manager;
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
	initHitActor(ACTOR_TYPE_TABE_PUKU, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	mHitActor = new TTPHitActor(*this);
	mHitActor->init();
	mHitActor->mPosition = mPosition;
}

void TTabePuku::initParams()
{
	mQuat.set(SMS_Eular2Quat(mRotation));
	u16 joint
	    = getModel()->getModelData()->getJointName()->getIndex("jnt_mouth_up");
	mMouthJoint = joint;
}

void TTabePuku::perform(u32 cue, JDrama::TGraphics* graphics)
{
	mHitActor->perform(cue, graphics);
	TSmallEnemy::perform(cue, graphics);
}

void TTabePuku::control()
{
	TLiveActor::control();
	mHitActor->checkHitActors();
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
	mHitActor->updateObjCollision();
	mHitActor->updateTerrainCollsion();
	mHitActor->bind();
	mPositionDelta = mHitActor->mPositionDelta;
	mTouchedWall   = mHitActor->mTouchedWall;
	if (mHitActor->mAirborne)
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	else
		offLiveFlag(LIVE_FLAG_AIRBORNE);
	mGroundPlane  = mHitActor->mGroundPlane;
	mGroundHeight = mHitActor->mGroundHeight;
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

// Bubbles from the mouth, more of them the deeper the fish swims.
void TTabePuku::emitEffects()
{
	// TODO: the particle enums are fabricated; this one is emitted as a loop
	// effect here.
	JPABaseEmitter* emitter
	    = SMS_EasyEmitParticle((E_SMS_EFFECT_LOOP_NORMAL)PARTICLE_MS_PUKU_AWA,
	                           getModel()->getAnmMtx(mMouthJoint), this,
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

BOOL TTabePuku::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		return false;
	}
	return TSmallEnemy::receiveMessage(sender, message);
}

// Where Mario hangs while the fish carries him: in front of and below the
// mouth.
MtxPtr TTabePuku::getTakingMtx()
{
	mTakingMtx.setQuat(mQuat);
	JGeometry::TVec3<f32> zDir;
	mTakingMtx.getZDir(zDir);
	JGeometry::TVec3<f32> yDir;
	mTakingMtx.getYDir(yDir);
	JGeometry::TVec3<f32> pos;
	pos.scaleAdd(getParam()->mCorrectZ.get(), zDir, mPosition);
	JGeometry::TVec3<f32> trans;
	trans.scaleAdd(getParam()->mCorrectY.get(), yDir, pos);
	mTakingMtx.setTrans(trans);
	return mTakingMtx;
}

const char** TTabePuku::getBasNameTable() const { return tabepuku_bastable; }

void TTabePuku::attackToMario()
{
	if (isBiting())
		return;

	if (!isGraphWander() && SMS_SendMessageToMario(this, HIT_MESSAGE_TAKE)) {
		mHeldObject = (TTakeActor*)SMS_GetMarioHitActor();
		mSpine->reset();
		mSpine->setNext(&TNerveTabePukuBite::theNerve());
	}
}

void TTabePuku::behaveToWater(THitActor*) { }

void TTabePuku::forceKill() { }

bool TTabePuku::isFindMario(f32 rate) { return isFindMarioFromParam(rate); }

bool TTabePuku::doKeepDistance() { return !(isAttacking() || isBiting()); }

// Size matches the map (0x1C4); body taken from TNerveTabePukuAttack.
// Mario got too far above/below, too far from the goal or left the
// territory around the rail graph.
bool TTabePuku::isMissMario() const
{
	f32 giveUpHeight = getParam()->mSLGiveUpHeight.get();
	if (fabs(gpMarioPos->y - mPosition.y) > giveUpHeight)
		return true;

	f32 giveUpLen = getParam()->mSLGiveUpLength.get();
	if ((mGoal.getPoint() - mPosition).length() > giveUpLen)
		return true;

	JGeometry::TVec3<f32> nearest
	    = unk124->getGraph()->getNearestPosOnGraphLink(mPosition);
	nearest -= mPosition;
	f32 range = getParam()->mTerritoryRange.get();
	if (range * range <= nearest.squared())
		return true;

	return false;
}

bool TTabePuku::isTouchedPlane() const
{
	return !(checkLiveFlag(LIVE_FLAG_AIRBORNE) ? TRUE : FALSE);
}

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
	JGeometry::TVec3<f32> dir = getGoal().getPoint();
	dir -= mPosition;
	dir += offset;
	swimTo(dir);
}

// TODO: UNUSED, 4 bytes.
void TTabePuku::doBite() { }

void TTabePuku::prepareDive()
{
	mDiveStartY = mPosition.y;
	setBckAnm(2);
	getMActor()
	    ->getFrameCtrl(ANM_TYPE_BCK)
	    ->setRate(2.0f * SMSGetAnmFrameRate());
	mMarchSpeed = getParam()->mDiveSpeed.get();
}

// Head straight down to the floor, then start dragging.
bool TTabePuku::doDive()
{
	swimTo(JGeometry::TVec3<f32>(0.0f, mGroundHeight - mPosition.y, 0.0f));
	if (mPosition.y - mDiveStartY < -getParam()->mApartHeight.get()
	    || mPosition.y - mGroundHeight < 200.0f || isTouchedPlane())
		return true;
	return false;
}

void TTabePuku::prepareDrag()
{
	mDragDir.set(0.0f, 0.0f, 1.0f);
	JGeometry::TQuat4<f32> quat;
	quat.setEulerY(MsRandF() * (2.0f * JGeometry::TUtil<f32>::PI()));
	quat.rotate(mDragDir);
	setGoal(mPosition);
}

// Drag Mario along the floor in a random direction until he gets away.
bool TTabePuku::doDrag()
{
	swimTo(mDragDir);
	if (mTouchedWall || !isAirborne()
	    || ((TTabePukuParams*)getSaveParam())->mDragLength.get()
	           < getGoalDistance()) {
		detach();
		return true;
	}
	return false;
}

// The cosAngle and front locals are a guess; the body has to be at least 15
// statements long, or the compiler would auto-inline it into doDrag.
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

// Swim forwards with the water slowing the fish down.
void TTabePuku::setMomentumFromQuat()
{
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	forward.scale(mMarchSpeed);
	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(getParam()->mWaterFric.get());
	velocity += forward;
	mVelocity   = velocity;
	mRotation.y = MsGetRotFromZaxisY(velocity);
}

void TTabePuku::detach()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_DETACH);
	mHeldObject = nullptr;
}

TTabePukuManager::TTabePukuManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TTabePukuManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TTabePukuParams("/enemy/tabepuku.prm");
	TSmallEnemyManager::load(stream);
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
	TTabePuku* puku = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		puku->getTracer()->reset();
		puku->goToShortestNextGraphNode();
		puku->setBckAnm(2);
		puku->mMarchSpeed = puku->getParam()->mMarchSpeed.get();
	}

	if (puku->isReachedToGoal())
		puku->goToRandomNextGraphNode();

	if (puku->isFindMario(1.0f)) {
		spine->pushAfterCurrent(&TNerveTabePukuFound::theNerve());
		return true;
	}

	puku->swimToCurPathNode(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	return false;
}

DEFINE_NERVE(TNerveTabePukuFound, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		puku->setBckAnm(1);
		puku->mMarchSpeed = 0.0f;
	}

	puku->setMomentumFromQuat();
	if (puku->checkCurAnmEnd(0)) {
		spine->pushAfterCurrent(&TNerveTabePukuAttack::theNerve());
		return true;
	}
	return false;
}

DEFINE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		puku->getTracer()->reset();
		puku->getTracer()->reset2();
		puku->goToShortestNextGraphNode();
		puku->mMarchSpeed = puku->getParam()->mMarchSpeed.get();
	}

	if (puku->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveTabePukuGraphWander::theNerve());
		return true;
	}

	// rise first when stuck on the floor or a wall
	bool rise = puku->isTouchedPlane() || puku->mTouchedWall;
	JGeometry::TVec3<f32> offset;
	if (rise)
		offset.set(0.0f, 10000.0f, 0.0f);
	else
		offset.zero();
	puku->swimToCurPathNode(offset);
	return false;
}

DEFINE_NERVE(TNerveTabePukuAttack, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0) {
		puku->setBckAnm(0);
		puku->setGoal((THitActor*)gpMarioAddress);
		puku->mMarchSpeed = puku->getParam()->mAttackSpeed.get();
	}

	// TODO: the target calls TVec3::dot out of line inside isMissMario's
	// length(), one inline level deeper than here.
	if (puku->isMissMario() || puku->mTouchedWall) {
		spine->pushAfterCurrent(&TNerveTabePukuRecoverGraph::theNerve());
		return true;
	}

	puku->swimToCurPathNode(JGeometry::TVec3<f32>(0.0f, 150.0f, 0.0f));
	return false;
}

DEFINE_NERVE(TNerveTabePukuBite, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();
	puku->setBckAnm(2);
	SMSGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_BITE, &puku->mPosition,
	                                0, nullptr, 0, 4);
	spine->pushAfterCurrent(&TNerveTabePukuDive::theNerve());
	return true;
}

DEFINE_NERVE(TNerveTabePukuDive, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();
	if (spine->getTime() == 0)
		puku->prepareDive();

	if (puku->doDive()) {
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

	if (self->doDrag()) {
		spine->pushAfterCurrent(&TNerveTabePukuRecoverGraph::theNerve());
		return true;
	}
	return false;
}
