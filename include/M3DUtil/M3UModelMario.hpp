#ifndef M3DUTIL_M3U_MODEL_MARIO_HPP
#define M3DUTIL_M3U_MODEL_MARIO_HPP

#include <M3DUtil/M3UModel.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class M3UMtxCalcSIAnmBlendQuat;

class M3UModelCommonMario : public M3UModelCommon {
public:
	virtual J3DMtxCalc* getMtxCalc(const M3UMtxCalcSetInfo& param_1);

public:
	/* 0x18 */ M3UMtxCalcSIAnmBlendQuat* unk18;
};

class M3UMarioMtxCalcSetInfo {
public:
	/* 0x0 */ u16 mJointIdx;
	/* 0x2 */ u8 unk2; // unknown
	/* 0x3 */ u8 mMtxCalcIdx;
	/* 0x4 */ u16 mAnmTransformIdx[2];
	/* 0x8 */ u8 mFrameCtrlIdx;
	/* 0xA */ u16 unkA;
	/* 0xC */ u16 unkC;
	/* 0xE */ u16 unkE;
	/* 0x10 */ u16 unk10;
	/* 0x12 */ u16 unk12;
};

class M3UModelMario : public M3UModel {
public:
	virtual void updateIn();
	virtual void updateOut();

	void updateInMotion();
	void changeMtxCalcSIAnmBQAnmTransform(int, int, u16);

public:
	/* 0x20 */ M3UModelCommonMario* unk20;
	/* 0x24 */ M3UMarioMtxCalcSetInfo* unk24;
	/* 0x28 */ J3DMtxCalcSoftimage unk28;
};

#endif
