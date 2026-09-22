#include <Enemy/KukkuNerve.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <Animal/AnimalBase.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <Player/MarioAccess.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// The six .bck slots of the tori model in the alphabetical order the model
// data indexes them. Slot 2 has no .bas, and the only name that sorts between
// "tori_down" and "tori_fall_end" is "tori_fall".
// TODO: slot 2's name is deduced from the alphabetical rule, not read from the
// model data.
enum {
	KUKKU_ANM_BACK     = 0,
	KUKKU_ANM_DOWN     = 1,
	KUKKU_ANM_FALL     = 2,
	KUKKU_ANM_FALL_END = 3,
	KUKKU_ANM_HIT      = 4,
	KUKKU_ANM_WAIT     = 5,
};

static const char* tori_bastable[] = {
	"/scene/tori/bas/tori_back.bas",
	"/scene/tori/bas/tori_down.bas",
	nullptr,
	"/scene/tori/bas/tori_fall_end.bas",
	"/scene/tori/bas/tori_hit.bas",
	"/scene/tori/bas/tori_wait.bas",
};

namespace {
// Indexed 1..3, so entry 0 is never read.
const int cDropCoinNumTable[] = { 3, 3, 1, 2 };
}

// UNUSED, 0x70 in the map.
TKukkuBall::TKukkuBall(MActor* actor)
    : THitActor("\x83\x4e\x83\x62\x83\x4e\x8b\xca")
    , mMActor(actor)
    , mFlags(KUKKUBALL_FLAG_DEAD)
    , unk7C(0)
{
}

// Binding level worth +8 of low region, landing TKukkuBall::init's frame at
// 0x88 (batch 121).
static inline J3DModel* KukkuGetModel(const MActor* p)
{
	J3DModel* model = p->getModel();
	return model;
}

// TODO: 99.9%, slot-only: the end()/`what` pair of the push_back sits 4 bytes
// high (0x6c/0x70 against 0x68/0x6c). Inert or worse: the chained search<T>
// receiver, `group->insert(this)`, a `(THitActor*)this` or named `self`
// argument, `image != nullptr`, raw mMActor->getModel(), one combined
// onHitFlag.
void TKukkuBall::init()
{
	initHitActor(0x1000002E, 1, -0x80000000, 30.0f, 30.0f, 0.0f, 0.0f);

	onHitFlag(HIT_FLAG_NO_COLLISION);
	onHitFlag(HIT_FLAG_CANNOT_GET_HIT);

	TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>(
	    "\x93\x47\x83\x4f\x83\x8b\x81\x5b\x83\x76");
	group->getChildren().push_back(this);

	ResTIMG* image = (ResTIMG*)JKRFileLoader::getGlbResource(
	    "/scene/map/pollution/H_ma_rak.bti");
	if (image)
		SMS_ChangeTextureAll(KukkuGetModel(mMActor)->getModelData(),
		                     "K_name_dummy", *image);
}

void TKukkuBall::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mFlags & KUKKUBALL_FLAG_DEAD)
		return;

	if (cue & CUE_CALC_ANIM) {
		JGeometry::TPosition3<
		    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
		    mtx;
		// Spelled out: behind translation() the identity33() expansion would be
		// one level deeper and retail keeps it inline.
		mtx.identity33();
		mtx.setTrans(mPosition);
		mMActor->getModel()->setBaseScale(mScaling);
		MTXCopy(mtx, mMActor->getModel()->getBaseTRMtx());
		mMActor->getModel()->calc();
	}

	if (cue & CUE_MOVE) {
		checkHitActors();
		moveObject();
		bind();
	}

	if (!(mFlags & KUKKUBALL_FLAG_NO_DRAW))
		mMActor->perform(cue, graphics);
}

// UNUSED, 0x3c in the map.
void TKukkuBall::moveObject()
{
	mVelocity.y -= 0.9f;
	mVelocity.scale(0.94f);
}

