#include "version.h"
#include <GC2D/ConsoleStr.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <System/StageUtil.hpp>
#include <System/FlagManager.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <dolphin/gx/GXCull.h>
#include <macros.h>
#include <stdio.h>

extern JPAEmitterManager* gpEmitterManager4D2;

#include <System/DummyStrings.hpp>

JUTPoint TConsoleStr::cShineGetRight1(150, -50);
JUTPoint TConsoleStr::cShineGetLeft1(-21, 7);
JUTPoint TConsoleStr::cShineGetRight2(0, 0);
JUTPoint TConsoleStr::cShineGetLeft2(-21, 7);
JUTPoint TConsoleStr::cShineGetRight3(0, 0);
JUTPoint TConsoleStr::cShineGetLeft3(-200, 65);

TConsoleStr::TConsoleStr(const char* name)
    : JDrama::TViewObj(name)
    , mSplashTextScreen(nullptr)
    , unk14(nullptr)
    , unk20(0xB4)
    , unk24(0)
    , unk2A8(0)
    , unk2B8(0)
    , unk2BC(STATE_UNK7)
{
}

f32 TConsoleStr::getWipeCloseTime() { return 30.0f / SMSGetVSyncTimesPerSec(); }

#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
#define GO_LETTERS_LENGTH        (this->mGoLettersLength)
#define SHINE_GET_LETTERS_LENGTH (this->mShineLettersLength)
#define MISS_LETTERS_LENGTH      (this->mMissLettersLength)
#define COMMON2D                 "/cmn2d"
#else
#define GO_LETTERS_LENGTH        (3)
#define SHINE_GET_LETTERS_LENGTH (9)
#define MISS_LETTERS_LENGTH      (5)
#define COMMON2D                 "/common/2d"
#endif

void TConsoleStr::load(JSUMemoryInputStream& stream)
{
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
	// what is default??
	mGoLettersLength      = 11;
	mShineLettersLength   = 11;
	mMissLettersLength    = 12;
	u32 sLoadResourceLang = TFlagManager::getInstance()->getFlag(MSF_LANGUAGE);
	switch (sLoadResourceLang) {
	case MSLANG_ENGLISH:
		mGoLettersLength    = 3;
		mShineLettersLength = 6;
		mMissLettersLength  = 7;
		break;
	case MSLANG_GERMAN:
		mGoLettersLength    = 4;
		mShineLettersLength = 11;
		mMissLettersLength  = 12;
		break;
	case MSLANG_FRENCH:
		mGoLettersLength    = 11;
		mShineLettersLength = 11;
		mMissLettersLength  = 8;
		break;
	case MSLANG_SPANISH:
		mGoLettersLength    = 10;
		mShineLettersLength = 11;
		mMissLettersLength  = 10;
		break;
	case MSLANG_ITALIAN:
		mGoLettersLength    = 4;
		mShineLettersLength = 10;
		mMissLettersLength  = 7;
		break;
	}
#endif

	JKRArchive* arch = SMSSwitch2DArchive("guide", gArBkConsole);
	JDrama::TViewObj::load(stream);
	mSplashTextScreen = new J2DSetScreen("big_tx_1.blo", arch);
	mSplashTextScreen->setCullBack(GX_CULL_BACK);
	unk14 = new J2DSetScreen("scenario_demo_1.blo", arch);
	unk14->setCullBack(GX_CULL_BACK);

	for (int i = 0; i < GO_LETTERS_LENGTH; ++i) {
		mGoLetters[i] = new TBoundPane(
		    mSplashTextScreen,
		    VERSION_SELECT(('go00' + i),
		                   GMSP01(((i / 10) << 8) + 'go00' + (i % 10))));
		mGoLetters[i]->unk0->hide();
	}

	for (int i = 0; i < SHINE_GET_LETTERS_LENGTH; ++i) {
		mShineGetLetters[i] = new TBoundPane(
		    mSplashTextScreen,
		    VERSION_SELECT(('sg00' + i),
		                   GMSP01(((i / 10) << 8) + 'sg00' + (i % 10))));
	}

	for (int i = 0; i < MISS_LETTERS_LENGTH; ++i) {
		mMissLetters[i] = new TBoundPane(
		    mSplashTextScreen,
		    VERSION_SELECT(('ms00' + i),
		                   GMSP01(((i / 10) << 8) + 'ms00' + (i % 10))));
#if defined(VERSION_GMSP01) || defined(VERSION_GMSP01_NX)
		mMissLetters[i]->unk0->hide();
#endif
	}

	for (int i = 0; i < 5; ++i) {
		unk27C[i] = new TExPane(mSplashTextScreen, 're00' + i);
	}

	mMapStoryTitles[0] = (J2DTextBox*)unk14->search('\0map');
	SMSMakeTextBuffer(mMapStoryTitles[0], 0x80);
	mMapStoryTitles[1] = (J2DTextBox*)unk14->search('stry');
	SMSMakeTextBuffer(mMapStoryTitles[1], 0x80);

	for (int i = 0; i < 2; ++i) {
		mHeaderFooterMasks[i] = new TExPane(unk14, 'msk1' + i);
		mMapStoryTitles[i]->setFont(gpSystemFont);
	}

	unk298[0] = new TExPane(unk14, 'wp_l');
	unk298[1] = new TExPane(unk14, 'wp_r');

	u32 uVar1     = SMS_getShineStage(SMSGetMarDirector()->mMap);
	u32 uVar9     = TFlagManager::getInstance()->getFlag(MSF_EPISODE);
	void* pvVar10 = JKRGetResource(COMMON2D "/stagename.bmg");
	mMapStoryTitles[0]->setString(SMSGetMessageData(pvVar10, uVar1));

	if (SMSGetMarDirector()->mMap != 15) {
		void* pvVar10 = JKRGetResource(COMMON2D "/scenarioname.bmg");

		s16 uVar2 = SMS_getShineID(uVar1, uVar9, false);

		const void* puVar15;
		if (pvVar10 == nullptr || uVar2 == -1)
			puVar15 = "";
		else
			puVar15 = SMSGetMessageData(pvVar10, SMS_getNormalStage(uVar2));

		snprintf(mMapStoryTitles[1]->getStringPtr(), 0x80, "%s", puVar15);
	}
}

