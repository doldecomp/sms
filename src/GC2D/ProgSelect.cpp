#include <GC2D/ProgSelect.hpp>
#include <stdio.h>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <System/Application.hpp>
#include <System/MarioGamePad.hpp>

#if defined(VERSION_GMSE01)
static char strDispProg[] = "Would you like to display in\nprogressive scan mode?";
static char strSetProg[] = "\033GM[0]Screen display has been set to\n"
                          "\033FX[24]\033FY[24]\033CC[ffff00]"
                          "progressive scan mode"
                          "\033FX[20]\033FY[20]\033CC[ffffff].";
static char strSetInter[] = "\033GM[0]Screen display has been set to\n"
                           "\033FX[24]\033FY[24]\033CC[ffff00]"
                           "interlace mode"
                           "\033FX[20]\033FY[20]\033CC[ffffff].";
static char strYes[] = "Yes";
static char strNo[] = "No";
#endif

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
#if defined(VERSION_GMSE01)
	snprintf(unk1C, 0x100, strDispProg);
	unk120[0] = new J2DTextBox(gpSystemFont->getResFont(), strYes);
	unk120[1] = new J2DTextBox(gpSystemFont->getResFont(), strNo);
#else
	snprintf(unk1C, 0x100, "プログレッシブモードで\n表示しますか？");
	unk120[0] = new J2DTextBox(gpSystemFont->getResFont(), "はい");
	unk120[1] = new J2DTextBox(gpSystemFont->getResFont(), "いいえ");
#endif

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

void TProgSelect::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
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

		// TODO: 8 bytes of uninitialised local the compiled function never
		// touches. Size (8), position (the named region, above
		// prevSelection) and the fact that the only 8-byte concept in the
		// block is the black/white colour pair passed to setBlackWhite are
		// the whole of the evidence.
		u32 black;
		u32 white;

		u32 prevSelection = getSelection();
		if (getGamePad()->checkFrameMeaning(0x8)) {
			if (mSelection) {
				mSelection = 0;
				unk128     = 0;
			}
		} else if (getGamePad()->checkFrameMeaning(0x10)) {
			if (mSelection != 1) {
				mSelection = 1;
				unk128     = 0;
			}
		} else if (getGamePad()->checkFrameMeaning(0x20) || thing()) {
			{
				if (!mSelection) {
#if defined(VERSION_GMSE01)
					snprintf(unk1C, 256, strSetProg);
#else
					snprintf(unk1C, 256,
					         "GM[0]画面表示モードは\n"
					         "FX[24]FY[24]CC[ffff00]"
					         "プログレッシブモード"
					         "FX[20]FY[20]CC[ffffff]に\n"
					         "セットされました。");
#endif
					OSSetProgressiveMode(1);
				} else {
#if defined(VERSION_GMSE01)
					snprintf(unk1C, 256, strSetInter);
#else
					snprintf(unk1C, 256,
					         "GM[0]画面表示モードは\n"
					         "FX[24]FY[24]CC[ffff00]"
					         "インターレースモード"
					         "FX[20]FY[20]CC[ffffff]に\n"
					         "セットされました。");
#endif
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

	if (cue & CUE_DRAW) {
		J2DOrthoGraph local_110(graphics->getViewport());
		local_110.setup2D();
		J2DPrint JStack_174(gpSystemFont, 0);
		JStack_174.setUnk50(32);
#if defined(VERSION_GMSE01)
		JStack_174.printReturn(unk1C, 360, 160, HBIND_CENTER, VBIND_TOP, 145,
		                       300, 255);
#else
		JStack_174.printReturn(unk1C, 300, 160, HBIND_CENTER, VBIND_TOP, 175,
		                       300, 255);
#endif
		if (!mHideTextBoxes) {
			unk120[0]->draw(240, 400);
			unk120[1]->draw(340, 400);
		}
	}
}
