#include <GC2D/CardSave.hpp>
#include <stdio.h>
#include <dolphin/gx/GXCull.h>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <JSystem/J2D/J2DTextBox.hpp>
#include <JSystem/J2D/J2DPicture.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <System/Application.hpp>
#include <System/FlagManager.hpp>
#include <System/MarioGamePad.hpp>
#include <System/MarDirector.hpp>
#include <GC2D/MessageUtil.hpp>
#include <GC2D/PauseMenu2.hpp>
#include <GC2D/ExPane.hpp>
#include <MSound/MSound.hpp>

// rogue includes needed for matching sinit & bss
#include <System/DummyStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

extern JPAEmitterManager* gpEmitterManager4D2;

// Some sites spell the centred-size computation with the width read before
// the height; ExPane.hpp's setCenteredSize has the other order, which is the
// one TConsoleStr::processReady matches. The order is per site. The width is
// read from the rect's fields, not through getWidth(), so that the second
// read costs no dead temporary.
// waitForSelectOver wants the same order but the pair of getWidth() temps:
// with the field reads its frame comes out 8 bytes short.
static inline void setCenteredSizeWHh(TExPane* pane, s32 time, s32 target_w,
                                      s32 target_h, s32 initial_w,
                                      s32 initial_h)
{
	pane->setPaneSize(time, target_w, target_h, initial_w, initial_h);
	s32 initH = pane->mInitialBounds.getHeight();
	pane->setPaneOffset(
	    time, (pane->mInitialBounds.getWidth() - target_w) * 0.5f,
	    (initH - target_h) * 0.5f,
	    (pane->mInitialBounds.getWidth() - initial_w) * 0.5f,
	    (initH - initial_h) * 0.5f);
}

static inline J2DPane* cardSaveGetPane(TExPane* pane)
{
	J2DPane* p = pane->getPane();
	return p;
}

static inline void setCenteredSizeWr(TExPane* pane, s32 time, s32 target_w,
                                     s32 target_h, s32 initial_w,
                                     s32 initial_h)
{
	pane->setPaneSize(time, target_w, target_h, initial_w, initial_h);
	s32 initH = pane->mInitialBounds.getHeight();
	pane->setPaneOffset(
	    time,
	    (pane->mInitialBounds.x2 - pane->mInitialBounds.x1 - target_w) * 0.5f,
	    (initH - target_h) * 0.5f,
	    (pane->mInitialBounds.x2 - pane->mInitialBounds.x1 - initial_w) * 0.5f,
	    (initH - initial_h) * 0.5f);
}

u32 TCardSave::cMessageID[] = {
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE,        0x11,       0x14,
	0x5,        0x6,        0x7,        0x18,       0x22,       0x16,
	0x10,       0xF,        0x2,        0xB,        0x12,       0x21,
	0x15,       0x1B,       0x3,        0x9,        0x1A,       0x23,
	0x17,       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x4,        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0x13,       0xD,        0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA,        0x8,        0xC,
};

TEProgress TCardSave::changeMode(s32 param_1)
{
	TEProgress result = PROGRESS_UNK4;
	switch (param_1) {
	case CARD_RESULT_NOCARD:
		result = PROGRESS_UNK3;
		break;
	case CARD_RESULT_FATAL_ERROR:
	case CARD_RESULT_WRONGDEVICE:
		result = PROGRESS_UNK4;
		break;
	case CARD_RESULT_IOERROR:
		result = PROGRESS_UNK2D;
		break;
	case -0x100:
		result = PROGRESS_UNK5;
		break;
	case CARD_RESULT_BROKEN:
		result = PROGRESS_UNK6;
		break;
	case CARD_RESULT_ENCODING:
		result = PROGRESS_UNK7;
		break;
	case CARD_RESULT_INSSPACE:
		result = PROGRESS_UNKC;
		break;
	case CARD_RESULT_NOENT:
		result = PROGRESS_UNKD;
		break;
	case CARD_RESULT_NOFILE:
		result = PROGRESS_UNKE;
		break;
	}
	return result;
}

TCardSave::TCardSave(const char* name, bool param_2)
    : JDrama::TViewObj(name)
    , unk18(param_2)
    , unk48(nullptr)
    , unk5C(0)
    , unk60(0)
    , unkA0(nullptr)
    , unkA4(nullptr)
    , unkA8(nullptr)
    , unkBC(0)
    , unkC0(0)
    , unkE0(nullptr)
    , unkF4(nullptr)
    , unkF8(nullptr)
    , unkFC(nullptr)
    , unk100(nullptr)
    , unk124(nullptr)
    , unk128(nullptr)
    , unk12C(nullptr)
    , unk130(nullptr)
    , unk134(nullptr)
    , unk160(nullptr)
    , unk174(nullptr)
    , unk178(nullptr)
    , unk2DC(1)
    , unk2DE(0)
    , unk2DF(1)
    , unk2E0(0)
    , unk2E8(0)
    , unk2E9(0)
    , unk2FC(0)
    , unk300(200)
    , unk302(200)
    , unk304(1)
    , unk310(PROGRESS_UNK1)
    , unk314(PROGRESS_UNK1)
{
}

void TCardSave::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	initData(SMSGetMarDirector()->getGamePad());
}

static inline void CardSaveInitRow(TCardSave* p, u32 upperTag, u32 lowerTag,
                                   u32 msg)
{
	J2DTextBox* upper = (J2DTextBox*)p->unk14->search(upperTag);
	J2DTextBox* lower = (J2DTextBox*)p->unk14->search(lowerTag);
	p->makeBuffer(upper, 0x80);
	p->makeBuffer(lower, 0x80);
	p->setMessage(upper, 0x80, msg);
	p->setMessage(lower, 0x80, msg);
}

