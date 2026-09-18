#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/JMath.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TWalkerEnemyParams::TWalkerEnemyParams(const char* path)
    : TSmallEnemyParams(path)
    , PARAM_INIT(mSLZigzagCycle, 1000.0f)
    , PARAM_INIT(mSLZigzagAngle, 30.0f)
    , PARAM_INIT(mSLMarchSpeedLow, 0.8f)
    , PARAM_INIT(mSLMarchSpeedHigh, 1.5f)
    , unk324(0.0f, 1.0f)
{
	TParams::load(mPrmPath);
	unk324.mMin = mSLMarchSpeedLow.get();
	unk324.mMax = mSLMarchSpeedHigh.get();
}

TWalkerEnemy::TWalkerEnemy(const char* name)
    : TSmallEnemy(name)
{
}

void TWalkerEnemy::init(TLiveManager* param_1)
{
	TSmallEnemy::init(param_1);
	mBinder = new TWalker;
	getWalker()->reset();
	getWalker()->unk18 = 150.0f;

	mMarchSpeed = getSaveParam2()->unk324.rand();
	getSpine()->initWith(&TNerveWalkerGenerate::theNerve());
}

// TODO: park for a shared header. `TPathNode::getPoint()` spelled with a
// direct `unk0->mPosition` read instead of `unk0->getPosition()` is what this
// function's 8-byte temp pool wants (frame 0x20 vs 0x30, 100% exact); the same
// spelling also closes `TFireWanwan::isMissMario` and `TNerveBPTouchDown`, but
// costs `TNerveBPTakeOff` and ~20 small fuzzy drops, so it cannot go into
// PathNode.hpp unilaterally. Parked TU-locally here.
static inline const JGeometry::TVec3<f32>& WalkerPathPoint(const TPathNode& node)
{
	if (node.unk0 != 0)
		return node.unk0->mPosition;

	return node.unk4;
}

// TODO: fake
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * JMACos(theta);
	f32 s = radius * JMASin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

// TODO: every instruction matches; the frame is 0x60 against retail's 0x88, a
// uniform 40 bytes of dead pool below the `polarXZ` return temp (retail 0x50,
// ours 0x30). The same 40 bytes are missing from TNerveWalkerEscape::execute,
// and both functions are the two places in this TU with a bare
// `getSaveParam();` statement whose result is discarded -- MWCC keeps the
// virtual call and dead-strips the load, so retail's real statement there can
// be anything whose values are all dead. 40 = 32 of pool + 8 of padding, which
// is the signature of two 16-byte non-trivial class locals (TPathNode is 16 and
// has a user constructor) inside an inlined callee. Finding that callee is one
// research item for both functions; see docs/catalog/frame-gaps.md, "The dead
// low region".
// Measured this batch: the gap is not only 32 bytes of pool below the
// `polarXZ` temp (ours 0x30, retail 0x50) but also a 4-byte pad on each
// 12-byte vector slot -- retail strides 0x50 -> 0x60 -> 0x70 where we pack
// 0x30 -> 0x3c -> 0x48. A 16-byte stride on a TVec3 is the signature of a
// *callee's* class local (sizeof rounded to 8) rather than a caller temp, so
// all four slots, the two live ones included, belong to inlined callees;
// `local` being a named local here is itself wrong. TNerveWalkerEscape shows
// the same 32+8 split around the `TSolidStack<TPathNode>::pop()` temp
// (ours 0x24, retail 0x44, with 16 rather than 8 free bytes above it).
void TWalkerEnemy::moveObject()
{
	if (!mGroundPlane->checkFlag(BG_CHECK_FLAG_ILLEGAL)
	    && (mInstanceIndex & 0xF) == (gpMarDirector->unk58 & 0xF)) {
		doShortCut();
	}

	TSmallEnemy::moveObject();

	if (getWalker()->getUnk1C()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyJump::theNerve()) {

		// TODO: some order of inlines should be used instead of tmps
		f32 yAngle = mRotation.y;
		f32 f      = getSaveParam2()->unk324.mMax;

		JGeometry::TVec3<f32> local = polarXZ(yAngle, f);

		getSaveParam();
		mVelocity.x = local.x;
		mVelocity.z = local.z;
		mSpine->pushNerve(&TNerveSmallEnemyJump::theNerve());
		onLiveFlag(LIVE_FLAG_AIRBORNE);
		mPosition.y += 5.0f;
	}
}

void TWalkerEnemy::attackToMario()
{
	sendAttackMsgToMario();
	if (mSpine->getCurrentNerve() != &TNerveSmallEnemyJump::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveSmallEnemyDie::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveWalkerGenerate::theNerve()) {
		mSpine->pushNerve(&TNerveWalkerPostAttack::theNerve());
		mSpine->pushNerve(&TNerveSmallEnemyJump::theNerve());
	}
}

