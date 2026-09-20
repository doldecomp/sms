#include <GC2D/ConsoleStr.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/ShineTable.hpp>
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
#include <stdio.h>

// rogue include needed for matching the .rodata string pool prefix
#include <System/DummyStrings.hpp>

JUTPoint TConsoleStr::cShineGetRight1(150, -50);
JUTPoint TConsoleStr::cShineGetLeft1(-21, 7);
JUTPoint TConsoleStr::cShineGetRight2(0, 0);
JUTPoint TConsoleStr::cShineGetLeft2(-50, 7);
JUTPoint TConsoleStr::cShineGetRight3(0, 0);
JUTPoint TConsoleStr::cShineGetLeft3(-200, 65);

TConsoleStr::TConsoleStr(const char* name)
    : JDrama::TViewObj(name)
    , unk10(nullptr)
    , unk14(nullptr)
    , unk20(0xB4)
    , unk24(0)
    , unk2A4(0)
    , unk2B4(0)
    , unk2B8(7)
{
}

f32 TConsoleStr::getWipeCloseTime() { return 30.0f / SMSGetVSyncTimesPerSec(); }

void TConsoleStr::load(JSUMemoryInputStream& stream)
{
	JKRArchive* arch = SMSSwitch2DArchive("guide", gArBkConsole);
	JDrama::TViewObj::load(stream);
	unk10 = new J2DSetScreen("big_tx_1.blo", arch);
	unk10->setCullBack(GX_CULL_BACK);
	unk14 = new J2DSetScreen("scenario_demo_1.blo", arch);
	unk14->setCullBack(GX_CULL_BACK);

	for (int i = 0; i < 3; ++i) {
		unk28[i] = new TBoundPane(unk10, 'go00' + i);
		unk28[i]->unk0->hide();
	}

	for (int i = 0; i < 6; ++i) {
		unk244[i] = new TBoundPane(unk10, 'sg00' + i);
	}

	for (int i = 0; i < 7; ++i) {
		unk25C[i] = new TBoundPane(unk10, 'ms00' + i);
	}

	for (int i = 0; i < 5; ++i) {
		unk278[i] = new TExPane(unk10, 're00' + i);
	}

	unk29C[0] = (J2DTextBox*)unk14->search('\0map');
	SMSMakeTextBuffer(unk29C[0], 0x80);
	unk29C[1] = (J2DTextBox*)unk14->search('stry');
	SMSMakeTextBuffer(unk29C[1], 0x80);

	for (int i = 0; i < 2; ++i) {
		unk28C[i] = new TExPane(unk14, 'msk1' + i);
		unk29C[i]->setFont(gpSystemFont);
	}

	unk294 = new TExPane(unk14, 'wp_l');
	unk298 = new TExPane(unk14, 'wp_r');

	u32 uVar1     = SMS_getShineStage(gpMarDirector->mMap);
	u32 uVar9     = TFlagManager::getInstance()->getFlag(0x40003);
	void* pvVar10 = JKRGetResource("/common/2d/stagename.bmg");
	unk29C[0]->setString(SMSGetMessageData(pvVar10, uVar1));

	if (gpMarDirector->mMap != 15) {
		void* pvVar10 = JKRGetResource("/common/2d/scenarioname.bmg");

		s16 uVar2 = SMS_getShineID(uVar1, uVar9, false);

		const void* puVar15;
		if (pvVar10 == nullptr || uVar2 == -1)
			puVar15 = "";
		else
			puVar15 = SMSGetMessageData(
			    pvVar10, (u16)SMS_getNormalStage(uVar2));

		snprintf(unk29C[1]->getStringPtr(), 0x80, "%s", puVar15);
	}
}

static inline JPABaseEmitter** ConsoleStrEmitters(TConsoleStr* p)
{
	JPABaseEmitter** emitters = p->unk2A8;
	return emitters;
}

void TConsoleStr::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	ConsoleStrEmitters(this)[0] = nullptr;
	unk2A8[1] = nullptr;
	unk2A8[2] = nullptr;
}

