#include <GC2D/GCConsole2.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/BlendPane.hpp>
#include <GC2D/ConsoleStr.hpp>
#include <GC2D/MessageLoader.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/HelpActor.hpp>
#include <GC2D/ExPane.hpp>
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

extern JPAEmitterManager* gpEmitterManager4D2;
bool SMS_isDivingMap();

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <System/DummyStrings.hpp>

JUTPoint TGCConsole2::cDownTopPoint(0, -100);
JUTPoint TGCConsole2::cDownMidPoint(0, 30);
JUTPoint TGCConsole2::cDownBotPoint(0, 0);
JUTPoint TGCConsole2::cUpTopPoint(0, 50);
JUTPoint TGCConsole2::cUpMidPoint(0, -30);
JUTPoint TGCConsole2::cUpBotPoint(0, 0);
JUTPoint TGCConsole2::cCoinTopPoint(0, 0);
JUTPoint TGCConsole2::cCoinMidPoint(0, 45);
JUTPoint TGCConsole2::cCoinBotPoint(0, 0);

// Possibly inline
static inline int getOffsetForAboveScreen(const TExPane* pane)
{
	return -(pane->getInitialY2() + 1);
}

// Possibly inline
static inline int getOffsetForBelowScreen(const TExPane* pane)
{
	return 465 - pane->getInitialY1();
}

// Possibly inline
static inline void setEmitterToPaneCenter(JPABaseEmitter* emitter,
                                          J2DPane* pane)
{
	JUTRect bounds(pane->mGlobalBounds);
	emitter->mGlobalTranslation.set(bounds.x1 + bounds.getWidth() * 0.5f,
	                                bounds.y1 + bounds.getHeight() * 0.5f,
	                                0.0f);
}

// fabricated
static inline void syncPaneBounds(TBoundPane* pane)
{
	pane->unk4 = pane->getPane()->mBounds;
}

// fabricated
static inline void detachPaneFromParent(J2DPane* pane)
{
	pane->mPaneTree.getParent()->removeChild(&pane->mPaneTree);
}

// fabricated
static inline void drawGaugeQuadF32(const JUTRect& rect, int top, int bottom,
                                    f32 topTex, f32 bottomTex)
{
	GXBegin(GX_QUADS, GX_VTXFMT0, 4);
	GXPosition2f32((f32)rect.x1, (f32)top);
	GXTexCoord2f32(0.0f, topTex);
	GXPosition2f32((f32)rect.x2, (f32)top);
	GXTexCoord2f32(1.0f, topTex);
	GXPosition2f32((f32)rect.x2, (f32)bottom);
	GXTexCoord2f32(1.0f, bottomTex);
	GXPosition2f32((f32)rect.x1, (f32)bottom);
	GXTexCoord2f32(0.0f, bottomTex);
}

// fabricated
static inline u32 getPressureFlashColor(u8 frame)
{
	u32 color = 0xff3f3f00;

	if (frame < 10) {
		color += ((u32)(s16)(s32)((f32)frame * -6.3f)) << 8;
		color += ((u32)(s32)((f32)frame * 19.2f)) << 16;
	} else if (frame < 15) {
		color = 0xffff0000;
	} else if (frame < 25) {
		u8 fade = 25 - frame;
		color += ((u32)(s16)(s32)((f32)fade * -6.3f)) << 8;
		color += ((u32)(s32)((f32)fade * 19.2f)) << 16;
	}

	return color;
}

// fabricated
static inline bool isMountedYoshi(TMario* mario)
{
	TYoshi* yoshi = mario->mYoshi;
	return yoshi != nullptr && yoshi->mState == TYoshi::STATE_MOUNTED;
}

// fabricated
static inline void updateWaterGaugeFill(TGCConsole2* console)
{
	TMario* mario       = gpMarioOriginal;
	TWaterGun* waterGun = mario->mWaterGun;
	TNozzleBase* nozzle = waterGun->getCurrentNozzle();
	s32 currentWater    = waterGun->mCurrentWater;
	s32 maxWater        = nozzle->mEmitParams.mAmountMax.get();
	u8 currentNozzle    = waterGun->mCurrentNozzle;
	f32 fill;

	if (console->unk2F8->isInterpolatorAtZero() && !console->unk45
	    && currentNozzle != console->unk310) {
		console->unk274->getPane()->hide();
		console->unk288->hide();
		switch (currentNozzle) {
		case TWaterGun::Spray:
			console->unk274 = console->unk278[0];
			console->unk288 = console->unk28C[0];
			break;
		case TWaterGun::Rocket:
			console->unk274 = console->unk278[2];
			console->unk288 = console->unk28C[2];
			break;
		case TWaterGun::Hover:
		case TWaterGun::Underwater:
			console->unk274 = console->unk278[1];
			console->unk288 = console->unk28C[1];
			break;
		case TWaterGun::Turbo:
			console->unk274 = console->unk278[3];
			console->unk288 = console->unk28C[3];
			break;
		}
		console->unk310 = currentNozzle;
		console->unk274->getPane()->show();
		console->unk288->show();
	}

	if (console->unk28 != currentWater && currentWater == maxWater
	    && SMSGetMSound()->gateCheck(0x4807)) {
		MSoundSESystem::MSoundSE::startSoundSystemSE(0x4807, 0, nullptr, 0);
	}

	if (isMountedYoshi(mario)) {
		TYoshi* yoshi = mario->mYoshi;
		fill          = (f32)yoshi->unkD4 / (f32)yoshi->unkD8;
	} else {
		fill = (f32)currentWater / (f32)maxWater;
	}

	console->unk2B8 = fill;
	if (fill >= 1.0f)
		console->unk2A0[0]->mAlpha = 0xff;
	else if (fill == 0.0f)
		console->unk2A0[0]->mAlpha = 0;
	else
		console->unk2A0[0]->mAlpha = 0x50;

	console->unk28 = currentWater;
}

// fabricated
static inline void updateYoshiJuiceIconState(TGCConsole2* console)
{
	if (isMountedYoshi(gpMarioOriginal)) {
		if (console->unk29C->getPane()->isVisible()) {
			console->unk29C->getPane()->hide();
			console->unk324->show();
			console->unk314[(u8)console->unk7A[0]]->show();
		}

		u8 type = gpModelWaterManager->unk5D5F;
		if ((u8)console->unk7A[0] != type) {
			console->unk314[(u8)console->unk7A[0]]->hide();
			console->unk314[type]->show();
			console->unk7A[0] = type;
		}
	} else {
		if (!console->unk29C->getPane()->isVisible()) {
			console->unk29C->getPane()->show();
			console->unk324->hide();
			console->unk314[(u8)console->unk7A[0]]->hide();
		}

		if (console->unkB4 >= 0xff) {
			console->unk29C->getPane()->show();
			console->unk324->hide();
		}
	}
}

// fabricated
static inline void playHudMoveSound(u32 soundID)
{
	if (gpMarDirector->unk124 != 0)
		return;
	if (gpMarioOriginal->mHealth == 0)
		return;
	if ((s16)gpMarioOriginal->mAir == 0)
		return;
	SMSGetMSound()->startSoundSystemSE(soundID, 0, nullptr, 0);
}

// fabricated
static inline void writeBalloonTextByte(TGCConsole2* console, u8 value)
{
	console->unk3D8->write(&value, 1);
	console->unk3DC->write(&value, 1);
}

// fabricated
static inline void writeBalloonColor(TGCConsole2* console, const char* text,
                                     int length)
{
	console->unk3D8->write(text, length);
	console->unk3DC->write(text, length);
}

// fabricated
static inline void processBalloonTextStep(TGCConsole2* console)
{
	JSUMemoryInputStream* input   = (JSUMemoryInputStream*)console->unk3D4;
	JSUMemoryOutputStream* output = (JSUMemoryOutputStream*)console->unk3D8;

	if (input->getAvailable() == 0 || output->getAvailable() == 0) {
		console->unk10 = 3;
		return;
	}

	for (int i = 0; i < 2; ++i) {
		u8 value;
		input->read(&value, 1);

		if (value == 0x1A) {
			u8 skip;
			input->read(&skip, 1);
			input->seek((s32)skip - 2, JSUStreamSeekFrom_CUR);
			continue;
		}

		if (value == 0) {
			writeBalloonTextByte(console, value);
			console->unk10 = 3;
			continue;
		}

		if (value >= 0x80) {
			u8 trail;
			input->read(&trail, 1);
			u8 color[4];
			u16 code = ((u16)value << 8) | trail;
			bool hasColor;
			switch (code) {
			case 0x817B:
				color[0] = 0xDC;
				color[1] = 0xDC;
				color[2] = 0xDC;
				color[3] = 0xFF;
				hasColor = true;
				break;
			case 0x8184:
			case 0x8185:
				color[0] = 0xFF;
				color[1] = 0xFF;
				color[2] = 0x00;
				color[3] = 0xFF;
				hasColor = true;
				break;
			case 0x8191:
				color[0] = 0x6E;
				color[1] = 0xE6;
				color[2] = 0xFF;
				color[3] = 0xFF;
				hasColor = true;
				break;
			case 0x8194:
				color[0] = 0xFF;
				color[1] = 0xA0;
				color[2] = 0x64;
				color[3] = 0xFF;
				hasColor = true;
				break;
			case 0x8196:
			case 0x8197:
				color[0] = 0x64;
				color[1] = 0xFF;
				color[2] = 0x64;
				color[3] = 0xFF;
				hasColor = true;
				break;
			default:
				hasColor = false;
				break;
			}
			if (hasColor) {
				char buffer[0xff];
				snprintf(buffer, 0xff,
				         "\033GM[0]\033CC[%02x%02x%02x]\033FX[28]\033FY[28]"
				         "\033SH[3]\033CD[4]",
				         color[0], color[1], color[2]);
				writeBalloonColor(console, buffer, 0x2B);
			}
			writeBalloonTextByte(console, value);
			writeBalloonTextByte(console, trail);
			if (hasColor)
				writeBalloonColor(console,
				                  "\033GM[0]\033CC\033FX\033FY\033SH\033CU[4]",
				                  0x18);
		} else {
			writeBalloonTextByte(console, value);
		}
	}
}

// fabricated
static inline bool startLifeMeterDisappear(TGCConsole2* console, u16 frame)
{
	if (console->unk38 || console->unk4C)
		return false;

	console->unk4C = 1;
	console->unk1C4->setPanePosition(
	    40, JUTPoint(0, 0),
	    JUTPoint(0, (-(console->unk1C4->unk4.y2 + 1)
	                 - console->unk174->getPane()->getHeight())
	                    >> 1),
	    JUTPoint(0, -(console->unk1C4->unk4.y2 + 1)
	                    - console->unk174->getPane()->getHeight()));
	console->unk84 = frame;
	return true;
}

// fabricated
static inline void updateLifeMeterColors(TGCConsole2* console, bool airMode)
{
	if (console->unk1CC[0] >= 4) {
		if (airMode) {
			((J2DPicture*)console->unk178->getPane())->mWhite = 0x00FFFFFF;
			((J2DPicture*)console->unk178->getPane())->mBlack = 0x003CFF00;
			((J2DPicture*)console->unk17C[0])->mWhite         = 0x00FFFFFF;
			((J2DPicture*)console->unk17C[0])->mBlack         = 0x003CFF00;
		} else {
			((J2DPicture*)console->unk178->getPane())->mWhite = 0xFFFFFFFF;
			((J2DPicture*)console->unk178->getPane())->mBlack = 0;
			((J2DPicture*)console->unk17C[0])->mWhite         = 0xFFFFFFFF;
			((J2DPicture*)console->unk17C[0])->mBlack         = 0;
		}
	} else {
		if (airMode) {
			((J2DPicture*)console->unk178->getPane())->mWhite = 0x0010FFFF;
			((J2DPicture*)console->unk178->getPane())->mBlack = 0x003CFF00;
			((J2DPicture*)console->unk17C[0])->mWhite         = 0x0010FFFF;
			((J2DPicture*)console->unk17C[0])->mBlack         = 0x003CFF00;
		} else {
			((J2DPicture*)console->unk178->getPane())->mWhite = 0x7F7F7FFF;
			((J2DPicture*)console->unk178->getPane())->mBlack = 0;
			((J2DPicture*)console->unk17C[0])->mWhite         = 0x7F7F7FFF;
			((J2DPicture*)console->unk17C[0])->mBlack         = 0;
		}
	}
}

