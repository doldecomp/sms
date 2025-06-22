#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JMath.hpp>

void J3DFrameCtrl::init(s16 end)
{
	mLoopMode     = 2;
	mFlags        = 0;
	mStartFrame   = 0;
	mEndFrame     = end;
	mLoopFrame    = 0;
	mSpeed        = 1.0;
	mCurrentFrame = 0.0;
}

BOOL J3DFrameCtrl::checkPass(float pass_frame)
{
	f32 cur_frame  = mCurrentFrame;
	f32 next_frame = cur_frame + mSpeed;

	switch (mLoopMode) {
	case LOOP_ONCE_e:
	case LOOP_ONCE_RESET_e:
		if (next_frame < mStartFrame) {
			next_frame = mStartFrame;
		}

		if (next_frame >= mEndFrame) {
			next_frame = mEndFrame - 0.001f;
		}

		if (cur_frame <= next_frame) {
			if (cur_frame <= pass_frame && pass_frame < next_frame) {
				return true;
			} else {
				return false;
			}
		}

		if (next_frame <= pass_frame && pass_frame < cur_frame) {
			return true;
		}
		return false;

	case LOOP_REPEAT_e:
		if (cur_frame < mStartFrame) {
			while (next_frame < mStartFrame) {
				if (mLoopFrame - mStartFrame <= 0.0f) {
					break;
				}
				next_frame += mLoopFrame - mStartFrame;
			}

			if (next_frame <= pass_frame && pass_frame < mLoopFrame) {
				return true;
			} else {
				return false;
			}
		} else if (mEndFrame <= cur_frame) {
			while (next_frame >= mEndFrame) {
				if (mEndFrame - mLoopFrame <= 0.0f) {
					break;
				}
				next_frame -= mEndFrame - mLoopFrame;
			}

			if (mLoopFrame <= pass_frame && pass_frame < next_frame) {
				return true;
			} else {
				return false;
			}
		} else if (next_frame < mStartFrame) {
			while (next_frame < mStartFrame) {
				if (mLoopFrame - mStartFrame <= 0.0f) {
					break;
				}
				next_frame += mLoopFrame - mStartFrame;
			}

			if ((mStartFrame <= pass_frame && pass_frame < cur_frame)
			    || (next_frame <= pass_frame && pass_frame < mLoopFrame)) {
				return true;
			} else {
				return false;
			}
		} else if (mEndFrame <= next_frame) {
			while (next_frame >= mEndFrame) {
				if (mEndFrame - mLoopFrame <= 0.0f) {
					break;
				}

				next_frame -= mEndFrame - mLoopFrame;
			}

			if ((cur_frame <= pass_frame && pass_frame < mEndFrame)
			    || (mLoopFrame <= pass_frame && pass_frame < next_frame)) {
				return true;
			} else {
				return false;
			}
		} else if (cur_frame <= next_frame) {
			if (cur_frame <= pass_frame && pass_frame < next_frame) {
				return true;
			} else {
				return false;
			}
		} else if (next_frame <= pass_frame && pass_frame < cur_frame) {
			return true;
		}
		return false;

	case LOOP_MIRROR_ONCE_e:
	case LOOP_MIRROR_REPEAT_e:
		if (next_frame >= mEndFrame) {
			next_frame = mEndFrame - 0.001f;
		}

		if (next_frame < mStartFrame) {
			next_frame = mStartFrame;
		}

		if (cur_frame <= next_frame) {
			if (cur_frame <= pass_frame && pass_frame < next_frame) {
				return true;
			} else {
				return false;
			}
		}

		if (next_frame <= pass_frame && pass_frame < cur_frame) {
			return true;
		}
		return false;

	default:
		return false;
	}
}

