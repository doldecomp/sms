#include <JSystem/JAudio/JAInterface/JAIEntryStream.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>

void JAIStreamEntry::storeBuffer(JAISound** sound, JAIActor* actor, u32 param_3,
                                 u32 param_4, u8 param_5, void* param_6)
{
	JAISound* controller;

	JAIData* data = unk0->unk0;
	if (checkSoundHandle(sound, param_3, param_6))
		return;

	controller = unk0->getControllerHandle(&data->unk21C);
	if (!controller) {
		*sound = nullptr;
		return;
	}

	controller->unk38 = unk0->getStreamParameter();
	if (controller->unk38 == 0) {
		*sound = nullptr;
		return;
	}

	data->initStreamParameter(controller->getStreamParameter());
	controller->unk1         = 1;
	controller->unk2         = 10;
	unk0->unk0->unk184->unk2 = 0;
	initSoundParameter(controller, sound, actor, param_3, param_4, param_5,
	                   param_6);
}
