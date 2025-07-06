#ifndef ENEMY_FEET_INV_HPP
#define ENEMY_FEET_INV_HPP

#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class TMtxCalcFootInv : public J3DMtxCalcSoftimageAnm {
public:
	TMtxCalcFootInv(u16, u16, u16, u16, u16, u16, f32);
	virtual void calc(u16);
};

#endif
