#include <Camera/CameraShake.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JGeometry/JGRotation3.hpp>
#include <JSystem/JGeometry/JGMatrix33.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <JSystem/JMath.hpp>
#include <macros.h>

TCameraShake* gpCameraShake;

// fabricated
static void unitVecTo(const Vec& from, const Vec& to,
                      JGeometry::TVec3<f32>* out)
{
	out->set(to.x - from.x, to.y - from.y, to.z - from.z);
	out->normalize();
}

#pragma strength off

TCameraShake::TCameraShake()
{
	mRollAccum = 0;

	for (int i = 0; i < ARRAY_COUNT(mShakeData); ++i)
		mShakeData[i] = new TCamSaveShake(mCamShakeNameSave[i]);

	for (int i = 0; i < ARRAY_COUNT(mShakeInfo); ++i)
		mShakeInfo[i].reset();
}

TCameraShake::TCamShakeInfo* TCameraShake::getUseShakeData_()
{
	int i;

	for (i = 0; i < ARRAY_COUNT(mShakeInfo); ++i)
		if (!mShakeInfo[i].isActive())
			return &mShakeInfo[i];

	TCamShakeInfo* best = mShakeInfo;
	u16 minRemaining    = -1;
	for (i = 0; i < ARRAY_COUNT(mShakeInfo); ++i) {
		if (mShakeInfo[i].mIsDecreasing != 0) {
			u16 remaining = mShakeInfo[i].mDuration - mShakeInfo[i].mFrame;
			if (remaining < minRemaining) {
				minRemaining = remaining;
				best         = &mShakeInfo[i];
			}
		}
	}
	return best;
}

void TCameraShake::setShakeAngleOne_(TCameraShake::TCamShakeAngle* angle,
                                     f32 amp, s16 vel, u16 duration, f32 scale)
{
	f32 a = amp * scale;
	s16 v = vel;
	if (scale < 0.0f) {
		a = -a;
		v = -v;
	}
	angle->mAmp = a;
	angle->mDec = a * (1.0f / (f32)duration);
	angle->mVel = v;
}

void TCameraShake::setShakeAngleAll_(TCameraShake::TCamShakeInfo* info,
                                     const TCamSaveShake* save, u16 duration,
                                     f32 scale)
{
	setShakeAngleOne_(&info->mAngleX, save->mShakeAmpX.get(),
	                  save->mShakeVelX.get(), duration, scale);
	setShakeAngleOne_(&info->mAngleY, save->mShakeAmpY.get(),
	                  save->mShakeVelY.get(), duration, scale);
	setShakeAngleOne_(&info->mAngleZ, save->mShakeAmpZ.get(),
	                  save->mShakeVelZ.get(), duration, scale);
}

void TCameraShake::startShake(EnumCamShakeMode mode, f32 scale)
{
	TCamSaveShake* save = mShakeData[mode];
	u16 duration        = save->mShakeTime.get();
	if (duration != 0) {
		TCamShakeInfo* info = getUseShakeData_();
		info->mMode         = mode;
		info->mIsKeep       = 0;
		info->mIsDecreasing = 0;
		info->mDuration     = duration;
		info->mFrame        = 0;
		setShakeAngleAll_(info, save, duration, scale);
	}
}

void TCameraShake::keepShake(EnumCamShakeMode mode, f32 scale)
{
	TCamSaveShake* save = mShakeData[mode];
	u16 duration        = (u16)save->mShakeTime.get();
	if (duration != 0) {
		TCamShakeInfo* it = mShakeInfo;
		for (int i = 0; i < ARRAY_COUNT(mShakeInfo); ++i, ++it) {
			if (it->mMode == mode && it->mIsDecreasing == 0) {
				it->mIsKeep = 1;
				return;
			}
		}

		TCamShakeInfo* info = getUseShakeData_();
		info->mMode         = mode;
		info->mIsKeep       = 1;
		info->mIsDecreasing = 0;
		info->mDuration     = duration;
		info->mFrame        = 0;
		setShakeAngleAll_(info, save, duration, scale);
	}
}

void TCameraShake::execShake(const JGeometry::TVec3<f32>& origin,
                             JGeometry::TVec3<f32>* pos,
                             JGeometry::TVec3<f32>* up)
{
	bool anyActive                = false;
	JGeometry::TVec3<f32> origPos = *pos;

	mRollAccum = 0;

	TCamShakeInfo* it = mShakeInfo;
	for (int i = 0; i < ARRAY_COUNT(mShakeInfo); ++i, ++it) {
		if (it->isActive()) {
			anyActive = true;
			break;
		}
	}

	if (anyActive) {
		f32 r;
		s16 vAngle, hAngle;
		CLBCrossToPolar(origin, *pos, &r, &vAngle, &hAngle);

		TCamShakeInfo* it = mShakeInfo;
		for (int i = 0; i < ARRAY_COUNT(mShakeInfo); ++i, ++it) {
			if (it->isActive()) {
				vAngle
				    += (s16)(it->mAngleX.mAmp
				             * JMASSin((s16)(it->mAngleX.mVel * it->mFrame)));
				hAngle
				    += (s16)(it->mAngleY.mAmp
				             * JMASSin((s16)(it->mAngleY.mVel * it->mFrame)));
				mRollAccum
				    += (s16)(it->mAngleZ.mAmp
				             * JMASSin((s16)(it->mAngleZ.mVel * it->mFrame)));
				it->mFrame += 1;

				bool finished = false;
				if (it->mIsKeep != 0) {
					it->mDuration += 1;
					it->mIsKeep = 0;
				} else {
					it->mIsDecreasing = 1;
					it->mAngleX.mAmp -= it->mAngleX.mDec;
					it->mAngleY.mAmp -= it->mAngleY.mDec;
					it->mAngleZ.mAmp -= it->mAngleZ.mDec;
					if (it->mFrame >= it->mDuration)
						finished = true;
				}

				if (finished)
					it->reset();
			}
		}

		CLBPolarToCross(origin, pos, r, vAngle, hAngle);
	}

	JGeometry::TVec3<f32> dir;
	unitVecTo(*pos, origPos, &dir);

	JGeometry::TRotation3<TMtx33f> rot(
	    dir, -(0.017453294f * (0.005493164f * (f32)mRollAccum)));

	JGeometry::TVec3<f32> oldUp = *up;
	up->x = oldUp.x * rot.at(0, 0) + oldUp.y * rot.at(1, 0)
	        + oldUp.z * rot.at(2, 0);
	up->y = oldUp.x * rot.at(0, 1) + oldUp.y * rot.at(1, 1)
	        + oldUp.z * rot.at(2, 1);
	up->z = oldUp.x * rot.at(0, 2) + oldUp.y * rot.at(1, 2)
	        + oldUp.z * rot.at(2, 2);
}