// UNUSED, 0x128 in the map: the mud ball's own one-step integrator, with a
// ground test that kills it and a wall test that slides it.
void TKukkuBall::bind()
{
	JGeometry::TVec3<f32> pos(mPosition);
	pos.x += mVelocity.x;
	pos.y += mVelocity.y;
	pos.z += mVelocity.z;

	const TBGCheckData* ground;
	f32 groundY
	    = gpMap->checkGround(pos.x, pos.y + mAttackHeight, pos.z, &ground)
	    + 1.0f;
	if (pos.y <= 0.05f + groundY)
		kill();

	gpMap->isTouchedOneWallAndMoveXZ(&pos.x, pos.y + mAttackHeight, &pos.z,
	                                 mAttackRadius);

	mPosition = pos;
}

// UNUSED, 0xc0 in the map.
void TKukkuBall::checkHitActors()
{
	THitActor** end = &mCollisions[mColCount];
	for (THitActor** col = mCollisions; col != end; col++) {
		switch ((*col)->mActorType) {
		case 0x80000001:
			SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
			kill();
			break;
		}
	}
}

// UNUSED, 0x4c in the map.
void TKukkuBall::startToMove(const JGeometry::TVec3<f32>& position,
                             const JGeometry::TVec3<f32>& velocity)
{
	offHitFlag(HIT_FLAG_NO_COLLISION);
	mFlags &= ~KUKKUBALL_FLAG_DEAD;
	mPosition = position;
	mVelocity = velocity;
}

// UNUSED, 0x50 in the map.
void TKukkuBall::kill()
{
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mFlags |= KUKKUBALL_FLAG_DEAD;
	gpPollution->stamp(1, mPosition.x, mPosition.y, mPosition.z, 500.0f);
}

// UNUSED, 0x28 in the map.
bool TKukkuBall::isDead() const
{
	return (mFlags & KUKKUBALL_FLAG_DEAD) && unk7C == 0;
}

// UNUSED, 0x44 in the map. TODO: fabricated; nothing in the retail code
// references this class, so only the three names and their sizes are evidence.
TEnemyCoinUnit::TEnemyCoinUnit(int num)
    : mItemNum(num)
{
	mItems = new TMapObjBase*[num];
}

// UNUSED, 0xa8 in the map. TODO: fabricated, see the constructor.
void TEnemyCoinUnit::init()
{
	for (int i = 0; i < mItemNum; i++)
		mItems[i] = TMapObjBaseManager::newAndRegisterObj("coin");
}

// UNUSED, 0x48 in the map. TODO: fabricated, see the constructor.
TMapObjBase* TEnemyCoinUnit::getUnusedItem()
{
	for (int i = 0; i < mItemNum; i++)
		if (mItems[i]->checkLiveFlag(LIVE_FLAG_DEAD))
			return mItems[i];
	return nullptr;
}

TKukku::TKukku(const char* name)
    : TSmallEnemy(name)
    , mOneUp(nullptr)
{
}

void TKukku::init(TLiveManager* live_manager)
{
	mManager = live_manager;
	mManager->manageActor(this);

	mMActorKeeper = new TMActorKeeper(mManager, 4);
	mMActor       = mMActorKeeper->createMActor("tori.bmd", 0);

	mSpine->initWith(&TNerveKukkuGraphWander::theNerve());

	mOneUp = TMapObjBaseManager::newAndRegisterObj("mushroom1upR");

	initBalls();
	initCollision();
	initAnmSound();
	initParticle();

	mCenterJointIndex
	    = getModel()->getModelData()->getJointName()->getIndex("center");

	reset();
}

// UNUSED, 0xc4 in the map.
void TKukku::initBalls()
{
	for (TKukkuBall** ball = mBalls; ball != &mBalls[3]; ball++) {
		*ball = new TKukkuBall(
		    mMActorKeeper->createMActor("torifun.bmd", 3));
		(*ball)->init();
	}
}

