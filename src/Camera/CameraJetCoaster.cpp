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
#include <M3DUtil/InfectiousStrings.hpp>

// NOTE: it is weak but I don't want to put it in the header since it's pretty
// big and only used in one function. Also, dunno even if the size is correct
inline void CPolarSubCamera::drawJetCoasterBalloonMessage_()
{
	u32 flagCount = TFlagManager::smInstance->getFlag(0x60001U);
	u32 objCount  = gpItemManager->getObjNumWithActorType(0x40000132U);

	if (unk2B8->unk38 > 2) {
		unk2B8->unk38 -= 1;
		if (unk2B8->unk38 == 2) {
			unk2B8->unk38 = 1;
			gpMarDirector->setNextStage(0xE05, nullptr);
		}
		return;
	}

	if (unk2B8->unk38 == 1)
		return;

	s32 balloonCode = -1;
	if (flagCount == objCount) {
		TFlagManager::smInstance->setBool(true, 0x30005U);
		unk2B8->unk38 = 0x12C;
		balloonCode   = 0xE002D;
	} else {
		switch (gpMarDirector->unk58) {
		case 0x3C:
			gpMarDirector->getConsole()->startAppearJetBalloon(0, objCount);
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
			unk78 = 0xE10;
			unk7C = 0;

			static const Vec sFixCameraPos = { 3005.0f, 4020.0f, -9560.0f };
			warpPosAndAt(sFixCameraPos, mTarget);

			unk2B8->unk38 = 1;
			break;
		}
	}

	if (balloonCode != -1)
		gpMarDirector->mConsole->startAppearBalloon(balloonCode, true);
}

// fabricated
static void unitVecTo(const JGeometry::TVec3<f32>& from,
                      const JGeometry::TVec3<f32>& to,
                      JGeometry::TVec3<f32>* out)
{
	out->set(to.x - from.x, to.y - from.y, to.z - from.z);
	out->normalize();
}

