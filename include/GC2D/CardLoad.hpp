#ifndef GC2D_CARD_LOAD_HPP
#define GC2D_CARD_LOAD_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/CardManager.hpp>
#include <GC2D/Progress.hpp>

class J2DPane;
class J2DPicture;
class J2DTextBox;
class J2DSetScreen;
class JPABaseEmitter;
class TCardLoad;
class TExPane;
class JUTTexture;
class TFileLoadBlock;
class TOptionControl;

extern TCardLoad* gpCardLoad;

class TCardLoad : public JDrama::TViewObj {
public:
	TCardLoad(const char* name = "<TCardLoad>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32, JDrama::TGraphics*);

	void changePattern(J2DPicture*, s16, u32);
	void setupTitleScreen();
	void setupScoreScreen();
	void loadAfter();
	void titleDraw();
	void makeBuffer(J2DTextBox*, int);
	void changeMode(s32);
	void setMessage(J2DTextBox*, s32, int);
	void waitForChoice(TEProgress, TEProgress, int);
	void waitForChoiceBM(TEProgress, TEProgress, int);
	void waitForAnyKey(TEProgress);
	void waitForAnyKeyBM(TEProgress);
	void waitForStart(TEProgress);
	void drawMessage(TEProgress);
	void drawMessageBM(TEProgress);
	void selectBookmark(TEProgress, TEProgress, bool);
	void selectFunction();
	void setSelected(u8);
	void changeScene();

	static u32 cMessageID;

public:
	struct UnkCardLoadStruct {
		/* 0x0 */ J2DPicture* unk0;
		/* 0x4 */ J2DPicture* unk4[3];
		/* 0x10 */ J2DPane* unk10[8];
		/* 0x30 */ J2DPane* unk30[2];
		/* 0x38 */ J2DPane* unk38;
	};

	/* 0x10 */ int unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ J2DSetScreen* unk28;
	/* 0x2C */ J2DSetScreen* unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ J2DSetScreen* unk34;
	/* 0x38 */ void* unk38;
	/* 0x3C */ char unk3C[4];
	/* 0x40 */ TCardBookmarkInfo unk40[3];
	/* 0xA0 */ void* unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ f32 unkA8;
	/* 0xAC */ JPABaseEmitter* unkAC;
	/* 0xB0 */ s8 unkB0;
	/* 0xB1 */ s8 unkB1;
	/* 0xB2 */ char unkB2[2];
	/* 0xB4 */ s16 unkB4;
	/* 0xB6 */ s8 unkB6;
	/* 0xB7 */ s8 unkB7;
	/* 0xB8 */ u8 unkB8;
	/* 0xBC */ u32 unkBC;
	/* 0xC0 */ int unkC0;
	/* 0xC4 */ int unkC4;
	/* 0xC8 */ JUTTexture* unkC8[10];
	/* 0xF0 */ TExPane* unkF0;
	/* 0xF4 */ TExPane* unkF4;
	/* 0xF8 */ TExPane* unkF8[11];
	/* 0x124 */ JUTRect unk124[11];
	/* 0x1D4 */ TExPane* unk1D4[13];
	/* 0x208 */ J2DPane* unk208;
	/* 0x20C */ s16 unk20C[11];
	/* 0x222 */ u8 unk222[11];
	/* 0x22E */ s16 unk22E[8];
	/* 0x23E */ char unk23E[0x248 - 0x23E];
	/* 0x248 */ u8 unk248[8];
	/* 0x250 */ char unk24D[0x258 - 0x250];
	/* 0x258 */ u16 unk258;
	/* 0x25C */ J2DPane* unk25C;
	/* 0x260 */ JUTRect unk260;
	/* 0x270 */ J2DPane* unk270;
	/* 0x274 */ u8 unk274;
	/* 0x275 */ u8 unk275;
	/* 0x278 */ TFileLoadBlock* unk278[3];
	/* 0x284 */ u32 unk284;
	/* 0x288 */ TExPane* unk288;
	/* 0x28C */ JUTRect unk28C;
	/* 0x29C */ J2DTextBox* unk29C;
	/* 0x2A0 */ J2DTextBox* unk2A0;
	/* 0x2A4 */ TExPane* unk2A4[3];
	/* 0x2B0 */ JUTRect unk2B0;
	/* 0x2C0 */ J2DTextBox* unk2C0[3];
	/* 0x2CC */ J2DTextBox* unk2CC[3];
	/* 0x2D8 */ J2DPane* unk2D8[3];
	/* 0x2E4 */ J2DPicture* unk2E4[3];
	/* 0x2F0 */ J2DPicture* unk2F0[3];
	/* 0x2FC */ J2DPane* unk2FC[3];
	/* 0x308 */ J2DPicture* unk308[3];
	/* 0x314 */ J2DPicture* unk314[3];
	/* 0x320 */ J2DPicture* unk320[3];
	/* 0x32C */ J2DPicture* unk32C[3];
	/* 0x338 */ int unk338;
	/* 0x33C */ TExPane* unk33C[3];
	/* 0x348 */ JUTRect unk348[3];
	/* 0x378 */ TExPane* unk378[3][4];
	/* 0x3A8 */ JUTRect unk3A8[3][4];
	/* 0x468 */ TExPane* unk468;
	/* 0x46C */ JUTRect unk46C;
	/* 0x47C */ J2DTextBox* unk47C;
	/* 0x480 */ J2DTextBox* unk480;
	/* 0x484 */ TExPane* unk484[2];
	/* 0x48C */ JUTRect unk48C[2];
	/* 0x4AC */ TExPane* unk4AC;
	/* 0x4B0 */ JUTRect unk4B0;
	/* 0x4C0 */ J2DTextBox* unk4C0;
	/* 0x4C4 */ J2DTextBox* unk4C4;
	/* 0x4C8 */ J2DPane* unk4C8;
	/* 0x4CC */ J2DPane* unk4CC[3];
	/* 0x4D8 */ TExPane* unk4D8[2];
	/* 0x4E0 */ JUTRect unk4E0[2];
	/* 0x500 */ J2DPane* unk500;
	/* 0x504 */ J2DPane* unk504[3];
	/* 0x510 */ J2DPane* unk510;
	/* 0x514 */ J2DPane* unk514[2];
	/* 0x51C */ J2DTextBox* unk51C;
	/* 0x520 */ J2DTextBox* unk520;
	/* 0x524 */ TExPane* unk524;
	/* 0x528 */ JUTRect unk528;
	/* 0x538 */ J2DTextBox* unk538;
	/* 0x53C */ J2DTextBox* unk53C;
	/* 0x540 */ J2DPane* unk540[3];
	/* 0x54C */ TExPane* unk54C;
	/* 0x550 */ JUTRect unk550;
	/* 0x560 */ J2DTextBox* unk560;
	/* 0x564 */ J2DTextBox* unk564;
	/* 0x568 */ TExPane* unk568;
	/* 0x56C */ JUTRect unk56C;
	/* 0x57C */ J2DTextBox* unk57C;
	/* 0x580 */ J2DTextBox* unk580;
	/* 0x584 */ UnkCardLoadStruct unk584[7];
	/* 0x728 */ J2DPane* unk728[3];
	/* 0x734 */ char unk734[0x740 - 0x734];
	/* 0x740 */ J2DPane* unk740;
	/* 0x744 */ J2DPane* unk744;
	/* 0x748 */ J2DPicture* unk748;
	/* 0x74C */ J2DPicture* unk74C;
	/* 0x750 */ J2DPicture* unk750;
	/* 0x754 */ TOptionControl* unk754;
};

#endif