// UNUSED, 0x58 in the map.
void TKukku::initCollision()
{
	initHitActor(0x1000002E, 1, -0x80000000, 30.0f, 30.0f, 100.0f, 100.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

// UNUSED, 0x8c in the map.
void TKukku::initParticle()
{
	SMS_LoadParticle("/scene/tori/jpa/ms_cooc_ase.jpa", 0x18c);
	SMS_LoadParticle("/scene/tori/jpa/ms_cooc_hane.jpa", 0x18d);
}

void TKukku::reset()
{
	mHitTimer         = 0;
	mShootTimer       = 0;
	mGravity          = 0.0f;
	mDroppedCoins     = 0;
	onLiveFlag(LIVE_FLAG_AIRBORNE);
	mScaledBodyRadius = 75.0f;
}

BOOL TKukku::receiveMessage(THitActor* sender, u32 message)
{
	if (checkLiveFlag(LIVE_FLAG_DEAD))
		return FALSE;

	switch (message) {
	case HIT_MESSAGE_TRAMPLE:
	case HIT_MESSAGE_HIP_DROP:
		behaveHitTrample();
		return TRUE;
	default:
		return TSmallEnemy::receiveMessage(sender, message);
	}
}

void TKukku::control()
{
	if (getHitTimer() > 0)
		mHitTimer--;

	TLiveActor::control();
}

void TKukku::calcRootMatrix()
{
	if (mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve()) {
		// Dead: lie flat against whatever it landed on instead of using the
		// spine enemy's upright matrix.
		JGeometry::TVec3<f32> up;
		if (mGroundPlane) {
			// Copying the normal into the local first is what lets the
			// squared length contract into fmadds, as retail does.
			up.set(mGroundPlane->getNormal());
			up.normalize();
		} else {
			up.set(0.0f, 1.0f, 0.0f);
		}

		JGeometry::TQuat4<f32> yaw;
		yaw.setEulerY(0.017453294f * mRotation.y);

		JGeometry::TQuat4<f32> tilt;
		tilt.setRotate(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), up,
		               JGeometry::TUtil<f32>::one());
		tilt.mul(yaw);

		JGeometry::TPosition3<
		    JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > >
		    mtx;
		// setQT() is the one-line forwarder that keeps setQuat() a `bl`.
		mtx.setQT(tilt, mPosition);

		MtxPtr src      = mtx;
		J3DModel* model = getModel();
		MTXCopy(src, model->getBaseTRMtx());
		getModel()->setBaseScale(getScaling());
	} else {
		TSpineEnemy::calcRootMatrix();
	}

	updateEffect();
}

void TKukku::bind() { TLiveActor::bind(); }

void TKukku::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TSmallEnemy::perform(cue, graphics);

	for (TKukkuBall** ball = mBalls; ball != &mBalls[3]; ball++)
		(*ball)->perform(cue, graphics);
}

// UNUSED, 0x130 in the map: the sweat trail while falling and the wing puff
// while flapping backwards.
void TKukku::updateEffect()
{
	if (isFalling())
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x18d, getModel()->getAnmMtx(mCenterJointIndex), 1, this);

	if (getMActor()->checkCurAnm("tori_back", ANM_TYPE_BCK))
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x18c, getModel()->getAnmMtx(mCenterJointIndex), 1, this);
}

void TKukku::behaveToWater(THitActor* water)
{
	if (isFalling())
		return;

	if (isRecoveringGraph())
		return;

	getSpine()->reset();
	getSpine()->setNext(&TNerveKukkuFall::theNerve());
}

// UNUSED, 0x58 in the map.
void TKukku::behaveHitTrample()
{
	mSpine->reset();
	mSpine->setNext(&TNerveSmallEnemyDie::theNerve());
}