void TCardSave::initData(TMarioGamePad* param_1)
{

	if (unk18) {
		JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("endsave");
		unk14            = new J2DSetScreen("save_end.blo", arch);
	} else {
		JKRArchive* arch = (JKRArchive*)JKRFileLoader::getVolume("game_6");
		unk14            = new J2DSetScreen("save.blo", arch);
	}

	unk14->setCullBack(GX_CULL_BACK);

	for (int i = 0; i < 10; ++i) {
		char acStack_48[0x28];
		if (unk18) {
			snprintf(acStack_48, 0x28, "/endsave/timg/coin_number_%d.bti", i);
		} else {
			snprintf(acStack_48, 0x28, "/game_6/timg/coin_number_%d.bti", i);
		}

		// TODO: retail copies the new texture into a second register
		// (`mr r28, r24`) for the inlined constructor's storeTIMG call;
		// neither this spelling nor the direct store reproduces it.
		JUTTexture* texture
		    = new JUTTexture((ResTIMG*)JKRGetResource(acStack_48));
		unk1C[i] = texture;
	}
	unk48 = new TExPane(unk14, 0x775f30);
	unk4C = unk48->getPane()->getBounds();

	unkA0 = (J2DTextBox*)unk14->search(0x6d5f3561);
	unkA4 = (J2DTextBox*)unk14->search(0x6d5f3562);
	makeBuffer(unkA0, 0x200);
	makeBuffer(unkA4, 0x200);

	unkA8 = new TExPane(unk14, 0x775f33);
	unkAC = unkA8->getPane()->getBounds();
	unkC4 = new TExPane(unk14, 0x775f33);
	unkC8 = unkC4->getPane()->getBounds();

	unkD8 = (J2DTextBox*)unk14->search(0x6d5f3761);
	unkDC = (J2DTextBox*)unk14->search(0x6d5f3762);
	makeBuffer(unkD8, 0x200);
	makeBuffer(unkDC, 0x200);
	unkE0 = new TExPane(unk14, 0x775f31);
	unkE4 = unkE0->getPane()->getBounds();

	unkF4 = (J2DTextBox*)unk14->search(0x6d5f3261);
	unkF8 = (J2DTextBox*)unk14->search(0x6d5f3262);
	makeBuffer(unkF4, 0x200);
	makeBuffer(unkF8, 0x200);
	unkFC  = new TExPane(unk14, 0x735f36);
	unk104 = unkFC->getPane()->getBounds();

	unk100 = new TExPane(unk14, 0x735f37);
	unk114 = unk100->getPane()->getBounds();

	unk124 = (J2DTextBox*)unk14->search(0x6d5f3361);
	unk128 = (J2DTextBox*)unk14->search(0x6d5f3362);
	makeBuffer(unk124, 0x20);
	makeBuffer(unk128, 0x20);
	unk12C = (J2DTextBox*)unk14->search(0x6d5f3461);
	unk130 = (J2DTextBox*)unk14->search(0x6d5f3462);
	makeBuffer(unk12C, 0x200);
	makeBuffer(unk130, 0x200);

	unk134 = unk14->search(0x665f7369);
	unk138 = unk14->search(0x665f7368);
	unk13C = (J2DPicture*)unk14->search(0x6e5f3461);
	unk140 = (J2DPicture*)unk14->search(0x6e5f3462);
	unk144 = (J2DPicture*)unk14->search(0x6e5f3463);
	unk148 = (J2DPicture*)unk14->search(0x6e5f3361);
	unk14C = (J2DPicture*)unk14->search(0x6e5f3362);
	unk150 = unk14->search(0x745f30);

	for (int i = 0; i < 3; ++i)
		unk154[i] = unk14->search(0x745f31 + i);

	unk160 = new TExPane(unk14, 0x775f32);
	unk164 = unk160->getPane()->getBounds();

	unk174 = (J2DTextBox*)unk14->search(0x6d5f3661);
	unk178 = (J2DTextBox*)unk14->search(0x6d5f3662);
	makeBuffer(unk174, 0x200);
	makeBuffer(unk178, 0x200);
	unk17C = new TExPane(unk14, 0x775f34);
	unk180 = unk17C->getPane()->getBounds();

	unk190 = (J2DTextBox*)unk14->search(0x736d3061);
	unk194 = (J2DTextBox*)unk14->search(0x736d3062);
	makeBuffer(unk190, 0x200);
	makeBuffer(unk194, 0x200);

	for (int i = 0; i < 2; ++i) {
		unk198[i][0] = (J2DTextBox*)unk14->search(0x736d3161 + 0x100 * i);
		unk198[i][0]->setFont((JUTFont*)gpSystemFont);

		unk198[i][1] = (J2DTextBox*)unk14->search(0x736d3162 + 0x100 * i);
		unk198[i][1]->setFont((JUTFont*)gpSystemFont);

		unk1A8[i][0] = unk14->search(0x73633161 + 0x100 * i);
		unk1A8[i][1] = unk14->search(0x73633162 + 0x100 * i);
	}
	unk1B8 = unk1A8[0][0]->getBounds().x1;

	unk1BC = new TExPane(unk14, 0x775f35);
	unk1C0 = unk1BC->getPane()->getBounds();

	unk200 = (J2DTextBox*)unk14->search(0x736d3661);
	unk204 = (J2DTextBox*)unk14->search(0x736d3662);
	makeBuffer(unk200, 0x200);
	makeBuffer(unk204, 0x200);

	unk208 = (J2DTextBox*)unk14->search(0x6d5f3861);
	unk20C = (J2DTextBox*)unk14->search(0x6d5f3862);
	makeBuffer(unk208, 0x20);
	makeBuffer(unk20C, 0x20);

	unk210 = unk14->search(0x665f736b);
	unk214 = unk14->search(0x665f736a);
	unk218 = unk14->search(0x6e5f3661);
	unk21C = unk14->search(0x6e5f3662);
	unk220 = unk14->search(0x6e5f3663);
	unk224 = unk14->search(0x6e5f3561);
	unk228 = unk14->search(0x6e5f3562);
	unk22C = unk14->search(0x745f34);

	for (int i = 0; i < 3; ++i)
		unk230[i] = unk14->search(0x745f35 + i);

	for (int i = 0; i < 3; ++i) {
		unk1E8[i][0] = (J2DTextBox*)unk14->search(0x736d3361 + 0x100 * i);
		unk1E8[i][0]->setFont((JUTFont*)gpSystemFont);
		unk1E8[i][1] = (J2DTextBox*)unk14->search(0x736d3362 + 0x100 * i);
		unk1E8[i][1]->setFont((JUTFont*)gpSystemFont);

		unk1D0[i][0] = (J2DTextBox*)unk14->search(0x73633361 + 0x100 * i);
		unk1D0[i][1] = (J2DTextBox*)unk14->search(0x73633362 + 0x100 * i);
	}
	unk23C = unk1D0[0][0]->getBounds().x1;

	unk240 = new TExPane(unk14, 0x775f36);
	unk244 = unk240->getPane()->getBounds();

	for (int i = 0; i < 2; ++i) {
		unk254[i][0] = (J2DTextBox*)unk14->search(0x73633661 + 0x100 * i);
		unk254[i][1] = (J2DTextBox*)unk14->search(0x73633662 + 0x100 * i);
		unk264[i]    = (J2DTextBox*)unk14->search(0x736d3762 + 0x100 * i);

		((J2DTextBox*)unk14->search(0x736d3761 + 0x100 * i))
		    ->setFont((JUTFont*)gpSystemFont);
		unk264[i]->setFont((JUTFont*)gpSystemFont);
	}

	unk26C = unk254[0][0]->getBounds().x1;

	unk14->search(0x6d61736b)->hide();
	unk2E4 = JKRGetResource("/common/2d/savemessage.bmg");
	if (!unk18)
		unk2D8 = JDrama::TNameRefGen::search<TPauseMenu2>("ポーズメニュー");

	// The default caption of every save-menu row. The panes are not kept:
	// the rows that can change their text later look them up again. There is
	// no `sm6` group. Each row is its own inline level: retail gives every
	// row the same register pair, which one shared pair of locals does not.
	CardSaveInitRow(this, 'sm1a', 'sm1b', 0x1C);
	CardSaveInitRow(this, 'sm2a', 'sm2b', 0x1D);
	CardSaveInitRow(this, 'sm3a', 'sm3b', 0x1C);
	CardSaveInitRow(this, 'sm4a', 'sm4b', 0x1E);
	CardSaveInitRow(this, 'sm5a', 'sm5b', 0x1D);
	CardSaveInitRow(this, 'sm7a', 'sm7b', 0x1F);
	CardSaveInitRow(this, 'sm8a', 'sm8b', 0x20);

	unk270 = param_1;
}

// TODO: UNUSED, 0xe8 bytes in the map. Dead in the shipped game, so there is
// no call site to read the parameters off; only the mangled signature
// (u32, u32, int) is known. Left as a stub so the symbol exists.
void TCardSave::changeMessage(u32, u32, int) { }

void TCardSave::init(int param_1)
{
	unk310 = PROGRESS_UNK0;
	unk2DF = 0;
	unk308 = param_1;
	unk2F8 = 0;
	unk2F9 = 1;
	if (unk308 == 7 || unk308 == 8 || unk308 == 9) {
		unk14->search('mask')->show();
		unk14->search('mask')->setAlpha(255);
	} else {
		unk14->search('mask')->hide();
	}
}

void TCardSave::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (unk2DF)
		return;

	if (cue & CUE_MOVE)
		execMovement_();

	if (cue & CUE_DRAW)
		execIssueGX_(graphics);
}

void TCardSave::makeBuffer(J2DTextBox* param_1, int size)
{
	SMSMakeTextBuffer(param_1, size);
	param_1->setFont((JUTFont*)gpSystemFont);
}

