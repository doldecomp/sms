#ifndef CAMERA_CAMERA_KIND_PARAM_HPP
#define CAMERA_CAMERA_KIND_PARAM_HPP

#include <dolphin/types.h>

class TCamSaveKindParam;

class TCameraKindParam {
public:
	void inbetweenData(const TCameraKindParam&, f32);
	void copySaveParam(const TCamSaveKindParam&);

public:
	/* 0x0 */ f32 mFovy;
	/* 0x4 */ f32 mNearClip;
	/* 0x8 */ f32 mDistMin;
	/* 0xC */ f32 mDistMax;
	/* 0x10 */ f32 mCushionMin;
	/* 0x14 */ f32 mCushionMax;
	/* 0x18 */ s16 mXAngleMin;
	/* 0x1A */ s16 mXAngleMax;
	/* 0x1C */ f32 mXRotRatioManualSpeed;
	/* 0x20 */ s16 mYAngleManualSpeedXMin;
	/* 0x22 */ s16 mYAngleManualSpeedXMax;
	/* 0x24 */ f32 mAtOffsetY;
	/* 0x28 */ f32 mXRotRatioAtOffsetY;
	/* 0x2C */ f32 mTargetAtJumpOffsetY;
	/* 0x30 */ f32 mAtJumpOffsetSpeed;
	/* 0x34 */ f32 mHeightPanChaseRateY;
	/* 0x38 */ f32 mSecureViewChase;
	/* 0x3C */ f32 mSecureViewDistXMin;
	/* 0x40 */ f32 mSecureViewDistZMin;
	/* 0x44 */ f32 mSecureViewDistXMax;
	/* 0x48 */ f32 mSecureViewDistZMax;
	/* 0x4C */ f32 mHoldAddDistXZMin;
	/* 0x50 */ f32 mHoldAddDistXZMax;
	/* 0x54 */ s16 mHoldOffsetAngleXMin;
	/* 0x56 */ s16 mHoldOffsetAngleXMax;
	/* 0x58 */ s16 mOffsetAngleX;
	/* 0x5A */ s16 mOffsetAngleY;
	/* 0x5C */ f32 mOffsetLookatXZ;
	/* 0x60 */ s16 mMaxAddAngleY;
	/* 0x64 */ s32 mAutoChaseStartFrame;
	/* 0x68 */ s32 mAutoChaseCompleteFrame;
	/* 0x6C */ f32 mFollowSpeedXmin;
	/* 0x70 */ f32 mFollowSpeedXmax;
	/* 0x74 */ f32 mJumpFollowSpeedXmin;
	/* 0x78 */ f32 mJumpFollowSpeedXmax;
	/* 0x7C */ f32 mObstructMaginfXmin;
	/* 0x80 */ f32 mObstructMaginfXmax;
	/* 0x84 */ f32 mInHouseMaginfXmin;
	/* 0x88 */ f32 mInHouseMaginfXmax;
	/* 0x8C */ f32 mLFollowMaginfXmin;
	/* 0x90 */ f32 mLFollowMaginfXmax;
	/* 0x94 */ f32 mPosChaseRateXZ;
	/* 0x98 */ f32 mPosChaseRateXZ_C;
	/* 0x9C */ f32 mPosChaseRateY;
	/* 0xA0 */ f32 mPosChaseRateY_C;
	/* 0xA4 */ f32 mAtChaseRateXZ;
	/* 0xA8 */ f32 mAtChaseRateY;
};

#endif