// UNUSED, 0x2c8 in the map. TODO: fabricated. The GraphWander nerve holds this
// code inline, and nothing distinguishes a pasted body from a dead helper.
void TKukku::doFlyToCurPathNode()
{
	if (isReachedToGoal()) {
		goToRandomNextGraphNode();
		if (!getMActor()->checkCurAnm("tori_wait", ANM_TYPE_BCK))
			changeBck("tori_wait");
	}

	if (isFindOutMario()) {
		if (getShootTimer() >= 0)
			mShootTimer--;
		else
			shotBall();
	}

	updateRotation();
	mLinearVelocity = calcMomentum(getSaveParams()->getMarchSpeed());
}

// Instruction-exact (99.8%); the residual is a 0x18 frame excess against the
// target's 0x70, which is the same excess the twelve-statement version had.
//
// Retail *calls* this at depth 1 (`mr r3, r30; bl updateRotation`), so by the
// statement budget in docs/catalog/codegen-tells.md its body needed 15 counted
// statements. The three that were missing are the ones written out below, and
// all three are readable off the asm rather than guessed:
//   - `length()` split into `squared()` plus TUtil<f32>::sqrt: the ROM's three
//     `fmuls`/two `fadds`, the `fcmpo 0.0f` guard and the single Newton step
//     are exactly that pair, and the split is also worth 8 bytes of frame.
//   - the two banking factors named per component, because the ROM expands
//     isFalling() twice (two `getLatestNerve` reads and four theNerve()
//     calls) instead of reusing one value.
// Together they take TNerveKukkuRecoverGraph::execute 0.0 -> 67.7% and
// TNerveKukkuGraphWander::execute 42.0 -> 92.9%.
//
// Measured and rejected: naming `0.5f * dist` inside the branch instead of one
// of the above (14 statements, both nerves back to 0.0/42.0); splitting the
// two parameter fetches into declaration plus assignment (no effect at all);
// keeping `length()` with the two bank locals (nerves back to 0.0/42.0 *and*
// frame 0x90).
//
// The last 0x18 of frame: each `TParamRT::get()` reference temporary is 0x10
// of pool, so retail read one speed raw and the other through the plain
// cast (0x88 -> 0x78; both raw is 0x68), and the two bank factors are one
// reassigned local (the second named `f32` was the extra 4-byte slot).
void TKukku::updateRotation()
{
	JGeometry::TVec3<f32> toGoal(getUnkF4().getPoint());
	toGoal.sub(mPosition);

	f32 distSq = toGoal.squared();
	f32 dist   = JGeometry::TUtil<f32>::sqrt(distSq);
	if (dist < 100.0f)
		return;

	f32 marchSpeed = getSaveParams()->mMarchSpeed.value;
	f32 turnSpeed  = ((TKukkuParams*)getSaveParam())->getTurnSpeed();

	if (dist <= 2.0f * calcMinimumTurnRadius(marchSpeed, turnSpeed))
		turnSpeed = calcTurnSpeedToReach(marchSpeed, 0.5f * dist);

	TAnimalBase::getRotationFlyToDir(&mRotation, toGoal, marchSpeed,
	                                 turnSpeed);

	// A falling gull keeps the yaw it had but loses its banking; retail
	// evaluates isFalling() once per component.
	f32 bank = isFalling() ? 0.0f : 1.0f;
	mRotation.x *= bank;
	bank = isFalling() ? 0.0f : 1.0f;
	mRotation.z *= bank;
}

