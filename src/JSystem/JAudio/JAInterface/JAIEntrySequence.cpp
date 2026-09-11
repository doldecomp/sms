#include <JSystem/JAudio/JAInterface/JAIEntrySequence.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>

void JAISeqEntry::storeBuffer(JAISound** out_handle, JAIActor* actor,
                              u32 sound_id, u32 fade, u8 param_5, void* info)
{
	JAISound* controller;
	JAIData* data = unk0->unk0;
	u8 bVar10     = unk0->getSeqTrackNumber(info);
	if (out_handle && *out_handle) {
		if ((*out_handle)->mTrack != bVar10) {
			(*out_handle)->stop(0);
		} else {
			if (checkSoundHandle(out_handle, sound_id, info))
				return;
		}
	}

	u32 doThing;

	JAISoundHandle& sound = data->mSeqTrackInfo[bVar10].mSound;
	if (!sound) {
		doThing = true;
	} else {
		if (sound->mState == SOUNDSTATE_Stopping) {
			JAISystemInterface::stopSeq(sound->getSeqParameter()->mSeqHandle);
			sound->clearMainSoundPPointer();
			unk0->stopSeq(sound);
			doThing = true;
		} else {
			if (sound->mState == SOUNDSTATE_Stored) {
				*out_handle = nullptr;
				return;
			}
			void* p = sound->mInfo;
			u8 p1   = unk0->getSoundPrioity(p);
			u8 p2   = unk0->getSoundPrioity(info);
			if (p1 <= p2) {
				JAISystemInterface::stopSeq(
				    sound->getSeqParameter()->mSeqHandle);
				sound->clearMainSoundPPointer();
				unk0->stopSeq(sound);
				doThing = true;
			} else {
				*out_handle = nullptr;
				return;
			}
		}
	}

	if (doThing) {
		controller = unk0->getControllerHandle(&data->mSeqControlBuffer);
		if (!controller) {
			*out_handle = nullptr;
			return;
		}

		controller->setCustomParameterPointer(
		    unk0->getSeqParametermeterPointer());
		if (!controller->mCustomParameter) {
			*out_handle = nullptr;
			return;
		}

		data->initSeqParameter(controller->getSeqParameter());
		sound           = controller;
		sound->mSoundID = sound_id;

		JAISeqUpdateData* pJVar1 = &data->mSeqTrackInfo[bVar10];
		JAISeqParameter* pvVar5  = sound->getSeqParameter();

		pvVar5->mUpdateData = pJVar1;

		sound->mTrack                    = bVar10;
		data->mSeqTrackInfo[bVar10].unk8 = 1;
		data->mSeqTrackInfo[bVar10].unk2 = 0;
		data->mSeqTrackInfo[bVar10].unk4 = 0;
		data->initSeqTrackInfoParameter(bVar10);
		if (unk0->getSoundSwBit(info) & 1) {
			for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
				JAISound* sound = data->mSeqTrackInfo[i].mSound;
				if (i != bVar10 && sound && sound->mState >= SOUNDSTATE_Started
				    && (sound->getSwBit() & 2) == 0) {
					sound->setSeqInterVolume(10, 0.0f, 10);
					JASystem::TrackMgr::handleToSeq(
					    sound->getSeqParameter()->mSeqHandle)
					    ->pauseTrackAll();
				}
			}
		}
	}

	initSoundParameter(controller, out_handle, actor, sound_id, fade, param_5,
	                   info);
}
