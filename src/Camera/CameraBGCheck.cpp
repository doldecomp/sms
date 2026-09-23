#include <Camera/Camera.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Map/MapCollisionData.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/camerasave.hpp>
#include <MarioUtil/MapUtil.hpp>
#include <JSystem/JMath.hpp>

void CPolarSubCamera::calcInHouseNoSub_()
{
	if (unk2CA != -1) {
		unk2C8 = unk2CA;
		if ((f32)unk2CC < (f32)mSaveEx->mInHouseMinFrame.get())
			unk2CC += 1;
	} else if (unk2C8 != -1) {
		if ((f32)unk2CC < (f32)mSaveEx->mInHouseMinFrame.get()) {
			unk2CC += 1;
		} else {
			unk2C8 = -1;
			unk2CC = 0;
		}
	}
}

void CPolarSubCamera::calcInHouseNo_(bool param_1)
{
	bool b = true;
	if (!param_1 && unk13C == unk124 && unk160 == unk148)
		b = false;

	if (b) {
		if (isThing3()) {
			unk2CA = -1;
			calcInHouseNoSub_();
			return;
		}

		JGeometry::TVec3<f32> local_120[2][9];
		S16Vec euler;

		CLBCalcNearNinePos(local_120[0], &euler, unk124, unk148,
		                   getFinalAngleZ(), mNear, mFovy, mAspect);

		// TODO: 97.3%, frame 0x1c8 vs 0x1c0. Walking a far pointer gives
		// retail's ctr-3 unroll (a plain [1][i] store unrolls fully), but
		// retail derives far from near (+0x6c) off one i*12 offset and keeps
		// a dead i increment; the euler slot also sits below local_12C in
		// retail. Inert: top-scope local_12C/local_138 declarations, a near
		// pointer walk, p[9], u8/s16/u32 counters, a do-while, a TVec3 temp.
		f32 fVar1 = unk2C4;
		JGeometry::TVec3<f32>* p = local_120[1];
		for (int i = 0; i < 9; ++i, ++p) {
			p->scaleAdd(fVar1, unk25C, local_120[0][i]);
		}

		f32 fVar2;
		f32 tmp = unk2C0;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 2; ++j) {
				fVar2 = 0.0f;
				for (int k = 0; k < 2; ++k) {
					JGeometry::TVec3<f32> local_12C(local_120[j][i].x,
					                                local_120[j][i].y
					                                    - fVar2 + -78.0f,
					                                local_120[j][i].z);
					const TBGCheckData* local_138;
					gpMap->checkGroundIgnoreWaterSurface(local_12C, &local_138);
					if (local_138 && local_138->isOob()) {
						unk2CA = local_138->getData();
						calcInHouseNoSub_();
						return;
					}

					fVar2 += tmp;
				}
			}
		}

		unk2CA = -1;
	}

	calcInHouseNoSub_();
}

bool CPolarSubCamera::isNeedGroundCheck_()
{
	bool result = true;
	if (mMode == CAMERA_MODE_REPRODUCE_DEMO
	    || (isLButtonCameraSpecifyMode(mMode) && !isNowInbetween() ? true
	                                                               : false)
	    || isRailCameraSpecifyMode(mMode) || mMode == CAMERA_MODE_MULTI_PLAYER
	    || unk278 != 0) {
		result = false;
	} else if (mMode != CAMERA_MODE_SLIDER
	           && (isNormalCameraSpecifyMode(mMode)
	               || isTowerCameraSpecifyMode(mMode))) {
		// TODO: 99.8%, three volatile-FPR names: retail loads mDistMin into f2
		// and the sine into f3 (product into f3); ours the other way round.
		// This split `*=`/`-=` spelling fixed the rest (99.3 -> 99.8). Tried:
		// sine first with `a = d * a` / `a *= d`, `a = a * s`, by-value
		// product/sine/mDistMin levels, distY or a declared first, an
		// NgMax-style helper, `(a > b ? a : b)`, JMASin over the s16, a named
		// sine, b declared first or split, a params local.
		f32 a = mCurrentParams->mDistMin;
		a *= JMASSin(mCurrentParams->mXAngleMin);
		f32 b = mCurrentParams->mDistMax * JMASSin(mCurrentParams->mXAngleMax);
		f32 distY = mPosition.y;
		distY -= mTarget.y;
		if (a > b)
			b = a;
		if (distY > 1.25f * b) {
			result = false;
			if (unk278 < 120)
				unk278 = 120;
		}
	}
	return result;
}

bool CPolarSubCamera::isNeedRoofCheck_() const
{
	bool result = true;
	// TODO: inline for the ternary thing
	if (mMode == CAMERA_MODE_REPRODUCE_DEMO
	    || (isLButtonCameraSpecifyMode(mMode) && !isNowInbetween() ? true
	                                                               : false)
	    || isRailCameraSpecifyMode(mMode) || mMode == CAMERA_MODE_MULTI_PLAYER
	    || unk27A != 0)
		result = false;

	return result;
}

bool CPolarSubCamera::isNeedWallCheck_() const
{
	bool result = true;
	if (mMode == CAMERA_MODE_REPRODUCE_DEMO || isLButtonCameraSpecifyMode(mMode)
	    || isLButtonCameraInbetween() || isTalkCameraSpecifyMode(mMode)
	    || isTalkCameraInbetween() || isRailCameraSpecifyMode(mMode)
	    || mMode == CAMERA_MODE_MULTI_PLAYER
	    || mMode == CAMERA_MODE_UNDER_GROUND || (unk64 & CAMERA_FLAG_UNK4)) {
		result = false;
	}
	return result;
}

