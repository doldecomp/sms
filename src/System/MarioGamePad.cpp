#include <System/MarioGamePad.hpp>
#include <System/Application.hpp>

u16 TMarioGamePad::mResetFlag = 0;

u32 TMarioGamePad::read()
{
	JUTGamePad::read();

	// TODO: I could not make the register check work properly here.
	s32 resetPort = 0;
	if (checkReset(&resetPort)) {
		handleReset(resetPort);
	}
}

void TMarioGamePad::onNeutralMarioKey() { this->_E4 = 0x3c; }

void TMarioGamePad::reset()
{
	this->setButtonRepeat(0xf00000f, 6.0f / SMSGetAnmFrameRate(),
	                      20.0f / SMSGetAnmFrameRate());
	this->_E4             = 0;
	this->mDisabledFrames = 0;
}

void TMarioGamePad::updateMeaning()
{
	u32 stackAlloc[83]; // A lot of stack space still missing
	// Initial processing
	if (this->mDisabledFrames > 0) {
		this->mDisabledFrames -= 1;
	}

	u32 prevMeaning = resetMeaning();

	this->updateMeaning(START, MEANING_0x1, prevMeaning);

	// Process flags with early returns for cleaner flow
	if (checkFlag(PAD_FLAG_0x80)) {
		updateMeaning0x80(prevMeaning);
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x1)) {
		updateMeaning0x1(prevMeaning);
		goto finalize;
	}

	if (!checkFlag(PAD_FLAG_0x2)) {
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x8)) {
		updateMeaning0x8(prevMeaning);
		goto finalize;
	}

	if (checkFlag(PAD_FLAG_0x10)) {
		goto finalize;
	}

	if (this->mDisabledFrames <= 0) {
		bool isLButtonCameraSpecifyMode
		    = gpCamera->isLButtonCameraSpecifyMode(gpCamera->mMode);
		if (isLButtonCameraSpecifyMode) {
			updateMeaningLCamera(prevMeaning);
		} else {
			updateMeaningRegular2(prevMeaning);
		}
	}

finalize:
	updateChangedMeaning(prevMeaning);
}
