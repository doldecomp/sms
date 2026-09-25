#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DStruct.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JMath.hpp>

void J3DFrameCtrl::init(s16 end)
{
	mAttribute = ATTR_LOOP;
	mState     = 0;
	mStart     = 0;
	mEnd       = end;
	mLoop      = 0;
	mRate      = 1.0;
	mFrame     = 0.0;
}

BOOL J3DFrameCtrl::checkPass(f32 pass_frame)
{
	f32 cur_frame  = mFrame;
	f32 next_frame = cur_frame + mRate;

	switch (mAttribute) {
	case ATTR_ONCE:
	case ATTR_ONCE_AND_RESET:
		if (next_frame < mStart) {
			next_frame = mStart;
		}

		if (next_frame >= mEnd) {
			next_frame = mEnd - 0.001f;
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

	case ATTR_LOOP:
		if (cur_frame < mStart) {
			while (next_frame < mStart) {
				if (mLoop - mStart <= 0.0f) {
					break;
				}
				next_frame += mLoop - mStart;
			}

			if (next_frame <= pass_frame && pass_frame < mLoop) {
				return true;
			} else {
				return false;
			}
		} else if (mEnd <= cur_frame) {
			while (next_frame >= mEnd) {
				if (mEnd - mLoop <= 0.0f) {
					break;
				}
				next_frame -= mEnd - mLoop;
			}

			if (mLoop <= pass_frame && pass_frame < next_frame) {
				return true;
			} else {
				return false;
			}
		} else if (next_frame < mStart) {
			while (next_frame < mStart) {
				if (mLoop - mStart <= 0.0f) {
					break;
				}
				next_frame += mLoop - mStart;
			}

			if ((mStart <= pass_frame && pass_frame < cur_frame)
			    || (next_frame <= pass_frame && pass_frame < mLoop)) {
				return true;
			} else {
				return false;
			}
		} else if (mEnd <= next_frame) {
			while (next_frame >= mEnd) {
				if (mEnd - mLoop <= 0.0f) {
					break;
				}

				next_frame -= mEnd - mLoop;
			}

			if ((cur_frame <= pass_frame && pass_frame < mEnd)
			    || (mLoop <= pass_frame && pass_frame < next_frame)) {
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

	case ATTR_PING_PONG:
	case ATTR_PING_PONG_LOOP:
		if (next_frame >= mEnd) {
			next_frame = mEnd - 0.001f;
		}

		if (next_frame < mStart) {
			next_frame = mStart;
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
	mState = 0;
	mFrame += mRate;
	switch (mAttribute) {
	case ATTR_ONCE:
		if (mFrame < mStart) {
			mFrame = mStart;
			mRate  = 0.0f;
			mState |= STATE_COMPLETED_ONCE;
		}
		if (mFrame >= mEnd) {
			mFrame = mEnd - 0.001f;
			mRate  = 0.0f;
			mState |= STATE_COMPLETED_ONCE;
		}
		break;

	case ATTR_ONCE_AND_RESET:
		if (mFrame < mStart) {
			mFrame = mStart;
			mRate  = 0.0f;
			mState |= STATE_COMPLETED_ONCE;
		}
		if (mFrame >= mEnd) {
			mFrame = mStart;
			mRate  = 0.0f;
			mState |= STATE_COMPLETED_ONCE;
		}
		break;

	case ATTR_LOOP:
		while (mFrame < mStart) {
			mState |= STATE_LOOPED_ONCE;
			if (mLoop - mStart <= 0.0f) {
				break;
			}
			mFrame += mLoop - mStart;
		}

		while (mFrame >= mEnd) {
			mState |= STATE_LOOPED_ONCE;
			if (mEnd - mLoop <= 0.0f) {
				break;
			}
			mFrame -= mEnd - mLoop;
		}
		break;

	case ATTR_PING_PONG:
		if (mFrame >= mEnd) {
			mFrame = mEnd - 0.001f;
			mRate  = -mRate;
		}
		if (mFrame < mStart) {
			mFrame = mStart;
			mRate  = 0.0f;
			mState |= STATE_COMPLETED_ONCE;
		}
		break;

	case ATTR_PING_PONG_LOOP:
		if (mFrame >= mEnd) {
			mFrame = mEnd - 0.001f;
			mRate  = -mRate;
		}
		if (mFrame < mStart) {
			mFrame = mStart;
			mRate  = -mRate;
			mState |= STATE_LOOPED_ONCE;
		}
		break;
	}
}

J3DAnmBase::J3DAnmBase()
{
	mFrame    = 0.0f;
	mMaxFrame = 0;
}

J3DAnmBase::J3DAnmBase(s16 maxFrame)
{
	mFrame    = 0.0f;
	mMaxFrame = maxFrame;
}

J3DAnmBase::~J3DAnmBase() { }

void J3DAnmTransformFull::getTransform(u16 idx, J3DTransformInfo* out) const
{
	/* Scale */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mScale.x
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 0].mScaleOffset];
		} else if (mFrame < 0.0f)
			out->mScale.x = mScaleData[mAnmTable[idx * 3 + 0].mScaleOffset];
		else
			out->mScale.x = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 0].mScaleOffset];
	}
	/* Seems they copy-pasted this block one too many times? */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mScale.x
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 0].mScaleOffset];
		} else if (mFrame < 0.0f)
			out->mScale.x = mScaleData[mAnmTable[idx * 3 + 0].mScaleOffset];
		else
			out->mScale.x = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 0].mScaleOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 1].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mScale.y
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 1].mScaleOffset];
		} else if (mFrame < 0.0f)
			out->mScale.y = mScaleData[mAnmTable[idx * 3 + 1].mScaleOffset];
		else
			out->mScale.y = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 1].mScaleOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 2].mScaleMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mScale.z
			    = mScaleData[(s32)mFrame + mAnmTable[idx * 3 + 2].mScaleOffset];
		} else if (mFrame < 0.0f)
			out->mScale.z = mScaleData[mAnmTable[idx * 3 + 2].mScaleOffset];
		else
			out->mScale.z = mScaleData[maxFrame - 1
			                           + mAnmTable[idx * 3 + 2].mScaleOffset];
	}

	/* Rotation */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mRotationMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mRotation.x
			    = mRotData[(s32)mFrame
			               + mAnmTable[idx * 3 + 0].mRotationOffset];
		} else if (mFrame < 0.0f)
			out->mRotation.x = mRotData[mAnmTable[idx * 3 + 0].mRotationOffset];
		else
			out->mRotation.x
			    = mRotData[maxFrame - 1
			               + mAnmTable[idx * 3 + 0].mRotationOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 1].mRotationMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mRotation.y
			    = mRotData[(s32)mFrame
			               + mAnmTable[idx * 3 + 1].mRotationOffset];
		} else if (mFrame < 0.0f)
			out->mRotation.y = mRotData[mAnmTable[idx * 3 + 1].mRotationOffset];
		else
			out->mRotation.y
			    = mRotData[maxFrame - 1
			               + mAnmTable[idx * 3 + 1].mRotationOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 2].mRotationMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mRotation.z
			    = mRotData[(s32)mFrame
			               + mAnmTable[idx * 3 + 2].mRotationOffset];
		} else if (mFrame < 0.0f)
			out->mRotation.z = mRotData[mAnmTable[idx * 3 + 2].mRotationOffset];
		else
			out->mRotation.z
			    = mRotData[maxFrame - 1
			               + mAnmTable[idx * 3 + 2].mRotationOffset];
	}

	/* Translation */
	{
		u16 maxFrame = mAnmTable[idx * 3 + 0].mTranslateMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mTranslate.x
			    = mTransData[(s32)mFrame
			                 + mAnmTable[idx * 3 + 0].mTranslateOffset];
		} else if (mFrame < 0.0f)
			out->mTranslate.x
			    = mTransData[mAnmTable[idx * 3 + 0].mTranslateOffset];
		else
			out->mTranslate.x
			    = mTransData[maxFrame - 1
			                 + mAnmTable[idx * 3 + 0].mTranslateOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 1].mTranslateMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mTranslate.y
			    = mTransData[(s32)mFrame
			                 + mAnmTable[idx * 3 + 1].mTranslateOffset];
		} else if (mFrame < 0.0f)
			out->mTranslate.y
			    = mTransData[mAnmTable[idx * 3 + 1].mTranslateOffset];
		else
			out->mTranslate.y
			    = mTransData[maxFrame - 1
			                 + mAnmTable[idx * 3 + 1].mTranslateOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx * 3 + 2].mTranslateMaxFrame;
		if (0.0f <= mFrame && mFrame < maxFrame) {
			out->mTranslate.z
			    = mTransData[(s32)mFrame
			                 + mAnmTable[idx * 3 + 2].mTranslateOffset];
		} else if (mFrame < 0.0f)
			out->mTranslate.z
			    = mTransData[mAnmTable[idx * 3 + 2].mTranslateOffset];
		else
			out->mTranslate.z
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
                                       J3DTransformInfo* out) const
{
	/* Scale */
	switch (mAnmTable[idx * 3 + 0].mScale.mMaxFrame) {
	case 0:
		out->mScale.x = 1.0f;
		break;
	case 1:
		out->mScale.x = mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset];
		break;
	default:
		out->mScale.x = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mScale.mMaxFrame) {
	case 0:
		out->mScale.y = 1.0f;
		break;
	case 1:
		out->mScale.y = mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset];
		break;
	default:
		out->mScale.y = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 2].mScale.mMaxFrame) {
	case 0:
		out->mScale.z = 1.0f;
		break;
	case 1:
		out->mScale.z = mScaleData[mAnmTable[idx * 3 + 2].mScale.mOffset];
		break;
	default:
		out->mScale.z = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 2].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 2].mScale.mOffset]);
		break;
	}

	/* Rotation */
	switch (mAnmTable[idx * 3 + 0].mRotation.mMaxFrame) {
	case 0:
		out->mRotation.x = 0;
		break;
	case 1:
		out->mRotation.x = mRotData[mAnmTable[idx * 3 + 0].mRotation.mOffset]
		                   << mDecShift;
		break;
	default:
		out->mRotation.x
		    = (s32)(J3DGetKeyFrameInterpolationS(
		          frame, &mAnmTable[idx * 3 + 0].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 0].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mRotation.mMaxFrame) {
	case 0:
		out->mRotation.y = 0;
		break;
	case 1:
		out->mRotation.y = mRotData[mAnmTable[idx * 3 + 1].mRotation.mOffset]
		                   << mDecShift;
		break;
	default:
		out->mRotation.y
		    = (s32)(J3DGetKeyFrameInterpolationS(
		          frame, &mAnmTable[idx * 3 + 1].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 1].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	switch (mAnmTable[idx * 3 + 2].mRotation.mMaxFrame) {
	case 0:
		out->mRotation.z = 0;
		break;
	case 1:
		out->mRotation.z = mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]
		                   << mDecShift;
		break;
	default:
		out->mRotation.z
		    = (s32)(J3DGetKeyFrameInterpolationS(
		          frame, &mAnmTable[idx * 3 + 2].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	/* Translation */
	switch (mAnmTable[idx * 3 + 0].mTranslate.mMaxFrame) {
	case 0:
		out->mTranslate.x = 0.0f;
		break;
	case 1:
		out->mTranslate.x
		    = mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset];
		break;
	default:
		out->mTranslate.x = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mTranslate.mMaxFrame) {
	case 0:
		out->mTranslate.y = 0.0f;
		break;
	case 1:
		out->mTranslate.y
		    = mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset];
		break;
	default:
		out->mTranslate.y = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 2].mTranslate.mMaxFrame) {
	case 0:
		out->mTranslate.z = 0.0f;
		break;
	case 1:
		out->mTranslate.z
		    = mTransData[mAnmTable[idx * 3 + 2].mTranslate.mOffset];
		break;
	default:
		out->mTranslate.z = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 2].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 2].mTranslate.mOffset]);
		break;
	}
}

