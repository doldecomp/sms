#include <Camera/Camera.hpp>
#include <System/MarioGamePad.hpp>
#include <System/MarDirector.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/CameraMarioData.hpp>
#include <NPC/NpcBase.hpp>

void CPolarSubCamera::makeMtxForTalk(const TBaseNPC* param_1)
{
	killHeightPan_();
	unk80.unk2C = unk80.unk26;
	unk80.unk26 = *gpMarioAngleY - 0x8000;
	unk58       = mMode;

	int r31 = 0xC;
	switch (param_1->getActorType()) {
	case 0x400001B:
		r31 = 0x3F;
		break;
	case 0x400001A:
		r31 = 0x40;
		break;
	case 0x4000007:
		r31 = 0xA;
		break;
	default:
		if (param_1->isSmallNpc())
			r31 = 0x2D;
		break;
	}

	changeCamModeSpecifyFrame_(r31, getCameraInbetweenFrame_(r31));
}

void CPolarSubCamera::makeMtxForPrevTalk()
{
	if (isTalkCameraSpecifyMode(mMode)) {
		unk80.unk26 = unk80.unk2C;

		int tmp = unk58;
		changeCamModeSpecifyFrame_(tmp, getCameraInbetweenFrame_(tmp));

		unk120->onNeutralMarioKey();

		JGadget::TVector_pointer<TBaseNPC>& npcList = gpMarDirector->unk88;
		for (TBaseNPC** it = npcList.begin(); it != npcList.end(); ++it)
			(*it)->npcTalkOut();
	}
}

void CPolarSubCamera::ctrlTalkCamera_()
{
	if (unk7C == 0)
		unk80.unkC.set(gpCameraMario->unk0);

	calcPosAndAt_();
}