void J3DFrameCtrl::update()
{

	mFlags = 0;
	mCurrentFrame += mSpeed;
	switch (mLoopMode) {
	case LOOP_ONCE_e:
		if (mCurrentFrame < mStartFrame) {
			mCurrentFrame = mStartFrame;
			mSpeed        = 0.0f;
			mFlags |= 1;
		}
		if (mCurrentFrame >= mEndFrame) {
			mCurrentFrame = mEndFrame - 0.001f;
			mSpeed        = 0.0f;
			mFlags |= 1;
		}
		break;
	case LOOP_ONCE_RESET_e:
		if (mCurrentFrame < mStartFrame) {
			mCurrentFrame = mStartFrame;
			mSpeed        = 0.0f;
			mFlags |= 1;
		}
		if (mCurrentFrame >= mEndFrame) {
			mCurrentFrame = mStartFrame;
			mSpeed        = 0.0f;
			mFlags |= 1;
		}
		break;
	case LOOP_REPEAT_e:
		while (mCurrentFrame < mStartFrame) {
			mFlags |= 2;
			if (mLoopFrame - mStartFrame <= 0.0f) {
				break;
			}
			mCurrentFrame += mLoopFrame - mStartFrame;
		}
		while (mCurrentFrame >= mEndFrame) {
			mFlags |= 2;
			if (mEndFrame - mLoopFrame <= 0.0f) {
				break;
			}
			mCurrentFrame -= mEndFrame - mLoopFrame;
		}
		break;
	case LOOP_MIRROR_ONCE_e:
		if (mCurrentFrame >= mEndFrame) {
			mCurrentFrame = mEndFrame - 0.001f;
			mSpeed        = -mSpeed;
		}
		if (mCurrentFrame < mStartFrame) {
			mCurrentFrame = mStartFrame;
			mSpeed        = 0.0f;
			mFlags |= 1;
		}
		break;
	case LOOP_MIRROR_REPEAT_e:
		if (mCurrentFrame >= mEndFrame) {
			mCurrentFrame = mEndFrame - 0.001f;
			mSpeed        = -mSpeed;
		}
		if (mCurrentFrame < mStartFrame) {
			mCurrentFrame = mStartFrame;
			mSpeed        = -mSpeed;
			mFlags |= 2;
		}
		break;
	}
}

J3DAnmBase::J3DAnmBase()
{
	mFrame    = 0.0f;
	mMaxFrame = 0;
}

J3DAnmBase::J3DAnmBase(s16 param_1)
{
	mFrame    = 0.0f;
	mMaxFrame = param_1;
}

J3DAnmBase::~J3DAnmBase() { }

void J3DAnmTransformFull::getTransform(u16 idx, J3DTransformInfo* dst) const
{
	/* Scale */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mScale.x
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 0].mScaleOffset];
		} else if (mFrame < 0.0f)
			dst->mScale.x = mScaleData[mAnmTable[idx * 3 + 0].mScaleOffset];
		else
			dst->mScale.x = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 0].mScaleOffset];
	}
	/* Seems they copy-pasted this block one too many times? */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mScale.x
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 0].mScaleOffset];
		} else if (mFrame < 0.0f)
			dst->mScale.x = mScaleData[mAnmTable[idx * 3 + 0].mScaleOffset];
		else
			dst->mScale.x = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 0].mScaleOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 1].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mScale.y
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 1].mScaleOffset];
		} else if (mFrame < 0.0f)
			dst->mScale.y = mScaleData[mAnmTable[idx * 3 + 1].mScaleOffset];
		else
			dst->mScale.y = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 1].mScaleOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 2].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mScale.z
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 2].mScaleOffset];
		} else if (mFrame < 0.0f)
			dst->mScale.z = mScaleData[mAnmTable[idx * 3 + 2].mScaleOffset];
		else
			dst->mScale.z = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 2].mScaleOffset];
	}

	/* Rotation */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mRotationMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mRotation.x
			    = mRotData[(s32)mFrame
			               + mAnmTable[idx * 3 + 0].mRotationOffset];
		} else if (mFrame < 0.0f)
			dst->mRotation.x = mRotData[mAnmTable[idx * 3 + 0].mRotationOffset];
		else
			dst->mRotation.x
			    = mRotData[maxFrame - 1
			               + mAnmTable[idx * 3 + 0].mRotationOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 1].mRotationMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mRotation.y
			    = mRotData[(s32)mFrame
			               + mAnmTable[idx * 3 + 1].mRotationOffset];
		} else if (mFrame < 0.0f)
			dst->mRotation.y = mRotData[mAnmTable[idx * 3 + 1].mRotationOffset];
		else
			dst->mRotation.y
			    = mRotData[maxFrame - 1
			               + mAnmTable[idx * 3 + 1].mRotationOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 2].mRotationMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mRotation.z
			    = mRotData[(s32)mFrame
			               + mAnmTable[idx * 3 + 2].mRotationOffset];
		} else if (mFrame < 0.0f)
			dst->mRotation.z = mRotData[mAnmTable[idx * 3 + 2].mRotationOffset];
		else
			dst->mRotation.z
			    = mRotData[maxFrame - 1
			               + mAnmTable[idx * 3 + 2].mRotationOffset];
	}

	/* Translation */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mTranslateMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mTranslate.x
			    = mTransData[(s32)mFrame
			                 + mAnmTable[idx * 3 + 0].mTranslateOffset];
		} else if (mFrame < 0.0f)
			dst->mTranslate.x
			    = mTransData[mAnmTable[idx * 3 + 0].mTranslateOffset];
		else
			dst->mTranslate.x
			    = mTransData[maxFrame - 1
			                 + mAnmTable[idx * 3 + 0].mTranslateOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 1].mTranslateMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mTranslate.y
			    = mTransData[(s32)mFrame
			                 + mAnmTable[idx * 3 + 1].mTranslateOffset];
		} else if (mFrame < 0.0f)
			dst->mTranslate.y
			    = mTransData[mAnmTable[idx * 3 + 1].mTranslateOffset];
		else
			dst->mTranslate.y
			    = mTransData[maxFrame - 1
			                 + mAnmTable[idx * 3 + 1].mTranslateOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 2].mTranslateMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			dst->mTranslate.z
			    = mTransData[(s32)mFrame
			                 + mAnmTable[idx * 3 + 2].mTranslateOffset];
		} else if (mFrame < 0.0f)
			dst->mTranslate.z
			    = mTransData[mAnmTable[idx * 3 + 2].mTranslateOffset];
		else
			dst->mTranslate.z
			    = mTransData[maxFrame - 1
			                 + mAnmTable[idx * 3 + 2].mTranslateOffset];
	}
}