// 96.3%, frame exact at 0x78, all 71 opcodes in place.
//
// Retail *calls* this at every site (`addi r4, r30, 0; addi r3, r1, 0x30; bl
// calcMomentum`), so by the depth-1 statement budget its body had to carry 15
// or more counted statements -- and a call to TQuat4<f32>::rotate contributes
// exactly one, because a callee's own inlined statements are free. Writing
// rotate's fifteen statements out here (the same expansion, so not one opcode
// moves) is what lifts the body over the budget, and it is what restores the
// three `bl`s: TNerveKukkuRecoverGraph::execute 67.7 -> 99.8%,
// TNerveKukkuGraphWander::execute 92.9 -> 94.5%.
//
// The frame ladder measured while landing 0x78, relative to this body:
//   both TQuat4 temporaries as objects   0x98   (the header's literal body)
//   q2 scalarised                        0x88
//   q scalarised                         0x70
//   both scalarised                      0x70
//   q an object, vx/vy/vz named          0x80
//   q an object, vx/vy/vz read in place  0x78   <- retail
// So retail kept the first temporary as a real TQuat4 and read the vector
// components straight out of `velocity` instead of naming them, exactly the
// opposite of the shape JGQuat4.hpp's own rotate() uses.
//
// TODO: the residual is volatile-FPR numbering only (52 markers, no opcode or
// operand-order difference) plus the low region's internal order -- retail
// parks SMS_Eular2Quat's return slot at 0x38 with 44 bytes of temporaries
// below it, we park it at 0x1c with 48 bytes split around it. Both totals are
// 0x78, so no frame lever applies; this is the known-open volatile-FPR class.
JGeometry::TVec3<f32> TKukku::calcMomentum(f32 speed)
{
	JGeometry::TQuat4<f32> quat = SMS_Eular2Quat(mRotation);
	JGeometry::TVec3<f32> velocity(0.0f, 0.0f, speed);

	f32 w = quat.w;
	f32 z = quat.z;
	f32 y = quat.y;
	f32 x = quat.x;

	// clang-format off
	JGeometry::TQuat4<f32> q;
	q.x =  w *  0 + y * velocity.z - z * velocity.y + w * velocity.x;
	q.y = -x * velocity.z + y *  0 + z * velocity.x + w * velocity.y;
	q.z =  x * velocity.y - y * velocity.x + z *  0 + w * velocity.z;
	q.w = -x * velocity.x - y * velocity.y - z * velocity.z + w *  0;

	f32 rx =  q.x *  w + q.y * -z - q.z * -y + q.w * -x;
	f32 ry = -q.x * -z + q.y *  w + q.z * -x + q.w * -y;
	f32 rz =  q.x * -y - q.y * -x + q.z *  w + q.w * -z;
	// clang-format on

	velocity.set(rx, ry, rz);
	return velocity;
}

// UNUSED, 0xa0 in the map. TODO: fabricated, see doFlyToCurPathNode().
void TKukku::doRecoverToCurPathNode()
{
	updateRotation();
	mLinearVelocity = calcMomentum(getSaveParams()->getMarchSpeed());
}

// UNUSED, 0xd8 in the map. TODO: dead and fabricated. "Habataki" is flapping,
// and mHabatakiTimer is what the recovery nerves count against.
void TKukku::doHabataki()
{
	if (getSaveParams()->getHabatakiTimer() < mSpine->getTime())
		return;

	decideFlyingAnm();
	mLinearVelocity = calcMomentum(getSaveParams()->getMarchSpeed());
}

// UNUSED, 0x7c in the map.
void TKukku::decideFlyingAnm()
{
	if (getMActor()->checkCurAnm("tori_back", ANM_TYPE_BCK))
		changeBck("tori_back");
	else
		changeBck("tori_wait");
}

// UNUSED, 0x20c in the map: spit one of the three mud balls straight up, with
// the joint lookup the original left in and never used.
void TKukku::shotBall()
{
	TKukkuBall* ball = getUnusedBall();
	if (!ball)
		return;

	JGeometry::TVec3<f32> velocity(0.0f, 1.0f, 0.0f);
	velocity.setLength(getSaveParams()->getShootSpeed());

	getModel()->getModelData()->getJointName()->getIndex("null_osen");

	JGeometry::TVec3<f32> position(mPosition);
	ball->startToMove(position, velocity);

	mShootTimer = getSaveParams()->getShootInterval();

	gpMSound->startSoundActor(MSD_SE_EN_TORI_SHIT, &mPosition, 0, nullptr, 0,
	                          4);
}

