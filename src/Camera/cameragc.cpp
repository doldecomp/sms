#include <Camera/Camera.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <System/StageUtil.hpp>
#include <System/MarDirector.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/CameraBck.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/CameraShake.hpp>
#include <Camera/CameraOption.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/CameraInbetween.hpp>
#include <Camera/CameraJetCoaster.hpp>
#include <Camera/CameraMapTool.hpp>
#include <Camera/CameraMarioData.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <macros.h>
#include <stdio.h>

static const char* dummyMactorStringValue1 = "\0\0\0\0\0\0\0\0\0\0\0";
static const char* SMS_NO_MEMORY_MESSAGE   = "メモリが足りません\n";

static const char* MtxCalcTypeName[] = {
	"MActorMtxCalcType_Basic クラシックスケールＯＮ",
	"MActorMtxCalcType_Softimage クラシックスケールＯＦＦ",
	"MActorMtxCalcType_MotionBlend モーションブレンド",
	"MActorMtxCalcType_User ユーザー定義",
};

const char cDirtyFileName[] = "/scene/map/pollution/H_ma_rak.bti";
const char cDirtyTexName[]  = "H_ma_rak_dummy";

CPolarSubCamera* gpCamera;

const char* cStartCamName          = "開始カメラ";
const char* cStartAfterCamName     = "開始後カメラ";
const char* cJetCoasterCam0BckName = "pinna2_camera";
const char* cJetCoasterCam1BckName = "tinkoopa_camera";
const char* cJetCoasterDemoBckName = "tinkoopa_killer_camera";
const char* cStartCamBckFileName   = "/scene/map/camera/StartCamera.bck";

CPolarSubCamera::CPolarSubCamera(const char* name)
    : JDrama::TLookAtCamera(CLBConstUpVec, name)
    , mMode(CAMERA_MODE_INVALID)
    , mPrevMode(CAMERA_MODE_INVALID)
    , mSavedModeBeforeTalk(CAMERA_MODE_INVALID)
    , mInitialMode(CAMERA_MODE_INVALID)
    , unk60(nullptr)
    , unk64(0)
    , unk70(nullptr)
    , unk74(0)
    , mPosFreezeFrames(0)
    , mTargetFreezeFrames(0)
    , unk11C(0)
    , unk120(nullptr)
    , mHeightPanOffset(0.0f)
    , unk250(0.0f)
    , unk254(0)
    , unk268(0.0f)
    , unk26C(1.0f)
    , unk274(0)
    , unk276(0)
    , unk278(0)
    , unk27A(0)
    , mDeadDemoCountdown(0)
    , mDeadDemoCountdownToFovZoom(0)
    , mDeadDemoFovZoomTimer(0)
    , unk282(0)
    , unk284(0)
    , unk288(1.0f)
    , unk28C(0)
    , unk28E(0)
    , unk290(0.0f)
    , unk294(0.0f)
    , unk298(0.0f)
    , unk2AC(nullptr)
    , unk2B0(nullptr)
    , mCameraDemo(nullptr)
    , unk2B8(nullptr)
    , unk2BC(nullptr)
    , unk2C0(3.0f)
    , unk2C4(2.0f)
{
	gpCamera                    = this;
	gpCameraShake               = new TCameraShake;
	TCameraMarioData* marioData = new TCameraMarioData;
	gpCameraOption              = nullptr;
	gpCameraMario               = marioData;
	unk60                       = new CameraUnk60Struct(4);
	mCurrentParams              = new TCameraKindParam;
	mInbetween                  = new TCameraInbetween;
	unk2AC                      = new CameraUnk2ACStruct;
	unk2B0                      = new TCameraBck;
	mCameraDemo                 = new TCameraDemo;
	mSaveNotice                 = new TCamSaveNotice;
	mSaveEx                     = new TCamSaveEx;
	for (int i = 0; i < ARRAY_COUNT(mSaveKindParam); ++i)
		mSaveKindParam[i] = new TCamSaveKindParam(mCamKindNameSaveFile[i]);
	if (SMS_isMultiPlayerMap())
		createMultiPlayer(4);
	int stage = gpMarDirector->getCurrentStage();
	if (gpMarDirector->getCurrentMap() == 58 && (stage == 0 || stage == 1)) {
		unk64 |= CAMERA_FLAG_JET_COASTER_SCENE;
		unk2B8 = new TCameraJetCoaster;
		switch (stage) {
		case 0:
			unk2B0->startDemo(cJetCoasterCam0BckName, nullptr);
			break;
		}
	}

	unk2CA = -1;
	unk2C8 = -1;
	unk2CC = 0;
}

void CPolarSubCamera::startJetCoasterCam1()
{
	unk2B0->startDemo(cJetCoasterCam1BckName, nullptr);
	unk2B0->setFrame(gpMarDirector->unk58 * 0.5f);
}

static s32 JetCoasterDemoCallBack(u32 param_1, u32 param_2)
{
	if (param_2 == 1)
		((CPolarSubCamera*)param_1)->startJetCoasterCam1();

	return true;
}