static f32 J3DHermiteInterpolationS(f32 t, s16* time0, s16* value0,
                                    s16* tangent0, s16* time1, s16* value1,
                                    s16* tangent1)
{
	// f32 v0 = *(f32*)value0;
	// f32 timeRange = (*(f32*)time1 - *(f32*)time0);
	// f32 kt = (t - *(f32*)time0) / timeRange;
	// f32 t1 = (-timeRange * *(f32*)tangent0 - (*(f32*)value1 - v0));
	// f32 t2 = kt * kt * ((*(f32*)tangent1 * timeRange + v0)
	//        - *(f32*)value1 - t1);
	// return (t1 * kt * kt + (timeRange * *(f32*)tangent0 + t2) * kt + v0)
	//        - t2;

	register f32 p1  = t;
	register s16* p2 = time0;
	register s16* p3 = value0;
	register s16* p4 = tangent0;
	register s16* p5 = time1;
	register s16* p6 = value1;
	register s16* p7 = tangent1;
	register f32 fout;
#ifdef __MWERKS__ // clang-format off
	asm {
		psq_l f0, 0(p2), 0x1, 5
		psq_l f3, 0(p5), 0x1, 5
		psq_l f2, 0(p3), 0x1, 5
		fsubs f4, f3, f0
		psq_l f3, 0(p6), 0x1, 5
		fsubs f6, fout, f0
		psq_l fout, 0(p7), 0x1, 5
		fsubs f5, f3, f2
		fdivs f6, f6, f4
		psq_l f0, 0(p4), 0x1, 5
		fmadds fout, fout, f4, f2
		fmuls f7, f6, f6
		fnmsubs f5, f4, f0, f5
		fsubs fout, fout, f3
		fsubs fout, fout, f5
		fmuls f3, f7, fout
		fmadds fout, f4, f0, f3
		fmadds fout, fout, f6, f2
		fmadds fout, f5, f7, fout
		fsubs fout, fout, f3
	}
#endif // clang-format on
	return fout;
}

static f32 J3DGetKeyFrameInterpolationS(f32 frame, J3DAnmKeyTableBase* table,
                                        s16* data)
{
	if (table->mType == 0) {
		if (frame < data[0]) {
			return data[1];
		} else if (data[3 * (table->mMaxFrame - 1)] <= frame) {
			return data[3 * (table->mMaxFrame - 1) + 1];
		} else {
			// bisect
			int num = table->mMaxFrame;
			while (num > 1) {
				int mid = num / 2;
				if (frame >= data[3 * mid]) {
					data += 3 * mid;
					num -= mid;
				} else {
					num = mid;
				}
			}

			return J3DHermiteInterpolationS(frame, &data[0], &data[1], &data[2],
			                                &data[3], &data[4], &data[5]);
		}
	} else {
		if (frame < data[0]) {
			return data[1];
		} else if (data[4 * (table->mMaxFrame - 1)] <= frame) {
			return data[4 * (table->mMaxFrame - 1) + 1];
		} else {
			// bisect
			int num = table->mMaxFrame;
			while (num > 1) {
				int mid = num / 2;
				if (frame >= data[4 * mid]) {
					data += 4 * mid;
					num -= mid;
				} else {
					num = mid;
				}
			}

			return J3DHermiteInterpolationS(frame, &data[0], &data[1], &data[3],
			                                &data[4], &data[5], &data[6]);
		}
	}
}

