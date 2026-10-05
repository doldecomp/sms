#ifndef GC2D_GUIDE_HPP
#define GC2D_GUIDE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTRect.hpp>

class JKRMemArchive;
class J2DPane;
class J2DPicture;
class J2DSetScreen;
class J2DTextBox;
class JUTTexture;
class TBoundPane;
class TExPane;
class TMarioGamePad;

struct TGuideStageData {
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 shineCount;
	/* 0x2 */ u8 etcShineCount;
	/* 0x3 */ u8 unk3;
	/* 0x4 */ u16 coinCount;
	/* 0x6 */ u8 etcShine;
	/* 0x7 */ u8 blueCoinCount;
};

class TGuide : public JDrama::TViewObj {
public:
	enum State {
		STATE_SELECT       = 0,
		STATE_APPEARING    = 1,
		STATE_SELECTED     = 2,
		STATE_DISAPPEARING = 3,
		STATE_UNK4         = 4,
		STATE_UNK5         = 5,
		STATE_UNK6         = 6,
		STATE_CLOSE        = 7,
		STATE_CLOSED       = 8,
		STATE_OPEN         = 9,
		STATE_OPENING      = 10,
		STATE_CLOSING      = 11,
	};
	TGuide(const char* name = "<Guide>");
	void load(JSUMemoryInputStream& stream);
	void resetObjects();
	void resetScore();
	JKRMemArchive* setup(JKRMemArchive*);
	void setup2(JKRMemArchive*);
	void startMoveCursor();
	void startMoveCursor2();
	void linkSelect();
	void changePattern(J2DPicture*, short, u32);
	void mirrorPattern(J2DPicture*, short, u32);
	void rotatePattern(J2DPicture*, short, u32, short);
	void shinePattern(TBoundPane*, short, u32);
	void mmarkPattern(TExPane*, short, u32);
	void searchNearPoint(short*, short*, short, short);
	int checkPoint(int, int);
	void changeBotStatus(int);
	void placeMario();
	void appearGuidePane(int);
	void disappearGuidePane(int);
	void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ s32 unk10;
	/* 0x14 */ TGuideStageData unk14[10];
	/* 0x64 */ char unk64[0xBC - 0x64];
	/* 0xBC */ J2DSetScreen* unkBC;
	/* 0xC0 */ TMarioGamePad* unkC0;
	/* 0xC4 */ u8 unkC4;
	/* 0xC5 */ u8 unkC5;
	/* 0xC8 */ JUTTexture* unkC8[10];
	/* 0xF0 */ u16 unkF0;
	/* 0xF2 */ char unkF2[2];
	/* 0xF4 */ J2DPane* unkF4;
	/* 0xF8 */ J2DPane* unkF8[2];
	/* 0x100 */ J2DPane* unk100;
	/* 0x104 */ J2DPane* unk104[2];
	/* 0x10C */ J2DPane* unk10C[3];
	/* 0x118 */ J2DPane* unk118;
	/* 0x11C */ J2DPane* unk11C[2];
	/* 0x124 */ J2DTextBox* unk124;
	/* 0x128 */ TExPane* unk128[2];
	/* 0x130 */ char unk130[4];
	/* 0x134 */ J2DPicture* unk134;
	/* 0x138 */ J2DPicture* unk138;
	/* 0x13C */ J2DPicture* unk13C;
	/* 0x140 */ J2DPicture* unk140;
	/* 0x144 */ J2DPicture* unk144;
	/* 0x148 */ J2DPicture* unk148;
	/* 0x14C */ J2DPicture* unk14C;
	/* 0x150 */ J2DPicture* unk150;
	/* 0x154 */ J2DPicture* unk154;
	/* 0x158 */ J2DPicture* unk158;
	/* 0x15C */ u8 unk15C;
	/* 0x160 */ s32 unk160;
	/* 0x164 */ u8 unk164;
	/* 0x168 */ J2DPane* unk168[14];
	/* 0x1A0 */ char unk1A0[0x1C0 - 0x1A0];
	/* 0x1C0 */ TExPane* unk1C0[14];
	/* 0x1F8 */ char unk1F8[0x218 - 0x1F8];
	/* 0x218 */ JUTRect unk218[22];
	/* 0x378 */ TExPane* unk378[14];
	/* 0x3B0 */ char unk3B0[0x3D0 - 0x3B0];
	/* 0x3D0 */ J2DPane* unk3D0[10];
	/* 0x3F8 */ char unk3F8[0x424 - 0x3F8];
	/* 0x424 */ TExPane* unk424;
	/* 0x428 */ TExPane* unk428;
	/* 0x42C */ s16 unk42C;
	/* 0x42E */ char unk42E[2];
	/* 0x430 */ J2DPane* unk430;
	/* 0x434 */ JUTRect unk434;
	/* 0x444 */ TBoundPane* unk444;
	/* 0x448 */ J2DPane* unk448;
	/* 0x44C */ J2DPane* unk44C[10];
	/* 0x474 */ void* unk474;
	/* 0x478 */ TExPane* unk478;
	/* 0x47C */ u8 unk47C;
	/* 0x480 */ s32 unk480;
	/* 0x484 */ char unk484[0x48C - 0x484];
	/* 0x48C */ JUTRect unk48C;
	/* 0x49C */ char unk49C[0x6F8 - 0x49C];
};

#endif
