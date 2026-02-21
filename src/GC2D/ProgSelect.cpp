#include <GC2D/ProgSelect.hpp>
#include <stdio.h>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>

TProgSelect::TProgSelect(u8 param_1, const char* name)
    : JDrama::TViewObj(name)
    , mPulsingTimer(255)
    , mSelection(param_1)
    , mIncreasePulsing(false)
    , mHideTextBoxes(false)
{
	f32 sync     = SMSGetVSyncTimesPerSec();
	unk128       = 0;
	mRefreshRate = sync;
	snprintf(unk1C, 0x100, "プログレッシブモードで\n表示しますか？");
	unk120[0] = new J2DTextBox(gpSystemFont->getResFont(), "はい");
	unk120[1] = new J2DTextBox(gpSystemFont->getResFont(), "いいえ");

	unk120[0]->setFontSize(28, 28);
	unk120[1]->setFontSize(28, 28);
	if (!mSelection) {
		unk120[0]->setBlackWhite(0x00ff0000, 0x00ff00ff);
		unk120[1]->setBlackWhite(0x7f7f7f00, 0x7f7f7fff);
	} else {
		unk120[1]->setBlackWhite(0x00ff0000, 0x00ff00ff);
		unk120[0]->setBlackWhite(0x7f7f7f00, 0x7f7f7fff);
	}
}

void TProgSelect::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		if (mIncreasePulsing) {
			mPulsingTimer += 16;
			if (mPulsingTimer > 255) {
				mPulsingTimer    = 255;
				mIncreasePulsing = false;
			}
		} else {
			mPulsingTimer -= 16;
			if (mPulsingTimer < 0) {
				mPulsingTimer    = 0;
				mIncreasePulsing = true;
			}
		}

		u32 prevSelection = mSelection;
		if (mGamePad->checkFrameMeaning(0x8)) {
			if (mSelection) {
				mSelection = 0;
				unk128     = 0;
			}
		} else if (mGamePad->checkFrameMeaning(0x10)) {
			if (mSelection != 1) {
				mSelection = 1;
				unk128     = 0;
			}
		} else if (mGamePad->checkFrameMeaning(0x20) || thing()) {
			{
				if (!mSelection) {
					snprintf(unk1C, 256,
					         "GM[0]画面表示モードは\n"
					         "FX[24]FY[24]CC[ffff00]"
					         "プログレッシブモード"
					         "FX[20]FY[20]CC[ffffff]に\n"
					         "セットされました。");
					OSSetProgressiveMode(1);
				} else {
					snprintf(unk1C, 256,
					         "GM[0]画面表示モードは\n"
					         "FX[24]FY[24]CC[ffff00]"
					         "インターレースモード"
					         "FX[20]FY[20]CC[ffffff]に\n"
					         "セットされました。");
					OSSetProgressiveMode(0);
				}
				mHideTextBoxes = true;
			}
		}

		if (prevSelection != mSelection) {
			mPulsingTimer = 255;
			unk120[mSelection]->setBlackWhite(0x00ff0000, 0x00ff00ff);
			unk120[prevSelection]->setBlackWhite(0x7f7f7f00, 0x7f7f7fff);
			unk120[prevSelection]->setAlpha(255);
		}

		unk120[mSelection]->setBlackWhite(0x00ff0000,
		                                  mPulsingTimer + 0x00ff0000);
	}

	if (param_1 & 0x8) {
		J2DOrthoGraph local_110(param_2->getUnk54());
		local_110.setup2D();
		J2DPrint JStack_174(gpSystemFont, 0);
		JStack_174.setUnk50(32);
		JStack_174.printReturn(unk1C, 300, 160, HBIND_CENTER, VBIND_TOP, 175,
		                       300, 255);
		if (!mHideTextBoxes) {
			unk120[0]->draw(240, 400);
			unk120[1]->draw(340, 400);
		}
	}

	char trahs[0x10];
}
