#include <JSystem/JAudio/JAInterface/JAIEntryStream.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>

void JAIStreamEntry::storeBuffer(JAISoundHandle* out_handle, JAIActor* actor,
                                 u32 sound_id, u32 fade, u8 param_5, void* info)
{
	JAISoundHandle sound;

	JAIData* data = unk0->unk0;
	if (checkSoundHandle(out_handle, sound_id, info))
		return;

	sound = unk0->getControllerHandle(&data->mStreamControlBuffer);
	if (!sound) {
		*out_handle = nullptr;
		return;
	}

	sound->setCustomParameterPointer(unk0->getStreamParameter());
	if (sound->mCustomParameter == 0) {
		*out_handle = nullptr;
		return;
	}

	data->initStreamParameter(sound->getStreamParameter());
	sound->mState                           = SOUNDSTATE_Stored;
	sound->mWaitTimer                       = 10;
	unk0->unk0->mStreamUpdate->mPrepareFlag = 0;
	initSoundParameter(sound, out_handle, actor, sound_id, fade, param_5, info);
}
