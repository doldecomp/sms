#include <Player/MarioAnimeData.hpp>
#include <Player/Mario.hpp>
#include <M3DUtil/M3UModelMario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// TODO: promote into Mario.hpp as `TWaterGun* TMario::getFludd() const`.
// checkPumpEnable() is instruction-identical to retail with either spelling,
// but retail's frame is 0x50 against 0x40 for the raw member: the accessor
// level buys two 8-byte inline-expansion slots in the low region and closes
// the gap exactly. The lever saturates at two, so only the first four of the
// six reads below go through it; using it for the two nozzle-switch
// predicates as well makes MWCC reload mSwitchToSecondNozzleSpeed instead of
// reusing f1 across the pair. Parked as a TU-local helper because the shared
// header is off limits in this batch.
static inline TWaterGun* MarioUpperGetFludd(const TMario* mario)
{
	return mario->mWaterGun;
}

void TMario::checkPumping()
{
	if (unk108->mAnalogR > 0.0f && mUpperState != UPPER_STATE_PUMPING) {
		mUpperState   = UPPER_STATE_PUMPING;
		mPumpCooldown = 0;
		return;
	}

	if ((gpMarioOriginal == this) && gpCamera->isLButtonCamera() != false
	    && checkPumpEnable()) {
		mUpperState   = UPPER_STATE_HOLDING_PUMP;
		mPumpCooldown = 0;
		return;
	}

	if (mStatus == MARIO_STATUS_TOROCCO) {
		mUpperState   = UPPER_STATE_HOLDING_PUMP;
		mPumpCooldown = 0;
		return;
	}

	if (mStatus == MARIO_STATUS_SQUAT && mUpperState == UPPER_STATE_IDLE) {
		mUpperState   = UPPER_STATE_HOLDING_PUMP;
		mPumpCooldown = 0;
		return;
	}

	if (checkFlag(MARIO_FLAG_FLUDD_EMITTING)) {
		mUpperState   = UPPER_STATE_PUMPING;
		mPumpCooldown = 0;
		return;
	}
}

BOOL TMario::checkPumpEnable()
{
	if ((MarioUpperGetFludd(this) != nullptr) && checkFlag(MARIO_FLAG_HAS_FLUDD)
	    && gMarioAnimeData[mAnimationId].isPumpOK() && !onYoshi()
	    && (!isSinking()
	        || !((mSinkTimer / (float)mGraffitoParams.mSinkTime.get()
	              > mGraffitoParams.mSinkPumpLimit.get())))
	    && mUpperState != UPPER_STATE_FIXED_ANIMATION
	    && mUpperState != UPPER_STATE_UNK3
	    && mUpperState != UPPER_STATE_HOLDING_OBJECT
	    && (mStatus != MARIO_STATUS_ROCKET_LANDING
	        || !MarioUpperGetFludd(this)->checkCurrentNozzleRocketType(
	            TWaterGun::Rocket))
	    && (!MarioUpperGetFludd(this)->checkCurrentNozzleKind(TWaterGun::Rocket)
	        || !MarioUpperGetFludd(this)->checkCurrentNozzleTriggerSprayState(
	            TNozzleTrigger::DEAD))
	    && !mWaterGun->isSwitchingToSprayNozzle()
	    && !mWaterGun->isSwitchingToSecondaryNozzle()
	    && !checkStatusType(MARIO_STATUS_FLAG_UNK1000)) {
		return TRUE;
	}

	mUpperState   = UPPER_STATE_IDLE;
	mPumpCooldown = 0;
	return FALSE;
}

void TMario::stateMachineUpper()
{
	switch (mUpperState) {
	case UPPER_STATE_PUMPING:
		if (!checkPumpEnable()) {
			mModel->getFrameCtrl(1).setFrame(0.0f);
			mUpperState = UPPER_STATE_IDLE;
		}
		if (unk108->mAnalogR == 0.0f) {
			mUpperState   = UPPER_STATE_HOLDING_PUMP;
			mPumpCooldown = mUpperBodyParams.mPumpWaitTime.get();
		}
		if (!checkFlag(MARIO_FLAG_IN_ANY_WATER) && mWaterGun != nullptr
		    && mWaterGun->isEmitting()) {
			emitSweatSometimes();
		}
		break;

	case UPPER_STATE_HOLDING_PUMP:
		if (!checkPumpEnable()) {
			mModel->getFrameCtrl(1).setFrame(0.0f);
			mUpperState = UPPER_STATE_IDLE;
		}
		if (mPumpCooldown != 0) {
			mPumpCooldown -= 1;
		} else {
			mModel->getFrameCtrl(1).setFrame(0.0f);
			mUpperState = UPPER_STATE_IDLE;
		}
		checkPumping();
		break;

	case UPPER_STATE_HOLDING_OBJECT:
		if (getStatus() == MARIO_STATUS_PUTTING)
			mUpperState = UPPER_STATE_IDLE;

		if (mHeldObject == nullptr)
			mUpperState = UPPER_STATE_IDLE;

		if (getStatus() == MARIO_STATUS_RUN && mForwardVel > 20.0f)
			emitSweatSometimes();
		break;

	case UPPER_STATE_FIXED_ANIMATION:
		if (mModel->someAnimationCompleted())
			mUpperState = UPPER_STATE_IDLE;
		break;

	case UPPER_STATE_UNK3:
	case UPPER_STATE_IDLE:
	default:
		if (checkPumpEnable())
			checkPumping();
		break;
	}
}
