#include <GC2D/GCConsole2.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/BlendPane.hpp>
#include <GC2D/ConsoleStr.hpp>
#include <GC2D/MessageLoader.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/HelpActor.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/Helpers.hpp>
#include <Camera/Camera.hpp>
#include <MSound/MSound.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/Mario.hpp>
#include <Enemy/BossEel.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/WaterGun.hpp>
#include <Player/Yoshi.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <Strategic/TakeActor.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DPrint.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DWindow.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdio.h>
#include <string.h>
#include <version.h>

extern JPAEmitterManager* gpEmitterManager4D2;
bool SMS_isDivingMap();

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

JUTPoint TGCConsole2::cDownTopPoint(0, -100);
JUTPoint TGCConsole2::cDownMidPoint(0, 30);
JUTPoint TGCConsole2::cDownBotPoint(0, 0);
JUTPoint TGCConsole2::cUpTopPoint(0, 50);
JUTPoint TGCConsole2::cUpMidPoint(0, -30);
JUTPoint TGCConsole2::cUpBotPoint(0, 0);
JUTPoint TGCConsole2::cCoinTopPoint(0, 0);
JUTPoint TGCConsole2::cCoinMidPoint(0, 45);
JUTPoint TGCConsole2::cCoinBotPoint(0, 0);

#if defined(VERSION_GMSP01_NX)
// fabricated
static inline void shiftWidePane(TExPane* pane, int x)
{
	pane->getPane()->add(x, 0);
	pane->mInitialBounds = pane->getPane()->getBounds();
}

// fabricated
static inline void shiftWidePane(TBoundPane* pane, int x)
{
	pane->getPane()->add(x, 0);
	pane->unk4 = pane->getPane()->getBounds();
}
#endif

// fabricated
static inline void syncPaneBounds(TBoundPane* pane)
{
	pane->unk4 = pane->getPane()->getBounds();
}

// fabricated
static inline bool isMountedYoshi(TMario* mario)
{
	TYoshi* yoshi = mario->mYoshi;
	return yoshi != nullptr && yoshi->mState == TYoshi::STATE_MOUNTED;
}

// fabricated
// TODO: the ROM calls TColor::set out of line in these two helpers, so their
// colour writes are two inline levels below perform. The split is a guess.
static inline void setLifeMeterWhiteHigh(TGCConsole2* console, bool airMode)
{
	if (airMode) {
		((J2DPicture*)console->mLifeMeterText->getPane())
		    ->setWhite(JUtility::TColor(0, 0xff, 0xff, 0xff));
		((J2DPicture*)console->mLifeMeterPetals[0])
		    ->setWhite(JUtility::TColor(0, 0xff, 0xff, 0xff));
	} else {
		((J2DPicture*)console->mLifeMeterText->getPane())
		    ->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
		((J2DPicture*)console->mLifeMeterPetals[0])
		    ->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
	}
}

// fabricated
static inline void setLifeMeterWhiteLow(TGCConsole2* console, bool airMode)
{
	if (airMode) {
		((J2DPicture*)console->mLifeMeterText->getPane())
		    ->setWhite(JUtility::TColor(0, 0xa0, 0xff, 0xff));
		((J2DPicture*)console->mLifeMeterPetals[0])
		    ->setWhite(JUtility::TColor(0, 0x80, 0x80, 0xff));
	} else {
		((J2DPicture*)console->mLifeMeterText->getPane())
		    ->setWhite(JUtility::TColor(0x80, 0x80, 0x80, 0xff));
		((J2DPicture*)console->mLifeMeterPetals[0])
		    ->setWhite(JUtility::TColor(0x80, 0x80, 0x80, 0xff));
	}
}

// fabricated
static inline void playLifeChangeSound(u32 sound)
{
	if (SMSGetMarDirector()->getState() == TMarDirector::STATE_UNK4
	    && SMSGetMarDirector()->isGameplayModeNow()) {
		SMSGetMSound()->startSoundSystemSE(sound, 0, nullptr, 0);
	}
}

// fabricated
static inline void detachBoundPaneFromParent(TBoundPane* pane)
{
	pane->getPane()->removeFromParent();
}

// fabricated
static inline int getHiddenOffsetY(TExPane* pane)
{
	return -(pane->mInitialBounds.y2 + 1);
}

// fabricated
static inline int getHiddenOffsetY(TBoundPane* pane)
{
	return -(pane->unk4.y2 + 1);
}

// fabricated
static inline int getBelowScreenOffsetY(TExPane* pane)
{
	return 465 - pane->getInitialBounds().y1;
}

// fabricated
static inline void initHiddenPaneAbove(TExPane* pane)
{
	pane->updatePaneOffset(1, 0, getHiddenOffsetY(pane));
	pane->update();
}

// fabricated
static inline void initHiddenPaneOffset(TExPane* pane, int offset)
{
	pane->updatePaneOffset(1, 0, offset);
	pane->update();
}

// fabricated
template <class Pane>
static inline void setDigitPane(Pane* pane, JUTTexture** textures, int digit)
{
	((J2DPicture*)pane->getPane())
	    ->changeTexture(textures[digit]->getTexInfo(), 0);
}

// fabricated
static inline void setTwoDigits(TBoundPane** panes, JUTTexture** textures,
                                int value)
{
	setDigitPane(panes[0], textures, (int)(value * 0.1f));
	setDigitPane(panes[1], textures, value % 10);
}

// fabricated
template <class Pane>
static inline void setCounterDigits(Pane** panes, JUTTexture** textures,
                                    const s32& value)
{
	if (value < 100) {
		setDigitPane(panes[0], textures, (int)(value * 0.1f));
		setDigitPane(panes[1], textures, value % 10);
	} else {
		setDigitPane(panes[0], textures, (int)(value * 0.01f));

		int remainder = value - (int)(value * 0.01f) * 100;
		setDigitPane(panes[1], textures, (int)(remainder * 0.1f));
		setDigitPane(panes[2], textures, remainder % 10);
	}
}

// fabricated
template <class Pane>
static inline void setBlueCoinDigits(Pane** panes, JUTTexture** textures,
                                     const s32& value)
{
	if (value < 100) {
		setDigitPane(panes[0], textures, (int)(value * 0.1f));
		setDigitPane(panes[1], textures, value % 10);
		if (panes[2]->getPane()->isVisible())
			panes[2]->getPane()->hide();
	} else {
		setDigitPane(panes[0], textures, (int)(value * 0.01f));

		int remainder = value - (int)(value * 0.01f) * 100;
		setDigitPane(panes[1], textures, (int)(remainder * 0.1f));
		setDigitPane(panes[2], textures, remainder % 10);
		if (!panes[2]->getPane()->isVisible())
			panes[2]->getPane()->show();
	}
}

// fabricated
template <class Pane>
static inline void setShineDigits(Pane** panes, JUTTexture** textures,
                                  const s32& value)
{
	if (value < 100) {
		setDigitPane(panes[0], textures, (int)(value * 0.1f));
		setDigitPane(panes[1], textures, value % 10);
	} else {
		if (!panes[2]->getPane()->isVisible())
			panes[2]->getPane()->show();

		setDigitPane(panes[0], textures, (int)(value * 0.01f));

		int remainder = value - (int)(value * 0.01f) * 100;
		setDigitPane(panes[1], textures, (int)(remainder * 0.1f));
		setDigitPane(panes[2], textures, remainder % 10);
	}
}