void TCardSave::setMessage(J2DTextBox* text_box, s32 param_2, u32 param_3)
{
	strncpy(text_box->getStringPtr(), SMSGetMessageData(unk2E4, param_3),
	        param_2);
}

/// Copies a message into a text box, wrapping the balloon colour markers
/// (single ASCII bytes) in the J2D escape sequences that actually change the
/// glyph colour. Unrecognised bytes are copied through untouched.
void TCardSave::setMessageC(J2DTextBox* text_box, s32 message_id, u32 size)
{
	JSUMemoryInputStream in(SMSGetMessageData(unk2E4, (u16)message_id), size);
	JSUMemoryOutputStream out(text_box->getStringPtr(), size);
	// TODO: 99.4%. Two residues, both slot-offset only (the frame size is
	// already exact): the ROM narrows the id with `clrlwi r4, r5, 16` at the
	// call while we hoist it into the prologue and move it, and the ROM's
	// buffer starts four bytes lower, so our body carries one 4-byte inline
	// temporary it does not have. `in.read(&c, 1)` instead of readU8(),
	// per-site `u8` copies instead of write(u8), a 260-byte buffer and an
	// extra named local before the buffer were all tried and are worse.
	char buffer[256];

	while (!in.isDrained() && !out.isDrained()) {
		u8 c = in.readU8();
		JUtility::TColor color;

		switch (c) {
		case 0x1A: {
			// A BMG escape: its second byte is the total tag length.
			in.skip(in.readU8() - 2);
			continue;
		}

		case 0x00:
			out.write(c);
			return;

		case 0x0A:
			out.write(c);
			continue;
		}

		bool colored = true;
		switch (c) {
		case '@':
			color.set(0x64, 0xFF, 0x64, 0xFF);
			break;
		case '#':
			color.set(0xFF, 0xA0, 0x64, 0xFF);
			break;
		case '%':
			color.set(0xFF, 0xFF, 0x00, 0xFF);
			break;
		case '+':
		case '<':
		case '>':
		case 0xA5:
			color.set(0xDC, 0xDC, 0xDC, 0xFF);
			break;
		case '$':
			color.set(0x6E, 0xE6, 0xFF, 0xFF);
			break;
		default:
			colored = false;
			break;
		}

		if (colored) {
			snprintf(buffer, 0xFF,
			         "\033GM[0]\033CC[%02x%02x%02x]\033SH[3]\033CD[4]", color.r,
			         color.g, color.b);
			out.write(buffer, 0x1D);
		}

		out.write(c);

		if (colored) {
			snprintf(buffer, 0xFF, "\033GM[0]\033CC\033FX\033FY\033SH\033CU[4]");
			out.write(buffer, 0x18);
		}
	}
}

s8 TCardSave::waitForStop(TEProgress param_1)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unkA0, 0x200, getCurMessageID());
		setMessage(unkA4, 0x200, getCurMessageID());
		setMessage(unkD8, 0x200, 1);
		setMessage(unkDC, 0x200, 1);

		unkA4->hide();
		unk48->getPane()->show();
		unk48->setCenteredSize(20, unk4C.getWidth(), unk4C.getHeight(), 0, 0);

		unkDC->hide();
		unkC4->setCenteredSize(20, unkC8.getWidth(), unkC8.getHeight(), 0, 0);

		if (unk310 == PROGRESS_UNK4 || unk310 == PROGRESS_UNK3
		    || unk310 == PROGRESS_UNK5 || unk310 == PROGRESS_UNKC
		    || unk310 == PROGRESS_UNKD || unk310 == PROGRESS_UNK2D
		    || unk310 == PROGRESS_UNK35) {
			unkC4->getPane()->show();
		} else {
			unkC4->getPane()->hide();
		}

		unk10 = 1;
		break;

	case 1: {
		u8 done = 1;
		done &= unk48->update();
		done &= unkC4->update();
		if (done) {
			unk10 = 2;
			unkA4->show();
			unkDC->show();
		}
		break;
	}

	case 2:
		if (unk270->mEnabledFrameMeaning & 0x20) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);

			unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
			                       unk4C.getHeight());
			unkC4->setCenteredSize(20, 0, 0, unkC8.getWidth(),
			                       unkC8.getHeight());
			unkA4->hide();
			unkDC->hide();
			unk10 = 3;
		}
		break;

	case 3: {
		u8 done = 1;
		done &= unk48->update();
		done &= unkC4->update();
		if (done) {
			unk48->getPane()->hide();
			unkC4->getPane()->hide();
			unk10 = 5;
		}
		break;
	}

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

void TCardSave::endWaitForChoice()
{
	if (unk10 != 2)
		return;

	unk134->hide();
	unk138->hide();
	unk128->hide();
	unkF8->hide();
	unk130->hide();
	unk150->hide();
	unkFC->getPane()->hide();
	unk100->getPane()->hide();
	unk10 = 3;
	setCenteredSizeWr(unkE0, 20, 0, 0, unkE4.getWidth(), unkE4.getHeight());
}

