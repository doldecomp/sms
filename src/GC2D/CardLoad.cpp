#include <GC2D/CardLoad.hpp>
#include <stdio.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <System/Application.hpp>
#include <System/StageUtil.hpp>
#include <System/MarDirector.hpp>
#include <System/FlagManager.hpp>
#include <GC2D/Option.hpp>
#include <GC2D/ExPane.hpp>
#include <GC2D/MessageUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static void clearBookmark(u32) { }

static void loadBookmark() { }

u32 TCardLoad::cMessageID;

TCardLoad::TCardLoad(const char* name)
    : JDrama::TViewObj(name)
    , unk10(5)
    , unk14(10)
    , unk1C(0)
    , unk28(nullptr)
    , unk2C(nullptr)
    , unk30(0)
    , unkA0(nullptr)
    , unkA4(0)
    , unkA8(60.0f)
    , unkAC(nullptr)
    , unkB8(0)
    , unkBC(0)
    , unk25C(nullptr)
    , unk270(nullptr)
    , unk274(0)
    , unk275(0)
    , unk284(0)
    , unk288(nullptr)
    , unk524(nullptr)
    , unk538(nullptr)
    , unk53C(nullptr)
    , unk754(0)
{
	gpCardLoad = this;
}

void TCardLoad::changePattern(J2DPicture*, s16, u32) { }