void TConsoleStr::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	unk2AC[0] = nullptr;
	unk2AC[1] = nullptr;
	unk2AC[2] = nullptr;
}

void TConsoleStr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		if (SMSGetMarDirector()->mState != 5) {
			bool bVar6 = false;

			switch (unk2B8) {
			case 1:
				bVar6 = processGo(unk18);
				break;

			case 2:
				if (unk18 == unk20) {
					mShineGetLetters[0]->setPanePosition(
					    0x28, cShineGetRight1, cShineGetLeft1, cShineGetLeft1);
					mShineGetLetters[0]->getPane()->show();
				} else if (unk18 > unk20) {
					bVar6 = processShineGet((unk18 - unk20) * 2.0f);
				}
				break;

			case 3:
				bVar6 = processMiss(unk18 * 2.0f);
				break;

			case 4:
				bVar6 = processScenario(unk18);
				break;

			case 5:
				bVar6 = processReady(unk18 * 2.0f);
				if (unk18 >= 60.0f) {
					if (unk18 == 60.0f) {
						mGoLetters[0]->getPane()->hide();
						mGoLetters[1]->getPane()->hide();
						mGoLetters[2]->getPane()->hide();
						mGoLetters[0]->getPane()->setAlpha(0xff);
						mGoLetters[1]->getPane()->setAlpha(0xff);
						mGoLetters[2]->getPane()->setAlpha(0xff);
					}
					bVar6 &= processGo(unk18 - 60.0f);
				}
				break;
			}

			if (unk2BC == STATE_UNK2) {
				bool uVar13 = true;

				for (int i = 0; i < 2; ++i) {
					uVar13 &= mHeaderFooterMasks[i]->update();
					mMapStoryTitles[i]->setAlpha(
					    0xff - mHeaderFooterMasks[i]->getPane()->getAlpha());
				}

				if (uVar13) {
					unk2BC = STATE_UNK3;
					unk18  = 0.0f;
				}
			} else if (unk2BC == STATE_UNK8) {
				bool uVar13 = true;

				for (int i = 0; i < 2; ++i)
					uVar13 &= mHeaderFooterMasks[i]->update();

				if (uVar13) {
					unk2BC = STATE_UNK3;
					unk18  = 0.0f;
				}
			} else if (unk2BC == STATE_UNK5) {
				bool uVar13 = true;
				for (int i = 0; i < 2; ++i)
					uVar13 &= unk298[i]->update();
				if (uVar13) {
					unk2BC = STATE_UNK6;
					bVar6  = true;
					unk298[0]->getPane()->hide();
					unk298[1]->getPane()->hide();
				}
			} else if (unk2BC == STATE_UNK3) {
				unk18 += 1.0f;
				if (unk18 > 60.0f) {
					unk18  = 0.0f;
					unk2BC = STATE_UNK4;
				}
			} else if (unk2BC == STATE_UNK1) {
				if (unk2A8 != 0)
					startCloseWipe(false);
			}

			if (bVar6)
				unk2B8 = 0;

			unk18 += 0.5f;
		}

		if (!unk2A9
		    && SMSGetMarDirector()->mState != TMarDirector::STATE_UNK4) {
			for (int i = 0; i < ARRAY_COUNT(unk2AC); ++i)
				if (unk2AC[i])
					unk2AC[i]->stopCreateParticle();
			unk2A9 = true;
		}

		if (unk2A9 && SMSGetMarDirector()->mState == TMarDirector::STATE_UNK4) {
			for (int i = 0; i < ARRAY_COUNT(unk2AC); ++i)
				if (unk2AC[i])
					unk2AC[i]->playCreateParticle();
			unk2A9 = false;
		}
	}

	if ((cue & CUE_DRAW) && unk2B8 != 0) {
		const JDrama::TRect& rect = graphics->getScissor();

		J2DOrthoGraph local_1a0(graphics->getViewport());
		local_1a0.setup2D();

		if (unk2B8 == 1 && unk18 > 60.0f) {
			for (int i = 0; i < 3; ++i) {
				int local_b0[3] = { 4, 10, 20 };

				u8 alpha         = mGoLetters[i]->getPane()->getAlpha();
				u8 trailAlpha    = alpha;
				JUTRect local_a0 = mGoLetters[i]->getPane()->getBounds();

				for (int j = 0; j < 3; ++j) {
					int iVar9       = local_b0[j];
					JUTPoint& point = unk34[i][iVar9];
					if (point.x != 0) {
						trailAlpha = 0.7f * trailAlpha;
						mGoLetters[i]->getPane()->setAlpha(trailAlpha);
						mGoLetters[i]->getPane()->resize(
						    local_a0.getWidth() - iVar9 * 3,
						    local_a0.getHeight() - iVar9 * 3);

						JUTRect local_90
						    = mGoLetters[i]->getPane()->getBounds();
						((J2DPicture*)mGoLetters[i]->getPane())
						    ->draw(point.x, point.y, local_90.getWidth(),
						           local_90.getHeight(), false, false, false);
					}
				}

				mGoLetters[i]->getPane()->setAlpha(alpha);
				mGoLetters[i]->getPane()->resize(local_a0.getWidth(),
				                                 local_a0.getHeight());
			}
		}

		local_1a0.setup2D();
		if (unk2B8 == 4) {
			unk14->draw(0, 0, &local_1a0);
		} else {
			mSplashTextScreen->draw(0, 0, &local_1a0);
		}

		local_1a0.setup2D();
		graphics->setScissor(rect);
	}
}