template <typename T>
f32 J3DGetKeyFrameInterpolation(f32 frame, J3DAnmKeyTableBase* table, T* data)
{
	if (table->mType == 0) {
		if (frame < data[0]) {
			return data[1];
		} else if (data[3 * (table->mMaxFrame - 1)] <= frame) {
			return data[3 * (table->mMaxFrame - 1) + 1];
		} else {
			// bisect
			int num = table->mMaxFrame;
			while (num > 1) {
				int mid = num / 2;
				if (frame >= data[3 * mid]) {
					data += 3 * mid;
					num -= mid;
				} else {
					num = mid;
				}
			}

			return JMAHermiteInterpolation(frame, data[0], data[1], data[2],
			                               data[3], data[4], data[5]);
		}
	} else {
		if (frame < data[0]) {
			return data[1];
		} else if (data[4 * (table->mMaxFrame - 1)] <= frame) {
			return data[4 * (table->mMaxFrame - 1) + 1];
		} else {
			// bisect
			int num = table->mMaxFrame;
			while (num > 1) {
				int mid = num / 2;
				if (frame >= data[4 * mid]) {
					data += 4 * mid;
					num -= mid;
				} else {
					num = mid;
				}
			}

			return JMAHermiteInterpolation(frame, data[0], data[1], data[3],
			                               data[4], data[5], data[6]);
		}
	}
}

void J3DAnmTransformKey::calcTransform(f32 frame, u16 idx,
                                       J3DTransformInfo* dst) const
{
	/* Scale */
	switch (mAnmTable[idx * 3 + 0].mScale.mMaxFrame) {
	case 0:
		dst->mScale.x = 1.0f;
		break;
	case 1:
		dst->mScale.x = mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset];
		break;
	default:
		dst->mScale.x = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mScale.mMaxFrame) {
	case 0:
		dst->mScale.y = 1.0f;
		break;
	case 1:
		dst->mScale.y = mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset];
		break;
	default:
		dst->mScale.y = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 2].mScale.mMaxFrame) {
	case 0:
		dst->mScale.z = 1.0f;
		break;
	case 1:
		dst->mScale.z = mScaleData[mAnmTable[idx * 3 + 2].mScale.mOffset];
		break;
	default:
		dst->mScale.z = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 2].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 2].mScale.mOffset]);
		break;
	}

	/* Rotation */
	switch (mAnmTable[idx * 3 + 0].mRotation.mMaxFrame) {
	case 0:
		dst->mRotation.x = 0;
		break;
	case 1:
		dst->mRotation.x = mRotData[mAnmTable[idx * 3 + 0].mRotation.mOffset]
		                   << mDecShift;
		break;
	default:
		dst->mRotation.x
		    = (s32)(J3DGetKeyFrameInterpolationS(
		          frame, &mAnmTable[idx * 3 + 0].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 0].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mRotation.mMaxFrame) {
	case 0:
		dst->mRotation.y = 0;
		break;
	case 1:
		dst->mRotation.y = mRotData[mAnmTable[idx * 3 + 1].mRotation.mOffset]
		                   << mDecShift;
		break;
	default:
		dst->mRotation.y
		    = (s32)(J3DGetKeyFrameInterpolationS(
		          frame, &mAnmTable[idx * 3 + 1].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 1].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	switch (mAnmTable[idx * 3 + 2].mRotation.mMaxFrame) {
	case 0:
		dst->mRotation.z = 0;
		break;
	case 1:
		dst->mRotation.z = mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]
		                   << mDecShift;
		break;
	default:
		dst->mRotation.z
		    = (s32)(J3DGetKeyFrameInterpolationS(
		          frame, &mAnmTable[idx * 3 + 2].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	/* Translation */
	switch (mAnmTable[idx * 3 + 0].mTranslate.mMaxFrame) {
	case 0:
		dst->mTranslate.x = 0.0f;
		break;
	case 1:
		dst->mTranslate.x
		    = mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset];
		break;
	default:
		dst->mTranslate.x = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mTranslate.mMaxFrame) {
	case 0:
		dst->mTranslate.y = 0.0f;
		break;
	case 1:
		dst->mTranslate.y
		    = mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset];
		break;
	default:
		dst->mTranslate.y = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 2].mTranslate.mMaxFrame) {
	case 0:
		dst->mTranslate.z = 0.0f;
		break;
	case 1:
		dst->mTranslate.z
		    = mTransData[mAnmTable[idx * 3 + 2].mTranslate.mOffset];
		break;
	default:
		dst->mTranslate.z = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 2].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 2].mTranslate.mOffset]);
		break;
	}
}

