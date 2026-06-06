#ifndef CAMERA_LENS_GLOW_HPP
#define CAMERA_LENS_GLOW_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>

class J3DModelData;
class J3DModel;
class J3DAnmBase;

class TLensGlow : public JDrama::TViewObj {
public:
	TLensGlow(bool, const char*);
	void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ J3DModelData* unk10;
	/* 0x14 */ J3DModel* unk14;
	/* 0x18 */ J3DAnmTextureSRTKey* unk18;
	/* 0x1C */ J3DFrameCtrl unk1C;
	/* 0x30 */ J3DAnmTevRegKey* unk30;
	/* 0x34 */ J3DFrameCtrl unk34;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ f32 unk4C;
	/* 0x50 */ f32 unk50;
	/* 0x54 */ f32 unk54;
	/* 0x58 */ f32 unk58;
	/* 0x5C */ u8 unk5C;
	/* 0x5D */ u8 unk5D;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ f32 unk64;
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ JGeometry::TVec3<f32> unk74;
	/* 0x80 */ f32 unk80;
	/* 0x84 */ f32 unk84;
	/* 0x88 */ f32 unk88;
	/* 0x8C */ f32 unk8C;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ f32 unk94;
};

#endif