// TODO: 80.7%. Retail keeps both quaternions scalar-replaced in FPRs (only
// sinf/cosf pairs survive of setEulerY/X) and calls TVec3::set<f>(f, f, f)
// out of line for `forward` (the unit's missing weak set<f>, depth 4), so
// its frame is 0x158 against our 0x1e8. Inert or worse: one-argument
// rotate(), rotating `forward` straight into `velocity`, `forward.set(...)`.
void TKukku::dropCoins()
{
	if (mDroppedCoins > 10)
		return;

	// The eleventh drop is the 1UP the gull was carrying.
	if (mDroppedCoins == 10 && mOneUp) {
		mDroppedCoins++;
		mOneUp->appear();
		mOneUp->JSGSetTranslation(getPosition());
		// Retail reloads mOneUp before each of the three calls above (they
		// clobber it) but holds it across the three velocity stores and the
		// flag clear, which needs a pointer local declared exactly here: the
		// member spelling reloads between the stores, and .set(0,0,0) turns
		// the first store into an `stfsu` that costs the register.
		TMapObjBase* oneUp = mOneUp;
		oneUp->mVelocity.x = 0.0f;
		oneUp->mVelocity.y = 0.0f;
		oneUp->mVelocity.z = 0.0f;
		oneUp->offLiveFlag(LIVE_FLAG_UNK10);
		return;
	}

	int index = (int)(4.0f * MsRandF());
	if (index < 1)
		index = 1;
	else if (index > 3)
		index = 3;
	int coinNum = cDropCoinNumTable[index];

	JGeometry::TQuat4<f32> spin;
	spin.setEulerY(6.2831855f / (f32)coinNum);

	JGeometry::TQuat4<f32> pitch;
	pitch.setEulerX(3.1415927f * getSaveParams()->getDropAngleX());

	f32 dropSpeed = getSaveParams()->getDropSpeed();
	JGeometry::TVec3<f32> forward(dropSpeed * JMASin(mRotation.y), 0.0f,
	                              dropSpeed * JMACos(mRotation.y));

	JGeometry::TVec3<f32> velocity(forward);
	pitch.rotate(velocity, velocity);
	spin.rotate(velocity, velocity);

	for (int i = 0; i < coinNum; i++) {
		TMapObjBase* coin = gpItemManager->makeObjAppear(0x2000000E);
		if (!coin)
			break;

		coin->appear();
		coin->JSGSetTranslation(getPosition());
		coin->mVelocity.set(velocity);
		coin->offLiveFlag(LIVE_FLAG_UNK10);

		if (++mDroppedCoins == 10)
			break;

		spin.rotate(velocity, velocity);
	}
}

// UNUSED, 0xac in the map.
bool TKukku::isFalling() const
{
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	return nerve == &TNerveKukkuFall::theNerve()
	    || nerve == &TNerveKukkuPostFall::theNerve();
}

// UNUSED, 0x98 in the map.
bool TKukku::isRecoveringGraph() const
{
	if (mSpine->getLatestNerve() != &TNerveKukkuRecoverGraph::theNerve())
		return false;
	return true;
}

// UNUSED, 0x4c in the map. TODO: dead and fabricated.
bool TKukku::isDying() const
{
	if (mSpine->getLatestNerve() == &TNerveSmallEnemyDie::theNerve())
		return true;
	return false;
}

// UNUSED, 0xb4 in the map: Mario is within the search range, measured flat.
bool TKukku::isFindOutMario() const
{
	JGeometry::TVec3<f32> toMario(*gpMarioPos);
	toMario.sub(mPosition);
	toMario.y = 0.0f;

	f32 range = getSaveParams()->getSearchRange();
	return toMario.squared() < range * range;
}

// UNUSED, 0x38 in the map.
void TKukku::changeBck(const char* name)
{
	getMActor()->setBck(name);
	setCurAnmSound();
}

void TKukku::setDeadAnm() { changeBck("tori_down"); }

void TKukku::setAfterDeadEffect()
{
	TSmallEnemy::setAfterDeadEffect();
	gpPollution->stamp(((TSmallEnemyManager*)mManager)->getUnk58(),
	                   mPosition.x, mPosition.y, mPosition.z, 1000.0f);
}

