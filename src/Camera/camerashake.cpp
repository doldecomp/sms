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
static inline void unitVecTo(const Vec& from, const Vec& to,
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

// Both parameters are modified in place rather than copied into locals: that
// is what makes retail load the amplitude straight into the multiply's
// destination register (`lfs f3, 0x2c(r31); fmuls f3, f3, f31`) instead of
// loading it into a scratch register first, and it also drops two statements,
// which is what keeps this body inlinable at depth 2 (the `f32 a = amp;
// a *= scale;` split pushed it out of line). Research batch 172.
void TCameraShake::setShakeAngleOne_(TCameraShake::TCamShakeAngle* angle,
                                     f32 amp, s16 vel, u16 duration, f32 scale)
{
	amp *= scale;
	if (scale < 0.0f) {
		amp = -amp;
		vel = -vel;
	}
	angle->mAmp = amp;
	angle->mDec = amp * (1.0f / (f32)duration);
	angle->mVel = vel;
}

void TCameraShake::setShakeAngleAll_(TCameraShake::TCamShakeInfo* info,
                                     const TCamSaveShake* save, u16 duration,
                                     f32 scale)
{
	// All six parameters are read through get(): TParamT<T>::get() is
	// fabricated as returning `const T&`, and a reference return reserves an
	// inline temporary per expansion, which is what reproduces retail's 0x50
	// frame in both callers (reading the velocities through `.value` instead
	// is 0x48). Header round 24 measured the shared-header alternative and it
	// is refuted: a by-value get() for the integer instantiations is total
	// matched_code 53.73 -> 53.24 tree-wide (53.61 for s32 alone, 52.94 for
	// every T), so the reference return is retail's.
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

// execShake is built from six TU-local inline helpers, each one measured:
//   * CSIsAnyActive / CSDecay: the two flag computations as bool-returning
//     helpers with a `ret` local. That is what makes MWCC share one `li 0`
//     (materialised before the prologue) between the flag, the loop counter
//     and `mRollAccum = 0`, and share `finished`'s constants with the
//     mIsKeep/mIsDecreasing stores -- the ten "structural" instructions the
//     old note blamed on constant CSE. `int i;` must be declared before `it`.
//   * CSRotateUp: rot and oldUp as callee block objects, which puts them in
//     retail's order (rot below oldUp, 8 bytes under dir). upZ is named before
//     upY for retail's f4/f5.
//   * CSWaveF (f32 return, narrowed at the call) at all three axes, and
//     CSRollRad over a CSShortToDeg that names its result: together exactly
//     the 56 bytes of pool below rot (s16-returning waves price 0x18, f32
//     0x20; the named/unnamed split of the two angle levels moves 4-byte
//     slots between the pool and the named block).
static inline bool CSIsAnyActive(TCameraShake* s)
{
	bool ret = false;
	int i;
	TCameraShake::TCamShakeInfo* it = s->mShakeInfo;
	for (i = 0; i < ARRAY_COUNT(s->mShakeInfo); ++i, ++it) {
		if (it->isActive()) {
			ret = true;
			break;
		}
	}
	return ret;
}

static inline bool CSDecay(TCameraShake::TCamShakeInfo* it)
{
	bool ret = false;
	if (it->mIsKeep != 0) {
		it->mDuration += 1;
		it->mIsKeep = 0;
	} else {
		it->mIsDecreasing = 1;
		it->mAngleX.mAmp -= it->mAngleX.mDec;
		it->mAngleY.mAmp -= it->mAngleY.mDec;
		it->mAngleZ.mAmp -= it->mAngleZ.mDec;
		if (it->mFrame >= it->mDuration)
			ret = true;
	}
	return ret;
}

static inline void CSRotateUp(JGeometry::TVec3<f32>* up,
                              const JGeometry::TVec3<f32>& dir, f32 angle)
{
	JGeometry::TRotation3<TMtx33f> rot(dir, angle);

	JGeometry::TVec3<f32> oldUp = *up;
	f32 upZ                     = oldUp.z;
	f32 upY                     = oldUp.y;
	up->x = oldUp.x * rot.at(0, 0) + upY * rot.at(1, 0) + upZ * rot.at(2, 0);
	up->y = oldUp.x * rot.at(0, 1) + upY * rot.at(1, 1) + upZ * rot.at(2, 1);
	up->z = oldUp.x * rot.at(0, 2) + upY * rot.at(1, 2) + upZ * rot.at(2, 2);
}

static inline f32 CSWaveF(const TCameraShake::TCamShakeAngle& a, u16 frame)
{
	return a.mAmp * JMASSin((s16)(a.mVel * frame));
}

static inline f32 CSShortToDeg(s16 a)
{
	f32 d = 0.005493164f * a;
	return d;
}

static inline f32 CSRollRad(s16 roll)
{
	return -(0.017453294f * CSShortToDeg(roll));
}

void TCameraShake::execShake(const JGeometry::TVec3<f32>& origin,
                             JGeometry::TVec3<f32>* pos,
                             JGeometry::TVec3<f32>* up)
{
	JGeometry::TVec3<f32> origPos = *pos;
	mRollAccum = 0;
	if (CSIsAnyActive(this)) {
		f32 r;
		s16 vAngle, hAngle;
		CLBCrossToPolar(origin, *pos, &r, &vAngle, &hAngle);

		TCamShakeInfo* it = mShakeInfo;
		for (int i = 0; i < ARRAY_COUNT(mShakeInfo); ++i, ++it) {
			if (it->isActive()) {
				vAngle += (s16)CSWaveF(it->mAngleX, it->mFrame);
				hAngle += (s16)CSWaveF(it->mAngleY, it->mFrame);
				mRollAccum += (s16)CSWaveF(it->mAngleZ, it->mFrame);
				it->mFrame += 1;

				if (CSDecay(it))
					it->reset();
			}
		}

		CLBPolarToCross(origin, pos, r, vAngle, hAngle);
	}

	JGeometry::TVec3<f32> dir;
	unitVecTo(origin, origPos, &dir);

	CSRotateUp(up, dir, CSRollRad(mRollAccum));
}