void J3DAnmTextureSRTKey::calcTransform(f32 frame, u16 idx,
                                        J3DTextureSRTInfo* out) const
{
	/* Scale */
	switch (mAnmTable[idx * 3 + 0].mScale.mMaxFrame) {
	case 0:
		out->mScaleX = 1.0f;
		break;
	case 1:
		out->mScaleX = mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset];
		break;
	default:
		out->mScaleX = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 0].mScale.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mScale.mMaxFrame) {
	case 0:
		out->mScaleY = 1.0f;
		break;
	case 1:
		out->mScaleY = mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset];
		break;
	default:
		out->mScaleY = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 1].mScale,
		    &mScaleData[mAnmTable[idx * 3 + 1].mScale.mOffset]);
		break;
	}

	/* Rotation */
	switch (mAnmTable[idx * 3 + 2].mRotation.mMaxFrame) {
	case 0:
		out->mRotation = 0;
		break;
	case 1:
		out->mRotation = mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]
		                 << mDecShift;
		break;
	default:
		out->mRotation
		    = (s32)(J3DGetKeyFrameInterpolation<s16>(
		          frame, &mAnmTable[idx * 3 + 2].mRotation,
		          &mRotData[mAnmTable[idx * 3 + 2].mRotation.mOffset]))
		      << mDecShift;
		break;
	}

	/* Translation */
	switch (mAnmTable[idx * 3 + 0].mTranslate.mMaxFrame) {
	case 0:
		out->mTranslationX = 0.0f;
		break;
	case 1:
		out->mTranslationX
		    = mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset];
		break;
	default:
		out->mTranslationX = J3DGetKeyFrameInterpolation<f32>(
		    frame, &mAnmTable[idx * 3 + 0].mTranslate,
		    &mTransData[mAnmTable[idx * 3 + 0].mTranslate.mOffset]);
		break;
	}

	switch (mAnmTable[idx * 3 + 1].mTranslate.mMaxFrame) {
	case 0:
		out->mTranslationY = 0.0f;
		break;
	case 1:
		out->mTranslationY
		    = mTransData[mAnmTable[idx * 3 + 1].mTranslate.mOffset];
		break;
	default:
		out->mTranslationY = J3DGetKeyFrameInterpolation<f32>(
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

void J3DAnmVtxColorFull::getColor(u8 chan, u16 idx, GXColor* out) const
{
	{
		u16 maxFrame = mTable[chan][idx].mRMaxFrame;
		if (mFrame < 0.0f)
			out->r = mColorR[mTable[chan][idx].mROffset];

		if (mFrame >= maxFrame)
			out->r = mColorR[maxFrame - 1 + mTable[chan][idx].mROffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			out->r = mColorR[(s32)mFrame + mTable[chan][idx].mROffset];
	}
	{
		u16 maxFrame = mTable[chan][idx].mGMaxFrame;
		if (mFrame < 0.0f)
			out->g = mColorG[mTable[chan][idx].mGOffset];

		if (mFrame >= maxFrame)
			out->g = mColorG[maxFrame - 1 + mTable[chan][idx].mGOffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			out->g = mColorG[(s32)mFrame + mTable[chan][idx].mGOffset];
	}
	{
		u16 maxFrame = mTable[chan][idx].mBMaxFrame;
		if (mFrame < 0.0f)
			out->b = mColorB[mTable[chan][idx].mBOffset];

		if (mFrame >= maxFrame)
			out->b = mColorB[maxFrame - 1 + mTable[chan][idx].mBOffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			out->b = mColorB[(s32)mFrame + mTable[chan][idx].mBOffset];
	}
	{
		u16 maxFrame = mTable[chan][idx].mAMaxFrame;
		if (mFrame < 0.0f)
			out->a = mColorA[mTable[chan][idx].mAOffset];

		if (mFrame >= maxFrame)
			out->a = mColorA[maxFrame - 1 + mTable[chan][idx].mAOffset];

		if (0.0f <= mFrame && mFrame < maxFrame)
			out->a = mColorA[(s32)mFrame + mTable[chan][idx].mAOffset];
	}
}

void J3DAnmVtxColorKey::getColor(u8 chan, u16 idx, GXColor* out) const
{
	switch (mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::RED].mMaxFrame) {
	case 0:
		out->r = 0;
		break;
	case 1:
		out->r = mColorR
		    [mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::RED].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::RED],
		    &mColorR[mTable[chan][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::RED]
		                 .mOffset]);

		if (frame < 0.0f)
			out->r = 0;
		if (frame > 255.0f)
			out->r = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->r = (u8)frame;
	}

	switch (
	    mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mMaxFrame) {
	case 0:
		out->g = 0;
		break;
	case 1:
		out->g = mColorG
		    [mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::GREEN],
		    &mColorG[mTable[chan][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::GREEN]
		                 .mOffset]);

		if (frame < 0.0f)
			out->g = 0;
		if (frame > 255.0f)
			out->g = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->g = (u8)frame;
	}

	switch (mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mMaxFrame) {
	case 0:
		out->b = 0;
		break;
	case 1:
		out->b = mColorB
		    [mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::BLUE],
		    &mColorB[mTable[chan][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::BLUE]
		                 .mOffset]);

		if (frame < 0.0f)
			out->b = 0;
		if (frame > 255.0f)
			out->b = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->b = (u8)frame;
	}

	switch (
	    mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mMaxFrame) {
	case 0:
		out->a = 0;
		break;
	case 1:
		out->a = mColorA
		    [mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mOffset];
		break;
	default:
		f32 frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[chan][idx].mColorInfo[J3DAnmColorKeyTable::ALPHA],
		    &mColorA[mTable[chan][idx]
		                 .mColorInfo[J3DAnmColorKeyTable::ALPHA]
		                 .mOffset]);

		if (frame < 0.0f)
			out->a = 0;
		if (frame > 255.0f)
			out->a = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->a = (u8)frame;
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

void J3DAnmColorFull::getColor(u16 idx, GXColor* out) const
{
	{
		u16 maxFrame = mAnmTable[idx].mRMaxFrame;
		if (mFrame < 0.0f)
			out->r = mColorR[mAnmTable[idx].mROffset];
		if (mFrame >= maxFrame)
			out->r = mColorR[maxFrame - 1 + mAnmTable[idx].mROffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			out->r = mColorR[(s32)mFrame + mAnmTable[idx].mROffset];
	}
	{
		u16 maxFrame = mAnmTable[idx].mGMaxFrame;
		if (mFrame < 0.0f)
			out->g = mColorG[mAnmTable[idx].mGOffset];
		if (mFrame >= maxFrame)
			out->g = mColorG[maxFrame - 1 + mAnmTable[idx].mGOffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			out->g = mColorG[(s32)mFrame + mAnmTable[idx].mGOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx].mBMaxFrame;
		if (mFrame < 0.0f)
			out->b = mColorB[mAnmTable[idx].mBOffset];
		if (mFrame >= maxFrame)
			out->b = mColorB[maxFrame - 1 + mAnmTable[idx].mBOffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			out->b = mColorB[(s32)mFrame + mAnmTable[idx].mBOffset];
	}
	{
		u16 maxFrame = mAnmTable[idx].mAMaxFrame;
		if (mFrame < 0.0f)
			out->a = mColorA[mAnmTable[idx].mAOffset];
		if (mFrame >= maxFrame)
			out->a = mColorA[maxFrame - 1 + mAnmTable[idx].mAOffset];
		if (0.0f <= mFrame && mFrame < maxFrame)
			out->a = mColorA[(s32)mFrame + mAnmTable[idx].mAOffset];
	}
}

void J3DAnmColorKey::getColor(u16 idx, GXColor* out) const
{
	f32 frame;
	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED].mMaxFrame) {
	case 0:
		out->r = 0;
		break;
	case 1:
		out->r
		    = mColorR[mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED],
		    &mColorR[mTable[idx].mColorInfo[J3DAnmColorKeyTable::RED].mOffset]);

		if (frame < 0.0f)
			out->r = 0;
		if (frame > 255.0f)
			out->r = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->r = frame;
	}

	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mMaxFrame) {
	case 0:
		out->g = 0;
		break;
	case 1:
		out->g = mColorG
		    [mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN],
		    &mColorG
		        [mTable[idx].mColorInfo[J3DAnmColorKeyTable::GREEN].mOffset]);

		if (frame < 0.0f)
			out->g = 0;
		if (frame > 255.0f)
			out->g = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->g = frame;
	}

	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mMaxFrame) {
	case 0:
		out->b = 0;
		break;
	case 1:
		out->b = mColorB
		    [mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE],
		    &mColorB
		        [mTable[idx].mColorInfo[J3DAnmColorKeyTable::BLUE].mOffset]);

		if (frame < 0.0f)
			out->b = 0;
		if (frame > 255.0f)
			out->b = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->b = frame;
	}

	switch (mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mMaxFrame) {
	case 0:
		out->a = 0;
		break;
	case 1:
		out->a = mColorA
		    [mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mOffset];
		break;
	default:
		frame = J3DGetKeyFrameInterpolation<s16>(
		    mFrame, &mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA],
		    &mColorA
		        [mTable[idx].mColorInfo[J3DAnmColorKeyTable::ALPHA].mOffset]);

		if (frame < 0.0f)
			out->a = 0;
		if (frame > 255.0f)
			out->a = 255;
		if (0.0f <= frame && frame <= 255.0f)
			out->a = frame;
	}
}

