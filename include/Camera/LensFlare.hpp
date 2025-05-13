#ifndef CAMERA_LENS_FLARE_HPP
#define CAMERA_LENS_FLARE_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class J3DModelData;
class J3DModel;

class TLensFlare : public JDrama::TViewObj {
public:
	TLensFlare(const char*);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ J3DModelData* unk10;
	/* 0x14 */ J3DModel* unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
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
};

#endif
