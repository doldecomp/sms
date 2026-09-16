#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>

class TMtxCalcFootInv : public J3DMtxCalcSoftimageAnm {
public:
	TMtxCalcFootInv(u16, u16, u16, u16, u16, u16, f32);
	virtual ~TMtxCalcFootInv();
	virtual void calc(u16);

	u16 unk68;
	u16 unk6A;
	u16 unk6C;
	u16 unk6E;
	u16 unk70;
	u16 unk72;
	f32 unk74;
};

TMtxCalcFootInv::~TMtxCalcFootInv() { }
