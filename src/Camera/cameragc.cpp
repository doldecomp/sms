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
    , mMode(-1)
    , unk54(-1)
    , unk58(-1)
    , unk5C(-1)
    , unk60(nullptr)
    , unk64(0)
    , unk70(nullptr)
    , unk74(0)
    , unk78(0)
    , unk7C(0)
    , unk11C(0)
    , unk120(nullptr)
    , unk24C(0.0f)
    , unk250(0.0f)
    , unk254(0)
    , unk268(0.0f)
    , unk26C(1.0f)
    , unk274(0)
    , unk276(0)
    , unk278(0)
    , unk27A(0)
    , unk27C(0)
    , unk27E(0)
    , unk280(0)
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
    , unk2B4(nullptr)
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
	unk68                       = new TCameraKindParam;
	unk6C                       = new TCameraInbetween;
	unk2AC                      = new CameraUnk2ACStruct;
	unk2B0                      = new TCameraBck;
	unk2B4                      = new CameraUnk2B4Struct;
	unk2D0                      = new TCamSaveNotice;
	unk2D4                      = new TCamSaveEx;
	for (int i = 0; i < ARRAY_COUNT(unk2D8); ++i)
		unk2D8[i] = new TCamSaveKindParam(mCamKindNameSaveFile[i]);
	if (SMS_isMultiPlayerMap())
		createMultiPlayer(4);
	int stage = gpMarDirector->getCurrentStage();
	if (gpMarDirector->getCurrentMap() == 58 && (stage == 0 || stage == 1)) {
		unk64 |= 0x1000;
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
	unk5C = 0;
	if (gpMarDirector->getCurrentMap() == 7) {
		unk5C = 0x14;
	} else if (SMS_isExMap()) {
		unk5C = 0x26;
	} else if (SMS_isMultiPlayerMap()) {
		unk5C = 2;
	} else if (unk64 & 0x1000) {
		unk5C = 0x2E;
	}

	TCameraMapTool* tool = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(cStartCamName), cStartCamName);

	if (tool)
		changeCamModeSpecifyCamMapToolAndFrame_(tool, 1);
	else
		changeCamModeSpecifyFrame_(unk5C, 1);

	unk68->copySaveParam(*unk2D8[mMode]);
	mFovy = unk68->mFovy;
	mNear = unk68->mNearClip;

	char acStack_54[0x40];
	snprintf(acStack_54, 0x40, "%s%d", cStartAfterCamName,
	         gpMarDirector->unkD0);
	TCameraMapTool* tool2 = (TCameraMapTool*)gpCamMapToolTable->searchF(
	    JDrama::TNameRef::calcKeyCode(acStack_54), acStack_54);
	if (tool2) {
		unk80.unk28 = MsClamp(tool2->unkC.y, unk268, unk26C);
		unk80.unk26 = CLBDegToShortAngle(tool2->unk18.y) - 0x8000;
	} else {
		unk80.unk28 = MsClamp(unk2D4->mXRotStart.get(), unk268, unk26C);
		unk80.unk26 = *gpMarioAngleY - 0x8000;
	}

	if ((unk64 & 0x1000) && unk2B8 != nullptr && (unk2B8->unkC & 0x1))
		setUpToLButtonCamera_(0x2E);

	unk270 = unk80.unk28;
	unk80.unk24
	    = CLBLinearInbetween(unk68->mXAngleMin, unk68->mXAngleMax, unk80.unk28);

	JGeometry::TVec3<f32> marPos = SMS_GetMarioPos();
	f32 fVar1;
	if (isNormalDeadDemo()) {
		fVar1 = 35.0f;
	} else {
		fVar1 = unk80.unk28 * unk68->mXRotRatioAtOffsetY + unk68->mAtOffsetY;
		if (SMS_GetMarioStatus() == MARIO_STATUS_KICK_ROOF_ROLL_UP)
			fVar1 += 260.0f;
		if (mMode == 9)
			fVar1 += unk290;
	}

	marPos.y += fVar1;
	gpCameraMario->unk0.set(marPos);

	if (unk70 != nullptr) {
		unk70->calcPosAndAt(&mPosition, &mTarget);
	} else {
		mTarget.set(gpCameraMario->unk0);
		f32 fVar7
		    = CLBLinearInbetween(unk68->mDistMin, unk68->mDistMax, unk80.unk28);
		CLBPolarToCross(mTarget, &mPosition, fVar7, unk80.unk24, unk80.unk26);
	}

	calcSecureViewTarget_(unk80.unk26, &unk294, &unk298);

	mPosition.x += unk294;
	mPosition.z += unk298;

	mTarget.x += unk294;
	mTarget.z += unk298;

	unk80.unk0.set(mPosition);
	unk80.unk18.set(mPosition);
	unk80.unkC.set(mTarget);

	TCameraOption* option = gpCameraOption;
	if (SMS_isOptionMap()) {
		unk80.unk0 = mPosition;
		unk80.unkC = mTarget;
		option     = new TCameraOption(mPosition, &unk80.unkC);
	}
	gpCameraOption = option;

	unk256 = unk80.unk24;
	unk258 = unk80.unk26;

	unkB4 = unk80;
	unkE8 = unkB4;

	unk124.set(mPosition);
	unk130.set(mPosition);
	unk13C.set(mPosition);
	unk148.set(mTarget);
	unk154.set(mTarget);
	unk160.set(mTarget);

	if (unk64 & 0x1000) {
		unk2B8->unk10.set(mPosition);
		unk2B8->unk1C.set(mTarget);
	}

	unk6C->initCameraInbetween(mPosition, mTarget, SMS_GetMarioPos());

	C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);
	C_MTXLookAt(unk1EC, unk124, mUp, unk148);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			unk1AC[i][j] = unk16C[i][j];

	MTXCopy(unk1EC, unk21C);

	fabricatedInline2();

	if ((unk64 & 0x1000) && gpMarDirector->unk7D == 1) {
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
	if (unk6C->unk4 > 0)
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
	const JGeometry::TVec3<f32>* thing = unk2B4->unk0;
	if (thing)
		result = *thing;
	else
		result = gpCameraMario->unk0;
	return result;
}