void TConsoleStr::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		if (gpMarDirector->mState != 5) {
			bool bVar6 = false;

			switch (unk2B4) {
			case 1:
				bVar6 = processGo(unk18);
				break;

			case 2:
				if (unk18 == unk20) {
					unk244[0]->setPanePosition(0x28, cShineGetRight1,
					                           cShineGetLeft1, cShineGetLeft1);
					unk244[0]->getPane()->show();
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
						unk28[0]->getPane()->hide();
						unk28[1]->getPane()->hide();
						unk28[2]->getPane()->hide();
						unk28[0]->getPane()->setAlpha(0xff);
						unk28[1]->getPane()->setAlpha(0xff);
						unk28[2]->getPane()->setAlpha(0xff);
					}
					bVar6 &= processGo(unk18 - 60.0f);
				}
				break;
			}

			if (unk2B8 == 2) {
				bool uVar13 = true;

				for (int i = 0; i < 2; ++i) {
					uVar13 &= unk28C[i]->update();
					unk29C[i]->setAlpha(0xff
					                    - unk28C[i]->getPane()->getAlpha());
				}

				if (uVar13) {
					unk2B8 = 3;
					unk18  = 0.0f;
				}
			} else if (unk2B8 == 8) {
				bool uVar13 = true;

				for (int i = 0; i < 2; ++i)
					uVar13 &= unk28C[i]->update();

				if (uVar13) {
					unk2B8 = 3;
					unk18  = 0.0f;
				}
			} else if (unk2B8 == 5) {
				bool uVar13 = true;
				for (int i = 0; i < 2; ++i)
					uVar13 &= unk28C[i]->update();
				if (uVar13) {
					unk2B8 = 6;
					bVar6  = true;
					unk294->getPane()->hide();
					unk298->getPane()->hide();
				}
			} else if (unk2B8 == 3) {
				unk18 += 1.0f;
				if (unk18 > 60.0f) {
					unk18  = 0.0f;
					unk2B8 = 4;
				}
			} else if (unk2B8 == 1) {
				if (unk2A4 != 0)
					startCloseWipe(false);
			}

			if (bVar6)
				unk2B4 = 0;

			unk18 += 0.5f;
		}

		// The "GO!" emitters are paused while the game is paused.
		if (!unk2A5 && gpMarDirector->mState != 4) {
			if (unk2A8[0])
				unk2A8[0]->stopCreateParticle();
			if (unk2A8[1])
				unk2A8[1]->stopCreateParticle();
			if (unk2A8[2])
				unk2A8[2]->stopCreateParticle();

			unk2A5 = true;
		}

		if (unk2A5 && gpMarDirector->mState == 4) {
			if (unk2A8[0])
				unk2A8[0]->playCreateParticle();
			if (unk2A8[1])
				unk2A8[1]->playCreateParticle();
			if (unk2A8[2])
				unk2A8[2]->playCreateParticle();

			unk2A5 = false;
		}
	}

	if ((cue & CUE_DRAW) && unk2B4 != 0) {
		const JDrama::TRect& rect = graphics->getScissor();

		J2DOrthoGraph local_1a0(graphics->getViewport());
		local_1a0.setup2D();

		if (unk2B4 == 1 && unk18 > 60.0f) {
			for (int i = 0; i < 3; ++i) {
				int trail[3] = { 4, 10, 20 };

				u8 alpha       = unk28[i]->getPane()->getAlpha();
				u8 fadedAlpha  = alpha;
				JUTRect bounds = unk28[i]->getPane()->getBounds();

				for (int j = 0; j < 3; ++j) {
					int slot = trail[j];
					if (unk34[i][slot].x != 0) {
						fadedAlpha = fadedAlpha * 0.7f;
						unk28[i]->getPane()->setAlpha(fadedAlpha);
						unk28[i]->getPane()->resize(
						    bounds.getWidth() - slot * 3,
						    bounds.getHeight() - slot * 3);

						JUTRect shrunk = unk28[i]->getPane()->getBounds();
						((J2DPicture*)unk28[i]->getPane())
						    ->draw(unk34[i][slot].x, unk34[i][slot].y,
						           shrunk.getWidth(), shrunk.getHeight(), false,
						           false, false);
					}
				}

				unk28[i]->getPane()->setAlpha(alpha);
				unk28[i]->getPane()->resize(bounds.getWidth(),
				                            bounds.getHeight());
			}
		}

		local_1a0.setup2D();
		if (unk2B4 == 4) {
			unk14->draw(0, 0, &local_1a0);
		} else {
			unk10->draw(0, 0, &local_1a0);
		}

		local_1a0.setup2D();
		graphics->setScissor(rect);
	}
}

