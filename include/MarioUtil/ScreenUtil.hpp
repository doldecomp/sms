#ifndef MARIO_UTIL_SCREEN_UTIL_HPP
#define MARIO_UTIL_SCREEN_UTIL_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

void SMS_FillScreenAlpha(u8);

class J3DModelData;
class JUTTexture;

class TAfterEffect;

extern TAfterEffect* gpAfterEffect;

class TAfterEffect : public JDrama::TViewObj {
public:
	TAfterEffect() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void setBlurDefaultValue();
	void calcDashBlurValue();

	BOOL checkFlag(u32 flag) { return unk14 & flag ? TRUE : FALSE; }

public:
	/* 0x10 */ JUTTexture* unk10;
	/* 0x14 */ u8 unk14;
	/* 0x15 */ u8 unk15;
	/* 0x16 */ u8 unk16;
	/* 0x17 */ u8 unk17;
	/* 0x18 */ u8 unk18;
	/* 0x19 */ u8 unk19;
	/* 0x1A */ u8 unk1A;
	/* 0x1B */ u8 unk1B;
	/* 0x1C */ u8 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ char unk4C[0x4];
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ u8 unk58;
	/* 0x59 */ u8 unk59;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ f32 unk64;
};

class TScreenTexture;

extern TScreenTexture* gpScreenTexture;

class TScreenTexture : public JDrama::TViewObj {
public:
	TScreenTexture() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*) { }

	bool replace(J3DModelData*, const char*);

	// fabricated
	JUTTexture* getTexture() { return unk10; }

public:
	/* 0x10 */ JUTTexture* unk10;
};

#endif
