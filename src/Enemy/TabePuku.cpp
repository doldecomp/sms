#include <Enemy/TabePukuNerve.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Parked here, not in a header: the map has no symbol for it, so retail had
// it as a file-scope `inline`. It is the level that reaches the map's
// out-of-line JGeometry::TVec3<f32>::sub, ::dot and TUtil<f32>::sqrt at the
// copy-and-subtract distance tests inside isMissMario and doDrag (the same
// shape as emario's EMarioCalcDist and AnimalNerve's calcDist).
static inline f32 TabePukuLength(const JGeometry::TVec3<f32>& v)
{
	return v.length();
}

// The three .bck slots of the tabepuku model, in the alphabetical order the
// model data indexes them; tabepuku_bastable names all three.
enum {
	TABEPUKU_ANM_CHASE  = 0,
	TABEPUKU_ANM_SEARCH = 1,
	TABEPUKU_ANM_SWIM   = 2,
};

static const char* tabepuku_bastable[] = {
	"/scene/tabepuku/bas/pukupuku_chase.bas",
	"/scene/tabepuku/bas/pukupuku_search.bas",
	"/scene/tabepuku/bas/pukupuku_swim.bas",
};

namespace {
// Dead: nothing in the translation unit reads it. It has to sit in .sbss with
// a dynamic initialiser in __sinit_TabePuku_cpp, which is what the inline
// PI() call buys -- a plain 0.3926991f literal lands in .sdata instead. The
// value is pi/8, i.e. 22.5 degrees.
f32 cAngleMax = JGeometry::TUtil<f32>::PI() / 8.0f;
}

TTabePukuParams::TTabePukuParams(const char* prm)
    : TSmallEnemyParams(prm)
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

// UNUSED, 0x60 in the map.
TTPHitActor::TTPHitActor(TTabePuku& owner)
    : THitActor("\x83\x5e\x83\x78\x83\x76\x83\x4e\x97\x70\x93\x96\x82\xbd\x82"
                "\xe8")
    , mOwner(&owner)
{
}

void TTPHitActor::init()
{
	initHitActor(0x10000035, 1, -0x80000000, 10.0f, 10.0f, 10.0f, 10.0f);

	offHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);

	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>(
	    "\x93\x47\x83\x4f\x83\x8b\x81\x5b\x83\x76");
	group->getChildren().push_back(this);
}

BOOL TTPHitActor::receiveMessage(THitActor* sender, u32 message)
{
	return mOwner->receiveMessage(sender, message);
}

// UNUSED, 0x124 in the map: the bite volume tracks the body's own attack and
// damage extents.
void TTPHitActor::updateObjCollision()
{
	setHitParams(mOwner->getSaveParams()->getSLAttackRadius(),
	             mOwner->getSaveParams()->getSLAttackHeight(),
	             mOwner->getSaveParams()->getSLDamageRadius(),
	             mOwner->getSaveParams()->getSLDamageHeight());
}

// TODO: 86.7%. Residual: retail loads the 2/3 constant before anything else
// and keeps the owner position base in one register with lfsu; the rest is
// float register numbering plus the isHolding() BOOL note below.
void TTPHitActor::updateTerrainCollsion()
{
	TTabePuku* owner = mOwner;

	JGeometry::TQuat4<f32> quat = owner->mQuat;
	JGeometry::TVec3<f32> up;
	quat.getYDir(up);

	f32 height   = getAttackHeight();
	mCheckHeight = height;
	mCheckRadius = getAttackRadius();

	f32 sink = (2.0f / 3.0f) * height;
	// TODO: retail materialises this test as a BOOL (li 1 / li 0 / cmpwi),
	// which means TTakeActor::isHolding() returned BOOL, not bool. Changing
	// that is a shared-header fix in Strategic/TakeActor.hpp.
	if (mOwner->isHolding()) {
		sink += mOwner->getHeldObject()->getDamageHeight();
		mCheckHeight += mOwner->getHeldObject()->getDamageHeight();
		mCheckRadius += mOwner->getHeldObject()->getDamageRadius();
	}

	f32 lift = 0.5f * getAttackHeight();
	JGeometry::TVec3<f32> down(0.0f, -1.0f, 0.0f);

	JGeometry::TVec3<f32> pos;
	JGeometry::TVec3<f32>& ownerPos = mOwner->mPosition;
	pos.x = up.x * lift + ownerPos.x + down.x * sink;
	pos.y = up.y * lift + ownerPos.y + down.y * sink;
	pos.z = up.z * lift + ownerPos.z + down.z * sink;

	JGeometry::TVec3<f32> moved(pos);
	moved.sub(mPosition);
	mVelocity = moved;
	mPosition = pos;
}

