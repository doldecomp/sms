#ifndef M3DUTIL_M3U_MODEL_MARIO_HPP
#define M3DUTIL_M3U_MODEL_MARIO_HPP

#include <M3DUtil/M3UModel.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class M3UMtxCalcSIAnmBlendQuat;

class M3UModelCommonMario : public M3UModelCommon {
public:
	virtual J3DMtxCalc* getMtxCalc(const M3UMtxCalcSetInfo& param_1)
	{
		return &unk18[param_1.mMtxCalcIdx];
	}

public:
	/* 0x18 */ M3UMtxCalcSIAnmBlendQuat* unk18;
};

struct SomeModelMarioStruct {
	/* 0x0 */ s16 unk0;
	/* 0x2 */ u8 unk2;
	/* 0x3 */ u8 unk3;
	/* 0x4 */ u16 unk4[2];
	/* 0x8 */ u8 unk8;
};

class M3UModelMario : public M3UModel {
public:
	virtual void updateIn();
	virtual void updateOut();

	void updateInMotion();
	void changeMtxCalcSIAnmBQAnmTransform(int, int, u16);

public:
	/* 0x20 */ M3UModelCommonMario* unk20;
	/* 0x24 */ SomeModelMarioStruct* unk24;
	/* 0x28 */ J3DMtxCalcSoftimage unk28;
};

#endif
