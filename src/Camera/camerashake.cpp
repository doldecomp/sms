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

// TODO: startShake and keepShake are 16 bytes *too big* (0x60 vs 0x50) with one
// FPR difference inside this inlined body: retail does `lfs f3, 0x2c(r31)` then
// `fmuls f3, f3, f31`, i.e. it multiplies the amp into amp's own register, while
// we load amp into f1 and write f3. The magic double-conversion pair is at the
// same distance from the frame top in both builds, so the 16 extra bytes are
// low region -- our three expansions of this helper reserve ~17 bytes each
// against retail's ~12. Rejected (all measured): `f32 a = amp; a *= scale;` and
// its `a = a * scale` variant (both push this helper out of line, 84
// instructions, 29%), `scale * amp` (99.3), named amp/vel locals in
// setShakeAngleAll_ (+0x18), fetching mShakeData[mode] twice instead of naming
// `save` (+8), moving the `duration` fetch inside the guard (77 instructions),
// reordering the three stores, and dropping `#pragma strength off` (inert).
// The signature is pinned by the map (all five parameters by value).
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

// TODO: 95.8%. The up-vector rotation and the whole shake loop are exact; what
// is left is a 56-byte low-region frame gap (retail 0x118, ours 0xe0, with
// `origPos` 4 bytes higher relative to the saves) plus ten structural
// instructions at the entry and in the `finished` block. Retail materialises one
// zero in r8 *before* the prologue and reuses it for `anyActive`, the loop index
// and `mRollAccum = 0`, and reuses the `1` it stores into `mIsDecreasing` for
// `finished = true`; we emit three separate `li 0`s and a separate `li 1`.
// Rejected: `bool finished` hoisted above the angle accumulation or to the top
// of the active block (both inert); a C-style `int i;` declaration block with
// `it` fetched before the origPos copy (moves the `it` fetch to retail's
// position but adds an extra `li 0` and a load reorder, 95.6).
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
	f32 upY                     = oldUp.y;
	f32 upZ                     = oldUp.z;
	up->x = oldUp.x * rot.at(0, 0) + upY * rot.at(1, 0) + upZ * rot.at(2, 0);
	up->y = oldUp.x * rot.at(0, 1) + upY * rot.at(1, 1) + upZ * rot.at(2, 1);
	up->z = oldUp.x * rot.at(0, 2) + upY * rot.at(1, 2) + upZ * rot.at(2, 2);
}
