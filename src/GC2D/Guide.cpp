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
#include <GC2D/ScrnFader.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <System/MarioGamePad.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/StageUtil.hpp>
// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static u8 setup_wait;

TGuide::TGuide(const char* name)
    : JDrama::TViewObj(name)
    , unk10(STATE_CLOSED)
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

	JKRMemArchive* archive = setup(SMSGetMarDirector()->unkD8);

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

	unk168[13] = unkBC->search('20');
	unk1C0[13] = new TExPane(unkBC, 'lwin');
	unk218[13] = unk1C0[13]->getPane()->getBounds();
	unk378[13] = new TExPane(unkBC, 'llin');

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

	unk148 = (J2DPicture*)unkBC->search('11');
	unk14C = (J2DPicture*)unkBC->search('14');
	unk150 = (J2DPicture*)unkBC->search('15');
	unk154 = (J2DPicture*)unkBC->search('12');
	unk154->setBasePosition(J2DBasePosition_4);
	unk158 = (J2DPicture*)unkBC->search('19');
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
	int totalShines = 0;
	for (u32 stage = 0; stage < 13; ++stage) {
		if (stage >= ARRAY_COUNT(unk14))
			continue;
		unk14[stage].unk0 = 0;
		int shines        = 0;
		if (stage != 0 && stage != 1) {
			for (u32 scenario = 0; scenario < 8; ++scenario)
				if (SMS_isGetShine(stage, scenario, false))
					++shines;
		}
		shines                  = shines < 100 ? shines : 99;
		unk14[stage].shineCount = shines;
		totalShines += shines;
		int extraShines = 0;
		if (stage != 0 && stage != 1) {
			if (SMS_isGetShine(stage, 1, true))
				extraShines = 1;
			if (SMS_isGetShine(stage, 2, true))
				++extraShines;
		}
		if (extraShines >= 10)
			extraShines = 9;
		unk14[stage].etcShineCount = extraShines;
		totalShines += extraShines;
		int coins = (u16)TFlagManager::getInstance()->getFlag(
		    MSF_COIN_RECORD_BASE + stage);
		unk14[stage].coinCount = coins < 1000 ? coins : 999;
		unk14[stage].etcShine  = SMS_isGetShine(stage, 0, true);
		if (unk14[stage].etcShine)
			++totalShines;
		int blueCoins = 0;
		if (stage != 0) {
			for (u8 coin = 0; coin < 50; ++coin)
				if (TFlagManager::getInstance()->getBlueCoinFlag(
				        scNormalStageTable[stage], coin))
					++blueCoins;
		}
		if (blueCoins >= 1000)
			blueCoins = 999;
		unk14[stage].blueCoinCount = blueCoins;
		if (TFlagManager::getInstance()->getBool(MSF_VISITED_BASE + stage)) {
			unk44C[stage]->show();
			unk168[stage]->show();
		} else {
			unk44C[stage]->hide();
			unk168[stage]->hide();
		}
	}
	unk14[9].unk0 = 1;
	int blueCoins = 0;
	for (u8 coin = 0; coin < 50; ++coin)
		if (TFlagManager::getInstance()->getBlueCoinFlag(scNormalStageTable[9],
		                                                 coin))
			++blueCoins;
	unk14[9].blueCoinCount = blueCoins;
	s16 airportShines      = 0;
	if (TFlagManager::getInstance()->getBool(MSF_SHINE_AIRSTRIP))
		airportShines = 1;
	if (TFlagManager::getInstance()->getBool(MSF_SHINE_AIRSTRIP_REDS))
		++airportShines;
	unk14[0].shineCount = airportShines;
	totalShines += airportShines;
	unk14[1].shineCount
	    = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT) - totalShines;
	changeBotStatus(-1);
	resetScore();
	unk128[0]->getPane()->show();
	unk128[1]->getPane()->show();
}