// TODO: 76.7%. The two dot products fuse their x and z terms into fmadds in
// retail while ours common-subexpressions them into one fmuls each, and the
// closing TVec3::sub() is a `bl` in retail but expands here (the known
// per-call-site TVec3::sub problem in docs/catalog/codegen-tells.md).
void TTPHitActor::bind()
{
	JGeometry::TVec3<f32> pos(mPosition);
	pos.add(mVelocity);

	TTabePuku* owner = mOwner;
	JGeometry::TVec3<f32> velocity = owner->mVelocity;
	pos.add(velocity);
	pos.add(owner->mLinearVelocity);

	f32 y = pos.y;
	f32 z = pos.z;

	mGroundHeight = gpMap->checkGroundIgnoreWaterSurface(
	    pos.x, y + mCheckHeight, z, &mGroundPlane);
	mGroundHeight += 1.0f;

	if (y <= 0.05f + mGroundHeight) {
		mAirborne = false;

		// Push the mouth back out along the plane it sank into, then sit it
		// exactly on the ground.
		f32 push = 1.0f
		         - (mGroundPlane->getNormal().dot(pos)
		            - mGroundPlane->getNormal().dot(
		                JGeometry::TVec3<f32>(pos.x, mGroundHeight, z)));
		if (push > 0.0f)
			pos.scaleAdd(push, mGroundPlane->getNormal(), pos);
		pos.y = mGroundHeight;
	} else {
		mAirborne = true;
	}

	// Keep the top of the bite volume under the water plane at y = 0.
	if (0.0f <= pos.y + mCheckHeight)
		pos.y = -mCheckHeight;

	TBGWallCheckRecord record;
	record.mCenter.set(pos);
	record.mRadius      = mCheckRadius;
	record.mMaxResults  = 1;
	record.mFlags       = 0;
	int touchedWall = gpMap->isTouchedWallsAndMoveXZ(&record);
	pos.x        = record.mCenter.x;
	pos.z        = record.mCenter.z;

	// `a = b - c` reaches the map's out-of-line TVec3::sub: operator= is one
	// inline level and the difference nested in its argument two more.
	mVelocity = pos - mPosition;
	mTouchedWall = touchedWall;
	mPosition = pos;
}

