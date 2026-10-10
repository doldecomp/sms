#include <NPC/NpcBase.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <NPC/NpcNerve.hpp>
#include <Camera/cameralib.hpp>

bool TBaseNPC::isCanWalk() const
{
	bool result = true;
	// TODO: TVec3::sub should use set internally I guess?
	if ((unkF4.getPoint() - mPosition).squared() < CLBSquared(2.5625f))
		result = false;
	return result;
}

void TBaseNPC::execWalk(bool param_1)
{
	if (mWalkForbidCount != 0 || SMSGetMarDirector()->isThing() || !isClean()
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

		// TODO: vector math is borked
		JGeometry::TVec3<f32> local_54 = getUnkF4().getPoint();
		local_54 -= mPosition;
		JGeometry::TVec3<f32> copy = local_54;

		f32 angle = MsGetRotFromZaxisY(copy);
		if (MsWrap(abs(mRotation.y - angle), 0.0f, 360.0f) < 0.001f)
			offUnk1DA(UNK1DA_FLAG_UNK1);

		return;
	}

	if (param_1) {
		EnumNpcAnmKind uVar4 = (EnumNpcAnmKind)mLodAnm->getCurrentAnmKind();

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

bool TBaseNPC::execUTurn()
{
	JGeometry::TVec3<f32> local_24 = unkF4.getPoint();
	local_24 -= mPosition;
	f32 targetYaw = MsGetRotFromZaxis(local_24).y;
	if (mRotation.y == targetYaw)
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

	switch (mActorType) {
	case ACTOR_TYPE_NPC_DUMMY:
	case ACTOR_TYPE_NPC_BOARD:
	case ACTOR_TYPE_NPC_MONTE_MG:
		break;

	default:
		const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
		if ((nerve == &TNerveNPCWaitMarioApproach::theNerve()
		     || nerve == &TNerveNPCTurnToMario::theNerve())
		    && (mActorType == ACTOR_TYPE_NPC_MONTE_ME
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
	case ACTOR_TYPE_NPC_MONTE_MF:
	case ACTOR_TYPE_NPC_MONTE_MG:
	case ACTOR_TYPE_NPC_SUNFLOWER_L:
	case ACTOR_TYPE_NPC_SUNFLOWER_S:
	case ACTOR_TYPE_NPC_BOARD:
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
	case ACTOR_TYPE_NPC_KINOPIO:
	case ACTOR_TYPE_NPC_KINOJII:
	case ACTOR_TYPE_NPC_PEACH:
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