void TGuide::resetScore()
{
	u8 extraShines = 0;
	u8 totalShines = 0;
	for (int stage = 0; stage < 10; ++stage) {
		if (stage == 9)
			continue;
		if (TFlagManager::getInstance()->getBool(MSF_VISITED_BASE + stage))
			unkBC->search('0_mn' + (stage << 24))->show();
		else
			unkBC->search('0_mn' + (stage << 24))->hide();
		if ((u32)stage <= 1)
			continue;
		for (int shine = 0; shine < 8; ++shine) {
			if (shine < unk14[stage].shineCount)
				unkBC->search('0ss1' + (stage << 24) + shine)->show();
			else
				unkBC->search('0ss1' + (stage << 24) + shine)->hide();
		}
		J2DPane* first = unkBC->search('0sq1' + (stage << 24));
		first->hide();
		J2DPane* second = unkBC->search('0sq2' + (stage << 24));
		second->hide();
		if (unk14[stage].etcShineCount != 0)
			first->show();
		if (unk14[stage].etcShineCount > 1)
			second->show();
		extraShines += unk14[stage].etcShineCount;
		totalShines += unk14[stage].shineCount;
	}
	totalShines += extraShines;
	if (extraShines == 0)
		unkBC->search('lqus')->hide();
	else
		unkBC->search('lqus')->show();
	for (int stage = 1; stage < 10; ++stage) {
		unk3D0[stage] = unkBC->search('mi00' + stage);
		if (stage == 9)
			continue;
		u16 coins = unk14[stage].coinCount;
		if (coins > 999)
			coins = 999;
		u32 tag              = '0c_1' + (stage << 24);
		J2DPicture* hundreds = (J2DPicture*)unkBC->search(tag);
		J2DPicture* tens     = (J2DPicture*)unkBC->search(tag + 1);
		J2DPicture* ones     = (J2DPicture*)unkBC->search(tag + 2);
		if (coins < 100) {
			hundreds->hide();
			tens->changeTexture(unkC8[coins / 10]->getTexInfo(), 0);
			ones->changeTexture(unkC8[coins % 10]->getTexInfo(), 0);
		} else {
			hundreds->show();
			int digit = coins / 100;
			hundreds->changeTexture(unkC8[digit]->getTexInfo(), 0);
			coins -= digit * 100;
			tens->changeTexture(unkC8[coins / 10]->getTexInfo(), 0);
			ones->changeTexture(unkC8[coins % 10]->getTexInfo(), 0);
		}
		if (unk14[stage].etcShine != 0) {
			unkBC->search('0c_s' + (stage << 24))->show();
			++totalShines;
		} else {
			unkBC->search('0c_s' + (stage << 24))->hide();
		}
	}
	unk3D0[0]         = unkBC->search('mi00');
	unk448            = unkBC->search('clic');
	s16 airportShines = 0;
	if (TFlagManager::getInstance()->getBool(MSF_SHINE_AIRSTRIP))
		airportShines = 1;
	if (TFlagManager::getInstance()->getBool(MSF_SHINE_AIRSTRIP_REDS))
		++airportShines;
	((J2DPicture*)unkBC->search('0s_1'))
	    ->changeTexture(unkC8[airportShines]->getTexInfo(), 0);
	totalShines += airportShines;
	int shines     = TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT);
	u8 plazaShines = shines - totalShines;
	if (plazaShines > 99)
		plazaShines = 99;
	((J2DPicture*)unkBC->search('1s_1'))
	    ->changeTexture(unkC8[plazaShines / 10]->getTexInfo(), 0);
	((J2DPicture*)unkBC->search('1s_2'))
	    ->changeTexture(unkC8[plazaShines % 10]->getTexInfo(), 0);
	if (shines > 999)
		shines = 999;
	J2DPicture* hundreds = (J2DPicture*)unkBC->search('lt_1');
	J2DPicture* tens     = (J2DPicture*)unkBC->search('lt_2');
	J2DPicture* ones     = (J2DPicture*)unkBC->search('lt_3');
	if (shines < 100) {
		hundreds->hide();
		tens->changeTexture(unkC8[shines / 10]->getTexInfo(), 0);
		ones->changeTexture(unkC8[shines % 10]->getTexInfo(), 0);
	} else {
		hundreds->show();
		int digit = shines / 100;
		hundreds->changeTexture(unkC8[digit]->getTexInfo(), 0);
		shines -= digit * 100;
		tens->changeTexture(unkC8[shines / 10]->getTexInfo(), 0);
		ones->changeTexture(unkC8[shines % 10]->getTexInfo(), 0);
	}
	switch (gpApplication.mSaveFile) {
	case 0:
		unkBC->search('ld_a')->show();
		unkBC->search('ld_b')->hide();
		unkBC->search('ld_c')->hide();
		break;
	case 1:
		unkBC->search('ld_a')->hide();
		unkBC->search('ld_b')->show();
		unkBC->search('ld_c')->hide();
		break;
	case 2:
		unkBC->search('ld_a')->hide();
		unkBC->search('ld_b')->hide();
		unkBC->search('ld_c')->show();
		break;
	}
	unk47C = 255.0f
	         * (1.0f
	            - TFlagManager::getInstance()->getFlag(MSF_SHINE_COUNT) / 30
	                  * 0.25f);
	unk478->getPane()->setAlpha(unk47C);
}