void TCardLoad::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);

	if (gpMarDirector->mMap != 15)
		return;

	JKRArchive* optionVolume = (JKRArchive*)JKRFileLoader::getVolume("option");

	unkA0 = JKRGetResource("/option/loadmessage.bmg");

	unk28 = new J2DSetScreen("load.blo", optionVolume);
	unk28->setCullBack(GX_CULL_BACK);
	unk2C = new J2DSetScreen("load_score.blo", optionVolume);
	unk2C->setCullBack(GX_CULL_BACK);
	unk754 = new TOptionControl;
	unk754->load();

	unk34 = new J2DSetScreen("title_1.blo", optionVolume);
	unk34->setCullBack(GX_CULL_BACK);

	for (int i = 0; i < 10; ++i) {
		char buffer[0x28];
		snprintf(buffer, 0x28, "/option/timg/coin_number_%d_min.bti", i);
		unkC8[i] = new JUTTexture((const ResTIMG*)JKRGetResource(buffer));
	}

	for (int i = 0; i < 11; ++i) {
		unk20C[i] = 25 * i + 150;
		unk222[i] = 4;
	}

	for (int i = 0; i < 8; ++i) {
		unk22E[i] = 400 * i;
		unk248[i] = 4;
	}

	for (int i = 0; i < 8; ++i) {
		unk22E[i] = 400 * i;
		unk248[i] = 4;
	}

	unk208 = unk34->search('ROOT');

	unkF0 = new TExPane(unk34, 'titl');
	unkF4 = new TExPane(unk34, 'nint');

	for (int i = 0; i < 11; ++i) {
		int key;
		if (i < 9)
			key = 's_01' + i;
		else
			key = 's_00' + ((i + 1) / 10) * 0x100 + (i + 1) % 10;

		unkF8[i] = new TExPane(unk34, key);
		unkF8[i]->getPane()->setAlpha(0);
		unk124[i] = unkF8[i]->getPane()->getBounds();
	}

	for (int i = 0; i < 13; ++i) {
		int key;
		if (i < 9)
			key = 'p_01' + i;
		else
			key = 'p_00' + ((i + 1) / 10) * 0x100 + (i + 1) % 10;

		unk1D4[i] = new TExPane(unk34, key);

		((J2DPicture*)unk1D4[i]->getPane())->mBlack = 0x01006667;

		unk1D4[i]->setPaneAlpha(20, 0xff, 0);
	}

	unk25C = unk28->search('yaji');
	unk260 = unk25C->getBounds();
	unk270 = unk28->search('\0s_1');

	unk25C->show();
	unk270->show();
	unk25C->setAlpha(0);
	unk288 = new TExPane(unk28, '\0w_0');
	unk28C = unk288->getPane()->getBounds();

	unk29C = (J2DTextBox*)unk28->search('m_0a');
	makeBuffer(unk29C, 0x80);

	unk2A0 = (J2DTextBox*)unk28->search('m_0b');
	makeBuffer(unk2A0, 0x80);

	JUTTexture* pJVar5 = new JUTTexture(
	    (const ResTIMG*)JKRGetResource("/option/timg/load_sun_1.bti"));
	for (int i = 0; i < 3; ++i) {
		unk2A4[i] = new TExPane(unk28, '\0w_1' + i);

		unk2C0[i] = (J2DTextBox*)unk28->search('m_1a' + 2 * i);
		makeBuffer(unk2C0[i], 0x80);

		unk2CC[i] = (J2DTextBox*)unk28->search('m_1b' + 2 * i);
		makeBuffer(unk2CC[i], 0x80);

		unk2D8[i] = unk28->search('shn0' + i);
		unk2E4[i] = (J2DPicture*)unk28->search('n_0a' + i * 0x100);
		unk2F0[i] = (J2DPicture*)unk28->search('n_0b' + i * 0x100);
		unk2FC[i] = unk28->search('shp0' + i);
		unk308[i] = (J2DPicture*)unk28->search('np0a' + i * 0x100);
		unk314[i] = (J2DPicture*)unk28->search('np0b' + i * 0x100);
		unk320[i] = (J2DPicture*)unk28->search('np0c' + i * 0x100);
		unk32C[i] = (J2DPicture*)unk28->search('cra1' + i * 0x100);

		unk32C[i]->insert(pJVar5, unk32C[i]->mTextureNum, 0.0f);
		unk28->search('cra2' + i)->hide();
	}

	unk2B0 = unk2A4[0]->getPane()->getBounds();

	for (int i = 0; i < 3; ++i) {
		unk33C[i] = new TExPane(unk28, '\0w_4' + i);
		unk348[i] = unk33C[i]->getPane()->getBounds();

		u32 key = 's_2a' + i;
		for (int j = 0; j < 4; ++j) {
			unk378[i][j] = new TExPane(unk28, key + j * 0x100);
			unk3A8[i][j] = unk378[i][j]->getPane()->getBounds();
		}
	}

	unk468 = new TExPane(unk28, '\0w_7');
	unk46C = unk468->getPane()->getBounds();

	unk47C = (J2DTextBox*)unk28->search('m_2a');
	makeBuffer(unk47C, 0x400);

	unk480 = (J2DTextBox*)unk28->search('m_2b');
	makeBuffer(unk480, 0x400);

	for (int i = 0; i < 2; ++i) {
		unk484[i] = new TExPane(unk28, 's_6a' + i * 0x100);
		unk48C[i] = unk484[i]->getPane()->getBounds();
	}

	unk4AC = new TExPane(unk28, '\0w_8');
	unk4B0 = unk4AC->getPane()->getBounds();

	unk4C0 = (J2DTextBox*)unk28->search('m_4a');
	makeBuffer(unk4C0, 0x400);

	unk4C4 = (J2DTextBox*)unk28->search('m_4b');
	makeBuffer(unk4C4, 0x400);

	unk4C8 = unk28->search('\0t_0');

	for (int i = 0; i < 3; ++i)
		unk4CC[i] = unk28->search('\0t_1' + i);

	for (int i = 0; i < 2; ++i) {
		unk4D8[i] = new TExPane(unk28, 's_6b' + i * 0x100);
		unk4E0[i] = unk4D8[i]->getPane()->getBounds();
	}

	unk500 = unk28->search('f_sh');
	unk510 = unk28->search('f_si');

	for (int i = 0; i < 3; ++i)
		unk504[i] = unk28->search('n_3a' + i);

	for (int i = 0; i < 2; ++i)
		unk514[i] = unk28->search('n_4a' + i);

	unk51C = (J2DTextBox*)unk28->search('m_3a');
	makeBuffer(unk51C, 0x14);

	unk520 = (J2DTextBox*)unk28->search('m_3b');
	makeBuffer(unk520, 0x14);

	unk524 = new TExPane(unk28, '\0w_9');
	unk528 = unk524->getPane()->getBounds();

	unk538 = (J2DTextBox*)unk28->search('m_5a');
	makeBuffer(unk538, 0x400);

	unk53C = (J2DTextBox*)unk28->search('m_5b');
	makeBuffer(unk53C, 0x400);
	unk53C->hide();

	for (int i = 0; i < 3; ++i)
		unk540[i] = unk28->search('s_0a' + i);

	unk54C = new TExPane(unk28, 'w_11');
	unk550 = unk54C->getPane()->getBounds();

	unk560 = (J2DTextBox*)unk28->search('m_7a');
	makeBuffer(unk560, 0x400);

	unk564 = (J2DTextBox*)unk28->search('m_7b');
	makeBuffer(unk564, 0x400);
	unk564->hide();

	unk568 = new TExPane(unk28, 'w_10');
	unk56C = unk568->getPane()->getBounds();

	unk57C = (J2DTextBox*)unk28->search('m_6a');
	makeBuffer(unk57C, 0x400);

	unk580 = (J2DTextBox*)unk28->search('m_6b');
	makeBuffer(unk580, 0x400);

	int local_90[] = { 2, 3, 4, 5, 6, 8, 7 };

	for (int i = 0; i < 7; ++i) {
		unk584[i].unk0 = (J2DPicture*)unk28->search('st_0' + i);
		for (int j = 0; j < 3; ++j)
			unk584[i].unk4[j]
			    = (J2DPicture*)unk28->search('n_0a' + i * 0x100 + j);

		int tmp = local_90[i];

		for (int j = 0; j < 8; ++j) {
			unk584[i].unk10[j] = unk28->search('sh0a' + i * 0x100 + j);

			if (!SMS_isGetShine(tmp, j, false))
				unk584[i].unk10[j]->hide();
		}

		for (int j = 0; j < 2; ++j) {
			unk584[i].unk30[j] = unk28->search('sh0i' + i * 0x100 + j);
			unk584[i].unk30[j]->hide();
		}

		unk584[i].unk38 = unk28->search('sh0k' + i);
		unk584[i].unk38->hide();
	}

	for (int i = 0; i < 3; ++i) {
		unk728[i] = unk28->search('\0t_1' + i);
		unk728[i]->hide();
	}

	unk740 = unk28->search('\0t_p');
	unk744 = unk28->search('s_tl');
	unk748 = (J2DPicture*)unk28->search('\0n_a');
	unk74C = (J2DPicture*)unk28->search('\0n_b');
	unk750 = (J2DPicture*)unk28->search('\0n_c');
}

void TCardLoad::setupTitleScreen() { }

void TCardLoad::setupScoreScreen() { }

void TCardLoad::loadAfter() { }

void TCardLoad::perform(u32, JDrama::TGraphics*) { }

void TCardLoad::titleDraw() { }

void TCardLoad::makeBuffer(J2DTextBox* text_box, int size)
{
	SMSMakeTextBuffer(text_box, size);
	text_box->setFont((JUTFont*)gpSystemFont);
}

void TCardLoad::changeMode(s32) { }

void TCardLoad::setMessage(J2DTextBox*, s32, int) { }

void TCardLoad::waitForChoice(TEProgress, TEProgress, int) { }

void TCardLoad::waitForChoiceBM(TEProgress, TEProgress, int) { }

void TCardLoad::waitForAnyKey(TEProgress) { }

void TCardLoad::waitForAnyKeyBM(TEProgress) { }

void TCardLoad::waitForStart(TEProgress) { }

void TCardLoad::drawMessage(TEProgress) { }

void TCardLoad::drawMessageBM(TEProgress) { }

void TCardLoad::selectBookmark(TEProgress, TEProgress, bool) { }

void TCardLoad::selectFunction() { }

void TCardLoad::setSelected(u8) { }

void TCardLoad::changeScene() { }