static inline bool should_clip_fabricated(const TBGCheckData* data)
{
	bool result = false;

	if (data != nullptr && data->isLegal() && data->isCameraWontClip())
		result = true;

	return result;
}

// The dot reads the raw normal, every push through getNormal(), and the plane
// distance is named first: that lands the frame and every slot.
// TODO: 99.9%. posCam copy-constructed from the member schedules the copy
// like retail (x, y loaded before the first store), but retail reads the
// three words from posArg's slot (0x60) rather than from mCurrentTarget.
// Tried: posCam from posArg (copy-init, assigned, set(), xyz ctor, Vec cast),
// posArg as a Vec.
bool CPolarSubCamera::execWallCheck_(Vec* param_1)
{
	bool moved = false;
	f32 radius = mSaveEx->mSLWallCheckRadius.get();
	if (radius > 0.0f) {
		TBGWallCheckRecord record(mCurrentTarget.mPosition.x,
		                          mPreviousTarget.mPosition.y + 10.0f,
		                          mCurrentTarget.mPosition.z, radius, 4, 0);

		if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
			int n = record.mResultWallsNum;
			for (int i = 0; i < n; ++i) {
				TBGCheckData* wall = record.mResultWalls[i];
				if (should_clip_fabricated(wall)) {
					JGeometry::TVec3<f32> posArg;
					posArg = mCurrentTarget.mPosition;
					JGeometry::TVec3<f32> posCam = mCurrentTarget.mPosition;
					f32 planeDist = wall->getPlaneDistance();
					f32 sd        = posCam.dot(wall->mNormal) + planeDist;
					f32 absSd = sd >= 0.0f ? sd : -sd;
					if (absSd < radius) {
						moved      = true;
						f32 pushSd = (radius - sd)
						             * mSaveEx->mSLWallRevisionRatio.get();
						posCam.x += pushSd * wall->getNormal().x;
						posCam.z += pushSd * wall->getNormal().z;
						mCurrentTarget.mPosition.x = posCam.x;
						mCurrentTarget.mPosition.z = posCam.z;
						f32 pushArg                = radius - sd;
						posArg.x += pushArg * wall->getNormal().x;
						posArg.z += pushArg * wall->getNormal().z;
						param_1->x = posArg.x;
						param_1->z = posArg.z;
					}
				}
			}
		}
	}
	return moved;
}

// By-value levels over the roof-height parameter and the target height (+4
// of pool each) land execRoofCheck_'s frame and `roof` slot.
static inline f32 CamRoofHeight(const CPolarSubCamera* p)
{
	return p->mSaveEx->mSLRoofHeight.get();
}

static inline f32 CamTargetY(const CPolarSubCamera* p)
{
	return p->mCurrentTarget.mPosition.y;
}

bool CPolarSubCamera::execRoofCheck_(Vec param_1)
{
	bool moved               = false;
	bool skipCheck           = false;
	const TBGCheckData* roof = nullptr;
	f32 roofHeight           = 0.0f;
	if (SMS_GetMonteVillageAreaInMario() == 0
	    && gpCamera->mMode != CAMERA_MODE_MONTE_HANG) {
		roofHeight = -512.5f;
		skipCheck  = true;
	} else {
		roofHeight = gpMap->checkRoof(param_1.x,
		                              mPreviousTarget.mPosition.y
		                                  - mSaveEx->mSLRoofChangeY.get(),
		                              param_1.z, &roof);
	}

	if (skipCheck || should_clip_fabricated(roof)) {
		f32 limit = roofHeight - CamRoofHeight(this);
		if (CamTargetY(this) > limit) {
			mCurrentTarget.mPosition.y = limit;
			moved = true;
		}
	}
	return moved;
}

// TODO: instructions and frame exact; only `ground` is 4 high (ours 0x2c,
// retail 0x28: retail has 4 more bytes above it in the named block). Tried:
// named TCamSaveEx*/CameraUnk2ACStruct*/TMap* locals and forks (inert or
// -0x10), by-value levels over the lerp ratio, groundChg, the lerp and
// mPreviousTarget.y (+4/+8, ground moves up), `ground` declared first,
// `groundY += groundOff` and a named sum.
bool CPolarSubCamera::execGroundCheck_(Vec param_1)
{
	bool moved    = false;
	f32 groundChg = mSaveEx->mGroundChangeY.get();
	f32 groundOff = CLBLinearInbetween<f32>(
	    mSaveEx->mSLGroundHeightNormal.get(),
	    mSaveEx->mSLGroundHeightReadyGun.get(), unk2AC->unk8);

	if (mMode == CAMERA_MODE_SLIDER) {
		groundChg = groundChg > 200.0f ? groundChg : 200.0f;
		groundOff = groundOff > 400.0f ? groundOff : 400.0f;
	}

	const TBGCheckData* ground;
	f32 groundY = gpMap->checkGroundIgnoreWaterSurface(
	    param_1.x, mPreviousTarget.mPosition.y + groundChg, param_1.z, &ground);

	if (should_clip_fabricated(ground)) {
		if (mCurrentTarget.mPosition.y < groundY + groundOff) {
			mCurrentTarget.mPosition.y = groundY + groundOff;
			moved                      = true;
		}
	}
	return moved;
}