// UNUSED, 0x94 in the map; ours is 0x84 because retail hoists the Mario actor
// type into a register (see the TODO below).
void TTPHitActor::checkHitActors()
{
	THitActor** end = &mCollisions[mColCount];
	for (THitActor** col = mCollisions; col != end; col++) {
		// A switch, not an `if`: only the switch's comparison materialises
		// 0x80000001 in a register and compares it with a signed cmpw, which
		// is what retail does. Every `if` spelling (plain literal, (s32) cast,
		// -0x7FFFFFFF, ACTOR_TYPE_PLAYER | 1, a hoisted int local) folds into
		// MWCC's addis+cmplwi equality trick instead.
		switch ((*col)->mActorType) {
		case 0x80000001:
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

void TTabePuku::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);
	setMActorAndKeeper();

	mSpine->initWith(&TNerveTabePukuGraphWander::theNerve());

	initCollision();
	initParams();
	initAnmSound();
}

void TTabePuku::reset() { mScaledBodyRadius = 130.0f; }

// UNUSED, 0xcc in the map.
void TTabePuku::initCollision()
{
	initHitActor(0x10000035, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	mMouthHit = new TTPHitActor(*this);
	mMouthHit->init();
	mMouthHit->mPosition = mPosition;
}

// UNUSED, 0x78 in the map.
void TTabePuku::initParams()
{
	mQuat.set(SMS_Eular2Quat(mRotation));
	mMouthJointIndex
	    = (u16)getModel()->getModelData()->getJointName()->getIndex(
	        "jnt_mouth_up");
}

void TTabePuku::perform(u32 cue, JDrama::TGraphics* graphics)
{
	mMouthHit->perform(cue, graphics);
	TSmallEnemy::perform(cue, graphics);
}

// TODO: 81.7%. checkHitActors() is instruction-identical apart from the
// actor-type compare (see its body) and the register/frame fallout.
void TTabePuku::control()
{
	TLiveActor::control();
	mMouthHit->checkHitActors();
	updateSound();
}

// UNUSED, 0x110 in the map.
void TTabePuku::updateSound()
{
	if (isBiting())
		gpMSound->startSoundActor(MSD_SE_EN_TOBIPUKU_CHEW, &mPosition);
}

void TTabePuku::bind()
{
	mMouthHit->updateObjCollision();
	mMouthHit->updateTerrainCollsion();
	mMouthHit->bind();

	mLinearVelocity = mMouthHit->mVelocity;
	mTouchedWall    = mMouthHit->mTouchedWall;

	int airborne = mMouthHit->mAirborne;
	if (airborne)
		onLiveFlag(LIVE_FLAG_AIRBORNE);
	else
		offLiveFlag(LIVE_FLAG_AIRBORNE);

	mGroundPlane  = mMouthHit->mGroundPlane;
	mGroundHeight = mMouthHit->mGroundHeight;
}

void TTabePuku::calcRootMatrix()
{
	if (isTaken()) {
		TSpineEnemy::calcRootMatrix();
		return;
	}

	JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
	    mtx;
	// setQT() is the one-line forwarder that keeps setQuat() a `bl` here; at
	// depth 1 MWCC expands setQuat whatever way it is spelled.
	mtx.setQT(mQuat, mPosition);

	getModel()->setBaseScale(mScaling);

	getModel()->setBaseTRMtx(mtx);

	emitEffects();
}

// UNUSED, 0x134 in the map: the bubble trail out of the mouth joint. Deeper
// water gives it a longer life, and the attack nerve thickens it.
void TTabePuku::emitEffects()
{
	JPABaseEmitter* emitter = SMS_EasyEmitParticle(
	    PARTICLE_MS_PUKU_AWA, getModel()->getAnmMtx(mMouthJointIndex), this,
	    JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
	if (!emitter)
		return;

	f32 depth = -mPosition.y / 100.0f;
	if (depth <= 0.0f)
		depth = 0.0f;

	int life = (int)depth * 20 + 2;
	if (life > 200)
		life = 200;
	emitter->setLifeTime(life);

	if (isAttacking())
		emitter->setRate(0.1f);
}

BOOL TTabePuku::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		return FALSE;
	default:
		return TSmallEnemy::receiveMessage(sender, message);
	}
}

// TODO: the frame is exact with one of the two param reads raw (either one
// measures the same; Z chosen arbitrarily). What remains is setQuat's FPR
// scheduling in JGRotation3.hpp, the known regswap there.
MtxPtr TTabePuku::getTakingMtx()
{
	mTakingMtx.setQuat(mQuat);

	JGeometry::TVec3<f32> zdir;
	mTakingMtx.getZDir(zdir);
	JGeometry::TVec3<f32> ydir;
	mTakingMtx.getYDir(ydir);

	JGeometry::TVec3<f32> mouth;
	mouth.scaleAdd(getSaveParams()->mCorrectZ.value, zdir, mPosition);
	mouth.scaleAdd(getSaveParams()->getCorrectY(), ydir, mouth);
	mTakingMtx.setTrans(mouth);

	return mTakingMtx;
}

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

void TTabePuku::behaveToWater(THitActor* water) { }

void TTabePuku::forceKill() { }

bool TTabePuku::isFindMario(f32 rate) { return isFindMarioFromParam(rate); }

bool TTabePuku::doKeepDistance() { return !(isAttacking() || isBiting()); }

// UNUSED, 0x1c4 in the map: Mario got too high, too far from the goal, or the
// puku has wandered outside the territory around its graph.
// TODO: ours is 0x1f0 and the Attack nerve stalls at 82.6% for the same
// reason: retail calls TVec3::sub()/dot()/TUtil<f32>::sqrt() out of line here
// and our build expands sub(). That is the open per-call-site inlining problem
// in docs/catalog/codegen-tells.md, not a shape difference.
bool TTabePuku::isMissMario() const
{
	if (fabsf(gpMarioPos->y - mPosition.y)
	    > getSaveParams()->getSLGiveUpHeight())
		return true;

	f32 giveUpLength = getSaveParams()->mSLGiveUpLength.get();
	if (TabePukuLength(unk104.getPoint() - mPosition) > giveUpLength)
		return true;

	JGeometry::TVec3<f32> onLink(
	    // TODO: TSpineEnemy::getTracer() has no const overload, so this reads
	    // the member directly. Adding one is a shared-header change.
	    unk124->getGraph()->getNearestPosOnGraphLink(mPosition));
	onLink.sub(mPosition);

	f32 range = getSaveParams()->getTerritoryRange();
	if (range * range <= onLink.squared())
		return true;
	return false;
}

// UNUSED, 0x3c in the map.
bool TTabePuku::isTouchedPlane() const
{
	return !isAirborne() || mTouchedWall;
}

// UNUSED, 0xac in the map.
bool TTabePuku::isGraphWander() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveTabePukuGraphWander::theNerve()
	    || nerve == &TNerveTabePukuRecoverGraph::theNerve();
}