void CPolarSubCamera::loadAfter()
{
	JDrama::TLookAtCamera::loadAfter();
	mInitialMode = CAMERA_MODE_FOLLOW;
	if (gpMarDirector->getCurrentMap() == 7) {
		mInitialMode = CAMERA_MODE_DELFINO;
	} else if (SMS_isExMap()) {
		mInitialMode = CAMERA_MODE_EX_MAP_0;
	} else if (SMS_isMultiPlayerMap()) {
		mInitialMode = CAMERA_MODE_MULTI_PLAYER;
	} else if (unk64 & CAMERA_FLAG_JET_COASTER_SCENE) {
		mInitialMode = CAMERA_MODE_JET_COASTER;
	}

	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(cStartCamName), cStartCamName);

	if (tool)
		changeCamModeSpecifyCamMapToolAndFrame_(tool, 1);
	else
		changeCamModeSpecifyFrame_(mInitialMode, 1);

	mCurrentParams->copySaveParam(*mSaveKindParam[mMode]);
	mFovy = mCurrentParams->mFovy;
	mNear = mCurrentParams->mNearClip;

	char acStack_54[0x40];
	snprintf(acStack_54, 0x40, "%s%d", cStartAfterCamName,
	         gpMarDirector->unkD0);
	TCameraMapTool* tool2 = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(acStack_54), acStack_54);
	if (tool2) {
		mCurrentTarget.unk28 = MsClamp(tool2->mPosition.y, unk268, unk26C);
		mCurrentTarget.mYaw  = CLBDegToShortAngle(tool2->getYaw()) - 0x8000;
	} else {
		mCurrentTarget.unk28
		    = MsClamp(mSaveEx->mXRotStart.get(), unk268, unk26C);
		mCurrentTarget.mYaw = *gpMarioAngleY - 0x8000;
	}

	if ((unk64 & CAMERA_FLAG_JET_COASTER_SCENE) && unk2B8 != nullptr
	    && (unk2B8->isLButtonMode()))
		setUpToLButtonCamera_(CAMERA_MODE_JET_COASTER);

	unk270 = mCurrentTarget.unk28;
	mCurrentTarget.mPitch
	    = CLBLinearInbetween(mCurrentParams->mXAngleMin,
	                         mCurrentParams->mXAngleMax, mCurrentTarget.unk28);

	JGeometry::TVec3<f32> marPos = SMS_GetMarioPos();
	f32 fVar1;
	if (isNormalDeadDemo()) {
		fVar1 = 35.0f;
	} else {
		fVar1 = mCurrentTarget.unk28 * mCurrentParams->mXRotRatioAtOffsetY
		        + mCurrentParams->mAtOffsetY;
		if (SMS_GetMarioStatus() == MARIO_STATUS_KICK_ROOF_ROLL_UP)
			fVar1 += 260.0f;
		if (mMode == CAMERA_MODE_DEFINITE_D2)
			fVar1 += unk290;
	}

	marPos.y += fVar1;
	gpCameraMario->unk0.set(marPos);

	if (unk70 != nullptr) {
		unk70->calcPosAndAt(&mPosition, &mTarget);
	} else {
		mTarget.set(gpCameraMario->unk0);
		f32 fVar7 = CLBLinearInbetween(mCurrentParams->mDistMin,
		                               mCurrentParams->mDistMax,
		                               mCurrentTarget.unk28);
		CLBPolarToCross(mTarget, &mPosition, fVar7, mCurrentTarget.mPitch,
		                mCurrentTarget.mYaw);
	}

	calcSecureViewTarget_(mCurrentTarget.mYaw, &unk294, &unk298);

	mPosition.x += unk294;
	mPosition.z += unk298;

	mTarget.x += unk294;
	mTarget.z += unk298;

	mCurrentTarget.mPosition.set(mPosition);
	mCurrentTarget.unk18.set(mPosition);
	mCurrentTarget.mTarget.set(mTarget);

	TCameraOption* option = gpCameraOption;
	if (SMS_isOptionMap()) {
		mCurrentTarget.mPosition = mPosition;
		mCurrentTarget.mTarget   = mTarget;
		option = new TCameraOption(mPosition, &mCurrentTarget.mTarget);
	}
	gpCameraOption = option;

	unk256 = mCurrentTarget.mPitch;
	unk258 = mCurrentTarget.mYaw;

	mPreviousTarget        = mCurrentTarget;
	mTargetBeforeFixedMode = mPreviousTarget;

	unk124.set(mPosition);
	unk130.set(mPosition);
	unk13C.set(mPosition);
	unk148.set(mTarget);
	unk154.set(mTarget);
	unk160.set(mTarget);

	if (unk64 & CAMERA_FLAG_JET_COASTER_SCENE) {
		unk2B8->unk10.set(mPosition);
		unk2B8->unk1C.set(mTarget);
	}

	mInbetween->initCameraInbetween(mPosition, mTarget, SMS_GetMarioPos());

	C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);
	C_MTXLookAt(unk1EC, unk124, mUp, unk148);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			unk1AC[i][j] = unk16C[i][j];

	MTXCopy(unk1EC, unk21C);

	fabricatedInline2();

	if ((unk64 & CAMERA_FLAG_JET_COASTER_SCENE) && gpMarDirector->unk7D == 1) {
		gpMarDirector->fireStartDemoCamera(
		    cJetCoasterDemoBckName, nullptr, -1, 0.0f, true,
		    &JetCoasterDemoCallBack, (u32)this, nullptr, JDrama::TFlagT<u16>());
	} else {
		if (!JKRGetResource(cStartCamBckFileName))
			calcInHouseNo_(true);
	}
}

