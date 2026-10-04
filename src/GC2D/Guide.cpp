#include "Player/MarioAccess.hpp"
#include "System/FlagManager.hpp"
#include <GC2D/Guide.hpp>
#include <stdio.h>
#include <string.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <System/Application.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>
#pragma dont_inline on
#include <System/StageUtil.hpp>
#pragma dont_inline off
// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static u8 setup_wait;

static u32 scNormalStageTable[] = {
	0, 1, 2, 3, 4, 0xd, 6, 8, 9, 0xa,
};

TGuide::TGuide(const char* name)
    : JDrama::TViewObj(name)
    , unk10(8)
    , unkBC(nullptr)
    , unkC0(nullptr)
    , unkC4(0)
    , unkC5(0)
    , unk160(0xff)
    , unk164(1)
    , unk434(0, 0, 0, 0)
    , unk480(-1)
    , unk48C(0, 0, 0, 0)
{
}

void TGuide::load(JSUMemoryInputStream& stream)
{
	unkC5 = 0;
	JDrama::TNameRef::load(stream);

	JKRMemArchive* archive = setup(gpMarDirector->unkD8);

	unkBC = new J2DSetScreen("guide_1.blo", archive);
	((J2DTextBox*)unkBC->search('a_ic'))->setFont(gpSystemFont);
	((J2DTextBox*)unkBC->search('a_tx'))->setFont(gpSystemFont);
	((J2DTextBox*)unkBC->search('b_ic'))->setFont(gpSystemFont);
	((J2DTextBox*)unkBC->search('b_tx'))->setFont(gpSystemFont);

	unk124 = nullptr;
	unk124 = (J2DTextBox*)unkBC->search('s_mn');
	SMSMakeTextBuffer(unk124, 0x1a);
	unk124->setFont(gpSystemFont);

	for (int i = 0; i < 10; ++i) {
		char buffer[0x100];
		snprintf(buffer, 0xff, "/guide/timg/coin_number_%d.bti", i);
		unkC8[i] = new JUTTexture((const ResTIMG*)JKRGetResource(buffer));
	}

	unkF4 = unkBC->search('ss_i');
	for (int i = 0; i < 2; ++i)
		unkF8[i] = unkBC->search('ss_1' + i);

	unk100 = unkBC->search('sq_i');
	for (int i = 0; i < 2; ++i)
		unk104[i] = unkBC->search('sq_1' + i);

	for (int i = 0; i < 3; ++i)
		unk10C[i] = unkBC->search('sc_1' + i);

	unk118 = unkBC->search('sc_s');
	for (int i = 0; i < 2; ++i)
		unk11C[i] = unkBC->search('sb_1' + i);

	JUTTexture* cursor = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/guide/timg/guide_cursor_2.bti"));
	for (int i = 0; i < 2; ++i) {
		unk128[i]           = new TExPane(unkBC, 'cu_a' + i);
		J2DPicture* picture = (J2DPicture*)unk128[i]->getPane();
		picture->insert(cursor, picture->mTextureNum, 0.0f);
	}

	for (int i = 0; i < 13; ++i) {
		u32 tag   = (i / 10 << 8) + (i % 10 + '00');
		unk168[i] = unkBC->search(tag);
		unk1C0[i] = new TExPane(unkBC, (tag << 16) + '_0');
		unk218[i] = unk1C0[i]->getPane()->getBounds();
		unk378[i] = new TExPane(unkBC, (tag << 16) + '_1');
		((J2DTextBox*)unkBC->search((tag << 16) + '_3'))->setFont(gpSystemFont);
		((J2DTextBox*)unkBC->search((tag << 16) + '_5'))->setFont(gpSystemFont);
	}

	unk19C     = unkBC->search('20');
	unk1F4     = new TExPane(unkBC, 'lwin');
	unk218[13] = unk1F4->getPane()->getBounds();
	unk3AC     = new TExPane(unkBC, 'llin');

	for (int i = 0; i < 10; ++i)
		unk44C[i] = unkBC->search('pn00' + i);

	unk430 = unkBC->search('01mi');
	unk434 = unkBC->search('01_9')->getBounds();
	unk474 = JKRGetResource("/common/2d/stagename.bmg");

	unk134              = (J2DPicture*)unkBC->search('10');
	JUTTexture* texture = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/guide/timg/guide_draw_sun_2.bti"));
	unk134->insert(texture, unk134->mTextureNum, 0.0f);

	unk138  = (J2DPicture*)unkBC->search('13');
	texture = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/guide/timg/guide_draw_ship_2.bti"));
	unk138->insert(texture, unk138->mTextureNum, 0.0f);

	unk13C  = (J2DPicture*)unkBC->search('16');
	texture = new JUTTexture((const ResTIMG*)JKRGetResource(
	    "/guide/timg/guide_draw_palmtree_2.bti"));
	unk13C->insert(texture, unk13C->mTextureNum, 0.0f);

	unk140  = (J2DPicture*)unkBC->search('17');
	texture = new JUTTexture((const ResTIMG*)JKRGetResource(
	    "/guide/timg/guide_draw_palmtree_1.bti"));
	unk140->insert(texture, unk140->mTextureNum, 0.0f);

	unk144  = (J2DPicture*)unkBC->search('18');
	texture = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/guide/timg/guide_draw_fish_2.bti"));
	unk144->insert(texture, unk144->mTextureNum, 0.0f);

	unk148 = unkBC->search('11');
	unk14C = unkBC->search('14');
	unk150 = unkBC->search('15');
	unk154 = unkBC->search('12');
	unk154->setBasePosition(J2DBasePosition_4);
	unk158 = unkBC->search('19');
	unk158->setBasePosition(J2DBasePosition_4);

	void* message = JKRGetResource("/guide/guidemess.bmg");
	for (int i = 0; i < 13; ++i) {
		u32 key = (i / 10 << 24) + ('00' << 16) + (i % 10 << 16) + '_0';

		J2DTextBox* box = (J2DTextBox*)unkBC->search(key + 3);
		SMSMakeTextBuffer(box, 0x1e);
		box->setFont(gpSystemFont);
		strncpy(box->getStringPtr(), SMSGetMessageData(message, i + 13), 0x1e);

		box = (J2DTextBox*)unkBC->search(key + 5);
		SMSMakeTextBuffer(box, 0x200);
		box->setFont(gpSystemFont);
		strncpy(box->getStringPtr(), SMSGetMessageData(message, i), 0x200);
	}

	unk478 = new TExPane(unkBC, 'mark');
	unk444 = new TBoundPane(unkBC, '20');
	resetObjects();
	unkC5 = 1;
}