// TODO: literal-pool order. The target asks for 1.0f (@4275) well before
// this function's -0.5f/1.5f/0.9f trio (@5958-@5960); ours requests 1.0f
// last of the four. Order only.
s8 TCardSave::waitForChoice(TEProgress param_1, TEProgress param_2, s8 param_3)
{
	// TODO: frame 0x3F8 vs original 0x428; centered-size registers,
	// particle-position scheduling and color/conversion slots still differ.
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unkF4, 0x200, getCurMessageID());
		setMessage(unkF8, 0x200, getCurMessageID());

		unk134->hide();
		unk138->hide();
		unk128->hide();
		unkF8->hide();
		unk130->hide();
		unk150->hide();
		unkFC->getPane()->hide();
		unk100->getPane()->hide();

		unkE0->getPane()->show();
		unkE0->setCenteredSize(20, unkE4.getWidth(), unkE4.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (unkE0->update()) {
			unkF8->show();
			unkFC->getPane()->show();
			unk100->getPane()->show();

			unk2E9 = param_3;
			if (param_3 == 0) {
				((J2DPicture*)unkFC->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
				((J2DPicture*)unk100->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
			} else {
				((J2DPicture*)unkFC->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
				((J2DPicture*)unk100->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
			}

			unkFC->updateCenteredSize(1, unk104.getWidth(), unk104.getHeight());
			unk100->updateCenteredSize(1, unk114.getWidth(),
			                           unk114.getHeight());
			unkFC->update();
			unk100->update();

			unk2FC = 0;
			unk10  = 2;
		}
		break;

	case 2: {
		s8 old    = unk2E9;
		u32 input = unk270->mEnabledFrameMeaning;

		if (input & 0x20) {
			if (unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			if (unk2E9 == 0) {
				gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			} else {
				gpMSound->startSoundSystemSE(0x481DU, 0, nullptr, 0);
			}

			unkF8->hide();
			unk150->hide();
			unkFC->getPane()->hide();
			unk100->getPane()->hide();
			unk10 = 3;
			unkE0->setCenteredSize(20, 0, 0, unkE4.getWidth(),
			                       unkE4.getHeight());
		} else if (input & 0x40) {
			if (unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			gpMSound->startSoundSystemSE(0x481DU, 0, nullptr, 0);

			unkF8->hide();
			unk150->hide();
			unkFC->getPane()->hide();
			unk100->getPane()->hide();
			unk10 = 3;
			unkE0->setCenteredSize(20, 0, 0, unkE4.getWidth(),
			                       unkE4.getHeight());
			unk2E9 = 1;
		} else if (input & 0x8) {
			unk2E9 = 0;
		} else if (input & 0x10) {
			unk2E9 = 1;
		}

		TExPane* selectedPane;
		JUTRect selectedRect;
		if (unk2E9 == 0) {
			selectedPane = unkFC;
			selectedRect = unk104;
		} else {
			selectedPane = unk100;
			selectedRect = unk114;
		}

		if (unk2FC == 4) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f,
			                              selectedRect.getWidth(),
			                              selectedRect.getHeight());
			if (!unk18) {
				JUTRect bounds = selectedPane->getPane()->mGlobalBounds;
				JGeometry::TVec3<f32> pos;
				pos.x = bounds.x1 + bounds.getWidth() * 0.5f;
				pos.y = bounds.y1 + bounds.getHeight() * 0.5f;
				pos.z = 0.0f;
				gpEmitterManager4D2->createEmitter(pos, 0x1FA, nullptr,
				                                   nullptr);

				unk44 = gpEmitterManager4D2->unkC8[0][0];
				unk44->setRotation(0, 0, DEG2SHORTANGLE(12));
				unk44->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
			}
		} else if (unk2FC == 44) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth(),
			                              selectedRect.getHeight(),
			                              selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f);
		}

		s32 thing = unk2FC < 40 ? unk2FC : 80 - unk2FC;
		thing     = (s32)(thing * 255.0f / 40.0f);
		((J2DPicture*)selectedPane->getPane())
		    ->setWhite((thing << 24) + 0xFF00FF);
		selectedPane->update();

		unk2FC += 1;
		if (unk2FC > 80)
			unk2FC = 0;

		if (old != unk2E9) {
			if (!unk18 && unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			unk2FC = 0;

			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			unkFC->updateCenteredSize(1, unk104.getWidth(), unk104.getHeight());
			unk100->updateCenteredSize(1, unk114.getWidth(),
			                           unk114.getHeight());
			unkFC->update();
			unk100->update();

			if (unk2E9 == 0) {
				((J2DPicture*)unkFC->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
				((J2DPicture*)unk100->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
			} else {
				((J2DPicture*)unkFC->getPane())
				    ->setWhite(JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF));
				((J2DPicture*)unk100->getPane())
				    ->setWhite(JUtility::TColor(0, 0xFF, 0, 0xFF));
			}
		}
		break;
	}

	case 3:
		if (unkE0->update()) {
			unkE0->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = unk2E9 == 0 ? param_1 : param_2;
		result = unk2E9;
		break;
	}

	return result;
}

s8 TCardSave::waitForChoiceBM(TEProgress param_1, TEProgress param_2,
                              s8 param_3)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		if (unk2EA == -1) {
			unk10 = 5;
		}

		setMessage(unk12C, 0x200, getCurMessageID());
		setMessage(unk130, 0x200, getCurMessageID());

		unk134->hide();
		unk138->hide();
		unk128->hide();
		unkF8->hide();
		unk130->hide();
		unk150->hide();
		unkFC->getPane()->hide();
		unk100->getPane()->hide();
		unk154[0]->hide();
		unk154[1]->hide();
		unk154[2]->hide();

		unkE0->getPane()->show();
		unkE0->setCenteredSize(20, unkE4.getWidth(), unkE4.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (unkE0->update()) {
			unk154[unk2EA]->show();
			unk150->show();
			unk130->show();

			if (unk310 == PROGRESS_UNK17) {
				unk128->hide();
				u16 score = TFlagManager::getInstance()->getFlag(0x40000);
				if (score < 100) {
					unk148->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
					unk14C->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
					unk134->hide();
					unk138->show();
				} else {
					unk13C->changeTexture(unk1C[score / 100]->getTexInfo(), 0);
					score -= 100;
					unk140->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
					unk144->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
					unk134->show();
					unk138->hide();
				}
			} else {
				if (getBookmarkInfo().unk0 == 1) {
					setMessage(unk124, 0x14, 0);
					setMessage(unk128, 0x14, 0);
					unk128->show();
					unk134->hide();
					unk138->hide();
				} else if (getBookmarkInfo().unk18 == 0) {
					setMessage(unk124, 0x14, 0x19);
					setMessage(unk128, 0x14, 0x19);
					unk128->show();
					unk134->hide();
					unk138->hide();
				} else {
					if (getBookmarkInfo().unk1C >= 100) {
						unk134->show();
						unk138->hide();
					} else {
						unk134->hide();
						unk138->show();
					}
					unk128->hide();
				}
			}

			unkFC->getPane()->show();
			unk100->getPane()->show();
			unkFC->updateCenteredSize(1, unk104.getWidth(), unk104.getHeight());
			unk100->updateCenteredSize(1, unk114.getWidth(),
			                           unk114.getHeight());
			unkFC->update();
			unk100->update();
			unk2E9 = param_3;

			if (param_3 == 0) {
				((J2DPicture*)unkFC->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
				((J2DPicture*)unk100->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
			} else {
				((J2DPicture*)unkFC->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
				((J2DPicture*)unk100->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
			}

			unk10 = 2;
		}
		break;

	case 2: {
		s8 old    = unk2E9;
		u32 input = unk270->mEnabledFrameMeaning;

		if (input & 0x20) {
			if (unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			if (unk2E9 == 0) {
				gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			} else {
				gpMSound->startSoundSystemSE(0x481DU, 0, nullptr, 0);
			}

			unk134->hide();
			unk138->hide();
			unk128->hide();
			unkF8->hide();
			unk130->hide();
			unk150->hide();
			unkFC->getPane()->hide();
			unk100->getPane()->hide();
			unk10 = 3;
			unkE0->setCenteredSize(20, 0, 0, unkE4.getWidth(),
			                       unkE4.getHeight());
		} else if (input & 0x8) {
			unk2E9 = 0;
		} else if (input & 0x10) {
			unk2E9 = 1;
		}

		TExPane* selectedPane;
		JUTRect selectedRect;
		if (unk2E9 == 0) {
			selectedPane = unkFC;
			selectedRect = unk104;
		} else {
			selectedPane = unk100;
			selectedRect = unk114;
		}

		if (unk2FC == 4) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f,
			                              selectedRect.getWidth(),
			                              selectedRect.getHeight());
			if (!unk18) {
				JUTRect bounds = selectedPane->getPane()->getGlobalBounds();
				JGeometry::TVec3<f32> pos;
				pos.x = bounds.x1 + bounds.getWidth() * 0.5f;
				pos.y = bounds.y1 + bounds.getHeight() * 0.5f;
				pos.z = 0.0f;
				gpEmitterManager4D2->createEmitter(pos, 0x1FA, nullptr,
				                                   nullptr);

				unk44 = gpEmitterManager4D2->unkC8[0][0];
				unk44->setRotation(0, 0, DEG2SHORTANGLE(12));
				unk44->setEmitterScale(JGeometry::TVec3<f32>(0.9f, 1.0f, 0.1f));
			}
		} else if (unk2FC == 44) {
			selectedPane->setCenteredSize(40, selectedRect.getWidth(),
			                              selectedRect.getHeight(),
			                              selectedRect.getWidth() * 1.5f,
			                              selectedRect.getHeight() * 1.5f);
		}

		s32 thing = unk2FC < 40 ? unk2FC : 80 - unk2FC;
		thing     = (s32)(thing * 255.0f / 40.0f);
		((J2DPicture*)selectedPane->getPane())->mWhite
		    = (thing << 24) + 0xFF00FF;
		selectedPane->update();

		unk2FC += 1;
		if (unk2FC > 80)
			unk2FC = 0;

		if (old != unk2E9) {
			if (!unk18 && unk44 != nullptr)
				gpEmitterManager4D2->forceDeleteEmitter(unk44);

			unk2FC = 0;

			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			unkFC->updateCenteredSize(1, unk104.getWidth(), unk104.getHeight());
			unk100->updateCenteredSize(1, unk114.getWidth(),
			                           unk114.getHeight());
			unkFC->update();
			unk100->update();
			if (unk2E9 == 0) {
				((J2DPicture*)unkFC->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
				((J2DPicture*)unk100->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
			} else {
				((J2DPicture*)unkFC->getPane())->mWhite
				    = JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF);
				((J2DPicture*)unk100->getPane())->mWhite
				    = JUtility::TColor(0, 0xFF, 0, 0xFF);
			}
		}
		break;
	}

	case 3:
		if (unkE0->update()) {
			unkE0->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = unk2E9 == 0 ? param_1 : param_2;
		result = unk2E9;
		break;
	}

	return result;
}

void TCardSave::endDrawMessage()
{
	if (unk10 != 2)
		return;

	unk134->hide();
	unk138->hide();
	unk128->hide();
	unkF8->hide();
	unk130->hide();
	unk150->hide();
	unkFC->getPane()->hide();
	unk100->getPane()->hide();
	unk10 = 3;
	unkE0->setCenteredSize(20, 0, 0, unkE4.getWidth(), unkE4.getHeight());
}

s8 TCardSave::drawMessage(TEProgress param_1)
{
	s8 result = -1;
	switch (unk10) {
	case 0:
		setMessage(unk174, 0x200, getCurMessageID());
		setMessage(unk178, 0x200, getCurMessageID());
		unk178->hide();
		unk160->mPane->show();
		unk160->setCenteredSize(20, unk164.getWidth(), unk164.getHeight(), 0,
		                        0);
		unk10 = 1;
		break;

	case 1:
		if (unk160->update()) {
			unk178->show();
			unk10 = 2;
		}
		break;

	case 3:
		if (unk160->update()) {
			unk160->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}
	return result;
}

s8 TCardSave::drawMessageBM(TEProgress param_1)
{

	s8 result = -1;

	switch (getUnk10()) {
	case 0:
		if (unk2EA == -1) {
			unk10 = 5;
		}

		setMessage(unk12C, 0x200, getCurMessageID());
		setMessage(unk130, 0x200, getCurMessageID());

		unk134->hide();
		unk138->hide();
		unk128->hide();
		unkF8->hide();
		unk130->hide();
		unk150->hide();
		unkFC->getPane()->hide();
		unk100->getPane()->hide();
		unk154[0]->hide();
		unk154[1]->hide();
		unk154[2]->hide();

		unkE0->getPane()->show();
		unkE0->setCenteredSize(20, unkE4.getWidth(), unkE4.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (unkE0->update()) {
			unk130->show();
			unk154[unk2EA]->show();
			unk150->show();

			if (getUnk308() == 0 || getUnk308() == 6 || getUnk308() == 8) {
				if (getBookmarkInfo().unk0 == 1) {
					setMessage(unk124, 0x14, 0);
					setMessage(unk128, 0x14, 0);
					unk128->show();
					unk134->hide();
					unk138->hide();
				} else if (getBookmarkInfo().unk18 == 0) {
					setMessage(unk124, 0x14, 0x19);
					setMessage(unk128, 0x14, 0x19);
					unk128->show();
					unk134->hide();
					unk138->hide();
				} else {
					unk128->hide();
					if (getBookmarkInfo().unk1C >= 100) {
						unk134->show();
						unk138->hide();
					} else {
						unk134->hide();
						unk138->show();
					}
				}
			}

			unk10 = 2;
		}
		break;

	case 3:
		if (unkE0->update()) {
			unkE0->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardSave::waitForAnyKey(TEProgress param_1)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unk174, 0x200, getCurMessageID());
		setMessage(unk178, 0x200, getCurMessageID());
		unk178->hide();
		unk160->mPane->show();
		unk160->setCenteredSize(20, unk164.getWidth(), unk164.getHeight(), 0,
		                        0);
		unk10 = 1;
		break;

	case 1:
		if (unk160->update()) {
			unk178->show();
			unk10 = 2;
		}
		break;

	case 2:
		if (unk2E0 > 300) {
			unk178->hide();
			setCenteredSizeWr(unk160, 20, 0, 0, unk164.getWidth(),
			                  unk164.getHeight());
			unk10 = 3;
		} else {
			unk2E0++;
		}
		break;

	case 3:
		if (unk160->update()) {
			unk160->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

s8 TCardSave::waitForSelectOver()
{
	s8 result = -1;

	switch (getUnk10()) {
	case 0:
		unk240->getPane()->show();
		unk240->setCenteredSize(20, unk244.getWidth(), unk244.getHeight(), 0,
		                        0);
		unk264[0]->hide();
		unk254[0][0]->hide();
		unk254[0][1]->hide();
		unk264[1]->hide();
		unk254[1][0]->hide();
		unk254[1][1]->hide();
		unk10 = 1;
		break;

	case 1:
		if (unk240->update()) {
			unk10  = 2;
			unk2E9 = 0;
			unk264[0]->show();
			unk264[1]->show();
			unk254[0][0]->show();
			unk254[0][1]->show();
		}
		break;

	case 2: {
		s8 oldSelect = getUnk2E9();
		u32 input    = unk270->mEnabledFrameMeaning;

		if (input & 0x20) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			unk10 = 4;
		} else if (input & 0x2) {
			unk2E9 = 0;
		} else if (input & 0x4) {
			unk2E9 = 1;
		}

		if (oldSelect != getUnk2E9()) {
			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			unk254[oldSelect][0]->hide();
			unk254[oldSelect][1]->hide();
			unk254[getUnk2E9()][0]->show();
			unk254[getUnk2E9()][1]->show();
		}
		break;
	}

	case 4:
		setCenteredSizeWHh(unk240, 20, 0, 0, unk244.getWidth(),
		                  unk244.getHeight());
		unk264[0]->hide();
		unk264[1]->hide();
		unk254[getUnk2E9()][0]->hide();
		unk254[getUnk2E9()][1]->hide();
		unk10 = 3;
		break;

	case 3:
		if (unk240->update()) {
			unk240->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		if (getUnk2E9() == 0) {
			unk310 = PROGRESS_UNK1;
		} else {
			unk310 = PROGRESS_UNK19;
		}
		result = getUnk2E9();
		break;
	}

	return result;
}

s8 TCardSave::waitForSelect2(TEProgress param_1, TEProgress param_2)
{
	s8 result = -1;

	switch (getUnk10()) {
	case 0: {
		u32 messageID = cMessageID[unk310];
		switch (getUnk308()) {
		case 1:
			messageID = 0x24;
			break;
		case 2:
			messageID = 0x25;
			break;
		case 3:
			messageID = 0x26;
			break;
		case 4:
			messageID = 0x27;
			break;
		case 5:
			messageID = 0x28;
			break;
		}

		setMessageC(unk190, messageID, 0x200);
		setMessage(unk194, 0x200, (u16)messageID);

		unk194->hide();
		unk17C->getPane()->show();
		unk17C->setCenteredSize(20, unk180.getWidth(), unk180.getHeight(), 0,
		                        0);

		unk198[0][1]->hide();
		unk1A8[0][0]->hide();
		unk1A8[0][1]->hide();
		unk198[1][1]->hide();
		unk1A8[1][0]->hide();
		unk1A8[1][1]->hide();

		unk10 = 1;
		break;
	}

	case 1:
		if (unk17C->update()) {
			unk194->show();
			unk10  = 2;
			unk2E9 = 0;

			unk198[0][1]->show();
			unk198[1][1]->show();
			unk1A8[0][0]->show();
			unk1A8[0][1]->show();

			unk2F8 = 0;
			unk2F9 = 1;
		}
		break;

	case 2: {
		s8 oldSelect = getUnk2E9();
		u32 input    = unk270->mEnabledFrameMeaning;

		if (input & 0x20) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			unk10 = 4;
		} else if (input & 0x2) {
			unk2E9 = 0;
		} else if (input & 0x4) {
			unk2E9 = 1;
		}

		if (oldSelect != getUnk2E9()) {
			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			unk1A8[oldSelect][0]->hide();
			unk1A8[oldSelect][1]->hide();
			unk1A8[getUnk2E9()][0]->show();
			unk1A8[getUnk2E9()][1]->show();

			unk2F8 = 0;
			unk2F9 = 1;
		}
		break;
	}

	case 4:
		unk194->hide();
		setCenteredSizeWr(unk17C, 20, 0, 0, unk180.getWidth(),
		                  unk180.getHeight());

		unk198[0][1]->hide();
		unk198[1][1]->hide();
		unk1A8[getUnk2E9()][0]->hide();
		unk1A8[getUnk2E9()][1]->hide();

		unk10 = 3;
		break;

	case 3:
		if (unk17C->update()) {
			unk17C->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		if (getUnk2E9() == 0) {
			unk310 = param_1;
		} else {
			unk310 = param_2;
		}
		result = getUnk2E9();
		break;
	}

	return result;
}

s8 TCardSave::waitForSelect3(TEProgress param_1, TEProgress param_2,
                             TEProgress param_3)
{
	s8 result = -1;

	switch (getUnk10()) {
	case 0:
		cardSaveGetPane(unk1BC)->show();
		unk1BC->setCenteredSize(20, unk1C0.getWidth(), unk1C0.getHeight(), 0,
		                        0);

		unk204->hide();
		setMessage(unk200, 0x200, 0x29);
		setMessage(unk204, 0x200, 0x29);

		unk1E8[0][1]->hide();
		unk1D0[0][0]->hide();
		unk1D0[0][1]->hide();
		unk1E8[1][1]->hide();
		unk1D0[1][0]->hide();
		unk1D0[1][1]->hide();
		unk1E8[2][1]->hide();
		unk1D0[2][0]->hide();
		unk1D0[2][1]->hide();

		unk230[0]->hide();
		unk230[1]->hide();
		unk230[2]->hide();
		unk20C->hide();
		unk210->hide();
		unk214->hide();
		unk22C->hide();

		unk10 = 1;
		break;

	case 1:
		if (unk1BC->update()) {
			unk10  = 2;
			unk2E9 = 0;

			unk1E8[0][1]->show();
			unk1E8[1][1]->show();
			unk1E8[2][1]->show();
			unk1D0[0][0]->show();
			unk1D0[0][1]->show();
			unk204->show();

			unk230[unk2EA]->show();
			unk22C->show();

			if (getBookmarkInfo().unk0 == 1) {
				setMessage(unk208, 0x14, 0);
				setMessage(unk20C, 0x14, 0);
				unk20C->show();
				unk210->hide();
				unk214->hide();
			} else if (getBookmarkInfo().unk18 == 0) {
				setMessage(unk208, 0x14, 0x19);
				setMessage(unk20C, 0x14, 0x19);
				unk20C->show();
				unk210->hide();
				unk214->hide();
			} else {
				s16 score = (s16)getBookmarkInfo().unk1C;
				if (score < 0) {
					score = 0;
				}

				if (score < 100) {
					((J2DPicture*)unk224)
					    ->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
					((J2DPicture*)unk228)
					    ->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
					unk214->show();
				} else {
					((J2DPicture*)unk218)
					    ->changeTexture(unk1C[score / 100]->getTexInfo(), 0);
					score -= 100;
					((J2DPicture*)unk21C)
					    ->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
					((J2DPicture*)unk220)
					    ->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
					unk210->show();
				}

				unk20C->hide();
			}
		}
		break;

	case 2: {
		s8 oldSelect = getUnk2E9();
		u32 input    = unk270->mEnabledFrameMeaning;

		if (input & 0x20) {
			gpMSound->startSoundSystemSE(0x481CU, 0, nullptr, 0);
			unk10 = 4;
		} else if (input & 0x2) {
			if (oldSelect > 0) {
				unk2E9 = oldSelect - 1;
			}
		} else if (input & 0x4) {
			if (oldSelect < 2) {
				unk2E9 = oldSelect + 1;
			}
		}

		if (oldSelect != getUnk2E9()) {
			gpMSound->startSoundSystemSE(0x481EU, 0, nullptr, 0);

			unk1D0[oldSelect][0]->hide();
			unk1D0[oldSelect][1]->hide();
			unk1D0[getUnk2E9()][0]->show();
			unk1D0[getUnk2E9()][1]->show();
		}
		break;
	}

	case 4:
		setCenteredSizeWr(unk1BC, 20, 0, 0, unk1C0.getWidth(),
		                  unk1C0.getHeight());

		unk1E8[0][1]->hide();
		unk1E8[1][1]->hide();
		unk1E8[2][1]->hide();
		unk1D0[getUnk2E9()][0]->hide();
		unk1D0[getUnk2E9()][1]->hide();

		unk230[0]->hide();
		unk230[1]->hide();
		unk230[2]->hide();
		unk204->hide();
		unk20C->hide();
		unk210->hide();
		unk214->hide();
		unk22C->hide();

		unk10 = 3;
		break;

	case 3:
		if (unk1BC->update()) {
			unk1BC->getPane()->hide();
			unk10 = 5;
		}
		break;

	case 5:
		if (getUnk2E9() == 0) {
			unk310 = param_1;
		} else if (getUnk2E9() == 1) {
			unk310 = param_2;
		} else {
			unk310 = param_3;
		}
		result = getUnk2E9();
		break;
	}

	return result;
}

s8 TCardSave::waitForAnyKeyBM(TEProgress param_1)
{
	s8 result = -1;

	switch (unk10) {
	case 0:
		setMessage(unk12C, 0x200, getCurMessageID());
		setMessage(unk130, 0x200, getCurMessageID());

		unkF8->hide();
		unk128->hide();
		unk134->hide();
		unk138->hide();
		unk128->hide();
		unk134->hide();
		unk138->hide();
		unk128->hide();
		unk134->hide();
		unk138->hide();
		unkFC->getPane()->hide();
		unk100->getPane()->hide();

		unkE0->mPane->show();
		unkE0->setCenteredSize(20, unkE4.getWidth(), unkE4.getHeight(), 0, 0);
		unk10 = 1;
		break;

	case 1:
		if (unkE0->update()) {
			unk154[unk2EA]->show();
			unk150->show();
			unk130->show();

			if (getBookmarkInfo().unk0 == 1) {
				setMessage(unk124, 0x14, 0);
				setMessage(unk128, 0x14, 0);
				unk128->show();
				unk134->hide();
				unk138->hide();
			} else if (getBookmarkInfo().unk18 == 0) {
				setMessage(unk124, 0x14, 0x19);
				setMessage(unk128, 0x14, 0x19);
				unk128->show();
				unk134->hide();
				unk138->hide();
			} else {
				unk128->hide();
				u16 score = getBookmarkInfo().unk1C;
				if (score < 100) {
					((J2DPicture*)unk148)
					    ->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
					((J2DPicture*)unk14C)
					    ->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
					unk134->hide();
					unk138->show();
				} else {
					((J2DPicture*)unk13C)
					    ->changeTexture(unk1C[score / 100]->getTexInfo(), 0);
					score -= 100;
					((J2DPicture*)unk140)
					    ->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
					((J2DPicture*)unk144)
					    ->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
					unk134->show();
					unk138->hide();
				}
			}

			unk10 = 2;
		}
		break;

	case 2:
		if (unk2E0 > 300) {
			endWaitForChoice();
			unk10 = 3;
		} else {
			unk2E0++;
		}
		break;

	case 3:
		if (unkE0->update()) {
			cardSaveGetPane(unkE0)->hide();
			unk10 = 5;
		}
		break;

	case 5:
		unk310 = param_1;
		result = 0;
		break;
	}

	return result;
}

void saveBookmark(u32 idx)
{
	JSUMemoryOutputStream stream;
	gpCardManager->getWriteStream(&stream);
	TFlagManager::getInstance()->save(stream);
	gpCardManager->writeBlock(idx);
}

void TCardSave::selectBookmarks(TEProgress, TEProgress, TEProgress, TEProgress)
{
}

void TCardSave::changePattern(J2DPicture*, s16, u32) { }

// TODO: 98.5%. The six remaining differences are all the same shape, at the
// inlined saveBookmark(): retail moves the stream's address with `mr` and
// keeps it in one register, we re-derive it with `addi rD, rS, 0` and carry an
// extra copy. saveBookmark() itself is size-exact against the map.
void TCardSave::execMovement_()
{

	TEProgress prevState = unk310;
	s32 prevMode         = unk10;

	unk270->onFlag(0x1);

	switch (unk310) {
	case PROGRESS_UNK0:
		unk2EA = gpApplication.mSaveFile;

		if (unk308 == 7) {
			unk310 = PROGRESS_UNK2E;
			break;
		}
		if (unk308 == 9) {
			unk310 = PROGRESS_UNK34;
			break;
		}

		unk310 = PROGRESS_UNK2;
		gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK1:
	case PROGRESS_UNK19:
		unk270->offFlag(0x1);
		gpCardManager->unmount();

		if (!unk18 && unk308 == 0)
			unk2D8->setDrawEnd();

		unk2DF = 1;
		break;

	case PROGRESS_UNK2: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_BUSY)
			break;

		if (status == CARD_RESULT_READY) {
			u16 score = getBookmarkInfo().unk1C;
			if (score > 0x3E7)
				score = 0x3E7;

			if (score < 100) {
				unk148->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
				unk14C->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
			} else {
				unk13C->changeTexture(unk1C[score / 100]->getTexInfo(), 0);
				score -= int(score * 0.01f) * 100;
				unk140->changeTexture(unk1C[score / 10]->getTexInfo(), 0);
				unk144->changeTexture(unk1C[score % 10]->getTexInfo(), 0);
			}

			switch (unk308) {
			case 0:
				unk310 = PROGRESS_UNK15;
				break;
			case 6:
				unk310 = PROGRESS_UNK2B;
				break;
			case 8:
				unk310 = PROGRESS_UNK33;
				break;
			default:
				unk310 = PROGRESS_UNK2A;
				break;
			}
		} else {
			unk310 = changeMode(status);
		}
		break;
	}

	case PROGRESS_UNK3:
		if (gpCardManager->getLastStatus() == CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				unk10 = 3;
			}

			if (waitForStop(PROGRESS_UNK2) != -1)
				gpCardManager->getBookmarkInfos(&unk278[0]);
		}
		break;

	case PROGRESS_UNK4:
	case PROGRESS_UNK5:
	case PROGRESS_UNKC:
	case PROGRESS_UNKD:
	case PROGRESS_UNK2D:
	case PROGRESS_UNK35:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				unk10 = 3;
			}
			waitForStop(PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK6:
	case PROGRESS_UNK7:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForChoice(PROGRESS_UNK8, PROGRESS_UNK4, 1);
			gpCardManager->probe();
		} else {
			if (unkF8->isVisible())
				endWaitForChoice();

			waitForChoice(PROGRESS_UNK3, PROGRESS_UNK3, 1);
		}
		break;

	case PROGRESS_UNK8: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_BUSY)
			break;

		if (status == CARD_RESULT_READY) {
			s8 r = waitForChoice(PROGRESS_UNK9, PROGRESS_UNK4, 1);
			if (r == 0) {
				unk2E0 = 0;
				gpCardManager->format();
			}
			gpCardManager->probe();
		} else {
			endWaitForChoice();

			TEProgress mode = changeMode(status);
			waitForChoice(mode, mode, 1);
		}
		break;
	}

	case PROGRESS_UNK9: {
		s32 status = gpCardManager->getLastStatus();
		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				drawMessage(PROGRESS_UNKA);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						unk10 = 3;
					}
				} else {
					unk2E0++;
				}
			} else {
				if (unk178->isVisible()) {
					unk178->hide();
					unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
					                        unk164.getHeight());
					unk10 = 3;
				}
				drawMessage(PROGRESS_UNKB);
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
		break;
	}

	case PROGRESS_UNKA:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForAnyKey(PROGRESS_UNK2);
			gpCardManager->probe();
			if (r != -1)
				gpCardManager->getBookmarkInfos(&unk278[0]);
		} else {
			// The state test is dead here -- this is `case PROGRESS_UNKA`, so
			// unk310 cannot be PROGRESS_UNK2 -- but the ROM compiles it, so
			// the original really did guard the row collapse with it.
			if (unk310 == PROGRESS_UNK2 && unk178->isVisible()) {
				unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
				                        unk164.getHeight());
				unk178->hide();
			}
			waitForAnyKey(PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNKB:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			unk310 = changeMode(gpCardManager->getLastStatus());
		break;

	case PROGRESS_UNKE:
		if (unk10 == 0)
			gpCardManager->probe();

		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForChoice(PROGRESS_UNKF, PROGRESS_UNK10, 0);
			gpCardManager->probe();
			if (r == 0)
				gpCardManager->createFile();
		} else {
			endWaitForChoice();
			waitForChoice(PROGRESS_UNK3, PROGRESS_UNK3, 1);
		}
		break;

	case PROGRESS_UNKF: {
		s32 status = gpCardManager->getLastStatus();
		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				drawMessage(PROGRESS_UNK11);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						unk10 = 3;
					}
				} else {
					unk2E0++;
				}
			} else {
				drawMessage(PROGRESS_UNK12);
				if (unk2E0 > 0x12C) {
					if (unk178->isVisible()) {
						unk178->hide();
						unk160->setCenteredSize(20, 0, 0, unk164.getWidth(),
						                        unk164.getHeight());
						unk10 = 3;
					}
				} else {
					unk2E0++;
				}
			}
		} else {
			drawMessage(PROGRESS_UNK0);
		}
		break;
	}

	case PROGRESS_UNK11:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			gpCardManager->getBookmarkInfos(&unk278[0]);
		break;

	case PROGRESS_UNK12:
		if (waitForAnyKey(PROGRESS_UNK2) != -1)
			unk310 = changeMode(gpCardManager->getLastStatus());
		break;

	case PROGRESS_UNK10:
		waitForStop(PROGRESS_UNK1);
		break;

	case PROGRESS_UNK13: {
		unk2E8 = unk2EA;

		TCardBookmarkInfo& bm = getBookmarkInfo();
		if (bm.unk0 == 1) {
			unk310 = PROGRESS_UNK14;
		} else if (bm.unk18 == 0) {
			unk310 = PROGRESS_UNK16;
			saveBookmark(unk2EA);
		} else if (bm.unk18 == 0
		           || bm.unk8
		                  == TFlagManager::getInstance()->getLastSaveTime()) {
			unk310 = PROGRESS_UNK2C;
		} else {
			unk310 = PROGRESS_UNK16;
		}
		break;
	}

	case PROGRESS_UNK14:
	case PROGRESS_UNK1E: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK2, 1);
			gpCardManager->probe();
			if (r == 0)
				saveBookmark(unk2EA);
		} else {
			endWaitForChoice();

			TEProgress mode = changeMode(status);
			waitForChoiceBM(mode, mode, 1);
		}
		break;
	}

	case PROGRESS_UNK15: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK1, 1);
			gpCardManager->probe();
			if (r == 0) {
				TCardBookmarkInfo& bm = getBookmarkInfo();
				if (bm.unk0 == 1) {
					unk310 = PROGRESS_UNK1E;
				} else {
					if (bm.unk18 == 0
					    || bm.unk8
					           == TFlagManager::getInstance()
					                  ->getLastSaveTime()) {

						saveBookmark(unk2EA);
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			endWaitForChoice();

			waitForChoiceBM(PROGRESS_UNK3, PROGRESS_UNK3, 1);
		}
		break;
	}

	case PROGRESS_UNK16: {
		s32 status = gpCardManager->getLastStatus();

		if (status != CARD_RESULT_BUSY) {
			if (status == CARD_RESULT_READY) {
				if (unk2DC != 0) {
					unk2DC = 0;
					if (unk2DD == 0)
						TFlagManager::getInstance()->saveSuccess();
				}

				s8 r = drawMessageBM(PROGRESS_UNK17);
				if (r != -1 && unk2DE != 0)
					unk310 = PROGRESS_UNK19;

				if (unk2E0 > 0x12C) {
					if (unk2DD != 0) {
						endWaitForChoice();
					} else {
						JSUMemoryOutputStream stream;
						gpCardManager->getOptionWriteStream(&stream);
						TFlagManager::getInstance()->saveOption(stream);
						gpCardManager->writeOptionBlock();
						unk2DD = 1;
					}
				} else {
					if (unk2DD == 0 && unk2E0 == 0xC8) {
						gpMSound->startSoundSystemSE(MSD_SE_SY_DECIDE_COMMON, 0,
						                             nullptr, 0);

						if (unk308 == 0 || unk308 == 6 || unk308 == 8) {
							unk128->hide();
							u16 score
							    = TFlagManager::smInstance->getFlag(0x40000);
							if (score > 0x3E7)
								score = 0x3E7;

							if (score < 100) {
								unk138->show();
								unk134->hide();
								unk148->changeTexture(
								    unk1C[score / 10]->getTexInfo(), 0);
								unk14C->changeTexture(
								    unk1C[score % 10]->getTexInfo(), 0);
							} else {
								unk134->show();
								unk138->hide();
								unk13C->changeTexture(
								    unk1C[score / 100]->getTexInfo(), 0);
								score -= ((s32)((f32)score * 0.01f)) * 100;
								unk140->changeTexture(
								    unk1C[score / 10]->getTexInfo(), 0);
								unk144->changeTexture(
								    unk1C[score % 10]->getTexInfo(), 0);
							}
						}

						unk2E0++;
					} else {
						unk2E0++;
					}
				}
				gpCardManager->probe();
			} else {
				drawMessageBM(PROGRESS_UNK18);

				endWaitForChoice();

				if (unk2DC != 0) {
					unk2DC = 0;
					if (unk2DD == 0)
						TFlagManager::smInstance->saveFail();
				}
			}
		} else {
			gpCardManager->probe();
			drawMessageBM(PROGRESS_UNK0);
			unk2E0++;
		}
		break;
	}

	case PROGRESS_UNK17:
		if (unk308 != 0 || unk18) {
			unk310 = PROGRESS_UNK1;
		} else {
			waitForChoiceBM(PROGRESS_UNK1, PROGRESS_UNK19, 0);
		}
		break;

	case PROGRESS_UNK18:
		if (waitForAnyKeyBM(PROGRESS_UNK2) != -1)
			unk310 = changeMode(gpCardManager->getLastStatus());
		break;

	case PROGRESS_UNK2A:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r = waitForSelect2(PROGRESS_UNK16, PROGRESS_UNK1);
			gpCardManager->probe();
			if (r == 0) {
				TCardBookmarkInfo& bm = getBookmarkInfo();
				if (bm.unk0 == 1) {
					unk310 = PROGRESS_UNK1E;
				} else {
					if (bm.unk18 == 0
					    || bm.unk8
					           == TFlagManager::getInstance()
					                  ->getLastSaveTime()) {
						saveBookmark(unk2EA);
						unk310 = PROGRESS_UNK16;
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			if (unk10 < 3)
				unk10 = 4;
			waitForSelect2(PROGRESS_UNK3, PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK2B:
		if (gpCardManager->getLastStatus() == CARD_RESULT_READY) {
			s32 r
			    = waitForSelect3(PROGRESS_UNK16, PROGRESS_UNK16, PROGRESS_UNK1);
			gpCardManager->probe();

			if (r != -1) {
				if (r == 0 || r == 1) {
					TCardBookmarkInfo& bm = getBookmarkInfo();
					if (bm.unk0 == 1) {
						unk310 = PROGRESS_UNK1E;
					} else {
						if (bm.unk18 == 0
						    || bm.unk8
						           == TFlagManager::getInstance()
						                  ->getLastSaveTime()) {
							saveBookmark(unk2EA);
							unk310 = PROGRESS_UNK16;
						} else {
							unk310 = PROGRESS_UNK2C;
						}
					}

					if (r == 0) {
						unk2DE = 0;
					} else if (r == 1) {
						unk2DE = 1;
					}
				} else {
					unk310 = PROGRESS_UNK1;
				}
			}
		} else {
			if (unk10 < 3)
				unk10 = 4;
			waitForSelect3(PROGRESS_UNK3, PROGRESS_UNK3, PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK2C:
		if (gpCardManager->getLastStatus() != CARD_RESULT_NOCARD) {
			waitForStop(PROGRESS_UNK1);
			gpCardManager->probe();
		} else {
			if (unkA4->isVisible()) {
				unk48->setCenteredSize(20, 0, 0, unk4C.getWidth(),
				                       unk4C.getHeight());
				unkA4->hide();
				unk10 = 3;
			}
			waitForStop(PROGRESS_UNK3);
		}
		break;

	case PROGRESS_UNK2E:
		waitForSelectOver();
		break;

	case PROGRESS_UNK33: {
		s32 status = gpCardManager->getLastStatus();
		if (status == CARD_RESULT_READY) {
			s32 r = waitForChoiceBM(PROGRESS_UNK16, PROGRESS_UNK1, 1);
			gpCardManager->probe();
			if (r == 0) {
				TCardBookmarkInfo& bm = getBookmarkInfo();
				if (bm.unk0 == 1) {
					unk310 = PROGRESS_UNK1E;
				} else {
					if (bm.unk18 == 0
					    || bm.unk8
					           == TFlagManager::getInstance()
					                  ->getLastSaveTime()) {
						saveBookmark(unk2EA);
						unk310 = PROGRESS_UNK16;
					} else {
						unk310 = PROGRESS_UNK2C;
					}
				}
			}
		} else {
			endWaitForChoice();

			TEProgress mode = changeMode(status);
			waitForChoiceBM(mode, mode, 1);
		}
		break;
	}

	case PROGRESS_UNK34:
		waitForChoice(PROGRESS_UNK19, PROGRESS_UNK1, 1);
		break;

	default:
		break;
	}

	if (prevState != unk310) {
		unk314 = unk310;
		unk318 = 0;
		unk2E0 = 0;
		unk2F8 = 0;
		unk2F9 = 1;
		unk2DC = 1;
		unk30C = 0;
		unk2DD = 0;
		unk10  = 0;
		unk44  = nullptr;
	}

	if (prevMode != unk10) {
		if (prevMode == 1) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_IN, 0, nullptr, 0);
		}

		if (unk10 == 3) {
			gpMSound->startSoundSystemSE(MSD_SE_SY_2D_OUT, 0, nullptr, 0);
		}
	}
}

u8 TCardSave::getNextState()
{
	if (unk2DF != 0) {
		if (unk310 == 0x19)
			return 1;

		if (unk310 == PROGRESS_UNK1)
			return 0;

		return 3;
	}

	return 3;
}

void TCardSave::execIssueGX_(JDrama::TGraphics* param_1)
{
	const JDrama::TRect& scissorRect = param_1->getScissor();
	{
		J2DOrthoGraph graph(param_1->getViewport());
		graph.setup2D();
		unk14->draw(0, 0, &graph);
	}

	param_1->setScissor(scissorRect);
}
