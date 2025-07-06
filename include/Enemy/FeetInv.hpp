#ifndef ENEMY_FEET_INV_HPP
#define ENEMY_FEET_INV_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class TMtxCalcFootInv : public J3DMtxCalcSoftimageAnm {
public:
	TMtxCalcFootInv(u16, u16, u16, u16, u16, u16, f32);
	virtual void calc(u16);

public:
	/* 0x68 */ u16 unk68;
	/* 0x6A */ u16 unk6A;
	/* 0x6C */ u16 unk6C;
	/* 0x6E */ u16 unk6E;
	/* 0x70 */ u16 unk70;
	/* 0x72 */ u16 unk72;
	/* 0x74 */ f32 unk74;
};

void FeetInvCalc(J3DModel*, u16, u16, u16, f32);

#endif
