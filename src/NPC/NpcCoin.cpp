#include <NPC/NpcCoin.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/ItemManager.hpp>
#include <System/MarDirector.hpp>
#include <MSound/MSound.hpp>
#include <Camera/cameralib.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

TNpcCoin::TNpcCoin(int param_1)
    : unk0(nullptr)
    , unk4(0)
    , unk8(0.0f, 0.0f, 0.0f)
    , unk14(0.0f, 0.0f, 0.0f)
{
	unk0 = TMapObjBaseManager::newAndRegisterObjByEventID(param_1, nullptr);
	unk0->kill();
}

void TNpcCoin::execAppearCoin_()
{
	if (unk0 != nullptr) {
		unk0->appear();
		unk0->mPosition = unk8;
		unk0->setVelocityAndFlag10(unk14.x, unk14.y, unk14.z);
		unk0 = nullptr;
	} else {
		TMapObjBase* coin = gpItemManager->makeObjAppear(unk8.x, unk8.y, unk8.z,
		                                                 0x2000000E, true);
		if (coin != nullptr)
			coin->setVelocityAndFlag10(unk14.x, unk14.y, unk14.z);
	}

	if (SMSGetMSound()->gateCheck(0x8807))
		MSoundSESystem::MSoundSE::startSoundNpcActor(0x8807, &unk8, 0, nullptr,
		                                             0, 4);
}

void TNpcCoin::requestAppearCoin(const Vec& param_1, f32 param_2, int param_3)
{
	unk4 = param_3;
	unk8.set(param_1);
	unk14.set(0.0f, JMASin(75), JMACos(75));
	s16 sangle = CLBDegToShortAngle(param_2);
	f32 x      = unk14.x;
	unk14.x    = x * JMASCos(sangle) + unk14.z * JMASSin(sangle);
	unk14.z    = -x * JMASSin(sangle) + unk14.z * JMASCos(sangle);
	unk14 *= 15.0f;
	if (unk4 == 0) {
		if (gpMarDirector->isTalkOrDemoModeNow())
			unk4 = 1;
		else
			execAppearCoin_();
	}
}

void TNpcCoin::updateCoin()
{
	if (unk4 > 0) {
		if (!gpMarDirector->isTalkOrDemoModeNow()) {
			unk4 -= 1;
			if (unk4 == 0)
				execAppearCoin_();
		}
	}
}