void TConsoleStr::startAppearReady()
{
	if (unk2B8 == 1)
		return;

	unk2B8 = 1;
	unk18  = 0.0f;
	unk2A9 = 0;
	mGoLetters[0]->unk0->hide();
	mGoLetters[1]->unk0->hide();
	mGoLetters[2]->unk0->hide();
	mGoLetters[0]->unk0->setAlpha(0xff);
	mGoLetters[1]->unk0->setAlpha(0xff);
	mGoLetters[2]->unk0->setAlpha(0xff);
}

void TConsoleStr::startAppearGo()
{
	if (unk2B8 == 1)
		return;

	unk2B8 = 1;
	unk18  = 0.0f;
	unk2A9 = 0;
	mGoLetters[0]->getPane()->hide();
	mGoLetters[1]->getPane()->hide();
	mGoLetters[2]->getPane()->hide();
	mGoLetters[0]->getPane()->setAlpha(0xff);
	mGoLetters[1]->getPane()->setAlpha(0xff);
	mGoLetters[2]->getPane()->setAlpha(0xff);
}

void TConsoleStr::startAppearShineGet()
{
	if (unk2B8 != 0)
		return;

	unk2B8 = 2;
	unk18  = 0.0f;
	unk24  = 0;
}

void TConsoleStr::startAppearMiss()
{
	if (unk2B8 == 3)
		return;

	if (unk2B8 == 1) {
		mGoLetters[0]->getPane()->hide();
		mGoLetters[1]->getPane()->hide();
		mGoLetters[2]->getPane()->hide();
	}

	unk2B8 = 3;
	unk18  = 0.0f;

	for (int i = 0; i < 5; ++i) {
		mMissLetters[i]->getPane()->hide();
		mMissLetters[i]->getPane()->setAlpha(0);
	}

	mMissLetters[0]->setPanePosition(0x28, JUTPoint(0, -270), JUTPoint(0, -270),
	                                 JUTPoint(0, 30));

	mMissLetters[0]->getPane()->show();
}