bool CPolarSubCamera::isNowInbetween() const
{
	if (mInbetween->getUnk4() > 0)
		return true;
	else
		return false;
}

MtxPtr CPolarSubCamera::getToroccoMtx_() const
{
	return gpMarioOriginal->mTorocco->unk4->getAnmMtx(2);
}

void CPolarSubCamera::setMarioLookat_() { }

JGeometry::TVec3<f32> CPolarSubCamera::getUsualLookat() const
{
	JGeometry::TVec3<f32> result;
	const JGeometry::TVec3<f32>* thing = mCameraDemo->unk0;
	if (thing)
		result = *thing;
	else
		result = gpCameraMario->unk0;
	return result;
}

s16 CPolarSubCamera::calcAngleXFromXRotRatio_() const
{
	return CLBLinearInbetween(mCurrentParams->mXAngleMin,
	                          mCurrentParams->mXAngleMax, mCurrentTarget.unk28);
}

f32 CPolarSubCamera::calcDistFromXRotRatio_() const
{
	return CLBLinearInbetween(mCurrentParams->mDistMin,
	                          mCurrentParams->mDistMax, mCurrentTarget.unk28);
}

void CPolarSubCamera::calcNowTargetFromPosAndAt_(const Vec& pos, const Vec& at)
{
	f32 radius;
	s16 xAngle;
	s16 yAngle;
	CLBCrossToPolar(at, pos, &radius, &xAngle, &yAngle);
	mCurrentTarget.unk28 = CLBCalcRatio(mCurrentParams->mXAngleMin,
	                                    mCurrentParams->mXAngleMax, xAngle);
	if (isLButtonCameraSpecifyMode(mMode)) {
		mCurrentTarget.unk28 = MsClamp(mCurrentTarget.unk28, 0.0f, 1.0f);
	} else {
		mCurrentTarget.unk28 = MsClamp(mCurrentTarget.unk28, unk268, unk26C);
	}

	mCurrentTarget.mPitch
	    = CLBLinearInbetween(mCurrentParams->mXAngleMin,
	                         mCurrentParams->mXAngleMax, mCurrentTarget.unk28);
	mCurrentTarget.mYaw = yAngle;
	mCurrentTarget.mPosition.set(pos);
	mCurrentTarget.unk18.set(pos);
	mCurrentTarget.mTarget.set(at);
}

void CPolarSubCamera::rotateX_ByStickY_(f32 param_1)
{
	if (SMS_IsMarioOpeningDoor())
		return;

	mCurrentTarget.unk28 -= param_1 * mCurrentParams->mXRotRatioManualSpeed;
	if (isLButtonCameraSpecifyMode(mMode)) {
		mCurrentTarget.unk28 = MsClamp(mCurrentTarget.unk28, 0.0f, 1.0f);
	} else {
		mCurrentTarget.unk28 = MsClamp(mCurrentTarget.unk28, unk268, unk26C);
	}
}

void CPolarSubCamera::rotateY_ByStickX_(f32 param_1)
{
	if (SMS_IsMarioOpeningDoor())
		return;

	s16 thing = param_1
	            * CLBLinearInbetween(mCurrentParams->mYAngleManualSpeedXMin,
	                                 mCurrentParams->mYAngleManualSpeedXMax,
	                                 mCurrentTarget.unk28);
	mCurrentTarget.mYaw += thing;
}

void CPolarSubCamera::offMoveApproach_() { mInbetween->unk44 = 0.0f; }

void CPolarSubCamera::onMoveApproach_()
{
	// Why...
	f32 d = MsSqrtf((mPosition.x - mTarget.x) * (mPosition.x - mTarget.x)
	                + (mPosition.y - mTarget.y) * (mPosition.y - mTarget.y)
	                + (mPosition.z - mTarget.z) * (mPosition.z - mTarget.z));
	mInbetween->unk44
	    = d
	      - CLBLinearInbetween(mCurrentParams->mDistMin,
	                           mCurrentParams->mDistMax, mCurrentTarget.unk28);
}

bool CPolarSubCamera::isMarioReadyGun_() const
{
	// NOTE: it is a complete MYSTERY to me as to why this
	// checkStatusType isn't inlined...
	return gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)
	       && gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK8000);
}

bool CPolarSubCamera::isMarioAimWithGun_() const
{
	return isMarioReadyGun_() && unk120->checkFrameMeaning(0x400);
}

bool CPolarSubCamera::isMarioCrabWalk_() const
{
	return isMarioReadyGun_() && unk120->checkFrameMeaning(0x8000);
}

