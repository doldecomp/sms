#include <NPC/NpcBase.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <NPC/NpcNerve.hpp>
#include <Camera/cameralib.hpp>

// TODO: retail *calls* JGeometry::TVec3<f32>::set<f32>(f32, f32, f32) from the
// unnamed vector's constructor here (the map lists the weak 16-byte symbol for
// this TU and execWalk's inlined copy has the `bl`), so retail reaches `set`
// one level deeper than we do - isCanWalk itself is probably inlined at depth 2
// in execWalk. `getPosition()` in the difference expressions does not move it.
bool TBaseNPC::isCanWalk() const
{
	bool result = true;
	JGeometry::TVec3<f32> target = unkF4.getPoint();
	if (JGeometry::TVec3<f32>(target.x - mPosition.x, 0.0f,
	                          target.z - mPosition.z).squared()
	    < CLBSquared(10.0f))
		result = false;
	return result;
}

void TBaseNPC::execWalk(bool param_1)
{
	if (mWalkForbidCount != 0 || gpMarDirector->isThing() || !isClean()
	    || checkActionFlag(NPC_ACTION_HAPPY)) {
		mMarchSpeed = 0.0f;
		mTurnSpeed  = 0.0f;
		return;
	}

	if (param_1 && checkUnk1DA(UNK1DA_FLAG_UNK1)) {
		f32 fVar1 = 4.0f;
		if (checkActionFlag(NPC_ACTION_RUN))
			fVar1 = 6.0f;

		SMS_GoRotate(mPosition, getUnkF4().getPoint(), fVar1, &mRotation.y);

		// TODO: retail makes *two* 12-byte copies of `direction` before
		// reading .z/.x (0xdc -> 0xfc -> 0x10c), and execWalk's frame only
		// reaches 0x130 with both. The source spelling that produces two
		// copies is unknown; a named copy plus one conversion temporary
		// reproduces the shape but is surely not what was written.
		JGeometry::TVec3<f32> direction = getUnkF4().getPoint();
		direction -= mPosition;
		JGeometry::TVec3<f32> copy;
		copy = direction;

		f32 angle = MsGetRotFromZaxisY(JGeometry::TVec3<f32>(copy));
		if (MsWrap(fabsf(mRotation.y - angle), 0.0f, 360.0f) < 0.001f)
			offUnk1DA(UNK1DA_FLAG_UNK1);

		return;
	}

	if (param_1) {
		EnumNpcAnmKind uVar4 = (EnumNpcAnmKind)unkD0->getCurrentAnmKind();

		f32 dVar11 = mIndividualParams->mSLMinMarchSpeed.get();
		f32 dVar12 = mIndividualParams->mMarchAccel.get();

		switch (uVar4) {
		case NPC_ANM_KIND_WALK:
			dVar11 = mIndividualParams->mMaxMarchSpeed.get();
			break;

		case NPC_ANM_KIND_RUN:
			dVar11 = mIndividualParams->mSLMaxRunSpeed.get();
			dVar12 = mIndividualParams->mSLRunAccel.get();
			if (checkActionFlag(NPC_ACTION_BURNING)) {
				f32 dVar10 = mPtrSaveNormal->mSLSmokeRunMagnif.get();
				dVar11 *= dVar10;
				dVar12 *= dVar10;
			}
			break;
		}

		CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed, dVar11, dVar12);
	} else {
		CLBChaseGeneralConstantSpecifySpeed(
		    &mMarchSpeed, 0.0f, mIndividualParams->mMarchDecrease.get());
	}

	if (mMarchSpeed < 0.001f)
		mTurnSpeed = mIndividualParams->mWaitTurnSpeed.get();
	else
		mTurnSpeed = mIndividualParams->mWalkTurnSpeed.get();

	if (isCanWalk())
		walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
}