void J3DAnmTextureSRTKey::calcTransform(float frame, u16 idx,
                                        J3DTextureSRTInfo* dst) const
{
	/* Scale */
	switch (mAnmTable[idx * 3 + 0].mScale.mMaxFrame) {
	case 0:
		dst->mScaleX = 1.0f;
		break;
	case 1:
		dst->mScaleX = mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset];
		break;
	default:
		dst->mScaleX = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mScale.mMaxFrame) {
	case 0:
		dst->mScaleY = 1.0f;
		break;
	case 1:
		dst->mScaleY = mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset];
		break;
	default:
		dst->mScaleY = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset]);
		break;
	}

	/* Rotation */
	switch (mAnmTable[idx * 3 + 2].mRotation.mMaxFrame) {
	case 0:
		dst->mRotation = 0;
		break;
	case 1:
		dst->mRotation = mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]
		                 << mDecShift;
		break;
	default:
		dst->mRotation
		    = (s32)(J3DGetKeyFrameInterpolation<s16>(
		          frame, &mAnmTable[idx * 3 + 2].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	/* Translation */
	switch (mAnmTable[idx * 3 + 0].mTranslate.mMaxFrame) {
	case 0:
		dst->mTranslationX = 0.0f;
		break;
	case 1:
		dst->mTranslationX
		    = mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset];
		break;
	default:
		dst->mTranslationX = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mTranslate.mMaxFrame) {
	case 0:
		dst->mTranslationY = 0.0f;
		break;
	case 1:
		dst->mTranslationY
		    = mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset];
		break;
	default:
		dst->mTranslationY = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset]);
		break;
	}
}

f32 J3DAnmClusterFull::getWeight(u16 idx) const
{
	u16 maxFrame = mAnmTable[idx].mMaxFrame;

	if (0.0f <= mFrame && mFrame < maxFrame)
		return mWeight[(s32)mFrame + mAnmTable[idx].mOffset];

	if (mFrame < 0.0f)
		return mWeight[mAnmTable[idx].mOffset];
	else if (mFrame >= mAnmTable[idx].mMaxFrame)
		return mWeight[maxFrame - 1 + mAnmTable[idx].mOffset];
	else
		return 0.0f;
}

f32 J3DAnmClusterKey::getWeight(u16 idx) const
{
	if (mAnmTable[idx].mWeightTable.mMaxFrame != 0
	    && mAnmTable[idx].mWeightTable.mMaxFrame != 1)
		return J3DGetKeyFrameInterpolation<f32>(
		    mFrame, &mAnmTable[idx].mWeightTable,
		    &mWeight[mAnmTable[idx].mWeightTable.mOffset]);
	else if (mAnmTable[idx].mWeightTable.mMaxFrame == 0)
		return 1.0f;
	else
		return mWeight[mAnmTable[idx].mWeightTable.mOffset];
}