void CPolarSubCamera::execInvalidAutoChase_()
{
	unk284 = mCurrentParams->mAutoChaseCompleteFrame;
}

bool CPolarSubCamera::isMomentDefinite_() const
{
	bool result = false;
	if (!(unk64 & CAMERA_FLAG_UNK100) && isNormalCameraCompletely()
	    && unk250 > 0.001f
	    && CLBLinearInbetween(mCurrentParams->mCushionMin,
	                          mCurrentParams->mCushionMax, mCurrentTarget.unk28)
	           > 0.001f)
		result = true;
	return result;
}

void CPolarSubCamera::calcSlopeAngleX_(s16* param_1)
{
	s16 result = 0;

	if (!isMarioReadyGun_()) {
		// TODO: MarioAccess inline?
		bool groundOK             = false;
		const TBGCheckData* plane = *gpMarioGroundPlane;
		if (plane != nullptr && plane->isThing())
			groundOK = true;

		if (groundOK && isSlopeCameraMode()) {
			JGeometry::TVec3<f32> diff;
			diff.set(gpMarioPos->x - mPosition.x, 0.0f,
			         gpMarioPos->z - mPosition.z);
			bool b = diff.isZero();
			if (!b) {
				f32 grLevel = SMS_GetMarioGrLevel();
				s16 maxAng  = mSaveEx->mSLSlopeMaxAngleX.get();
				f32 fwdDist = mSaveEx->mSLSlopeForwardDistXZ.get();

				JGeometry::TVec3<f32> norm;
				MsVECNormalize(&diff, &norm);
				norm *= fwdDist;

				JGeometry::TVec3<f32> sample = SMS_GetMarioPos();
				sample += norm;
				JGeometry::TVec3<f32> p2 = sample;
				JGeometry::TVec3<f32> p3 = p2;

				const TBGCheckData* ground;
				f32 height
				    = gpMap->checkGroundIgnoreWaterSurface(
				          p3.x,
				          fwdDist * fakeTan(maxAng) + 10.0f + gpMarioPos->y,
				          p3.z, &ground)
				      - grLevel;
				s16 angle = 0;
				if (height > 0.0f)
					angle = matan(fwdDist, height);
				if (angle > maxAng)
					result = 0;
				else
					result = angle;
			}
		}
	}

	s16 speed = mSaveEx->mSLSlopeSpeedAngleX.get();
	CLBChaseGeneralConstantSpecifySpeed<s16>(&unk28C, result, speed);

	*param_1 -= unk28C;
	*param_1 = MsClamp(*param_1, mSaveEx->mSLLimitMinAngleX.get(),
	                   mSaveEx->mSLLimitMaxAngleX.get());
}

