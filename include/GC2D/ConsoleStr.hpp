#ifndef GC2D_CONSOLE_STR_HPP
#define GC2D_CONSOLE_STR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>

class J2DSetScreen;
class TExPane;
class J2DTextBox;
class TBoundPane;

class TConsoleStr : public JDrama::TViewObj {
public:
	TConsoleStr(const char* name = "<ConsoleStr>");
	f32 getWipeCloseTime();
	void load(JSUMemoryInputStream&);
	void loadAfter();
	void perform(u32, JDrama::TGraphics*);
	void startAppearReady();
	void startAppearGo();
	void startAppearShineGet();
	void startAppearMiss();
	void startAppearScenario();
	bool processReady(int);
	bool processGo(float);
	bool processShineGet(int);
	bool processMiss(int);
	bool processScenario(int);
	void startCloseWipe(bool);
	void startOpenWipe();

	// TODO: wrong types
	static JUTPoint cShineGetRight1;
	static JUTPoint cShineGetLeft1;
	static JUTPoint cShineGetRight2;
	static JUTPoint cShineGetLeft2;
	static JUTPoint cShineGetRight3;
	static JUTPoint cShineGetLeft3;

public:
	/* 0x10 */ J2DSetScreen* unk10;
	/* 0x14 */ J2DSetScreen* unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ TBoundPane* unk28[3];
	/* 0x34 */ JUTPoint unk34[66];
	/* 0x244 */ TBoundPane* unk244[9];
	/* 0x268 */ TBoundPane* unk268[5];
	/* 0x27C */ TExPane* unk27C[5];
	/* 0x290 */ TExPane* unk290[2];
	/* 0x298 */ TExPane* unk298;
	/* 0x29C */ TExPane* unk29C;
	/* 0x2A0 */ J2DTextBox* unk2A0[2];
	/* 0x2A8 */ u8 unk2A8;
	/* 0x2A9 */ u8 unk2A9;
	/* 0x2AC */ void* unk2AC;
	/* 0x2B0 */ void* unk2B0;
	/* 0x2B4 */ void* unk2B4;
	/* 0x2B8 */ int unk2B8;
	/* 0x2BC */ int unk2BC;
};

#endif
