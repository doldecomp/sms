#ifndef M3DUTIL_MOTION_BLEND_CTRL_HPP
#define M3DUTIL_MOTION_BLEND_CTRL_HPP

#include <dolphin/types.h>

class J3DAnmTransform;
class M3UMtxCalcSIAnmBlendQuat;

class TMotionBlendCtrl {
public:
	TMotionBlendCtrl(bool);
	TMotionBlendCtrl(bool, int);

	float getOldMotionBlendFrame() const;
	void setOldMotionBlendAnmPtr(J3DAnmTransform*);
	J3DAnmTransform* getOldMotionBlendAnmPtr() const;
	void setNewAnm(J3DAnmTransform*);
	void keepCurAnm(J3DAnmTransform*, float);
	void setMotionBlendRatio(float);
	float getMotionBlendRatio() const;
	void execSimpleMotionBlend();

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ float unk4;
	/* 0x8 */ M3UMtxCalcSIAnmBlendQuat* unk8;
};

#endif
