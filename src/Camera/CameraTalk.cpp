#include <Camera/Camera.hpp>
#include <System/MarioGamePad.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraMarioData.hpp>
#include <NPC/NpcBase.hpp>

void CPolarSubCamera::makeMtxForTalk(const TBaseNPC* param_1)
{
	killHeightPan_();
	mCurrentTarget.unk2C = mCurrentTarget.mYaw;
	mCurrentTarget.mYaw  = *gpMarioAngleY - 0x8000;
	mSavedModeBeforeTalk = mMode;

	int r31 = CAMERA_MODE_TALK_A;
	switch (param_1->getActorType()) {
	case 0x400001B:
		r31 = CAMERA_MODE_TALK_C;
		break;
	case 0x400001A:
		r31 = CAMERA_MODE_TALK_D;
		break;
	case 0x4000007:
		r31 = CAMERA_MODE_TALK_E;
		break;
	default:
		if (param_1->isSmallNpc())
			r31 = CAMERA_MODE_TALK_B;
		break;
	}

	changeCamMode_(r31);
}

void CPolarSubCamera::makeMtxForPrevTalk()
{
	if (isTalkCameraSpecifyMode(mMode)) {
		mCurrentTarget.mYaw = mCurrentTarget.unk2C;

		changeCamMode_(mSavedModeBeforeTalk);

		unk120->onNeutralMarioKey();

		JGadget::TVector_pointer<TBaseNPC>& npcList = gpMarDirector->unk88;
		for (TBaseNPC** it = npcList.begin(); it != npcList.end(); ++it)
			(*it)->npcTalkOut();
	}
}

void CPolarSubCamera::ctrlTalkCamera_()
{
	if (mTargetFreezeFrames == 0)
		mCurrentTarget.mTarget.set(gpCameraMario->unk0);

	calcPosAndAt_();
}