void TGuide::resetObjects()
{
	s32 totalShineCount = 0;
	for (u32 stage = 0; stage < 13; stage++) {
		if (stage < 10) {
			unk14[stage].unk0 = false;

			s32 shineCount = 0;
			if (stage != 0 && stage != 1) {
				for (u32 shineIdx = 0; shineIdx < 8; shineIdx++) {
					if (SMS_isGetShine(stage, shineIdx, false)) {
						shineCount++;
					}
				}
			}

			unk14[stage].shineCount = shineCount < 100 ? shineCount : 99;
			totalShineCount += shineCount;

			s32 etcShineCount = 0;
			if (stage != 0 && stage != 1) {
				if (SMS_isGetShine(stage, 1, true)) {
					etcShineCount++;
				}
				if (SMS_isGetShine(stage, 2, true)) {
					etcShineCount++;
				}
			}

			unk14[stage].etcShineCount = etcShineCount < 10 ? etcShineCount : 9;
			totalShineCount += etcShineCount;

			u16 coinCount
			    = TFlagManager::getInstance()->getFlag(stage + 0x20005);

			unk14[stage].coinCount
			    = (s32)coinCount < 1000 ? (s32)coinCount : 999;

			unk14[stage].etcShine = SMS_isGetShine(stage, 0, true);

			if (unk14[stage].etcShine) {
				totalShineCount++;
			}

			s32 blueCoinCount = 0;
			if (stage != 0) {
				for (u8 blueCoin = 0; blueCoin < 50; blueCoin++) {
					if (TFlagManager::getInstance()->getBlueCoinFlag(
					        scNormalStageTable[stage], blueCoin)) {
						blueCoinCount++;
					}
				}
			}

			unk14[stage].blueCoinCount
			    = blueCoinCount < 1000 ? blueCoinCount : 999;

			if (TFlagManager::getInstance()->getBool(stage + 0x103A5)) {
				unk44C[stage]->mVisible = false;
				unk168[stage]->mVisible = false;
			} else {
				unk44C[stage]->mVisible = true;
				unk168[stage]->mVisible = true;
			}
		}
	}

	unk14[9].unk0     = true;
	s32 blueCoinCount = 0;
	for (u8 blueCoin = 0u; blueCoin < 50; blueCoin++) {
		if (TFlagManager::getInstance()->getBlueCoinFlag(scNormalStageTable[9],
		                                                 blueCoin)) {
			blueCoinCount++;
		}
	}
	unk14[9].blueCoinCount = blueCoinCount;

	s16 shineCount = 0;
	if (TFlagManager::getInstance()->getBool(0x10056)) {
		shineCount++;
	}
	if (TFlagManager::getInstance()->getBool(0x10058)) {
		shineCount++;
	}
	unk14[0].shineCount = shineCount;
	totalShineCount += shineCount;

	unk14[1].shineCount
	    = TFlagManager::getInstance()->getFlag(0x40000) - totalShineCount;

	changeBotStatus(-1);
	resetScore();

	unk128[0]->getPane()->mVisible = true;
	unk128[1]->getPane()->mVisible = true;
}

