#ifndef NPC_INBETWEEN_HPP
#define NPC_INBETWEEN_HPP

#include <JSystem/JGeometry/JGVec3.hpp>

class MActor;

class TNpcInbetween {
public:
	TNpcInbetween(int pos_inbetween_frame, int motion_blend_frame)
	    : mPosInbetweenFrame(pos_inbetween_frame)
	    , mMotionBlendFrame(motion_blend_frame)
	    , mPosInbetweenTimer(0)
	    , mTargetPos(0.0f, 0.0f, 0.0f)
	    , mCurrentPos(0.0f, 0.0f, 0.0f)
	    , mMotionBlendTimer(0)
	    , mForcedBlendRatio(0.0f)
	{
	}

	void execMotionBlend(MActor* mactor);
	void execPosInbetween(JGeometry::TVec3<f32>* cur_pos);

	// fabricated
	bool isMotionBlending() const
	{
		if (mMotionBlendTimer > 0)
			return true;
		else
			return false;
	}
	bool isForcedBlendRatio() const
	{
		if (mForcedBlendRatio > 0.0f)
			return true;
		else
			return false;
	}
	void startPosBlend() { mPosInbetweenTimer = mPosInbetweenFrame; }
	void startMotionBlend() { mMotionBlendTimer = mMotionBlendFrame; }
	void stopMotionBlend() { mMotionBlendTimer = 0; }
	void applyPos(JGeometry::TVec3<f32>& pos)
	{
		if (mPosInbetweenTimer > 0)
			pos.set(mCurrentPos);
	}
	void reset()
	{
		mMotionBlendTimer = 0;
		mForcedBlendRatio = 0.0f;
	}

public:
	/* 0x0 */ int mPosInbetweenFrame;
	/* 0x4 */ int mMotionBlendFrame;
	/* 0x8 */ int mPosInbetweenTimer;
	/* 0xC */ JGeometry::TVec3<f32> mTargetPos;
	/* 0x18 */ JGeometry::TVec3<f32> mCurrentPos;
	/* 0x24 */ int mMotionBlendTimer;
	/* 0x28 */ f32 mForcedBlendRatio;
};

#endif