void TConsoleStr::startAppearReady()
{
	if (unk2B4 == 1)
		return;

	unk2B4 = 1;
	unk18  = 0.0f;
	unk2A5 = 0;
	unk28[0]->unk0->hide();
	unk28[1]->unk0->hide();
	unk28[2]->unk0->hide();
	unk28[0]->unk0->mAlpha = 0xff;
	unk28[1]->unk0->mAlpha = 0xff;
	unk28[2]->unk0->mAlpha = 0xff;
}

void TConsoleStr::startAppearGo()
{
	if (unk2B4 == 1)
		return;

	unk2B4 = 1;
	unk18  = 0.0f;
	unk2A5 = 0;
	unk28[0]->getPane()->hide();
	unk28[1]->getPane()->hide();
	unk28[2]->getPane()->hide();
	unk28[0]->getPane()->setAlpha(0xff);
	unk28[1]->getPane()->setAlpha(0xff);
	unk28[2]->getPane()->setAlpha(0xff);
}

void TConsoleStr::startAppearShineGet()
{
	if (unk2B4 != 0)
		return;

	unk2B4 = 2;
	unk18  = 0.0f;
	unk24  = 0;
}

void TConsoleStr::startAppearMiss()
{
	if (unk2B4 == 3)
		return;

	if (unk2B4 == 1) {
		unk28[0]->getPane()->hide();
		unk28[1]->getPane()->hide();
		unk28[2]->getPane()->hide();
	}

	unk2B4 = 3;
	unk18  = 0.0f;

	for (int i = 0; i < 7; ++i) {
		unk25C[i]->getPane()->hide();
		unk25C[i]->getPane()->setAlpha(0);
	}

	unk25C[0]->setPanePosition(0x28, JUTPoint(0, -270), JUTPoint(0, -270),
	                           JUTPoint(0, 30));

	unk25C[0]->getPane()->show();
}

void TConsoleStr::startAppearScenario()
{
	if (unk2B4 != 0)
		return;

	unk2B4 = 4;
	unk2B8 = 0;
	unk18  = 0.0f;
	unk1C  = -200;

	for (int i = 0; i < 2; ++i) {
		unk29C[i]->setAlpha(0);
		unk28C[i]->getPane()->show();
	}

	unk28C[0]->setPaneOffset(0x1E, 0.0f, 0.0f, 0.0f,
	                         -(unk28C[0]->getInitialBounds().y2 + 1));
	unk28C[1]->setPaneOffset(0x1E, 0.0f, 0.0f, 0.0f,
	                         465 - unk28C[1]->getInitialBounds().y1);
}

bool TConsoleStr::processReady(int param_1)
{
	bool result = false;

	for (int i = 0; i < 5; ++i) {
		if (param_1 == i * 10) {
			JUTRect rect = unk278[i]->getPane()->getBounds();
			int w        = rect.getWidth();
			int h        = rect.getHeight();
			unk278[i]->setCenteredSize(0x1E, w, h, w + 80, h + 80);
			unk278[i]->getPane()->show();
			unk278[i]->getPane()->setAlpha(0);
		} else if (param_1 < i * 10 + 30) {
			unk278[i]->update();
			u16 alpha = unk278[i]->getPane()->getAlpha() + 9;
			if (alpha > 0xFF)
				alpha = 0xFF;
			unk278[i]->getPane()->setAlpha(alpha);
		} else if (param_1 >= i * 10 + 130) {
			if (param_1 == i * 10 + 130) {
				JUTRect rect = unk278[i]->getPane()->getBounds();
				int w        = rect.getWidth();
				int h        = rect.getHeight();
				unk278[i]->setCenteredSize(0x1E, w - 20, h - 20, w, h);
			} else if (param_1 < i * 10 + 160) {
				unk278[i]->update();
				s16 alpha = unk278[i]->getPane()->getAlpha();
				alpha -= 9;
				if (alpha < 0)
					alpha = 0;
				unk278[i]->getPane()->setAlpha(alpha);
			} else if (i == 4) {
				result = true;
			}
		}
	}

	return result;
}

