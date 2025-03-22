#include <JSystem/JAudio/JAInterface/JAIEntrySequence.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAISystemInterface.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIParameters.hpp>
#include <JSystem/JAudio/JASystem/JASTrackMgr.hpp>

void JAISeqEntry::storeBuffer(JAISound** sound, JAIActor* actor, u32 param_3,
                              u32 param_4, u8 param_5, void* param_6)
{
	JAISound* controller;
	JAIData* data = unk0->unk0;
	u8 bVar10     = unk0->getSeqTrackNumber(param_6);
	if (sound && *sound) {
		if ((*sound)->unk0 != bVar10) {
			(*sound)->stop(0);
		} else {
			if (checkSoundHandle(sound, param_3, param_6))
				return;
		}
	}

	u32 doThing;

	JAISound** soundSlot = &data->unk180[bVar10].unk48;
	if (!*soundSlot) {
		doThing = true;
	} else {
		if ((*soundSlot)->unk1 == 5) {
			JAISystemInterface::stopSeq((*soundSlot)->getSeqParameter()->unk0);
			(*soundSlot)->clearMainSoundPPointer();
			unk0->stopSeq(*soundSlot);
			doThing = true;
		} else {
			if ((*soundSlot)->unk1 == 1) {
				*sound = nullptr;
				return;
			}
			void* p = (*soundSlot)->unk3C;
			u8 p1   = unk0->getSoundPrioity(p);
			u8 p2   = unk0->getSoundPrioity(param_6);
			if (p1 <= p2) {
				JAISystemInterface::stopSeq(
				    (*soundSlot)->getSeqParameter()->unk0);
				(*soundSlot)->clearMainSoundPPointer();
				unk0->stopSeq(*soundSlot);
				doThing = true;
			} else {
				*sound = nullptr;
				return;
			}
		}
	}

	if (doThing) {
		controller = unk0->getControllerHandle(&data->unk210);
		if (!controller) {
			*sound = nullptr;
			return;
		}

		controller->unk38 = unk0->getSeqParametermeterPointer();
		if (!controller->unk38) {
			*sound = nullptr;
			return;
		}

		data->initSeqParameter(controller->getSeqParameter());
		*soundSlot         = controller;
		(*soundSlot)->unk8 = param_3;

		JAISeqUpdateData* pJVar1 = &data->unk180[bVar10];
		JAISeqParameter* pvVar5  = (*soundSlot)->getSeqParameter();

		pvVar5->unk1850 = pJVar1;

		(*soundSlot)->unk0        = bVar10;
		data->unk180[bVar10].unk8 = 1;
		data->unk180[bVar10].unk2 = 0;
		data->unk180[bVar10].unk4 = 0;
		data->initSeqTrackInfoParameter(bVar10);
		if (unk0->getSoundSwBit(param_6) & 1) {
			for (u32 i = 0; i < JAIGlobalParameter::seqPlayTrackMax; ++i) {
				JAISound* sound = data->unk180[i].unk48;
				if (i != bVar10 && sound && sound->unk1 >= 3
				    && (sound->getSwBit() & 2) == 0) {
					sound->setSeqInterVolume(10, 0.0f, 10);
					JASystem::TrackMgr::handleToSeq(
					    sound->getSeqParameter()->unk0)
					    ->pauseTrackAll();
				}
			}
		}
	}

	initSoundParameter(controller, sound, actor, param_3, param_4, param_5,
	                   param_6);
}