void CPolarSubCamera::calcPosAndAt_()
{
	if (gpCameraMario->mFrameMoveDistHorizontal >= 0.05f)
		unk64 &= ~CAMERA_FLAG_UNK80;

	if (!(unk64 & CAMERA_FLAG_UNK80)) {
		if (unk284 > 0) {
			s32 acf   = mCurrentParams->mAutoChaseCompleteFrame;
			s32 acs   = mCurrentParams->mAutoChaseStartFrame;
			s32 delta = (acf - unk284) + 1;
			if (delta <= acs)
				unk288 = 0.0f;
			else
				unk288 = CLBCalcRatio<s32>(acs, acf, delta);
		} else {
			unk288 = 1.0f;
		}
	}

	s16 yawDelta  = 0;
	f32 distDelta = 0.0f;
	s16 yawSpeed  = CLBLinearInbetween<s16>(
        mCurrentParams->mYAngleManualSpeedXMin,
        mCurrentParams->mYAngleManualSpeedXMax, mCurrentTarget.unk28);
	f32 distSpeed = CLBLinearInbetween<f32>(mCurrentParams->mHoldAddDistXZMin,
	                                        mCurrentParams->mHoldAddDistXZMax,
	                                        mCurrentTarget.unk28);

	if (isNormalCameraSpecifyMode(mMode)) {
		if (isMarioReadyGun_()) {
			f32 mag   = gpCameraMario->unk1C;
			distDelta = mag * distSpeed;
			yawDelta  = (s16)(mag * (f32)yawSpeed);
		}
	}

	CLBChaseAngleDecrease(&unk2AC->unk0, yawDelta,
	                      mSaveEx->mSLAimAngleYChaseMin.get());
	CLBChaseDecrease(&unk2AC->unk4, distDelta, mSaveEx->mSLHoldDistChase.get(),
	                 0.0f);

	if (distSpeed < 0.001f) {
		if (yawSpeed != 0) {
			s16 absSpeed = CLBAbs(yawSpeed);
			s16 absDelta = CLBAbs(yawDelta);
			unk2AC->unkC = (f32)absDelta * (1.0f / (f32)absSpeed);
			unk2AC->unkC = MsClamp(unk2AC->unkC, 0.0f, 1.0f);
		}
	} else {
		unk2AC->unkC = distDelta * (1.0f / distSpeed);
		unk2AC->unkC = MsClamp(unk2AC->unkC, 0.0f, 1.0f);
	}

	CLBChaseDecrease(&unk2AC->unk8, unk2AC->unkC,
	                 mSaveEx->mSLHoldDistChase.get(), 0.0f);

	unk64 &= ~CAMERA_FLAG_UNK100;
	if (mPosFreezeFrames != 0)
		mPosFreezeFrames -= 1;
	if (mTargetFreezeFrames != 0)
		mTargetFreezeFrames -= 1;

	bool freeze = true;
	if (!(gpCameraMario->mFrameMoveDistHorizontal >= 0.05f
	      || gpCameraMario->mFrameMoveDistVertical >= 0.05f))
		freeze = false;

	bool inputActive = true;

	bool checkInput = true;
	if (!isMarioReadyGun_() && !(unk64 & CAMERA_FLAG_UNK4))
		checkInput = false;

	if (!checkInput) {
		bool hasInput = false;
		if (unk120 != nullptr) {
			bool nonZero = true;
			if (unk120->mCompSPos[6] == 0.0f && unk120->mCompSPos[7] == 0.0f)
				nonZero = false;
			if (nonZero)
				hasInput = true;
		}
		if (!hasInput)
			inputActive = false;
	}

	if (unk64 & CAMERA_FLAG_UNK40) {
		if (mPosFreezeFrames == 0)
			mPosFreezeFrames = 1;
		if (mTargetFreezeFrames == 0)
			mTargetFreezeFrames = 1;
	} else {
		if (freeze || inputActive) {
			mTargetFreezeFrames = 0;
			mPosFreezeFrames    = 0;
		}
		if ((unk64 & CAMERA_FLAG_DEAD_DEMO) && mPosFreezeFrames == 0)
			mPosFreezeFrames = 1;
	}

	JGeometry::TVec3<f32> finalAt;
	if (mPosFreezeFrames == 0 || mTargetFreezeFrames == 0) {
		if (mPosFreezeFrames == 0 && mTargetFreezeFrames == 0)
			unk64 &= ~CAMERA_FLAG_UNK40;

		if (!mInbetween->isThing() || freeze || inputActive) {
			if (mTargetFreezeFrames == 0
			    && isDefiniteCameraSpecifyMode(mMode)) {
				const JGeometry::TVec3<f32>* ovr = mCameraDemo->unk0;
				JGeometry::TVec3<f32> origin;
				if (ovr != nullptr)
					origin = *ovr;
				else
					origin = gpCameraMario->unk0;
				JGeometry::TVec3<f32> copy = origin;
				mCurrentTarget.mTarget.set(copy);
			}

			if (isFixOrDefiniteCameraSpecifyMode(mMode)) {
				finalAt = mCurrentTarget.mTarget;
				unk28C  = 0;
			} else {
				mCurrentTarget.mPitch = CLBLinearInbetween<s16>(
				    mCurrentParams->mXAngleMin, mCurrentParams->mXAngleMax,
				    mCurrentTarget.unk28);
				s16 xAngle = mCurrentTarget.mPitch
				             + mCurrentParams->mOffsetAngleX + unk2AC->unk0;
				s16 yAngle
				    = mCurrentTarget.mYaw + mCurrentParams->mOffsetAngleY;

				if (gpCameraMario->mFrameMoveDistHorizontal >= 0.05f) {
					s16 mAngle = *gpMarioAngleY - 0x8000;
					f32 m      = MsClamp<f32>(
                        (f32)mCurrentParams->mMaxAddAngleY
                            * (0.5f * (1.0f - JMASCos((mAngle - unk258) * 2))),
                        -32766.998f, 32766.998f);

					if ((s16)(mAngle - yAngle) < 0)
						m = -m;

					s16 addAngleYSpeed = mSaveEx->mSLAddAngleYSpeed.get();
					CLBChaseGeneralConstantSpecifySpeed<s16>(
					    &unk28E, CLBRoundf<s16>(m), addAngleYSpeed);
				}
				yAngle += unk28E;

				calcSlopeAngleX_(&xAngle);

				if (mMode == CAMERA_MODE_TOWER_E) {
					if (gpCameraMario->isMarioRocketing()) {
						xAngle = mSaveEx->mSLLimitMaxAngleX.get() - 0x1770;
					} else if (SMS_GetMarioStatus() == MARIO_STATUS_HIP_DROP) {
						xAngle = 0x3E8;
					}
				}

				finalAt.x = -(mCurrentParams->mOffsetLookatXZ
				                  * JMASSin(yAngle - 0x4000)
				              - mCurrentTarget.mTarget.x);
				finalAt.y = mCurrentTarget.mTarget.y;
				finalAt.z = -(mCurrentParams->mOffsetLookatXZ
				                  * JMASCos(yAngle - 0x4000)
				              - mCurrentTarget.mTarget.z);

				if (mMode != CAMERA_MODE_MULTI_PLAYER) {
					execSecureView_(yAngle, finalAt);

					if (!isRailCameraSpecifyMode(mMode)) {
						JGeometry::TVec3<f32> saveAt;
						saveAt.set(finalAt);
						f32 dist = CLBLinearInbetween<f32>(
						    mCurrentParams->mDistMin, mCurrentParams->mDistMax,
						    mCurrentTarget.unk28);
						s16 cushAng = CLBLinearInbetween<s16>(
						    mCurrentParams->mXAngleMin,
						    mCurrentParams->mXAngleMax, mCurrentTarget.unk28);
						f32 cushion = JMASCos(cushAng);

						JGeometry::TVec3<f32> newPos;
						CLBPolarToCross(saveAt, newPos,
						                dist + mInbetween->unk44, xAngle,
						                yAngle);

						if (isNormalCameraCompletely()) {
							JGeometry::TVec3<f32> base
							    = mPreviousTarget.mPosition;
							base.y = mPreviousTarget.unk18.y;
							f32 nDist;
							s16 nVA, nHA;
							CLBCrossToPolar(saveAt, base, &nDist, &nVA, &nHA);
							f32 cushDist = cushion * nDist;
							if (cushDist > cushion * dist) {
								unk64 |= CAMERA_FLAG_UNK100;
								mCurrentTarget.mPosition = newPos;
							} else {
								f32 minDist
								    = cushion
								      * (dist
								         - CLBLinearInbetween<f32>(
								             mCurrentParams->mCushionMin,
								             mCurrentParams->mCushionMax,
								             mCurrentTarget.unk28));
								if (minDist < mSaveEx->mSLMinCushionXZ.get())
									minDist = mSaveEx->mSLMinCushionXZ.get();

								if (fabricatedInline3() && cushDist < minDist) {
									f32 diff = minDist - cushDist;
									base.x += diff * JMASSin(nHA);
									base.z += diff * JMASCos(nHA);
									CLBCrossToPolar(saveAt, base, &nDist, &nVA,
									                &nHA);
								}

								bool useMid = false;
								if (!(unk64 & CAMERA_FLAG_UNK100)
								    && isNormalCameraCompletely()
								    && unk250 > 0.001f) {
									if (CLBLinearInbetween<f32>(
									        mCurrentParams->mCushionMin,
									        mCurrentParams->mCushionMax,
									        mCurrentTarget.unk28)
									    > 0.001f)
										useMid = true;
								}
								if (useMid) {
									mCurrentTarget.mPosition.set(base);
								} else {
									CLBPolarToCross(saveAt,
									                mCurrentTarget.mPosition,
									                nDist, nVA, yAngle);
								}
								mCurrentTarget.mPosition.y = newPos.y;
							}
						} else {
							mCurrentTarget.mPosition = newPos;
						}

						f32 sY = JMASSin(yAngle);
						f32 cY = JMASCos(yAngle);

						if (fabricatedInline3()) {
							f32 dx = gpMarioPos->x - mCurrentTarget.mPosition.x;
							f32 dz = gpMarioPos->z - mCurrentTarget.mPosition.z;
							f32 d  = MsSqrtf(dx * dx + dz * dz);
							f32 minD = mSaveEx->mSLMinCushionXZ.get();
							f32 mD2
							    = minD < dist * cushion ? dist * cushion : minD;
							if (d < mD2) {
								f32 add = mD2 - d;
								mCurrentTarget.mPosition.x += sY * add;
								mCurrentTarget.mPosition.z += cY * add;
							}
						}

						switch (mMode) {
						case CAMERA_MODE_UNDER_GROUND:
						case CAMERA_MODE_MARE_UNDER_GROUND:
							break;
						default:
							mCurrentTarget.mPosition.x = -(
							    unk2AC->unk4 * sY - mCurrentTarget.mPosition.x);
							mCurrentTarget.mPosition.z = -(
							    unk2AC->unk4 * cY - mCurrentTarget.mPosition.z);
							break;
						}

						if (mInbetween->isThing() && inputActive) {
							JGeometry::TVec3<f32> warpPos;
							f32 d = CLBLinearInbetween<f32>(
							    mCurrentParams->mDistMin,
							    mCurrentParams->mDistMax, mCurrentTarget.unk28);
							CLBPolarToCross(mInbetween->mAt, warpPos,
							                d + mInbetween->unk44, xAngle,
							                yAngle);
							mInbetween->warpPosAndAt(warpPos, mInbetween->mAt);
						}
					}
				}
				mPosition.x = mCurrentTarget.mPosition.x;
				mPosition.z = mCurrentTarget.mPosition.z;
				execHeightPan_();
				finalAt.y = mCurrentTarget.unk18.y;
				Vec posCpy;
				posCpy = mPosition;
				if (isNeedWallCheck_() && execWallCheck_(&posCpy)) {
					CLBCrossToPolar(mCurrentTarget.mTarget, mPosition,
					                &mCurrentTarget.mPitch,
					                &mCurrentTarget.mYaw);
					mCurrentTarget.unk28 = mCurrentTarget.unk30;
				}
				if (isNeedRoofCheck_()) {
					Vec roofCheck = posCpy;
					execRoofCheck_(roofCheck);
				}
				if (isNeedGroundCheck_()) {
					Vec groundCheck = posCpy;
					execGroundCheck_(groundCheck);
				}
			}
			JGeometry::TVec3<f32> copy;
			copy.set(finalAt);
			mInbetween->execCameraInbetween(mCurrentTarget.mPosition, copy,
			                                SMS_GetMarioPos());
		}
	}

	if (mPosFreezeFrames == 0) {
		f32 chaseXZ;
		f32 chaseY;
		if (isRailCameraSpecifyMode(mPrevMode) && isRailCameraSpecifyMode(mMode)
		    && isNowInbetween()) {
			chaseXZ = 1.0f;
			chaseY  = 1.0f;
		} else {
			chaseXZ = mCurrentParams->mPosChaseRateXZ;
			chaseY  = mCurrentParams->mPosChaseRateY;
			if (mMode == CAMERA_MODE_TOWER_E
			    && SMS_GetMarioStatus() == MARIO_STATUS_HIP_DROP) {
				u32 v = gpCameraMario->mFramesSinceMarioStatusChange;
				if (v < 120) {
					chaseY = CLBLinearInbetween<f32>(
					    mCurrentParams->mPosChaseRateY, 0.0f,
					    (f32)(120 - v) / 120.0f);
				}
			}
			if (unk64 & CAMERA_FLAG_UNK4) {
				chaseXZ = 1.0f;
			} else if (unk120->mCompSPos[6] != 0.0f
			           || unk120->mCompSPos[7] != 0.0f) {
				f32 v = gpCameraMario->mFrameMoveDistHorizontal;
				if (v > 20.0f)
					v = 20.0f;
				chaseXZ = CLBEaseInInbetween<f32>(
				    mCurrentParams->mPosChaseRateXZ,
				    mCurrentParams->mPosChaseRateXZ_C,
				    CLBCalcRatio<f32>(20.0f, 0.0f, v));
			}
			if (unk120->mCompSPos[6] != 0.0f) {
				f32 v = gpCameraMario->mFrameMoveDistVertical;
				if (v > 20.0f)
					v = 20.0f;
				chaseY = CLBEaseInInbetween<f32>(
				    mCurrentParams->mPosChaseRateY,
				    mCurrentParams->mPosChaseRateY_C,
				    CLBCalcRatio<f32>(20.0f, 0.0f, v));
			}
		}
		const JGeometry::TVec3<f32>& v = mInbetween->mPos;
		CLBChaseDecrease(&mPosition.x, v.x, chaseXZ, 0.0f);
		CLBChaseDecrease(&mPosition.y, v.y, chaseY, 0.0f);
		CLBChaseDecrease(&mPosition.z, v.z, chaseXZ, 0.0f);
	}

	if (mTargetFreezeFrames == 0) {
		f32 atXZ = mCurrentParams->mAtChaseRateXZ;
		f32 atY  = mCurrentParams->mAtChaseRateY;

		const JGeometry::TVec3<f32>& v = mInbetween->mAt;
		CLBChaseDecrease(&mTarget.x, v.x, atXZ, 0.0f);
		CLBChaseDecrease(&mTarget.y, v.y, atY, 0.0f);
		CLBChaseDecrease(&mTarget.z, v.z, atXZ, 0.0f);
	}
}

