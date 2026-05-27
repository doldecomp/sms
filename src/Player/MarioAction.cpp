#include <Player/MarioMain.hpp>
#include <M3DUtil/M3UModelMario.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

BOOL TMario::taking()
{
	if ((mInput & 4) != 0) {
		return changePlayerDropping(0x88C, 0);
	}

	setAnimation(ANIM_RAISE, 1.0f);
	if (unk384 != nullptr && mModel->getFrameCtrl(0).checkPass(11.0f)) {
		if (unk384->receiveMessage(this, HIT_MESSAGE_TAKE) == true) {
			startVoice(MSD_SE_MV15_EXERT_INST_01);
			mHeldObject = (TTakeActor*)unk384;
		} else {
			return changePlayerStatus(STATUS_WAIT, 0, false);
		}
		unk384 = nullptr;
	}
	if (isLast1AnimeFrame()) {
		unk380 = 2;
		unk37E = 0;
		return changePlayerStatus(STATUS_WAIT, 0, false);
	} else {
		waitProcess();
		return FALSE;
	}
}

BOOL TMario::takePose()
{
	if ((mInput & 4) != 0)
		return changePlayerDropping(0x88C, 0);

	if (isLast1AnimeFrame()) {
		setAnimation(ANIM_WAIT, 1.0f);
		return changePlayerStatus(STATUS_WAIT, 0, false);
	}

	stopCommon(0x110, STATUS_WAIT);
	return FALSE;
}

BOOL TMario::catchLost()
{
	if ((mInput & 2) != 0) {
		if (considerRotateJumpStart()) {
			return TRUE;
		} else {
			return changePlayerJumping(STATUS_JUMP, false);
		}
	} else {
		if ((mInput & 4) != 0) {
			return changePlayerStatus(0x88C, 0, false);
		} else {
			if ((mInput & 8) != 0) {
				return changePlayerStatus(0x50, 0, false);
			} else {
				stopCommon(0x5A, STATUS_WAIT);
				return FALSE;
			}
		}
	}
}

BOOL TMario::putting()
{
	if ((mInput & 4) != 0)
		return changePlayerDropping(0x88c, 0);

	stopCommon(0x6E, STATUS_WAIT);
	if (mHeldObject != nullptr && mModel->getFrameCtrl(0).checkPass(20.0f)) {
		mHeldObject->receiveMessage(this, HIT_MESSAGE_UNK6);
		mHeldObject = nullptr;
	}

	return FALSE;
}

BOOL TMario::pitching()
{
	if ((mInput & 4) != 0)
		return changePlayerDropping(0x88C, 0);

	stopCommon(0x65, STATUS_WAIT);
	checkThrowObject();
	return FALSE;
}

BOOL TMario::actnMain()
{
	u32 status = mStatus;

	BOOL result = FALSE;

	switch (status) {
	case STATUS_TAKE:
		result = taking();
		break;

	case STATUS_TAKE_POSE:
		result = takePose();
		break;

	case STATUS_CATCH_LOST:
		result = catchLost();
		break;

	case STATUS_PUTTING:
		result = putting();
		break;

	case STATUS_PITCHING:
		result = pitching();
		break;
	}
	return result;
}