// TODO: frame 0x60 vs 0x58, one `fmr f2, f0` retail has and we lack (a second
// variable holding MsGetRotFromZaxis(...).y) and a swapped `fcmpu`. Spelling it
// as two locals with the compare reversed is worse (99.0 -> 97.3), and
// getUnkF4() here is worse too (97.9): this site wants the raw member where
// execWalk wants the accessor.
bool TBaseNPC::execUTurn()
{
	JGeometry::TVec3<f32> local_24 = unkF4.getPoint();
	local_24 -= mPosition;
	f32 targetYaw = MsGetRotFromZaxis(local_24).y;
	if (targetYaw == mRotation.y)
		return true;

	if (!isClean() || checkActionFlag(NPC_ACTION_HAPPY))
		return false;

	bool result = false;

	targetYaw   = MsWrap(targetYaw, 0.0f, 360.0f);
	mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);
	if (checkLiveFlag(LIVE_FLAG_UNK200000)) {
		if (mRotation.y > targetYaw)
			mRotation.y -= 360.0f;
	} else {
		if (mRotation.y < targetYaw)
			mRotation.y += 360.0f;
	}

	BOOL r = CLBChaseGeneralConstantSpecifySpeed(
	    &mRotation.y, targetYaw, mIndividualParams->mUTurnSpeed.get());
	mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);
	if (!r)
		result = true;

	return result;
}

// TODO: frame 0x50 vs 0x40, uniform +0x10 on every slot (low region). Naming
// the param fetch makes it worse (0x38), `getRotation().y` for angle1 adds an
// instruction (99.7 -> 96.9). Two more accessor levels are needed somewhere in
// the CLBDegToShortAngle / CLBChaseGeneralConstantSpecifySpeed chain.
bool TBaseNPC::execTurnToFirstState()
{
	if (mRotation.y == unk1A0.y)
		return true;

	bool result = false;

	s16 angle1 = CLBDegToShortAngle(mRotation.y);
	s16 angle2 = CLBDegToShortAngle(unk1A0.y);
	s16 angle3
	    = CLBDegToShortAngle(mIndividualParams->mFirstStateTurnSpeed.get());
	if (!CLBChaseGeneralConstantSpecifySpeed(&angle1, angle2, angle3)) {
		result      = true;
		mRotation.y = unk1A0.y;
	} else {
		mRotation.y = SHORTANGLE2DEG(angle1);
	}

	return result;
}

bool TBaseNPC::isNeedTurnToFirstState() const
{
	if (!isClean() || checkActionFlag(NPC_ACTION_HAPPY))
		return false;

	bool result = false;

	switch (getActorType()) {
	case 0x400001C:
	case 0x400001D:
	case 0x4000008:
		break;

	default:
		const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
		if ((nerve == &TNerveNPCWaitMarioApproach::theNerve()
		     || nerve == &TNerveNPCTurnToMario::theNerve())
		    && (getActorType() == 0x4000006
		        || !checkActionFlag(NPC_ACTION_UNK800 | NPC_ACTION_UNK400
		                            | NPC_ACTION_UNK1))) {
			result = true;
		}
		break;
	}

	return result;
}

bool TBaseNPC::isTurnToMarioWhenTalk() const
{
	bool result = true;
	switch (mActorType) {
	case 0x4000007:
	case 0x4000008:
	case 0x400001A:
	case 0x400001B:
	case 0x400001D:
		result = false;
		break;

	default:
		if (checkActionFlag(NPC_ACTION_UNK800 | NPC_ACTION_UNK400
		                    | NPC_ACTION_UNK1))
			result = false;
		break;
	}
	return result;
}

bool TBaseNPC::isTurnToMarioWhenApproach() const
{
	if (!isClean() || checkActionFlag(NPC_ACTION_HAPPY))
		return false;

	bool result = true;

	switch (mActorType) {
	case 0x4000016:
	case 0x4000017:
	case 0x4000018:
		if (checkActionFlag(
		        NPC_ACTION_BURNING | NPC_ACTION_UNK2000 | NPC_ACTION_UNK1000
		        | NPC_ACTION_UNK800 | NPC_ACTION_UNK400 | NPC_ACTION_HAPPY
		        | NPC_ACTION_UNK40 | NPC_ACTION_UNK20 | NPC_ACTION_UNK10
		        | NPC_ACTION_RUN | NPC_ACTION_DANCE | NPC_ACTION_UNK2
		        | NPC_ACTION_UNK1))
			result = false;
		break;

	default:
		result = false;
		break;
	}
	return result;
}
