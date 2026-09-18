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

// TODO: 95.9%. Every instruction in the body is exact; what is left is a
// 56-byte low-region frame gap (retail 0x118, ours 0xe0) and ten structural
// instructions, both fully diagnosed by re-pass 175's probes.
//
// Retail's slot map, read off the dtk asm (0xc0/0xc4 and everything below 0x90
// are never referenced): dead 0xc..0x90, rot 0x90..0xb4, oldUp 0xb4..0xc0,
// *8 dead bytes* 0xc0..0xc8, dir 0xc8, hAngle/vAngle/r 0xd4/0xd6/0xd8,
// origPos 0xdc, three 8-byte conversion temps 0xe8/0xf0/0xf8, saves 0x100.
// Ours is the same list with rot *below* oldUp, no 8-byte gap and only 80 dead
// bytes instead of 132. Two spellings reproduce the map byte-for-byte:
//   (1) `volatile char hole[8]` between dir and oldUp, `oldUp` declared
//       uninitialised *before* rot and assigned `= *up` after it, and
//       `volatile char trash[52]` declared last. Frame 0x118 exact, 345
//       instructions, every stack displacement exact, 96.1%, leaving only the
//       ten register markers below.
//   (2) moving rot/oldUp into a TU-local `static inline` helper, which makes
//       them inline temporaries growing *up* from the pool (rot then oldUp, so
//       the copy lands after setRotate for free) and makes the 8-byte gap the
//       region boundary. That alone is frame 0x110 - 48 of the 56 bytes - but
//       leaves rot 60 bytes too low, because the helper's own expansion adds 52
//       bytes of pool *above* rot where retail has 8.
// So the residue is one dead 8-byte named local between dir and oldUp plus 52
// dead pool bytes below rot, and no legal carrier is available: the TU's only
// UNUSED callees (setShakeAngleAll_/One_) are not called from here, and the
// inlined callees in scope (TVec3::set/setLength, TCamShakeInfo::isActive and
// ::reset, JMASSin, TMatrix33::identity, SMatrix33C::at which returns by value)
// all have empty local frames. Do not commit the padding.
//
// The ten structural markers are one phenomenon: retail CSEs the *constants*
// that a bool flag shares with a neighbouring field store, and we do not.
// In the entry block it materialises a single 0 in r8 before the prologue and
// reuses it for `anyActive`, for `i` (`addi r4, r8, 0`) and for `mRollAccum = 0`
// (`sth r8`), giving it=r3/i=r4; we emit three separate `li 0`s and get
// it=r5/i=r6. In the shake loop `finished` lives in r0, sharing its 0 with
// `it->mIsKeep = 0` and taking its 1 from the `li r6, 1` stored to
// `mIsDecreasing` (`mr r0, r6`); ours keeps `finished` in r5 with its own
// constants. `mRollAccum = 0` cannot move above the origPos copy (the store
// would have to cross the loads from *pos, and retail's loads come first).
// Rejected: `bool finished` hoisted above the angle accumulation or to the top
// of the active block (both inert); a C-style `int i;` declaration block with
// `it` fetched before the origPos copy (95.6 at the old frame).
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
	unitVecTo(origin, origPos, &dir);

	JGeometry::TRotation3<TMtx33f> rot(
	    dir, -(0.017453294f * (0.005493164f * (f32)mRollAccum)));

	JGeometry::TVec3<f32> oldUp = *up;
	f32 upY                     = oldUp.y;
	f32 upZ                     = oldUp.z;
	up->x = oldUp.x * rot.at(0, 0) + upY * rot.at(1, 0) + upZ * rot.at(2, 0);
	up->y = oldUp.x * rot.at(0, 1) + upY * rot.at(1, 1) + upZ * rot.at(2, 1);
	up->z = oldUp.x * rot.at(0, 2) + upY * rot.at(1, 2) + upZ * rot.at(2, 2);
}
