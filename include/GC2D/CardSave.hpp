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
	TEProgress changeMode(long);

	TCardSave(const char* name = "<TCardSave>", bool = false);

	void load(JSUMemoryInputStream& stream);
	void initData(TMarioGamePad*);
	void init(int);
	void perform(unsigned long, JDrama::TGraphics*);
	void makeBuffer(J2DTextBox*, int);
	void setMessage(J2DTextBox*, long, unsigned long);
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
	void changePattern(J2DPicture*, short, unsigned long);
	void execMovement_();
	u8 getNextState();
	void execIssueGX_(JDrama::TGraphics*);

	static u32 cMessageID[];

	// fabricated
	u16 getCurMessageID() { return cMessageID[unk310]; }
	TCardBookmarkInfo& getBookmarkInfo() { return unk278[unk2EA]; }

public:
	/* 0x10 */ int unk10;
	/* 0x14 */ J2DScreen* unk14;
	/* 0x18 */ bool unk18;
	/* 0x1C */ JUTTexture* unk1C[10];
	/* 0x44 */ JPABaseEmitter* unk44;
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
	/* 0xE0 */ TExPane* unkE0;
	/* 0xE4 */ JUTRect unkE4;
	/* 0xF4 */ J2DTextBox* unkF4;
	/* 0xF8 */ J2DTextBox* unkF8;
	/* 0xFC */ TExPane* unkFC;
	/* 0x100 */ TExPane* unk100;
	/* 0x104 */ JUTRect unk104;
	/* 0x114 */ JUTRect unk114;
	/* 0x124 */ J2DTextBox* unk124;
	/* 0x128 */ J2DTextBox* unk128;
	/* 0x12C */ J2DTextBox* unk12C;
	/* 0x130 */ J2DTextBox* unk130;
	/* 0x134 */ J2DPane* unk134;
	/* 0x138 */ J2DPane* unk138;
	/* 0x13C */ J2DPicture* unk13C;
	/* 0x140 */ J2DPicture* unk140;
	/* 0x144 */ J2DPicture* unk144;
	/* 0x148 */ J2DPicture* unk148;
	/* 0x14C */ J2DPicture* unk14C;
	/* 0x150 */ J2DPane* unk150;
	/* 0x154 */ J2DPane* unk154[3];
	/* 0x160 */ TExPane* unk160;
	/* 0x164 */ JUTRect unk164;
	/* 0x174 */ J2DTextBox* unk174;
	/* 0x178 */ J2DTextBox* unk178;
	/* 0x17C */ TExPane* unk17C;
	/* 0x180 */ JUTRect unk180;
	/* 0x190 */ J2DTextBox* unk190;
	/* 0x194 */ J2DTextBox* unk194;
	/* 0x198 */ J2DTextBox* unk198[2][2];
	/* 0x1A8 */ J2DPane* unk1A8[2][2];
	/* 0x1B8 */ u16 unk1B8;
	/* 0x1BC */ TExPane* unk1BC;
	/* 0x1C0 */ JUTRect unk1C0;
	/* 0x1D0 */ J2DTextBox* unk1D0[3][2];
	/* 0x1E8 */ J2DTextBox* unk1E8[3][2];
	/* 0x200 */ J2DTextBox* unk200;
	/* 0x204 */ J2DTextBox* unk204;
	/* 0x208 */ J2DTextBox* unk208;
	/* 0x20C */ J2DTextBox* unk20C;
	/* 0x210 */ J2DPane* unk210;
	/* 0x214 */ J2DPane* unk214;
	/* 0x218 */ J2DPane* unk218;
	/* 0x21C */ J2DPane* unk21C;
	/* 0x220 */ J2DPane* unk220;
	/* 0x224 */ J2DPane* unk224;
	/* 0x228 */ J2DPane* unk228;
	/* 0x22C */ J2DPane* unk22C;
	/* 0x230 */ J2DPane* unk230[3];
	/* 0x23C */ u16 unk23C;
	/* 0x240 */ TExPane* unk240;
	/* 0x244 */ JUTRect unk244;
	/* 0x254 */ J2DTextBox* unk254[2][2];
	/* 0x264 */ J2DTextBox* unk264[2];
	/* 0x26C */ u16 unk26C;
	/* 0x270 */ TMarioGamePad* unk270;
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
