#ifndef GC2D_CARD_SAVE_HPP
#define GC2D_CARD_SAVE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/CardManager.hpp>
#include <GC2D/Progress.hpp>

class TMarioGamePad;
class J2DScreen;
class J2DPane;
class J2DTextBox;
class J2DPicture;
class JUTTexture;
class JPABaseEmitter;
class TExPane;
class TPauseMenu2;

class TCardSave : public JDrama::TViewObj {
public:
	TEProgress changeMode(s32);

	TCardSave(const char* name = "<TCardSave>", bool = false);

	void load(JSUMemoryInputStream& stream);
	void initData(TMarioGamePad*);
	void init(int);
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void makeBuffer(J2DTextBox*, int);
	void setMessage(J2DTextBox*, s32, u32);
	void setMessageC(J2DTextBox*, s32, u32);
	void changeMessage(u32, u32, int);
	s8 waitForStop(TEProgress);
	void endWaitForChoice();
	s8 waitForChoice(TEProgress, TEProgress, signed char);
	s8 waitForChoiceBM(TEProgress, TEProgress, signed char);
	void endDrawMessage();
	s8 drawMessage(TEProgress);
	s8 drawMessageBM(TEProgress);
	s8 waitForAnyKey(TEProgress);
	s8 waitForSelectOver();
	s8 waitForSelect2(TEProgress, TEProgress);
	s8 waitForSelect3(TEProgress, TEProgress, TEProgress);
	s8 waitForAnyKeyBM(TEProgress);
	void selectBookmarks(TEProgress, TEProgress, TEProgress, TEProgress);
	void changePattern(J2DPicture*, short, u32);
	void execMovement_();
	u8 getNextState();
	void execIssueGX_(JDrama::TGraphics*);

	static u32 cMessageID[];

	// fabricated
	u16 getCurMessageID() { return cMessageID[unk310]; }
	TCardBookmarkInfo& getBookmarkInfo() { return unk278[unk2EA]; }

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ J2DScreen* mSaveScreen;
	/* 0x18 */ bool unk18;
	/* 0x1C */ JUTTexture* mLargeDigitFontTextures[10];
	/* 0x44 */ JPABaseEmitter* unk44;

	// Card error window
	/* 0x48 */ TExPane* unk48;
	/* 0x4C */ JUTRect unk4C;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ char unk64[0xA0 - 0x64];
	/* 0xA0 */ J2DTextBox* unkA0;
	/* 0xA4 */ J2DTextBox* unkA4;
	/* 0xA8 */ TExPane* unkA8;
	/* 0xAC */ JUTRect unkAC;
	/* 0xBC */ u32 unkBC;
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ TExPane* unkC4;
	/* 0xC8 */ JUTRect unkC8;
	/* 0xD8 */ J2DTextBox* unkD8;
	/* 0xDC */ J2DTextBox* unkDC;

	// Confirmation window for saving and continuing/quitting after
	/* 0xE0 */ TExPane* mConfirmWindow;
	/* 0xE4 */ JUTRect mConfirmWindowInitialBounds;
	/* 0xF4 */ J2DTextBox* unkF4;
	/* 0xF8 */ J2DTextBox* unkF8;
	/* 0xFC */ TExPane* mConfirmWindowYes;
	/* 0x100 */ TExPane* mConfirmWindowNo;
	/* 0x104 */ JUTRect mConfirmWindowYesInitialBounds;
	/* 0x114 */ JUTRect mConfirmWindowNoInitialBounds;
	/* 0x124 */ J2DTextBox* unk124;
	/* 0x128 */ J2DTextBox* unk128;
	/* 0x12C */ J2DTextBox* mConfirmWindowText;
	/* 0x130 */ J2DTextBox* mConfirmWindowTextShadow;
	/* 0x134 */ J2DPane* mShineCountOver100;
	/* 0x138 */ J2DPane* mShineCountUnder100;
	/* 0x13C */ J2DPicture* mShineCountHundredsOver100;
	/* 0x140 */ J2DPicture* mShineCountTensOver100;
	/* 0x144 */ J2DPicture* mShineCountOnesOver100;
	/* 0x148 */ J2DPicture* mShineCountTensUnder100;
	/* 0x14C */ J2DPicture* mShineCountOnesUnder100;
	/* 0x150 */ J2DPane* mConfirmWindowMarioText;
	/* 0x154 */ J2DPane* mSaveNameABC[3];

