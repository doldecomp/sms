#ifndef M3DUTIL_M3U_JOINT_HPP
#define M3DUTIL_M3U_JOINT_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <dolphin/mtx.h>

class M3UMtxCalcSIAnmBlendQuat : public J3DMtxCalcSoftimage {
public:
	M3UMtxCalcSIAnmBlendQuat();
	M3UMtxCalcSIAnmBlendQuat(bool);
	~M3UMtxCalcSIAnmBlendQuat() { }

	void calc(u16);
	void init(const Vec&, const Mtx&);
	void calcTransform(u16, const J3DTransformInfo&);

public:
	/* 0x50 */ float unk50;
	/* 0x54 */ J3DAnmTransform* unk54; // "new"
	/* 0x58 */ J3DAnmTransform* unk58; // "old"
	/* 0x5C */ bool unk5C;
	/* 0x60 */ float unk60;
};

void M3UMtxCalcBlendAux(u16 param_1, J3DTransformInfo* param_2,
                        J3DTransformInfo* param_3, float param_4, bool param_5);

#endif
