#ifndef GCCONSOLE2_HPP
#define GCCONSOLE2_HPP

#include <types.h>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JUtility/JUTPoint.hpp>

class THelpActor;
class TBlendPane;
class J2DOrthoGraph;
class J2DSetScreen;
class J2DScreen;
class TBoundPane;
class JUTTexture;
class TExPane;
class JPABaseEmitter;
class J2DPicture;
class J2DPane;
class J2DTextBox;
class TMessageLoader;
class TConsoleStr;

class TGCConsole2 : public JDrama::TViewObj {
public:
	TGCConsole2(const char* name = "<TGCConsole2>");
	void load(JSUMemoryInputStream&);
	void loadAfter();
	void entryHelpActor(THelpActor*);
	void startCameraDemo();
	void resetMoveTank();
	void endCameraDemo();
	void startAppearTank();
	void startDisappearTank();
	void startAppearCoin();
	void startDisappearCoin();
	void startInsertLife(int);
	void resetLife(int);
	void startAppearLife(int);
	void startDisappearLife(int);
	void startDownLeftBot();
	void startUpLeftBot();
	void startAppearTelop(bool);
	void startDisappearTelop();
	void startDisappearTimer();
	void startAppearTimer(int, s32);
	void startInsertTimer();
	void startAppearJetBalloon(int, int);
	void startInsertJetBalloon();
	void startAppearRedCoin();
	void pauseIn();
	void pauseOut();
	bool startDisappearBalloon(u32, bool);
	bool startAppearBalloon(u32, bool);
	void startDisappearStar();
	void startAppearStar();
	void drawWaterBack();
	void startDisappearMario();
	void startAppearMario(bool);
	void processMoveNozzle();
	void changeNum(TBlendPane*, int, int);
	void setTimer(s32);
	void startMoveTimer(int);
	void stopMoveTimer();
	int getFinishedTime();

	void perform(u32, JDrama::TGraphics*);

	void processAppearLife(int) { }
	void processInsertLife(int) { }
	void processAppearStar(int) { }
	void processDownCoin(int) { }
	void processAppearTank(int) { }
	void processAppearCoin(int) { }
	void processAppearMario(int) { }
	void processDrawTelop(u32) { }
	void checkChangeTelopArray() { }
	void processAppearJet(int) { }
	void processAppearRed(int) { }
	void processAppearTimer(int) { }
	void processAppearBalloon() { }
	void processDisappearBalloon() { }
	void drawJuice(J2DOrthoGraph&, u32) { }
	void drawWater(J2DOrthoGraph&) { }

