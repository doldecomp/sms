#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>

BOOL JAIEntry::checkSoundHandle(JAISound** sound_ptr, u32 param, void* data)
{
	BOOL result = FALSE;

	if (sound_ptr) {
		if (*sound_ptr) {
			if ((param & 0xC0000000) != ((*sound_ptr)->unk8 & 0xC0000000)) {
				(*sound_ptr)->stop(0);
			} else {
				if (unk0->getSoundPrioity((*sound_ptr)->unk3C)
				    <= unk0->getSoundPrioity(data))
					(*sound_ptr)->stop(0);
				else
					result = TRUE;
			}
		}
	}
	return result;
}

void JAIEntry::initSoundParameter(JAISound* param_1, JAISound** param_2,
                                  JAIActor* param_3, u32 param_4, u32 param_5,
                                  u8 param_6, void* param_7)
{
	param_1->unk8 = param_4;
	if (param_3) {
		param_1->unk20 = param_3->unk0;
		if (param_3->unk0) {
			param_1->unk24 = param_3->unk4;
			param_1->unk28 = param_3->unk8;
			param_1->unk18 = param_3->unkC;
		} else {
			param_1->unk24 = 0;
			param_1->unk28 = 0;
			param_1->unk18 = param_3->unkC;
		}
	} else {
		param_1->unk20 = 0;
		param_1->unk24 = 0;
		param_1->unk28 = 0;
		param_1->unk18 = 0;
	}
	param_1->unk34 = param_2;
	param_1->unk10 = param_5;
	param_1->unk4  = param_6;
	param_1->unk3C = param_7;
	param_1->unk2  = 10;
	param_1->unk5  = JAIGlobalParameter::distanceParameterMoveTime;
	param_1->unk6  = 0;
	param_1->unk14 = 0;
	if (param_2 == nullptr)
		return;
	*param_2 = param_1;
}

BOOL JAIEntry::checkSoundHandle(JAISound** sound, JAISound* soundParam) { }

void JAIEntry::checkAllSoundHandle(JAISound** sound) { }