void CPolarSubCamera::calcFinalPosAndAt_()
{
	if (mMode != CAMERA_MODE_REPRODUCE_DEMO) {
		gpCameraShake->execShake(unk124, &unk148, &mUp);
		if (mMode != CAMERA_MODE_JET_COASTER) {
			if (!isFixCameraSpecifyMode(mMode) || isNowInbetween()) {
				if (isHellDeadDemo()) {
					if (unk256 > mSaveEx->mSLLimitMaxAngleX.get()) {
						mTargetFreezeFrames = 1;
						unk64 |= CAMERA_FLAG_UNK40;
					}
				} else {
					CLBRevisionLookatByAngleX(mSaveEx->mSLLimitMinAngleX.get(),
					                          mSaveEx->mSLLimitMaxAngleX.get(),
					                          unk124, &unk148);
				}
			}
		}
	}

	if (unk124 == unk148) {
		unk124.set(unk130);
		unk148.set(unk154);
	} else {
		unk130.set(unk124);
		unk154.set(unk148);
	}
}

void CPolarSubCamera::calcExternalData_() { }

// TODO: this should be weak/inline
void CPolarSubCamera::ctrlGameCamera_()
{
	if (!(unk64 & CAMERA_FLAG_DEAD_DEMO))
		execDeadDemoProc_();
	if (!(unk64 & CAMERA_FLAG_UNK80)) {
		if (unk284 > 0)
			unk284 -= 1;
	}
	if (unk278 != 0)
		unk278 -= 1;
	if (unk27A != 0)
		unk27A -= 1;
	if (unk282 != 0)
		unk282 -= 1;

	JGeometry::TVec3<f32> marPos = *gpMarioPos;
	f32 yOffset;
	if (isNormalDeadDemo()) {
		yOffset = 35.0f;
	} else {
		yOffset = mCurrentTarget.unk28 * mCurrentParams->mXRotRatioAtOffsetY
		          + mCurrentParams->mAtOffsetY;
		if (SMS_GetMarioStatus() == MARIO_STATUS_KICK_ROOF_ROLL_UP)
			yOffset += 260.0f;
		if (mMode == CAMERA_MODE_DEFINITE_D2)
			yOffset += unk290;
	}
	marPos.y += yOffset;
	gpCameraMario->unk0.set(marPos);
	gpCameraMario->calcAndSetMarioData();

	mPreviousTarget = mCurrentTarget;

	if (gpMarDirector->mState == 4 && !(unk64 & CAMERA_FLAG_DEAD_DEMO)) {
		if (isTalkCameraSpecifyMode(mMode)) {
			if (!gpMarDirector->isTalkModeNow())
				changeCamMode_(mInitialMode);
		} else if (!isSimpleDemoCamera()) {
			int code;
			if (controlByCameraCode_(&code))
				execCameraModeChangeProc_(code);
		}
	}

	TCameraKindParam param;
	param.copySaveParam(*mSaveKindParam[mMode]);

	if (isNowInbetween())
		mCurrentParams->inbetweenData(param, (f32)mInbetween->getUnk4());
	else
		*mCurrentParams = param;

	if (unk284 > mCurrentParams->mAutoChaseCompleteFrame)
		unk284 = mCurrentParams->mAutoChaseCompleteFrame;

	if (!isNormalDeadDemo()
	    && !(unk64 & (CAMERA_FLAG_JET_COASTER_SCENE | CAMERA_FLAG_GATE_DEMO)))
		mFovy = mCurrentParams->mFovy;
	mNear = mCurrentParams->mNearClip;
	mFar  = 300000.0f;

	if (isNormalDeadDemo()) {
		ctrlNormalDeadDemo_();
	} else {
		switch (mMode) {
		case CAMERA_MODE_MULTI_PLAYER:
			ctrlMultiPlayerCamera_();
			break;
		case CAMERA_MODE_JET_COASTER:
			ctrlJetCoasterCamera_();
			break;
		default:
			if (isFixOrDefiniteCameraSpecifyMode(mMode))
				calcPosAndAt_();
			else if (isLButtonCameraSpecifyMode(mMode))
				ctrlLButtonCamera_();
			else if (isTalkCameraSpecifyMode(mMode))
				ctrlTalkCamera_();
			else
				ctrlNormalOrTowerCamera_();
			break;
		}
	}

	unk124.set(mPosition);
	unk148.set(mTarget);
}