JKRMemArchive* TGuide::setup(JKRMemArchive* archive)
{
	if (archive)
		SMSMountAramArchive(archive, gArBkGuide);
	else
		setup_wait = 0x10;

	unkC4 = 0;
	return archive;
}

void TGuide::setup2(JKRMemArchive*)
{
	SMSSwitch2DArchive("game_6", gArBkGuide);
	unkC4 = 0;
}

void TGuide::startMoveCursor()
{
	unk10  = STATE_OPEN;
	unk164 = 0;
}

void TGuide::startMoveCursor2()
{
	s16 stage = SMS_getShineStage(SMSGetMarDirector()->mMap);
	unk42C    = stage;
	resetObjects();
	changeBotStatus(stage);
	for (int i = 0; i < ARRAY_COUNT(unk3D0); ++i) {
		if (i == stage)
			unk3D0[i]->show();
		else
			unk3D0[i]->hide();
	}
	unk164 = 0;
}

void TGuide::linkSelect()
{
	unkC0->onFlag(TMarioGamePad::PAD_FLAG_GUIDE_INPUT);
	if (unkC0->checkFrameMeaning(TMarioGamePad::MEANING_MENU_B)
	    || (unkC0->mButton.mTrigger & JUTGamePad::Z))
		unk10 = STATE_CLOSE;
	s16 dx = unkC0->mCompSPos[8] * 3.2f;
	s16 dy = -3.2f * unkC0->mCompSPos[9];
	int x  = unk128[0]->getPane()->getBounds().x1;
	int y  = unk128[0]->getPane()->getBounds().y1;
	x += dx;
	y += dy;
	if (x > 568)
		x = 568;
	if (x < 0)
		x = 0;
	if (y > 360)
		y = 360;
	if (y < 56)
		y = 56;
	unk128[0]->getPane()->move(x, y);
	unk128[1]->getPane()->move(x + 7, y + 4);
	int stage = checkPoint(x - 2, y + 10);
	if (stage != -1 && unkC0->checkMeaning(TMarioGamePad::MEANING_MENU_A))
		appearGuidePane(stage);
	if (stage != -1 && stage < 10) {
		int alpha = unk44C[stage]->getAlpha();
		alpha     = unk164 ? alpha + 4 : alpha - 4;
		if (alpha < 30) {
			unk164 = 1;
			alpha  = 30;
		} else if (alpha > 255) {
			unk164 = 0;
			alpha  = 255;
		}
		unk44C[stage]->setAlpha(alpha);
		changePattern((J2DPicture*)unk128[0]->getPane(), 45, unkF0);
		changePattern((J2DPicture*)unk128[1]->getPane(), 45, unkF0);
	}
	if (unk480 != stage) {
		changeBotStatus(stage);
		if (unk480 != -1 && unk480 < 10)
			unk44C[unk480]->setAlpha(255);
		if (stage == -1) {
			J2DPicture* cursor = (J2DPicture*)unk128[0]->getPane();
			cursor->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
			cursor->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);
			cursor = (J2DPicture*)unk128[1]->getPane();
			cursor->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
			cursor->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);
		}
		unk164 = 0;
		unk480 = stage;
	}
	changePattern(unk134, 90, unkF0);
	mirrorPattern(unk148, 90, unkF0);
	rotatePattern(unk154, 90, unkF0, 30);
	changePattern(unk138, 90, unkF0);
	mirrorPattern(unk14C, 90, unkF0);
	mirrorPattern(unk150, 90, unkF0);
	changePattern(unk13C, 90, unkF0);
	changePattern(unk140, 90, unkF0);
	changePattern(unk144, 90, unkF0);
	rotatePattern(unk158, 90, unkF0, -45);
	shinePattern(unk444, 90, unkF0);
	mmarkPattern(unk478, 270, unkF0);
	if (unk15C) {
		unk160 += 3;
		if (unk160 > 300)
			unk15C = 0;
	} else {
		unk160 -= 3;
		if (unk160 < 30)
			unk15C = 1;
	}
	u8 alpha;
	if (unk160 < 30)
		alpha = 30;
	else if (unk160 > 255)
		alpha = 255;
	else
		alpha = unk160;
	for (int i = 0; i < 10; ++i)
		unk168[i]->setAlpha(alpha);
	++unkF0;
	if (unkF0 > 540)
		unkF0 = 0;
}