void TGuide::resetScore() { }

JKRMemArchive* TGuide::setup(JKRMemArchive* archive)
{
	if (archive)
		SMSMountAramArchive(archive, gArBkGuide);
	else
		setup_wait = 0x10;

	unkC4 = 0;
	return archive;
}

void TGuide::setup2(JKRMemArchive*) { }

void TGuide::startMoveCursor()
{
	unk10  = 9;
	unk164 = 0;
}

void TGuide::startMoveCursor2() { }

void TGuide::linkSelect() { }

void TGuide::changePattern(J2DPicture*, s16, u32) { }

void TGuide::mirrorPattern(J2DPicture*, s16, u32) { }

void TGuide::rotatePattern(J2DPicture*, s16, u32, s16) { }

void TGuide::shinePattern(TBoundPane*, s16, u32) { }

void TGuide::mmarkPattern(TExPane*, s16, u32) { }

void TGuide::searchNearPoint(s16*, s16*, s16, s16) { }

int TGuide::checkPoint(int param_1, int param_2)
{
	int result = -1;
	for (int i = 0; i < 14; ++i) {
		JUTRect rect = unk168[i]->getBounds();
		if (param_1 > rect.x1 && param_1 < rect.x2 && param_2 > rect.y1
		    && param_2 < rect.y2) {
			result = i;
			break;
		}
	}

	if (result == -1) {
		for (int i = 0; i < 10; ++i) {
			JUTRect rect = unk44C[i]->getBounds();
			if (param_1 > rect.x1 && param_1 < rect.x2 && param_2 > rect.y1
			    && param_2 < rect.y2) {
				result = i;
				break;
			}
		}
	}

	if (result >= 0 && result < 10 && !unk44C[result]->isVisible())
		result = -1;

	return result;
}

void TGuide::changeBotStatus(int) { }

void TGuide::placeMario() { }

void TGuide::appearGuidePane(int) { }

void TGuide::disappearGuidePane(int) { }

void TGuide::perform(u32, JDrama::TGraphics*) { }
