#include <Camera/CameraJetCoaster.hpp>
#include <Camera/camerasave.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/cameralib.hpp>
#include <Camera/Camera.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/MarioGamePad.hpp>
#include <MoveBG/ItemManager.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MSound/MSound.hpp>
#include <GC2D/GCConsole2.hpp>
#include <JSystem/JGeometry.hpp>
#include <JSystem/JGeometry/JGRotation3.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>
#include <MarioUtil/MathUtil.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// NOTE: it is weak but I don't want to put it in the header since it's pretty
// big and only used in one function. Also, dunno even if the size is correct
inline void CPolarSubCamera::drawJetCoasterBalloonMessage_()
{
	u32 flagCount = TFlagManager::getInstance()->getFlag(MSF_BALLOON_COUNT);
	u32 objCount
	    = gpItemManager->getObjNumWithActorType(ACTOR_TYPE_BALLOON_KOOPA_JR);

	if (unk2B8->getUnk38() > 2) {
		unk2B8->unk38 -= 1;
		if (unk2B8->getUnk38() == 2) {
			unk2B8->unk38 = 1;
			SMSGetMarDirector()->setNextStage(0xE05, nullptr);
		}
		return;
	}

	if (unk2B8->getUnk38() == 1)
		return;

	s32 balloonCode = -1;
	if (flagCount == objCount) {
		TFlagManager::getInstance()->setBool(
		    true, MSF_POPPED_ALL_BALLOONS_IN_PREV_STAGE);
		unk2B8->unk38 = 300;
		balloonCode   = 0xE002D;
	} else {
		switch (SMSGetMarDirector()->mMoveTickCount) {
		case 0x3C:
			SMSGetMarDirector()->getConsole()->startAppearJetBalloon(0,
			                                                         objCount);
			break;
		case 0x1DB:
			balloonCode = 0xE0029;
			break;
		case 0x1D4C:
			balloonCode = 0xE002A;
			break;
		case 0x3A98:
			if ((u32)(objCount - flagCount) >= 7U)
				balloonCode = 0xE002B;
			else
				balloonCode = 0xE002C;
			break;
		case 0x57E4:
			gpMarioOriginal->loserExec();
			mPosFreezeFrames    = 3600;
			mTargetFreezeFrames = 0;

			static const Vec sFixCameraPos = { 3005.0f, 4020.0f, -9560.0f };
			warpPosAndAt(sFixCameraPos, mTarget);

			unk2B8->unk38 = 1;
			break;
		}
	}

	if (balloonCode != -1)
		SMSGetMarDirector()->getConsole()->startAppearBalloon(balloonCode,
		                                                      true);
}

// fabricated
static void unitVecTo(const JGeometry::TVec3<f32>& from,
                      const JGeometry::TVec3<f32>& to,
                      JGeometry::TVec3<f32>* out)
{
	out->set(to.x - from.x, to.y - from.y, to.z - from.z);
	out->normalize();
}

