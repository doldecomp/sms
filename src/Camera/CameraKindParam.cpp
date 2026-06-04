#include <Camera/CameraKindParam.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/cameralib.hpp>

// TODO: part of cameralib?
static inline void inbetweenS16(s16* dst, s16 src, f32 ratio)
{
	if (ratio < 0.001f) {
		*dst = src;
	} else {
		*dst += CLBRoundf<s16>((1.0f / ratio) * (f32)(s16)(src - *dst));
	}
}

void TCameraKindParam::copySaveParam(const TCamSaveKindParam& src)
{
	mFovy                   = src.mSLFovy.get();
	mNearClip               = src.mSLNearClip.get();
	mDistMin                = src.mSLDistMin.get();
	mDistMax                = src.mSLDistMax.get();
	mCushionMin             = src.mSLCushionMin.get();
	mCushionMax             = src.mSLCushionMax.get();
	mXAngleMin              = src.mSLXAngleMin.get();
	mXAngleMax              = src.mSLXAngleMax.get();
	mXRotRatioManualSpeed   = src.mSLXRotRatioManualSpeed.get();
	mYAngleManualSpeedXMin  = src.mSLYAngleManualSpeedXMin.get();
	mYAngleManualSpeedXMax  = src.mSLYAngleManualSpeedXMax.get();
	mAtOffsetY              = src.mSLAtOffsetY.get();
	mXRotRatioAtOffsetY     = src.mSLXRotRatioAtOffsetY.get();
	mTargetAtJumpOffsetY    = src.mSLTargetAtJumpOffsetY.get();
	mAtJumpOffsetSpeed      = src.mSLAtJumpOffsetSpeed.get();
	mHeightPanChaseRateY    = src.mSLHeightPanChaseRateY.get();
	mSecureViewChase        = src.mSLSecureViewChase.get();
	mSecureViewDistXMin     = src.mSLSecureViewDistXMin.get();
	mSecureViewDistZMin     = src.mSLSecureViewDistZMin.get();
	mSecureViewDistXMax     = src.mSLSecureViewDistXMax.get();
	mSecureViewDistZMax     = src.mSLSecureViewDistZMax.get();
	mHoldAddDistXZMin       = src.mSLHoldAddDistXZMin.get();
	mHoldAddDistXZMax       = src.mSLHoldAddDistXZMax.get();
	mHoldOffsetAngleXMin    = src.mSLHoldOffsetAngleXMin.get();
	mHoldOffsetAngleXMax    = src.mSLHoldOffsetAngleXMax.get();
	mOffsetAngleX           = src.mSLOffsetAngleX.get();
	mOffsetAngleY           = src.mSLOffsetAngleY.get();
	mOffsetLookatXZ         = src.mSLOffsetLookatXZ.get();
	mMaxAddAngleY           = src.mSLMaxAddAngleY.get();
	mAutoChaseStartFrame    = src.mSLAutoChaseStartFrame.get();
	mAutoChaseCompleteFrame = src.mSLAutoChaseCompleteFrame.get();
	mFollowSpeedXmin        = src.mSLFollowSpeedXmin.get();
	mFollowSpeedXmax        = src.mSLFollowSpeedXmax.get();
	mJumpFollowSpeedXmin    = src.mSLJumpFollowSpeedXmin.get();
	mJumpFollowSpeedXmax    = src.mSLJumpFollowSpeedXmax.get();
	mObstructMaginfXmin     = src.mSLObstructMaginfXmin.get();
	mObstructMaginfXmax     = src.mSLObstructMaginfXmax.get();
	mInHouseMaginfXmin      = src.mSLInHouseMaginfXmin.get();
	mInHouseMaginfXmax      = src.mSLInHouseMaginfXmax.get();
	mLFollowMaginfXmin      = src.mSLLFollowMaginfXmin.get();
	mLFollowMaginfXmax      = src.mSLLFollowMaginfXmax.get();
	mPosChaseRateXZ         = src.mSLPosChaseRateXZ.get();
	mPosChaseRateXZ_C       = src.mSLPosChaseRateXZ_C.get();
	mPosChaseRateY          = src.mSLPosChaseRateY.get();
	mPosChaseRateY_C        = src.mSLPosChaseRateY_C.get();
	mAtChaseRateXZ          = src.mSLAtChaseRateXZ.get();
	mAtChaseRateY           = src.mSLAtChaseRateY.get();
}

