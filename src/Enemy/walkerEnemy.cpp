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

	mMarchSpeed = getSaveParam2()->unk324.get();
	mSpine->initWith(&TNerveWalkerGenerate::theNerve());
}

// TODO: fake
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * JMACos(theta);
	f32 s = radius * JMASin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

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
		mRotation.y += 5.0f;
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

	mMarchSpeed = getSaveParam2()->unk324.get();

	((TWalker*)mBinder)->reset();
	mSpine->reset();
	mSpine->setNext(mSpine->getDefault());
	setGoalPathMario();
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

void TWalkerEnemy::behaveToFindMario()
{
	if (checkUnk150(2)) {
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
	f32 fVar1 = getSaveParam2()->getSLGiveUpLength();

	if (dist(unk104.getPoint(), mPosition) > fVar1)
		return true;
	else
		return false;
}

bool TWalkerEnemy::isReachedToGoalXZ()
{
	JGeometry::TVec3<f32> tmp = unk104.getPoint();
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

DEFINE_NERVE(TNerveWalkerAttack, TLiveActor)
{
	TWalkerEnemy* self = (TWalkerEnemy*)spine->getBody();

	if (spine->getTime() == 0)
		self->setRunAnm();

	// TODO: what is the inlines play here?
	if (self->unkF4.unk0 == (THitActor*)gpMarioAddress) {
		if (SMS_CheckMarioFlag(0x2) || SMS_CheckMarioFlag(0x10000)
		    || SMS_GetMarioGroundPlane()->isWaterSurface()
		    || SMS_CheckMarioFlag(0x20000))
			return true;

		f32 giveUpHeight = self->getSaveParam2()->mSLGiveUpHeight.get();
		if (abs(SMS_GetMarioPos().y - self->getPosition().y) > giveUpHeight)
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
	if (SMS_CheckMarioFlag(0x2)) {
		if (!self->unk114.empty())
			self->unkF4 = self->unk114.pop();
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

	if (SMS_CheckMarioFlag(0x2) || SMS_CheckMarioFlag(0x10000)
	    || SMS_GetMarioGroundPlane()->isWaterSurface()
	    || SMS_CheckMarioFlag(0x20000)) {

		spine->pushAfterCurrent(&TNerveWalkerTraceMario::theNerve());
		spine->pushAfterCurrent(&TNerveSmallEnemyWait::theNerve());
		return true;
	}

	f32 giveUpHeight = self->getSaveParam2()->mSLGiveUpHeight.get();
	if (abs(SMS_GetMarioPos().y - self->getPosition().y) > giveUpHeight)
		return true;

	self->walkBehavior(2, 3.0f);
	if (self->isResignationAttack())
		return true;

	return false;
}
