#ifndef GC2D_TALK_2D_2_HPP
#define GC2D_TALK_2D_2_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class JUTPoint;
class J2DPane;
class J2DSetScreen;
class J2DTextBox;
class JUTTexture;
class JMSMesgEntry;
class TMessageLoader;
class TBoundPane;
class TBaseNPC;

class TTalk2D2;
class TMarioGamePad;

extern TTalk2D2* gpTalk2D;

class TTalk2D2 : public JDrama::TViewObj {
public:
	TTalk2D2(const char* name = "<TTalk2D2>");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void setMessageID(u32, u32);
	void forceCloseTalk();
	void closeTalkWindow();
	void openTalkWindow(TBaseNPC*);
	void makeBoxLine(s8, char*);
	bool openBoardWindow();
	bool openNormalWindow();
	void moveBoardWindow();
	void checkBoardControler();
	void moveTalkWindow();
	void checkControler();
	bool closeNormalWindow();
	bool closeBoardWindow();
	bool eraseNormalWindow();
	bool eraseBoardWindow();
	bool appearBoardBoxWindow();
	void makeLine(f32*, f32*, f32, JUTPoint&, JUTPoint&, JUTPoint&);
	void setupBoardTextBox(const void*, JMSMesgEntry*);
	void setupTextBox(const void*, JMSMesgEntry*);
	void setTagParam(JSUMemoryInputStream&, J2DTextBox&, int*, int*);
	void openWindow(s8, f32);

	static u32 cColorTable[6];

	int getTalkMode() const { return unk248; }
	s8 getSelectedValue() const { return unk214; }

	enum {
		STATE_UNK0 = 0x0,
		STATE_UNK1 = 0x1,
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
		STATE_UNK4 = 0x4,
		STATE_UNK5 = 0x5,
		STATE_UNK6 = 0x6,
		STATE_UNK7 = 0x7,
		STATE_UNK8 = 0x8,
	};

public:
	/* 0x10 */ J2DSetScreen* unk10;
	/* 0x14 */ TBoundPane* unk14;
	/* 0x18 */ J2DTextBox* unk18;
	/* 0x1C */ J2DPane* unk1C;
	/* 0x20 */ J2DPane* unk20;
	/* 0x24 */ J2DPane* unk24;
	/* 0x28 */ u8 unk28;
	/* 0x29 */ u8 unk29;
	/* 0x2A */ char unk2A[0x2C - 0x2A];
	/* 0x2C */ J2DSetScreen* unk2C;
	/* 0x30 */ J2DPane* unk30[3];
	/* 0x3C */ J2DPane* unk3C[3];
	/* 0x48 */ J2DPane* unk48[3];
	/* 0x54 */ J2DPane* unk54[3];
	/* 0x60 */ J2DPane* unk60[3];
	/* 0x6C */ J2DPane* unk6C[3];
	/* 0x78 */ J2DPane* unk78[3];
	/* 0x84 */ J2DPane* unk84[3];
	/* 0x90 */ J2DPane* unk90;
	/* 0x94 */ f32 unk94;
	/* 0x98 */ char unk98[0x9C - 0x98];
	/* 0x9C */ J2DTextBox* unk9C[90];
	/* 0x204 */ J2DPane* unk204;
	/* 0x208 */ J2DTextBox* unk208;
	/* 0x20C */ J2DPane* unk20C[2];
	/* 0x214 */ s8 unk214; // the line the player selected in a choice window
	/* 0x215 */ char unk215[0x218 - 0x215];
	/* 0x218 */ char* unk218[2];
	/* 0x220 */ s16 unk220;
	/* 0x222 */ s16 unk222;
	/* 0x224 */ u8 unk224[3];
	/* 0x227 */ char unk227;
	/* 0x228 */ int unk228[3];
	/* 0x234 */ f32 unk234[3];
	/* 0x240 */ char unk240[0x244 - 0x240];
	/* 0x244 */ JUTTexture* unk244;
	/* 0x248 */ u32 unk248; // talk mode
	/* 0x24C */ TMarioGamePad* unk24C;
	/* 0x250 */ u8 unk250;
	/* 0x251 */ u8 unk251;
	/* 0x252 */ u8 unk252;
	/* 0x253 */ char unk253;
	/* 0x254 */ JMSMesgEntry* unk254;
	/* 0x258 */ TMessageLoader* unk258;
	/* 0x25C */ TMessageLoader* unk25C;
	/* 0x260 */ TMessageLoader* unk260;
	/* 0x264 */ u32 unk264;
	/* 0x268 */ char unk268[0x26A - 0x268];
	/* 0x26A */ u8 unk26A;
	/* 0x26B */ u8 unk26B;
	/* 0x26C */ u8 unk26C;
	/* 0x26D */ u8 unk26D;
	/* 0x26E */ char unk26E[0x270 - 0x26E];
	/* 0x270 */ u32 unk270;
	/* 0x274 */ int unk274;
	/* 0x278 */ u32 unk278;
	/* 0x27C */ s32 unk27C;
	/* 0x280 */ u8 unk280;
	/* 0x281 */ u8 unk281[0x2DC - 0x281];
	/* 0x2DC */ s16 unk2DC;
	/* 0x2DE */ u16 unk2DE;
	struct TUnk2E0 {
		/* 0x0 */ TBaseNPC* unk0;
		/* 0x4 */ u32 unk4;
	};

	/* 0x2E0 */ TUnk2E0 unk2E0[10];
	/* 0x330 */ s16 unk330;
	/* 0x332 */ s16 unk332;
	/* 0x334 */ s16 unk334;
	/* 0x336 */ char unk336[0x338 - 0x336];
	/* 0x338 */ f32 unk338;
	/* 0x33C */ f32 unk33C;
	/* 0x340 */ s16 unk340;
	/* 0x342 */ char unk342[0x344 - 0x342];
};

#endif
