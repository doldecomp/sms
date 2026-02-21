#include <GC2D/GCConsole2.hpp>
#include <GC2D/BoundPane.hpp>
#include <GC2D/BlendPane.hpp>
#include <GC2D/MessageLoader.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/HelpActor.hpp>
#include <GC2D/ExPane.hpp>
#include <Strategic/Strategy.hpp>
#include <System/Application.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/JUtility/JUTResFont.hpp>
#include <stdio.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

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
    , unk530(nullptr)
    , unk554(0)
    , unk558(0)
    , unk55C(0)
    , unk560(60)
    , unk562(5)
    , unk564(0.7f)
    , unk568(0.0f)
    , unk56C(1)
    , unk56D(1)
    , unk570(0)
{
	for (int i = 0; i < 2; ++i)
		unkE0[i] = 0;

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
		unk134[i] = new TBoundPane(unkB0, 'd_n1' + i);

	unk160 = new TExPane(unkB0, '\0d_0');
	unk174 = new TBoundPane(unkB0, 'l_ba');
	unk178 = new TBoundPane(unkB0, 'l_tx');
	unk1C4 = new TBoundPane(unkB0, '\0l_0');

	for (int i = 0; i < 9; ++i) {
		unk17C[i]     = unkB0[i].search('lm01');
		unk17C[9 + i] = unkB0[i].search('lm02');
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
	unk328 = unkB0->search('j_rq');
	unk32C = unkB0->search('j_ms');

	for (int i = 0; i < 20; ++i) {
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

	unk3B0 = unkB0->search('he_w');
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

	((J2DPicture*)unk414[2]->getPane())->changeTexture(unkE0[8]->mTexInfo, 0);

	unk428 = new TExPane(unkB0, '\0b_1');
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
	unk508 = ((J2DPicture*)unk458[6]->getPane())->mWhite;
	unk50C = ((J2DPicture*)unk458[7]->getPane())->mWhite;

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

void TGCConsole2::loadAfter() { }

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

void TGCConsole2::startCameraDemo() { }

void TGCConsole2::resetMoveTank() { }

void TGCConsole2::endCameraDemo() { }

void TGCConsole2::startAppearTank() { }

void TGCConsole2::startDisappearTank() { }

void TGCConsole2::startAppearCoin() { }

void TGCConsole2::startDisappearCoin() { }

void TGCConsole2::startInsertLife(int) { }

void TGCConsole2::resetLife(int) { }

void TGCConsole2::startAppearLife(int) { }

void TGCConsole2::startDisappearLife(int) { }

void TGCConsole2::startDownLeftBot() { }

void TGCConsole2::startUpLeftBot() { }

void TGCConsole2::startAppearTelop(bool) { }

void TGCConsole2::startDisappearTelop() { }

void TGCConsole2::startDisappearTimer() { }

void TGCConsole2::startAppearTimer(int, s32) { }

void TGCConsole2::startInsertTimer() { }

void TGCConsole2::startAppearJetBalloon(int, int) { }

void TGCConsole2::startInsertJetBalloon() { }

void TGCConsole2::startAppearRedCoin() { }

void TGCConsole2::pauseIn() { }

void TGCConsole2::pauseOut() { }

bool TGCConsole2::startDisappearBalloon(u32 param_1, bool param_2)
{
	if (!param_2 && unk3F4 == 0xffffffff && (param_1 != unk3E0 || unk3E4 != 0))
		return false;

	unk3B8->hide();
	unk34[20] = 0;
	unk10     = 4;
	return true;
}

bool TGCConsole2::startAppearBalloon(u32, bool) { }

void TGCConsole2::startDisappearStar() { }

void TGCConsole2::startAppearStar() { }

void TGCConsole2::drawWaterBack() { }

void TGCConsole2::startDisappearMario() { }

void TGCConsole2::startAppearMario(bool) { }

void TGCConsole2::processMoveNozzle() { }

void TGCConsole2::changeNum(TBlendPane*, int, int) { }

void TGCConsole2::setTimer(long) { }

void TGCConsole2::startMoveTimer(int param_1)
{
	unk34[22] = 1;
	unk518    = param_1 * 100;
}

void TGCConsole2::stopMoveTimer()
{
	if (unk34[22] == 0)
		return;

	unk34[22] = 0;
}

int TGCConsole2::getFinishedTime() { return unk4FC; }

void TGCConsole2::perform(u32, JDrama::TGraphics*) { }