// UNUSED, 0x90 in the map.
bool TTabePuku::isAttacking() const
{
	return mSpine->getLatestNerve() == &TNerveTabePukuAttack::theNerve();
}

// UNUSED, 0xd0 in the map.
bool TTabePuku::isBiting() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveTabePukuBite::theNerve()
	    || nerve == &TNerveTabePukuDive::theNerve()
	    || nerve == &TNerveTabePukuDrag::theNerve();
}

// UNUSED, 0xb8 in the map: swim at the current path node, biased by an offset
// the nerves pick (straight at it while wandering, above it while chasing).
void TTabePuku::swimToCurPathNode(const JGeometry::TVec3<f32>& offset)
{
	JGeometry::TVec3<f32> dir(getUnk104().getPoint());
	dir.sub(mPosition);
	dir.add(offset);
	swimTo(dir);
}

// UNUSED, 0x4 in the map: empty in retail too.
void TTabePuku::doBite() { }

// UNUSED, 0x68 in the map.
void TTabePuku::prepareDive()
{
	mDiveStartY = mPosition.y;
	setBckAnm(TABEPUKU_ANM_SWIM);
	getMActor()->getFrameCtrl(ANM_TYPE_BCK)->setRate(2.0f
	                                                 * SMSGetAnmFrameRate());
	mMarchSpeed = getSaveParams()->getDiveSpeed();
}

// UNUSED, 0xbc in the map.
bool TTabePuku::doDive()
{
	swimTo(JGeometry::TVec3<f32>(0.0f, mGroundHeight - mPosition.y, 0.0f));

	if (mPosition.y - mDiveStartY < -getSaveParams()->mApartHeight.get()
	    || mPosition.y - mGroundHeight < 200.0f || !isAirborne())
		return true;

	return false;
}

// UNUSED, 0x17c in the map: pick a random horizontal direction to drag Mario
// in and head off in it from where the puku is now.
void TTabePuku::prepareDrag()
{
	mDragDir.set(0.0f, 0.0f, 1.0f);

	JGeometry::TQuat4<f32> spin;
	spin.setEulerY(MsRandF() * 6.2831855f);
	spin.rotate(mDragDir, mDragDir);

	setGoalPath(mPosition);
	mMarchSpeed = getSaveParams()->getDiveSpeed();
}