void J3DAnmVtxColorFull::getColor(u8 col, u16 idx, GXColor* dst) const
{
	{
		u16 maxFrame = mTable[col][idx].mRMaxFrame;
		if (mFrame < 0.0f)
			dst->r = mColorR[mTable[col][idx].mROffset];

		if (mFrame >= maxFrame)
			dst->r = mColorR[maxFrame - 1 + mTable[col][idx].mROffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->r = mColorR[(s32)mFrame + mTable[col][idx].mROffset];
	}
	{
		u16 maxFrame = mTable[col][idx].mGMaxFrame;
		if (mFrame < 0.0f)
			dst->g = mColorG[mTable[col][idx].mGOffset];

		if (mFrame >= maxFrame)
			dst->g = mColorG[maxFrame - 1 + mTable[col][idx].mGOffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->g = mColorG[(s32)mFrame + mTable[col][idx].mGOffset];
	}
	{
		u16 maxFrame = mTable[col][idx].mBMaxFrame;
		if (mFrame < 0.0f)
			dst->b = mColorB[mTable[col][idx].mBOffset];

		if (mFrame >= maxFrame)
			dst->b = mColorB[maxFrame - 1 + mTable[col][idx].mBOffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->b = mColorB[(s32)mFrame + mTable[col][idx].mBOffset];
	}
	{
		u16 maxFrame = mTable[col][idx].mAMaxFrame;
		if (mFrame < 0.0f)
			dst->a = mColorA[mTable[col][idx].mAOffset];

		if (mFrame >= maxFrame)
			dst->a = mColorA[maxFrame - 1 + mTable[col][idx].mAOffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->a = mColorA[(s32)mFrame + mTable[col][idx].mAOffset];
	}
}

void J3DAnmVtxColorKey::getColor(u8 col, u16 idx, GXColor* dst) const
{
	switch (mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::RED].mMaxFrame) {
	case 0:
		dst->r = 0;
		break;
	case 1:
		dst->r = mColorR
		    [mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::RED].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::RED],
		    &mColorR[mTable[col][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::RED]
		                 .mOffset]);

		if (frame < 0.0f)
			dst->r = 0;
		if (frame > 255.0f)
			dst->r = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->r = (u8)frame;
	}

	switch (mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mMaxFrame) {
	case 0:
		dst->g = 0;
		break;
	case 1:
		dst->g = mColorG
		    [mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::GREEN],
		    &mColorG[mTable[col][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::GREEN]
		                 .mOffset]);

		if (frame < 0.0f)
			dst->g = 0;
		if (frame > 255.0f)
			dst->g = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->g = (u8)frame;
	}

	switch (mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mMaxFrame) {
	case 0:
		dst->b = 0;
		break;
	case 1:
		dst->b = mColorB
		    [mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::BLUE],
		    &mColorB[mTable[col][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::BLUE]
		                 .mOffset]);

		if (frame < 0.0f)
			dst->b = 0;
		if (frame > 255.0f)
			dst->b = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->b = (u8)frame;
	}

	switch (mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mMaxFrame) {
	case 0:
		dst->a = 0;
		break;
	case 1:
		dst->a = mColorA
		    [mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[col][idx].mColorInfo[J3DAnmColorKeyTable::ALPHA],
		    &mColorA[mTable[col][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::ALPHA]
		                 .mOffset]);

		if (frame < 0.0f)
			dst->a = 0;
		if (frame > 255.0f)
			dst->a = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->a = (u8)frame;
	}
}

void J3DAnmColor::searchUpdateMaterialID(J3DModelData* table)
{
	for (u16 i = 0; i < mUpdateMaterialNum; i++) {
		s32 materialID = table->getMaterialName()->getIndex(
		    mUpdateMaterialName->getName(i));
		if (materialID != -1)
			mUpdateMaterialID[i] = materialID;
		else
			mUpdateMaterialID[i] = 0xFFFF;
	}
}