void TGuide::changePattern(J2DPicture* picture, s16 period, u32 frame)
{
	if (frame % period == 0) {
		if ((frame / period) % 2 == 0) {
			picture->setBlendKonstColor(0.0f, 1.0f, 0.0f, 0.0f);
			picture->setBlendKonstAlpha(0.0f, 1.0f, 0.0f, 0.0f);
		} else {
			picture->setBlendKonstColor(1.0f, 0.0f, 0.0f, 0.0f);
			picture->setBlendKonstAlpha(1.0f, 0.0f, 0.0f, 0.0f);
		}
	}
}

void TGuide::mirrorPattern(J2DPicture* picture, s16 period, u32 frame)
{
	if (frame % period == 0) {
		if ((frame / period) % 2 == 0)
			picture->mMirror = MIRROR0;
		else
			picture->mMirror = J2DMirror_X;
	}
}

void TGuide::rotatePattern(J2DPicture* picture, s16 period, u32 frame,
                           s16 angle)
{
	if (frame % period == 0) {
		if ((frame / period) % 2 == 0)
			picture->mRotation = 0.0f;
		else
			picture->mRotation = angle;
	}
}

void TGuide::shinePattern(TBoundPane* pane, s16 period, u32 frame)
{
	int phase = frame % period;
	if (phase == 0)
		pane->setPanePosition(period / 2, JUTPoint(0, 0), JUTPoint(0, -5),
		                      JUTPoint(0, 0));
	else if (phase == period / 2)
		pane->setPanePosition(period / 2, JUTPoint(0, 0), JUTPoint(0, 5),
		                      JUTPoint(0, 0));
	u8 alpha = frame % (period * 2) < 130 ? 255 : 0;
	unk448->setAlpha(alpha);
	pane->update();
}

void TGuide::mmarkPattern(TExPane* pane, s16 period, u32 frame)
{
	if (frame % period == 0) {
		if ((frame / period) % 2 == 0)
			pane->setPaneAlpha(period, 0, unk47C);
		else
			pane->setPaneAlpha(period, unk47C, 0);
	}
	pane->update();
}

