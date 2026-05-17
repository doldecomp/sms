#include <Player/MarioAnimeData.hpp>
#include <Player/MarioMain.hpp>
#include <M3DUtil/M3UModelMario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TMario::checkPumping()
{
	if (unk108->mAnalogR > 0.0f && unk380 != 0) {
		unk380 = 0;
		unk37E = 0;
	} else {
		if ((gpMarioOriginal == this)
		    && (gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode) != 0)
		    && checkPumpEnable()) {
			unk380 = 1;
			unk37E = 0;
		} else if (mStatus == 0x00800447) {
			unk380 = 1;
			unk37E = 0;
		} else if (mStatus == 0x0c008220 && unk380 == 5) {
			unk380 = 1;
			unk37E = 0;
		} else if (checkFlag(0x4000)) {
			unk380 = 0;
			unk37E = 0;
		}
	}
}

BOOL TMario::checkPumpEnable()
{
	if ((mWaterGun != nullptr) && checkFlag(0x8000)
	    && gMarioAnimeData[mAnimationId].isPumpOK() && !onYoshi()
	    && (!checkUnk368()
	        || !((unk368 / (float)mGraffitoParams.mSinkTime.get()
	              > mGraffitoParams.mSinkPumpLimit.get())))
	    && unk380 != 4 && unk380 != 3 && unk380 != 2
	    && (mStatus != 0x88d || !mWaterGun->checkCurrentNozzleRocketType(1u))
	    && (!mWaterGun->checkCurrentNozzleKind(1)
	        || !mWaterGun->checkCurrentNozzleTriggerSprayState(2))
	    && !mWaterGun->isUnk1D00LessThanZero()
	    && !mWaterGun->isUnk1D00GreaterThanZero() && !checkActionThing()) {
		return TRUE;
	}

	unk380 = 5;
	unk37E = 0;
	return FALSE;
}

void TMario::stateMachineUpper()
{
	switch (unk380) {

	case 0:
		if (!checkPumpEnable()) {
			mModel->unkC[1].setFrame(0.0f);
			unk380 = 5;
		}
		if (unk108->mAnalogR == 0.0f) {
			unk380 = 1;
			unk37E = mUpperBodyParams.mPumpWaitTime.get();
		}
		if (!checkFlag(0x30000) && mWaterGun != nullptr) {
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
	case 1:
		if (!checkPumpEnable()) {
			mModel->unkC[1].setFrame(0.0f);
			unk380 = 5;
		}
		if (unk37E != 0) {
			unk37E -= 1;
		} else {
			mModel->unkC[1].setFrame(0.0f);
			unk380 = 5;
		}
		checkPumping();
		break;
	case 2:
		if (mStatus == -0x7ffffc79) {
			unk380 = 5;
		}
		if (mHeldObject == nullptr) {
			unk380 = 5;
		}
		if (mStatus == 0x4000440 && mForwardVel > 20.0f) {
			emitSweatSometimes();
		}
		break;
	case 4:
		if (mModel->checkModelState()) {
			unk380 = 5;
		}
		break;

	case 3:
	case 5:
	default:
		if (checkPumpEnable()) {
			checkPumping();
		}
		break;
	}
}