s16 CPolarSubCamera::calcAngleXFromXRotRatio_() const
{
	return CLBLinearInbetween(unk68->mXAngleMin, unk68->mXAngleMax,
	                          unk80.unk28);
}

f32 CPolarSubCamera::calcDistFromXRotRatio_() const
{
	return CLBLinearInbetween(unk68->mDistMin, unk68->mDistMax, unk80.unk28);
}

void CPolarSubCamera::calcNowTargetFromPosAndAt_(const Vec& pos, const Vec& at)
{
	f32 radius;
	s16 xAngle;
	s16 yAngle;
	CLBCrossToPolar(at, pos, &radius, &xAngle, &yAngle);
	unk80.unk28 = CLBCalcRatio(unk68->mXAngleMin, unk68->mXAngleMax, xAngle);
	if (isLButtonCameraSpecifyMode(mMode)) {
		unk80.unk28 = MsClamp(unk80.unk28, 0.0f, 1.0f);
	} else {
		unk80.unk28 = MsClamp(unk80.unk28, unk268, unk26C);
	}

	unk80.unk24
	    = CLBLinearInbetween(unk68->mXAngleMin, unk68->mXAngleMax, unk80.unk28);
	unk80.unk26 = yAngle;
	unk80.unk0.set(pos);
	unk80.unk18.set(pos);
	unk80.unkC.set(at);
}

void CPolarSubCamera::rotateX_ByStickY_(f32 param_1)
{
	if (SMS_IsMarioOpeningDoor())
		return;

	unk80.unk28 -= param_1 * unk68->mXRotRatioManualSpeed;
	if (isLButtonCameraSpecifyMode(mMode)) {
		unk80.unk28 = MsClamp(unk80.unk28, 0.0f, 1.0f);
	} else {
		unk80.unk28 = MsClamp(unk80.unk28, unk268, unk26C);
	}
}

void CPolarSubCamera::rotateY_ByStickX_(f32 param_1)
{
	if (SMS_IsMarioOpeningDoor())
		return;

	s16 thing
	    = param_1
	      * CLBLinearInbetween(unk68->mYAngleManualSpeedXMin,
	                           unk68->mYAngleManualSpeedXMax, unk80.unk28);
	unk80.unk26 += thing;
}