// TODO: This UNUSED method has no remaining body in the retail binary.
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

void TGuide::changeBotStatus(int stage)
{
	if (stage == -1 || stage >= 10) {
		unk124->hide();
		return;
	}
	if (unk14[stage].unk0 == 0) {
		unk124->show();
		unkF4->show();
		const char* name = SMSGetMessageData(unk474, stage);
		strncpy(unk124->getStringPtr(), name, 0x1a);
		int shines = unk14[stage].shineCount;
		if (shines < 0)
			shines = 0;
		if (shines > 99)
			shines = 99;
		if (shines < 10) {
			unkF8[1]->hide();
			((J2DPicture*)unkF8[0])
			    ->changeTexture(unkC8[shines]->getTexInfo(), 0);
		} else {
			unkF8[1]->show();
			((J2DPicture*)unkF8[0])
			    ->changeTexture(unkC8[shines / 10]->getTexInfo(), 0);
			((J2DPicture*)unkF8[1])
			    ->changeTexture(unkC8[shines % 10]->getTexInfo(), 0);
		}
		if ((u32)stage <= 1 || unk14[stage].etcShineCount == 0) {
			unk100->hide();
			unk104[0]->hide();
			unk104[1]->hide();
		} else if (unk14[stage].etcShineCount == 1) {
			unk100->show();
			unk104[0]->show();
			unk104[1]->hide();
		} else {
			unk100->show();
			unk104[0]->show();
			unk104[1]->show();
		}
		int coins = unk14[stage].coinCount;
		if (coins < 0)
			coins = 0;
		if (coins > 999)
			coins = 999;
		if (coins < 100) {
			unk10C[2]->hide();
			((J2DPicture*)unk10C[0])
			    ->changeTexture(unkC8[coins / 10]->getTexInfo(), 0);
			((J2DPicture*)unk10C[1])
			    ->changeTexture(unkC8[coins % 10]->getTexInfo(), 0);
		} else {
			unk10C[2]->show();
			int digit = coins / 100;
			((J2DPicture*)unk10C[0])
			    ->changeTexture(unkC8[digit]->getTexInfo(), 0);
			coins -= digit * 100;
			((J2DPicture*)unk10C[1])
			    ->changeTexture(unkC8[coins / 10]->getTexInfo(), 0);
			((J2DPicture*)unk10C[2])
			    ->changeTexture(unkC8[coins % 10]->getTexInfo(), 0);
		}
		if (unk14[stage].etcShine != 0)
			unk118->show();
		else
			unk118->hide();
		int blueCoins = unk14[stage].blueCoinCount;
		if (blueCoins < 0)
			blueCoins = 0;
		if (blueCoins > 99)
			blueCoins = 99;
		if (stage == 0) {
			unkBC->search('sb_i')->hide();
			unkBC->search('sc_t')->hide();
		} else {
			unkBC->search('sb_i')->show();
			unkBC->search('sc_t')->show();
		}
		if (blueCoins < 10) {
			unk11C[1]->hide();
			((J2DPicture*)unk11C[0])
			    ->changeTexture(unkC8[blueCoins % 10]->getTexInfo(), 0);
		} else {
			unk11C[1]->show();
			((J2DPicture*)unk11C[0])
			    ->changeTexture(unkC8[blueCoins / 10]->getTexInfo(), 0);
			((J2DPicture*)unk11C[1])
			    ->changeTexture(unkC8[blueCoins % 10]->getTexInfo(), 0);
		}
	} else {
		unk124->show();
		unkF4->hide();
		int blueCoins = unk14[stage].blueCoinCount;
		if (blueCoins < 0)
			blueCoins = 0;
		if (blueCoins > 99)
			blueCoins = 99;
		if (blueCoins < 10) {
			unk11C[1]->hide();
			((J2DPicture*)unk11C[0])
			    ->changeTexture(unkC8[blueCoins % 10]->getTexInfo(), 0);
		} else {
			unk11C[1]->show();
			((J2DPicture*)unk11C[0])
			    ->changeTexture(unkC8[blueCoins / 10]->getTexInfo(), 0);
			((J2DPicture*)unk11C[1])
			    ->changeTexture(unkC8[blueCoins % 10]->getTexInfo(), 0);
		}
		unkBC->search('sb_i')->show();
		unkBC->search('sc_t')->hide();
		unkBC->search('sq_i')->hide();
		const char* name = SMSGetMessageData(unk474, stage);
		strncpy(unk124->getStringPtr(), name, 0x1a);
	}
}