// TODO: 99.8%. Every instruction and register matches; the frame is 0xC0
// short of 0x2B0. 0x14 is missing in the early temps before the
// RotateAboutAxis copy, 0xA8 in the temps after it.
void CPolarSubCamera::ctrlJetCoasterCamera_()
{
	if (SMSGetMarDirector()->getCurrentMap() == 0x3A
	    && SMSGetMarDirector()->getCurrentStage() == 0)
		drawJetCoasterBalloonMessage_();

	bool startedLButton = false;
	if (unk120->checkFrameMeaning(TMarioGamePad::MEANING_Y)) {
		startedLButton     = true;
		MSoundSEId soundID = MSD_SE_SY_CAMERA_OUT;
		unk2B8->toggleLButtonMode();
		if (unk2B8->isLButtonMode())
			soundID = MSD_SE_SY_CAMERA_UP;
		SMSGetMSound()->startSoundSystemSE(soundID, 0, nullptr, 0);
	}

	JGeometry::TVec3<f32> newTarget;

	if (unk2B8->isLButtonMode()) {
		if (startedLButton)
			setUpToLButtonCamera_(CAMERA_MODE_JET_COASTER);

		f32 stickY = -unk120->mCompSPos[1];
		if (mTargetFreezeFrames == 0)
			getNozzleTopPos_(&mCurrentTarget.mTarget);

		if (mPosFreezeFrames == 0) {
			rotateX_ByStickY_(stickY);
			mCurrentTarget.mPitch = calcAngleXFromXRotRatio_();
			mCurrentTarget.mYaw   = gpMarioOriginal->mToroccoAngle;
		}

		newTarget = mCurrentTarget.mTarget;

		s16 angleX        = mCurrentTarget.mPitch + getOffsetAngleX();
		s16 angleY        = mCurrentTarget.mYaw + getOffsetAngleY();
		MtxPtr toroccoMtx = getToroccoMtx_();

		JGeometry::TVec3<f32> toroccoAxisX;
		JGeometry::TVec3<f32> toroccoAxisY;
		JGeometry::TVec3<f32> toroccoAxisZ;
		toroccoAxisX.set(toroccoMtx[0][0], toroccoMtx[1][0], toroccoMtx[2][0]);
		toroccoAxisY.set(toroccoMtx[0][1], toroccoMtx[1][1], toroccoMtx[2][1]);
		toroccoAxisZ.x = toroccoMtx[0][2];
		toroccoAxisZ.y = toroccoMtx[1][2];
		toroccoAxisZ.z = toroccoMtx[2][2];
		mUp.set(toroccoAxisY);

		mCurrentTarget.unk18.scaleAdd(-calcDistFromXRotRatio_(), newTarget,
		                              toroccoAxisZ);

		CLBRotatePosAndUp(angleX, angleY, toroccoAxisX, toroccoAxisY, newTarget,
		                  &mCurrentTarget.unk18, &mUp);

		JGeometry::TVec3<f32> toTarget;
		unitVecTo(mCurrentTarget.unk18, newTarget, &toTarget);

		JGeometry::TVec3<f32> lookUp = mUp;
		MsVECNormalize(&lookUp, &lookUp);

		lookUp *= mCurrentTarget.unk28 * mCurrentParams->mXRotRatioAtOffsetY
		          + mCurrentParams->mAtOffsetY;

		mCurrentTarget.unk18 += lookUp;
		newTarget += lookUp;

		lookUp = mUp;
		RotateAboutAxis(toTarget, -1.570796f, &lookUp);
		lookUp *= mCurrentParams->mOffsetLookatXZ;

		mCurrentTarget.unk18 += lookUp;
		newTarget += lookUp;

		unk254 = CLBDegToShortAngle(MsGetRotFromYaxisZ(toroccoAxisY));
		mFovy  = mCurrentParams->mFovy;
	} else {
		if (startedLButton)
			setUpFromLButtonCamera_();

		unk2B8->calcNowOffsetAngle(unk120->mCompSPos[7], unk120->mCompSPos[6]);

		mCurrentTarget.unk18   = unk2B8->unk10;
		mCurrentTarget.mTarget = unk2B8->unk1C;
		mUp                    = unk2B8->unk28;
		mFovy                  = unk2B8->unk34;

		newTarget = mCurrentTarget.mTarget;

		JGeometry::TVec3<f32> local_b0;
		JGeometry::TVec3<f32> local_bc;
		unitVecTo(mCurrentTarget.unk18, SMS_GetMarioPos(), &local_bc);

		local_b0.cross2(mUp, local_bc);
		MsVECNormalize(&local_b0, &local_b0);

		CLBRotatePosAndUp(unk2B8->getOffsetAngleX(), unk2B8->getOffsetAngleY(),
		                  local_b0, mUp, SMS_GetMarioPos(),
		                  &mCurrentTarget.unk18, &mUp);
	}

	mCurrentTarget.mPosition = mCurrentTarget.unk18;

	if (startedLButton) {
		if (mPosFreezeFrames == 0)
			mPosition = mCurrentTarget.mPosition;

		if (mTargetFreezeFrames == 0)
			mTarget = newTarget;
	} else {
		if (mPosFreezeFrames == 0) {
			CLBChaseDecrease(&mPosition, mCurrentTarget.mPosition,
			                 mCurrentParams->mPosChaseRateXZ,
			                 mCurrentParams->mPosChaseRateY,
			                 mCurrentParams->mPosChaseRateXZ, 0.0f);
		}
		if (mTargetFreezeFrames == 0) {
			CLBChaseDecrease(&mTarget, newTarget,
			                 mCurrentParams->mAtChaseRateXZ,
			                 mCurrentParams->mAtChaseRateY,
			                 mCurrentParams->mAtChaseRateXZ, 0.0f);
		}
	}
}

TCameraJetCoaster::TCameraJetCoaster()
    : unk4(0)
    , unk6(0)
    , unk8(0)
    , unkA(0)
    , mLButtonMode(1)
    , unk10(0.0f, 0.0f, 500.0f)
    , unk1C(0.0f, 0.0f, 0.0f)
    , unk28(0.0f, 1.0f, 0.0f)
    , unk34(60.0f)
    , unk38(0)
{
	unk0 = new TCamSaveJetCoaster;
}

void TCameraJetCoaster::calcNowOffsetAngle(f32 stick_y, f32 stick_x)
{
	unk8 -= stick_y * unk0->mSLOffsetAngleXManualSpeed.get();
	unkA += stick_x * unk0->mSLOffsetAngleYManualSpeed.get();

	unk8 = MsClamp<s16>(unk8, -unk0->mSLOffsetAngleXLimit.get(),
	                    unk0->mSLOffsetAngleXLimit.get());
	unkA = MsClamp<s16>(unkA, -unk0->mSLOffsetAngleYLimit.get(),
	                    unk0->mSLOffsetAngleYLimit.get());

	CLBChaseAngleDecrease(&unk4, unk8, unk0->mSLOffsetAngleXChase.get());
	CLBChaseAngleDecrease(&unk6, unkA, unk0->mSLOffsetAngleYChase.get());
}