// fabricated
static inline void playLifeChangeSound(u32 sound)
{
	if (gpMarDirector->mState == TMarDirector::STATE_UNK4
	    && gpMarDirector->unk124 == 0) {
		SMSGetMSound()->startSoundSystemSE(sound, 0, nullptr, 0);
	}
}

// fabricated
static inline void updateLifeMeterState(TGCConsole2* console)
{
	u8 amount;
	bool airMode = true;

	switch (console->unk18) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 7:
		airMode = false;
		amount  = gpMarioOriginal->mHealth;
		break;
	case 4:
	case 5:
	case 6:
	case 8:
	case 9:
		amount = (s16)gpMarioOriginal->mAir;
		break;
	}

	if (amount > 8)
		amount = 8;

	if (airMode && gpMarDirector->mState == TMarDirector::STATE_UNK5) {
		s16 alpha = console->unk1C4->getPane()->mAlpha - 0x10;
		if (alpha < 0)
			alpha = 0;
		console->unk1C4->getPane()->mAlpha = alpha;
	} else if (console->unk1C4->getPane()->mAlpha != 0xff
	           && gpMarDirector->mState != TMarDirector::STATE_UNK5) {
		u16 alpha = console->unk1C4->getPane()->mAlpha + 0x10;
		if (alpha > 0xff)
			alpha = 0xff;
		console->unk1C4->getPane()->mAlpha = alpha;
	}

	bool airTimeout = false;
	if (gpMarioOriginal->isUnderWater()) {
		int airTimer = console->unk268;
		if (airTimer < 0xf0)
			console->unk268 = airTimer + 1;
		else
			airTimeout = true;
	} else {
		console->unk268 = 0;
	}

	switch (console->unk18) {
	case 0:
		if (console->unk1C4->getPane()->isVisible())
			console->unk1C4->getPane()->hide();

		if (SMS_isDivingMap()) {
			console->startInsertLife(1);
			console->unk18 = 8;
		}

		if (airTimeout) {
			console->resetLife(amount);
			console->startAppearLife(1);
			amount             = (s16)gpMarioOriginal->mAir;
			console->unk1CC[0] = amount;
			if (gpMarDirector->mState == TMarDirector::STATE_UNK5)
				console->unk1C4->getPane()->mAlpha = 0;
			console->unk18 = 4;
		}

		if (console->unk1CC[0] < 8 && !console->unk1C4->getPane()->isVisible()
		    && amount != 0) {
			console->startAppearLife(0);
			console->unk18 = 1;
		}
		break;
	case 1:
		if (console->processAppearLife(console->unk84++)) {
			console->unk18 = 2;
			console->unk38 = 0;
		}
		if (airTimeout && startLifeMeterDisappear(console, 1))
			console->unk18 = 3;
		break;
	case 2:
		if (console->unk1CC[0] == 8) {
			if (startLifeMeterDisappear(console, 0))
				console->unk18 = 3;
		} else if (airTimeout) {
			if (startLifeMeterDisappear(console, 0))
				console->unk18 = 3;
		}
		break;
	case 3:
		if (console->unk84 > 0x78) {
			if (console->unk1C4->update()) {
				console->unk4C = 0;
				console->unk18 = 0;
			}
		} else {
			++console->unk84;
		}
		break;
	case 4:
		if (console->processInsertLife(console->unk84++))
			console->unk18 = 2;
		break;
	case 5:
		if (console->processInsertLife(console->unk84++))
			console->unk18 = SMS_isDivingMap() ? 9 : 5;
		break;
	case 6:
		if (console->processAppearLife(console->unk84)) {
			console->unk18 = 5;
			console->unk38 = 0;
		}
		if (!gpMarioOriginal->isUnderWater() && console->unk84 == 0)
			console->unk84 = 1;
		if (!(amount >= 8 && console->unk84 == 0) && console->unk84 < 1000)
			++console->unk84;
		break;
	case 7:
		if (!gpMarioOriginal->isUnderWater()) {
			if (console->unk1CC[0] == 8) {
				if (startLifeMeterDisappear(console, 0))
					console->unk18 = 6;
			} else if (console->unk1CC[0] != console->unk1C) {
				console->startAppearLife(1);
				amount             = gpMarioOriginal->mHealth;
				console->unk1CC[0] = amount;
				console->resetLife(amount);
				console->unk18 = 1;
			}
		}
		break;
	case 8:
		if (console->unk84 > 0x78) {
			if (console->unk1C4->update()) {
				console->unk4C = 0;
				if (gpMarioOriginal->mHealth == 8) {
					console->unk18 = 0;
				} else {
					amount             = gpMarioOriginal->mHealth;
					console->unk1CC[0] = amount;
					console->startInsertLife(0);
					console->resetLife(amount);
					console->unk18 = 7;
				}
			}
		} else {
			++console->unk84;
		}
		break;
	case 10:
		if (console->unk50) {
			if (console->unk1C4->update())
				console->unk4C = 0;
		} else if (gpMarioOriginal->mHealth != 8) {
			console->startInsertLife(0);
			console->unk18 = 7;
		} else if (gpMarioOriginal->isUnderWater() || SMS_isDivingMap()) {
			console->startInsertLife(1);
			console->unk18 = 8;
		} else {
			console->unk1CC[0] = 8;
			console->unk18     = 0;
		}
		break;
	}

	if (console->unk18 == 10 && amount == 0) {
		console->unk1C4->getPane()->hide();
		console->unk274->getPane()->hide();
		console->unk270->getPane()->hide();
		console->unk26C->getPane()->hide();
		console->unk3A8->getPane()->hide();
	}

	if (amount != console->unk1CC[0] && console->unk18 != 10) {
		if (console->unk1CC[0] < amount) {
			++console->unk1CC[0];
			console->unk17C[console->unk1CC[0] * 2]->show();
			playLifeChangeSound(0x4801);
		} else if (console->unk1CC[0] > 0) {
			console->unk17C[console->unk1CC[0] * 2]->hide();
			console->unk17C[console->unk1CC[0] * 2]->setBounds(
			    JUTRect(console->unk1D0[console->unk1CC[0]].x1,
			            console->unk1D0[console->unk1CC[0]].y1,
			            console->unk1D0[console->unk1CC[0]].x2,
			            console->unk1D0[console->unk1CC[0]].y2));
			console->unk17C[console->unk1CC[0] * 2 + 1]->setBounds(
			    JUTRect(console->unk1D0[console->unk1CC[0]].x1,
			            console->unk1D0[console->unk1CC[0]].y1,
			            console->unk1D0[console->unk1CC[0]].x2,
			            console->unk1D0[console->unk1CC[0]].y2));
			--console->unk1CC[0];
			playLifeChangeSound(0x4823);
		}
		console->unk1C = console->unk1CC[0];
		if (console->unk1CC[0] >= 4) {
			if (airMode) {
				((J2DPicture*)console->unk178->getPane())->mWhite = 0x00FFFFFF;
				((J2DPicture*)console->unk178->getPane())->mBlack = 0x003CFF00;
				((J2DPicture*)console->unk17C[0])->mWhite         = 0x00FFFFFF;
				((J2DPicture*)console->unk17C[0])->mBlack         = 0x003CFF00;
			} else {
				((J2DPicture*)console->unk178->getPane())->mWhite = 0xFFFFFFFF;
				((J2DPicture*)console->unk178->getPane())->mBlack = 0;
				((J2DPicture*)console->unk17C[0])->mWhite         = 0xFFFFFFFF;
				((J2DPicture*)console->unk17C[0])->mBlack         = 0;
			}
		} else {
			if (airMode) {
				((J2DPicture*)console->unk178->getPane())->mWhite = 0x0010FFFF;
				((J2DPicture*)console->unk178->getPane())->mBlack = 0x003CFF00;
				((J2DPicture*)console->unk17C[0])->mWhite         = 0x0010FFFF;
				((J2DPicture*)console->unk17C[0])->mBlack         = 0x003CFF00;
			} else {
				((J2DPicture*)console->unk178->getPane())->mWhite = 0x7F7F7FFF;
				((J2DPicture*)console->unk178->getPane())->mBlack = 0;
				((J2DPicture*)console->unk17C[0])->mWhite         = 0x7F7F7FFF;
				((J2DPicture*)console->unk17C[0])->mBlack         = 0;
			}
		}
	}
}

// fabricated
static inline void detachBoundPaneFromParent(TBoundPane* pane)
{
	detachPaneFromParent(pane->getPane());
}