// UNUSED, 0x8 in the map. TODO: two instructions, so it returns a constant,
// but nothing in the binary says which one.
f32 TKukku::getWaterDamageRate() const { return 1.0f; }

// UNUSED, 0x38 in the map.
f32 TKukku::getWaterPowerY() const
{
	return getSaveParams()->getWaterPowerY();
}

// UNUSED, 0x4c in the map.
TKukkuBall* TKukku::getUnusedBall()
{
	for (TKukkuBall** ball = mBalls; ball != &mBalls[3]; ball++)
		if ((*ball)->isDead())
			return *ball;
	return nullptr;
}

// UNUSED, 0x250 in the map.
TKukkuParams::TKukkuParams(const char* prm)
    : TSmallEnemyParams(prm)
    , PARAM_INIT(mMarchSpeed, 3.0f)
    , PARAM_INIT(mTurnSpeed, 0.2f)
    , PARAM_INIT(mWaterPowerY, 12.0f)
    , PARAM_INIT(mShootSpeed, 1.0f)
    , PARAM_INIT(mShootInterval, 60)
    , PARAM_INIT(mSearchRange, 800.0f)
    , PARAM_INIT(mHabatakiTimer, 45)
    , PARAM_INIT(mAirFric, 0.97f)
    , PARAM_INIT(mUpperVelocityY, 0.0f)
    , PARAM_INIT(mDropSpeed, 5.0f)
    , PARAM_INIT(mDropAngleX, -0.25f)
{
	TParams::load(mPrmPath);
}

TKukkuManager::TKukkuManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TKukkuManager::load(JSUMemoryInputStream& stream)
{
	TKukkuParams* params = new TKukkuParams("/enemy/kukku.prm");
	unk38                = params;

	// The gull's collision is the same for every map, so the .prm never gets
	// to override these four.
	params->mSLAttackRadius.set(30);
	params->mSLAttackHeight.set(30);
	params->mSLDamageRadius.set(100);
	params->mSLDamageHeight.set(100);
	params->mSLBodyRadius.set(5.0f);

	TSmallEnemyManager::load(stream);
}

// UNUSED, 0x4 in the map.
void TKukkuManager::initCoins() { }

void TKukkuManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "tori.bmd", 0x10210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

const char** TKukku::getBasNameTable() const { return tori_bastable; }

// TODO: 94.5%. The residue is one refusal split that has no lever: retail
// *inlines* calcMomentum here (and then calls SMS_Eular2Quat, TVec4's copy
// constructor, TVec3::set<f> and TQuat4::rotate out of line from inside the
// expansion) while it *calls* calcMomentum from TNerveKukkuRecoverGraph, whose
// source statement is character-for-character the same. Since a body MWCC
// inlines at depth 1 is by definition under the allowance, the two sites
// cannot both be explained by the statement budget; the 15-statement body in
// calcMomentum() is the side that wins (RecoverGraph 67.7 -> 99.8, this nerve
// 92.9 -> 94.5), and its cost is that TVec3::set<f> is no longer instantiated
// in this object -- retail's only two `bl`s to it are here and in dropCoins().
DEFINE_NERVE(TNerveKukkuGraphWander, TLiveActor)
{
	TKukku* kukku = (TKukku*)spine->getBody();

	if (spine->getTime() == 0) {
		kukku->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		kukku->getTracer()->reset();
		kukku->goToShortestNextGraphNode();
		kukku->decideFlyingAnm();
	}

	if (kukku->isReachedToGoal()) {
		kukku->goToRandomNextGraphNode();
		if (!kukku->getMActor()->checkCurAnm("tori_wait", ANM_TYPE_BCK))
			kukku->changeBck("tori_wait");
	}

	if (kukku->isFindOutMario()) {
		if (kukku->getShootTimer() >= 0)
			kukku->mShootTimer--;
		else
			kukku->shotBall();
	}

	kukku->updateRotation();
	kukku->mLinearVelocity
	    = kukku->calcMomentum(kukku->getSaveParams()->getMarchSpeed());
	return FALSE;
}