void CPolarSubCamera::ctrlJetCoasterCamera_()
{
	if (gpMarDirector->getCurrentMap() == 0x3A
	    && gpMarDirector->getCurrentStage() == 0)
		drawJetCoasterBalloonMessage_();

	bool startedLButton = false;
	if (unk120->mEnabledFrameMeaning & 0x4000) {
		startedLButton = true;
		u32 soundID    = 0x4825;
		unk2B8->unkC ^= 1;
		if (unk2B8->unkC & 1)
			soundID = 0x4824;
		SMSGetMSound()->startSoundSystemSE(soundID, 0, nullptr, 0);
	}

	JGeometry::TVec3<f32> newTarget;

	if (unk2B8->unkC & 1) {
		if (startedLButton)
			setUpToLButtonCamera_(CAMERA_MODE_JET_COASTER);

		f32 stickY = -unk120->mCompSPos[5];
		if (unk7C == 0)
			getNozzleTopPos_(&unk80.unkC);

		if (unk78 == 0) {
			rotateX_ByStickY_(stickY);
			unk80.unk24 = calcAngleXFromXRotRatio_();
			unk80.unk26 = gpMarioOriginal->mToroccoAngle;
		}

		newTarget                        = unk80.unkC;
		JGeometry::TVec3<f32>& cameraPos = unk80.unk18;

		s16 offsetAngleX  = getOffsetAngleX();
		s16 angleX        = unk80.unk24;
		s16 offsetAngleY  = getOffsetAngleY();
		s16 angleY        = unk80.unk26;
		MtxPtr toroccoMtx = getToroccoMtx_();

		JGeometry::TVec3<f32> toroccoAxisX;
		JGeometry::TVec3<f32> toroccoAxisY;
		JGeometry::TVec3<f32> toroccoAxisZ;
		toroccoAxisX.set(toroccoMtx[0][0], toroccoMtx[1][0], toroccoMtx[2][0]);
		toroccoAxisY.set(toroccoMtx[0][1], toroccoMtx[1][1], toroccoMtx[2][1]);
		toroccoAxisZ.set(toroccoMtx[0][2], toroccoMtx[1][2], toroccoMtx[2][2]);
		mUp.set(toroccoAxisY);

		cameraPos.scaleAdd(-calcDistFromXRotRatio_(), newTarget, toroccoAxisZ);

		CLBRotatePosAndUp(offsetAngleX + angleX, offsetAngleY + angleY,
		                  toroccoAxisX, toroccoAxisY, newTarget, &cameraPos,
		                  &mUp);

		JGeometry::TVec3<f32> toTarget;
		unitVecTo(cameraPos, newTarget, &toTarget);

		JGeometry::TVec3<f32> lookUp = mUp;
		MsVECNormalize(&lookUp, &lookUp);

		lookUp *= unk80.unk28 * unk68->mXRotRatioAtOffsetY + unk68->mAtOffsetY;

		cameraPos += lookUp;
		newTarget += lookUp;

		JGeometry::TVec3<f32> offsetUp = mUp;
		JGeometry::TRotation3<TMtx33f> rotation(toTarget, -1.570796f);
		JGeometry::TVec3<f32> offsetUpTmp = offsetUp;
		rotation.mult33(offsetUpTmp, offsetUp);
		offsetUp *= unk68->mOffsetLookatXZ;

		cameraPos += offsetUpTmp;
		newTarget += offsetUpTmp;

		// TODO: MsGetRotFromZaxisY is the wrong axis function? Need new one?
		unk254 = CLBDegToShortAngle(MsGetRotFromZaxisY(newTarget));
		mFovy  = unk68->mFovy;
	} else {
		if (startedLButton)
			setUpFromLButtonCamera_();

		// Update jetcoaster manual offsets and chase towards limits
		{
			TCameraJetCoaster* jc = unk2B8;

			// X manual speed
			jc->unk8 = (f32)jc->unk8
			           - unk120->mCompSPos[7]
			                 * (f32)jc->unk0->mSLOffsetAngleXManualSpeed.get();

			jc->unkA = (f32)jc->unkA
			           + unk120->mCompSPos[6]
			                 * (f32)jc->unk0->mSLOffsetAngleYManualSpeed.get();

			jc->unk8
			    = MsClamp<s16>(jc->unk8, -jc->unk0->mSLOffsetAngleXLimit.get(),
			                   jc->unk0->mSLOffsetAngleXLimit.get());
			jc->unkA
			    = MsClamp<s16>(jc->unkA, -jc->unk0->mSLOffsetAngleYLimit.get(),
			                   jc->unk0->mSLOffsetAngleYLimit.get());

			CLBChaseAngleDecrease(&jc->unk4, jc->unk8,
			                      jc->unk0->mSLOffsetAngleXChase.get());
			CLBChaseAngleDecrease(&jc->unk6, jc->unkA,
			                      jc->unk0->mSLOffsetAngleYChase.get());
		}
		// TODO: probably an inline ends here because unk2B8 is re-loaded?

		unk80.unk18 = unk2B8->unk10;
		unk80.unkC  = unk2B8->unk1C;
		mUp         = unk2B8->unk28;
		mFovy       = unk2B8->unk34;

		newTarget = unk80.unkC;

		JGeometry::TVec3<f32> local_bc;
		unitVecTo(unk80.unk18, *gpMarioPos, &local_bc);

		JGeometry::TVec3<f32> local_b0;
		local_b0.cross(mUp, local_bc);
		MsVECNormalize(&local_b0, &local_b0);

		CLBRotatePosAndUp(unk2B8->unk4, unk2B8->unk6, local_b0, mUp,
		                  SMS_GetMarioPos(), &unk80.unk18, &mUp);
	}

	unk80.unk0 = unk80.unk18;

	if (startedLButton) {
		if (unk78 == 0)
			mPosition = unk80.unk0;

		if (unk7C == 0)
			mTarget = newTarget;
	} else {
		if (unk78 == 0) {
			f32 chaseXZ = unk68->mPosChaseRateXZ;
			f32 chaseY  = unk68->mPosChaseRateY;
			CLBChaseDecrease(&mPosition.x, unk80.unk0.x, chaseXZ, 0.0f);
			CLBChaseDecrease(&mPosition.y, unk80.unk0.y, chaseY, 0.0f);
			CLBChaseDecrease(&mPosition.z, unk80.unk0.z, chaseXZ, 0.0f);
		}
		if (unk7C == 0) {
			f32 chaseXZ = unk68->mAtChaseRateXZ;
			f32 chaseY  = unk68->mAtChaseRateY;
			CLBChaseDecrease(&mTarget.x, newTarget.x, chaseXZ, 0.0f);
			CLBChaseDecrease(&mTarget.y, newTarget.y, chaseY, 0.0f);
			CLBChaseDecrease(&mTarget.z, newTarget.z, chaseXZ, 0.0f);
		}
	}
}

TCameraJetCoaster::TCameraJetCoaster()
    : unk4(0)
    , unk6(0)
    , unk8(0)
    , unkA(0)
    , unkC(1)
    , unk10(0.0f, 0.0f, 500.0f)
    , unk1C(0.0f, 0.0f, 0.0f)
    , unk28(0.0f, 1.0f, 0.0f)
    , unk34(60.0f)
    , unk38(0)
{
	unk0 = new TCamSaveJetCoaster;
}

void TCameraJetCoaster::calcNowOffsetAngle(f32, f32) { }