void TWalkerEnemy::reset()
{
	unk124->reset2();
	unk124->reset();
	TSmallEnemy::reset();

	mMarchSpeed = getSaveParam2()->unk324.rand();

	getWalker()->reset();
	getSpine()->reset();
	mSpine->setNext(mSpine->getDefault());

	// The node is a named local and the `getSpine()` above carries the +4 of
	// pool that puts it at 0x38: `setGoalPathMario()` lands the temp at 0x20,
	// the named node alone at 0x34, and routing `setNext` through `getSpine()`
	// as well overshoots the frame to 0x68.
	TPathNode node((THitActor*)gpMarioAddress);
	setGoalPath(node);
}

void TWalkerEnemy::walkBehavior(int param_1, float param_2)
{
	f32 fVar1 = param_2 * unk158;
	switch (param_1) {
	case 0:
		zigzagToCurPathNode(mMarchSpeed * fVar1, mTurnSpeed * fVar1,
		                    getSaveParam2()->mSLZigzagCycle.get(),
		                    getSaveParam2()->mSLZigzagAngle.get());
		break;

	case 1:
	case 2:
		walkToCurPathNode(mMarchSpeed * fVar1, mTurnSpeed * fVar1, 0.0f);
		break;

	case 3:
		walkToCurPathNode(0.0f, mTurnSpeed * fVar1, 0.0f);
		break;

	case 4:
		walkToCurPathNode(mMarchSpeed * fVar1, 0.0f, 0.0f);
		break;

	case 5:
		break;
	}
}

// Binding level worth +8 of low region, landing
// TWalkerEnemy::behaveToFindMario's frame at 0x50 (batch 124).
static inline bool WalkerEnemyCheckUnk150(const TWalkerEnemy* p, u32 i)
{
	bool unk150 = p->checkUnk150(i);
	return unk150;
}

// TODO: frame-exact at 0x50, but the `setGoalPath` TPathNode temp sits at 0x2c
// and retail's at 0x34, i.e. eight bytes of pool short with the frame already
// right (there are twelve bytes of slack above the temp, so a +8 pool item
// costs no frame). Measured, all with `WalkerEnemyCheckUnk150` in place:
// `setGoalPathMario()` 0x2c; a named `TPathNode node(...)` + `setGoalPath(node)`
// 0x38; the same as an unnamed `setGoalPath(TPathNode(...))` temp 0x38 (so the
// spelled-out constructor is +12, not +8); dropping the binding out of
// `WalkerEnemyCheckUnk150` is -8 on both temp and frame; `getSpine()` on the
// three `pushAfterCurrent`s overshoots to frame 0x60.
// This batch bracketed it: `setGoalPath((THitActor*)gpMarioAddress)` -- the
// implicit TPathNode conversion at the call site instead of inside
// `setGoalPathMario` -- keeps the frame at 0x50 and puts the temp at 0x38,
// four bytes *over* retail, so retail sits exactly between the two spellings.
// All four combinations: binder+setGoalPathMario 0x2c/frame 0x50,
// binder+direct 0x38/0x50, no-binder+setGoalPathMario 0x24, no-binder+direct
// 0x30/frame 0x48 -- a uniform 8 per binder and 12 per conversion level, so
// the missing item is worth 4. The only 4-byte inline temp frame-gaps.md
// knows is a *pointer* binding (batch 142: reference 8, pointer 4, void 0),
// so one pointer rather than the bool is bound below the temp; a pointer
// binder on the Mario global itself overshoots (+12, temp 0x44).
// That 4 was then found and is now in the shared header: `setGoalPathMario()`
// binds `THitActor* mario` before calling `setGoalPath`, which is the +4 that
// lands seven callers tree-wide (header round 28). Here it moves the temp
// 0x2c -> 0x30 (the predicted +4) but the frame 0x50 -> 0x58, so this function
// is now 4 short of pool *and* 8 over on frame at the same time -- the two no
// longer track each other, so the residue is a high-region item 12 too big
// rather than one more pool step.
void TWalkerEnemy::behaveToFindMario()
{
	if (WalkerEnemyCheckUnk150(this, 2)) {
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushAfterCurrent(&TNerveWalkerEscape::theNerve());
		mSpine->pushAfterCurrent(&TNerveSmallEnemyJump::theNerve());
	} else {
		setGoalPathMario();
		mSpine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
		mSpine->pushAfterCurrent(&TNerveWalkerAttack::theNerve());
		mSpine->pushAfterCurrent(&TNerveSmallEnemyJump::theNerve());
	}
}

void TWalkerEnemy::initAttacker(THitActor* param_1)
{
	mRotation = param_1->getRotation();
	unk184    = 1;
}

// TODO: look for more places to use this
static inline f32 dist(const JGeometry::TVec3<f32>& a,
                       const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> tmp = a;
	tmp.sub(b);
	return tmp.length();
}

bool TWalkerEnemy::isResignationAttack()
{
	f32 fVar1 = getSaveParam2()->mSLGiveUpLength.get();

	if (dist(WalkerPathPoint(unk104), mPosition) > fVar1)
		return true;
	else
		return false;
}