void TConsoleStr::startAppearScenario()
{
	if (unk2B8 != 0)
		return;

	unk2B8 = 4;
	unk2BC = STATE_UNK0;
	unk18  = 0.0f;
	unk1C  = -200;

	for (int i = 0; i < 2; ++i) {
		mMapStoryTitles[i]->setAlpha(0);
		mHeaderFooterMasks[i]->getPane()->show();
	}

	mHeaderFooterMasks[0]->setPaneOffset(
	    0x1E, 0.0f, 0.0f, 0.0f,
	    -(mHeaderFooterMasks[0]->getInitialBounds().y2 + 1));
	mHeaderFooterMasks[1]->setPaneOffset(
	    0x1E, 0.0f, 0.0f, 0.0f,
	    465 - mHeaderFooterMasks[1]->getInitialBounds().y1);
}

bool TConsoleStr::processReady(int param_1)
{
	bool result = false;

	for (int i = 0; i < 5; ++i) {
		if (param_1 == i * 10) {
			JUTRect local_d8 = unk27C[i]->getPane()->getBounds();

			int width  = local_d8.getWidth();
			int height = local_d8.getHeight();
			unk27C[i]->setCenteredSize(0x1E, width, height, width + 80,
			                           height + 80);
			unk27C[i]->getPane()->show();
			unk27C[i]->getPane()->setAlpha(0);
		} else if (param_1 < i * 10 + 30) {
			unk27C[i]->update();
			u16 alpha = unk27C[i]->getPane()->getAlpha();
			alpha += 9;
			if (alpha > 0xff)
				alpha = 0xff;
			unk27C[i]->getPane()->setAlpha(alpha);
		} else if (param_1 >= i * 10 + 130) {
			if (param_1 == i * 10 + 130) {
				JUTRect local_e8 = unk27C[i]->getPane()->getBounds();

				int width  = local_e8.getWidth();
				int height = local_e8.getHeight();
				unk27C[i]->setCenteredSize(0x1E, width - 20, height - 20, width,
				                           height);
			} else if (param_1 < i * 10 + 160) {
				unk27C[i]->update();
				s16 alpha = unk27C[i]->getPane()->getAlpha();
				alpha -= 9;
				if (alpha < 0)
					alpha = 0;
				unk27C[i]->getPane()->setAlpha(alpha);
			} else if (i == 4) {
				result = true;
			}
		}
	}

	return result;
}