	static JUTPoint cDownTopPoint;
	static JUTPoint cDownMidPoint;
	static JUTPoint cDownBotPoint;
	static JUTPoint cUpTopPoint;
	static JUTPoint cUpMidPoint;
	static JUTPoint cUpBotPoint;
	static JUTPoint cCoinTopPoint;
	static JUTPoint cCoinMidPoint;
	static JUTPoint cCoinBotPoint;

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u32 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u16 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u32 unk28;
	/* 0x2C */ u32 unk2C;
	/* 0x30 */ u32 unk30;
	/* 0x34 */ u8 unk34[30];
	/* 0x54 */ u32 unk54;
	/* 0x58 */ u8 unk58;
	/* 0x59 */ u8 unk59;
	/* 0x5A */ u8 unk5A;
	/* 0x5C */ u32 unk5C;
	/* 0x60 */ u8 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u8 unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ char unk70[0x8];
	/* 0x78 */ u16 unk78;
	/* 0x7A */ char unk7A[2];
	/* 0x7C */ u16 unk7C;
	/* 0x80 */ u32 unk80;
	/* 0x84 */ u16 unk84;
	/* 0x86 */ u16 unk86;
	/* 0x88 */ u8 unk88;
	/* 0x8A */ u16 unk8A;
	/* 0x8C */ int unk8C;
	/* 0x90 */ THelpActor** unk90;
	/* 0x94 */ TConsoleStr* unk94;
	/* 0x98 */ u16 unk98;
	/* 0x9A */ u8 unk9A[20];
	/* 0xB0 */ J2DSetScreen* unkB0;
	/* 0xB4 */ u8 unkB4;
	/* 0xB6 */ u16 unkB6;
	/* 0xB8 */ u32 unkB8;
	/* 0xBC */ u32 unkBC;
	/* 0xC0 */ u32 unkC0;
	/* 0xC4 */ u32 unkC4;
	/* 0xC8 */ TBoundPane* unkC8;
	/* 0xCC */ TBoundPane* unkCC;
	/* 0xD0 */ TBoundPane* unkD0;
	/* 0xD4 */ TBlendPane* unkD4[3];
	/* 0xE0 */ JUTTexture* unkE0[10];
	/* 0x108 */ TExPane* unk108;
	/* 0x10C */ char unk10C[0x124 - 0x10C];
	/* 0x124 */ JPABaseEmitter* unk124;
	/* 0x128 */ TBoundPane* unk128;
	/* 0x12C */ TBoundPane* unk12C;
	/* 0x130 */ TBoundPane* unk130;
	/* 0x134 */ TBoundPane* unk134[3];
	/* 0x140 */ TExPane* unk140;
	/* 0x144 */ JPABaseEmitter* unk144;
	/* 0x148 */ TBoundPane* unk148;
	/* 0x14C */ TBoundPane* unk14C;
	/* 0x150 */ TBoundPane* unk150;
	/* 0x154 */ J2DPicture** unk154;
	/* 0x158 */ J2DPicture** unk158;
	/* 0x15C */ J2DPicture** unk15C;
	/* 0x160 */ TExPane* unk160;
	/* 0x164 */ JPABaseEmitter* unk164;
	/* 0x168 */ char unk168[0xC];
	/* 0x174 */ TBoundPane* unk174;
	/* 0x178 */ TBoundPane* unk178;
	/* 0x17C */ J2DPane* unk17C[18];
	/* 0x1C4 */ TBoundPane* unk1C4;
	/* 0x1C8 */ u16 unk1C8;
	/* 0x1CA */ u16 unk1CA;
	/* 0x1CC */ char unk1CC[4];
	/* 0x1D0 */ JUTRect unk1D0[9];
	/* 0x260 */ TBoundPane* unk260;
	/* 0x264 */ u16 unk264;
	/* 0x266 */ u8 unk266;
	/* 0x268 */ u16 unk268;
	/* 0x26A */ u16 unk26A;
	/* 0x26C */ TBoundPane* unk26C;
	/* 0x270 */ TBoundPane* unk270;
	/* 0x274 */ TBoundPane* unk274;
	/* 0x278 */ TBoundPane* unk278[4];
	/* 0x288 */ J2DPane* unk288;
	/* 0x28C */ J2DPane* unk28C[4];
	/* 0x29C */ TBoundPane* unk29C;
	/* 0x2A0 */ J2DPicture* unk2A0[3];
	/* 0x2AC */ J2DPicture* unk2AC[3];
	/* 0x2B8 */ char unk2B8[0x4];
	/* 0x2BC */ JUTRect unk2BC[3];
	/* 0x2EC */ JUtility::TColor unk2EC[3];
	/* 0x2F8 */ TExPane* unk2F8;
	/* 0x2FC */ JUTRect unk2FC;
	/* 0x30C */ u8 unk30C;
	/* 0x310 */ u32 unk310;
	/* 0x314 */ J2DPane* unk314[4];
	/* 0x324 */ J2DPane* unk324;
	/* 0x328 */ J2DPane* unk328;
	/* 0x32C */ J2DPane* unk32C;
	/* 0x330 */ u8 unk330;
	/* 0x334 */ J2DPane* unk334[22];
	/* 0x38C */ TBoundPane* unk38C;
	/* 0x390 */ TBoundPane* unk390;
	/* 0x394 */ TBoundPane* unk394;
	/* 0x398 */ TBoundPane* unk398;
	/* 0x39C */ TBoundPane* unk39C[3];
	/* 0x3A8 */ TExPane* unk3A8;
	/* 0x3AC */ char unk3AC[0x4];
	/* 0x3B0 */ J2DPane* unk3B0;
	/* 0x3B4 */ J2DTextBox* unk3B4;
	/* 0x3B8 */ J2DTextBox* unk3B8;
	/* 0x3BC */ JUTRect unk3BC;
	/* 0x3CC */ u16 unk3CC;
	/* 0x3D0 */ TMessageLoader* unk3D0;
	/* 0x3D4 */ JSUInputStream* unk3D4;
	/* 0x3D8 */ JSUOutputStream* unk3D8;
	/* 0x3DC */ JSUOutputStream* unk3DC;
	/* 0x3E0 */ u32 unk3E0;
	/* 0x3E4 */ int unk3E4;
	/* 0x3E8 */ u16 unk3E8;
	/* 0x3EC */ f32 unk3EC;
	/* 0x3F0 */ u16 unk3F0;
	/* 0x3F4 */ u32 unk3F4;
	/* 0x3F8 */ char unk3F8[0x4];
	/* 0x3FC */ TExPane* unk3FC;
	/* 0x400 */ TBoundPane* unk400;
	/* 0x404 */ TBoundPane* unk404;
	/* 0x408 */ TBoundPane* unk408;
	/* 0x40C */ TBoundPane* unk40C;
	/* 0x410 */ TBoundPane* unk410;
	/* 0x414 */ TBoundPane* unk414[4];
	/* 0x424 */ char unk424[0x2];
	/* 0x426 */ u8 unk426;
	/* 0x428 */ TExPane* unk428;
	/* 0x42C */ TBoundPane* unk42C;
	/* 0x430 */ TBoundPane* unk430;
	/* 0x434 */ TBoundPane* unk434;
	/* 0x438 */ TBoundPane* unk438;
	/* 0x43C */ TBoundPane* unk43C[2];
	/* 0x444 */ u32 unk444;
	/* 0x448 */ u8 unk448;
	/* 0x44C */ TExPane* unk44C;
	/* 0x450 */ TBoundPane* unk450;
	/* 0x454 */ TBoundPane* unk454;
	/* 0x458 */ TBoundPane* unk458[10];
	/* 0x480 */ TBoundPane* unk480[3];
	/* 0x48C */ JUTRect unk48C;
	/* 0x49C */ JUTRect unk49C;
	/* 0x4AC */ JUTRect unk4AC;
	/* 0x4BC */ JUTPoint unk4BC[6];
	/* 0x4EC */ JUTPoint unk4EC[2];
	/* 0x4FC */ int unk4FC;
	/* 0x500 */ J2DPane* unk500[2];
	/* 0x508 */ int unk508;
	/* 0x50C */ int unk50C;
	/* 0x510 */ char unk510[0x8];
	/* 0x518 */ int unk518;
	/* 0x51C */ u8 unk51C;
	/* 0x520 */ TExPane* unk520;
	/* 0x524 */ TExPane* unk524;
	/* 0x528 */ J2DTextBox* unk528;
	/* 0x52C */ J2DTextBox* unk52C;
	/* 0x530 */ TMessageLoader* unk530;
	/* 0x534 */ JUTRect unk534;
	/* 0x544 */ JUTRect unk544;
	/* 0x554 */ u32 unk554;
	/* 0x558 */ u8 unk558;
	/* 0x55C */ u32 unk55C;
	/* 0x560 */ u16 unk560;
	/* 0x562 */ u16 unk562;
	/* 0x564 */ f32 unk564;
	/* 0x568 */ f32 unk568;
	/* 0x56C */ u8 unk56C;
	/* 0x56D */ u8 unk56D;
	/* 0x570 */ u32 unk570;
};

#endif