void TGuide::placeMario()
{
	if (SMS_getShineStage(SMSGetMarDirector()->mMap) != 1) {
		unk430->hide();
		return;
	}
	JGeometry::TVec3<f32> position = SMS_GetMarioPos();
	int width                      = unk434.getWidth();
	int height                     = unk434.getHeight();
	position.set(position.x * width / 25000.0f, 0.0f,
	             position.z * height / 21200.0f);
	int x = 0.5f * width + position.x - 0.5f * unk430->getWidth() - 2.0f;
	int y = 0.5f * height + position.z + 0.5f * unk430->getHeight();
	if (x > width - unk430->getWidth())
		x = width - unk430->getWidth();
	if (x < 0)
		x = 0;
	if (y > height - unk430->getHeight())
		y = height - unk430->getHeight();
	if (y < 0)
		y = 0;
	unk430->show();
	unk430->move(x, y);
	for (int i = 2; i < 10; ++i) {
		if (TFlagManager::getInstance()->getBool(MSF_VISITED_BASE + i))
			unkBC->search('01g/' + i)->show();
		else
			unkBC->search('01g/' + i)->hide();
	}
}

void TGuide::appearGuidePane(int index)
{
	unk424 = unk1C0[index];
	unk428 = unk378[index];
	JUTRect bounds(unk218[index]);
	JUTRect label(unk168[index]->getBounds());
	unk424->getPane()->show();
	int width     = bounds.getWidth();
	int height    = bounds.getHeight();
	TExPane* pane = unk424;
	pane->setPaneSize(20, width, height, 0, 0);
	const JUTRect& initial = pane->getInitialBounds();
	pane->setPaneOffset(20, (initial.getWidth() - width) * 0.5f,
	                    (initial.getHeight() - height) * 0.5f,
	                    initial.getWidth() * 0.5f, initial.getHeight() * 0.5f);
	unk424->setPaneOffset(20, 0, 0, label.x1 - bounds.x1,
	                      label.y1 - bounds.x1 - 40);
	unk428->getPane()->setAlpha(0);
	unk428->getPane()->show();
	unk428->setPaneAlpha(20, 255, 0);
	unk128[0]->setPaneAlpha(20, 0, 255);
	unk128[1]->setPaneAlpha(20, 0, 80);
	if (index == 1)
		placeMario();
	SMSGetMSound()->startSoundSystemSE(0x4804, 0, nullptr, 0);
	unk42C = index;
	unk10  = STATE_APPEARING;
	if (index != -1 && index < 10) {
		unk164 = 0;
		unk44C[index]->setAlpha(255);
	}
}