bool TConsoleStr::processGo(float param_1)
{
	bool result = false;

	if (param_1 < 90.0f) {

		for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mGoLetters),
		                                   GMSP01(mGoLettersLength),
		                                   GMSP01_NX(mGoLettersLength));
		     ++i) {
			if (param_1 == i * 10) {
				mGoLetters[i]->setPanePosition(
				    0x28, JUTPoint(0, 60), JUTPoint(0, -40), JUTPoint(0, -40));
				mGoLetters[i]->getPane()->show();
			}
		}

		for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mGoLetters),
		                                   GMSP01(mGoLettersLength),
		                                   GMSP01_NX(mGoLettersLength));
		     ++i) {
			if (mGoLetters[i]->update()) {
				if (mGoLetters[i]->unk14.x != 0 || mGoLetters[i]->unk14.y != 0)
					mGoLetters[i]->setPanePosition(0x1E, JUTPoint(0, -40),
					                               JUTPoint(0, -40),
					                               JUTPoint(0, 0));
			}
		}
	} else if (param_1 >= 95.0f) {
		if (param_1 == 95.0f) {
			mGoLetters[0]->setPanePosition(0x50, JUTPoint(0, 0),
			                               JUTPoint(-170, -180),
			                               JUTPoint(-340, -360));
			mGoLetters[1]->setPanePosition(
			    0x50, JUTPoint(0, 0), JUTPoint(0, -220), JUTPoint(0, -440));
			mGoLetters[2]->setPanePosition(
			    0x50, JUTPoint(0, 0), JUTPoint(160, -180), JUTPoint(320, -360));

			for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mGoLetters),
			                                   GMSP01(mGoLettersLength),
			                                   GMSP01_NX(mGoLettersLength));
			     ++i) {
				for (int j = 0; j < 16; ++j) {
					JUTRect bounds = mGoLetters[i]->getPane()->getGlbBounds();
					unk34[i][j].set(bounds.x1, bounds.y1);
				}

				JUTRect bounds = mGoLetters[i]->getPane()->getBounds();
				JGeometry::TVec3<f32> pos(bounds.x1 + bounds.getWidth() * 0.5f,
				                          bounds.y1 + bounds.getHeight() * 0.5f,
				                          0.0f);
				gpEmitterManager4D2->createEmitter(pos, 0x1FD, nullptr,
				                                   nullptr);
				unk2AC[i] = gpEmitterManager4D2->unkC8[0][0];
			}
		} else if (param_1 < 175.0f) {
			for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mGoLetters),
			                                   GMSP01(mGoLettersLength),
			                                   GMSP01_NX(mGoLettersLength));
			     ++i) {
				int alpha = mGoLetters[i]->getPane()->getAlpha() - 4;
				if (alpha < 0)
					alpha = 0;
				JUTRect bounds = mGoLetters[i]->getPane()->getGlbBounds();
				mGoLetters[i]->getPane()->setAlpha(alpha);
				mGoLetters[i]->getPane()->resize(bounds.getWidth() + 2,
				                                 bounds.getHeight() + 2);
				unk2AC[i]->setGlobalTranslation(
				    bounds.x1 + bounds.getWidth() * 0.5f,
				    bounds.y1 + bounds.getHeight() * 0.5f, 0.0f);
				mGoLetters[i]->update();

				if ((int)param_1 % 2 == 0) {
					for (int j = 14; j >= 0; --j)
						unk34[i][j + 1] = unk34[i][j];
					unk34[i][0].set(bounds.x1, bounds.y1);
				}
			}
		} else if (param_1 == 175.0f) {
			for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mGoLetters),
			                                   GMSP01(mGoLettersLength),
			                                   GMSP01_NX(mGoLettersLength));
			     ++i) {
				JUTRect bounds = mGoLetters[i]->getPane()->getBounds();
				mGoLetters[i]->getPane()->resize(bounds.getWidth() - 80,
				                                 bounds.getHeight() - 80);
			}
		} else {
			for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mGoLetters),
			                                   GMSP01(mGoLettersLength),
			                                   GMSP01_NX(mGoLettersLength));
			     ++i) {
				mGoLetters[i]->getPane()->hide();
				if (unk2AC[i])
					unk2AC[i]->stopCreateParticle();
			}
			result = true;
		}
	}

	return result;
}