	// Card formatting window
	/* 0x160 */ TExPane* unk160;
	/* 0x164 */ JUTRect unk164;
	/* 0x174 */ J2DTextBox* unk174;
	/* 0x178 */ J2DTextBox* unk178;

	// Event save window: blue coins, nozzles, yoshi
	/* 0x17C */ TExPane* mEventSaveWindow;
	/* 0x180 */ JUTRect mEventSaveWindowInitialBounds;
	/* 0x190 */ J2DTextBox* mEventSaveWindowText;
	/* 0x194 */ J2DTextBox* mEventSaveWindowTextShadow;
	/* 0x198 */ J2DTextBox* mEventSaveOptions[2][2];
	/* 0x1A8 */ J2DPane* mEventSavePointerHands[2][2];
	/* 0x1B8 */ u16 unk1B8;

	// Shine get save window
	/* 0x1BC */ TExPane* mShineGetSaveWindow;
	/* 0x1C0 */ JUTRect mShineGetSaveWindowInitialBounds;
	/* 0x1D0 */ J2DTextBox* mShineGetSavePointerHands[3][2];
	/* 0x1E8 */ J2DTextBox* mShineGetSaveOptions[3][2];
	/* 0x200 */ J2DTextBox* mShineGetSaveWindowText;
	/* 0x204 */ J2DTextBox* mShineGetSaveWindowTextShadow;
	/* 0x208 */ J2DTextBox* unk208;
	/* 0x20C */ J2DTextBox* unk20C;
	/* 0x210 */ J2DPane* unk210;
	/* 0x214 */ J2DPane* unk214;
	/* 0x218 */ J2DPane* unk218;
	/* 0x21C */ J2DPane* unk21C;
	/* 0x220 */ J2DPane* unk220;
	/* 0x224 */ J2DPane* unk224;
	/* 0x228 */ J2DPane* unk228;
	/* 0x22C */ J2DPane* mShineGetSaveMarioText;
	/* 0x230 */ J2DPane* mShineGetSaveNameABC[3];
	/* 0x23C */ u16 unk23C;

	// Game over: continue or quit
	/* 0x240 */ TExPane* mGameOverSaveWindow;
	/* 0x244 */ JUTRect mGameOverSaveWindowInitialBounds;
	/* 0x254 */ J2DTextBox* mGameOverSavePointerHands[2][2];
	/* 0x264 */ J2DTextBox* mGameOverSaveOptions[2];
	/* 0x26C */ u16 unk26C;

	/* 0x270 */ TMarioGamePad* mGamePad;
	/* 0x274 */ char unk274[0x4];
	/* 0x278 */ TCardBookmarkInfo unk278[3];
	/* 0x2D8 */ TPauseMenu2* unk2D8;
	/* 0x2DC */ u8 unk2DC;
	/* 0x2DD */ u8 unk2DD;
	/* 0x2DE */ u8 unk2DE;
	/* 0x2DF */ u8 unk2DF;
	/* 0x2E0 */ int unk2E0;
	/* 0x2E4 */ void* unk2E4;
	/* 0x2E8 */ u8 unk2E8;
	/* 0x2E9 */ s8 unk2E9;
	/* 0x2EA */ s8 unk2EA;
	/* 0x2EB */ char unk2EB[0x2F8 - 0x2EB];
	/* 0x2F8 */ u8 unk2F8;
	/* 0x2F9 */ u8 unk2F9;
	/* 0x2FC */ s32 unk2FC;
	/* 0x300 */ s16 unk300;
	/* 0x302 */ s16 unk302;
	/* 0x304 */ s16 unk304;
	/* 0x308 */ int unk308;
	/* 0x30C */ u8 unk30C;
	/* 0x310 */ TEProgress unk310;
	/* 0x314 */ TEProgress unk314;
	/* 0x318 */ u32 unk318;
	/* 0x31C */ char unk31C[0x4];
};

#endif
