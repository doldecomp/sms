#ifndef GC2D_CONSOLE_STR_HPP
#define GC2D_CONSOLE_STR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>

class J2DSetScreen;
class TExPane;
class J2DTextBox;
class TBoundPane;
class JPABaseEmitter;

class TConsoleStr : public JDrama::TViewObj {
public:
	TConsoleStr(const char* name = "<ConsoleStr>");
	f32 getWipeCloseTime();
	void load(JSUMemoryInputStream&);
	void loadAfter();
	void perform(u32 cue, JDrama::TGraphics* graphics);
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
	/* 0x20 */ int unk20;
	/* 0x24 */ u32 unk24;
	// Layout read off load()/the constructor: the 'sg00' loop runs six times
	// and the 'ms00' loop seven, __construct_array builds 66 JUTPoints at
	// 0x34 (three rows of 22, the row stride in processGo is 0xb0), and the
	// three JPABaseEmitter pointers loadAfter() clears sit at 0x2a8.
	/* 0x28 */ TBoundPane* unk28[3];
	/* 0x34 */ JUTPoint unk34[3][22];
	/* 0x244 */ TBoundPane* unk244[6];
	/* 0x25C */ TBoundPane* unk25C[7];
	/* 0x278 */ TExPane* unk278[5];
	/* 0x28C */ TExPane* unk28C[2];
	/* 0x294 */ TExPane* unk294;
	/* 0x298 */ TExPane* unk298;
	/* 0x29C */ J2DTextBox* unk29C[2];
	/* 0x2A4 */ u8 unk2A4;
	/* 0x2A5 */ u8 unk2A5;
	/* 0x2A8 */ JPABaseEmitter* unk2A8[3];
	/* 0x2B4 */ int unk2B4;
	/* 0x2B8 */ int unk2B8;
};

#endif
