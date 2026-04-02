#ifndef M3DUTIL_MOTION_BLEND_CTRL_HPP
#define M3DUTIL_MOTION_BLEND_CTRL_HPP

#include <dolphin/types.h>

class J3DAnmTransform;
class M3UMtxCalcSIAnmBlendQuat;

class TMotionBlendCtrl {
public:
	TMotionBlendCtrl(bool);
	TMotionBlendCtrl(bool, int);

	f32 getOldMotionBlendFrame() const;
	void setOldMotionBlendAnmPtr(J3DAnmTransform*);
	J3DAnmTransform* getOldMotionBlendAnmPtr() const;
	void setNewAnm(J3DAnmTransform*);
	void keepCurAnm(J3DAnmTransform*, f32);
	void setMotionBlendRatio(f32);
	f32 getMotionBlendRatio() const;
	void execSimpleMotionBlend();

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ M3UMtxCalcSIAnmBlendQuat* unk8;
};

#endif