void J3DAnmColorFull::getColor(u16 idx, GXColor* dst) const
{
	{
		u16 maxFrame = mAnmTable[idx].mRMaxFrame;
		if (mFrame < 0.0f)
			dst->r = mColorR[mAnmTable[idx].mROffset];
		if (mFrame >= maxFrame)
			dst->r = mColorR[maxFrame - 1 + mAnmTable[idx].mROffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->r = mColorR[(s32)mFrame + mAnmTable[idx].mROffset];
	}
	{
		u16 maxFrame = mAnmTable[idx].mGMaxFrame;
		if (mFrame < 0.0f)
			dst->g = mColorG[mAnmTable[idx].mGOffset];
		if (mFrame >= maxFrame)
			dst->g = mColorG[maxFrame - 1 + mAnmTable[idx].mGOffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->g = mColorG[(s32)mFrame + mAnmTable[idx].mGOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx].mBMaxFrame;
		if (mFrame < 0.0f)
			dst->b = mColorB[mAnmTable[idx].mBOffset];
		if (mFrame >= maxFrame)
			dst->b = mColorB[maxFrame - 1 + mAnmTable[idx].mBOffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->b = mColorB[(s32)mFrame + mAnmTable[idx].mBOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx].mAMaxFrame;
		if (mFrame < 0.0f)
			dst->a = mColorA[mAnmTable[idx].mAOffset];
		if (mFrame >= maxFrame)
			dst->a = mColorA[maxFrame - 1 + mAnmTable[idx].mAOffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			dst->a = mColorA[(s32)mFrame + mAnmTable[idx].mAOffset];
	}
}

void J3DAnmColorKey::getColor(u16 idx, GXColor* dst) const
{
	f32 frame;
	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED].mMaxFrame) {
	case 0:
		dst->r = 0;
		break;
	case 1:
		dst->r
		    = mColorR[mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED],
		    &mColorR[mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED].mOffset]);

		if (frame < 0.0f)
			dst->r = 0;
		if (frame > 255.0f)
			dst->r = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->r = frame;
	}

	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mMaxFrame) {
	case 0:
		dst->g = 0;
		break;
	case 1:
		dst->g = mColorG
		    [mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN],
		    &mColorG
		        [mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mOffset]);

		if (frame < 0.0f)
			dst->g = 0;
		if (frame > 255.0f)
			dst->g = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->g = frame;
	}

	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mMaxFrame) {
	case 0:
		dst->b = 0;
		break;
	case 1:
		dst->b = mColorB
		    [mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE],
		    &mColorB
		        [mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mOffset]);

		if (frame < 0.0f)
			dst->b = 0;
		if (frame > 255.0f)
			dst->b = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->b = frame;
	}

	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mMaxFrame) {
	case 0:
		dst->a = 0;
		break;
	case 1:
		dst->a = mColorA
		    [mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA],
		    &mColorA
		        [mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mOffset]);

		if (frame < 0.0f)
			dst->a = 0;
		if (frame > 255.0f)
			dst->a = 255;
		if (0.0f <= frame && frame <= 255.0f)
			dst->a = frame;
	}
}

void J3DAnmTexPattern::getTexNo(u16 idx, u16* dst) const
{
	u16 maxFrame = mAnmTable[idx].mMaxFrame;
	if (0.0f <= mFrame && mFrame < maxFrame)
		*dst = mTextureIndex[(s32)mFrame + mAnmTable[idx].mOffset];
	if (mFrame < 0.0f)
		*dst = mTextureIndex[mAnmTable[idx].mOffset];
	if (mFrame >= maxFrame)
		*dst = mTextureIndex[maxFrame - 1 + mAnmTable[idx].mOffset];
}

void J3DAnmTexPattern::searchUpdateMaterialID(J3DModelData* table)
{
	for (u16 i = 0; i < mUpdateMaterialNum; i++) {
		s32 materialID = table->getMaterialName()->getIndex(
		    mUpdateMaterialName->getName(i));
		if (materialID != -1)
			mUpdateMaterialID[i] = materialID;
		else
			mUpdateMaterialID[i] = 0xFFFF;
	}
}

void J3DAnmTextureSRTKey::searchUpdateMaterialID(J3DModelData* table)
{
	for (u16 i = 0; i < getUpdateMaterialNum(); i++) {
		s32 materialID = table->getMaterialName()->getIndex(
		    mUpdateMaterialName->getName(i));
		if (materialID != -1)
			mUpdateMaterialID[i] = materialID;
		else
			mUpdateMaterialID[i] = 0xFFFF;
	}
	for (u16 i = 0; i < getPostUpdateMaterialNum(); i++) {
		s32 materialID = table->getMaterialName()->getIndex(
		    mPostUpdateMaterialName->getName(i));
		if (materialID != -1)
			mPostUpdateMaterialID[i] = materialID;
		else
			mPostUpdateMaterialID[i] = 0xFFFF;
	}
}

