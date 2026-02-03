#include <Player/MarioMain.hpp>

bool TMario::taking()
{
	if ((mInput & 4) != 0) {
		return changePlayerDropping(0x88C, 0);
	}

	setAnimation(0x6B, 1.0f);
	if (unk384 != nullptr && unk3A8->getFrameCtrl()->checkPass(11.0f)) {
		if (unk384->receiveMessage(this, 0x4) == true) {
			startVoice(0x788F);
			mHeldObject = (TTakeActor*)unk384;
		} else {
			return changePlayerStatus(0xC400201, 0, false);
		}
		unk384 = nullptr;
	}
	if (isLast1AnimeFrame()) {
		unk380 = 2;
		unk37E = 0;
		return changePlayerStatus(0xC400201, 0, false);
	} else {
		waitProcess();
		return false;
	}
}

bool TMario::actnMain()
{
	u32 action = mAction;

	bool result = false;

	// TODO: Action enum
	switch (action) {
	case 0x383:
		result = taking();
		break;
	case 0x384:
		if ((mInput & 4) != 0) {
			result = changePlayerDropping(0x88C, 0);
		} else {
			if (isLast1AnimeFrame()) {
				setAnimation(0xC3, 1.0f);
				result = changePlayerStatus(0xC400201, 0, false);
			} else {
				stopCommon(0x110, 0xC400201);
				result = false;
			}
		}
		break;
	case 0x386:
		if ((mInput & 2) != 0) {
			if (considerRotateJumpStart()) {
				result = true;
			} else {
				result = changePlayerJumping(0x2000880, false);
			}
		} else {
			if ((mInput & 4) != 0) {
				result = changePlayerStatus(0x88C, 0, false);
			} else {
				if ((mInput & 8) != 0) {
					result = changePlayerStatus(0x50, 0, false);
				} else {
					stopCommon(0x5A, 0xC400201);
					result = false;
				}
			}
		}
		break;
	case 0x80000387:
		if ((mInput & 4) != 0) {
			result = changePlayerDropping(0x88c, 0);
		} else {
			stopCommon(0x6E, 0xC400201);
			if (mHeldObject != nullptr
			    && unk3A8->getFrameCtrl()->checkPass(20.0f)) {
				mHeldObject->receiveMessage(this, 0x6);
				mHeldObject = nullptr;
			}
			result = false;
		}
		break;
	case 0x80000588:
		if ((mInput & 4) != 0) {
			result = changePlayerDropping(0x88C, 0);
		} else {
			stopCommon(0x65, 0xC400201);
			checkThrowObject();
			result = false;
		}
		break;
	}
	return result;
}
