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
		if ((f32)unk2D4->mInHouseMinFrame.get() < (f32)unk2CC)
			unk2CC += 1;
	} else if (unk2C8 != -1) {
		if ((f32)unk2CC < (f32)unk2D4->mInHouseMinFrame.get()) {
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

		JGeometry::TVec3<f32> local_120[18];
		S16Vec SStack_134[9];

		CLBCalcNearNinePos(local_120, SStack_134, unk124, unk148,
		                   getFinalAngleZ(), mNear, mFovy, mAspect);

		f32 fVar1 = unk2C4;
		for (int i = 0; i < 9; ++i) {
			local_120[9 + i].scaleAdd(fVar1, local_120[i], unk25C);
		}

		f32 tmp = unk2C0;
		for (int i = 0; i < 9; ++i) {
			for (int j = 0; j < 2; ++j) {
				f32 fVar2 = 0.0f;
				for (int k = 0; k < 2; ++k) {
					JGeometry::TVec3<f32> local_12C(local_120[j * 9 + i].x,
					                                local_120[j * 9 + i].y
					                                    - fVar2 + -78.0f,
					                                local_120[j * 9 + i].z);
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
	}

	unk2CA = -1;
	calcInHouseNoSub_();
}

bool CPolarSubCamera::isNeedGroundCheck_()
{
	bool result = true;
	if (mMode == CAMERA_MODE_COUNT
	    || (isLButtonCameraSpecifyMode(mMode) && !isNowInbetween() ? true
	                                                               : false)
	    || isRailCameraSpecifyMode(mMode) || mMode == CAMERA_MODE_MULTI_PLAYER
	    || unk278 != 0) {
		result = false;
	} else if (mMode != CAMERA_MODE_SLIDER
	           && (isNormalCameraSpecifyMode(mMode)
	               || isTowerCameraSpecifyMode(mMode))) {
		f32 a     = unk68->mDistMin * JMASSin(unk68->mXAngleMin);
		f32 b     = unk68->mDistMax * JMASSin(unk68->mXAngleMax);
		f32 distY = mPosition.y - mTarget.y;
		if (a > b)
			b = a;
		if (distY > 1.25f * b) {
			result = false;
			if (unk278 < 0x78)
				unk278 = 0x78;
		}
	}
	return result;
}

bool CPolarSubCamera::isNeedRoofCheck_() const
{
	bool result = true;
	// TODO: inline for the ternary thing
	if (mMode == CAMERA_MODE_COUNT
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
	if (mMode == CAMERA_MODE_COUNT || isLButtonCameraSpecifyMode(mMode)
	    || isLButtonCameraInbetween() || isTalkCameraSpecifyMode(mMode)
	    || isTalkCameraInbetween() || isRailCameraSpecifyMode(mMode)
	    || mMode == CAMERA_MODE_MULTI_PLAYER
	    || mMode == CAMERA_MODE_UNDER_GROUND || (unk64 & 4U)) {
		result = false;
	}
	return result;
}

static bool should_clip_fabricated(const TBGCheckData* data)
{
	bool result = false;

	if (data != nullptr && data->isLegal() && data->isCameraWontClip())
		result = true;

	return result;
}

bool CPolarSubCamera::execWallCheck_(Vec* param_1)
{
	bool moved = false;
	f32 radius = unk2D4->mSLWallCheckRadius.get();
	if (radius > 0.0f) {
		TBGWallCheckRecord record(unk80.unk0.x, unkB4.unk0.y + 10.0f,
		                          unk80.unk0.z, radius, 4, 0);

		if (gpMap->isTouchedWallsAndMoveXZ(&record)) {
			int n = record.mResultWallsNum;
			for (int i = 0; i < n; ++i) {
				TBGCheckData* wall = record.mResultWalls[i];
				if (should_clip_fabricated(wall)) {
					JGeometry::TVec3<f32> posArg = unk80.unk0;
					JGeometry::TVec3<f32> posCam = posArg;

					f32 sd = posCam.dot(wall->getNormal())
					         + wall->getPlaneDistance();
					f32 absSd = sd >= 0.0f ? sd : -sd;
					if (absSd < radius) {
						moved      = true;
						f32 pushSd = (radius - sd)
						             * unk2D4->mSLWallRevisionRatio.get();
						posCam.x += pushSd * wall->getNormal().x;
						posCam.z += pushSd * wall->getNormal().z;
						unk80.unk0.x = posCam.x;
						unk80.unk0.z = posCam.z;
						f32 pushArg  = radius - sd;
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
		roofHeight = gpMap->checkRoof(
		    param_1.x, unkB4.unk0.y - unk2D4->mSLRoofChangeY.get(), param_1.z,
		    &roof);
	}

	if (skipCheck || should_clip_fabricated(roof)) {
		if (unk80.unk0.y > roofHeight - unk2D4->mSLRoofHeight.get()) {
			unk80.unk0.y = roofHeight - unk2D4->mSLRoofHeight.get();
			moved        = true;
		}
	}
	return moved;
}

bool CPolarSubCamera::execGroundCheck_(Vec param_1)
{
	bool moved    = false;
	f32 groundChg = unk2D4->mGroundChangeY.get();
	f32 groundOff = CLBLinearInbetween<f32>(
	    unk2D4->mSLGroundHeightNormal.get(),
	    unk2D4->mSLGroundHeightReadyGun.get(), unk2AC->unk8);

	if (mMode == CAMERA_MODE_SLIDER) {
		groundChg = groundChg > 200.0f ? groundChg : 200.0f;
		groundOff = groundOff > 400.0f ? groundOff : 400.0f;
	}

	const TBGCheckData* ground;
	f32 groundY = gpMap->checkGroundIgnoreWaterSurface(
	    param_1.x, unkB4.unk0.y + groundChg, param_1.z, &ground);

	if (should_clip_fabricated(ground)) {
		if (unk80.unk0.y < groundY + groundOff) {
			unk80.unk0.y = groundY + groundOff;
			moved        = true;
		}
	}
	return moved;
}