void J3DAnmTevRegKey::getTevColorReg(u16 idx, GXColorS10* dst) const
{
	{
		switch (mAnmCRegKeyTable[idx].mRTable.mMaxFrame) {
		case 0:
			dst->r = 0;
			break;
		case 1:
			dst->r = mAnmCRegDataR[mAnmCRegKeyTable[idx].mRTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mRTable,
			    &mAnmCRegDataR[mAnmCRegKeyTable[idx].mRTable.mOffset]);
			if (v < -1024.0f)
				dst->r = -1024;
			if (v > 1023.0f)
				dst->r = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				dst->r = v;
			break;
		}
	}
	{
		switch (mAnmCRegKeyTable[idx].mGTable.mMaxFrame) {
		case 0:
			dst->g = 0;
			break;
		case 1:
			dst->g = mAnmCRegDataG[mAnmCRegKeyTable[idx].mGTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mGTable,
			    &mAnmCRegDataG[mAnmCRegKeyTable[idx].mGTable.mOffset]);
			if (v < -1024.0f)
				dst->g = -1024;
			if (v > 1023.0f)
				dst->g = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				dst->g = v;
			break;
		}
	}
	{
		switch (mAnmCRegKeyTable[idx].mBTable.mMaxFrame) {
		case 0:
			dst->b = 0;
			break;
		case 1:
			dst->b = mAnmCRegDataB[mAnmCRegKeyTable[idx].mBTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mBTable,
			    &mAnmCRegDataB[mAnmCRegKeyTable[idx].mBTable.mOffset]);
			if (v < -1024.0f)
				dst->b = -1024;
			if (v > 1023.0f)
				dst->b = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				dst->b = v;
			break;
		}
	}
	{
		switch (mAnmCRegKeyTable[idx].mATable.mMaxFrame) {
		case 0:
			dst->a = 0;
			break;
		case 1:
			dst->a = mAnmCRegDataA[mAnmCRegKeyTable[idx].mATable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mATable,
			    &mAnmCRegDataA[mAnmCRegKeyTable[idx].mATable.mOffset]);
			if (v < -1024.0f)
				dst->a = -1024;
			if (v > 1023.0f)
				dst->a = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				dst->a = v;
			break;
		}
	}
}

void J3DAnmTevRegKey::getTevKonstReg(u16 idx, GXColor* dst) const
{
	{
		switch (mAnmKRegKeyTable[idx].mRTable.mMaxFrame) {
		case 0:
			dst->r = 0;
			break;
		case 1:
			dst->r = mAnmKRegDataR[mAnmKRegKeyTable[idx].mRTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mRTable,
			    &mAnmKRegDataR[mAnmKRegKeyTable[idx].mRTable.mOffset]);
			if (v < 0.0f)
				dst->r = 0;
			if (v > 255.0f)
				dst->r = 255;
			if (0.0f <= v && v <= 255.0f)
				dst->r = v;
			break;
		}
	}
	{
		switch (mAnmKRegKeyTable[idx].mGTable.mMaxFrame) {
		case 0:
			dst->g = 0;
			break;
		case 1:
			dst->g = mAnmKRegDataG[mAnmKRegKeyTable[idx].mGTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mGTable,
			    &mAnmKRegDataG[mAnmKRegKeyTable[idx].mGTable.mOffset]);
			if (v < 0.0f)
				dst->g = 0;
			if (v > 255.0f)
				dst->g = 255;
			if (0.0f <= v && v <= 255.0f)
				dst->g = v;
			break;
		}
	}
	{
		switch (mAnmKRegKeyTable[idx].mBTable.mMaxFrame) {
		case 0:
			dst->b = 0;
			break;
		case 1:
			dst->b = mAnmKRegDataB[mAnmKRegKeyTable[idx].mBTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mBTable,
			    &mAnmKRegDataB[mAnmKRegKeyTable[idx].mBTable.mOffset]);
			if (v < 0.0f)
				dst->b = 0;
			if (v > 255.0f)
				dst->b = 255;
			if (0.0f <= v && v <= 255.0f)
				dst->b = v;
			break;
		}
	}
	{
		switch (mAnmKRegKeyTable[idx].mATable.mMaxFrame) {
		case 0:
			dst->a = 0;
			break;
		case 1:
			dst->a = mAnmKRegDataA[mAnmKRegKeyTable[idx].mATable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mATable,
			    &mAnmKRegDataA[mAnmKRegKeyTable[idx].mATable.mOffset]);
			if (v < 0.0f)
				dst->a = 0;
			if (v > 255.0f)
				dst->a = 255;
			if (0.0f <= v && v <= 255.0f)
				dst->a = v;
			break;
		}
	}
}

void J3DAnmTevRegKey::searchUpdateMaterialID(J3DModelData* table)
{
	for (u16 i = 0; i < mCRegUpdateMaterialNum; i++) {
		s32 materialID = table->getMaterialName()->getIndex(
		    mCRegUpdateMaterialName->getName(i));
		if (materialID != -1)
			mCRegUpdateMaterialID[i] = materialID;
		else
			mCRegUpdateMaterialID[i] = 0xFFFF;
	}
	for (u16 i = 0; i < mKRegUpdateMaterialNum; i++) {
		s32 materialID = table->getMaterialName()->getIndex(
		    mKRegUpdateMaterialName->getName(i));
		if (materialID != -1)
			mKRegUpdateMaterialID[i] = materialID;
		else
			mKRegUpdateMaterialID[i] = 0xFFFF;
	}
}