bool TWalkerEnemy::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> tmp = WalkerPathPoint(getUnk104());
	tmp -= mPosition;
	tmp.y = 0.0f;

	if (tmp.x == 0.0f && tmp.z == 0.0f)
		return true;

	if (MsVECMag2(&tmp) < 100.0f)
		return true;
	else
		return false;
}

void TWalkerEnemy::initialGraphNode()
{
	unk124->reset();
	goToShortestNextGraphNode();
}

DEFINE_NERVE(TNerveWalkerGenerate, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setGenerateAnm();
	} else {
		if (self->checkCurAnmEnd(0)) {
			spine->pushAfterCurrent(&TNerveWalkerGraphWander::theNerve());
			return true;
		}
	}

	return false;
}

DEFINE_NERVE(TNerveWalkerGraphWander, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();

	if (spine->getTime() == 0) {
		self->setWalkAnm();
		self->initialGraphNode();
	}

	if (self->isReachedToGoal())
		self->goToRandomNextGraphNode();

	if (self->isFindMario(1.0f)) {
		self->behaveToFindMario();
		return true;
	} else {
		self->walkBehavior(0, 1.0f);
		return false;
	}
}

// Binding level worth +16 of low region, landing
// TNerveWalkerAttack::execute's frame at 0x50 (batch 121).
static inline const JGeometry::TVec3<f32>&
WalkerEnemyGetPosition(const TWalkerEnemy* p)
{
	const JGeometry::TVec3<f32>& position = p->getPosition();
	return position;
}

DEFINE_NERVE(TNerveWalkerAttack, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();

	if (spine->getTime() == 0)
		self->setRunAnm();

	// TODO: what is the inlines play here?
	if (self->unkF4.unk0 == (THitActor*)gpMarioAddress) {
		if (SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE)
		    || SMS_CheckMarioFlag(MARIO_FLAG_IN_SHALLOW_WATER)
		    || SMS_GetMarioGroundPlane()->isWaterSurface()
		    || SMS_CheckMarioFlag(MARIO_FLAG_IN_WATER))
			return true;

		f32 giveUpHeight = self->getSaveParam2()->mSLGiveUpHeight.get();
		if (abs(SMS_GetMarioPos().y - WalkerEnemyGetPosition(self).y)
		    > giveUpHeight)
			return true;
	}

	self->walkBehavior(2, 3.0f);
	if (self->isResignationAttack())
		return true;
	return false;
}

DEFINE_NERVE(TNerveWalkerPostAttack, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();
	if (spine->getTime() == 0)
		self->setWaitAnm();

	s32 delay = self->getSaveParam2()->getSLAttackWait();
	if (spine->getTime() >= delay && self->checkCurAnmEnd(0)) {
		return true;
	}

	return false;
}

// TODO: instruction-exact; frame 0x48 against retail's 0x70, the same missing
// 40 bytes as TWalkerEnemy::moveObject and the same dead `getSaveParam();`
// statement. `switchNextGoalPath()` in place of the two spelled-out statements
// is frame-neutral here.
DEFINE_NERVE(TNerveWalkerEscape, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();
	if (spine->getTime() == 0) {
		self->goToRandomEscapeGraphNode();
		self->setWalkAnm();
	}

	if (self->isReachedToGoal())
		self->goToRandomEscapeGraphNode();

	self->getSaveParam();
	self->getSaveParam();
	if (SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE)) {
		self->switchNextGoalPath();
		return true;
	}

	f32 giveUpLength2 = self->getSaveParam2()->mSLGiveUpLength.get();
	giveUpLength2 *= giveUpLength2;

	self->updateSquareToMario();

	if (self->mDistToMarioSquared > giveUpLength2)
		return true;

	self->walkBehavior(1, 2.0f);
	return false;
}

DEFINE_NERVE(TNerveWalkerTraceMario, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();
	if (spine->getTime() == 0) {
		self->setRunAnm();
		self->setGoalPathMario();
	}

	if (spine->getTime() == 10)
		self->offLiveFlag(LIVE_FLAG_HIDDEN);

	if (SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE)
	    || SMS_CheckMarioFlag(MARIO_FLAG_IN_SHALLOW_WATER)
	    || SMS_GetMarioGroundPlane()->isWaterSurface()
	    || SMS_CheckMarioFlag(MARIO_FLAG_IN_WATER)) {

		spine->pushAfterCurrent(&TNerveWalkerTraceMario::theNerve());
		spine->pushAfterCurrent(&TNerveSmallEnemyWait::theNerve());
		return true;
	}

	f32 giveUpHeight = self->getSaveParam2()->mSLGiveUpHeight.get();
	// Raw `mPosition`, not `getPosition()`: the accessor is worth 4 bytes of
	// pool here, which this function no longer needs now that
	// `setGoalPathMario()` carries its own pointer binding. Frame 0x60, exact.
	if (abs(SMS_GetMarioPos().y - self->mPosition.y) > giveUpHeight)
		return true;

	self->walkBehavior(2, 3.0f);
	if (self->isResignationAttack())
		return true;

	return false;
}