// fabricated
static inline void updateDownBlendPaneState(TBlendPane*& pane, bool& isFinished)
{
	if (pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cDownMidPoint,
			                      TGCConsole2::cDownMidPoint,
			                      TGCConsole2::cDownBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void updateDownPaneState(TBoundPane*& pane, bool& isFinished)
{
	if (pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cDownMidPoint,
			                      TGCConsole2::cDownMidPoint,
			                      TGCConsole2::cDownBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void updateUpPaneState(TBoundPane*& pane, bool& isFinished)
{
	if (pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cUpMidPoint,
			                      TGCConsole2::cUpMidPoint,
			                      TGCConsole2::cUpBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void updateUpPaneStateAfter(TBoundPane*& pane, bool& isFinished,
                                          int frame, int startFrame)
{
	if (frame >= startFrame && pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cUpMidPoint,
			                      TGCConsole2::cUpMidPoint,
			                      TGCConsole2::cUpBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void updateUpBlendPaneState(TBlendPane*& pane, bool& isFinished)
{
	if (pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cUpMidPoint,
			                      TGCConsole2::cUpMidPoint,
			                      TGCConsole2::cUpBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void updateCoinPaneState(TBoundPane*& pane, bool& isFinished)
{
	if (pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cCoinMidPoint,
			                      TGCConsole2::cCoinMidPoint,
			                      TGCConsole2::cCoinBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void updateCoinBlendPaneState(TBlendPane*& pane, bool& isFinished)
{
	pane->update();

	if (!pane->unk24) {
		bool paneFinished = false;
		if (pane->unk14.x == 0 && pane->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			pane->setPanePosition(30, TGCConsole2::cCoinMidPoint,
			                      TGCConsole2::cCoinMidPoint,
			                      TGCConsole2::cCoinBotPoint);
			isFinished = false;
		}
	} else {
		isFinished = false;
	}
}

// fabricated
static inline void drawWaterOrJuice(TGCConsole2* console, J2DOrthoGraph& graph)
{
	if (gpMarioOriginal->mYoshi->onYoshi()) {
		switch (gpModelWaterManager->unk5D5F) {
		case 1:
			console->drawJuice(graph, ((u32)console->unkA2.r << 24)
			                              + ((u32)console->unkA2.g << 16)
			                              + ((u32)console->unkA2.b << 8)
			                              + console->unkA2.a);
			break;
		case 2:
			console->drawJuice(graph, ((u32)console->unkA6.r << 24)
			                              + ((u32)console->unkA6.g << 16)
			                              + ((u32)console->unkA6.b << 8)
			                              + console->unkA6.a);
			break;
		case 3:
			console->drawJuice(graph, ((u32)console->unkAA.r << 24)
			                              + ((u32)console->unkAA.g << 16)
			                              + ((u32)console->unkAA.b << 8)
			                              + console->unkAA.a);
			break;
		case 0:
		default:
			console->drawJuice(graph, 0);
			break;
		}
	} else if (console->unk2F8->getPane()->isVisible()) {
		console->drawWater(graph);
	}
}

static const s32 scNozzleSoundList[] = {
	0x88B0, 0x88B1, 0x88B2, 0x88B3, 0x88B4, 0x88B5,
	0x88B6, 0x88B7, 0x88B8, 0x88B9, 0x88BA, -1,
};

static u32 scDolpicNewsDolpic0[]   = { 0x000E0000, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic1[]   = { 0x000E0001, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_1[] = { 0x000E0009, 0x000E0011, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_2[] = { 0x000E0012, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_3[] = { 0x000E0013, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_4[] = { 0x000E0012, 0x000E0013, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic6[]   = { 0x000E0002, 0x000E0004, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic7[]   = { 0x000E0005, 0x000E0006, 0xFFFFFFFF };
#if defined(VERSION_GMSJ01)
static u32 scDolpicNewsDolpic8_1[] = { 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_2[] = { 0x000E0007, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_3[] = { 0x000E0008, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_4[] = { 0x000E000E, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_5[] = { 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_6[]
    = { 0x000E000E, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_7[] = { 0x000E0010, 0x000E0003, 0xFFFFFFFF };
#else
static u32 scDolpicNewsDolpic8_Aa1[]
    = { 0x000E0007, 0x000E000E, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Aa2[]
    = { 0x000E0007, 0x000E000E, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Aa3[]
    = { 0x000E0007, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Aa4[] = { 0x000E0007, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ab1[]
    = { 0x000E0008, 0x000E000E, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ab2[]
    = { 0x000E0008, 0x000E000E, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ab3[]
    = { 0x000E0008, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ab4[] = { 0x000E0008, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ac1[] = { 0x000E000E, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ac2[]
    = { 0x000E000E, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ac3[] = { 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ac4[] = { 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ba1[]
    = { 0x000E0007, 0x000E000E, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ba2[] = { 0x000E0007, 0x000E000E, 0x000E000F,
	                                     0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ba3[]
    = { 0x000E0007, 0x000E000F, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ba4[]
    = { 0x000E0007, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bb1[]
    = { 0x000E0008, 0x000E000E, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bb2[] = { 0x000E0008, 0x000E000E, 0x000E000F,
	                                     0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bb3[]
    = { 0x000E0008, 0x000E000F, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bb4[]
    = { 0x000E0008, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bc1[]
    = { 0x000E000E, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bc2[]
    = { 0x000E000E, 0x000E000F, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bc3[]
    = { 0x000E000F, 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Bc4[] = { 0x000E0003, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ca1[]
    = { 0x000E0007, 0x000E000E, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ca2[]
    = { 0x000E0007, 0x000E000E, 0x000E000F, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ca3[]
    = { 0x000E0007, 0x000E000F, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Ca4[] = { 0x000E0007, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cb1[]
    = { 0x000E0008, 0x000E000E, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cb2[]
    = { 0x000E0008, 0x000E000E, 0x000E000F, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cb3[]
    = { 0x000E0008, 0x000E000F, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cb4[] = { 0x000E0008, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cc1[] = { 0x000E000E, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cc2[]
    = { 0x000E000E, 0x000E000F, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cc3[] = { 0x000E000F, 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Cc4[] = { 0x000E0010, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Da1[] = { 0x000E0007, 0x000E000E, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Da2[]
    = { 0x000E0007, 0x000E000E, 0x000E000F, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Da3[] = { 0x000E0007, 0x000E000F, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Da4[] = { 0x000E0007, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Db1[] = { 0x000E0008, 0x000E000E, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Db2[]
    = { 0x000E0008, 0x000E000E, 0x000E000F, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Db3[] = { 0x000E0008, 0x000E000F, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Db4[] = { 0x000E0008, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Dc1[] = { 0x000E000E, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Dc2[] = { 0x000E000E, 0x000E000F, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_Dc3[] = { 0x000E000F, 0xFFFFFFFF };
#endif
static u32 scDolpicNewsDolpic9[]  = { 0x000E000A, 0x000E000B, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic10[] = { 0x000E000C, 0x000E000D, 0xFFFFFFFF };

// rogue include
#include <M3DUtil/MActorAnm.hpp>

TGCConsole2::TGCConsole2(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(0)
    , unk18(LIFE_HIDDEN)
    , unk1C(0xffff)
    , unk20(0)
    , unk24(0)
    , unk28(0)
    , unk2C(0)
    , unk30(0xfffffe70)
    , unk34(0)
    , unk35(0)
    , unk36(0)
    , unk37(0)
    , unk38(0)
    , unk39(0)
    , unk3A(0)
    , unk3B(0)
    , unk3C(0)
    , unk3D(0)
    , unk3E(0)
    , unk3F(0)
    , unk40(0)
    , unk41(0)
    , unk42(0)
    , unk43(0)
    , unk44(0)
    , unk45(0)
    , unk46(0)
    , unk47(0)
    , unk48(0)
    , unk49(0)
    , unk4A(0)
    , unk4B(0)
    , unk4C(0)
    , unk4D(0)
    , unk4E(0)
    , unk4F(0)
    , unk50(0)
    , unk51(0)
    , unk54(0)
    , unk58(0)
    , unk59(0)
    , unk5A(0)
    , unk5C(0)
    , unk60(0)
    , unk64(0)
    , unk68(0)
    , unk6C(0)
    , unk78(20)
    , unk7C(0)
    , unk80(0)
    , unk84(0)
    , unk86(0)
    , unk88(0)
    , unk8A(0)
    , unk8C(0)
    , unk90(nullptr)
    , unk98(20)
    , unk9A(0x64, 0xDC, 0xFF, 0xFF)
    , unk9E(0x00, 0xB4, 0xF0, 0xFF)
    , unkA2(0xFF, 0xCF, 0x00, 0x7F)
    , unkA6(0xFF, 0x00, 0xFF, 0x7F)
    , unkAA(0xFF, 0x7F, 0x7F, 0x7F)
    , mHudScreen(nullptr)
    , unkB4(0)
    , unkB6(0)
    , unkB8(0)
    , unkBC(0)
    , unkC0(0)
    , unkC4(0)
    , mCoinCountBackground(nullptr)
    , mCoinCountIcon(nullptr)
    , mCoinCountX(nullptr)
    , mCoinCountRoot(nullptr)
    , mShineCountBackground(nullptr)
    , mShineCountIcon(nullptr)
    , mShineCountX(nullptr)
    , mShineCountRoot(nullptr)
    , mBlueCoinCountBackground(nullptr)
    , mBlueCoinCountIcon(nullptr)
    , mBlueCoinCountX(nullptr)
    , unk160(nullptr)
#if !defined(VERSION_GMSJ01)
    , unk164(nullptr)
    , unk16C(0)
    , unk170(0)
#endif
    , mLifeMeterBackground(nullptr)
    , mLifeMeterText(nullptr)
    , mLifeMeterRoot(nullptr)
    , unk1C8(0xFF6A)
    , unk1CA(0xB4)
    , mLifeMeterPetalsRoot(nullptr)
    , unk264(300)
    , unk266(0)
    , unk268(0)
    , unk26A(0)
    , unk26C(nullptr)
    , unk270(nullptr)
    , unk274(nullptr)
    , unk29C(nullptr)
    , unk2F8(nullptr)
    , unk30C(0)
    , unk310(0)
    , unk324(nullptr)
    , unk328(nullptr)
    , unk32C(nullptr)
    , unk330(0x17)
    , mLifeCountBackground(nullptr)
    , mLifeCountText(nullptr)
    , mLifeCountIcon(nullptr)
    , mLifeCountX(nullptr)
#if !defined(VERSION_GMSJ01)
    , unk3AE(0)
    , unk3AD(0)
#endif
    , unk3CC(0)
    , mBalloonMessageLoader(nullptr)
    , unk3D4(nullptr)
    , unk3D8(nullptr)
    , unk3DC(nullptr)
    , unk3E0(0)
    , unk3E4(0)
    , unk3E8(100)
    , unk3EC(0.7f)
    , unk3F0(0)
    , unk3F4(0xffffffff)
#if !defined(VERSION_GMSJ01)
    , unk3F8(0)
    , unk3FC(nullptr)
    , unk400(nullptr)
    , unk404(nullptr)
    , unk408(nullptr)
    , unk40C(nullptr)
    , unk410(nullptr)
#endif
    , unk426(0)
    , unk444(0)
    , unk448(0)
#if !defined(VERSION_GMSJ01)
    , unk4FC(0)
#endif
    , unk510(false)
#if !defined(VERSION_GMSJ01)
    , unk514(0)
    , unk518(0)
#endif
    , unk51C(0)
    , unk530(nullptr)
    , mTelopTextWidth(0)
    , unk558(0)
    , unk55C(0)
    , unk560(60)
    , unk562(5)
    , unk564(0.7f)
    , unk568(0.0f)
    , unk56C(1)
    , unk56D(1)
    , unk570(nullptr)
{
	for (int i = 0; i < 3; ++i)
		mCoinCountDigits[i] = nullptr;

	for (int i = 0; i < 10; ++i)
		unkE0[i] = nullptr;

	for (int i = 0; i < 3; ++i)
		mShineCountDigits[i] = nullptr;

#if !defined(VERSION_GMSJ01)
	for (int i = 0; i < 4; ++i)
		unk414[i] = nullptr;
#endif

	unk90 = new THelpActor*[32];
}

void TGCConsole2::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("game_6");
	mHudScreen       = new J2DSetScreen("standard_1.blo", arch);
	mHudScreen->setCullBack(GX_CULL_BACK);

	mCoinCountBackground = new TBoundPane(mHudScreen, 'c_ba');
	mCoinCountIcon       = new TBoundPane(mHudScreen, 'c_ic');
	mCoinCountX          = new TBoundPane(mHudScreen, '\0c_x');
	mCoinCountRoot       = new TExPane(mHudScreen, '\0c_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(mCoinCountRoot, -84);
#endif

	for (int i = 0; i < 3; ++i)
		mCoinCountDigits[i] = new TBlendPane(mHudScreen, 'c_n1' + i);

	for (int i = 0; i < 10; ++i) {
		char acStack_ac[40];
		snprintf(acStack_ac, 40, "/game_6/timg/coin_number_%d.bti", i);
		unkE0[i] = new JUTTexture((const ResTIMG*)JKRGetResource(acStack_ac));
	}

	mShineCountBackground = new TBoundPane(mHudScreen, 's_ba');
	mShineCountIcon       = new TBoundPane(mHudScreen, 's_ic');
	mShineCountX          = new TBoundPane(mHudScreen, '\0s_x');
	mShineCountRoot       = new TExPane(mHudScreen, '\0s_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(mShineCountRoot, -84);
#endif

	for (int i = 0; i < 3; ++i)
		mShineCountDigits[i] = new TBlendPane(mHudScreen, 's_n1' + i);

	mBlueCoinCountBackground = new TBoundPane(mHudScreen, 'd_ba');
	mBlueCoinCountIcon       = new TBoundPane(mHudScreen, 'd_ic');
	mBlueCoinCountX          = new TBoundPane(mHudScreen, '\0d_x');

	for (int i = 0; i < 3; ++i)
		mBlueCoinCountDigits[i] = new TBoundPane(mHudScreen, 'd_n1' + i);

	unk160 = new TExPane(mHudScreen, '\0d_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(unk160, -84);
#endif
	mLifeMeterBackground = new TBoundPane(mHudScreen, 'l_ba');
	mLifeMeterText       = new TBoundPane(mHudScreen, 'l_tx');
	mLifeMeterRoot       = new TBoundPane(mHudScreen, '\0l_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(mLifeMeterRoot, 84);
#endif
	for (int i = 0; i < 9; ++i) {
		mLifeMeterPetals[i * 2]     = mHudScreen->search('lm01' + (i << 8));
		mLifeMeterPetals[i * 2 + 1] = mHudScreen->search('lm02' + (i << 8));
		mLifeMeterPetalsOriginalBounds[i]
		    = mLifeMeterPetals[i * 2]->getBounds();
	}
	mLifeMeterPetalsRoot = new TBoundPane(mHudScreen, 'lm_0');

	unk26C = new TBoundPane(mHudScreen, 'w_ba');
	unk270 = new TBoundPane(mHudScreen, 'w_tx');

	for (int i = 0; i < 4; ++i) {
		unk278[i] = new TBoundPane(mHudScreen, 'nz01' + i);
		unk28C[i] = mHudScreen->search('xb01' + i);
	}

	unk274 = unk278[0];
	unk288 = unk28C[0];

	unk274->getPane()->setBasePosition(J2DBasePosition_5);
	unk288->show();

	unk29C = new TBoundPane(mHudScreen, 'w_t0');
	unk2F8 = new TExPane(mHudScreen, '\0w_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(unk2F8, 84);
#endif
	for (int i = 0; i < 3; ++i) {
		unk2A0[i] = (J2DPicture*)mHudScreen->search('w_t1' + i);

		unk2A0[i]->setBlendColorRatio(1.0f, 0.0f, 0.0f, 0.0f);
		unk2A0[i]->setBlendAlphaRatio(0.0f, 0.0f, 0.0f, 0.0f);

		unk2A0[i]->hide();

		if (i != 0) {
			unk2AC[i] = (J2DPicture*)mHudScreen->search('w_m1' + i);

			unk2A0[i]->setBlendColorRatio(0.0f, 0.0f, 0.0f, 0.0f);
			unk2A0[i]->setBlendAlphaRatio(1.0f, 0.0f, 0.0f, 0.0f);

			unk2AC[i]->hide();
		}
	}

	for (int i = 0; i < 4; ++i)
		unk314[i] = mHudScreen->search(i == 0 ? 'j_t0' : 'j_t1');
#if defined(VERSION_GMSP01_NX)
	unk314[0]->add(84, 0);
#endif

	unk324 = mHudScreen->search('j_ic');
	unk328 = (J2DPicture*)mHudScreen->search('j_rq');
	unk32C = (J2DPicture*)mHudScreen->search('j_ms');

	for (int i = 0; i < 22; ++i) {
		if (i < 9)
			unk334[i] = mHudScreen->search('j_01' + i);
		else
			unk334[i] = mHudScreen->search('j_0X' + i);
	}

	mLifeCountBackground = new TBoundPane(mHudScreen, 'm_ba');
	mLifeCountIcon       = new TBoundPane(mHudScreen, 'm_ic');
	mLifeCountText       = new TBoundPane(mHudScreen, 'm_tx');
	mLifeCountX          = new TBoundPane(mHudScreen, '\0m_x');
	mLifeCountRoot       = new TExPane(mHudScreen, '\0m_0');

	for (int i = 0; i < 3; ++i)
		mLifeCountDigits[i] = new TBoundPane(mHudScreen, 'm_n1' + i);

	unk3B0 = (J2DWindow*)mHudScreen->search('he_w');
#if defined(VERSION_GMSP01_NX)
	unk3B0->add(84, 0);
#endif
	unk3B8 = (J2DTextBox*)mHudScreen->search('he_2');
#if defined(VERSION_GMSP01_NX)
	unk3B8->add(84, 0);
#endif
	unk3B4 = (J2DTextBox*)mHudScreen->search('he_1');

	SMSMakeTextBuffer(unk3B8, 0x401);
	SMSMakeTextBuffer(unk3B4, 0x401);

	unk3B8->setFont(gpSystemFont);
	unk3B4->setFont(gpSystemFont);

	unk3B4->show();
	mBalloonMessageLoader = new TMessageLoader(
	    VERSION_SELECT("/common/2d/balloon.bmg", GMSP01("/cmn2d/balloon.bmg"),
	                   GMSP01_NX("/cmn2d/balloon.bmg")));

	unk3FC = new TExPane(mHudScreen, '\0b_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(unk3FC, -84);
#endif
	unk400 = new TBoundPane(mHudScreen, 'b_ba');
	unk408 = new TBoundPane(mHudScreen, 'b_ic');
	unk40C = new TBoundPane(mHudScreen, 'moic');
	unk410 = new TBoundPane(mHudScreen, 'b_sl');

	for (int i = 0; i < 4; ++i)
		unk414[i] = new TBlendPane(mHudScreen, 'b_n1' + i);

	((J2DPicture*)unk414[2]->getPane())
	    ->changeTexture(unkE0[8]->getTexInfo(), 0);

	unk428 = new TExPane(mHudScreen, '\0r_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(unk428, -84);
#endif
	unk42C = new TBoundPane(mHudScreen, 'r_ba');
	unk430 = new TBoundPane(mHudScreen, 'r_ic');
	unk434 = new TBoundPane(mHudScreen, '\0r_x');
	unk438 = new TBoundPane(mHudScreen, 'r_sl');

	for (int i = 0; i < 2; ++i)
		unk43C[i] = new TBoundPane(mHudScreen, 'r_n1' + i);

	unk44C = new TExPane(mHudScreen, '\0t_0');
#if defined(VERSION_GMSP01_NX)
	shiftWidePane(unk44C, -84);
#endif
	unk450 = new TBoundPane(mHudScreen, 't_ba');
	unk454 = new TBoundPane(mHudScreen, 't_tx');

	for (int i = 0; i < 9; ++i)
		unk458[i] = new TBoundPane(mHudScreen, 't_n1' + i);

	unk458[9] = new TBoundPane(mHudScreen, 't_n0');

	// TODO: is this the right cast?
	unk508 = ((J2DPicture*)unk458[6]->getPane())->getWhite();
	unk50C = ((J2DPicture*)unk458[7]->getPane())->getWhite();

	for (int i = 0; i < 3; ++i)
		unk480[i] = new TBoundPane(mHudScreen, 't_c1' + i);

	for (int i = 0; i < 2; ++i)
		unk500[i] = mHudScreen->search('\0t_1' + i);

	mTelopRoot = new TExPane(mHudScreen, 'te_0');
	mTelop     = new TExPane(mHudScreen, 'te_w');

	unk528 = (J2DTextBox*)mHudScreen->search('tet2');
	unk52C = (J2DTextBox*)mHudScreen->search('tet1');

	unk530 = new TMessageLoader(
	    VERSION_SELECT("/common/2d/infomess.bmg", GMSP01("/cmn2d/infomess.bmg"),
	                   GMSP01_NX("/cmn2d/infomess.bmg")));
	unk55C = 0;
	SMSMakeTextBuffer(unk528, 0x401);
	SMSMakeTextBuffer(unk52C, 0x401);
}

void TGCConsole2::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	unk94 = static_cast<TConsoleStr*>(
	    JDrama::TNameRefGen::search("コンソール文字"));

	JUTRect bounds(unk2F8->getPane()->getBounds());

	unk2A0[0]->add(bounds.x1, bounds.y1);
	unk270->getPane()->add(bounds.x1, bounds.y1);
	syncPaneBounds(unk270);
	unk26C->getPane()->add(bounds.x1, bounds.y1);
	syncPaneBounds(unk26C);
	unk328->add(bounds.x1, bounds.y1);

	for (int i = 0; i < 3; ++i) {
		unk2BC[i] = unk2A0[i]->getBounds();
		unk2BC[i].add(bounds.x1, bounds.y1);
	}

	unk2A0[0]->show();
	unk2A0[0]->removeFromParent();
	detachBoundPaneFromParent(unk270);
	for (int i = 0; i < 4; ++i) {
		unk278[i]->getPane()->add(bounds.x1, bounds.y1);
		syncPaneBounds(unk278[i]);
		detachBoundPaneFromParent(unk278[i]);
	}
	detachBoundPaneFromParent(unk26C);
	unk328->removeFromParent();
	unk288->hide();

	unk2EC[1] = JUtility::TColor(0x64DCFF00);
	unk2EC[2] = JUtility::TColor(0x00B4F000);

	s16 health = gpMarioOriginal->getHealth();
	if (health < 0)
		health = 0;

	mLifeMeterRoot->getPane()->hide();
	for (int i = 0; i < 9; ++i) {
		if (i < health + 1)
			mLifeMeterPetals[i * 2]->show();
		else
			mLifeMeterPetals[i * 2]->hide();
	}
	unk1CC = health;
	unk1C  = health;

	unk26A = mShineCountRoot->getPane()->getBounds().y1
	         - mCoinCountRoot->getPane()->getBounds().y1;

	initHiddenPaneAbove(mShineCountRoot);
	mShineCountRoot->getPane()->hide();
	initHiddenPaneAbove(unk160);
	unk160->getPane()->hide();
	initHiddenPaneOffset(mCoinCountRoot, unk26A);
	mCoinCountRoot->getPane()->hide();
	initHiddenPaneAbove(mLifeCountRoot);
	mLifeCountRoot->getPane()->hide();

	unk168 = TFlagManager::getInstance()->getFlag(MSF_BLUE_COIN_COUNT);

	int blueCoins = 0;
	for (int flag = 0x46; flag < 0x56; ++flag)
		if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag) != 0)
			++blueCoins;

	for (int flag = 0x6C; flag <= 0x73; ++flag)
		if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag) != 0)
			++blueCoins;

	blueCoins = unk168 - blueCoins * 10;
	if (blueCoins < 0)
		blueCoins = 0;
	setBlueCoinDigits(mBlueCoinCountDigits, unkE0, blueCoins);
	unk170 = blueCoins;

	unk20 = TFlagManager::getInstance()->getFlag(MSF_GOLD_COIN_COUNT);
	if (unk20 > 999)
		unk20 = 999;
	else if (unk20 < 0)
		unk20 = 0;
	unk6C = unk20;
	setCounterDigits(mCoinCountDigits, unkE0, unk20);

	unk24 = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
	if (unk24 > 999)
		unk24 = 999;
	else if (unk24 < 0)
		unk24 = 0;
	unk64 = unk24;
	setShineDigits(mShineCountDigits, unkE0, unk24);

	int lives = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);
	if (lives > 99)
		lives = 99;
	unk3AC = lives;
	setTwoDigits(mLifeCountDigits, unkE0, lives);

	unk39 = 1;

	unk2F8->getPane()->hide();
	mLifeCountRoot->getPane()->hide();
	unk270->getPane()->hide();
	unk274->getPane()->hide();
	unk29C->getPane()->hide();
	mTelopRoot->getPane()->hide();
	mTelop->getPane()->show();
	unk528->hide();
	unk528->setFont(gpSystemFont);
	JUTRect textBounds(unk528->getBounds());
	unk528->resize(gpSystemFont->getWidth() << 10, textBounds.getHeight());

	unk52C->hide();
	unk52C->setFont(gpSystemFont);
	unk52C->resize(gpSystemFont->getWidth() << 10, textBounds.getHeight());

	JUTRect telopBounds(mTelop->getPane()->getBounds());
	bounds = mTelopRoot->getPane()->getBounds();
	telopBounds.add(bounds.x1, bounds.y1 - 3);
	unk544.set(telopBounds.x1 + 8, telopBounds.y1 + 8, telopBounds.x2 - 8,
	           telopBounds.y2 - 8);

	unk534.add(unk544.x2, unk544.y1 + unk528->getHeight());

#if defined(VERSION_GMSP01_NX)
	unk544.add(0, -16);
	unk568    = unk544.x2;
	unk544.x1 = (unk544.x1 + 110) * 640.0f / 820.0f;
	unk544.x2 = (unk544.x2 + 110) * 640.0f / 820.0f;
	unk544.y1 = unk544.y1 * 360.0f / 448.0f;
	unk544.y2 = unk544.y2 * 360.0f / 448.0f;
#else
	unk544.resize(unk544.getWidth() * (16.0f / 15.0f), unk544.getHeight());
	unk544.add(0, -16);
	unk568 = unk544.x2;
#endif

	unk3BC = unk3B0->getBounds();
	unk3CC = 30;

	unk3D8 = new JSUMemoryOutputStream(unk3B4->getStringPtr(), 0x400);
	unk3DC = new JSUMemoryOutputStream(unk3B8->getStringPtr(), 0x400);
	unk3D4 = new JSUMemoryInputStream(nullptr, 0x400);

	unk48C = unk450->getPane()->getBounds();
	unk49C = unk458[0]->getPane()->getBounds();
	unk4AC = unk480[0]->getPane()->getBounds();

	for (int i = 0; i < 6; ++i) {
		JUTRect bounds(unk458[i]->getPane()->getBounds());
		unk4BC[i].set(bounds.x1, bounds.y1);
	}

	for (int i = 0; i < 2; ++i) {
		JUTRect bounds(unk480[i]->getPane()->getBounds());
		unk4EC[i].set(bounds.x1, bounds.y1);
	}

	unk7A = gpModelWaterManager->unk5D5F;

	JGeometry::TVec3<f32> emitterPos(-20.0f, -20.0f, 0.0f);

	gpEmitterManager4D2->createEmitter(emitterPos, 0x1FB, nullptr, nullptr);
	unk124                  = gpEmitterManager4D2->unkC8[0][0];
	unk124->mChildSpawnRate = 0.01f;

	gpEmitterManager4D2->createEmitter(emitterPos, 0x1FB, nullptr, nullptr);
	unk164 = gpEmitterManager4D2->unkC8[0][0];
	unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
	unk164->mChildSpawnRate = 0.012f;

	gpEmitterManager4D2->createEmitter(emitterPos, 0x1FB, nullptr, nullptr);
	unk144 = gpEmitterManager4D2->unkC8[0][0];
	unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);

	unk28 = gpMarioOriginal->mWaterGun->getMaxWater();

	unkBC = static_cast<TBathtub*>(JDrama::TNameRefGen::search("バスタブ"));
#if !defined(VERSION_GMSJ01)
	if (unkBC != nullptr) {
		unk310 = TWaterGun::Rocket;
		unk274 = unk278[2];
		unk288 = unk28C[2];
		unk274->getPane()->setBasePosition(J2DBasePosition_5);
		unk288->show();
	}
#endif
	unkC0 = static_cast<TBossEel*>(JDrama::TNameRefGen::search("めおとウナギ"));
	unkC4 = JDrama::TNameRefGen::search("ピーチ姫");
}

void TGCConsole2::entryHelpActor(THelpActor* param_1)
{
	if (unk8C < 32) {
		unk90[unk8C] = param_1;

		static_cast<TIdxGroupObj*>(
		    JDrama::TNameRefGen::search("マップグループ"))
		    ->getChildren()
		    .push_back(param_1);

		++unk8C;
	}
}

void TGCConsole2::startCameraDemo()
{
	bool marioDead = true;
	if (gpMarioOriginal->getHealth() != 0 && gpMarioOriginal->getAir() != 0)
		marioDead = false;

	if (marioDead) {
		mLifeMeterRoot->getPane()->hide();
		unk274->getPane()->hide();
		unk2F8->getPane()->hide();
		unk270->getPane()->hide();
		unk26C->getPane()->hide();
		mLifeCountRoot->getPane()->hide();
		unk44C->getPane()->hide();
		unk3FC->getPane()->hide();
		unk428->getPane()->hide();
		unk46 = 0;
		unk59 = 0;
		unk5A = 0;
		return;
	}

	if (unk50
	    || !TFlagManager::getInstance()->getBool(MSF_LOST_LIFE_IN_PREV_STAGE)
	           && unk39)
		return;

	unk50 = 1;

	startDisappearTelop();
	startDisappearLife(120);

	unk18 = LIFE_CAMERA_DEMO;
	unk48 = 0;

	unk274->setPanePosition(1, JUTPoint(0, 0), JUTPoint(0, 0), JUTPoint(0, 0));
	unk274->update();
	unk26C->setPanePosition(1, JUTPoint(0, 0), JUTPoint(0, 0), JUTPoint(0, 0));
	unk26C->update();
	unk270->setPanePosition(1, JUTPoint(0, 0), JUTPoint(0, 0), JUTPoint(0, 0));
	unk270->update();

	if (unkBC != nullptr) {
		if (!unkBC->unk29A)
			startDisappearTank();
	} else if (unkC0 != nullptr) {
		if (!unkC0->isInBossEelMoguDemo())
			startDisappearTank();
	} else {
#if defined(VERSION_GMSJ01_01)
		if (SMSGetMarDirector()->getCurrentStage() != 0x3A
		    || SMSGetMarDirector()->getCurrentScenario() != 1)
			startDisappearTank();
#else
		startDisappearTank();
#endif
	}

	startDisappearMario();
	startDownLeftBot();

	if (TFlagManager::getInstance()->getBool(MSF_LOST_LIFE_IN_PREV_STAGE)) {
		mCoinCountRoot->getPane()->hide();
		startAppearMario(true);
	} else if (gpMarDirector->checkFlag(
	               TMarDirector::DIRECTOR_FLAG_SHINE_TAKEN)) {
		startAppearStar();
	} else {
		startDisappearStar();
		startDisappearCoin();
	}

	unkB6 = 0;
}

void TGCConsole2::resetMoveTank() { }

void TGCConsole2::endCameraDemo()
{
	if (unk39)
		return;

	if (!unk50)
		return;

	// TODO: the ROM has one extra branch here, as if the whole body
	// were nested in the unk50 test rather than an early return.

	if (unkB6 < 3) {
		++unkB6;
		return;
	}

	unk50 = 0;

	if (!unk2F8->isInterpolatorAtZero() && !unk45
	    && !TFlagManager::getInstance()->getBool(MSF_LOST_LIFE_IN_PREV_STAGE)) {
		unk45 = 1;
		unk59 = 1;
		unk7C = 0;
		unk2F8->getPane()->show();
		unk2F8->setPaneOffset(unk98, 0, 0, 0, 465 - unk2F8->mInitialBounds.y1);
		unk26C->setPanePosition(50, JUTPoint(0, 100), JUTPoint(0, -30),
		                        JUTPoint(0, -30));
		unk274->getPane()->hide();
		unk29C->getPane()->hide();
	}

	unk40 = 1;
	unk41 = 0;
	unk59 = 1;

	if (unk51C) {
		startInsertTimer();
		unk51C = 0;
	}

	if (unk448) {
		startAppearRedCoin();
		unk448 = 0;
	}

	if (unk426) {
		startInsertJetBalloon();
		unk426 = 0;
	}

	if (!SMSGetMarDirector()->checkFlag(TMarDirector::DIRECTOR_FLAG_SHINE_TAKEN)
	    && !mCoinCountRoot->getPane()->isVisible())
		startAppearCoin();
}

void TGCConsole2::startAppearTank()
{
	if (unk45
	    || TFlagManager::getInstance()->getBool(MSF_LOST_LIFE_IN_PREV_STAGE)) {
		return;
	}

	// TODO: needs register swapping
	unk45 = 1;
	unk59 = 1;
	unk7C = 0;

	unk2F8->getPane()->show();
	unk2F8->setPaneOffset(unk98, 0, 0, 0, 465 - unk2F8->mInitialBounds.y1);

	unk26C->setPanePosition(50, JUTPoint(0, 100), JUTPoint(0, -30),
	                        JUTPoint(0, -30));

	unk274->getPane()->hide();
	unk29C->getPane()->hide();
}

void TGCConsole2::startDisappearTank()
{
	unk4B = 1;
	unk5A = 1;

	int offset = getBelowScreenOffsetY(unk2F8) + 60;
	unk2F8->updatePaneOffset(40, 0, offset);

	JUTPoint start(0, 0);
	JUTPoint mid(0, offset >> 1);
	JUTPoint end(0, offset);
	unk48 = 0;
	unk274->setPanePosition(40, start, mid, end);
	unk270->setPanePosition(40, start, mid, end);
	unk26C->setPanePosition(40, start, mid, end);
#if !defined(VERSION_GMSJ01)
	if (gpMarioOriginal->mYoshi->onYoshi()) {
		unk324->hide();
		for (int i = 0; i < 4; ++i)
			unk314[i]->hide();
	}
#endif
}

void TGCConsole2::startAppearCoin()
{
	if (mCoinCountRoot->getPane()->isVisible()) {
		return;
	}

	unk4F = 1;
	unk59 = 1;
	unk88 = 0;

	mCoinCountRoot->getPane()->show();
	mCoinCountRoot->setPaneOffset(unk98, 0, unk26A, 0,
	                              getHiddenOffsetY(mCoinCountRoot));

	mCoinCountBackground->setPanePosition(50, cDownTopPoint, cDownMidPoint,
	                                      cDownMidPoint);

	mCoinCountIcon->getPane()->hide();
	mCoinCountX->getPane()->hide();
	for (int i = 0; i < 3; i++) {
		mCoinCountDigits[i]->getPane()->hide();
	}

	unk124->clearStatus(JPABaseEmitter::STATUS_STOP_EMIT);
}

void TGCConsole2::startDisappearCoin()
{
	unk4D = true;
	unk5A = true;

	if (mShineCountRoot->isInterpolatorAtZero())
		mShineCountRoot->updatePaneOffset(
		    40, 0,
		    -(mShineCountRoot->mInitialBounds.y2 + 1
		      + mShineCountBackground->getPane()->getHeight()));

	mCoinCountRoot->updatePaneOffset(
	    40, 0,
	    getHiddenOffsetY(mCoinCountRoot)
	        - mCoinCountBackground->getPane()->getHeight());

	unk124->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
}

void TGCConsole2::startInsertLife(int param_1)
{
	if (param_1 == 0) {
		for (int i = 1; i < 9; ++i) {
			((J2DPicture*)mLifeMeterPetals[i * 2])->setWhite(0xFFFFFFFF);
			((J2DPicture*)mLifeMeterPetals[i * 2])->setBlack(0);
			if (gpMarioOriginal->getHealth() + 1 > i)
				mLifeMeterPetals[i * 2]->show();
			else
				mLifeMeterPetals[i * 2]->hide();
		}
		((J2DPicture*)mLifeMeterText->getPane())->setWhite(0xFFFFFFFF);
		((J2DPicture*)mLifeMeterText->getPane())->setBlack(0);
		((J2DPicture*)mLifeMeterBackground->getPane())->setWhite(0xFF4C00C8);
		((J2DPicture*)mLifeMeterBackground->getPane())->setBlack(0xFF4C0000);
		if (unk1CC >= 4) {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0xFFFFFFFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0xFFFFFFFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0);
		} else {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x7F7F7FFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0x7F7F7FFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0);
		}
	} else if (param_1 == 1) {
		for (int i = 1; i < 9; ++i) {
			((J2DPicture*)mLifeMeterPetals[i * 2])->setWhite(0x00FFFFFF);
			((J2DPicture*)mLifeMeterPetals[i * 2])->setBlack(0x003CFF00);
			if (gpMarioOriginal->getAir() + 1 > i)
				mLifeMeterPetals[i * 2]->show();
			else
				mLifeMeterPetals[i * 2]->hide();
		}
		((J2DPicture*)mLifeMeterBackground->getPane())->setWhite(0x0000FF78);
		((J2DPicture*)mLifeMeterBackground->getPane())->setBlack(0x0000FF00);
		if (unk1CC >= 4) {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x00FFFFFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0x003CFF00);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0x00FFFFFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0x003CFF00);
		} else {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x0010FFFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0x003CFF00);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0x0010FFFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0x003CFF00);
		}
	}

	unk51 = 1;
	unk84 = 0;

	int offset = -(mLifeMeterRoot->unk4.y2 + 1);
	mLifeMeterRoot->getPane()->show();
#if !defined(VERSION_GMSJ01)
	mLifeMeterRoot->getPane()->setAlpha(0xFF);
#endif
	mLifeMeterRoot->setPanePosition(unk98, JUTPoint(0, offset),
	                                JUTPoint(0, offset >> 1), JUTPoint(0, 0));
	mLifeMeterBackground->setPanePosition(50, cDownTopPoint, cDownMidPoint,
	                                      cDownMidPoint);
	mLifeMeterText->getPane()->hide();
	mLifeMeterPetalsRoot->getPane()->hide();
}

void TGCConsole2::resetLife(int param_1)
{
	for (int i = 1; i < 9; i++) {
		if (i < param_1 + 1) {
			mLifeMeterPetals[2 * i]->show();
		} else {
			mLifeMeterPetals[2 * i]->hide();
		}

		JUTRect local_38(mLifeMeterPetalsOriginalBounds[i].x1,
		                 mLifeMeterPetalsOriginalBounds[i].y1,
		                 mLifeMeterPetalsOriginalBounds[i].x2,
		                 mLifeMeterPetalsOriginalBounds[i].y2);
		mLifeMeterPetals[2 * i]->place(local_38);

		JUTRect local_48(mLifeMeterPetalsOriginalBounds[i].x1,
		                 mLifeMeterPetalsOriginalBounds[i].y1,
		                 mLifeMeterPetalsOriginalBounds[i].x2,
		                 mLifeMeterPetalsOriginalBounds[i].y2);
		mLifeMeterPetals[2 * i + 1]->place(local_48);
	}
}

bool TGCConsole2::startAppearLife(int param_1)
{
	if (unk38 || unk50 || gpMarioOriginal->getHealth() == 0)
		return false;

	if (param_1 == 0) {
		for (int i = 0; i < 9; ++i) {
			((J2DPicture*)mLifeMeterPetals[i * 2])->setWhite(0xFFFFFFFF);
			((J2DPicture*)mLifeMeterPetals[i * 2])->setBlack(0);
			if (gpMarioOriginal->getHealth() + 1 > i)
				mLifeMeterPetals[i * 2]->show();
			else
				mLifeMeterPetals[i * 2]->hide();
		}
		((J2DPicture*)mLifeMeterText->getPane())->setWhite(0xFFFFFFFF);
		((J2DPicture*)mLifeMeterText->getPane())->setBlack(0);
		((J2DPicture*)mLifeMeterBackground->getPane())->setWhite(0xFF4C00C8);
		((J2DPicture*)mLifeMeterBackground->getPane())->setBlack(0xFF4C0000);
		if (unk1CC >= 4) {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0xFFFFFFFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0xFFFFFFFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0);
		} else {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x7F7F7FFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0x7F7F7FFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0);
		}
	} else if (param_1 == 1) {
		for (int i = 0; i < 9; ++i) {
			((J2DPicture*)mLifeMeterPetals[i * 2])->setWhite(0x00FFFFFF);
			((J2DPicture*)mLifeMeterPetals[i * 2])->setBlack(0x003CFF00);
			if (gpMarioOriginal->getAir() + 1 > i)
				mLifeMeterPetals[i * 2]->show();
			else
				mLifeMeterPetals[i * 2]->hide();
		}
		((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x00FFFFFF);
		((J2DPicture*)mLifeMeterText->getPane())->setBlack(0x003CFF00);
		((J2DPicture*)mLifeMeterBackground->getPane())->setWhite(0x0000FF78);
		((J2DPicture*)mLifeMeterBackground->getPane())->setBlack(0x0000FF00);
		if (unk1CC >= 4) {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x00FFFFFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0x003CFF00);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0x00FFFFFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0x003CFF00);
		} else {
			((J2DPicture*)mLifeMeterText->getPane())->setWhite(0x0010FFFF);
			((J2DPicture*)mLifeMeterText->getPane())->setBlack(0x003CFF00);
			((J2DPicture*)mLifeMeterPetals[0])->setWhite(0x0010FFFF);
			((J2DPicture*)mLifeMeterPetals[0])->setBlack(0x003CFF00);
		}
	}

	unk38 = 1;
	unk84 = 0;
	mLifeMeterRoot->getPane()->add(unk1C8, unk1CA);
	return true;
}

bool TGCConsole2::startDisappearLife(int param_1)
{
	if (unk38 || unk4C)
		return false;

	unk4C = 1;
	mLifeMeterRoot->setPanePosition(
	    40, JUTPoint(0, 0),
	    JUTPoint(0, (getHiddenOffsetY(mLifeMeterRoot)
	                 - mLifeMeterBackground->getPane()->getHeight())
	                    >> 1),
	    JUTPoint(0, getHiddenOffsetY(mLifeMeterRoot)
	                    - mLifeMeterBackground->getPane()->getHeight()));
	unk84 = param_1;
	return true;
}

void TGCConsole2::startDownLeftBot()
{
	if (unk41) {
		return;
	}

	unk41 = 1;
	unk40 = 0;
	unk5A = 1;

	if (unk44C->getPane()->isVisible() && unk44C->isInterpolatorAtZero()) {
		unk44C->updatePaneOffset(20, 0, getBelowScreenOffsetY(unk44C) + 60);
		unk51C = 1;
	}

	if (unk428->getPane()->isVisible()) {
		unk428->updatePaneOffset(20, 0, getBelowScreenOffsetY(unk428) + 60);
		unk448 = 1;
	}

	if (unk3FC->getPane()->isVisible()) {
		unk3FC->updatePaneOffset(20, 0, getBelowScreenOffsetY(unk3FC) + 60);
		unk426 = 1;
	}
}

void TGCConsole2::startUpLeftBot() { }

void TGCConsole2::startAppearTelop(bool param_1)
{
	if (unk50) {
		return;
	}
	if (unk530->unk4 == nullptr) {
		return;
	}
	if (unk570 == 0 || unk44C->getPane()->isVisible()) {
		return;
	}
	if (!(param_1 || unk44)) {
		return;
	}

	unk42  = 1;
	unk59  = 1;
	unk56D = 1;
	mTelopRoot->getPane()->show();

	mTelopRoot->setPaneOffset(80, 0, 0, 0, 465 - mTelopRoot->mInitialBounds.y1);

	if (param_1) {
		// TODO: needs regswapping
		const u8* messageText
		    = &unk530->getMessageData()[unk530->unk8[unk570[unk558] & 0xffff]
		                                    .unk0];

		snprintf(unk528->getStringPtr(), 0x3ff, "%s", messageText);
		snprintf(unk52C->getStringPtr(), 0x3ff, "%s", messageText);

		J2DPrint print(gpSystemFont, 0);
		mTelopTextWidth = print.getWidth(unk528->getStringPtr());

		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_NEWS, 0, nullptr, 0);
	}
}

void TGCConsole2::startDisappearTelop()
{
	if (unk43 || !mTelopRoot->getPane()->isVisible()) {
		return;
	}

	unk43 = 1;
	unk5A = 1;

	mTelopRoot->updatePaneOffset(80, 0,
	                             465 - mTelopRoot->getInitialBounds().y1);
}

void TGCConsole2::startDisappearTimer()
{
	unk44C->updatePaneOffset(40, 0, getBelowScreenOffsetY(unk44C) + 60);
	unk3F = 1;
	unk5A = 1;
}

void TGCConsole2::startAppearTimer(int param_1, s32 param_2)
{
	startDisappearTelop();

	if (param_1 == 0) {
		unk510 = true;
		unk514 = 0;
	} else {
		unk510 = false;
		unk514 = param_2 * 100;
	}

	if (unk510 || param_2 >= 0x3C) {
		unk500[0]->show();
		unk500[1]->hide();
	} else {
		unk500[0]->hide();
		unk500[1]->show();
	}

	if (param_2 > 0) {
		setTimer(unk514);
	}

	for (int i = 6; i <= 9; i++) {
		((J2DPicture*)unk458[i]->getPane())->setWhite(unk50C);
	}
	((J2DPicture*)unk480[2]->getPane())->setWhite(unk50C);

	startInsertTimer();
}

void TGCConsole2::startInsertTimer()
{
	unk3E = 1;

	for (int i = 0; i < 10; i++) {
		unk458[i]->getPane()->hide();
	}

	for (int i = 0; i < 3; i++) {
		unk480[i]->getPane()->hide();
	}

	unk59 = 1;

	unk44C->getPane()->show();
	unk44C->setPaneOffset(40, 0, 0, 0, 465 - unk44C->getInitialBounds().y1);

	unk450->getPane()->show();
	unk450->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);

	unk454->getPane()->hide();
	unk518 = 0;
	unk76  = 0;
}

void TGCConsole2::startAppearJetBalloon(int nozzleKind, int count)
{
	unk424[0] = count;

	if (count >= 10) {
		((J2DPicture*)unk414[2]->getPane())
		    ->changeTexture(unkE0[(int)(count * 0.1f)]->getTexInfo(), 0);
		((J2DPicture*)unk414[3]->getPane())
		    ->changeTexture(unkE0[count % 10]->getTexInfo(), 0);
	} else {
		((J2DPicture*)unk414[2]->getPane())
		    ->changeTexture(unkE0[count]->getTexInfo(), 0);
	}

	if (nozzleKind == 0)
		unk404 = unk408;
	else if (nozzleKind == 1)
		unk404 = unk40C;

	startInsertJetBalloon();
}

void TGCConsole2::startInsertJetBalloon()
{
	unk3D = 1;
	unk59 = 1;

	unk3FC->getPane()->show();
	unk400->getPane()->show();
	unk400->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);

	unk404->getPane()->hide();
	unk410->getPane()->hide();
	unk72     = 0;
	unk424[1] = 0;

	for (int i = 0; i < 4; ++i)
		unk414[i]->getPane()->hide();

	if (unk404 == unk408)
		unk3FC->setPaneOffset(80, 0, 0, 0, 465 - unk3FC->mInitialBounds.y1);
	else
		unk3FC->setPaneOffset(80, 0, -73, 0, 465 - unk3FC->mInitialBounds.y1);
}

void TGCConsole2::startAppearRedCoin()
{
	unk3C = 1;
	unk59 = 1;

	unk428->getPane()->show();
	if (unk44C->getPane()->isVisible())
		unk428->setPaneOffset(40, 0, -73, 0, 465 - unk428->mInitialBounds.y1);
	else
		unk428->setPaneOffset(40, 0, 0, 0, 465 - unk428->mInitialBounds.y1);

	unk42C->getPane()->show();
	unk42C->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);

	unk430->getPane()->hide();
	unk438->getPane()->hide();
	unk434->getPane()->hide();
	unk74 = 0;

	for (int i = 0; i < 2; ++i)
		unk43C[i]->getPane()->hide();
}

// fabricated
static inline void closeBalloon(TGCConsole2* console)
{
	console->startDisappearBalloon(console->unk3E0, true);
}

void TGCConsole2::pauseIn()
{
	closeBalloon(this);
	startAppearMario(false);
	startAppearStar();
	startDisappearTelop();
	startDownLeftBot();
	unk59 = 0;
}

void TGCConsole2::pauseOut()
{
	startAppearTelop(false);

	unk40 = 1;
	unk41 = 0;
	unk59 = 1;

	if (unk51C) {
		startInsertTimer();
		unk51C = 0;
	}

	if (unk448) {
		startAppearRedCoin();
		unk448 = 0;
	}

	if (unk426) {
		startInsertJetBalloon();
		unk426 = 0;
	}

	startDisappearMario();
	startDisappearStar();
	unk5A = 0;
}

bool TGCConsole2::startDisappearBalloon(u32 param_1, bool param_2)
{
	if (!param_2 && unk3F4 == 0xffffffff && (param_1 != unk3E0 || unk3E4 != 0))
		return false;

	unk3B8->hide();
	unk48 = 0;
	unk10 = 4;
	return true;
}

bool TGCConsole2::startAppearBalloon(u32 messageID, bool autoClose)
{
	TMessageLoader::EntryInfo* entry
	    = mBalloonMessageLoader->getMessageEntry(messageID & 0xffff);

#if defined(VERSION_GMSJ01_01)
	if (entry->unk4 <= unk3F0 || gpMarioOriginal->getHealth() == 0)
		return false;
#else
	if (entry->unk4 <= unk3F0)
		return false;
#endif

	if (unk10 != 0) {
		if (unk3F4 == 0xffffffff) {
			unk3F4 = messageID;
			startDisappearBalloon(unk3E0, false);
			return true;
		}
		return false;
	}

	if (SMSGetMarDirector()->getState() == TMarDirector::STATE_PAUSE_MENU
	    || !unk46)
		return false;

	unk3F0 = entry->unk4;
	unk3B0->setAlpha(0);
	unk3B0->show();

	JUTRect contents(unk3B0->getContentsBounds());
	unk3B0->resize(unk3BC.getWidth(),
	               unk3BC.getHeight() - contents.getHeight());
	unk3B0->add(0, contents.getHeight());

	unk3D8->setBuffer(unk3B4->getStringPtr(), 0x400);
	unk3DC->setBuffer(unk3B8->getStringPtr(), 0x400);

	const u8* messageText
	    = mBalloonMessageLoader->getMessageData() + entry->unk0;
	unk3D4->setBuffer(messageText, 0x400);

	unk3E0     = messageID;
	size_t len = strlen((const char*)messageText);
	unk3F8     = autoClose;
	unk3E4     = (s32)(len * unk3EC + unk3E8);

	if (unk3E0 == VERSION_SELECT(0x000E002C, GMSJ01(0x000E002F)))
		unk3E4 = 0x96;

	if (unk3E4 <= 0)
		unk3E4 = 1;

	unk48 = 1;
	unk14 = 0;
	unk10 = 1;

	s32 soundID = scNozzleSoundList[(u8)entry->unk8[VERSION_SELECT(2, GMSJ01(0),
	                                                               GMSE01(0))]];
	if (soundID != -1)
		SMSGetMSound()->startSoundSystemSE(soundID, 0, nullptr, 0);

	return true;
}

void TGCConsole2::startDisappearStar()
{
	mShineCountRoot->updatePaneOffset(
	    40, 0, getHiddenOffsetY(mShineCountRoot) + unk26A);
	unk160->updatePaneOffset(40, 0, getHiddenOffsetY(unk160));
	mCoinCountRoot->updatePaneOffset(40, 0, unk26A);

	unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
	unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);

	unk35 = 1;
	unk5A = 1;
}

void TGCConsole2::startAppearStar()
{
	if (unk34 || mShineCountRoot->isInterpolatorAtZero())
		return;

	unk59 = 1;

	if (unk35) {
		mShineCountRoot->getPane()->hide();
		unk160->getPane()->hide();
		unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
		unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
		unk35 = 0;
	}

	mShineCountRoot->setPaneOffset(40, 0, 0, 0,
	                               getHiddenOffsetY(mShineCountRoot));
	mShineCountRoot->getPane()->show();
	mShineCountBackground->setPanePosition(50, cDownTopPoint, cDownMidPoint,
	                                       cDownMidPoint);

	mShineCountIcon->getPane()->hide();
	mShineCountX->getPane()->hide();
	for (int i = 0; i < 3; ++i)
		mShineCountDigits[i]->getPane()->hide();

	unk160->setPaneOffset(40, 0, 0, 0, getHiddenOffsetY(unk160));
	unk160->getPane()->show();
	mBlueCoinCountBackground->setPanePosition(50, cDownTopPoint, cDownMidPoint,
	                                          cDownMidPoint);

	mBlueCoinCountIcon->getPane()->hide();
	mBlueCoinCountX->getPane()->hide();
	for (int i = 0; i < 3; ++i)
		mBlueCoinCountDigits[i]->getPane()->hide();

	if (!mCoinCountRoot->getPane()->isVisible())
		mCoinCountRoot->getPane()->show();

	mCoinCountRoot->updatePaneOffset(40, 0, 0);
	mCoinCountBackground->setPanePosition(50, JUTPoint(0, 0), cCoinMidPoint,
	                                      cCoinMidPoint);

	unk144->clearStatus(JPABaseEmitter::STATUS_STOP_EMIT);
	unk164->clearStatus(JPABaseEmitter::STATUS_STOP_EMIT);

	unk5C = 0;
	unk34 = 1;
}

void TGCConsole2::drawWaterBack()
{
	if (gpMarioOriginal->getHealth() == 0 || gpMarioOriginal->getAir() == 0)
		return;

	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, 0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetNumTexGens(1);
	GXSetNumTevStages(1);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	((J2DPicture*)unk26C->getPane())->getTexture(0)->load(GX_TEXMAP0);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

	TWaterGun* waterGun = gpMarioOriginal->mWaterGun;
	JUTRect bounds(((J2DPicture*)unk26C->getPane())->getBounds());
	GXSetTevColor(GX_TEVREG0, JUtility::TColor(0x0000ff78));
	GXSetTevColor(GX_TEVREG1, JUtility::TColor(0x0000ff00));

	if (waterGun->isPressureOn()) {
		f32 pressure    = waterGun->getPressure();
		f32 pressureMax = waterGun->getPressureMax();
		f32 hiddenRatio = 1.0f - pressure / pressureMax;
		int fillTop     = bounds.y1 + (int)(hiddenRatio * bounds.getHeight());

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2f32(bounds.x1, bounds.y1);
		GXTexCoord2f32(0.0f, 0.0f);
		GXPosition2f32(bounds.x2, bounds.y1);
		GXTexCoord2f32(1.0f, 0.0f);
		GXPosition2f32(bounds.x2, fillTop);
		GXTexCoord2f32(1.0f, hiddenRatio);
		GXPosition2f32(bounds.x1, fillTop);
		GXTexCoord2f32(0.0f, hiddenRatio);
		GXEnd();

		if (!unk50 && pressure != 0.0f && !unk48) {
			unk14 = 1;
			unk48 = 1;
		}

		if (pressure == pressureMax) {
			if (unk49)
				unk49 = 0;

			u32 color = 0xff3f3f00;
			if (unk30C < 10) {
				s16 green = unk30C * -6.3f;
				s32 blue  = unk30C * 19.2f;
				color += green << 8;
				color += blue << 16;
			} else if ((int)unk30C < 15) {
				color = 0xffff0000;
			} else if ((int)unk30C < 25) {
				int fade  = 25 - unk30C;
				s16 green = fade * -6.3f;
				s32 blue  = fade * 19.2f;
				color += green << 8;
				color += blue << 16;
			} else {
				unk30C = 0;
			}
			GXSetTevColor(GX_TEVREG0, JUtility::TColor(color + 0xc8));
			GXSetTevColor(GX_TEVREG1, JUtility::TColor(color));
			++unk30C;
		} else {
			if (unk30C != 0)
				unk30C = 0;
			GXSetTevColor(GX_TEVREG0, JUtility::TColor(0xff3f3fc8));
			GXSetTevColor(GX_TEVREG1, JUtility::TColor(0xff3f3f00));
		}

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2f32(bounds.x1, fillTop);
		GXTexCoord2f32(0.0f, hiddenRatio);
		GXPosition2f32(bounds.x2, fillTop);
		GXTexCoord2f32(1.0f, hiddenRatio);
		GXPosition2f32(bounds.x2, bounds.y2);
		GXTexCoord2f32(1.0f, 1.0f);
		GXPosition2f32(bounds.x1, bounds.y2);
		GXTexCoord2f32(0.0f, 1.0f);
		GXEnd();
	} else {
		if (unk48 && unk30C != 0) {
			unk274->setPanePosition(90, JUTPoint(0, 0), JUTPoint(0, -100),
			                        JUTPoint(0, 0));
			unk30C = 0;
			unk49  = 1;
		}

		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2f32(bounds.x1, bounds.y1);
		GXTexCoord2f32(0.0f, 0.0f);
		GXPosition2f32(bounds.x2, bounds.y1);
		GXTexCoord2f32(1.0f, 0.0f);
		GXPosition2f32(bounds.x2, bounds.y2);
		GXTexCoord2f32(1.0f, 1.0f);
		GXPosition2f32(bounds.x1, bounds.y2);
		GXTexCoord2f32(0.0f, 1.0f);
		GXEnd();
	}

	if (unk334[unk330]->isVisible())
		unk334[unk330]->hide();
}

void TGCConsole2::startDisappearMario()
{
	if (!mLifeCountRoot->getPane()->isVisible() || unk3B)
		return;

	mLifeCountRoot->updatePaneOffset(50, 0, getHiddenOffsetY(mLifeCountRoot));
	unk3B = 1;
}

void TGCConsole2::startAppearMario(bool param_1)
{
	if (mLifeCountRoot->getPane()->isVisible() && !unk3B)
		return;

	mLifeCountRoot->getPane()->show();
	mLifeCountRoot->setPaneOffset(50, 0, 0, 0,
	                              getHiddenOffsetY(mLifeCountRoot));

	mLifeCountBackground->setPanePosition(50, cDownTopPoint, cDownMidPoint,
	                                      cDownMidPoint);

	mLifeCountIcon->getPane()->hide();
	mLifeCountText->getPane()->hide();
	mLifeCountX->getPane()->hide();
	for (int i = 0; i < 3; ++i)
		mLifeCountDigits[i]->getPane()->hide();

	if (param_1) {
		int lives = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT) + 1;
		if (lives > 99)
			lives = 99;

		((J2DPicture*)mLifeCountDigits[0]->getPane())
		    ->changeTexture(unkE0[(int)(lives * 0.1f)]->getTexInfo(), 0);
		((J2DPicture*)mLifeCountDigits[1]->getPane())
		    ->changeTexture(unkE0[lives % 10]->getTexInfo(), 0);
	}

	unk3A  = 1;
	unk3AD = param_1;
	unk3B  = 0;
	unk59  = 1;
	unk70  = 0;
}

void TGCConsole2::processMoveNozzle()
{
	if (!unk274->update())
		return;

	if (!unk48)
		return;

	switch (unk14) {
	case 1:
		if (unk30C == 0) {
			unk14 = 2;
			return;
		}
		unk274->setPanePosition(30, JUTPoint(0, 0), JUTPoint(0, -30),
		                        JUTPoint(0, 0));
		break;
	case 0:
		unk274->setPanePosition(50, JUTPoint(0, 0), JUTPoint(0, -20),
		                        JUTPoint(0, 0));
		break;
	case 2:
		unk274->setPanePosition(30, JUTPoint(0, 0), JUTPoint(0, -25),
		                        JUTPoint(0, 0));
		unk48 = 0;
		break;
	}
}

void TGCConsole2::changeNum(TBlendPane* pane, int num, int frames)
{
	pane->setPaneBlend(frames, unkE0[num], nullptr);
	JUTRect bounds(pane->getPane()->getGlbBounds());
	JGeometry::TVec3<f32> pos(bounds.x1 + bounds.getWidth() * 0.5f,
	                          bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
	gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr, nullptr);
}

#if !defined(VERSION_GMSJ01)
u32* TGCConsole2::checkDolpic8()
{
	int pinnaNokiNews;
	if (TFlagManager::getInstance()->getBool(MSF_PINNA_VISITED)) {
		if (TFlagManager::getInstance()->getBool(MSF_NOKI_AVAILABLE)
		    && !TFlagManager::getInstance()->getBool(MSF_NOKI_VISITED))
			pinnaNokiNews = 2;
		else
			pinnaNokiNews = 3;
	} else {
		if (TFlagManager::getInstance()->getBool(MSF_NOKI_AVAILABLE)
		    && !TFlagManager::getInstance()->getBool(MSF_NOKI_VISITED))
			pinnaNokiNews = 1;
		else
			pinnaNokiNews = 0;
	}

	int shadowMarioNews;
	switch (TFlagManager::getInstance()->getFlag(MSF_SHADOW_MARIO_EVENT)) {
	case 3:
		if (TFlagManager::getInstance()->getNozzleRight(1, 0))
			shadowMarioNews = 2;
		else
			shadowMarioNews = 1;
		break;
	case 2:
		if (TFlagManager::getInstance()->getNozzleRight(1, 1))
			shadowMarioNews = 2;
		else
			shadowMarioNews = 1;
		break;
	case 1:
		if (TFlagManager::getInstance()->getFlag(MSF_YOSHI_UNLOCKED) != 0)
			shadowMarioNews = 2;
		else
			shadowMarioNews = 0;
		break;
	default:
		shadowMarioNews = 2;
		break;
	}

	int yoshiNozzleNews;
	if (TFlagManager::getInstance()->getFlag(MSF_SHADOW_MARIO_EVENT) == 0
	    && TFlagManager::getInstance()->getFlag(MSF_YOSHI_UNLOCKED) != 0) {
		if (TFlagManager::getInstance()->getFlag(MSF_SHADOW_MARIO_EVENT) == 0
		    && (TFlagManager::getInstance()->getNozzleRight(1, 0)
		        || TFlagManager::getInstance()->getNozzleRight(1, 1)))
			yoshiNozzleNews = 1;
		else
			yoshiNozzleNews = 0;
	} else {
		if (TFlagManager::getInstance()->getFlag(MSF_SHADOW_MARIO_EVENT) == 0
		    && (TFlagManager::getInstance()->getNozzleRight(1, 0)
		        || TFlagManager::getInstance()->getNozzleRight(1, 1)))
			yoshiNozzleNews = 2;
		else
			yoshiNozzleNews = 3;
	}

	u32* news = unk570;
	switch (pinnaNokiNews) {
	case 0:
		switch (shadowMarioNews) {
		case 0:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Aa1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Aa2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Aa3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Aa4;
				break;
			}
#if !defined(VERSION_GMSE01)
			break;
#endif
		case 1:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Ab1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Ab2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Ab3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Ab4;
				break;
			}
#if !defined(VERSION_GMSE01)
			break;
#endif
		case 2:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Ac1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Ac2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Ac3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Ac4;
				break;
			}
#if !defined(VERSION_GMSE01)
			break;
#endif
		}
#if !defined(VERSION_GMSE01)
		break;
#endif
	case 1:
		switch (shadowMarioNews) {
		case 0:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Ba1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Ba2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Ba3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Ba4;
				break;
			}
			break;
		case 1:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Bb1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Bb2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Bb3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Bb4;
				break;
			}
			break;
		case 2:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Bc1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Bc2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Bc3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Bc4;
				break;
			}
			break;
		}
		break;
	case 2:
		switch (shadowMarioNews) {
		case 0:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Ca1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Ca2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Ca3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Ca4;
				break;
			}
			break;
		case 1:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Cb1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Cb2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Cb3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Cb4;
				break;
			}
			break;
		case 2:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Cc1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Cc2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Cc3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Cc4;
				break;
			}
			break;
		}
		break;
	case 3:
		switch (shadowMarioNews) {
		case 0:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Da1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Da2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Da3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Da4;
				break;
			}
			break;
		case 1:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Db1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Db2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Db3;
				break;
			case 3:
				news = scDolpicNewsDolpic8_Db4;
				break;
			}
			break;
		case 2:
			switch (yoshiNozzleNews) {
			case 0:
				news = scDolpicNewsDolpic8_Dc1;
				break;
			case 1:
				news = scDolpicNewsDolpic8_Dc2;
				break;
			case 2:
				news = scDolpicNewsDolpic8_Dc3;
				break;
			case 3:
				news = nullptr;
				break;
			}
			break;
		}
		break;
	}

	return news;
}
#endif

void TGCConsole2::setTimer(s32 param_1)
{
	u32 timerValue;

	if (param_1 == -1) {
		timerValue = SMSGetMarDirector()->getTimerTime();

		if (!unk510) {
			if (unk514 < timerValue) {
				timerValue = 0;
			} else {
				timerValue = unk514 - timerValue;
			}
		}
	} else {
		timerValue = param_1;
	}

	// Cap at 5999.99 seconds (99:59.99)
	if (timerValue > 599999) {
		timerValue = 599999;
	}

	u16 minutes          = (timerValue - timerValue % 100) / 6000;
	u32 timeMinusMinutes = timerValue - minutes * 6000;
	u16 seconds          = timeMinusMinutes * 0.01;
	u16 centis           = timeMinusMinutes - 100 * seconds;

	if (unk500[0]->isVisible()) {
		((J2DPicture*)unk458[0]->getPane())
		    ->changeTexture(unkE0[minutes / 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[1]->getPane())
		    ->changeTexture(unkE0[minutes % 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[2]->getPane())
		    ->changeTexture(unkE0[seconds / 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[3]->getPane())
		    ->changeTexture(unkE0[seconds % 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[4]->getPane())
		    ->changeTexture(unkE0[centis / 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[5]->getPane())
		    ->changeTexture(unkE0[centis % 10]->getTexInfo(), 0);
	} else {
		if (timerValue < 1000
		    && ((J2DPicture*)unk458[9]->getPane())->getWhite() != getUnk508()) {
			for (int i = 6; i <= 9; i++) {
				((J2DPicture*)unk458[i]->getPane())->setWhite(getUnk508());
			}
			((J2DPicture*)unk480[2]->getPane())->setWhite(getUnk508());
		}
		((J2DPicture*)unk458[6]->getPane())
		    ->changeTexture(unkE0[seconds / 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[7]->getPane())
		    ->changeTexture(unkE0[seconds % 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[8]->getPane())
		    ->changeTexture(unkE0[centis / 10]->getTexInfo(), 0);
		((J2DPicture*)unk458[9]->getPane())
		    ->changeTexture(unkE0[centis % 10]->getTexInfo(), 0);
	}

	if (timerValue != 0 && timerValue < unk518
	    && SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU) {
		SMSGetMSound()->playTimer(timerValue * 10);
	}

	unk4FC = timerValue;
}

#if !defined(VERSION_GMSJ01)
void TGCConsole2::countShine()
{
	int shines    = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
	int blueCoins = TFlagManager::getInstance()->getFlag(MSF_BLUE_COIN_COUNT);
	if (unk8A == 0 && unk64 != shines)
		unk8A = 1;

	if (unk8A != 0) {
		if (unk8A > 0xFB) {
			int spentBlueCoins = 0;
			for (int flag = 0x46; flag < 0x56; ++flag)
				if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
				    != 0)
					++spentBlueCoins;
			for (int flag = 0x6C; flag <= 0x73; ++flag)
				if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
				    != 0)
					++spentBlueCoins;

			if (unk170 != blueCoins - spentBlueCoins * 10) {
				--unk170;
				if (unk170 < 0) {
					unk170 = 0;
				} else if (SMSGetMSound()->gateCheck(0x405C)) {
					MSoundSESystem::MSoundSE::startSoundSystemSE(0x405C, 0,
					                                             nullptr, 0);
				}
				if (unk170 < 100) {
					setDigitPane(mBlueCoinCountDigits[0], unkE0,
					             (int)(unk170 * 0.1f));
					setDigitPane(mBlueCoinCountDigits[1], unkE0, unk170 % 10);
					if (mBlueCoinCountDigits[2]->getPane()->isVisible())
						mBlueCoinCountDigits[2]->getPane()->hide();
				} else {
					setDigitPane(mBlueCoinCountDigits[0], unkE0,
					             (int)(unk170 * 0.01f));
					int remainder = unk170 - (int)(unk170 * 0.01f) * 100;
					setDigitPane(mBlueCoinCountDigits[1], unkE0,
					             (int)(remainder * 0.1f));
					setDigitPane(mBlueCoinCountDigits[2], unkE0,
					             remainder % 10);
					if (!mBlueCoinCountDigits[2]->getPane()->isVisible())
						mBlueCoinCountDigits[2]->getPane()->show();
				}
			}
		}

		if (unk8A == 0xFC) {
			int value = unk64;
			// TODO: never used here. The coin counter in performCalcAnim
			// declares the same default JUTRect and uses it, so this one is
			// probably left over from code that was moved into changeNum.
			JUTRect bounds;

			if (value < 100) {
				if (mShineCountDigits[2]->getPane()->isVisible())
					mShineCountDigits[2]->getPane()->hide();

				if (value % 10 == 0)
					changeNum(mShineCountDigits[0], value / 10, 10);

				changeNum(mShineCountDigits[1], value - value / 10 * 10, 10);
			} else {
				if (value % 100 == 0) {
					changeNum(mShineCountDigits[0], value / 100, 10);
					if (!mShineCountDigits[0]->getPane()->isVisible())
						mShineCountDigits[0]->getPane()->show();
				}

				int remainder = value - (int)(value * 0.01f) * 100;
				if (remainder % 10 == 0)
					changeNum(mShineCountDigits[1], remainder / 10, 10);

				if (!mShineCountDigits[2]->getPane()->isVisible())
					mShineCountDigits[2]->getPane()->show();
				changeNum(mShineCountDigits[2], remainder % 10, 10);
			}
		} else if (unk8A == 0x106) {
			if (shines > unk64) {
				++unk64;
				unk8A = 0xFB;
			}
		} else if (!unk34 && !unk35) {
			mShineCountDigits[0]->update();
			mShineCountDigits[1]->update();
			mShineCountDigits[2]->update();
		}

		++unk8A;
	}
}

void TGCConsole2::countBlueCoin()
{
	if (unk168 != TFlagManager::getInstance()->getFlag(MSF_BLUE_COIN_COUNT)) {
		++unk168;

		int spentBlueCoins = 0;
		for (int flag = 0x46; flag < 0x56; ++flag)
			if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
			    != 0)
				++spentBlueCoins;
		for (int flag = 0x6C; flag < 0x74; ++flag)
			if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
			    != 0)
				++spentBlueCoins;

		int value = unk168 - spentBlueCoins * 10;
		if (value < 0)
			value = 0;

		if (value < 100) {
			setDigitPane(mBlueCoinCountDigits[0], unkE0, (int)(value * 0.1f));
			setDigitPane(mBlueCoinCountDigits[1], unkE0, value % 10);
			if (mBlueCoinCountDigits[2]->getPane()->isVisible())
				mBlueCoinCountDigits[2]->getPane()->hide();
		} else {
			setDigitPane(mBlueCoinCountDigits[0], unkE0, (int)(value * 0.01f));
			int remainder = value - (int)(value * 0.01f) * 100;
			setDigitPane(mBlueCoinCountDigits[1], unkE0,
			             (int)(remainder * 0.1f));
			setDigitPane(mBlueCoinCountDigits[2], unkE0, remainder % 10);
			if (!mBlueCoinCountDigits[2]->getPane()->isVisible())
				mBlueCoinCountDigits[2]->getPane()->show();
		}
		if (unk160->getPane()->isVisible()) {
			JUTRect bounds(mBlueCoinCountDigits[1]->getPane()->getGlbBounds());
			JGeometry::TVec3<f32> pos(bounds.x1 + bounds.getWidth() * 0.5f,
			                          bounds.y1 + bounds.getHeight() * 0.5f,
			                          0.0f);
			gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr, nullptr);
			if (value % 10 == 0) {
				bounds = mBlueCoinCountDigits[0]->getPane()->getGlbBounds();
				pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
				        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
				gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
				                                   nullptr);
			}
		} else {
			startAppearStar();
		}
		unk170 = value;
		unk16C = 1;
	}

	if (unk16C != 0) {
		++unk16C;
		if (unk16C > 400)
			unk16C = 0;
	}
}
#endif

void TGCConsole2::startMoveTimer(int param_1)
{
	unk4A  = 1;
	unk518 = param_1 * 100;
}

void TGCConsole2::stopMoveTimer()
{
	if (unk4A == 0)
		return;

	unk4A = 0;
}

int TGCConsole2::getFinishedTime() { return unk4FC; }

bool TGCConsole2::processAppearLife(int param_1)
{
	if (gpMarioOriginal->getHealth() == 0 || gpMarioOriginal->getAir() == 0)
		return true;

	bool isFinished = true;

	if (param_1 == 0) {
		mLifeMeterRoot->setPanePosition(40, JUTPoint(unk1C8, unk1CA),
		                                JUTPoint(-10, 10), JUTPoint(0, 0));
		mLifeMeterRoot->getPane()->show();
		mLifeMeterRoot->update();
		isFinished = false;
	} else if (param_1 < 100) {
		isFinished = false;
	} else if (!mLifeMeterRoot->update()) {
		isFinished = false;
	}

	return isFinished;
}

bool TGCConsole2::processInsertLife(int param_1)
{
	bool isFinished = true;

	isFinished &= mLifeMeterRoot->update();

	if (param_1 == 14) {
		mLifeMeterText->getPane()->show();
		mLifeMeterText->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                cDownMidPoint);
	}

	if (param_1 == 24) {
		mLifeMeterPetalsRoot->getPane()->show();
		mLifeMeterPetalsRoot->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                      cDownMidPoint);
	}

	updateDownPaneState(mLifeMeterBackground, isFinished);
	updateDownPaneState(mLifeMeterText, isFinished);
	updateDownPaneState(mLifeMeterPetalsRoot, isFinished);

	return isFinished;
}

bool TGCConsole2::processAppearStar(int param_1)
{
	bool isFinished = true;

	isFinished &= mShineCountRoot->update();

	isFinished &= unk160->update();

	if (param_1 == 14) {
		mShineCountIcon->getPane()->show();
		mShineCountIcon->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                 cDownMidPoint);
		mBlueCoinCountIcon->getPane()->show();
		mBlueCoinCountIcon->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                    cDownMidPoint);
	}

	if (param_1 == 24) {
		mShineCountX->getPane()->show();
		mShineCountX->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                              cDownMidPoint);
		mBlueCoinCountX->getPane()->show();
		mBlueCoinCountX->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                 cDownMidPoint);
	}

	int shines = TFlagManager::smInstance->getFlag(MSF_SHINE_COUNT);
	for (int i = 0; i < 3; ++i) {
		if (param_1 == i * 6 + 28) {
			if (i == 2) {
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSE01)
				if ((!unk50 && shines >= 100) || (shines > 100 && unk50))
					mShineCountDigits[i]->getPane()->show();
#else
				if (!unk50 && shines >= 100) {
					mShineCountDigits[i]->getPane()->show();
				} else if (unk50) {
					if (gpMarDirector->checkDemoFlag(
					        TMarDirector::DEMO_FLAG_SHINE_ALREADY_COLLECTED)) {
						if (shines > 99)
							mShineCountDigits[i]->getPane()->show();
					} else if (shines > 100) {
						mShineCountDigits[i]->getPane()->show();
					}
				}
#endif
			} else {
				mShineCountDigits[i]->getPane()->show();
			}
			mShineCountDigits[i]->setPanePosition(50, cDownTopPoint,
			                                      cDownMidPoint, cDownMidPoint);
		}
	}

	TFlagManager::smInstance->getFlag(MSF_BLUE_COIN_COUNT);

	int blueCoins = 0;
	for (int flag = 0x46; flag < 0x56; ++flag)
		if (TFlagManager::smInstance->getFlag(MSF_SHINE_BASE + flag) != 0)
			++blueCoins;

	for (int flag = 0x6C; flag <= 0x73; ++flag)
		if (TFlagManager::smInstance->getFlag(MSF_SHINE_BASE + flag) != 0)
			++blueCoins;

	blueCoins = unk168 - blueCoins * 10;
	if (blueCoins < 0)
		blueCoins = 0;

	for (int i = 0; i < 3; ++i) {
		if (param_1 == i * 6 + 28) {
			if (i == 2) {
				if (blueCoins >= 100)
					mBlueCoinCountDigits[i]->getPane()->show();
			} else {
				mBlueCoinCountDigits[i]->getPane()->show();
			}
			mBlueCoinCountDigits[i]->setPanePosition(
			    50, cDownTopPoint, cDownMidPoint, cDownMidPoint);
		}
	}

	updateDownPaneState(mShineCountBackground, isFinished);
	updateDownPaneState(mBlueCoinCountBackground, isFinished);
	updateDownPaneState(mShineCountIcon, isFinished);
	updateDownPaneState(mBlueCoinCountIcon, isFinished);
	updateDownPaneState(mShineCountX, isFinished);
	updateDownPaneState(mBlueCoinCountX, isFinished);

	for (int i = 0; i < 3; ++i)
		updateDownBlendPaneState(mShineCountDigits[i], isFinished);
	for (int i = 0; i < 3; ++i) {
		updateDownPaneState(mBlueCoinCountDigits[i], isFinished);
	}

	JUTRect bounds = mShineCountIcon->getPane()->getGlbBounds();
	unk144->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
	                             bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);

	bounds = mBlueCoinCountIcon->getPane()->getGlbBounds();
	unk164->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
	                             bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);

	return isFinished;
}

bool TGCConsole2::processDownCoin(int param_1)
{
	bool isFinished = true;

	if (param_1 == 14)
		mCoinCountIcon->setPanePosition(50, JUTPoint(0, 0), cCoinMidPoint,
		                                cCoinMidPoint);

	if (param_1 == 24)
		mCoinCountX->setPanePosition(50, JUTPoint(0, 0), cCoinMidPoint,
		                             cCoinMidPoint);

	for (int i = 0; i < 3; ++i) {
		if (param_1 == i * 6 + 28)
			mCoinCountDigits[i]->setPanePosition(50, JUTPoint(0, i * 6),
			                                     cCoinMidPoint, cCoinMidPoint);
	}

	updateCoinPaneState(mCoinCountBackground, isFinished);

	if (param_1 < 14)
		isFinished = false;
	else
		updateCoinPaneState(mCoinCountIcon, isFinished);

	if (param_1 < 24)
		isFinished = false;
	else
		updateCoinPaneState(mCoinCountX, isFinished);

	for (int i = 0; i < 3; ++i) {
		if (param_1 < i * 6 + 28)
			isFinished = false;
		else
			updateCoinBlendPaneState(mCoinCountDigits[i], isFinished);
	}

	isFinished &= mCoinCountRoot->update();

	JUTRect bounds(mCoinCountIcon->getPane()->getGlbBounds());
	unk124->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
	                             bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);

	return isFinished;
}

bool TGCConsole2::processAppearTank(int param_1)
{
	bool isFinished = true;
	isFinished &= unk2F8->update();

	if (param_1 == 6) {
		unk270->getPane()->show();
		unk270->setPanePosition(40, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	if (param_1 == 14) {
		unk29C->getPane()->show();
		unk29C->setPanePosition(40, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	if (param_1 == 26) {
		unk274->getPane()->show();
		unk274->setPanePosition(40, JUTPoint(0, 100), cUpMidPoint, cUpMidPoint);
	}

	updateUpPaneState(unk26C, isFinished);

	updateUpPaneStateAfter(unk270, isFinished, param_1, 6);
	updateUpPaneStateAfter(unk29C, isFinished, param_1, 14);

	if (param_1 >= 26 && unk274->update()) {
		bool paneFinished = false;
		if (unk274->unk14.x == 0 && unk274->unk14.y == 0)
			paneFinished = true;
		if (!paneFinished) {
			unk274->setPanePosition(30, cUpMidPoint, cUpMidPoint, cUpBotPoint);
			isFinished = false;
		} else {
			unk288->show();
		}
	} else {
		isFinished = false;
	}

	return isFinished;
}

bool TGCConsole2::processAppearCoin(int param_1)
{
	bool isFinished = true;
	isFinished &= mCoinCountRoot->update();

	if (param_1 == 14) {
		mCoinCountIcon->getPane()->show();
		mCoinCountIcon->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                cDownMidPoint);
	}

	if (param_1 == 24) {
		mCoinCountX->getPane()->show();
		mCoinCountX->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                             cDownMidPoint);
	}

	for (int i = 0; i < 3; ++i) {
		if ((unk6C >= 100 || i != 2) && param_1 == i * 6 + 28) {
			mCoinCountDigits[i]->getPane()->show();
			mCoinCountDigits[i]->setPanePosition(50, cDownTopPoint,
			                                     cDownMidPoint, cDownMidPoint);
		}
	}

	updateDownPaneState(mCoinCountBackground, isFinished);
	updateDownPaneState(mCoinCountIcon, isFinished);
	updateDownPaneState(mCoinCountX, isFinished);

	for (int i = 0; i < 3; ++i) {
		if (unk6C >= 100 || i != 2)
			updateDownBlendPaneState(mCoinCountDigits[i], isFinished);
	}

	JUTRect bounds(mCoinCountIcon->getPane()->getGlbBounds());
	unk124->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
	                             bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);

	return isFinished;
}

bool TGCConsole2::processAppearMario(int param_1)
{
	bool isFinished = mLifeCountRoot->update();

	if (param_1 == 14) {
		mLifeCountIcon->getPane()->show();
		mLifeCountIcon->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                cDownMidPoint);
	}

	if (param_1 == 24) {
		mLifeCountText->getPane()->show();
		mLifeCountText->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                                cDownMidPoint);
	}

	if (param_1 == 24) {
		mLifeCountX->getPane()->show();
		mLifeCountX->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                             cDownMidPoint);
	}

	for (int i = 0; i < 3; ++i) {
		if (i == 2)
			break;
		if (param_1 == i * 6 + 24) {
			mLifeCountDigits[i]->getPane()->show();
			mLifeCountDigits[i]->setPanePosition(50, cDownTopPoint,
			                                     cDownMidPoint, cDownMidPoint);
		}
	}

	updateDownPaneState(mLifeCountBackground, isFinished);
	updateDownPaneState(mLifeCountIcon, isFinished);
	updateDownPaneState(mLifeCountText, isFinished);
	updateDownPaneState(mLifeCountX, isFinished);

	for (int i = 0; i < 3; ++i) {
		if (i == 2)
			break;
		updateDownPaneState(mLifeCountDigits[i], isFinished);
	}

	return isFinished;
}

bool TGCConsole2::processDrawTelop(u32)
{
	bool isFinished = false;

	JUTRect paneBounds(mTelop->getPane()->getGlbBounds());
	JUTRect textBounds(unk528->getBounds());

	if (unk534.x1 < paneBounds.x1 - mTelopTextWidth) {
		isFinished = true;
	} else {
		unk568 -= unk564;
		unk534.move((int)unk568, unk534.y1);
	}

	return isFinished;
}

void TGCConsole2::checkChangeTelopArray()
{
	u32* oldArray = unk570;

	if (SMSGetMarDirector()->getCurrentStage() == 1) {
		switch (SMSGetMarDirector()->getCurrentScenario()) {
		case 0:
			unk570 = scDolpicNewsDolpic0;
			break;
		case 1:
			if (static_cast<TTakeActor*>(static_cast<JDrama::TNameRef*>(unkC4))
			        ->getHolder()
			    != nullptr)
				unk570 = scDolpicNewsDolpic1;
			else
				unk570 = nullptr;
			break;
		case 6:
			unk570 = scDolpicNewsDolpic6;
			break;
		case 7:
			unk570 = scDolpicNewsDolpic7;
			break;
		case 9:
			unk570 = scDolpicNewsDolpic9;
			break;
		case 2:
			unk570 = scDolpicNewsDolpic10;
			break;
		case 5:
			if (TFlagManager::getInstance()->getBool(MSF_RICCO_UNLOCKABLE)) {
				if (TFlagManager::getInstance()->getBool(MSF_GELATO_UNLOCKABLE))
					unk570 = scDolpicNewsDolpic5_4;
				else
					unk570 = scDolpicNewsDolpic5_2;
			} else {
				if (TFlagManager::getInstance()->getBool(MSF_GELATO_UNLOCKABLE))
					unk570 = scDolpicNewsDolpic5_3;
				else
					unk570 = scDolpicNewsDolpic5_1;
			}
			break;
#if defined(VERSION_GMSJ01)
		case 8: {
			int eventState
			    = TFlagManager::getInstance()->getFlag(MSF_SHADOW_MARIO_EVENT);
			switch (eventState) {
			case 0:
				if (TFlagManager::getInstance()->getBool(MSF_YOSHI_UNLOCKED)) {
					if (TFlagManager::getInstance()->getNozzleRight(1, 0)
					    || TFlagManager::getInstance()->getNozzleRight(1, 1))
						unk570 = scDolpicNewsDolpic8_6;
					else
						unk570 = scDolpicNewsDolpic8_4;
				} else {
					if (TFlagManager::getInstance()->getNozzleRight(1, 0)
					    || TFlagManager::getInstance()->getNozzleRight(1, 1))
						unk570 = scDolpicNewsDolpic8_5;
					else if (TFlagManager::getInstance()->getFlag(
					             MSF_SHINE_COUNT)
					         >= 20)
						unk570 = scDolpicNewsDolpic8_7;
					else
						unk570 = scDolpicNewsDolpic8_1;
				}
				break;
			case 1:
				if (TFlagManager::getInstance()->getBool(MSF_YOSHI_UNLOCKED))
					unk570 = scDolpicNewsDolpic8_1;
				else
					unk570 = scDolpicNewsDolpic8_2;
				break;
			default:
				if (TFlagManager::getInstance()->getNozzleRight(1, 0)
				    || TFlagManager::getInstance()->getNozzleRight(1, 1))
					unk570 = scDolpicNewsDolpic8_1;
				else
					unk570 = scDolpicNewsDolpic8_3;
				break;
			}
			break;
		}
#else
		case 8:
			unk570 = checkDolpic8();
			break;
#endif
		default:
			unk570 = nullptr;
			break;
		}
	} else {
		unk570 = nullptr;
	}

	if (oldArray != unk570)
		unk558 = 0;
}

bool TGCConsole2::processAppearJet(int param_1)
{
	bool isFinished = true;
	int startDigit  = 0;
	int endDigit    = 4;

	if (unk2C < 10)
		startDigit = 1;

	if ((u8)unk424[0] < 10) {
		startDigit = 1;
		endDigit   = 3;
	}

	isFinished &= unk3FC->update();

	if (param_1 == 7) {
		unk404->getPane()->show();
		unk404->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	for (int i = startDigit; i < endDigit; ++i) {
		if (param_1 == i * 3 + 14) {
			unk414[i]->getPane()->show();
			unk414[i]->setPanePosition(50, cUpTopPoint, cUpMidPoint,
			                           cUpMidPoint);
		}
	}

	if (param_1 == 16) {
		unk410->getPane()->show();
		unk410->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	updateUpPaneState(unk400, isFinished);
	updateUpPaneState(unk404, isFinished);

	for (int i = 0; i < 4; ++i) {
		updateUpBlendPaneState(unk414[i], isFinished);
	}

	updateUpPaneState(unk410, isFinished);

	return isFinished;
}

bool TGCConsole2::processAppearRed(int param_1)
{
	bool isFinished = true;
	isFinished &= unk428->update();

	if (param_1 == 14) {
		unk430->getPane()->show();
		unk430->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	if (param_1 == 24) {
		unk434->getPane()->show();
		unk434->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	for (int i = 0; i < 2; ++i) {
		if (param_1 == i * 6 + 28) {
			unk43C[i]->getPane()->show();
			unk43C[i]->setPanePosition(50, cUpTopPoint, cUpMidPoint,
			                           cUpMidPoint);
		}
	}

	if (param_1 == 32) {
		unk438->getPane()->show();
		unk438->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	updateUpPaneState(unk42C, isFinished);
	updateUpPaneState(unk430, isFinished);
	updateUpPaneState(unk434, isFinished);

	for (int i = 0; i < 2; ++i) {
		updateUpPaneState(unk43C[i], isFinished);
	}

	updateUpPaneState(unk438, isFinished);

	return isFinished;
}

bool TGCConsole2::processAppearTimer(int param_1)
{
	bool isFinished = true;
	isFinished &= unk44C->update();

	if (param_1 == 24) {
		unk454->getPane()->show();
		unk454->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	if (unk500[0]->isVisible()) {
		for (int i = 0; i < 6; ++i) {
			if (param_1 == i * 6 + 24) {
				unk458[i]->getPane()->show();
				unk458[i]->setPanePosition(50, cUpTopPoint, cUpMidPoint,
				                           cUpMidPoint);
			}
		}
	} else {
		for (int i = 6; i < 10; ++i) {
			if (param_1 == (i - 6) * 6 + 24) {
				unk458[i]->getPane()->show();
				unk458[i]->setPanePosition(50, cUpTopPoint, cUpMidPoint,
				                           cUpMidPoint);
			}
		}
	}

	if (unk500[0]->isVisible()) {
		for (int i = 0; i < 2; ++i) {
			if (param_1 == i * 12 + 32) {
				unk480[i]->getPane()->show();
				unk480[i]->setPanePosition(50, cUpTopPoint, cUpMidPoint,
				                           cUpMidPoint);
			}
		}
	} else if (param_1 == 32) {
		unk480[2]->getPane()->show();
		unk480[2]->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);
	}

	updateUpPaneState(unk450, isFinished);
	updateUpPaneState(unk454, isFinished);

	if (unk500[0]->isVisible()) {
		for (int i = 0; i < 6; ++i) {
			updateUpPaneState(unk458[i], isFinished);
		}
	} else {
		for (int i = 6; i < 10; ++i) {
			updateUpPaneState(unk458[i], isFinished);
		}
	}

	if (unk500[0]->isVisible()) {
		for (int i = 0; i < 2; ++i) {
			updateUpPaneState(unk480[i], isFinished);
		}
	} else
		updateUpPaneState(unk480[2], isFinished);

	return isFinished;
}

bool TGCConsole2::processAppearBalloon()
{
	bool isFinished = false;

	u16 alpha = unk3B0->getAlpha();
	if (alpha < 0xFF) {
		alpha += 0x30;
		if (alpha > 0xFF)
			alpha = 0xFF;
		unk3B0->setAlpha(alpha);
	} else {
		JUTRect bounds = unk3B0->getBounds();
		int nextHeight = unk3CC + bounds.getHeight();
		int maxHeight  = unk3BC.getHeight();

		if (nextHeight > maxHeight) {
			nextHeight = maxHeight;
			isFinished = true;
		}

		unk3B0->resize(unk3BC.getWidth(), nextHeight);

		unk3B0->place(
		    JUTRect(unk3BC.x1, unk3BC.y2 - nextHeight, unk3BC.x2, unk3BC.y2));
	}

	return isFinished;
}

bool TGCConsole2::processDisappearBalloon()
{
	bool isFinished = false;

	JUTRect bounds = unk3B0->getBounds();
	int height     = bounds.getHeight();

	int contentHeight = unk3B0->getContentsBounds().getHeight();

	if (contentHeight > 0) {
		int nextHeight = height - unk3CC;
		if (contentHeight - unk3CC < 0)
			nextHeight += unk3CC - contentHeight;

		unk3B0->resize(unk3BC.getWidth(), nextHeight);

		unk3B0->place(
		    JUTRect(unk3BC.x1, unk3BC.y2 - nextHeight, unk3BC.x2, unk3BC.y2));
	} else {
		s16 alpha = unk3B0->getAlpha();
		alpha -= 0x30;
		if (alpha < 0) {
			alpha      = 0;
			isFinished = true;
		}
		unk3B0->setAlpha(alpha);
	}

	return isFinished;
}

void TGCConsole2::drawJuice(J2DOrthoGraph& graph, u32 color)
{
	if (unk50)
		return;

	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, 0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetNumTexGens(2);
	GXSetNumTevStages(2);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });

	GXSetTevColor(GX_TEVREG0, JUtility::TColor(color & 0xFFFFFF00));
	GXSetTevColor(GX_TEVREG1, JUtility::TColor(color));
	GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);

	unk32C->load(GX_TEXMAP0, 0);
	unk328->load(GX_TEXMAP1, 0);

	u8 selected = 0x17;
	for (u8 i = 0x16; (u8)i != 0; --i) {
		int threshold = (int)(64.0f * (1.0f - unk2B8));
		if (unk334[i]->getBounds().y1 >= threshold) {
			selected = i + 1;
			break;
		}
	}

	f32 translateY;
	if (selected < 0x17)
		translateY = 1.0f - (f32)unk334[selected]->getBounds().y1 * 0.015625f;
	else
		translateY = 1.0f;

	if (unk334[1]->getBounds().y1 < (int)(64.0f * (1.0f - unk2B8)))
		translateY = 0.0f;

	MTXTrans(mtx, 0.0f, translateY, 0.0f);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

	GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
	                GX_CC_ZERO);
	GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA,
	                GX_CA_ZERO);
	GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
	                GX_TRUE, GX_TEVPREV);
	MTXIdentity(mtx);
	GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);

	JUTRect bounds = unk328->getBounds();
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2f32((f32)bounds.x1, (f32)bounds.y1);
	GXTexCoord2s8(0, 0);
	GXPosition2f32((f32)bounds.x2, (f32)bounds.y1);
	GXTexCoord2s8(1, 0);
	GXPosition2f32((f32)bounds.x2, (f32)bounds.y2);
	GXTexCoord2s8(1, 1);
	GXPosition2f32((f32)bounds.x1, (f32)bounds.y2);
	GXTexCoord2s8(0, 1);
	GXEnd();

	graph.setup2D();

	if (unk330 != selected - 1)
		unk334[unk330]->hide();

	if (selected < 0x17) {
		((J2DPicture*)unk334[selected - 1])->setWhite(color);
		unk334[selected - 1]->show();
		unk330 = selected - 1;
	}
}

void TGCConsole2::drawWater(J2DOrthoGraph& graph)
{
	static const f32 height[2]  = { 0.16099999845f, 0.12999999523f };
	static const s16 topDiff[2] = { 7, 10 };

	Mtx mtx;
	MTXIdentity(mtx);
	GXLoadPosMtxImm(mtx, 0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetNumTexGens(2);
	GXSetNumTevStages(2);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S8, 0);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetNumChans(1);
	GXSetChanCtrl(GX_COLOR0A0, GX_TRUE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL,
	              GX_DF_NONE, GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, (GXColor) { 0xff, 0xff, 0xff, 0xff });

	unk2EC[1] = JUtility::TColor(((u32)unk9A.r << 24) + ((u32)unk9A.g << 16)
	                             + ((u32)unk9A.b << 8));
	unk2EC[2] = JUtility::TColor(((u32)unk9E.r << 24) + ((u32)unk9E.g << 16)
	                             + ((u32)unk9E.b << 8));

	u8 alpha[3] = { 0 };
	alpha[1]    = unk9A.a;
	alpha[2]    = unk9E.a;

	for (int layer = 2; layer > 0; --layer) {
		GXSetTevColor(GX_TEVREG0, unk2EC[layer]);
		GXSetTevColor(GX_TEVREG1,
		              JUtility::TColor((u32)unk2EC[layer] + alpha[layer]));

		GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		unk2AC[layer]->load(GX_TEXMAP0, 0);
		unk2A0[layer]->load(GX_TEXMAP1, 0);

		MTXTrans(mtx, 0.0f, unk2B8 * (1.0f - height[layer - 1]), 0.0f);
		GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
		GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0,
		                  GX_FALSE, GX_PTIDENTITY);
		GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

		GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA,
		                GX_CA_ZERO);
		GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		MTXIdentity(mtx);
		GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
		GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1,
		                  GX_FALSE, GX_PTIDENTITY);
		GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);

		f32 top    = unk29C->getPane()->getGlbBounds().y1 + topDiff[layer - 1];
		f32 bottom = top + unk2A0[layer]->getHeight();
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2f32(unk2BC[layer].x1, top);
		GXTexCoord2s8(0, 0);
		GXPosition2f32(unk2BC[layer].x2, top);
		GXTexCoord2s8(1, 0);
		GXPosition2f32(unk2BC[layer].x2, bottom);
		GXTexCoord2s8(1, 1);
		GXPosition2f32(unk2BC[layer].x1, bottom);
		GXTexCoord2s8(0, 1);
		GXEnd();
	}

	graph.setup2D();

	JUTRect bounds = unk2A0[0]->getBounds();
	f32 hidden     = 47.0f * (1.0f - unk2B8) - 0.5f;
	int top        = unk29C->getPane()->getGlbBounds().y1 + (int)hidden;
	int y          = ++top;
	if (top < 0)
		y = 0;

	unk2A0[0]->draw(bounds.x1, y, false, false, false);

	bounds = ((J2DPicture*)unk270->getPane())->getBounds();
	((J2DPicture*)unk270->getPane())
	    ->draw(bounds.x1, bounds.y1, false, false, false);

	bounds = ((J2DPicture*)unk274->getPane())->getBounds();
	((J2DPicture*)unk274->getPane())
	    ->draw(bounds.x1, bounds.y1, false, false, false);

	JUTRect maskOffset = unk288->getBounds();
	bounds.add(maskOffset.x1, maskOffset.y1);
	((J2DPicture*)unk288)->draw(bounds.x1, bounds.y1, false, false, false);
}

// fabricated
// TODO: the ROM shows that all of the movement cue work is one inline body
// below perform. resetLife, startAppearTank, processAppearStar and
// processDownCoin stay out of line, and the life meter local "amount" stays in
// one register from the life meter code down to the blink code.
void TGCConsole2::performMovement()
{
	if (!unk50) {
		if (gpCamera->isDemoCamera() || SMS_CheckMarioFlag(0x400)
		    || (!unk3AD
		        && TFlagManager::getInstance()->getBool(
		            MSF_LOST_LIFE_IN_PREV_STAGE)))
			startCameraDemo();
	} else if (!gpCamera->isDemoCamera() && !SMS_CheckMarioFlag(0x400)) {
		endCameraDemo();
	}

	if (unk40) {
		bool done = true;
		if (!unk3F && unk44C->getPane()->isVisible())
			done &= unk44C->update();
		if (unk428->getPane()->isVisible())
			done &= unk428->update();
		if (unk3FC->getPane()->isVisible())
			done &= unk3FC->update();
		if (done)
			unk40 = 0;
	}

	if (unk41) {
		bool done = true;
		if (unk44C->getPane()->isVisible())
			done &= unk44C->update();
		if (unk428->getPane()->isVisible())
			done &= unk428->update();
		if (unk3FC->getPane()->isVisible())
			done &= unk3FC->update();
		if (done)
			unk41 = 0;
	}

	if (unk39) {
		if (unkB4 == 1 && unk6C < 100)
			mCoinCountDigits[2]->getPane()->hide();

		if (unkB4 == 0) {
			startAppearCoin();
			startAppearMario(false);
#if !defined(VERSION_GMSJ01) && !defined(VERSION_GMSE01)
			int nozzle = gpMarioOriginal->mWaterGun->mCurrentNozzle;
			if (nozzle == TWaterGun::Spray) {
				unk274 = unk278[0];
				unk288 = unk28C[0];
			} else if (nozzle == TWaterGun::Rocket) {
				unk274 = unk278[2];
				unk288 = unk28C[2];
			} else if (nozzle == TWaterGun::Hover) {
				unk274 = unk278[1];
				unk288 = unk28C[1];
			} else if (nozzle == TWaterGun::Turbo) {
				unk274 = unk278[3];
				unk288 = unk28C[3];
			} else if (nozzle == TWaterGun::Underwater) {
				unk274 = unk278[1];
				unk288 = unk28C[1];
			}
			unk310 = nozzle;
#endif
			mHudScreen->search('ROOT')->setAlpha(0);
		} else {
			mHudScreen->search('ROOT')->setAlpha(0xff);
		}

		++unkB4;
		if (unkB4 > 0xa0)
			unk39 = 0;
	}

	u8 amount;
	bool airMode = true;

	switch (unk18) {
	case LIFE_HIDDEN:
	case LIFE_HEALTH_APPEARING:
	case LIFE_HEALTH_VISIBLE:
	case LIFE_HEALTH_DISAPPEARING:
	case LIFE_HEALTH_INSERTING:
#if !defined(VERSION_GMSJ01)
	case LIFE_CAMERA_DEMO:
	default:
#endif
		airMode = false;
		amount  = gpMarioOriginal->getHealth();
		break;
	case LIFE_AIR_APPEARING:
	case LIFE_AIR_VISIBLE:
	case LIFE_AIR_DISAPPEARING:
	case LIFE_AIR_INSERTING:
	case LIFE_AIR_DIVING:
		amount = gpMarioOriginal->getAir();
		break;
	}

	if (amount > 8)
		amount = 8;

	if (airMode
	    && SMSGetMarDirector()->getState() == TMarDirector::STATE_PAUSE_MENU) {
		s16 alpha = mLifeMeterRoot->getPane()->getAlpha();
		alpha -= 0x10;
		if (alpha < 0)
			alpha = 0;
		mLifeMeterRoot->getPane()->setAlpha(alpha);
	}

	if (mLifeMeterRoot->getPane()->getAlpha() != 0xff
	    && SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU) {
		u16 alpha = mLifeMeterRoot->getPane()->getAlpha() + 0x10;
		if (alpha > 0xff)
			alpha = 0xff;
		mLifeMeterRoot->getPane()->setAlpha(alpha);
	}

	bool airTimeout = false;
	if (gpMarioOriginal->isUnderWater()) {
		int airTimer = unk268;
		if (airTimer < 0xf0)
			unk268 = airTimer + 1;
		else
			airTimeout = true;
	} else {
		unk268 = 0;
	}

	switch (unk18) {
	case LIFE_HIDDEN:
		if (mLifeMeterRoot->getPane()->isVisible())
			mLifeMeterRoot->getPane()->hide();

		if (SMS_isDivingMap()) {
			startInsertLife(1);
			unk18 = LIFE_AIR_INSERTING;
		}

		if (airTimeout) {
			resetLife(amount);
			startAppearLife(1);
			amount = gpMarioOriginal->getAir();
			unk1CC = amount;
			if (SMSGetMarDirector()->getState()
			    == TMarDirector::STATE_PAUSE_MENU)
				mLifeMeterRoot->getPane()->setAlpha(0);
#if defined(VERSION_GMSJ01)
			unk18 = LIFE_AIR_APPEARING;
		}

		if (unk1CC < 8 && !mLifeMeterRoot->getPane()->isVisible()
		    && amount != 0) {
#else
			unk84 = 0;
			unk18 = LIFE_AIR_APPEARING;
		} else if (!airMode && unk1CC < 8
		           && !mLifeMeterRoot->getPane()->isVisible() && amount != 0) {
#endif
			startAppearLife(0);
			unk18 = LIFE_HEALTH_APPEARING;
		}
		break;
	case LIFE_HEALTH_APPEARING:
		if (processAppearLife(unk84++)) {
			unk18 = LIFE_HEALTH_VISIBLE;
			unk38 = 0;
		}
		if (airTimeout && startDisappearLife(1))
			unk18 = LIFE_HEALTH_DISAPPEARING;
		break;
	case LIFE_HEALTH_VISIBLE:
		if (unk1CC == 8) {
			startDisappearLife(0);
			unk18 = LIFE_HEALTH_DISAPPEARING;
		} else if (airTimeout) {
			if (startDisappearLife(1))
				unk18 = LIFE_HEALTH_DISAPPEARING;
		}
		break;
	case LIFE_HEALTH_DISAPPEARING:
		if (unk84 > 0x78) {
			if (mLifeMeterRoot->update()) {
				unk4C = 0;
				unk18 = LIFE_HIDDEN;
			}
		} else {
			++unk84;
		}
		break;
	case LIFE_HEALTH_INSERTING:
		if (processInsertLife(unk84++))
			unk18 = LIFE_HEALTH_VISIBLE;
		break;
	case LIFE_AIR_INSERTING:
		if (processInsertLife(unk84++)) {
			if (SMS_isDivingMap())
				unk18 = LIFE_AIR_DIVING;
			else
				unk18 = LIFE_AIR_VISIBLE;
		}
		break;
	case LIFE_AIR_APPEARING:
		if (processAppearLife(unk84)) {
			unk18 = LIFE_AIR_VISIBLE;
			unk38 = 0;
		}
		if (!gpMarioOriginal->isUnderWater() && unk84 == 0)
			unk84 = 1;
		if (!(amount >= 8 && unk84 == 0) && unk84 < 1000)
			++unk84;
		break;
	case LIFE_AIR_VISIBLE:
		if (!gpMarioOriginal->isUnderWater()) {
			if (unk1CC == 8) {
				startDisappearLife(0);
				unk18 = LIFE_AIR_DISAPPEARING;
			} else if (unk1CC != unk1C) {
				startAppearLife(1);
				amount = gpMarioOriginal->getHealth();
				unk1CC = amount;
				resetLife(amount);
				unk18 = LIFE_HEALTH_APPEARING;
			}
		}
		break;
	case LIFE_AIR_DISAPPEARING:
		if (unk84 > 0x78) {
			if (mLifeMeterRoot->update()) {
				unk4C = 0;
				if (gpMarioOriginal->getHealth() == 8) {
					unk18 = LIFE_HIDDEN;
				} else {
					amount = gpMarioOriginal->getHealth();
					unk1CC = amount;
					startInsertLife(0);
					resetLife(amount);
					unk18 = LIFE_HEALTH_INSERTING;
				}
			}
		} else {
			++unk84;
		}
		break;
	case LIFE_CAMERA_DEMO:
		if (unk50) {
			if (mLifeMeterRoot->update())
				unk4C = 0;
		} else if (gpMarioOriginal->getHealth() != 8) {
			startInsertLife(0);
			unk18 = LIFE_HEALTH_INSERTING;
		} else if (gpMarioOriginal->isUnderWater() || SMS_isDivingMap()) {
			startInsertLife(1);
			unk18 = LIFE_AIR_INSERTING;
		} else {
			amount = 8;
			unk1CC = amount;
			unk18  = LIFE_HIDDEN;
		}
		break;
	}

	if (unk18 == LIFE_CAMERA_DEMO && amount == 0) {
		mLifeMeterRoot->getPane()->hide();
		unk274->getPane()->hide();
		unk270->getPane()->hide();
		unk26C->getPane()->hide();
		mLifeCountRoot->getPane()->hide();
	}

	if (amount != unk1CC && unk18 != LIFE_CAMERA_DEMO) {
		if (unk1CC < amount) {
			++unk1CC;
			mLifeMeterPetals[unk1CC * 2]->show();
			playLifeChangeSound(0x4801);
		} else if (unk1CC != amount) {
#if defined(VERSION_GMSJ01)
			playLifeChangeSound(0x4823);
#else
			if (SMSGetMarDirector()->getState() == TMarDirector::STATE_UNK4
			    && SMSGetMarDirector()->isGameplayModeNow()) {
				if (airMode)
					SMSGetMSound()->startSoundSystemSE(0x480C, 0, nullptr, 0);
				else
					SMSGetMSound()->startSoundSystemSE(0x4823, 0, nullptr, 0);
			}
#endif
			mLifeMeterPetals[unk1CC * 2]->hide();
			mLifeMeterPetals[unk1CC * 2]->place(
			    JUTRect(mLifeMeterPetalsOriginalBounds[unk1CC].x1,
			            mLifeMeterPetalsOriginalBounds[unk1CC].y1,
			            mLifeMeterPetalsOriginalBounds[unk1CC].x2,
			            mLifeMeterPetalsOriginalBounds[unk1CC].y2));
			mLifeMeterPetals[unk1CC * 2 + 1]->place(
			    JUTRect(mLifeMeterPetalsOriginalBounds[unk1CC].x1,
			            mLifeMeterPetalsOriginalBounds[unk1CC].y1,
			            mLifeMeterPetalsOriginalBounds[unk1CC].x2,
			            mLifeMeterPetalsOriginalBounds[unk1CC].y2));
			--unk1CC;
			if (unk1CC == 0) {
				mLifeMeterPetals[0]->hide();
				mLifeMeterPetals[0]->place(
				    JUTRect(mLifeMeterPetalsOriginalBounds[0].x1,
				            mLifeMeterPetalsOriginalBounds[0].y1,
				            mLifeMeterPetalsOriginalBounds[0].x2,
				            mLifeMeterPetalsOriginalBounds[0].y2));
				mLifeMeterPetals[1]->place(
				    JUTRect(mLifeMeterPetalsOriginalBounds[0].x1,
				            mLifeMeterPetalsOriginalBounds[0].y1,
				            mLifeMeterPetalsOriginalBounds[0].x2,
				            mLifeMeterPetalsOriginalBounds[0].y2));
			}
		}
		unk1C = unk1CC;
		if (unk1CC >= 4)
			setLifeMeterWhiteHigh(this, airMode);
		else
			setLifeMeterWhiteLow(this, airMode);
	}

	int coins = TFlagManager::getInstance()->getFlag(MSF_GOLD_COIN_COUNT);
	if (coins > 999)
		coins = 999;
	else if (coins < 0)
		coins = 0;

	if (coins != unk20) {
		if (unk68 == 0)
			unk68 = 1;
		unk20 = coins;
	}

	if (gpMarioOriginal->mStatus == 0xC400201
	    && SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU
	    && !unk50) {
		if (!mShineCountRoot->isInterpolatorAtZero()) {
			++unk30;
			if (unk30 > 200) {
				startAppearStar();
				startAppearMario(false);
				unk70 = 0xffff;
				unk59 = 0;
				unk30 = 0;
			}
		}
	} else {
		unk30 = 0;
		if (!unk34 && mShineCountRoot->isInterpolatorAtZero() && !unk60
		    && SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU
		    && SMSGetMarDirector()->getState() != TMarDirector::STATE_CARD_SAVE
		    && !unk50 && unk16C == 0 && unk8A == 0
		    && !SMSGetMarDirector()->isTalkMainModeNow()) {
			startDisappearStar();
			startDisappearMario();
			unk5A = 0;
		}
	}

	if (unk35) {
		bool done = true;
		done &= mCoinCountRoot->update();
		done &= mShineCountRoot->update();
		done &= unk160->update();

		JUTRect bounds(mCoinCountIcon->getPane()->getGlbBounds());
		unk124->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
		                             bounds.y1 + bounds.getHeight() * 0.5f,
		                             0.0f);
		bounds = mBlueCoinCountIcon->getPane()->getGlbBounds();
		unk164->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
		                             bounds.y1 + bounds.getHeight() * 0.5f,
		                             0.0f);
		bounds = mShineCountIcon->getPane()->getGlbBounds();
		unk144->setGlobalTranslation(bounds.x1 + bounds.getWidth() * 0.5f,
		                             bounds.y1 + bounds.getHeight() * 0.5f,
		                             0.0f);

		if (done) {
			mShineCountRoot->getPane()->hide();
			unk160->getPane()->hide();
			unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
			unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
			unk35 = 0;
		}
	}

#if defined(VERSION_GMSJ01)
	int blueCoins = TFlagManager::getInstance()->getFlag(MSF_BLUE_COIN_COUNT);
	if (unk168 != blueCoins) {
		++unk168;

		int spentBlueCoins = 0;
		for (int flag = 0x46; flag < 0x56; ++flag)
			if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
			    != 0)
				++spentBlueCoins;
		for (int flag = 0x6C; flag < 0x74; ++flag)
			if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
			    != 0)
				++spentBlueCoins;

		int value = unk168 - spentBlueCoins * 10;
		if (value < 0)
			value = 0;

		if (value < 100) {
			setDigitPane(mBlueCoinCountDigits[0], unkE0, (int)(value * 0.1f));
			setDigitPane(mBlueCoinCountDigits[1], unkE0, value % 10);
			if (mBlueCoinCountDigits[2]->getPane()->isVisible())
				mBlueCoinCountDigits[2]->getPane()->hide();
		} else {
			setDigitPane(mBlueCoinCountDigits[0], unkE0, (int)(value * 0.01f));
			int remainder = value - (int)(value * 0.01f) * 100;
			setDigitPane(mBlueCoinCountDigits[1], unkE0,
			             (int)(remainder * 0.1f));
			setDigitPane(mBlueCoinCountDigits[2], unkE0, remainder % 10);
			if (!mBlueCoinCountDigits[2]->getPane()->isVisible())
				mBlueCoinCountDigits[2]->getPane()->show();
		}
		if (unk160->getPane()->isVisible()) {
			JUTRect bounds(mBlueCoinCountDigits[1]->getPane()->getGlbBounds());
			JGeometry::TVec3<f32> pos(bounds.x1 + bounds.getWidth() * 0.5f,
			                          bounds.y1 + bounds.getHeight() * 0.5f,
			                          0.0f);
			gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr, nullptr);
			if (value % 10 == 0) {
				bounds = mBlueCoinCountDigits[0]->getPane()->getGlbBounds();
				pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
				        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
				gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
				                                   nullptr);
			}
		} else {
			startAppearStar();
		}
		unk170 = value;
		unk16C = 1;
	}

	if (unk16C != 0) {
		++unk16C;
		if (unk16C > 400)
			unk16C = 0;
	}

	int shines = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
	if (unk8A == 0 && unk64 != shines)
		unk8A = 1;

	if (unk8A != 0) {
		if (unk8A > 0xFB) {
			int spentBlueCoins = 0;
			for (int flag = 0x46; flag < 0x56; ++flag)
				if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
				    != 0)
					++spentBlueCoins;
			for (int flag = 0x6C; flag <= 0x73; ++flag)
				if (TFlagManager::getInstance()->getFlag(MSF_SHINE_BASE + flag)
				    != 0)
					++spentBlueCoins;

			if (unk170 != blueCoins - spentBlueCoins * 10) {
				--unk170;
				if (unk170 < 0) {
					unk170 = 0;
				} else if (SMSGetMSound()->gateCheck(0x4850)) {
					MSoundSESystem::MSoundSE::startSoundSystemSE(0x4850, 0,
					                                             nullptr, 0);
				}
				if (unk170 < 100) {
					setDigitPane(mBlueCoinCountDigits[0], unkE0,
					             (int)(unk170 * 0.1f));
					setDigitPane(mBlueCoinCountDigits[1], unkE0, unk170 % 10);
					if (mBlueCoinCountDigits[2]->getPane()->isVisible())
						mBlueCoinCountDigits[2]->getPane()->hide();
				} else {
					setDigitPane(mBlueCoinCountDigits[0], unkE0,
					             (int)(unk170 * 0.01f));
					int remainder = unk170 - (int)(unk170 * 0.01f) * 100;
					setDigitPane(mBlueCoinCountDigits[1], unkE0,
					             (int)(remainder * 0.1f));
					setDigitPane(mBlueCoinCountDigits[2], unkE0,
					             remainder % 10);
					if (!mBlueCoinCountDigits[2]->getPane()->isVisible())
						mBlueCoinCountDigits[2]->getPane()->show();
				}
			}
		}

		if (unk8A == 0xFC) {
			int value = unk64;
			// TODO: never used here. The coin counter in performCalcAnim
			// declares the same default JUTRect and uses it, so this one is
			// probably left over from code that was moved into changeNum.
			JUTRect bounds;

			if (value < 100) {
				if (mShineCountDigits[2]->getPane()->isVisible())
					mShineCountDigits[2]->getPane()->hide();

				if (value % 10 == 0)
					changeNum(mShineCountDigits[0], value / 10, 10);

				changeNum(mShineCountDigits[1], value - value / 10 * 10, 10);
			} else {
				if (value % 100 == 0) {
					changeNum(mShineCountDigits[0], value / 100, 10);
					if (!mShineCountDigits[0]->getPane()->isVisible())
						mShineCountDigits[0]->getPane()->show();
				}

				int remainder = value - (int)(value * 0.01f) * 100;
				if (remainder % 10 == 0)
					changeNum(mShineCountDigits[1], remainder / 10, 10);

				if (!mShineCountDigits[2]->getPane()->isVisible())
					mShineCountDigits[2]->getPane()->show();
				changeNum(mShineCountDigits[2], remainder % 10, 10);
			}
		} else if (unk8A == 0x106) {
			if (shines > unk64) {
				++unk64;
				unk8A = 0xFB;
			}
		} else if (!unk34 && !unk35) {
			mShineCountDigits[0]->update();
			mShineCountDigits[1]->update();
			mShineCountDigits[2]->update();
		}

		++unk8A;
	}

#else
	countBlueCoin();
	countShine();
#endif

	if (unk34) {
		bool done = true;
		done &= processAppearStar(unk5C);
		done &= processDownCoin(unk5C);
		if (done) {
			int shines = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
			if (unk24 != shines)
				unk24 = shines;
			unk34 = 0;
		}
		++unk5C;
	}

	TWaterGun* waterGun = gpMarioOriginal->getWaterGun();
	s32 currentWater    = waterGun->getCurrentWater();
	s32 maxWater        = waterGun->getMaxWater();
	TWaterGun::TNozzleType currentNozzle
	    = (TWaterGun::TNozzleType)waterGun->getCurrentNozzleType();

	if (unk2F8->isInterpolatorAtZero() && !unk45 && currentNozzle != unk310) {
		unk274->getPane()->hide();
		unk288->hide();
		if (currentNozzle == TWaterGun::Spray) {
			unk274 = unk278[0];
			unk288 = unk28C[0];
		} else if (currentNozzle == TWaterGun::Rocket) {
			unk274 = unk278[2];
			unk288 = unk28C[2];
		} else if (currentNozzle == TWaterGun::Hover) {
			unk274 = unk278[1];
			unk288 = unk28C[1];
		} else if (currentNozzle == TWaterGun::Turbo) {
			unk274 = unk278[3];
			unk288 = unk28C[3];
		} else if (currentNozzle == TWaterGun::Underwater) {
			unk274 = unk278[1];
			unk288 = unk28C[1];
		}
		unk310 = currentNozzle;
		unk274->getPane()->show();
		unk288->show();
	}

	if (unk28 != currentWater && currentWater == maxWater
	    && SMSGetMSound()->gateCheck(0x4807)) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(0x4807, 0, nullptr, 0);
	}

	TYoshi* yoshi = gpMarioOriginal->mYoshi;
	if (yoshi->onYoshi())
		unk2B8 = (f32)yoshi->unkC / (f32)yoshi->unk8;
	else
		unk2B8 = (f32)currentWater / (f32)maxWater;

	if (unk2B8 >= 1.0f)
		unk2A0[0]->setAlpha(0xff);
	else if (unk2B8 == 0.0f)
		unk2A0[0]->setAlpha(0);
	else
		unk2A0[0]->setAlpha(0x50);

	unk28 = currentWater;
	if (!unk46) {
		if (gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD) && !unk45
		    && !unk50)
			startAppearTank();
	}

	if (unk45) {
		if (processAppearTank(unk7C++)) {
			unk45 = 0;
			unk46 = 1;
			unk7C = 0;
		}
	}

	if (unk46) {
		if (SMSGetMarDirector()->getState() == TMarDirector::STATE_PAUSE_MENU)
			unk7C = 0;

		if (unk7C < 0x46)
			unk288->setAlpha(0);
		else if (unk7C < 0x10e)
			unk288->setAlpha(0xff);
		else
			unk7C = 0;

		++unk7C;
	}

	if (unk4B) {
		bool done = true;
		done &= unk2F8->update();
		done &= unk274->update();
		done &= unk270->update();
		done &= unk26C->update();
		if (done) {
			unk4B = 0;
			unk46 = 0;
		}
	}

	if (mLifeMeterRoot->getPane()->isVisible()) {
		f32 rate = 100.0f;
		if (amount == 3)
			rate = 80.0f;
		if (amount == 2)
			rate = 60.0f;
		if (amount == 1)
			rate = 35.0f;

		if ((f32)unk86 < rate * 2.0f) {
			if (unk1CC <= 3 && unk1CC != 0 && unk86 == (int)(1.5f * rate)
			    && SMSGetMarDirector()->getState() == TMarDirector::STATE_UNK4
			    && SMSGetMarDirector()->isGameplayModeNow()
			    && SMSGetMSound()->gateCheck(0x4800)) {
				MSoundSESystem::MSoundSE::startSoundSystemSE(0x4800, 0, nullptr,
				                                             0);
			}

			f32 diff  = (f32)unk86 - rate;
			f32 ratio = (diff >= 0.0f ? diff : -diff) / rate;

			f32 mul = 3.0f;
			if (amount <= 3)
				mul = 5.0f;

			u8 offset = mul * (int)(0.5f + ratio);
			for (int i = 0; i < 9; ++i) {
				if (mLifeMeterPetals[i * 2]->isVisible()) {
					mLifeMeterPetals[i * 2]->place(
					    JUTRect(mLifeMeterPetalsOriginalBounds[i].x1 - offset,
					            mLifeMeterPetalsOriginalBounds[i].y1 - offset,
					            mLifeMeterPetalsOriginalBounds[i].x2 + offset,
					            mLifeMeterPetalsOriginalBounds[i].y2 + offset));
					mLifeMeterPetals[i * 2 + 1]->place(
					    JUTRect(mLifeMeterPetalsOriginalBounds[i].x1 - offset,
					            mLifeMeterPetalsOriginalBounds[i].y1 - offset,
					            mLifeMeterPetalsOriginalBounds[i].x2 + offset,
					            mLifeMeterPetalsOriginalBounds[i].y2 + offset));
				}
			}

			++unk86;
		} else if ((f32)unk86 < rate * 2.0f) {
			++unk86;
		} else {
			unk86 = 0;
		}
	}

	if (unk4F) {
		if (processAppearCoin(unk88++))
			unk4F = 0;
	}

	if (unk4D && mCoinCountRoot->update() && mShineCountRoot->update()) {
		mCoinCountRoot->getPane()->hide();
		unk4D = 0;
	}

	if (!unk45)
		processMoveNozzle();

	if (unk3A) {
		if (processAppearMario(unk70++)) {
			if (unk3AD) {
				if (unk70 == 200) {
					int lives
					    = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);
					if (lives > 99)
						lives = 99;
					((J2DPicture*)mLifeCountDigits[0]->getPane())
					    ->changeTexture(
					        unkE0[(int)(lives * 0.1f)]->getTexInfo(), 0);
					((J2DPicture*)mLifeCountDigits[1]->getPane())
					    ->changeTexture(unkE0[lives % 10]->getTexInfo(), 0);
					TFlagManager::getInstance()->setBool(
					    false, MSF_LOST_LIFE_IN_PREV_STAGE);
					endCameraDemo();
					unk3A = 0;
				}
			} else {
				unk3AD = 0;
				unk70  = 0;
			}
		}
	}

#if defined(VERSION_GMSJ01)
	if (!unk3A && !unk3B && mLifeCountRoot->getPane()->isVisible()
	    && gpMarioOriginal->mStatus != 0xC400201
	    && SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU) {
		++unk70;
		if (unk70 > 400)
			startDisappearMario();
	}
#else
	if (mLifeCountRoot->getPane()->isVisible()
	    && gpMarioOriginal->mStatus != 0xC400201
	    && SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU) {
		++unk3AE;
		if (unk3AE > 400)
			startDisappearMario();
	}
#endif

	int lives = TFlagManager::getInstance()->getFlag(MSF_LIFE_COUNT);
	if (lives > unk3AC) {
		if (lives > 99)
			lives = 99;
		unk3AC = lives;
		((J2DPicture*)mLifeCountDigits[0]->getPane())
		    ->changeTexture(unkE0[(int)(lives * 0.1f)]->getTexInfo(), 0);
		((J2DPicture*)mLifeCountDigits[1]->getPane())
		    ->changeTexture(unkE0[lives % 10]->getTexInfo(), 0);
		startAppearMario(false);
	}

	if (unk3B && mLifeCountRoot->update()) {
		unk3B = 0;
		mLifeCountRoot->getPane()->hide();
	}

	if (unk42) {
		if (mTelopRoot->update()) {
			unk42 = 0;
			unk44 = 1;
			unk80 = 0;
		}
	} else if (unk43 && mTelopRoot->update()) {
		unk43 = 0;
		mTelopRoot->getPane()->hide();
	}

	if (unk44 && mTelopRoot->getPane()->isVisible()) {
		if (processDrawTelop(unk80++)) {
			JUTRect bounds(mTelop->getPane()->getGlbBounds());
			unk568 = unk544.x2;
			unk534.add(mTelopTextWidth + bounds.x2, 0);

			if (unk56D) {
				unk56D = 0;
			} else {
				unk44  = 0;
				unk55C = 0;
				startDisappearTelop();
			}
		}
	}

	s16 telopWait = unk56C ? unk562 : unk560;
	if (!unk44 && !unk42 && !unk43 && unk530->unk4 != nullptr
	    && unk55C >= (u32)(telopWait * 120)
	    && gpMarioOriginal->mStatus == 0xC400201) {
		if (unk56C)
			unk56C = 0;
		if (unk570 != nullptr) {
			++unk558;
			if (unk570[unk558] == 0xffffffff)
				unk558 = 0;
		}
		checkChangeTelopArray();
		startAppearTelop(true);
	}

	if (SMSGetMarDirector()->getState() != TMarDirector::STATE_PAUSE_MENU
	    && SMSGetMarDirector()->isGameplayModeNow()) {
		if (unk55C < 0xffffffff)
			++unk55C;
	}

	if (unk3D) {
		if (processAppearJet(unk72++))
			unk3D = 0;
	}

	int redCoins = TFlagManager::getInstance()->getFlag(MSF_RED_COIN_COUNT);
	if (redCoins != unk444) {
		if (redCoins < 0)
			redCoins = 0;
		if (redCoins > 8)
			redCoins = 8;

		((J2DPicture*)unk43C[0]->getPane())
		    ->changeTexture(unkE0[redCoins]->getTexInfo(), 0);
		if (!unk428->getPane()->isVisible())
			startAppearRedCoin();

		JUTRect bounds(unk43C[0]->getPane()->getGlbBounds());
		JGeometry::TVec3<f32> pos(bounds.x1 + bounds.getWidth() * 0.5f,
		                          bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
		gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr, nullptr);
		unk444 = redCoins;
	}

	if (unk3C) {
		if (processAppearRed(unk74++))
			unk3C = 0;
	}

	if (unk58 && unk428->update())
		unk58 = 0;

	if (unk3E) {
		if (processAppearTimer(unk76++))
			unk3E = 0;
	}

	if (unk3F) {
		if (unk44C->update())
			unk3F = 0;
	} else if (unk4A) {
		setTimer(-1);
	}

	if (unk59) {
		if (SMSGetMarDirector()->isGameplayModeNow()
		    && gpMarioOriginal->getHealth() != 0
		    && gpMarioOriginal->getAir() != 0
#if !defined(VERSION_GMSJ01)
		    && (SMSGetMarDirector()->getCurrentStage() != 9
		        || SMSGetMarDirector()->getCurrentScenario() != 2)
#endif
		    && SMSGetMSound()->gateCheck(0x4819))
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4819, 0, nullptr, 0);
		unk59 = 0;
	}

	if (unk5A) {
		if (SMSGetMarDirector()->isGameplayModeNow()
		    && gpMarioOriginal->getHealth() != 0
		    && gpMarioOriginal->getAir() != 0
#if !defined(VERSION_GMSJ01)
		    && (SMSGetMarDirector()->getCurrentStage() != 9
		        || SMSGetMarDirector()->getCurrentScenario() != 2)
#endif
		    && SMSGetMSound()->gateCheck(0x481A))
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x481A, 0, nullptr, 0);
		unk5A = 0;
	}
}

// fabricated
// TODO: the ROM keeps TBlendPane::setPaneBlend and processAppearBalloon out of
// line here, so the calc-anim cue work is one inline body below perform, as
// performMovement is.
void TGCConsole2::performCalcAnim()
{
	switch (unk10) {
	case 1:
		if (processAppearBalloon()) {
			unk10      = 2;
			char* text = unk3B8->getStringPtr();
			int n      = 0x400;
			while (n--)
				*text++ = 0;
			text = unk3B4->getStringPtr();
			n    = 0x400;
			while (n--)
				*text++ = 0;
			unk3B8->show();
		}
		break;
	case 2:
		if (unk3D4->getAvailable() != 0 && unk3D8->getAvailable() != 0) {
			for (int i = 0; i < 2; ++i) {
				u8 c = unk3D4->read8b();

				switch (c) {
				case 0x1A:
					unk3D4->skip(unk3D4->read8b() - 2);
					break;
				case '\0':
					unk10 = 3;
				case '\n':
					*unk3D8 << c;
					*unk3DC << c;
					break;
				default:
#if defined(VERSION_GMSP01_NX)
					if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE)
					        == MSLANG_NX_JAPANESE
					    && c >= 0x80) {
#elif defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)
					if (c >= 0x80) {
#endif
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)                      \
    || defined(VERSION_GMSP01_NX)
						u8 trail = unk3D4->read8b();
						u16 code = (c << 8) + trail;

						char buffer[256];
						JUtility::TColor color;
						bool colored = true;
						switch (code) {
						case 0x8197:
							color.set(100, 255, 100, 255);
							break;
						case 0x8193:
							color.set(255, 160, 100, 255);
							break;
						case 0x8192:
							color.set(255, 255, 0, 255);
							break;
						case 0x817B:
						case 0x8183:
						case 0x8184:
						case 0x818E:
							color.set(220, 220, 220, 255);
							break;
						case 0x818F:
							color.set(110, 230, 255, 255);
							break;
						default:
							colored = false;
							break;
						}

						if (colored) {
							snprintf(buffer, 255,
							         "\033GM[0]\033CC[%02x%02x%02x]\033FX[28]"
							         "\033FY[28]\033SH[3]\033CD[4]",
							         color.r, color.g, color.b);
							unk3D8->write(buffer, 0x2B);
							unk3DC->write(buffer, 0x2B);
						}

						*unk3D8 << code;
						*unk3DC << code;

						if (colored) {
							snprintf(
							    buffer, 255,
							    "\033GM[0]\033CC\033FX\033FY\033SH\033CU[4]");
							unk3D8->write(buffer, 0x18);
							unk3DC->write(buffer, 0x18);
						}
#if defined(VERSION_GMSP01_NX)
					} else if (TFlagManager::getInstance()->getFlag(
					               MSF_LANGUAGE)
					           == 5) {
						*unk3D8 << c;
						*unk3DC << c;
					} else {
						char buffer[256];
						JUtility::TColor color;
						bool special = Helpers::IsSpecialChar(c);
						if (special) {
							color.set(255, 255, 255, 255);
							snprintf(buffer, 255,
							         "\033GM[0]\033CC[%02x%02x%02x]\033FX[22]"
							         "\033FY[24]\033SH[0]\033CD[3]",
							         color.r, color.g, color.b);
							unk3D8->write(buffer, 0x2B);
							unk3DC->write(buffer, 0x2B);
						}

						*unk3D8 << c;
						*unk3DC << c;

						if (special) {
							snprintf(
							    buffer, 255,
							    "\033GM[0]\033CC\033FX\033FY\033SH\033CU[3]");
							unk3D8->write(buffer, 0x18);
							unk3DC->write(buffer, 0x18);
						}
					}
#else
					} else {
						*unk3D8 << c;
						*unk3DC << c;
					}
#endif
					break;
#else
						{
							char buffer[256];
							JUtility::TColor color;
							bool colored = true;
							switch (c) {
							case '@':
								color.set(100, 255, 100, 255);
								break;
							case '#':
								color.set(255, 160, 100, 255);
								break;
							case '%':
								color.set(255, 255, 0, 255);
								break;
							case '+':
							case '<':
							case '>':
							case 0xA5:
								color.set(220, 220, 220, 255);
								break;
							case '$':
								color.set(110, 230, 255, 255);
								break;
#if defined(VERSION_GMSP01)
							case '*':
								color.set(220, 220, 220, 255);
								break;
#endif
							default:
								colored = false;
								break;
							}

							if (colored) {
								snprintf(
								    buffer, 255,
								    "\033GM[0]\033CC[%02x%02x%02x]\033FX[30]"
								    "\033FY[26]\033SH[3]\033CD[4]",
								    color.r, color.g, color.b);
								unk3D8->write(buffer, 0x2B);
								unk3DC->write(buffer, 0x2B);
							}

							*unk3D8 << c;
							*unk3DC << c;

							if (colored) {
								snprintf(buffer, 255,
								         "\033GM[0]"
								         "\033CC\033FX\033FY\033SH\033CU[4]");
								unk3D8->write(buffer, 0x18);
								unk3DC->write(buffer, 0x18);
							}
							break;
						}
#endif
				}
			}
		} else {
			unk10 = 3;
		}
	case 3:
		if (unk3E4 > 0) {
			--unk3E4;
			if ((unk3F8 && unk3E4 == 0) || unk3F4 != 0xffffffff)
				startDisappearBalloon(unk3E0, false);
		}
		break;
	case 4: {
		bool done = true;
		if (processDisappearBalloon()) {
			unk3B0->hide();
			unk3B0->resize(0, unk3BC.getHeight());
			unk3B0->add(0, -unk3B0->getContentsBounds().getHeight());
		} else {
			done = false;
		}

		if (done) {
			unk3F0 = 0;
			unk10  = 0;
			if (unk3F4 != 0xffffffff) {
				startAppearBalloon(unk3F4, true);
				unk3F4 = 0xffffffff;
			}
		}
		break;
	}
	}

	if (unk404 == unk408) {
		int balloons = TFlagManager::getInstance()->getFlag(MSF_BALLOON_COUNT);
		if (balloons > 99)
			balloons = 99;

		if (balloons >= unk2C) {
			bool done = unk414[0]->update();
			done &= unk414[1]->update();
			if (done) {
				if (unk2C > 99)
					unk2C = 99;

				if (unk2C != balloons) {
					++unk2C;
					if (unk2C >= 10) {
						if (!unk414[0]->getPane()->isVisible())
							unk414[0]->getPane()->show();
						unk414[0]->setPaneBlend(12, unkE0[unk2C / 10],
						                        unkE0[unk2C / 10 - 1]);
					} else {
						if (unk414[0]->getPane()->isVisible())
							unk414[0]->getPane()->hide();
					}
					unk414[1]->setPaneBlend(12, unkE0[unk2C % 10],
					                        unkE0[unk2C % 10 - 1]);
				}
			}
		}
	} else if (unk404 == unk40C) {
		int cleaned
		    = TFlagManager::getInstance()->getFlag(MSF_PIANTA_CLEAN_COUNT);
		bool done = true;
		done &= unk414[0]->update();
		done &= unk414[1]->update();
		if (done) {
			if (cleaned > 99)
				cleaned = 99;
			if (cleaned < 0)
				cleaned = 0;

			if (unk2C < cleaned) {
				++unk2C;
				if (unk2C >= 10) {
					if (!unk414[0]->getPane()->isVisible())
						unk414[0]->getPane()->show();
					if (unk2C % 10 == 0)
						setDigitPane(unk414[0], unkE0, unk2C / 10);
				} else {
					if (unk414[0]->getPane()->isVisible())
						unk414[0]->getPane()->hide();
				}
				setDigitPane(unk414[1], unkE0, unk2C % 10);
			}
		}
	}

	if (unk68 > 0) {
		bool counting = true;
		if (unk68 == 1) {
			if (unk20 >= unk6C)
				++unk6C;
			if (unk20 < unk6C) {
				--unk6C;
				counting = false;
			}

			if (unk6C > 999) {
				unk6C = 999;
				unk20 = 999;
			}
			if (unk6C < 0) {
				unk6C = 0;
				unk20 = 0;
			}

			if (counting) {
				JUTRect bounds;
				JGeometry::TVec3<f32> pos;
				if (unk6C >= 100) {
					if (unk6C % 100 == 0) {
						setDigitPane(mCoinCountDigits[0], unkE0, unk6C / 100);
						bounds = mCoinCountDigits[0]->getPane()->getGlbBounds();
						pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
						        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
						gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
						                                   nullptr);
					}

					int value     = unk6C;
					int remainder = value - (int)(value * 0.01f) * 100;
					if (remainder % 10 == 0) {
						setDigitPane(mCoinCountDigits[1], unkE0,
						             remainder / 10);
						bounds = mCoinCountDigits[1]->getPane()->getGlbBounds();
						pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
						        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
						gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
						                                   nullptr);
					}

					if (!mCoinCountDigits[2]->getPane()->isVisible())
						mCoinCountDigits[2]->getPane()->show();
					setDigitPane(mCoinCountDigits[2], unkE0, remainder % 10);
					bounds = mCoinCountDigits[2]->getPane()->getGlbBounds();
					pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
					        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
					gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
					                                   nullptr);
				} else {
					if (unk6C % 10 == 0) {
						setDigitPane(mCoinCountDigits[0], unkE0, unk6C / 10);
						bounds = mCoinCountDigits[0]->getPane()->getGlbBounds();
						pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
						        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
						gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
						                                   nullptr);
					}

					setDigitPane(mCoinCountDigits[1], unkE0, unk6C % 10);
					bounds = mCoinCountDigits[1]->getPane()->getGlbBounds();
					pos.set(bounds.x1 + bounds.getWidth() * 0.5f,
					        bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
					gpEmitterManager4D2->createEmitter(pos, 0x1FC, nullptr,
					                                   nullptr);

					if (mCoinCountDigits[2]->getPane()->isVisible())
						mCoinCountDigits[2]->getPane()->hide();
				}
			} else if (unk6C >= 100) {
				if (unk6C % 100 == 99)
					setDigitPane(mCoinCountDigits[0], unkE0, unk6C / 100);

				int value     = unk6C;
				int remainder = value - (int)(value * 0.01f) * 100;
				if (remainder % 10 == 9)
					setDigitPane(mCoinCountDigits[1], unkE0, remainder / 10);

				if (!mCoinCountDigits[2]->getPane()->isVisible())
					mCoinCountDigits[2]->getPane()->show();
				setDigitPane(mCoinCountDigits[2], unkE0, remainder % 10);
			} else {
				if (unk6C % 10 == 9)
					setDigitPane(mCoinCountDigits[0], unkE0, unk6C / 10);
				setDigitPane(mCoinCountDigits[1], unkE0, unk6C % 10);
				if (mCoinCountDigits[2]->getPane()->isVisible())
					mCoinCountDigits[2]->getPane()->hide();
			}

			++unk68;
		}

		bool done = true;
		if (!unk34) {
			for (int i = 0; i < 3; ++i)
				done &= mCoinCountDigits[i]->update();
		}

		if (done) {
			if (unk20 == unk6C) {
				unk68 = 0;
				unk78 = 20;
			} else {
				unk68 = 1;
			}
		}
	}

	if (gpMarioOriginal->mYoshi->onYoshi()) {
		if (unk29C->getPane()->isVisible()) {
			unk29C->getPane()->hide();
			unk324->show();
			unk314[unk7A]->show();
		}

		u8 type = gpModelWaterManager->unk5D5F;
		if (unk7A != type) {
			unk314[unk7A]->hide();
			unk314[type]->show();
			unk7A = type;
		}
	} else {
		if (!unk29C->getPane()->isVisible()) {
			unk29C->getPane()->show();
			unk324->hide();
			unk314[unk7A]->hide();
		}

		if (unkB4 >= 0xff) {
			unk29C->getPane()->show();
			unk324->hide();
		}
	}
}

void TGCConsole2::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE)
		performMovement();

	if (cue & CUE_CALC_ANIM)
		performCalcAnim();

	if (cue & CUE_DRAW) {
		J2DOrthoGraph graph(graphics->getViewport());
		graph.setup2D();

		if (unk46 || unk45)
			drawWaterBack();

		graph.setup2D();

		mHudScreen->draw(0, 0, &graph);

		if (unk44 && !unk43 && !unk42 && mTelopRoot->getPane()->isVisible()) {
			graphics->setScissor(
			    JDrama::TRect(unk544.x1, unk544.y1, unk544.x2, unk544.y2));
			graph.setup2D();
			unk528->show();
			unk52C->show();
			unk52C->draw(unk534.x1 + 2, unk534.y1 + 2);
			unk528->draw(unk534.x1, unk534.y1);
			unk528->hide();
			unk52C->hide();
		}

		graphics->setScissor(graphics->mViewportRect);
		graph.setup2D();
		drawWaterOrJuice(this, graph);
	}
}