bool TConsoleStr::processShineGet(int param_1)
{
	bool result = true;

	for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mShineGetLetters),
	                                   GMSP01(mShineLettersLength),
	                                   GMSP01_NX(mShineLettersLength));
	     ++i) {
		if (param_1 == 6 * i) {
			mShineGetLetters[i]->getPane()->show();
			mShineGetLetters[i]->setPanePosition(
			    0x28, cShineGetRight1, cShineGetLeft1, cShineGetLeft1);
			JUTRect local_74 = mShineGetLetters[i]->getPane()->getBounds();
			JGeometry::TVec3<f32> local_80(
			    local_74.x1 + local_74.getWidth() * 0.5f,
			    local_74.y1 + local_74.getHeight() * 0.5f, 0.0f);
			gpEmitterManager4D2->createEmitter(local_80, 0x1FE, nullptr,
			                                   nullptr);
		}

		if (param_1 == i * 6 + 40) {
			mShineGetLetters[i]->setPanePosition(
			    0x14, cShineGetLeft2, cShineGetLeft2, cShineGetRight2);
		}

		if (param_1 == i * 6 + 60) {
			JUTRect local_94 = mShineGetLetters[i]->getPane()->getBounds();
			JGeometry::TVec3<f32> local_80(
			    local_94.x1 + local_94.getWidth() * 0.5f,
			    local_94.y1 + local_94.getHeight() * 0.5f, 0.0f);
			gpEmitterManager4D2->createEmitter(local_80, 0x1FF, nullptr,
			                                   nullptr);
		}

		if (param_1 == i * 6 + 200) {
			mShineGetLetters[i]->setPanePosition(
			    0x28, cShineGetRight3, cShineGetRight3, cShineGetLeft3);
		}

		if (param_1 < i * 6 + 40) {
			u16 alpha = mShineGetLetters[i]->getPane()->getAlpha() + 7;
			if (alpha > 0xff)
				alpha = 0xff;
			mShineGetLetters[i]->getPane()->setAlpha(alpha);
		}

		if (param_1 > i * 6 + 200) {
			s16 alpha = mShineGetLetters[i]->getPane()->getAlpha() - 7;
			if (alpha < 0)
				alpha = 0;
			mShineGetLetters[i]->getPane()->setAlpha(alpha);
		}

		if (mShineGetLetters[i]->update()) {
			if (param_1 > i * 6 + 280) {
				mShineGetLetters[i]->getPane()->hide();
				result &= true;
			} else {
				result = 0;
			}
		} else {
			result = 0;
		}
	}

	return result;
}

bool TConsoleStr::processMiss(int param_1)
{
	bool result = true;

	for (int i = 0; i < VERSION_SELECT(ARRAY_COUNT(mMissLetters),
	                                   GMSP01(mMissLettersLength),
	                                   GMSP01_NX(mMissLettersLength));
	     ++i) {
		if (param_1 == i * 10) {
			mMissLetters[i]->getPane()->show();
			mMissLetters[i]->setPanePosition(
			    0x3C, JUTPoint(0, -270), JUTPoint(0, -270), JUTPoint(0, 30));
		}

		if (param_1 == i * 10 + 1) {
			JUTRect local_9c = mMissLetters[i]->getPane()->getBounds();
			JGeometry::TVec3<f32> local_a8(
			    local_9c.x1 + local_9c.getWidth() * 0.5f,
			    local_9c.y1 + local_9c.getHeight() * 0.5f, 0.0f);
			gpEmitterManager4D2->createEmitter(local_a8, 0x1F9, nullptr,
			                                   nullptr);
		}

		if (param_1 == i * 10 + 60) {
			mMissLetters[i]->getPane()->rotate(0.0f);
			mMissLetters[i]->setPanePosition(
			    0x28, JUTPoint(0, 30), JUTPoint(0, -80), JUTPoint(0, -80));
		}

		if (param_1 == i * 10 + 100) {
			mMissLetters[i]->setPanePosition(0x28, JUTPoint(0, -80),
			                                 JUTPoint(0, -80), JUTPoint(0, 0));
		}

		if (param_1 == i * 10 + 300) {
			mMissLetters[i]->setPanePosition(0x1E, JUTPoint(0, 0),
			                                 JUTPoint(0, 0), JUTPoint(0, 150));
		}

		if (param_1 < i * 10 + 60) {
			u16 alpha = mMissLetters[i]->getPane()->getAlpha();
			alpha += 12;
			if (alpha > 0xff)
				alpha = 0xff;
			mMissLetters[i]->getPane()->setAlpha(alpha);
		}

		if (mMissLetters[i]->update()) {
			if (param_1 > i * 10 + 360) {
				mMissLetters[i]->getPane()->hide();
				result &= true;
			} else {
				result = false;
			}
		} else {
			if (param_1 < i * 10 + 60) {
				mMissLetters[i]->getPane()->rotate((i * 10 - param_1) * 6);
			}

			result = false;
		}
	}

	return result;
}