void J3DAnmTexPattern::getTexNo(u16 idx, u16* out) const
{
	u16 maxFrame = mAnmTable[idx].mMaxFrame;
	if (0.0f <= mFrame && mFrame < maxFrame)
		*out = mTextureIndex[(s32)mFrame + mAnmTable[idx].mOffset];
	if (mFrame < 0.0f)
		*out = mTextureIndex[mAnmTable[idx].mOffset];
	if (mFrame >= maxFrame)
		*out = mTextureIndex[maxFrame - 1 + mAnmTable[idx].mOffset];
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

void J3DAnmTevRegKey::getTevColorReg(u16 idx, GXColorS10* out) const
{
	{
		switch (mAnmCRegKeyTable[idx].mRTable.mMaxFrame) {
		case 0:
			out->r = 0;
			break;
		case 1:
			out->r = mAnmCRegDataR[mAnmCRegKeyTable[idx].mRTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mRTable,
			    &mAnmCRegDataR[mAnmCRegKeyTable[idx].mRTable.mOffset]);
			if (v < -1024.0f)
				out->r = -1024;
			if (v > 1023.0f)
				out->r = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				out->r = v;
			break;
		}
	}
	{
		switch (mAnmCRegKeyTable[idx].mGTable.mMaxFrame) {
		case 0:
			out->g = 0;
			break;
		case 1:
			out->g = mAnmCRegDataG[mAnmCRegKeyTable[idx].mGTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mGTable,
			    &mAnmCRegDataG[mAnmCRegKeyTable[idx].mGTable.mOffset]);
			if (v < -1024.0f)
				out->g = -1024;
			if (v > 1023.0f)
				out->g = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				out->g = v;
			break;
		}
	}
	{
		switch (mAnmCRegKeyTable[idx].mBTable.mMaxFrame) {
		case 0:
			out->b = 0;
			break;
		case 1:
			out->b = mAnmCRegDataB[mAnmCRegKeyTable[idx].mBTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mBTable,
			    &mAnmCRegDataB[mAnmCRegKeyTable[idx].mBTable.mOffset]);
			if (v < -1024.0f)
				out->b = -1024;
			if (v > 1023.0f)
				out->b = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				out->b = v;
			break;
		}
	}
	{
		switch (mAnmCRegKeyTable[idx].mATable.mMaxFrame) {
		case 0:
			out->a = 0;
			break;
		case 1:
			out->a = mAnmCRegDataA[mAnmCRegKeyTable[idx].mATable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmCRegKeyTable[idx].mATable,
			    &mAnmCRegDataA[mAnmCRegKeyTable[idx].mATable.mOffset]);
			if (v < -1024.0f)
				out->a = -1024;
			if (v > 1023.0f)
				out->a = 1023;
			if (-1024.0f <= v && v <= 1023.0f)
				out->a = v;
			break;
		}
	}
}

void J3DAnmTevRegKey::getTevKonstReg(u16 idx, GXColor* out) const
{
	{
		switch (mAnmKRegKeyTable[idx].mRTable.mMaxFrame) {
		case 0:
			out->r = 0;
			break;
		case 1:
			out->r = mAnmKRegDataR[mAnmKRegKeyTable[idx].mRTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mRTable,
			    &mAnmKRegDataR[mAnmKRegKeyTable[idx].mRTable.mOffset]);
			if (v < 0.0f)
				out->r = 0;
			if (v > 255.0f)
				out->r = 255;
			if (0.0f <= v && v <= 255.0f)
				out->r = v;
			break;
		}
	}
	{
		switch (mAnmKRegKeyTable[idx].mGTable.mMaxFrame) {
		case 0:
			out->g = 0;
			break;
		case 1:
			out->g = mAnmKRegDataG[mAnmKRegKeyTable[idx].mGTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mGTable,
			    &mAnmKRegDataG[mAnmKRegKeyTable[idx].mGTable.mOffset]);
			if (v < 0.0f)
				out->g = 0;
			if (v > 255.0f)
				out->g = 255;
			if (0.0f <= v && v <= 255.0f)
				out->g = v;
			break;
		}
	}
	{
		switch (mAnmKRegKeyTable[idx].mBTable.mMaxFrame) {
		case 0:
			out->b = 0;
			break;
		case 1:
			out->b = mAnmKRegDataB[mAnmKRegKeyTable[idx].mBTable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mBTable,
			    &mAnmKRegDataB[mAnmKRegKeyTable[idx].mBTable.mOffset]);
			if (v < 0.0f)
				out->b = 0;
			if (v > 255.0f)
				out->b = 255;
			if (0.0f <= v && v <= 255.0f)
				out->b = v;
			break;
		}
	}
	{
		switch (mAnmKRegKeyTable[idx].mATable.mMaxFrame) {
		case 0:
			out->a = 0;
			break;
		case 1:
			out->a = mAnmKRegDataA[mAnmKRegKeyTable[idx].mATable.mOffset];
			break;
		default:
			f32 v = J3DGetKeyFrameInterpolation<s16>(
			    mFrame, &mAnmKRegKeyTable[idx].mATable,
			    &mAnmKRegDataA[mAnmKRegKeyTable[idx].mATable.mOffset]);
			if (v < 0.0f)
				out->a = 0;
			if (v > 255.0f)
				out->a = 255;
			if (0.0f <= v && v <= 255.0f)
				out->a = v;
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