// TODO: dead in retail (0xfc, with its vtable and destructor UNUSED too), so
// this body is a guess from the name and the tori_hit animation.
DEFINE_NERVE(TNerveKukkuHit, TLiveActor)
{
	TKukku* kukku = (TKukku*)spine->getBody();

	if (spine->getTime() == 0)
		kukku->changeBck("tori_hit");

	if (kukku->checkCurAnmEnd(ANM_TYPE_BCK)) {
		spine->pushAfterCurrent(&TNerveKukkuFall::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKukkuFall, TLiveActor)
{
	TKukku* kukku = (TKukku*)spine->getBody();

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_wait");
		J3DFrameCtrl* ctrl = kukku->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
		ctrl->setRate(2.0f * SMSGetAnmFrameRate());
		kukku->setVelocity(JGeometry::TVec3<f32>(
		    0.0f, -kukku->getSaveParams()->getWaterPowerY(), 0.0f));
		kukku->dropCoins();
	}

	// checkLiveFlag(), not isAirborne(): retail branches on the flag directly
	// instead of materialising isAirborne()'s BOOL.
	if (!kukku->checkLiveFlag(LIVE_FLAG_AIRBORNE)) {
		SMS_EasyEmitParticle((E_SMS_EFFECT_ONETIME_NORMAL)0xa1,
		                     &kukku->mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		SMS_EasyEmitParticle((E_SMS_EFFECT_ONETIME_NORMAL)0xa2,
		                     &kukku->mPosition, nullptr,
		                     JGeometry::TVec3<f32>(1.0f, 1.0f, 1.0f));
		spine->pushAfterCurrent(&TNerveSmallEnemyDie::theNerve());
		return TRUE;
	}

	JGeometry::TVec3<f32> velocity(kukku->mVelocity);
	velocity.scale(kukku->getSaveParams()->getAirFric());

	bool landed = false;
	velocity.y += kukku->getSaveParams()->getUpperVelocityY();
	if (velocity.y > -0.1f) {
		landed     = true;
		velocity.y = 0.0f;
	}
	kukku->setVelocity(velocity);

	if (landed) {
		spine->pushAfterCurrent(&TNerveKukkuRecoverGraph::theNerve());
		return TRUE;
	}

	return FALSE;
}

DEFINE_NERVE(TNerveKukkuPostFall, TLiveActor)
{
	TKukku* kukku = (TKukku*)spine->getBody();

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_back");
		kukku->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	}

	if (kukku->checkCurAnmEnd(ANM_TYPE_BCK)
	    && kukku->getSaveParams()->getHabatakiTimer() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return TRUE;
	}

	return FALSE;
}

// TODO: the frame is exact (0x60 -> 0x58 by reading the habataki timer
// through the plain cast: `TParamRT::get()` behind getSaveParams() costs a
// reference temporary), but calcMomentum's return slot sits at 0x38 against
// retail's 0x30 with the velocity temporary exact at 0x44. Tried and inert:
// a raw or cast march-speed read, a named result copied into
// mLinearVelocity, `.set()`, a named zero vector for setVelocity (block or
// function scope); `!spine->getTime()` moves the result to 0x34 but the
// velocity temporary to 0x40.
DEFINE_NERVE(TNerveKukkuRecoverGraph, TLiveActor)
{
	TKukku* kukku = (TKukku*)spine->getBody();

	if (spine->getTime() == 0) {
		kukku->changeBck("tori_back");
		kukku->setVelocity(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
	}

	if (((TKukkuParams*)kukku->getSaveParam())->getHabatakiTimer() < spine->getTime()) {
		spine->pushAfterCurrent(&TNerveKukkuGraphWander::theNerve());
		return TRUE;
	}

	kukku->updateRotation();
	kukku->mLinearVelocity
	    = kukku->calcMomentum(kukku->getSaveParams()->getMarchSpeed());
	return FALSE;
}