extern JPAEmitterManager* gpEmitterManager4D2;

bool TConsoleStr::processGo(f32 param_1)
{
	bool result = false;

	if (param_1 < 90.0f) {
		for (int i = 0; i < 3; ++i) {
			if (param_1 == i * 10) {
				unk28[i]->setPanePosition(0x28, JUTPoint(0, 60),
				                          JUTPoint(0, -40), JUTPoint(0, -40));
				unk28[i]->getPane()->show();
			}
		}

		for (int i = 0; i < 3; ++i) {
			JUTRect rect = unk28[i]->getPane()->getBounds();
			if (unk28[i]->update()) {
				// The test is on TBoundPane's own target rect, not on the
				// pane's bounds: the ROM reads +0x14/+0x18 off the
				// TBoundPane itself.
				bool landed
				    = unk28[i]->unk14.x == 0 && unk28[i]->unk14.y == 0;
				if (!landed) {
					unk28[i]->setPanePosition(0x1E, JUTPoint(0, -40),
					                          JUTPoint(0, -40),
					                          JUTPoint(0, 0));
				}
			}
		}
	} else if (param_1 < 95.0f) {
		// nothing: the letters hold still for five frames
	} else {
		if (95.0f == param_1) {
			// The three letters fly apart, each trailing a particle emitter.
			unk28[0]->setPanePosition(0x50, JUTPoint(0, 0),
			                          JUTPoint(-170, -180),
			                          JUTPoint(-340, -360));
			unk28[1]->setPanePosition(0x50, JUTPoint(0, 0), JUTPoint(0, -220),
			                          JUTPoint(0, -440));
			unk28[2]->setPanePosition(0x50, JUTPoint(0, 0), JUTPoint(160, -180),
			                          JUTPoint(320, -360));

			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 16; ++j) {
					JUTRect global = unk28[i]->getPane()->getGlobalBounds();
					unk34[i][j].x  = global.x1;
					unk34[i][j].y  = global.y1;
				}

				JUTRect rect = unk28[i]->getPane()->getBounds();
				JGeometry::TVec3<f32> pos(rect.x1 + rect.getWidth() * 0.5f,
				                          rect.y1 + rect.getHeight() * 0.5f,
				                          0.0f);
				gpEmitterManager4D2->createEmitter(pos, 0x1FD, nullptr,
				                                   nullptr);
				unk2A8[i] = gpEmitterManager4D2->unkC8[0][0];
			}
		} else if (param_1 < 175.0f) {
			// Every other frame the trail shifts one slot down and the current
			// global position is pushed in at the front.
			int frame = param_1;

			for (int i = 0; i < 3; ++i) {
				s32 alpha = unk28[i]->getPane()->getAlpha() - 4;
				if (alpha < 0)
					alpha = 0;

				JUTRect global = unk28[i]->getPane()->getGlobalBounds();
				unk28[i]->getPane()->setAlpha(alpha);
				unk28[i]->getPane()->resize(global.getWidth() + 2,
				                            global.getHeight() + 2);

				unk2A8[i]->setGlobalTranslation(
				    global.x1 + global.getWidth() * 0.5f,
				    global.y1 + global.getHeight() * 0.5f, 0.0f);

				unk28[i]->update();

				if (frame % 2 == 0) {
					for (int j = 14; j >= 0; --j)
						unk34[i][j + 1] = unk34[i][j];

					unk34[i][0].x = global.x1;
					unk34[i][0].y = global.y1;
				}
			}
		} else if (param_1 == 175.0f) {
			for (int i = 0; i < 3; ++i) {
				JUTRect rect = unk28[i]->getPane()->getBounds();
				unk28[i]->getPane()->resize(rect.getWidth() - 80,
				                            rect.getHeight() - 80);
			}
		} else {
			unk28[0]->getPane()->hide();
			if (unk2A8[0])
				unk2A8[0]->stopCreateParticle();
			unk28[1]->getPane()->hide();
			if (unk2A8[1])
				unk2A8[1]->stopCreateParticle();
			unk28[2]->getPane()->hide();
			if (unk2A8[2])
				unk2A8[2]->stopCreateParticle();

			result = true;
		}
	}

	return result;
}