// UNUSED, 0x12c in the map.
bool TTabePuku::doDrag()
{
	if (!mTouchedWall && isAirborne()) {
		// Written inverted because retail branches on a bare `bge`; the
		// direct `length() <= getDragLength()` adds a cror.
		if (!(getSaveParams()->getDragLength()
		      < TabePukuLength(unk104.getPoint() - mPosition)))
			return false;
	}

	detach();
	return true;
}

// TODO: 94.0%, and all of it is a 0x80 frame gap plus the float register
// numbering that follows from it. Every instruction matches.
void TTabePuku::swimTo(const JGeometry::TVec3<f32>& dir)
{
	JGeometry::TVec3<f32> d(dir);

	if (JGeometry::TUtil<f32>::epsilonEquals(0.0f, dir.squared(),
	        JGeometry::TUtil<f32>::epsilon())) {
		// setMomentumFromQuat() spelled out: retail inlines
		// MsGetRotFromZaxisY() at both of these sites, which only happens with
		// the body one level shallower, so the UNUSED helper is not what this
		// function calls.
		JGeometry::TVec3<f32> forward;
		mQuat.getZDir(forward);
		forward.scale(mMarchSpeed);

		JGeometry::TVec3<f32> velocity = mVelocity;
		velocity.scale(getSaveParams()->getWaterFric());
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
		target.setRotate(zaxis, d, JGeometry::TUtil<f32>::one());

	mQuat.slerp(target, getSaveParams()->getTurnSlerpRate());
	mQuat.normalize();

	// setMomentumFromQuat() spelled out again, for the same reason.
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	forward.scale(mMarchSpeed);

	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(getSaveParams()->getWaterFric());
	velocity.add(forward);
	mVelocity = velocity;

	mRotation.y = MsGetRotFromZaxisY(velocity);
}

// UNUSED, 0x1d4 in the map: swim along the quaternion's forward axis, with the
// old velocity damped by the water friction.
void TTabePuku::setMomentumFromQuat()
{
	JGeometry::TVec3<f32> forward;
	mQuat.getZDir(forward);
	forward.scale(mMarchSpeed);

	JGeometry::TVec3<f32> velocity = mVelocity;
	velocity.scale(getSaveParams()->getWaterFric());
	velocity.add(forward);
	mVelocity = velocity;

	mRotation.y = MsGetRotFromZaxisY(velocity);
}

// UNUSED, 0x38 in the map.
void TTabePuku::detach()
{
	SMS_SendMessageToMario(this, HIT_MESSAGE_UNK8);
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
		puku->setBckAnm(TABEPUKU_ANM_SWIM);
		puku->mMarchSpeed = puku->getSaveParams()->mMarchSpeed.value;
	}

	if (puku->isReachedToGoal())
		puku->goToRandomNextGraphNode();

	if (puku->isFindMario(1.0f)) {
		spine->pushAfterCurrent(&TNerveTabePukuFound::theNerve());
		return TRUE;
	}

	puku->swimToCurPathNode(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	return FALSE;
}

// Binding level worth +8 of low region, landing
// TNerveTabePukuFound::execute's frame at 0x90 (batch 121).
static inline TTabePukuParams* TabePukuGetSaveParams(const TTabePuku* p)
{
	TTabePukuParams* saveParams = p->getSaveParams();
	return saveParams;
}

DEFINE_NERVE(TNerveTabePukuFound, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->setBckAnm(TABEPUKU_ANM_SEARCH);
		puku->mMarchSpeed = 0.0f;
	}

	// setMomentumFromQuat() spelled out, as in swimTo().
	JGeometry::TVec3<f32> forward;
	puku->mQuat.getZDir(forward);
	forward.scale(puku->mMarchSpeed);

	JGeometry::TVec3<f32> velocity = puku->mVelocity;
	velocity.scale(TabePukuGetSaveParams(puku)->getWaterFric());
	velocity.add(forward);
	puku->mVelocity = velocity;

	puku->mRotation.y = MsGetRotFromZaxisY(velocity);

	if (puku->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveTabePukuAttack::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveTabePukuRecoverGraph, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->getTracer()->reset();
		puku->getTracer()->reset2();
		puku->goToShortestNextGraphNode();
		puku->mMarchSpeed = ((TTabePukuParams*)puku->getSaveParam())->mMarchSpeed.get();
	}

	if (puku->isReachedToGoal()) {
		spine->pushAfterCurrent(&TNerveTabePukuGraphWander::theNerve());
		return TRUE;
	}

	// Aim far above the node while stuck on the ground or a wall, so the puku
	// climbs off it before swimming on.
	JGeometry::TVec3<f32> offset;
	if (puku->isTouchedPlane())
		offset.set(0.0f, 10000.0f, 0.0f);
	else
		offset.zero();
	puku->swimToCurPathNode(offset);
	return FALSE;
}