void CPolarSubCamera::offMoveApproach_() { unk6C->unk44 = 0.0f; }

void CPolarSubCamera::onMoveApproach_()
{
	// Why...
	f32 d = MsSqrtf((mPosition.x - mTarget.x) * (mPosition.x - mTarget.x)
	                + (mPosition.y - mTarget.y) * (mPosition.y - mTarget.y)
	                + (mPosition.z - mTarget.z) * (mPosition.z - mTarget.z));
	unk6C->unk44
	    = d - CLBLinearInbetween(unk68->mDistMin, unk68->mDistMax, unk80.unk28);
}

bool CPolarSubCamera::isMarioReadyGun_() const { }

bool CPolarSubCamera::isMarioAimWithGun_() const
{
	return gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)
	       && gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK8000)
	       && unk120->checkFrameMeaning(0x400);
}

bool CPolarSubCamera::isMarioCrabWalk_() const
{
	return gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)
	       && gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK8000)
	       && unk120->checkFrameMeaning(0x8000);
}

void CPolarSubCamera::execInvalidAutoChase_()
{
	unk284 = unk68->mAutoChaseCompleteFrame;
}

bool CPolarSubCamera::isMomentDefinite_() const
{
	bool result = false;
	if (!(unk64 & 0x100) && isNormalCameraCompletely() && unk250 > 0.001f
	    && CLBLinearInbetween(unk68->mCushionMin, unk68->mCushionMax,
	                          unk80.unk28)
	           > 0.001f)
		result = true;
	return result;
}