bool TConsoleStr::processShineGet(int param_1)
{
	bool result = true;

	for (int i = 0; i < 6; ++i) {
		if (param_1 == 6 * i) {
			unk244[i]->getPane()->show();
			unk244[i]->setPanePosition(0x28, cShineGetRight1, cShineGetLeft1,
			                           cShineGetLeft1);
			JUTRect local_74 = unk244[i]->getPane()->getBounds();
			JGeometry::TVec3<f32> local_80(
			    local_74.x1 + local_74.getWidth() * 0.5f,
			    local_74.y1 + local_74.getHeight() * 0.5f, 0.0f);
			gpEmitterManager4D2->createEmitter(local_80, 0x1FE, nullptr,
			                                   nullptr);
		}

		if (param_1 == i * 6 + 40) {
			unk244[i]->setPanePosition(0x14, cShineGetLeft2, cShineGetLeft2,
			                           cShineGetRight2);
		}

		if (param_1 == i * 6 + 60) {
			JUTRect local_94 = unk244[i]->getPane()->getBounds();
			JGeometry::TVec3<f32> local_80(
			    local_94.x1 + local_94.getWidth() * 0.5f,
			    local_94.y1 + local_94.getHeight() * 0.5f, 0.0f);
			gpEmitterManager4D2->createEmitter(local_80, 0x1FF, nullptr,
			                                   nullptr);
		}

		if (param_1 == i * 6 + 200) {
			unk244[i]->setPanePosition(0x28, cShineGetRight3, cShineGetRight3,
			                           cShineGetLeft3);
		}

		if (param_1 < i * 6 + 40) {
			u16 alpha = unk244[i]->getPane()->getAlpha() + 7;
			if (alpha > 0xff)
				alpha = 0xff;
			unk244[i]->getPane()->setAlpha(alpha);
		}

		if (param_1 > i * 6 + 200) {
			s16 alpha = unk244[i]->getPane()->getAlpha();
			alpha -= 7;
			if (alpha < 0)
				alpha = 0;
			unk244[i]->getPane()->setAlpha(alpha);
		}

		if (unk244[i]->update()) {
			if (param_1 > i * 6 + 280) {
				unk244[i]->getPane()->hide();
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

	for (int i = 0; i < 7; ++i) {
		if (param_1 == i * 10) {
			unk25C[i]->getPane()->show();
			unk25C[i]->setPanePosition(0x3C, JUTPoint(0, -270),
			                           JUTPoint(0, -270), JUTPoint(0, 30));
		}

		if (param_1 == i * 10 + 1) {
			JUTRect local_9c = unk25C[i]->getPane()->getBounds();
			JGeometry::TVec3<f32> local_a8(
			    local_9c.x1 + local_9c.getWidth() * 0.5f,
			    local_9c.y1 + local_9c.getHeight() * 0.5f, 0.0f);
			gpEmitterManager4D2->createEmitter(local_a8, 0x1F9, nullptr,
			                                   nullptr);
		}

		if (param_1 == i * 10 + 60) {
			unk25C[i]->setPanePosition(0x28, JUTPoint(0, 30), JUTPoint(0, -80),
			                           JUTPoint(0, -80));
		}

		if (param_1 == i * 10 + 100) {
			unk25C[i]->setPanePosition(0x28, JUTPoint(0, -80), JUTPoint(0, -80),
			                           JUTPoint(0, 0));
		}

		if (param_1 == i * 10 + 300) {
			unk25C[i]->setPanePosition(0x1E, JUTPoint(0, 0), JUTPoint(0, 0),
			                           JUTPoint(0, 150));
		}

		if (param_1 < i * 10 + 60) {
			u16 alpha = unk25C[i]->getPane()->getAlpha();
			alpha += 12;
			if (alpha > 0xff)
				alpha = 0xff;
			unk25C[i]->getPane()->setAlpha(alpha);
		}

		if (unk25C[i]->update()) {
			if (param_1 > i * 10 + 360) {
				unk25C[i]->getPane()->hide();
				result &= true;
			} else {
				result = false;
			}
		} else {
			if (param_1 < i * 10 + 60) {
				unk25C[i]->getPane()->mRotation = (i * 10 - param_1) * 6;
			}

			result = false;
		}
	}

	return result;
}

bool TConsoleStr::processScenario(int)
{
	if (unk2B8 > 0 && unk2B8 != 7)
		return 0;

	bool uVar2 = true;
	uVar2 &= unk28C[0]->update();
	uVar2 &= unk28C[1]->update();

	unk1C += 2;

	if (unk1C > 355) {
		unk1C = 355;
		uVar2 &= true;
	} else {
		uVar2 = false;
	}

	int alpha = unk1C > 100 ? unk1C - 100 : 0;

	unk29C[0]->setAlpha(alpha);
	unk29C[1]->setAlpha(alpha);
	if (uVar2)
		unk2B8 = 1;

	return false;
}

void TConsoleStr::startCloseWipe(bool param_1)
{
	if (param_1) {
		unk28C[0]->getPane()->show();
		unk28C[1]->getPane()->show();
		unk29C[0]->hide();
		unk29C[1]->hide();

		// centerY has to be a local: the ROM converts it to float at run time
		// rather than folding a 224.0f constant, and the other branch computes
		// 464 - centerY instead of folding to 240.
		int centerY  = 224;
		JUTRect rect = unk28C[0]->getPane()->getBounds();
		unk28C[0]->setPaneSize(0x2D, rect.getWidth(), centerY, rect.getWidth(),
		                       0);
		unk28C[0]->setPaneAlpha(0x2D, 255, 0);

		rect = unk28C[1]->getPane()->getBounds();
		unk28C[1]->setPaneOffset(0x2D, 0, 224 - rect.y1, 0,
		                         465 - unk28C[1]->getInitialBounds().y1);
		unk28C[1]->setPaneSize(
		    0x2D, rect.getWidth(),
		    465 - unk28C[1]->getInitialBounds().y1 + centerY, rect.getWidth(),
		    0);
		unk28C[1]->setPaneAlpha(0x2D, 255, 0);

		unk2B8 = 8;
		unk2B4 = 4;
		unk2A4 = 1;
	} else if (unk2B8 != 1) {
		unk2A4 = 1;
	} else {
		unk2B8 = 2;

		int centerY  = 224;
		JUTRect rect = unk28C[0]->getPane()->getBounds();
		unk28C[0]->setPaneSize(0x2D, rect.getWidth(), centerY,
		                       rect.getWidth(), rect.getHeight());
		unk28C[0]->setPaneAlpha(0x2D, 255, unk28C[0]->getPane()->getAlpha());

		rect = unk28C[1]->getPane()->getBounds();
		unk28C[1]->setPaneOffset(0x2D, 0, 224 - rect.y1, 0, 0);
		unk28C[1]->setPaneSize(0x2D, rect.getWidth(), 464 - centerY,
		                       rect.getWidth(), rect.getHeight());
		unk28C[1]->setPaneAlpha(0x2D, 255, unk28C[1]->getPane()->getAlpha());
	}
}

void TConsoleStr::startOpenWipe()
{
	unk2A4 = 0;
	unk2B8 = 5;
	unk28C[0]->mPane->hide();
	unk294->mPane->show();
	unk28C[1]->getPane()->hide();
	unk298->getPane()->show();

	JUTRect local_3c = unk294->getPane()->getBounds();
	unk294->setPaneOffset(0x1E, -local_3c.getWidth(), 0, 0, 0);
	unk294->setPaneAlpha(30, 100, 255);

	local_3c = unk298->getPane()->getBounds();
	unk298->setPaneOffset(0x1E, local_3c.getWidth(), 0, 0, 0);
	unk298->setPaneAlpha(30, 100, 255);
}