DEFINE_NERVE(TNerveTabePukuAttack, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();

	if (spine->getTime() == 0) {
		puku->setBckAnm(TABEPUKU_ANM_CHASE);
		puku->setGoalPathMario();
		puku->mMarchSpeed = puku->getSaveParams()->mAttackSpeed.value;
	}

	if (puku->isMissMario() || puku->mTouchedWall) {
		spine->pushAfterCurrent(&TNerveTabePukuRecoverGraph::theNerve());
		return TRUE;
	}

	puku->swimToCurPathNode(JGeometry::TVec3<f32>(0.0f, 150.0f, 0.0f));
	return FALSE;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveTabePukuBite::execute (batch 127).
static inline MSound* TabePukuGetMSound()
{
	MSound* mSound = gpMSound;
	return mSound;
}

DEFINE_NERVE(TNerveTabePukuBite, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();

	puku->doBite();
	puku->setBckAnm(TABEPUKU_ANM_SWIM);
	TabePukuGetMSound()->startSoundActor(MSD_SE_EN_TOBIPUKU_BITE, &puku->mPosition, 0,
	                          nullptr, 0, 4);

	spine->pushAfterCurrent(&TNerveTabePukuDive::theNerve());
	return TRUE;
}

DEFINE_NERVE(TNerveTabePukuDive, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();

	if (!spine->getTime())
		puku->prepareDive();

	if (puku->doDive()) {
		spine->pushAfterCurrent(&TNerveTabePukuDrag::theNerve());
		return TRUE;
	}

	return FALSE;
}

// TODO: 62.2%. Retail's inlined TQuat4::rotate() gives its first TQuat4
// temporary a stack home and calls the empty JGeometry::TVec4<f32>::TVec4()
// out of line (the map emits it weak in this object); our build scalarises
// both temporaries and never emits that constructor, which is a shared-header
// question about JGVec4.hpp, not about this nerve.
DEFINE_NERVE(TNerveTabePukuDrag, TLiveActor)
{
	TTabePuku* puku = (TTabePuku*)spine->getBody();

	if (spine->getTime() == 0) {
		// prepareDrag() spelled out: retail inlines TQuat4::rotate() here,
		// which only happens when the body sits at inline depth 1, so the
		// out-of-line helper cannot be the thing this nerve calls.
		puku->mDragDir.set(0.0f, 0.0f, 1.0f);

		JGeometry::TQuat4<f32> spin;
		spin.setEulerY(MsRandF() * 6.2831855f);
		spin.rotate(puku->mDragDir, puku->mDragDir);

		puku->setGoalPath(puku->mPosition);
		puku->mMarchSpeed = puku->getSaveParams()->getDiveSpeed();
	}

	puku->swimTo(puku->mDragDir);

	if (puku->doDrag()) {
		spine->pushAfterCurrent(&TNerveTabePukuRecoverGraph::theNerve());
		return TRUE;
	}

	return FALSE;
}