// fabricated
static inline void initHiddenPaneAbove(TExPane* pane)
{
	pane->updatePaneOffset(1, 0, -(pane->getInitialY2() + 1));
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
template <class Pane>
static inline void setThreeDigits(Pane** panes, JUTTexture** textures,
                                  int value, bool showHundreds)
{
	int hundreds = value / 100;
	int tens     = (value / 10) % 10;
	int ones     = value % 10;

	setDigitPane(panes[0], textures, hundreds);
	setDigitPane(panes[1], textures, tens);
	setDigitPane(panes[2], textures, ones);

	if (showHundreds && value >= 100)
		panes[0]->getPane()->show();
	else
		panes[0]->getPane()->hide();
}

// fabricated
static inline void setTwoDigits(TBoundPane** panes, JUTTexture** textures,
                                int value)
{
	setDigitPane(panes[0], textures, (int)(value * 0.1f));
	setDigitPane(panes[1], textures, value % 10);
}

// fabricated
static inline void updateMarioLifeCounter(TGCConsole2* console)
{
	int lives = TFlagManager::smInstance->getFlag(0x20001);
	if (lives > console->unk3AC[0]) {
		if (lives > 99)
			lives = 99;
		console->unk3AC[0] = lives;
		setTwoDigits(console->unk39C, console->unkE0, lives);
		console->startAppearMario(false);
	}
}

// fabricated
template <class Pane>
static inline void setCounterDigits(Pane** panes, JUTTexture** textures,
                                    int value)
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
                                     int value)
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
                                  int value)
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
static inline void emitCounterParticle(TBoundPane* pane)
{
	JUTRect bounds(pane->getPane()->mGlobalBounds);
	JGeometry::TVec3<f32> position;
	position.set(bounds.x1 + bounds.getWidth() * 0.5f,
	             bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
	gpEmitterManager4D2->createEmitter(position, 0x1FC, nullptr, nullptr);
}

// fabricated
static inline void setBlendDigit(TBlendPane* pane, JUTTexture** textures,
                                 int digit)
{
	pane->setPaneBlend(10, textures[digit], nullptr);
	JUTRect bounds(pane->getPane()->mGlobalBounds);
	JGeometry::TVec3<f32> position(bounds.x1 + bounds.getWidth() * 0.5f,
	                               bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
	gpEmitterManager4D2->createEmitter(position, 0x1FC, nullptr, nullptr);
}

// fabricated
static inline void updateRedCoinCounter(TGCConsole2* console)
{
	int redCoins = TFlagManager::smInstance->getFlag(0x60000);
	if (redCoins != (int)console->unk444) {
		if (redCoins < 0)
			redCoins = 0;
		if (redCoins > 8)
			redCoins = 8;

		setDigitPane(console->unk43C[0], console->unkE0, redCoins);
		if (!console->unk428->getPane()->isVisible())
			console->startAppearRedCoin();

		emitCounterParticle(console->unk43C[0]);
		console->unk444 = redCoins;
	}
}

// fabricated
static inline void updateCoinCounterAnimation(TGCConsole2* console)
{
	if ((s8)console->unk68 <= 0)
		return;

	if ((s8)console->unk68 == 1) {
		bool incrementing = true;
		int target        = (int)console->unk20;
		int display       = (int)console->unk6C;

		if (target >= display)
			++display;
		if (target < display) {
			--display;
			incrementing = false;
		}

		if (display > 999) {
			display        = 999;
			target         = 999;
			console->unk20 = target;
		}
		if (display < 0) {
			display        = 0;
			target         = 0;
			console->unk20 = target;
		}

		console->unk6C = display;
		setCounterDigits(console->unkD4, console->unkE0, display);

		if (incrementing) {
			if (display >= 100) {
				if (display % 100 == 0)
					emitCounterParticle(console->unkD4[0]);
				if (display % 10 == 0)
					emitCounterParticle(console->unkD4[1]);
				emitCounterParticle(console->unkD4[2]);
			} else {
				if (display % 10 == 0)
					emitCounterParticle(console->unkD4[0]);
				emitCounterParticle(console->unkD4[1]);
			}
		}
		++console->unk68;
	}

	bool done = true;
	if (!console->unk34) {
		for (int i = 0; i < 3; ++i)
			done = console->unkD4[i]->update() && done;
	}

	if (done) {
		if (console->unk20 == console->unk6C) {
			console->unk68 = 0;
			console->unk78 = 0x14;
		} else {
			console->unk68 = 1;
		}
	}
}

// fabricated
static inline void updateJetCounterAnimation(TGCConsole2* console)
{
	int flag   = -1;
	bool blend = false;

	if (console->unk404 == console->unk408) {
		flag  = 0x60001;
		blend = true;
	} else if (console->unk404 == console->unk40C) {
		flag = 0x60002;
	}

	if (flag < 0)
		return;

	int target = TFlagManager::smInstance->getFlag(flag);
	if (target > 99)
		target = 99;

	if (blend && target < (int)console->unk2C)
		return;

	bool done = console->unk414[0]->update();
	done      = console->unk414[1]->update() && done;
	if (!done)
		return;

	if (console->unk2C > 99)
		console->unk2C = 99;

	if (blend) {
		if ((int)console->unk2C == target)
			return;
	} else {
		if (target < 0)
			target = 0;
		if ((int)console->unk2C >= target)
			return;
	}

	++console->unk2C;

	int value = (int)console->unk2C;
	if (value < 0)
		value = 0;
	if (value > 99)
		value = 99;

	if (value >= 10) {
		console->unk414[0]->getPane()->show();
		int tens = value / 10;
		if (blend)
			((TBlendPane*)console->unk414[0])
			    ->setPaneBlend(12, console->unkE0[tens],
			                   console->unkE0[(tens + 9) % 10]);
		else if (value % 10 == 0)
			setDigitPane(console->unk414[0], console->unkE0, tens);
	} else {
		console->unk414[0]->getPane()->hide();
	}

	int ones = value % 10;
	if (blend)
		((TBlendPane*)console->unk414[1])
		    ->setPaneBlend(12, console->unkE0[ones],
		                   console->unkE0[(ones + 9) % 10]);
	else
		setDigitPane(console->unk414[1], console->unkE0, ones);
}

// fabricated
static inline void updateCounterState(TGCConsole2* console)
{
	TFlagManager* flags = TFlagManager::smInstance;

	int coins = flags->getFlag(0x40002);
	if (coins > 999)
		coins = 999;
	else if (coins < 0)
		coins = 0;

	if (coins != console->unk20) {
		if (!console->unk68)
			console->unk68 = 1;
		console->unk20 = coins;
	}

	bool waitForStarHud = gpMarioOriginal->mStatus == 0xC400201
	                      && gpMarDirector->mState != TMarDirector::STATE_UNK5
	                      && !console->unk50
	                      && !console->unk140->isInterpolatorAtZero();
	if (waitForStarHud) {
		++console->unk30;
		if (console->unk30 > 0xc8) {
			console->startAppearStar();
			console->startAppearMario(false);
			console->unk70 = 0xffff;
			console->unk59 = 0;
			console->unk30 = 0;
		}
	} else {
		console->unk30 = 0;
	}

	int blueTotal = flags->getFlag(0x40001);
	if ((int)console->unk168 != blueTotal) {
		++console->unk168;

		int spentBlueCoins = 0;
		for (int flag = 0x46; flag < 0x56; ++flag)
			if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
				++spentBlueCoins;

		for (int flag = 0x6C; flag <= 0x73; ++flag)
			if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
				++spentBlueCoins;
		int blueValue = console->unk168 - spentBlueCoins * 10;
		if (blueValue < 0)
			blueValue = 0;

		setBlueCoinDigits(console->unk154, console->unkE0, blueValue);
		if (console->unk160->getPane()->isVisible()) {
			emitCounterParticle(console->unk154[1]);
			if (blueValue % 10 == 0)
				emitCounterParticle(console->unk154[0]);
		} else {
			console->startAppearStar();
		}
		console->unk170 = blueValue;
		console->unk16C = 1;
	}

	if (console->unk16C != 0) {
		++console->unk16C;
		if (console->unk16C > 0x190)
			console->unk16C = 0;
	}

	int shines = flags->getFlag(0x40000);
	if (console->unk8A == 0 && (int)console->unk64 != shines)
		console->unk8A = 1;

	if (console->unk8A != 0) {
		if (console->unk8A > 0xFB) {
			int spentBlueCoins = 0;
			for (int flag = 0x46; flag < 0x56; ++flag)
				if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
					++spentBlueCoins;

			for (int flag = 0x6C; flag <= 0x73; ++flag)
				if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
					++spentBlueCoins;
			int target = blueTotal - spentBlueCoins * 10;
			if (console->unk170 != target) {
				--console->unk170;
				if (console->unk170 < 0) {
					console->unk170 = 0;
				} else if (SMSGetMSound()->gateCheck(0x4850)) {
					MSoundSESystem::MSoundSE::startSoundSystemSE(0x4850, 0,
					                                             nullptr, 0);
				}
				setBlueCoinDigits(console->unk154, console->unkE0,
				                  console->unk170);
			}
		}

		if (console->unk8A == 0xFC) {
			int value = console->unk64;

			if (value < 100) {
				console->unk134[2]->getPane()->hide();

				if (value % 10 == 0)
					setBlendDigit(console->unk134[0], console->unkE0,
					              value / 10);

				setBlendDigit(console->unk134[1], console->unkE0, value % 10);
			} else {
				int hundreds = value / 100;
				if (value % 100 == 0) {
					setBlendDigit(console->unk134[0], console->unkE0, hundreds);
					console->unk134[0]->getPane()->show();
				}

				value -= hundreds * 100;
				if (value % 10 == 0)
					setBlendDigit(console->unk134[1], console->unkE0,
					              value / 10);

				console->unk134[2]->getPane()->show();
				setBlendDigit(console->unk134[2], console->unkE0, value % 10);
			}
		} else if (console->unk8A == 0x106) {
			if (shines > (int)console->unk64) {
				++console->unk64;
				console->unk8A = 0xFB;
			}
		} else if (!console->unk34 && !console->unk35) {
			console->unk134[0]->update();
			console->unk134[1]->update();
			console->unk134[2]->update();
		}

		++console->unk8A;
	}
}

// fabricated
static inline void updateStarHudAutoHide(TGCConsole2* console)
{
	if (console->unk34)
		return;
	if (!console->unk140->isInterpolatorAtZero())
		return;
	if (console->unk60)
		return;
	if (gpMarDirector->mState == TMarDirector::STATE_UNK5
	    || gpMarDirector->mState == TMarDirector::STATE_UNK11)
		return;
	if (console->unk50 || console->unk16C != 0 || console->unk8A != 0)
		return;
	if (gpMarDirector->unk124 == 2)
		return;

	console->startDisappearStar();
	if (console->unk3A8->getPane()->isVisible() && !console->unk3B)
		console->startDisappearMario();
	console->unk5A = 0;
}

// fabricated
static inline void updateLifeMeterBlink(TGCConsole2* console)
{
	if (!console->unk1C4->getPane()->isVisible())
		return;

	f32 rate = 100.0f;
	if (console->unk1CC[0] == 3)
		rate = 80.0f;
	if (console->unk1CC[0] == 2)
		rate = 60.0f;
	if (console->unk1CC[0] == 1)
		rate = 35.0f;

	if ((f32)console->unk86 < rate * 2.0f) {
		if (console->unk1CC[0] <= 3 && console->unk1CC[0] != 0
		    && console->unk86 == (int)(1.5f * rate)
		    && gpMarDirector->mState == TMarDirector::STATE_UNK4
		    && gpMarDirector->unk124 == 0
		    && SMSGetMSound()->gateCheck(0x4800)) {
			MSoundSESystem::MSoundSE::startSoundSystemSE(0x4800, 0, nullptr, 0);
		}

		f32 diff = (f32)console->unk86 - rate;
		if (diff < 0.0f)
			diff = -diff;

		f32 mul     = console->unk1CC[0] > 3 ? 3.0f : 5.0f;
		int delta   = (int)(0.5f + diff / rate);
		int scaledY = (int)(mul * (f32)(-delta));

		for (int i = 0; i < 9; ++i) {
			if (console->unk17C[i * 2]->isVisible()) {
				console->unk17C[i * 2]->setBounds(
				    JUTRect(console->unk1D0[i].x1 - scaledY,
				            console->unk1D0[i].y1 - scaledY,
				            console->unk1D0[i].x2 + scaledY,
				            console->unk1D0[i].y2 + scaledY));
				console->unk17C[i * 2 + 1]->setBounds(
				    JUTRect(console->unk1D0[i].x1 - scaledY,
				            console->unk1D0[i].y1 - scaledY,
				            console->unk1D0[i].x2 + scaledY,
				            console->unk1D0[i].y2 + scaledY));
			}
		}

		++console->unk86;
	} else if ((f32)console->unk86 < rate * 2.0f) {
		++console->unk86;
	} else {
		console->unk86 = 0;
	}
}

// fabricated
static inline void updateDownBlendPaneState(TBlendPane*& pane, bool& isFinished)
{
	if (pane->update()) {
		bool paneFinished = false;
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
		if (pane->unk14.x1 == 0 && pane->unk14.y1 == 0)
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
static inline void updateShineAppearState(TGCConsole2* console)
{
	if (!console->unk34)
		return;

	bool done = console->processAppearStar(console->unk5C);
	done      = console->processDownCoin(console->unk5C) && done;
	if (done) {
		int shines = TFlagManager::smInstance->getFlag(0x40000);
		if ((int)console->unk24 != shines)
			console->unk24 = shines;
		console->unk34 = 0;
	}
	++console->unk5C;
}

// fabricated
static inline void updateJetAppearState(TGCConsole2* console)
{
	if (console->unk3D && console->processAppearJet(console->unk72++)) {
		console->unk3D = 0;
		console->unk72 = 0;
	}
}

// fabricated
static inline void updateRedCoinAppearState(TGCConsole2* console)
{
	if (console->unk3C && console->processAppearRed(console->unk74++)) {
		console->unk3C = 0;
		console->unk74 = 0;
	}
}

// fabricated
static inline void updateTimerAppearState(TGCConsole2* console)
{
	if (console->unk3E && console->processAppearTimer(console->unk76++)) {
		console->unk3E = 0;
		console->unk76 = 0;
	}
}

// fabricated
static inline void updateTelopState(TGCConsole2* console, u32 flags)
{
	if (console->unk3F && console->unk44C->update()) {
		console->unk44C->getPane()->hide();
		console->unk3F = 0;
	}

	if (console->unk42) {
		if (console->unk520->update()) {
			console->unk42 = 0;
			console->unk44 = 1;
			console->unk80 = 0;
		}
	} else if (console->unk43 && console->unk520->update()) {
		console->unk43 = 0;
		console->unk520->getPane()->hide();
	}

	if (console->unk44 && console->unk520->getPane()->isVisible()) {
		++console->unk80;
		if (console->processDrawTelop(flags)) {
			console->unk534 = console->unk524->getPane()->mBounds;
			console->unk568 = console->unk544.x2;
			console->unk534.add(console->mTelopTextWidth + console->unk534.x2,
			                    0);

			if (console->unk56D) {
				console->unk56D = 0;
			} else {
				console->unk44  = 0;
				console->unk55C = 0;
				if (!console->unk43 && console->unk520->getPane()->isVisible())
					console->startDisappearTelop();
			}
		}
	}

	u16 telopWait = console->unk56C ? console->unk562 : console->unk560;
	TMessageLoader* telopMessages = console->unk530;
	if (!console->unk44 && !console->unk42 && !console->unk43
	    && telopMessages->unk4 != nullptr
	    && console->unk55C >= (u32)((s16)telopWait * 120)
	    && gpMarioOriginal->mStatus == 0xC400201) {
		if (console->unk56C)
			console->unk56C = 0;
		if (console->unk570 != nullptr) {
			++console->unk558;
			if (console->unk570[console->unk558] == 0xffffffff)
				console->unk558 = 0;
		}
		console->checkChangeTelopArray();
		console->startAppearTelop(true);
	}

	if (gpMarDirector->mState != TMarDirector::STATE_UNK5
	    && gpMarDirector->unk124 == 0 && console->unk55C < 0xffffffff)
		++console->unk55C;
}

// fabricated
static inline void updateWaterTankState(TGCConsole2* console)
{
	if (console->unk45 && console->processAppearTank(console->unk7C++)) {
		console->unk45 = 0;
		console->unk46 = 1;
		console->unk7C = 0;
	}

	if (console->unk46) {
		if (gpMarDirector->mState == TMarDirector::STATE_UNK5)
			console->unk7C = 0;

		if (console->unk7C < 0x46)
			console->unk288->mAlpha = 0;
		else if (console->unk7C < 0x10e)
			console->unk288->mAlpha = 0xff;
		else
			console->unk7C = 0;

		++console->unk7C;
	}

	if (console->unk4B) {
		bool done = true;
		if (!console->unk2F8->update())
			done = false;
		if (!console->unk26C->update())
			done = false;
		if (!console->unk270->update())
			done = false;
		if (!console->unk274->update())
			done = false;
		if (done) {
			console->unk2F8->getPane()->hide();
			console->unk274->getPane()->hide();
			console->unk29C->getPane()->hide();
			console->unk4B = 0;
			console->unk46 = 0;
		}
	}
}

// fabricated
static inline void updateCoinAppearState(TGCConsole2* console)
{
	if (console->unk4F && console->processAppearCoin(console->unk88++))
		console->unk4F = 0;
}

// fabricated
static inline void updateMarioAppearState(TGCConsole2* console)
{
	if (console->unk3A && console->processAppearMario(console->unk70++)) {
		if (console->unk3AC[1]) {
			if (console->unk70 == 0xc8) {
				int lives = TFlagManager::smInstance->getFlag(0x20001);
				if (lives > 99)
					lives = 99;
				setTwoDigits(console->unk39C, console->unkE0, lives);
				TFlagManager::smInstance->setBool(false, 0x30002);
				console->endCameraDemo();
				console->unk3A = 0;
			}
		} else {
			console->unk3AC[1] = 0;
			console->unk70     = 0;
		}
	}

	if (!console->unk3A && !console->unk3B
	    && console->unk3A8->getPane()->isVisible()
	    && gpMarioOriginal->mStatus != 0xC400201
	    && gpMarDirector->mState != TMarDirector::STATE_UNK5) {
		if (++console->unk70 > 0x190)
			console->startDisappearMario();
	}

	if (console->unk3B && console->unk3A8->update()) {
		console->unk3B = 0;
		console->unk3A8->getPane()->hide();
	}
}

// fabricated
static inline bool updateBalloonAppearState(TGCConsole2* console)
{
	if (!console->processAppearBalloon())
		return false;

	console->unk10 = 2;
	memset(console->unk3B8->getStringPtr(), 0, 0x400);
	memset(console->unk3B4->getStringPtr(), 0, 0x400);
	console->unk3B8->show();
	return true;
}

// fabricated
static inline bool updateBalloonDisappearState(TGCConsole2* console)
{
	if (!console->processDisappearBalloon())
		return false;

	u32 nextMessage = console->unk3F4;
	console->unk10  = 0;
	console->unk3F0 = 0;
	console->unk3B0->hide();
	console->unk3B0->resize(0, console->unk3BC.getHeight());
	JUTRect contents(console->unk3B0->getContentsBounds());
	console->unk3B0->add(0, -contents.getHeight());

	if (nextMessage != 0xffffffff)
		console->startAppearBalloon(nextMessage, true);
	console->unk3F4 = 0xffffffff;
	return true;
}

// fabricated
static inline void drawWaterOrJuice(TGCConsole2* console, J2DOrthoGraph& graph)
{
	if (isMountedYoshi(gpMarioOriginal)) {
		switch (gpModelWaterManager->unk5D5F) {
		case 1:
			console->drawJuice(graph, console->unkA2);
			break;
		case 2:
			console->drawJuice(graph, console->unkA6);
			break;
		case 3:
			console->drawJuice(graph, console->unkAA);
			break;
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

// TODO: three dead .data objects the original TU still emits, recovered from
// the ROM. They are never read, so which function's statics they were is
// unknown; they are placed here only to keep the .data layout right.
static f32 scUnusedScale1[] = { 1.0f, 1.0f, 1.0f };
static f32 scUnusedScale2[] = { 1.0f, 1.0f, 1.0f };
static int scUnusedTable[]  = { 0, 2, 1, 3 };

static u32 scDolpicNewsDolpic0[]   = { 0x000E0000, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic1[]   = { 0x000E0001, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_1[] = { 0x000E0009, 0x000E0011, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_2[] = { 0x000E0012, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_3[] = { 0x000E0013, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic5_4[] = { 0x000E0012, 0x000E0013, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic6[]   = { 0x000E0002, 0x000E0004, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic7[]   = { 0x000E0005, 0x000E0006, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_1[] = { 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_2[] = { 0x000E0007, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_3[] = { 0x000E0008, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_4[] = { 0x000E000E, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_5[] = { 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_6[]
    = { 0x000E000E, 0x000E000F, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic8_7[] = { 0x000E0010, 0x000E0003, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic9[]   = { 0x000E000A, 0x000E000B, 0xFFFFFFFF };
static u32 scDolpicNewsDolpic10[]  = { 0x000E000C, 0x000E000D, 0xFFFFFFFF };

// rogue include
#include <M3DUtil/InfectiousStrings.hpp>

TGCConsole2::TGCConsole2(const char* name)
    : JDrama::TViewObj(name)
    , unk10(0)
    , unk14(0)
    , unk18(0)
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
    , unkB0(nullptr)
    , unkB4(0)
    , unkB6(0)
    , unkB8(0)
    , unkBC(0)
    , unkC0(0)
    , unkC4(0)
    , unkC8(nullptr)
    , unkCC(nullptr)
    , unkD0(nullptr)
    , unk108(nullptr)
    , unk128(nullptr)
    , unk12C(nullptr)
    , unk130(nullptr)
    , unk140(nullptr)
    , unk148(nullptr)
    , unk14C(nullptr)
    , unk150(nullptr)
    , unk160(nullptr)
    , unk174(nullptr)
    , unk178(nullptr)
    , unk1C4(nullptr)
    , unk1C8(0xFF6A)
    , unk1CA(0xB4)
    , unk260(nullptr)
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
    , unk38C(nullptr)
    , unk390(nullptr)
    , unk394(nullptr)
    , unk398(nullptr)
    , unk3CC(0)
    , unk3D0(nullptr)
    , unk3D4(nullptr)
    , unk3D8(nullptr)
    , unk3DC(nullptr)
    , unk3E0(0)
    , unk3E4(0)
    , unk3E8(100)
    , unk3EC(0.7f)
    , unk3F0(0)
    , unk3F4(0xffffffff)
    , unk426(0)
    , unk444(0)
    , unk448(0)
    , unk510(false)
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
		unkD4[i] = nullptr;

	for (int i = 0; i < 10; ++i)
		unkE0[i] = nullptr;

	for (int i = 0; i < 3; ++i)
		unk134[i] = nullptr;

	unk90 = new THelpActor*[32];
}

void TGCConsole2::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("game_6");
	unkB0            = new J2DSetScreen("standard_1.blo", arch);
	unkB0->setCullBack(GX_CULL_BACK);

	unkC8  = new TBoundPane(unkB0, 'c_ba');
	unkCC  = new TBoundPane(unkB0, 'c_ic');
	unkD0  = new TBoundPane(unkB0, '\0c_x');
	unk108 = new TExPane(unkB0, '\0c_0');

	for (int i = 0; i < 3; ++i)
		unkD4[i] = new TBlendPane(unkB0, 'c_n1' + i);

	for (int i = 0; i < 10; ++i) {
		char acStack_ac[40];
		snprintf(acStack_ac, 40, "/game_6/timg/coin_number_%d.bti", i);
		unkE0[i] = new JUTTexture((const ResTIMG*)JKRGetResource(acStack_ac));
	}

	unk128 = new TBoundPane(unkB0, 's_ba');
	unk12C = new TBoundPane(unkB0, 's_ic');
	unk130 = new TBoundPane(unkB0, '\0s_x');
	unk140 = new TExPane(unkB0, '\0s_0');

	for (int i = 0; i < 3; ++i)
		unk134[i] = new TBlendPane(unkB0, 's_n1' + i);

	unk148 = new TBoundPane(unkB0, 'd_ba');
	unk14C = new TBoundPane(unkB0, 'd_ic');
	unk150 = new TBoundPane(unkB0, '\0d_x');

	for (int i = 0; i < 3; ++i)
		unk154[i] = new TBoundPane(unkB0, 'd_n1' + i);

	unk160 = new TExPane(unkB0, '\0d_0');
	unk174 = new TBoundPane(unkB0, 'l_ba');
	unk178 = new TBoundPane(unkB0, 'l_tx');
	unk1C4 = new TBoundPane(unkB0, '\0l_0');

	for (int i = 0; i < 9; ++i) {
		unk17C[i * 2]     = unkB0->search('lm01' + (i << 8));
		unk17C[i * 2 + 1] = unkB0->search('lm02' + (i << 8));
		unk1D0[i]         = unk17C[i * 2]->getBounds();
	}

	unk260 = new TBoundPane(unkB0, 'lm_0');
	unk26C = new TBoundPane(unkB0, 'w_ba');
	unk270 = new TBoundPane(unkB0, 'w_tx');

	for (int i = 0; i < 4; ++i) {
		unk278[i] = new TBoundPane(unkB0, 'nz01' + i);
		unk28C[i] = unkB0->search('xb01' + i);
	}

	unk274 = unk278[0];
	unk288 = unk28C[0];

	unk274->getPane()->setBasePosition(J2DBasePosition_5);
	unk288->show();

	unk29C = new TBoundPane(unkB0, 'w_t0');
	unk2F8 = new TExPane(unkB0, '\0w_0');
	for (int i = 0; i < 3; ++i) {
		unk2A0[i] = (J2DPicture*)unkB0->search('w_t1' + i);

		unk2A0[i]->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
		unk2A0[i]->setBlendKonstAlpha(0.0f, 0.0f, 0.0f, 0.0f);

		unk2A0[i]->hide();

		if (i != 0) {
			unk2AC[i] = (J2DPicture*)unkB0->search('w_m1' + i);

			unk2AC[i]->setBlendKonstColor(0.0f, 0.0f, 0.0f, 0.0f);
			unk2AC[i]->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);

			unk2AC[i]->hide();
		}
	}

	for (int i = 0; i < 4; ++i)
		unk314[i] = unkB0->search(i == 0 ? 'j_t0' : 'j_t1');

	unk324 = unkB0->search('j_ic');
	unk328 = (J2DPicture*)unkB0->search('j_rq');
	unk32C = (J2DPicture*)unkB0->search('j_ms');

	for (int i = 0; i < 22; ++i) {
		if (i < 9)
			unk334[i] = unkB0->search('j_01' + i);
		else
			unk334[i] = unkB0->search('j_0X' + i);
	}

	unk38C = new TBoundPane(unkB0, 'm_ba');
	unk394 = new TBoundPane(unkB0, 'm_ic');
	unk390 = new TBoundPane(unkB0, 'm_tx');
	unk398 = new TBoundPane(unkB0, '\0m_x');
	unk3A8 = new TExPane(unkB0, '\0m_0');

	for (int i = 0; i < 3; ++i)
		unk39C[i] = new TBoundPane(unkB0, 'm_n1' + i);

	unk3B0 = (J2DWindow*)unkB0->search('he_w');
	unk3B8 = (J2DTextBox*)unkB0->search('he_2');
	unk3B4 = (J2DTextBox*)unkB0->search('he_1');

	SMSMakeTextBuffer(unk3B8, 0x401);
	SMSMakeTextBuffer(unk3B4, 0x401);

	unk3B8->setFont(gpSystemFont);
	unk3B4->setFont(gpSystemFont);

	unk3B4->show();
	unk3D0 = new TMessageLoader("/common/2d/balloon.bmg");

	unk3FC = new TExPane(unkB0, '\0b_0');
	unk400 = new TBoundPane(unkB0, 'b_ba');
	unk408 = new TBoundPane(unkB0, 'b_ic');
	unk40C = new TBoundPane(unkB0, 'moic');
	unk410 = new TBoundPane(unkB0, 'b_sl');

	for (int i = 0; i < 4; ++i)
		unk414[i] = new TBlendPane(unkB0, 'b_n1' + i);

	((J2DPicture*)unk414[2]->getPane())
	    ->changeTexture(unkE0[8]->getTexInfo(), 0);

	unk428 = new TExPane(unkB0, '\0r_0');
	unk42C = new TBoundPane(unkB0, 'r_ba');
	unk430 = new TBoundPane(unkB0, 'r_ic');
	unk434 = new TBoundPane(unkB0, '\0r_x');
	unk438 = new TBoundPane(unkB0, 'r_sl');

	for (int i = 0; i < 2; ++i)
		unk43C[i] = new TBoundPane(unkB0, 'r_n1' + i);

	unk44C = new TExPane(unkB0, '\0t_0');
	unk450 = new TBoundPane(unkB0, 't_ba');
	unk454 = new TBoundPane(unkB0, 't_tx');

	for (int i = 0; i < 9; ++i)
		unk458[i] = new TBoundPane(unkB0, 't_n1' + i);

	unk458[9] = new TBoundPane(unkB0, 't_n0');

	// TODO: is this the right cast?
	unk508 = ((J2DPicture*)unk458[6]->getPane())->getWhite();
	unk50C = ((J2DPicture*)unk458[7]->getPane())->getWhite();

	for (int i = 0; i < 3; ++i)
		unk480[i] = new TBoundPane(unkB0, 't_c1' + i);

	for (int i = 0; i < 2; ++i)
		unk500[i] = unkB0->search('\0t_1' + i);

	unk520 = new TExPane(unkB0, 'te_0');
	unk524 = new TExPane(unkB0, 'te_w');

	unk528 = (J2DTextBox*)unkB0->search('tet2');
	unk52C = (J2DTextBox*)unkB0->search('tet1');

	unk530 = new TMessageLoader("/common/2d/infomess.bmg");
	unk55C = 0;
	SMSMakeTextBuffer(unk528, 0x401);
	SMSMakeTextBuffer(unk52C, 0x401);
}

void TGCConsole2::loadAfter()
{
	JDrama::TNameRef::loadAfter();

	unk94 = JDrama::TNameRefGen::search<TConsoleStr>("コンソール文字");

	JUTRect waterBounds(unk2F8->getPane()->mBounds);

	unk2A0[0]->add(waterBounds.x1, waterBounds.y1);
	unk270->getPane()->add(waterBounds.x1, waterBounds.y1);
	syncPaneBounds(unk270);
	unk26C->getPane()->add(waterBounds.x1, waterBounds.y1);
	syncPaneBounds(unk26C);
	unk328->add(waterBounds.x1, waterBounds.y1);

	for (int i = 0; i < 3; ++i) {
		unk2BC[i] = unk2A0[i]->mBounds;
		unk2BC[i].add(waterBounds.x1, waterBounds.y1);
	}

	unk2A0[0]->show();
	detachPaneFromParent(unk2A0[0]);
	detachBoundPaneFromParent(unk270);
	for (int i = 0; i < 4; ++i) {
		unk278[i]->getPane()->add(waterBounds.x1, waterBounds.y1);
		syncPaneBounds(unk278[i]);
		detachBoundPaneFromParent(unk278[i]);
	}
	detachBoundPaneFromParent(unk26C);
	detachPaneFromParent(unk328);
	unk288->hide();

	unk2EC[1] = JUtility::TColor(0x64DCFF00);
	unk2EC[2] = JUtility::TColor(0x00B4F000);

	s16 health = gpMarioOriginal->mHealth;
	if (health < 0)
		health = 0;

	unk1C4->getPane()->hide();
	for (int i = 0; i < 9; ++i) {
		if (i < health + 1)
			unk17C[i * 2]->show();
		else
			unk17C[i * 2]->hide();
	}
	unk1CC[0] = health;
	unk1C     = health;

	unk26A = unk140->getPane()->mBounds.y1 - unk108->getPane()->mBounds.y1;

	initHiddenPaneAbove(unk140);
	unk140->getPane()->hide();
	initHiddenPaneAbove(unk160);
	unk160->getPane()->hide();
	initHiddenPaneOffset(unk108, unk26A);
	unk108->getPane()->hide();
	initHiddenPaneAbove(unk3A8);
	unk3A8->getPane()->hide();

	unk168 = TFlagManager::smInstance->getFlag(0x40001);

	int spentBlueCoins = 0;
	for (int flag = 0x46; flag < 0x56; ++flag)
		if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
			++spentBlueCoins;

	for (int flag = 0x6C; flag <= 0x73; ++flag)
		if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
			++spentBlueCoins;

	int blueCoinValue = unk168 - spentBlueCoins * 10;
	if (blueCoinValue < 0)
		blueCoinValue = 0;
	setBlueCoinDigits(unk154, unkE0, blueCoinValue);
	unk170 = blueCoinValue;

	unk20 = TFlagManager::smInstance->getFlag(0x40002);
	if (unk20 > 999)
		unk20 = 999;
	else if (unk20 < 0)
		unk20 = 0;
	unk6C = unk20;
	setCounterDigits(unkD4, unkE0, unk20);

	unk24 = TFlagManager::smInstance->getFlag(0x40000);
	if (unk24 > 999)
		unk24 = 999;
	else if (unk24 < 0)
		unk24 = 0;
	unk64 = unk24;
	setShineDigits(unk134, unkE0, unk24);

	int lives = TFlagManager::smInstance->getFlag(0x20001);
	if (lives > 99)
		lives = 99;
	unk3AC[0] = lives;
	setTwoDigits(unk39C, unkE0, lives);

	unk39 = 1;

	unk2F8->getPane()->hide();
	unk3A8->getPane()->hide();
	unk270->getPane()->hide();
	unk274->getPane()->hide();
	unk29C->getPane()->hide();
	unk520->getPane()->hide();
	unk524->getPane()->show();
	unk528->hide();
	unk528->setFont(gpSystemFont);
	JUTRect bounds(unk528->mBounds);
	unk528->resize(gpSystemFont->getWidth() << 10, bounds.getHeight());

	unk52C->hide();
	unk52C->setFont(gpSystemFont);
	unk52C->resize(gpSystemFont->getWidth() << 10, bounds.getHeight());

	JUTRect telopBounds(unk524->getPane()->mBounds);
	JUTRect telopPaneBounds(unk520->getPane()->mBounds);
	telopBounds.add(telopPaneBounds.x1, telopPaneBounds.y1 - 3);
	unk544.set(telopBounds.x1 + 8, telopBounds.y1 + 8, telopBounds.x2 - 8,
	           telopBounds.y2 - 8);

	unk534 = unk528->mBounds;
	unk534.add(unk544.x2, unk544.y1 + unk528->mBounds.getHeight());

	unk544.resize(unk544.getWidth() * (16.0f / 15.0f), unk544.getHeight());
	unk544.add(0, -16);
	unk568 = unk544.x2;

	unk3BC = unk3B0->mBounds;
	unk3CC = 30;

	unk3D8 = new JSUMemoryOutputStream(unk3B4->getStringPtr(), 0x400);
	unk3DC = new JSUMemoryOutputStream(unk3B8->getStringPtr(), 0x400);
	unk3D4 = new JSUMemoryInputStream(nullptr, 0x400);

	unk48C = unk450->getPane()->mBounds;
	unk49C = unk458[0]->getPane()->mBounds;
	unk4AC = unk480[0]->getPane()->mBounds;

	for (int i = 0; i < 6; ++i) {
		JUTRect bounds(unk458[i]->getPane()->mBounds);
		unk4BC[i].set(bounds.x1, bounds.y1);
	}

	for (int i = 0; i < 2; ++i) {
		JUTRect bounds(unk480[i]->getPane()->mBounds);
		unk4EC[i].set(bounds.x1, bounds.y1);
	}

	unk7A[0] = gpModelWaterManager->unk5D5F;

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

	TNozzleBase* nozzle = gpMarioOriginal->mWaterGun->getCurrentNozzle();
	unk28               = *(u32*)((u8*)nozzle + 0xCC);

	unkBC = JDrama::TNameRefGen::search<TBathtub>("バスタブ");
	unkC0 = JDrama::TNameRefGen::search<TBossEel>("めおとウナギ");
	unkC4 = JDrama::TNameRefGen::search<JDrama::TNameRef>("ピーチ姫");
}

void TGCConsole2::entryHelpActor(THelpActor* param_1)
{
	if (unk8C < 32) {
		unk90[unk8C] = param_1;

		JDrama::TNameRefGen::search<TIdxGroupObj>("マップグループ")
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
		unk1C4->getPane()->hide();
		unk274->getPane()->hide();
		unk2F8->getPane()->hide();
		unk270->getPane()->hide();
		unk26C->getPane()->hide();
		unk3A8->getPane()->hide();
		unk44C->getPane()->hide();
		unk3FC->getPane()->hide();
		unk428->getPane()->hide();
		unk46 = 0;
		unk59 = 0;
		unk5A = 0;
		return;
	}

	if (unk50 || !TFlagManager::smInstance->getBool(0x30002) && unk39)
		return;

	unk50 = 1;

	startDisappearTelop();
	startLifeMeterDisappear(this, 120);

	unk18 = 10;
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
		startDisappearTank();
	}

	startDisappearMario();
	startDownLeftBot();

	if (TFlagManager::smInstance->getBool(0x30002)) {
		unk108->getPane()->hide();
		startAppearMario(true);
	} else if (gpMarDirector->checkUnk4CFlag(0x8000)) {
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
	    && !TFlagManager::smInstance->getBool(0x30002)) {
		unk45 = 1;
		unk59 = 1;
		unk7C = 0;
		unk2F8->getPane()->show();
		unk2F8->setPaneOffset(unk98, 0, 0, 0, 465 - unk2F8->getInitialY1());
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

	if (!gpMarDirector->checkUnk4CFlag(0x8000)
	    && !unk108->getPane()->isVisible())
		startAppearCoin();
}

void TGCConsole2::startAppearTank()
{
	if (unk45 || TFlagManager::smInstance->getBool(0x30002)) {
		return;
	}

	// TODO: needs register swapping
	unk45 = 1;
	unk59 = 1;
	unk7C = 0;

	unk2F8->getPane()->show();
	unk2F8->setPaneOffset(unk98, 0, 0, 0, 465 - unk2F8->getInitialY1());

	unk26C->setPanePosition(50, JUTPoint(0, 100), JUTPoint(0, -30),
	                        JUTPoint(0, -30));

	unk274->getPane()->show();
	unk29C->getPane()->show();
}

void TGCConsole2::startDisappearTank()
{
	unk4B = 1;
	unk5A = 1;

	int offset = 465 - unk2F8->getInitialY1();
	offset += 60;
	unk2F8->updatePaneOffset(40, 0, offset);

	JUTPoint start(0, 0);
	JUTPoint mid(0, offset >> 1);
	JUTPoint end(0, offset);
	unk48 = 0;
	unk274->setPanePosition(40, start, mid, end);
	unk270->setPanePosition(40, start, mid, end);
	unk26C->setPanePosition(40, start, mid, end);
}

void TGCConsole2::startAppearCoin()
{
	if (unk108->getPane()->isVisible()) {
		return;
	}

	unk4F = 1;
	unk59 = 1;
	unk88 = 0;

	unk108->getPane()->show();
	unk108->setPaneOffset(unk98, 0, unk26A, 0, -(unk108->getInitialY2() + 1));

	unkC8->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);

	unkCC->getPane()->hide();
	unkD0->getPane()->hide();
	for (int i = 0; i < 3; i++) {
		unkD4[i]->getPane()->hide();
	}

	unk124->clearStatus(JPABaseEmitter::STATUS_STOP_EMIT);
}

void TGCConsole2::startDisappearCoin()
{
	unk4D = true;
	unk5A = true;

	if (unk140->isInterpolatorAtZero())
		unk140->updatePaneOffset(40, 0,
		                         getOffsetForAboveScreen(unk140)
		                             - unk128->getPane()->getHeight());

	unk108->updatePaneOffset(
	    40, 0, getOffsetForAboveScreen(unk108) - unkC8->getPane()->getHeight());

	unk124->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
}

void TGCConsole2::startInsertLife(int param_1)
{
	if (param_1 == 0) {
		for (int i = 1; i < 9; ++i) {
			((J2DPicture*)unk17C[i * 2])->mWhite = 0xFFFFFFFF;
			((J2DPicture*)unk17C[i * 2])->mBlack = 0;
			if (gpMarioOriginal->mHealth + 1 > i)
				unk17C[i * 2]->show();
			else
				unk17C[i * 2]->hide();
		}
		((J2DPicture*)unk178->getPane())->mWhite = 0xFFFFFFFF;
		((J2DPicture*)unk178->getPane())->mBlack = 0;
		((J2DPicture*)unk174->getPane())->mWhite = 0xFF4C00C8;
		((J2DPicture*)unk174->getPane())->mBlack = 0xFF4C0000;
		updateLifeMeterColors(this, false);
	} else if (param_1 == 1) {
		for (int i = 1; i < 9; ++i) {
			((J2DPicture*)unk17C[i * 2])->mWhite = 0x00FFFFFF;
			((J2DPicture*)unk17C[i * 2])->mBlack = 0x003CFF00;
			if ((s16)gpMarioOriginal->mAir + 1 > i)
				unk17C[i * 2]->show();
			else
				unk17C[i * 2]->hide();
		}
		((J2DPicture*)unk174->getPane())->mWhite = 0x0000FF78;
		((J2DPicture*)unk174->getPane())->mBlack = 0x0000FF00;
		updateLifeMeterColors(this, true);
	}

	unk51 = 1;
	unk84 = 0;

	int offset = -(unk1C4->unk4.y2 + 1);
	unk1C4->getPane()->show();
	unk1C4->setPanePosition(unk98, JUTPoint(0, offset),
	                        JUTPoint(0, offset >> 1), JUTPoint(0, 0));
	unk174->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);
	unk178->getPane()->hide();
	unk260->getPane()->hide();
}

void TGCConsole2::resetLife(int param_1)
{
	for (int i = 1; i < 9; i++) {
		if (i < param_1 + 1) {
			unk17C[2 * i]->show();
		} else {
			unk17C[2 * i]->hide();
		}

		// TODO: This feels like a fakematch, is setBounds correct?

		JUTRect local_38(unk1D0[i].x1, unk1D0[i].y1, unk1D0[i].x2,
		                 unk1D0[i].y2);
		unk17C[2 * i]->setBounds(local_38);

		JUTRect local_48(unk1D0[i].x1, unk1D0[i].y1, unk1D0[i].x2,
		                 unk1D0[i].y2);
		unk17C[2 * i + 1]->setBounds(local_48);
	}
}

bool TGCConsole2::startAppearLife(int param_1)
{
	if (unk38 || unk50 || gpMarioOriginal->getHealth() == 0)
		return false;

	if (param_1 == 0) {
		for (int i = 0; i < 9; ++i) {
			((J2DPicture*)unk17C[i * 2])->mWhite = 0xFFFFFFFF;
			((J2DPicture*)unk17C[i * 2])->mBlack = 0;
			if (gpMarioOriginal->mHealth + 1 > i)
				unk17C[i * 2]->show();
			else
				unk17C[i * 2]->hide();
		}
		((J2DPicture*)unk178->getPane())->mWhite = 0xFFFFFFFF;
		((J2DPicture*)unk178->getPane())->mBlack = 0;
		((J2DPicture*)unk174->getPane())->mWhite = 0xFF4C00C8;
		((J2DPicture*)unk174->getPane())->mBlack = 0xFF4C0000;
		updateLifeMeterColors(this, false);
	} else if (param_1 == 1) {
		for (int i = 0; i < 9; ++i) {
			((J2DPicture*)unk17C[i * 2])->mWhite = 0x00FFFFFF;
			((J2DPicture*)unk17C[i * 2])->mBlack = 0x003CFF00;
			if ((s16)gpMarioOriginal->mAir + 1 > i)
				unk17C[i * 2]->show();
			else
				unk17C[i * 2]->hide();
		}
		((J2DPicture*)unk178->getPane())->mWhite = 0x00FFFFFF;
		((J2DPicture*)unk178->getPane())->mBlack = 0x003CFF00;
		((J2DPicture*)unk174->getPane())->mWhite = 0x0000FF78;
		((J2DPicture*)unk174->getPane())->mBlack = 0x0000FF00;
		updateLifeMeterColors(this, true);
	}

	unk38 = 1;
	unk84 = 0;
	unk1C4->getPane()->add(unk1C8, unk1CA);
	return true;
}

void TGCConsole2::startDisappearLife(int) { }

void TGCConsole2::startDownLeftBot()
{
	if (unk41) {
		return;
	}

	unk41 = 1;
	unk40 = 0;
	unk5A = 1;

	if (unk44C->getPane()->isVisible() && unk44C->isInterpolatorAtZero()) {
		unk44C->updatePaneOffset(20, 0, getOffsetForBelowScreen(unk44C) + 60);
		unk51C = 1;
	}

	if (unk428->getPane()->isVisible()) {
		unk428->updatePaneOffset(20, 0, getOffsetForBelowScreen(unk428) + 60);
		unk448 = 1;
	}

	if (unk3FC->getPane()->isVisible()) {
		unk3FC->updatePaneOffset(20, 0, getOffsetForBelowScreen(unk3FC) + 60);
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
	unk520->getPane()->show();

	unk520->setPaneOffset(80, 0, 0, 0, 465 - unk520->getInitialY1());

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
	if (unk43 || !unk520->getPane()->isVisible()) {
		return;
	}

	unk43 = 1;
	unk5A = 1;

	unk520->updatePaneOffset(80, 0, 465 - unk520->getInitialY1());
}

void TGCConsole2::startDisappearTimer()
{
	unk44C->updatePaneOffset(40, 0, getOffsetForBelowScreen(unk44C) + 60);
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
	unk44C->setPaneOffset(40, 0, 0, 0, 465 - unk44C->getInitialY1());

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
		unk3FC->setPaneOffset(80, 0, 0, 0, 465 - unk3FC->getInitialY1());
	else
		unk3FC->setPaneOffset(80, 0, -73, 0, 465 - unk3FC->getInitialY1());
}

void TGCConsole2::startAppearRedCoin()
{
	unk3C = 1;
	unk59 = 1;

	unk428->getPane()->show();
	if (unk44C->getPane()->isVisible())
		unk428->setPaneOffset(40, 0, -73, 0, 465 - unk428->getInitialY1());
	else
		unk428->setPaneOffset(40, 0, 0, 0, 465 - unk428->getInitialY1());

	unk42C->getPane()->show();
	unk42C->setPanePosition(50, cUpTopPoint, cUpMidPoint, cUpMidPoint);

	unk430->getPane()->hide();
	unk438->getPane()->hide();
	unk434->getPane()->hide();
	unk74 = 0;

	for (int i = 0; i < 2; ++i)
		unk43C[i]->getPane()->hide();
}

void TGCConsole2::pauseIn()
{
	startDisappearBalloon(unk3E0, true);
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

// TODO: figure out inlining without pragmas
#pragma dont_inline on
bool TGCConsole2::startDisappearBalloon(u32 param_1, bool param_2)
{
	if (!param_2 && unk3F4 == 0xffffffff && (param_1 != unk3E0 || unk3E4 != 0))
		return false;

	unk3B8->hide();
	unk48 = 0;
	unk10 = 4;
	return true;
}
#pragma dont_inline off

bool TGCConsole2::startAppearBalloon(u32 messageID, bool autoClose)
{
	TMessageLoader::EntryInfo* entry
	    = unk3D0->getMessageEntry(messageID & 0xffff);

	if (entry->unk4 <= unk3F0)
		return false;

	if (unk10 != 0) {
		if (unk3F4 != 0xffffffff)
			return false;

		unk3F4 = messageID;
		// The ROM compares unk3E0 with itself here, so the middle term is
		// always true. Probably a copy-paste slip in the original source.
		if (unk3F4 != 0xffffffff || (unk3E0 == unk3E0 && unk3E4 == 0)) {
			unk3B8->hide();
			unk48 = 0;
			unk10 = 4;
		}
		return true;
	}

	if (gpMarDirector->mState == TMarDirector::STATE_UNK5 || !unk46)
		return false;

	unk3F0         = entry->unk4;
	unk3B0->mAlpha = 0;
	unk3B0->show();

	// TODO: the ROM copies the contents rect twice here, as if
	// J2DWindow::getContentsBounds() returned a JUTRect by value.
	JUTRect contents(unk3B0->getContentsBounds());
	unk3B0->resize(unk3BC.getWidth(),
	               unk3BC.getHeight() - contents.getHeight());
	unk3B0->add(0, contents.getHeight());

	((JSUMemoryOutputStream*)unk3D8)->setBuffer(unk3B4->getStringPtr(), 0x400);
	((JSUMemoryOutputStream*)unk3DC)->setBuffer(unk3B8->getStringPtr(), 0x400);

	const u8* messageText = unk3D0->getMessageData() + entry->unk0;
	((JSUMemoryInputStream*)unk3D4)->setBuffer(messageText, 0x400);

	unk3E0 = messageID;
	unk3F8 = autoClose;
	unk3E4 = (s32)(strlen((const char*)messageText) * unk3EC + unk3E8);

	if (unk3E0 == 0x000E002F)
		unk3E4 = 0x96;

	if (unk3E4 <= 0)
		unk3E4 = 1;

	unk48 = 1;
	unk14 = 0;
	unk10 = 1;

	s32 soundID = scNozzleSoundList[(u8)entry->unk8[0]];
	if (soundID != -1 && SMSGetMSound()->gateCheck(soundID))
		MSoundSESystem::MSoundSE::startSoundSystemSE(soundID, 0, nullptr, 0);

	return true;
}

void TGCConsole2::startDisappearStar()
{
	unk140->updatePaneOffset(40, 0, getOffsetForAboveScreen(unk140) + unk26A);
	unk160->updatePaneOffset(40, 0, -(unk160->getInitialY2() + 1));
	unk108->updatePaneOffset(40, 0, unk26A);

	unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
	unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);

	unk35 = 1;
	unk5A = 1;
}

void TGCConsole2::startAppearStar()
{
	if (unk34 || unk140->isInterpolatorAtZero())
		return;

	unk59 = 1;

	if (unk35) {
		unk140->getPane()->hide();
		unk160->getPane()->hide();
		unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
		unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
		unk35 = 0;
	}

	unk140->setPaneOffset(40, 0, 0, 0, -(unk140->getInitialY2() + 1));
	unk140->getPane()->show();
	unk128->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);

	unk12C->getPane()->hide();
	unk130->getPane()->hide();
	for (int i = 0; i < 3; ++i)
		unk134[i]->getPane()->hide();

	unk160->setPaneOffset(40, 0, 0, 0, -(unk160->getInitialY2() + 1));
	unk160->getPane()->show();
	unk148->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);

	unk14C->getPane()->hide();
	unk150->getPane()->hide();
	for (int i = 0; i < 3; ++i)
		unk154[i]->getPane()->hide();

	if (!unk108->getPane()->isVisible())
		unk108->getPane()->show();

	unk108->updatePaneOffset(40, 0, 0);
	unkC8->setPanePosition(50, JUTPoint(0, 0), cCoinMidPoint, cCoinMidPoint);

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

	JUTTexture* backgroundTexture;
	if (((J2DPicture*)unk26C->getPane())->mTextureNum > 0)
		backgroundTexture = ((J2DPicture*)unk26C->getPane())->mTextures[0];
	else
		backgroundTexture = nullptr;
	backgroundTexture->load(GX_TEXMAP0);
	GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0,
	                  GX_FALSE, GX_PTIDENTITY);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

	JUTRect bounds(((J2DPicture*)unk26C->getPane())->mBounds);
	TWaterGun* waterGun = gpMarioOriginal->mWaterGun;
	GXSetTevColor(GX_TEVREG0, JUtility::TColor(0x0000ff78));
	GXSetTevColor(GX_TEVREG1, JUtility::TColor(0x0000ff00));

	if (waterGun->isPressureOn()) {
		f32 pressure    = waterGun->getPressure();
		f32 pressureMax = waterGun->getPressureMax();
		f32 hiddenRatio = 1.0f - pressure / pressureMax;
		int fillTop     = bounds.y1 + (int)(hiddenRatio * bounds.getHeight());

		drawGaugeQuadF32(bounds, bounds.y1, fillTop, 0.0f, hiddenRatio);

		if (!unk50 && pressure != 0.0f && !unk48) {
			unk14 = 1;
			unk48 = 1;
		}

		if (pressure == pressureMax) {
			if (unk49)
				unk49 = 0;

			if (unk30C >= 25)
				unk30C = 0;

			u32 color = getPressureFlashColor(unk30C);
			GXSetTevColor(GX_TEVREG0, JUtility::TColor(color + 0xc8));
			GXSetTevColor(GX_TEVREG1, JUtility::TColor(color));
			++unk30C;
		} else {
			if (unk30C != 0)
				unk30C = 0;
			GXSetTevColor(GX_TEVREG0, JUtility::TColor(0xff3f3fc8));
			GXSetTevColor(GX_TEVREG1, JUtility::TColor(0xff3f3f00));
		}

		drawGaugeQuadF32(bounds, fillTop, bounds.y2, hiddenRatio, 1.0f);
	} else if (unk48) {
		if (unk30C != 0) {
			unk274->setPanePosition(90, JUTPoint(0, 0), JUTPoint(0, -100),
			                        JUTPoint(0, 0));
			unk30C = 0;
			unk49  = 1;
		}

		drawGaugeQuadF32(bounds, bounds.y1, bounds.y2, 0.0f, 1.0f);
	} else {
		drawGaugeQuadF32(bounds, bounds.y1, bounds.y2, 0.0f, 1.0f);
	}

	if (unk334[unk330]->isVisible())
		unk334[unk330]->hide();
}

void TGCConsole2::startDisappearMario()
{
	if (!unk3A8->getPane()->isVisible() || unk3B)
		return;

	unk3A8->updatePaneOffset(50, 0, -(unk3A8->getInitialY2() + 1));
	unk3B = 1;
}

void TGCConsole2::startAppearMario(bool param_1)
{
	if (unk3A8->getPane()->isVisible() && !unk3B)
		return;

	unk3A8->getPane()->show();
	unk3A8->setPaneOffset(50, 0, 0, 0, -(unk3A8->getInitialY2() + 1));

	unk38C->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);

	unk394->getPane()->hide();
	unk390->getPane()->hide();
	unk398->getPane()->hide();
	for (int i = 0; i < 3; ++i)
		unk39C[i]->getPane()->hide();

	if (param_1) {
		int lives = TFlagManager::smInstance->getFlag(0x20001) + 1;
		if (lives > 99)
			lives = 99;

		((J2DPicture*)unk39C[0]->getPane())
		    ->changeTexture(unkE0[(int)(lives * 0.1f)]->getTexInfo(), 0);
		((J2DPicture*)unk39C[1]->getPane())
		    ->changeTexture(unkE0[lives % 10]->getTexInfo(), 0);
	}

	unk3A     = 1;
	unk3AC[1] = param_1;
	unk3B     = 0;
	unk59     = 1;
	unk70     = 0;
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

void TGCConsole2::changeNum(TBlendPane*, int, int) { }

void TGCConsole2::setTimer(s32 param_1)
{
	// TODO: Needs regswaps but should otherwise be equivalent

	u32 timerValue;

	if (param_1 == -1) {
		s64 uVar3 = gpMarDirector->unkC8;
		s64 uVar5 = OSCheckStopwatch(&gpMarDirector->unkE8);
		s64 uVar9 = OSTicksToMilliseconds(uVar3);
		s64 uVar8 = OSTicksToMilliseconds(uVar5);

		timerValue = (uVar8 - uVar9) * 0.1f;

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
		if (timerValue < 1000) {
			JUtility::TColor currentColor
			    = ((J2DPicture*)unk458[9]->getPane())->mWhite;
			if (currentColor != unk508) {
				for (int i = 6; i <= 9; i++) {
					((J2DPicture*)unk458[i]->getPane())->mWhite = unk508;
				}
				((J2DPicture*)unk480[2]->getPane())->mWhite = unk508;
			}
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
	    && gpMarDirector->mState != TMarDirector::STATE_UNK5) {
		SMSGetMSound()->playTimer(timerValue * 10);
	}

	unk4FC = timerValue;
}

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
		unk1C4->setPanePosition(40, JUTPoint(unk1C8, unk1CA), JUTPoint(-10, 10),
		                        JUTPoint(0, 0));
		unk1C4->getPane()->show();
		unk1C4->update();
		isFinished = false;
	} else if (param_1 < 100) {
		isFinished = false;
	} else if (!unk1C4->update()) {
		isFinished = false;
	}

	return isFinished;
}

bool TGCConsole2::processInsertLife(int param_1)
{
	bool isFinished = true;

	isFinished &= unk1C4->update();

	if (param_1 == 14) {
		unk178->getPane()->show();
		unk178->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	if (param_1 == 24) {
		unk260->getPane()->show();
		unk260->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	updateDownPaneState(unk174, isFinished);
	updateDownPaneState(unk178, isFinished);
	updateDownPaneState(unk260, isFinished);

	return isFinished;
}

bool TGCConsole2::processAppearStar(int param_1)
{
	bool isFinished = true;

	isFinished &= unk140->update();

	isFinished &= unk160->update();

	if (param_1 == 14) {
		unk12C->getPane()->show();
		unk12C->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
		unk14C->getPane()->show();
		unk14C->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	if (param_1 == 24) {
		unk130->getPane()->show();
		unk130->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
		unk150->getPane()->show();
		unk150->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	int shines = TFlagManager::smInstance->getFlag(0x40000);
	for (int i = 0; i < 3; ++i) {
		if (param_1 == i * 6 + 28) {
			if (i == 2) {
				if ((!unk50 && shines >= 100) || (shines > 100 && unk50))
					unk134[i]->getPane()->show();
			} else {
				unk134[i]->getPane()->show();
			}
			unk134[i]->setPanePosition(50, cDownTopPoint, cDownMidPoint,
			                           cDownMidPoint);
		}
	}

	TFlagManager::smInstance->getFlag(0x40001);

	int blueCoins = 0;
	for (int flag = 0x46; flag < 0x56; ++flag)
		if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
			++blueCoins;

	for (int flag = 0x6C; flag <= 0x73; ++flag)
		if (TFlagManager::smInstance->getFlag(0x10000 + flag) != 0)
			++blueCoins;

	int blueCoinValue = unk168 - blueCoins * 10;
	if (blueCoinValue < 0)
		blueCoinValue = 0;

	for (int i = 0; i < 3; ++i) {
		if (param_1 == i * 6 + 28) {
			if (i == 2) {
				if (blueCoinValue >= 100)
					unk154[i]->getPane()->show();
			} else {
				unk154[i]->getPane()->show();
			}
			unk154[i]->setPanePosition(50, cDownTopPoint, cDownMidPoint,
			                           cDownMidPoint);
		}
	}

	updateDownPaneState(unk128, isFinished);
	updateDownPaneState(unk148, isFinished);
	updateDownPaneState(unk12C, isFinished);
	updateDownPaneState(unk14C, isFinished);
	updateDownPaneState(unk130, isFinished);
	updateDownPaneState(unk150, isFinished);

	for (int i = 0; i < 3; ++i)
		updateDownBlendPaneState(unk134[i], isFinished);
	for (int i = 0; i < 3; ++i) {
		updateDownPaneState(unk154[i], isFinished);
	}

	JUTRect bounds(unk12C->getPane()->mGlobalBounds);
	unk144->mGlobalTranslation.set(bounds.x1 + bounds.getWidth() * 0.5f,
	                               bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);

	JUTRect bounds2(unk14C->getPane()->mGlobalBounds);
	unk164->mGlobalTranslation.set(bounds2.x1 + bounds2.getWidth() * 0.5f,
	                               bounds2.y1 + bounds2.getHeight() * 0.5f,
	                               0.0f);

	return isFinished;
}

bool TGCConsole2::processDownCoin(int param_1)
{
	bool isFinished = true;

	if (param_1 == 14)
		unkCC->setPanePosition(50, JUTPoint(0, 0), cCoinMidPoint,
		                       cCoinMidPoint);

	if (param_1 == 24)
		unkD0->setPanePosition(50, JUTPoint(0, 0), cCoinMidPoint,
		                       cCoinMidPoint);

	for (int i = 0; i < 3; ++i) {
		if (param_1 == i * 6 + 28)
			unkD4[i]->setPanePosition(50, JUTPoint(0, i * 6), cCoinMidPoint,
			                          cCoinMidPoint);
	}

	updateCoinPaneState(unkC8, isFinished);

	if (param_1 < 14)
		isFinished = false;
	else
		updateCoinPaneState(unkCC, isFinished);

	if (param_1 < 24)
		isFinished = false;
	else
		updateCoinPaneState(unkD0, isFinished);

	for (int i = 0; i < 3; ++i) {
		if (param_1 < i * 6 + 28)
			isFinished = false;
		else
			updateCoinBlendPaneState(unkD4[i], isFinished);
	}

	isFinished &= unk108->update();

	setEmitterToPaneCenter(unk124, unkCC->getPane());

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
		if (unk274->unk14.x1 == 0 && unk274->unk14.y1 == 0)
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
	isFinished &= unk108->update();

	if (param_1 == 14) {
		unkCC->getPane()->show();
		unkCC->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);
	}

	if (param_1 == 24) {
		unkD0->getPane()->show();
		unkD0->setPanePosition(50, cDownTopPoint, cDownMidPoint, cDownMidPoint);
	}

	for (int i = 0; i < 3; ++i) {
		if ((unk6C >= 100 || i != 2) && param_1 == i * 6 + 28) {
			unkD4[i]->getPane()->show();
			unkD4[i]->setPanePosition(50, cDownTopPoint, cDownMidPoint,
			                          cDownMidPoint);
		}
	}

	updateDownPaneState(unkC8, isFinished);
	updateDownPaneState(unkCC, isFinished);
	updateDownPaneState(unkD0, isFinished);

	for (int i = 0; i < 3; ++i) {
		if (unk6C >= 100 || i != 2)
			updateDownBlendPaneState(unkD4[i], isFinished);
	}

	setEmitterToPaneCenter(unk124, unkCC->getPane());

	return isFinished;
}

bool TGCConsole2::processAppearMario(int param_1)
{
	bool isFinished = unk3A8->update();

	if (param_1 == 14) {
		unk394->getPane()->show();
		unk394->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	if (param_1 == 24) {
		unk390->getPane()->show();
		unk390->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	if (param_1 == 24) {
		unk398->getPane()->show();
		unk398->setPanePosition(50, cDownTopPoint, cDownMidPoint,
		                        cDownMidPoint);
	}

	for (int i = 0; i < 3; ++i) {
		if (i == 2)
			break;
		if (param_1 == i * 6 + 24) {
			unk39C[i]->getPane()->show();
			unk39C[i]->setPanePosition(50, cDownTopPoint, cDownMidPoint,
			                           cDownMidPoint);
		}
	}

	updateDownPaneState(unk38C, isFinished);
	updateDownPaneState(unk394, isFinished);
	updateDownPaneState(unk390, isFinished);
	updateDownPaneState(unk398, isFinished);

	for (int i = 0; i < 3; ++i) {
		if (i == 2)
			break;
		updateDownPaneState(unk39C[i], isFinished);
	}

	return isFinished;
}

bool TGCConsole2::processDrawTelop(u32)
{
	bool isFinished = false;

	JUTRect paneBounds(unk524->getPane()->mGlobalBounds);
	JUTRect textBounds(unk528->mBounds);

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

	if (gpMarDirector->mMap == 1) {
		switch (gpMarDirector->unk7D) {
		case 0:
			unk570 = scDolpicNewsDolpic0;
			break;
		case 1:
			if (*(u32*)((u8*)unkC4 + 0x68) != 0)
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
			if (TFlagManager::smInstance->getBool(0x50001)) {
				if (TFlagManager::smInstance->getBool(0x50002))
					unk570 = scDolpicNewsDolpic5_4;
				else
					unk570 = scDolpicNewsDolpic5_2;
			} else {
				if (TFlagManager::smInstance->getBool(0x50002))
					unk570 = scDolpicNewsDolpic5_3;
				else
					unk570 = scDolpicNewsDolpic5_1;
			}
			break;
		case 8: {
			int eventState = TFlagManager::smInstance->getFlag(0x60003);
			switch (eventState) {
			case 0:
				if (TFlagManager::smInstance->getBool(0x1038F)) {
					if (TFlagManager::smInstance->getNozzleRight(1, 0)
					    || TFlagManager::smInstance->getNozzleRight(1, 1))
						unk570 = scDolpicNewsDolpic8_6;
					else
						unk570 = scDolpicNewsDolpic8_4;
				} else {
					if (TFlagManager::smInstance->getNozzleRight(1, 0)
					    || TFlagManager::smInstance->getNozzleRight(1, 1))
						unk570 = scDolpicNewsDolpic8_5;
					else if (TFlagManager::smInstance->getFlag(0x40000) >= 20)
						unk570 = scDolpicNewsDolpic8_7;
					else
						unk570 = scDolpicNewsDolpic8_1;
				}
				break;
			case 1:
				if (TFlagManager::smInstance->getBool(0x1038F))
					unk570 = scDolpicNewsDolpic8_1;
				else
					unk570 = scDolpicNewsDolpic8_2;
				break;
			default:
				if (TFlagManager::smInstance->getNozzleRight(1, 0)
				    || TFlagManager::smInstance->getNozzleRight(1, 1))
					unk570 = scDolpicNewsDolpic8_1;
				else
					unk570 = scDolpicNewsDolpic8_3;
				break;
			}
			break;
		}
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

	if ((s32)unk2C < 10)
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

		unk3B0->setBounds(
		    JUTRect(unk3BC.x1, unk3BC.y2 - nextHeight, unk3BC.x2, unk3BC.y2));
	}

	return isFinished;
}

bool TGCConsole2::processDisappearBalloon()
{
	bool isFinished = false;

	JUTRect bounds = unk3B0->getBounds();
	int height     = bounds.getHeight();

	JUTRect contents  = unk3B0->getContentsBounds();
	int contentHeight = contents.getHeight();

	if (contentHeight > 0) {
		int nextHeight = height - unk3CC;
		if (contentHeight - unk3CC < 0)
			nextHeight += unk3CC - contentHeight;

		unk3B0->resize(unk3BC.getWidth(), nextHeight);

		unk3B0->setBounds(
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

	if (unk32C->mTextureNum > 0)
		unk32C->mTextures[0]->load(GX_TEXMAP0);
	if (unk328->mTextureNum > 0)
		unk328->mTextures[0]->load(GX_TEXMAP1);

	u8 selected = 0x17;
	for (u8 i = 0x16; (u8)i != 0; --i) {
		int threshold = (int)(64.0f * (1.0f - unk2B8));
		if (unk334[i]->mBounds.y1 >= threshold) {
			selected = i + 1;
			break;
		}
	}

	f32 translateY;
	if (selected < 0x17)
		translateY = 1.0f - (f32)unk334[selected]->mBounds.y1 * 0.015625f;
	else
		translateY = 1.0f;

	if (unk334[1]->mBounds.y1 < (int)(64.0f * (1.0f - unk2B8)))
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
		if (unk2AC[layer]->mTextureNum > 0)
			unk2AC[layer]->mTextures[0]->load(GX_TEXMAP0);
		if (unk2A0[layer]->mTextureNum > 0)
			unk2A0[layer]->mTextures[0]->load(GX_TEXMAP1);

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

		int top    = unk29C->getPane()->mGlobalBounds.y1 + topDiff[layer - 1];
		int bottom = top + unk2BC[layer].getHeight();
		int left   = unk2BC[layer].x1;
		int right  = unk2BC[layer].x2;
		GXBegin(GX_QUADS, GX_VTXFMT0, 4);
		GXPosition2f32((f32)left, (f32)top);
		GXTexCoord2s8(0, 0);
		GXPosition2f32((f32)right, (f32)top);
		GXTexCoord2s8(1, 0);
		GXPosition2f32((f32)right, (f32)bottom);
		GXTexCoord2s8(1, 1);
		GXPosition2f32((f32)left, (f32)bottom);
		GXTexCoord2s8(0, 1);
	}

	graph.setup2D();

	JUTRect bounds = unk2A0[0]->getBounds();
	f32 hidden     = 47.0f * (1.0f - unk2B8) - 0.5f;
	int y          = unk29C->getPane()->mGlobalBounds.y1 + (int)hidden;
	y += 1;
	if (y < 0)
		y = 0;

	JUTTexture* texture = unk2A0[0]->mTextures[0];
	unk2A0[0]->draw(bounds.x1, y, texture->getWidth(), texture->getHeight(),
	                false, false, false);

	bounds  = ((J2DPicture*)unk270->getPane())->getBounds();
	texture = ((J2DPicture*)unk270->getPane())->mTextures[0];
	((J2DPicture*)unk270->getPane())
	    ->draw(bounds.x1, bounds.y1, texture->getWidth(), texture->getHeight(),
	           false, false, false);

	bounds  = ((J2DPicture*)unk274->getPane())->getBounds();
	texture = ((J2DPicture*)unk274->getPane())->mTextures[0];
	((J2DPicture*)unk274->getPane())
	    ->draw(bounds.x1, bounds.y1, texture->getWidth(), texture->getHeight(),
	           false, false, false);

	JUTRect maskOffset = unk288->getBounds();
	bounds.add(maskOffset.x1, maskOffset.y1);
	texture = ((J2DPicture*)unk288)->mTextures[0];
	((J2DPicture*)unk288)
	    ->draw(bounds.x1, bounds.y1, texture->getWidth(), texture->getHeight(),
	           false, false, false);
}

void TGCConsole2::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 1) {
		if (!unk50) {
			if (gpCamera->isDemoCamera() || SMS_CheckMarioFlag(0x400)
			    || (!unk3AC[1] && TFlagManager::smInstance->getBool(0x30002)))
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
				unkD4[2]->getPane()->hide();

			if (unkB4 == 0) {
				startAppearCoin();
				startAppearMario(false);
				unkB0->search('ROOT')->mAlpha = 0;
			} else {
				unkB0->search('ROOT')->mAlpha = 0xff;
			}

			++unkB4;
			if (unkB4 > 0xa0)
				unk39 = 0;
		}

		updateLifeMeterState(this);
		updateStarHudAutoHide(this);
		updateShineAppearState(this);

		if (unk35) {
			bool done = true;
			if (!unk140->update())
				done = false;
			if (!unk160->update())
				done = false;
			if (!unk108->update())
				done = false;

			setEmitterToPaneCenter(unk124, unkCC->getPane());
			setEmitterToPaneCenter(unk164, unk14C->getPane());
			setEmitterToPaneCenter(unk144, unk12C->getPane());

			if (done) {
				unk140->getPane()->hide();
				unk160->getPane()->hide();
				unk144->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
				unk164->setStatus(JPABaseEmitter::STATUS_STOP_EMIT);
				unk35 = 0;
			}
		}

		updateWaterGaugeFill(this);
		updateCounterState(this);
		updateJetAppearState(this);

		updateRedCoinCounter(this);
		updateRedCoinAppearState(this);
		updateTimerAppearState(this);
		updateTelopState(this, flags);

		if (!unk46 && SMS_CheckMarioFlag(0x10000) && !unk45 && !unk50)
			startAppearTank();
		updateWaterTankState(this);

		updateLifeMeterBlink(this);
		updateCoinAppearState(this);

		if (unk4D) {
			if (unk108->update() && unk140->update()) {
				unk108->getPane()->hide();
				unk4D = 0;
			}
		}

		if (!unk45)
			processMoveNozzle();
		updateMarioAppearState(this);

		updateMarioLifeCounter(this);

		if (!unk3F && unk4A)
			setTimer(-1);

		if (unk59) {
			playHudMoveSound(0x4819);
			unk59 = 0;
		}

		if (unk5A) {
			playHudMoveSound(0x481A);
			unk5A = 0;
		}
	}

	if (flags & 2) {
		switch (unk10) {
		case 1:
			updateBalloonAppearState(this);
			break;
		case 2:
			processBalloonTextStep(this);
			break;
		case 3:
			if (unk3E4 > 0)
				--unk3E4;
			if ((unk3F8 && unk3E4 == 0) || unk3F4 != 0xffffffff)
				startDisappearBalloon(unk3E0, false);
			break;
		case 4:
			updateBalloonDisappearState(this);
			break;
		}

		updateJetCounterAnimation(this);
		updateCoinCounterAnimation(this);
		updateYoshiJuiceIconState(this);
	}

	if (flags & 8) {
		J2DOrthoGraph graph(graphics->mViewportRect);
		graph.setup2D();

		if (unk46 || unk45)
			drawWaterBack();

		graph.setup2D();

		unkB0->draw(0, 0, &graph);

		if (unk44 && !unk43 && !unk42 && unk520->getPane()->isVisible()) {
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