bool TConsoleStr::processScenario(int)
{
	if (unk2BC > STATE_UNK0 && unk2BC != STATE_UNK7)
		return 0;

	bool uVar2 = true;
	uVar2 &= mHeaderFooterMasks[0]->update();
	uVar2 &= mHeaderFooterMasks[1]->update();

	unk1C += 2;

	if (unk1C > 355) {
		unk1C = 355;
		uVar2 &= true;
	} else {
		uVar2 = false;
	}

	int alpha = unk1C > 100 ? unk1C - 100 : 0;

	mMapStoryTitles[0]->setAlpha(alpha);
	mMapStoryTitles[1]->setAlpha(alpha);
	if (uVar2)
		unk2BC = STATE_UNK1;

	return false;
}

void TConsoleStr::startCloseWipe(bool param_1)
{
	if (param_1) {
		mHeaderFooterMasks[0]->getPane()->show();
		mHeaderFooterMasks[1]->getPane()->show();
		mMapStoryTitles[0]->hide();
		mMapStoryTitles[1]->hide();

		JUTRect local_74 = mHeaderFooterMasks[0]->getPane()->getBounds();
		s32 h            = 224;
		mHeaderFooterMasks[0]->setPaneSize(0x2D, local_74.getWidth(), h,
		                                   local_74.getWidth(), 0);
		mHeaderFooterMasks[0]->setPaneAlpha(0x2D, 255, 0);

		local_74 = mHeaderFooterMasks[1]->getPane()->getBounds();
		mHeaderFooterMasks[1]->setPaneOffset(
		    0x2D, 0, h - local_74.y1, 0,
		    465 - mHeaderFooterMasks[1]->getInitialBounds().y1);
		mHeaderFooterMasks[1]->setPaneSize(
		    0x2D, local_74.getWidth(),
		    465 - mHeaderFooterMasks[1]->getInitialBounds().y1 + h,
		    local_74.getWidth(), 0);
		mHeaderFooterMasks[1]->setPaneAlpha(0x2D, 255, 0);

		unk2BC = STATE_UNK8;
		unk2B8 = 4;
		unk2A8 = 1;
	} else if (unk2BC != STATE_UNK1) {
		unk2A8 = 1;
	} else {
		unk2BC           = STATE_UNK2;
		JUTRect local_88 = mHeaderFooterMasks[0]->getPane()->getBounds();
		s32 h            = 224;
		mHeaderFooterMasks[0]->setPaneSize(0x2D, local_88.getWidth(), h,
		                                   local_88.getWidth(),
		                                   local_88.getHeight());
		mHeaderFooterMasks[0]->setPaneAlpha(
		    0x2D, 255, mHeaderFooterMasks[0]->getPane()->getAlpha());

		local_88 = mHeaderFooterMasks[1]->getPane()->getBounds();
		mHeaderFooterMasks[1]->setPaneOffset(0x2D, 0, h - local_88.y1, 0, 0);
		mHeaderFooterMasks[1]->setPaneSize(0x2D, local_88.getWidth(), 464 - h,
		                                   local_88.getWidth(),
		                                   local_88.getHeight());
		mHeaderFooterMasks[1]->setPaneAlpha(
		    0x2D, 255, mHeaderFooterMasks[1]->getPane()->getAlpha());
	}
}

void TConsoleStr::startOpenWipe()
{
	unk2A8 = 0;
	unk2BC = STATE_UNK5;
	mHeaderFooterMasks[0]->getPane()->hide();
	unk298[0]->getPane()->show();
	mHeaderFooterMasks[1]->getPane()->hide();
	unk298[1]->getPane()->show();

	// TODO: TExPane::setPaneAlpha is wrong

	JUTRect local_3c = unk298[0]->getPane()->getBounds();
	unk298[0]->setPaneOffset(0x1E, -local_3c.getWidth(), 0, 0, 0);
	unk298[0]->setPaneAlpha(30, 100, 255);

	local_3c = unk298[1]->getPane()->getBounds();
	unk298[1]->setPaneOffset(0x1E, local_3c.getWidth(), 0, 0, 0);
	unk298[1]->setPaneAlpha(30, 100, 255);
}