void TCameraKindParam::inbetweenData(const TCameraKindParam& other, f32 ratio)
{
	CLBChaseConstantSpecifyFrame<f32>(&mFovy, other.mFovy, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mNearClip, other.mNearClip, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mDistMin, other.mDistMin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mDistMax, other.mDistMax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mCushionMin, other.mCushionMin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mCushionMax, other.mCushionMax, ratio);

	inbetweenS16(&mXAngleMin, other.mXAngleMin, ratio);
	inbetweenS16(&mXAngleMax, other.mXAngleMax, ratio);

	CLBChaseConstantSpecifyFrame<f32>(&mXRotRatioManualSpeed,
	                                  other.mXRotRatioManualSpeed, ratio);

	inbetweenS16(&mYAngleManualSpeedXMin, other.mYAngleManualSpeedXMin, ratio);
	inbetweenS16(&mYAngleManualSpeedXMax, other.mYAngleManualSpeedXMax, ratio);

	CLBChaseConstantSpecifyFrame<f32>(&mAtOffsetY, other.mAtOffsetY, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mXRotRatioAtOffsetY,
	                                  other.mXRotRatioAtOffsetY, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mTargetAtJumpOffsetY,
	                                  other.mTargetAtJumpOffsetY, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mAtJumpOffsetSpeed,
	                                  other.mAtJumpOffsetSpeed, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mHeightPanChaseRateY,
	                                  other.mHeightPanChaseRateY, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mSecureViewChase, other.mSecureViewChase,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mSecureViewDistXMin,
	                                  other.mSecureViewDistXMin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mSecureViewDistZMin,
	                                  other.mSecureViewDistZMin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mSecureViewDistXMax,
	                                  other.mSecureViewDistXMax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mSecureViewDistZMax,
	                                  other.mSecureViewDistZMax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mHoldAddDistXZMin,
	                                  other.mHoldAddDistXZMin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mHoldAddDistXZMax,
	                                  other.mHoldAddDistXZMax, ratio);

	inbetweenS16(&mHoldOffsetAngleXMin, other.mHoldOffsetAngleXMin, ratio);
	inbetweenS16(&mHoldOffsetAngleXMax, other.mHoldOffsetAngleXMax, ratio);

	mOffsetAngleX = other.mOffsetAngleX;
	mOffsetAngleY = other.mOffsetAngleY;

	CLBChaseConstantSpecifyFrame<f32>(&mOffsetLookatXZ, other.mOffsetLookatXZ,
	                                  ratio);

	inbetweenS16(&mMaxAddAngleY, other.mMaxAddAngleY, ratio);

	mAutoChaseStartFrame    = other.mAutoChaseStartFrame;
	mAutoChaseCompleteFrame = other.mAutoChaseCompleteFrame;

	CLBChaseConstantSpecifyFrame<f32>(&mFollowSpeedXmin, other.mFollowSpeedXmin,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mFollowSpeedXmax, other.mFollowSpeedXmax,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mJumpFollowSpeedXmin,
	                                  other.mJumpFollowSpeedXmin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mJumpFollowSpeedXmax,
	                                  other.mJumpFollowSpeedXmax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mObstructMaginfXmin,
	                                  other.mObstructMaginfXmin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mObstructMaginfXmax,
	                                  other.mObstructMaginfXmax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mInHouseMaginfXmin,
	                                  other.mInHouseMaginfXmin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mInHouseMaginfXmax,
	                                  other.mInHouseMaginfXmax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mLFollowMaginfXmin,
	                                  other.mLFollowMaginfXmin, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mLFollowMaginfXmax,
	                                  other.mLFollowMaginfXmax, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mPosChaseRateXZ, other.mPosChaseRateXZ,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mPosChaseRateXZ_C,
	                                  other.mPosChaseRateXZ_C, ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mPosChaseRateY, other.mPosChaseRateY,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mPosChaseRateY_C, other.mPosChaseRateY_C,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mAtChaseRateXZ, other.mAtChaseRateXZ,
	                                  ratio);
	CLBChaseConstantSpecifyFrame<f32>(&mAtChaseRateY, other.mAtChaseRateY,
	                                  ratio);
}