void CPolarSubCamera::calcSlopeAngleX_(s16* param_1)
{
	s16 result        = 0;
	bool isAimingFlag = false;
	if (gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)
	    && gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK8000))
		isAimingFlag = true;

	if (!isAimingFlag) {
		bool groundOK             = false;
		const TBGCheckData* plane = *gpMarioGroundPlane;
		if (plane != nullptr && (plane->mBGType & 0xA000 ? true : false))
			groundOK = true;

		if (groundOK && isSlopeCameraMode()) {
			JGeometry::TVec3<f32> diff;
			diff.set(gpMarioPos->x - mPosition.x, 0.0f,
			         gpMarioPos->z - mPosition.z);
			bool b = diff.isZero();
			if (!b) {
				f32 grLevel = SMS_GetMarioGrLevel();
				s16 maxAng  = unk2D4->mSLSlopeMaxAngleX.get();
				f32 fwdDist = unk2D4->mSLSlopeForwardDistXZ.get();

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

	CLBChaseGeneralConstantSpecifySpeed<s16>(&unk28C, result,
	                                         unk2D4->mSLSlopeSpeedAngleX.get());
	*param_1 -= unk28C;
	*param_1 = MsClamp(*param_1, unk2D4->mSLLimitMinAngleX.get(),
	                   unk2D4->mSLLimitMaxAngleX.get());
}

void CPolarSubCamera::calcPosAndAt_()
{
	if (gpCameraMario->unkC >= 0.05f)
		unk64 &= ~0x80;

	if (!(unk64 & 0x80)) {
		if (unk284 > 0) {
			s32 acf   = unk68->mAutoChaseCompleteFrame;
			s32 acs   = unk68->mAutoChaseStartFrame;
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
	s16 yawSpeed
	    = CLBLinearInbetween<s16>(unk68->mYAngleManualSpeedXMin,
	                              unk68->mYAngleManualSpeedXMax, unk80.unk28);
	f32 distSpeed = CLBLinearInbetween<f32>(
	    unk68->mHoldAddDistXZMin, unk68->mHoldAddDistXZMax, unk80.unk28);

	if (isNormalCameraSpecifyMode(mMode)) {
		if (gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)
		    && gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK8000)) {
			f32 mag   = gpCameraMario->unk1C;
			distDelta = mag * distSpeed;
			yawDelta  = (s16)(mag * (f32)yawSpeed);
		}
	}

	CLBChaseAngleDecrease(&unk2AC->unk0, yawDelta,
	                      unk2D4->mSLAimAngleYChaseMin.get());
	CLBChaseDecrease(&unk2AC->unk4, distDelta, unk2D4->mSLHoldDistChase.get(),
	                 0.0f);

	if (distSpeed < 0.001f) {
		if (yawSpeed != 0) {
			s16 absSpeed = CLBAbs(yawSpeed);
			s16 absDelta = CLBAbs(yawDelta);
			unk2AC->unkC = (f32)absDelta * (1.0f / (f32)absSpeed);
			if (unk2AC->unkC > 1.0f)
				unk2AC->unkC = 1.0f;
			else if (unk2AC->unkC < 0.0f)
				unk2AC->unkC = 0.0f;
		}
	} else {
		unk2AC->unkC = distDelta * (1.0f / distSpeed);
		if (unk2AC->unkC > 1.0f)
			unk2AC->unkC = 1.0f;
		else if (unk2AC->unkC < 0.0f)
			unk2AC->unkC = 0.0f;
	}

	CLBChaseDecrease(&unk2AC->unk8, unk2AC->unkC,
	                 unk2D4->mSLHoldDistChase.get(), 0.0f);

	unk64 &= ~0x100;
	if (unk78 != 0)
		unk78 -= 1;
	if (unk7C != 0)
		unk7C -= 1;

	bool freeze = true;
	if (gpCameraMario->unkC >= 0.05f && gpCameraMario->unk10 >= 0.05f)
		freeze = false;

	bool checkInput = true;
	bool isAiming   = false;
	if (gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_FLUDD)
	    && gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_UNK8000))
		isAiming = true;
	if (!isAiming && !(unk64 & 4))
		checkInput = false;

	bool inputActive = true;
	if (checkInput) {
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

	if (unk64 & 0x40) {
		if (unk78 == 0)
			unk78 = 1;
		if (unk7C == 0)
			unk7C = 1;
	} else {
		if (freeze || inputActive) {
			unk7C = 0;
			unk78 = 0;
		}
		if ((unk64 & 0x400) && unk78 == 0)
			unk78 = 1;
	}

	JGeometry::TVec3<f32> finalAt;
	if (unk78 == 0 || unk7C == 0) {
		if (unk78 == 0 && unk7C == 0)
			unk64 &= ~0x40;

		if (!unk6C->isThing() || freeze || inputActive) {
			if (unk7C == 0 && isDefiniteCameraSpecifyMode(mMode)) {
				const JGeometry::TVec3<f32>* ovr = unk2B4->unk0;
				JGeometry::TVec3<f32> origin;
				if (ovr != nullptr)
					origin = *ovr;
				else
					origin = gpCameraMario->unk0;
				JGeometry::TVec3<f32> copy = origin;
				unk80.unkC.set(copy);
			}

			if (isThing4(mMode)) {
				finalAt = unk80.unkC;
				unk28C  = 0;
			} else {
				unk80.unk24 = CLBLinearInbetween<s16>(
				    unk68->mXAngleMin, unk68->mXAngleMax, unk80.unk28);
				s16 xAngle = unk80.unk24 + unk68->mOffsetAngleX + unk2AC->unk0;
				s16 yAngle = unk80.unk26 + unk68->mOffsetAngleY;

				if (gpCameraMario->unkC >= 0.05f) {
					s16 mAngle = *gpMarioAngleY - 0x8000;
					f32 m      = MsClamp<f32>(
                        (f32)unk68->mMaxAddAngleY
                            * (0.5f * (1.0f - JMASCos((mAngle - unk258) * 2))),
                        -32766.998f, 32766.998f);

					if ((s16)(mAngle - yAngle) < 0)
						m = -m;

					s16 addAngleYSpeed = unk2D4->mSLAddAngleYSpeed.get();
					CLBChaseGeneralConstantSpecifySpeed<s16>(
					    &unk28E, CLBRoundf<s16>(m), addAngleYSpeed);
				}
				yAngle += unk28E;

				calcSlopeAngleX_(&xAngle);

				if (mMode == 0x41) {
					if (gpCameraMario->isMarioRocketing()) {
						xAngle = unk2D4->mSLLimitMaxAngleX.get() - 0x1770;
					} else if (SMS_GetMarioStatus() == MARIO_STATUS_HIP_DROP) {
						xAngle = 0x3E8;
					}
				}

				finalAt.x = -(unk68->mOffsetLookatXZ * JMASSin(yAngle - 0x4000)
				              - unk80.unkC.x);
				finalAt.y = unk80.unkC.y;
				finalAt.z = -(unk68->mOffsetLookatXZ * JMASCos(yAngle - 0x4000)
				              - unk80.unkC.z);

				if (mMode != 2) {
					execSecureView_(yAngle, finalAt);

					if (!isRailCameraSpecifyMode(mMode)) {
						JGeometry::TVec3<f32> saveAt;
						saveAt.set(finalAt);
						f32 dist = CLBLinearInbetween<f32>(
						    unk68->mDistMin, unk68->mDistMax, unk80.unk28);
						s16 cushAng = CLBLinearInbetween<s16>(
						    unk68->mXAngleMin, unk68->mXAngleMax, unk80.unk28);
						f32 cushion = JMASCos(cushAng);

						JGeometry::TVec3<f32> newPos;
						CLBPolarToCross(saveAt, newPos, dist + unk6C->unk44,
						                xAngle, yAngle);

						if (isNormalCameraCompletely()) {
							JGeometry::TVec3<f32> base = unkB4.unk0;
							base.y                     = unkB4.unk18.y;
							f32 nDist;
							s16 nVA, nHA;
							CLBCrossToPolar(saveAt, base, &nDist, &nVA, &nHA);
							f32 cushDist = cushion * nDist;
							if (cushDist > cushion * dist) {
								unk64 |= 0x100;
								unk80.unk0 = newPos;
							} else {
								f32 minDist
								    = cushion
								      * (dist
								         - CLBLinearInbetween<f32>(
								             unk68->mCushionMin,
								             unk68->mCushionMax, unk80.unk28));
								if (minDist < unk2D4->mSLMinCushionXZ.get())
									minDist = unk2D4->mSLMinCushionXZ.get();

								if (fabricatedInline3() && cushDist < minDist) {
									f32 diff = minDist - cushDist;
									base.x += diff * JMASSin(nHA);
									base.z += diff * JMASCos(nHA);
									CLBCrossToPolar(saveAt, base, &nDist, &nVA,
									                &nHA);
								}

								bool useMid = false;
								if (!(unk64 & 0x100)
								    && isNormalCameraCompletely()
								    && unk250 > 0.001f) {
									if (CLBLinearInbetween<f32>(
									        unk68->mCushionMin,
									        unk68->mCushionMax, unk80.unk28)
									    > 0.001f)
										useMid = true;
								}
								if (useMid) {
									unk80.unk0.set(base);
								} else {
									CLBPolarToCross(saveAt, unk80.unk0, nDist,
									                nVA, yAngle);
								}
								unk80.unk0.y = newPos.y;
							}
						} else {
							unk80.unk0 = newPos;
						}

						f32 sY = JMASSin(yAngle);
						f32 cY = JMASCos(yAngle);

						if (fabricatedInline3()) {
							f32 dx   = gpMarioPos->x - unk80.unk0.x;
							f32 dz   = gpMarioPos->z - unk80.unk0.z;
							f32 d    = MsSqrtf(dx * dx + dz * dz);
							f32 minD = unk2D4->mSLMinCushionXZ.get();
							f32 mD2
							    = minD < dist * cushion ? dist * cushion : minD;
							if (d < mD2) {
								f32 add = mD2 - d;
								unk80.unk0.x += sY * add;
								unk80.unk0.z += cY * add;
							}
						}

						switch (mMode) {
						case 0xD:
						case 0x8:
							break;
						default:
							unk80.unk0.x = -(unk2AC->unk4 * sY - unk80.unk0.x);
							unk80.unk0.z = -(unk2AC->unk4 * cY - unk80.unk0.z);
							break;
						}

						if (unk6C->isThing() && inputActive) {
							JGeometry::TVec3<f32> warpPos;
							f32 d = CLBLinearInbetween<f32>(
							    unk68->mDistMin, unk68->mDistMax, unk80.unk28);
							CLBPolarToCross(unk6C->unk24, warpPos,
							                d + unk6C->unk44, xAngle, yAngle);
							unk6C->warpPosAndAt(warpPos, unk6C->unk24);
						}
					}
				}
				mPosition.x = unk80.unk0.x;
				mPosition.z = unk80.unk0.z;
				execHeightPan_();
				finalAt.y = unk80.unk18.y;
				Vec posCpy;
				posCpy = mPosition;
				if (isNeedWallCheck_() && execWallCheck_(&posCpy)) {
					CLBCrossToPolar(unk80.unkC, mPosition, &unk80.unk24,
					                &unk80.unk26);
					unk80.unk28 = unk80.unk30;
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
			unk6C->execCameraInbetween(unk80.unk0, copy, SMS_GetMarioPos());
		}
	}

	if (unk78 == 0) {
		f32 chaseXZ;
		f32 chaseY;
		if (isRailCameraSpecifyMode(unk54) && isRailCameraSpecifyMode(mMode)
		    && unk6C->isThing2()) {
			chaseXZ = 1.0f;
			chaseY  = 1.0f;
		} else {
			chaseXZ = unk68->mPosChaseRateXZ;
			chaseY  = unk68->mPosChaseRateY;
			if (mMode == 0x41
			    && SMS_GetMarioStatus() == MARIO_STATUS_HIP_DROP) {
				u32 v = gpCameraMario->unk18;
				if (v < 0x78) {
					chaseY = CLBLinearInbetween<f32>(
					    unk68->mPosChaseRateY, 0.0f, (f32)(0x78 - v) / 120.0f);
				}
			}
			if (unk64 & 4) {
				chaseXZ = 1.0f;
			} else if (unk120->mCompSPos[6] != 0.0f
			           || unk120->mCompSPos[7] != 0.0f) {
				f32 v = gpCameraMario->unkC;
				if (v > 20.0f)
					v = 20.0f;
				chaseXZ = CLBEaseInInbetween<f32>(
				    unk68->mPosChaseRateXZ, unk68->mPosChaseRateXZ_C,
				    CLBCalcRatio<f32>(20.0f, 0.0f, v));
			}
			if (unk120->mCompSPos[6] != 0.0f) {
				f32 v = gpCameraMario->unk10;
				if (v > 20.0f)
					v = 20.0f;
				chaseY = CLBEaseInInbetween<f32>(
				    unk68->mPosChaseRateY, unk68->mPosChaseRateY_C,
				    CLBCalcRatio<f32>(20.0f, 0.0f, v));
			}
		}
		const JGeometry::TVec3<f32>& v = unk6C->unk18;
		CLBChaseDecrease(&mPosition.x, v.x, chaseXZ, 0.0f);
		CLBChaseDecrease(&mPosition.y, v.y, chaseY, 0.0f);
		CLBChaseDecrease(&mPosition.z, v.z, chaseXZ, 0.0f);
	}

	if (unk7C == 0) {
		f32 atXZ = unk68->mAtChaseRateXZ;
		f32 atY  = unk68->mAtChaseRateY;

		const JGeometry::TVec3<f32>& v = unk6C->unk24;
		CLBChaseDecrease(&mTarget.x, v.x, atXZ, 0.0f);
		CLBChaseDecrease(&mTarget.y, v.y, atY, 0.0f);
		CLBChaseDecrease(&mTarget.z, v.z, atXZ, 0.0f);
	}
}

void CPolarSubCamera::calcFinalPosAndAt_()
{
	if (mMode != 0x49) {
		gpCameraShake->execShake(unk124, &unk148, &mUp);
		if (mMode != 0x2E) {
			if (!isFixCameraSpecifyMode(mMode) || unk6C->isThing2()) {
				if (isHellDeadDemo()) {
					if (unk256 > unk2D4->mSLLimitMaxAngleX.get()) {
						unk7C = 1;
						unk64 |= 0x40;
					}
				} else {
					CLBRevisionLookatByAngleX(unk2D4->mSLLimitMinAngleX.get(),
					                          unk2D4->mSLLimitMaxAngleX.get(),
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
	if (!(unk64 & 0x400))
		execDeadDemoProc_();
	if (!(unk64 & 0x80)) {
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
		yOffset = unk80.unk28 * unk68->mXRotRatioAtOffsetY + unk68->mAtOffsetY;
		if (SMS_GetMarioStatus() == MARIO_STATUS_KICK_ROOF_ROLL_UP)
			yOffset += 260.0f;
		if (mMode == 9)
			yOffset += unk290;
	}
	marPos.y += yOffset;
	gpCameraMario->unk0.set(marPos);
	gpCameraMario->calcAndSetMarioData();

	unkB4 = unk80;

	if (gpMarDirector->mState == 4 && !(unk64 & 0x400)) {
		if (isTalkCameraSpecifyMode(mMode)) {
			bool inTalk = true;
			u8 v        = gpMarDirector->unk124;
			if (v != 1 && v != 2)
				inTalk = false;
			if (!inTalk) {
				int mode = unk5C;
				changeCamModeSpecifyFrame_(mode,
				                           getCameraInbetweenFrame_(mode));
			}
		} else if (!isSimpleDemoCamera()) {
			int code;
			if (controlByCameraCode_(&code))
				execCameraModeChangeProc_(code);
		}
	}

	TCameraKindParam param;
	param.copySaveParam(*unk2D8[mMode]);

	if (unk6C->unk4 > 0)
		unk68->inbetweenData(param, (f32)unk6C->unk4);
	else
		*unk68 = param;

	if (unk284 > unk68->mAutoChaseCompleteFrame)
		unk284 = unk68->mAutoChaseCompleteFrame;

	if (!isNormalDeadDemo() && !(unk64 & 0x1200))
		mFovy = unk68->mFovy;
	mNear = unk68->mNearClip;
	mFar  = 300000.0f;

	if (isNormalDeadDemo()) {
		ctrlNormalDeadDemo_();
	} else {
		switch (mMode) {
		case 2:
			ctrlMultiPlayerCamera_();
			break;
		case 0x2E:
			ctrlJetCoasterCamera_();
			break;
		default:
			if (isThing4(mMode))
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
		if (mMode != 0x49) {
			if (SMS_isOptionMap())
				ctrlOptionCamera_();
			else
				ctrlGameCamera_();
			calcFinalPosAndAt_();

			fabricatedInline2();
		}

		if (mMode != 0x49) {
			C_MTXPerspective(unk16C, mFovy, mAspect, mNear, mFar);
			C_MTXLookAt(unk1EC, unk124, mUp, unk148);
		}

		bool flag2 = (param_2->unk0 & 2) ? true : false;
		if (flag2) {
			if (mMode != 0x49) {
				u16 flags = unk64;
				if (!(flags & 0x400)) {
					if (flags & 0x200) {
						updateGateDemoCamera_();
					} else if (flags & 0x1000) {
						unk2B0->updateDemo(&unk2B8->unk10, &unk2B8->unk1C,
						                   &unk2B8->unk28, &unk2B8->unk34);
					}
				}
			}
			if (!SMS_isOptionMap()) {
				if (mMode != 0x2E && mMode != 0x49)
					calcInHouseNo_(false);
			}
		}
		updateDemoCamera_(flag2);
	}

	if (param_1 & 0x14) {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				param_2->mProjMtx.mMtx[i][j] = unk16C[i][j];
		MTXCopy(unk1EC, param_2->getUnkB4());
		param_2->mNearPlane = mNear;
		param_2->mFarPlane  = mFar;
		if (param_1 & 0x10)
			GXSetProjection(param_2->mProjMtx.mMtx, GX_PERSPECTIVE);
	}
}

s16 CPolarSubCamera::getOffsetAngleX() const { return unk68->mOffsetAngleX; }

s16 CPolarSubCamera::getOffsetAngleY() const { return unk68->mOffsetAngleY; }

s16 CPolarSubCamera::getFinalAngleZ() const
{
	return unk254 + gpCameraShake->mRollAccum;
}
