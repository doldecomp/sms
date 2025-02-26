#ifndef M3DUTIL_MOTION_BLEND_CTRL_HPP
#define M3DUTIL_MOTION_BLEND_CTRL_HPP

class J3DAnmTransform;

class TMotionBlendCtrl {
public:
	TMotionBlendCtrl(bool);
	TMotionBlendCtrl(bool, int);

	void getOldMotionBlendFrame() const;
	void setOldMotionBlendAnmPtr(J3DAnmTransform*);
	void getOldMotionBlendAnmPtr() const;
	void setNewAnm(J3DAnmTransform*);
	void keepCurAnm(J3DAnmTransform*, float);
	void setMotionBlendRatio(float);
	void getMotionBlendRatio() const;
	void execSimpleMotionBlend();
};

#endif
