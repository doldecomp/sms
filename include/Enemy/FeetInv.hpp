#ifndef ENEMY_FEET_INV_HPP
#define ENEMY_FEET_INV_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class J3DModel;

/**
 * @brief MtxCalc that plants the two feet of a walking model on the ground
 * after the animation has been applied.
 *
 * @details It calculates joints as J3DMtxCalcSoftimageAnm does, and whenever
 * the joint it has just calculated is one of the two ankles, it hands that
 * leg's three joint indices to FeetInvCalc(), which bends the leg. The ankle
 * is calculated after the joints above it, so by then both bones of the leg
 * are in place.
 */
class TMtxCalcFootInv : public J3DMtxCalcSoftimageAnm {
public:
	TMtxCalcFootInv(u16, u16, u16, u16, u16, u16, f32);
	virtual void calc(u16);

public:
	/* 0x68 */ u16 unk68; ///< first leg: joint at the top of the leg
	/* 0x6A */ u16 unk6A; ///< first leg: joint between the two bones
	/* 0x6C */ u16 unk6C; ///< first leg: ankle, and the trigger joint
	/* 0x6E */ u16 unk6E; ///< second leg: joint at the top of the leg
	/* 0x70 */ u16 unk70; ///< second leg: joint between the two bones
	/* 0x72 */ u16 unk72; ///< second leg: ankle, and the trigger joint
	/* 0x74 */ f32 unk74; ///< how far above the ground an ankle should sit
};

void FeetInvCalc(J3DModel* model, u16 jnt_hip, u16 jnt_knee, u16 jnt_foot,
                 f32 height);

#endif
