#ifndef MOVE_BG_MAP_OBJ_WAVE_HPP
#define MOVE_BG_MAP_OBJ_WAVE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <dolphin/gx/GXStruct.h>

struct ResTIMG;
class TMapObjWave;

extern TMapObjWave* gpMapObjWave;

class TMapObjWave : public JDrama::TViewObj {
public:
	TMapObjWave(const char* name = "波の表現");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void movement();
	void updateTime();
	void updateHeightAndAlpha();
	void draw();
	s32 getAlpha(float, float) const;
	void noWave();
	f32 getHeight(float, float, float) const;
	f32 getWaveHeight(float, float) const;
	f32 getStaticTexPos0(float) const;
	f32 getStaticTexPos1(float) const;
	f32 getMoveTexPos0(float) const;
	f32 getMoveTexPos1(float) const;
	void initDraw();

public:
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ s32 unk20;
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
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ f32 unk5C;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ f32 unk64;
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ f32 unk74;
	/* 0x78 */ f32 unk78;
	/* 0x7C */ GXColorS10 unk7C;
	/* 0x84 */ GXColorS10 unk84;
	/* 0x8C */ GXColorS10 unk8C;
	/* 0x94 */ ResTIMG* unk94;
	/* 0x98 */ u16 unk98;
};

#endif