void TGuide::disappearGuidePane(int index)
{
	unk428->setPaneAlpha(20, 0, 255);
	JUTRect bounds(unk218[index]);
	JUTRect label(unk168[index]->getBounds());
	SMSGetMSound()->startSoundSystemSE(0x4805, 0, nullptr, 0);
	int width     = bounds.getWidth();
	int height    = bounds.getHeight();
	TExPane* pane = unk424;
	pane->setPaneSize(20, 0, 0, width, height);
	const JUTRect& initial = pane->getInitialBounds();
	pane->setPaneOffset(20, initial.getWidth() * 0.5f,
	                    initial.getHeight() * 0.5f,
	                    (initial.getWidth() - width) * 0.5f,
	                    (initial.getHeight() - height) * 0.5f);
	unk424->setPaneOffset(20, label.x1 - bounds.x1, label.y1 - bounds.x1 - 40,
	                      0, 0);
	unk128[0]->setPaneAlpha(20, 255, 0);
	unk128[1]->setPaneAlpha(20, 80, 0);
	unk10 = STATE_DISAPPEARING;
}

void TGuide::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (setup_wait != 0) {
		--setup_wait;
		if (setup_wait != 0)
			return;
		setup2(nullptr);
		startMoveCursor2();
	}
	if ((flags & 8) && unk10 != STATE_OPEN && unk10 != STATE_CLOSED) {
		J2DOrthoGraph graph(graphics->mViewportRect);
		graph.setup2D();
		unkBC->draw(0, 0, &graph);
	}
	if (!(flags & 1))
		return;
	bool done = true;
	switch (unk10) {
	case STATE_OPEN: {
		if (unkC5 && gpApplication.getFader()->isFullyFadedOut()) {
			gpApplication.getFader()->startWipe(5, 1.0f, 0.0f);
			unk10 = STATE_OPENING;
		}
		JUTRect bounds(
		    unk168[SMS_getShineStage(SMSGetMarDirector()->mMap)]->getBounds());
		unk128[0]->getPane()->move(bounds.x1 + 6, bounds.y1 - 1);
		unk128[1]->getPane()->move(bounds.x1 + 6, bounds.y1 - 1);
		break;
	}
	case STATE_OPENING:
		if (gpApplication.getFader()->isFullyFadedIn()) {
			unk10  = STATE_SELECT;
			unk428 = nullptr;
			unk424 = nullptr;
			unk128[0]->getPane()->setAlpha(255);
			unk128[1]->getPane()->setAlpha(80);
		}
		break;
	case STATE_SELECT:
		linkSelect();
		break;
	case STATE_APPEARING:
		done &= unk424->update();
		for (int i = 0; i < ARRAY_COUNT(unk128); ++i)
			done &= unk128[i]->update();
		if (done && unk428->update())
			unk10 = STATE_SELECTED;
		break;
	case STATE_SELECTED:
		if (unkC0->mEnabledFrameMeaning
		    & (TMarioGamePad::MEANING_MENU_A | TMarioGamePad::MEANING_MENU_B))
			disappearGuidePane(unk42C);
		else if (unkC0->mButton.mTrigger & JUTGamePad::Z)
			unk10 = STATE_CLOSE;
		break;
	case STATE_DISAPPEARING:
		if (unk428->update()) {
			done &= unk424->update();
			for (int i = 0; i < ARRAY_COUNT(unk128); ++i)
				done &= unk128[i]->update();
			if (done) {
				unk424->getPane()->hide();
				unk428->getPane()->hide();
				unk10 = STATE_SELECT;
				unkF0 = 0;
			}
		}
		break;
	case STATE_CLOSE:
		gpApplication.getFader()->startWipe(6, 1.0f, 0.0f);
		unkC0->offFlag(TMarioGamePad::PAD_FLAG_GUIDE_INPUT);
		SMSGetMSound()->startSoundSystemSE(0x4818, 0, nullptr, 0);
		unk10 = STATE_CLOSING;
		break;
	case STATE_CLOSING:
		if (gpApplication.getFader()->isFullyFadedOut()) {
			gpApplication.getFader()->startWipe(5, 1.0f, 0.0f);
			if (unk424 && unk424->getPane()->isVisible())
				unk424->getPane()->hide();
			if (unk428 && unk428->getPane()->isVisible())
				unk428->getPane()->hide();
			unkC4 = 1;
			unk10 = STATE_CLOSED;
		}
		break;
	}
}
