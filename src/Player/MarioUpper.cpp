#include <Player/MarioAnimeData.hpp>
#include <Player/Mario.hpp>
#include <M3DUtil/M3UModelMario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::checkPumping()
{
	if (unk108->mAnalogR > 0.0f && mUpperState != UPPER_STATE_PUMPING) {
		mUpperState   = UPPER_STATE_PUMPING;
		mPumpCooldown = 0;
		return;
	}

	if ((gpMarioOriginal == this)
	    && (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode) != 0)
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
	if ((mWaterGun != nullptr) && checkFlag(MARIO_FLAG_HAS_FLUDD)
	    && gMarioAnimeData[mAnimationId].isPumpOK() && !onYoshi()
	    && (!isSinking()
	        || !((mSinkTimer / (float)mGraffitoParams.mSinkTime.get()
	              > mGraffitoParams.mSinkPumpLimit.get())))
	    && mUpperState != UPPER_STATE_FIXED_ANIMATION
	    && mUpperState != UPPER_STATE_UNK3
	    && mUpperState != UPPER_STATE_HOLDING_OBJECT
	    && (mStatus != MARIO_STATUS_ROCKET_LANDING
	        || !mWaterGun->checkCurrentNozzleRocketType(TWaterGun::Rocket))
	    && (!mWaterGun->checkCurrentNozzleKind(TWaterGun::Rocket)
	        || !mWaterGun->checkCurrentNozzleTriggerSprayState(
	            TNozzleTrigger::DEAD))
	    && !mWaterGun->isSwitchingToSprayNozzle()
	    && !mWaterGun->isSwitchingToSecondaryNozzle()
	    && !checkStatusFlag(MARIO_STATUS_FLAG_UNK1000)) {
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
			mModel->unkC[1].setFrame(0.0f);
			mUpperState = UPPER_STATE_IDLE;
		}
		if (unk108->mAnalogR == 0.0f) {
			mUpperState   = UPPER_STATE_HOLDING_PUMP;
			mPumpCooldown = mUpperBodyParams.mPumpWaitTime.get();
		}
		if (!checkFlag(MARIO_FLAG_IN_ANY_WATER) && mWaterGun != nullptr) {
			s32 flag;
			if (mWaterGun->mCurrentWater == 0) {
				flag = 0;
			} else {
				TWaterGun const* waterGun = mWaterGun;
				if (waterGun->checkCurrentNozzleKind(1)) {
					if (waterGun->checkCurrentNozzleTriggerSprayState(1)) {
						flag = 1;
					} else {
						flag = 0;
					}
				} else {
					if (waterGun->getCurrentNozzle()->unk378 > 0.0f) {
						flag = 1;
					} else {
						flag = 0;
					}
				}
			}

			if (flag != 0) {
				emitSweatSometimes();
			}
		}
		break;

	case UPPER_STATE_HOLDING_PUMP:
		if (!checkPumpEnable()) {
			mModel->unkC[1].setFrame(0.0f);
			mUpperState = UPPER_STATE_IDLE;
		}
		if (mPumpCooldown != 0) {
			mPumpCooldown -= 1;
		} else {
			mModel->unkC[1].setFrame(0.0f);
			mUpperState = UPPER_STATE_IDLE;
		}
		checkPumping();
		break;

	case UPPER_STATE_HOLDING_OBJECT:
		if (mStatus == MARIO_STATUS_PUTTING)
			mUpperState = UPPER_STATE_IDLE;

		if (mHeldObject == nullptr)
			mUpperState = UPPER_STATE_IDLE;

		if (mStatus == MARIO_STATUS_RUN && mForwardVel > 20.0f)
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