void CPolarSubCamera::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		if (param_2->unk0 & 1) {
			unk13C.set(unk124);
			unk160.set(unk148);
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					unk1AC[i][j] = unk16C[i][j];
			MTXCopy(unk1EC, unk21C);
		}
		mUp.set(CLBConstUpVec);
		unk254 = 0;
		if (mMode != CAMERA_MODE_REPRODUCE_DEMO) {
			if (SMS_isOptionMap())
				ctrlOptionCamera_();
			else
				ctrlGameCamera_();
			calcFinalPosAndAt_();

			fabricatedInline2();
		}

		if (mMode != CAMERA_MODE_REPRODUCE_DEMO) {
			C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);
			C_MTXLookAt(unk1EC, unk124, mUp, unk148);
		}

		bool flag2 = (param_2->unk0 & 2) ? true : false;
		if (flag2) {
			if (mMode != CAMERA_MODE_REPRODUCE_DEMO) {
				if (!(unk64 & CAMERA_FLAG_DEAD_DEMO)) {
					if (unk64 & CAMERA_FLAG_GATE_DEMO) {
						updateGateDemoCamera_();
					} else if (unk64 & CAMERA_FLAG_JET_COASTER_SCENE) {
						unk2B0->updateDemo(&unk2B8->unk10, &unk2B8->unk1C,
						                   &unk2B8->unk28, &unk2B8->unk34);
					}
				}
			}
			if (!SMS_isOptionMap()) {
				if (mMode != CAMERA_MODE_JET_COASTER
				    && mMode != CAMERA_MODE_REPRODUCE_DEMO)
					calcInHouseNo_(false);
			}
		}
		updateDemoCamera_(flag2);
	}

	if (param_1 & 0x14) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				param_2->mProjMtx.mMtx[i][j] = unk16C[i][j];
		param_2->setViewMtx(unk1EC);
		param_2->mNearPlane = mNear;
		param_2->mFarPlane  = mFar;
		if (param_1 & 0x10)
			GXSetProjection(param_2->mProjMtx.mMtx, GX_PERSPECTIVE);
	}
}

s16 CPolarSubCamera::getOffsetAngleX() const
{
	return mCurrentParams->mOffsetAngleX;
}

s16 CPolarSubCamera::getOffsetAngleY() const
{
	return mCurrentParams->mOffsetAngleY;
}

s16 CPolarSubCamera::getFinalAngleZ() const
{
	return unk254 + gpCameraShake->mRollAccum;
}
