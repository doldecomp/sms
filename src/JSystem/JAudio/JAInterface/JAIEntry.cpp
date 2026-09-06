#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>

u32 JAIEntry::checkSoundHandle(JAISound** sound_ptr, u32 param, void* data)
{
	u32 result = 0;

	if (sound_ptr) {
		if (*sound_ptr) {
			if ((param & JAISoundID_TypeMask)
			    != ((*sound_ptr)->getID() & JAISoundID_TypeMask)) {
				(*sound_ptr)->stop(0);
			} else {
				if (unk0->getSoundPrioity((*sound_ptr)->mInfo)
				    <= unk0->getSoundPrioity(data))
					(*sound_ptr)->stop(0);
				else
					result = 1;
			}
		}
	}
	return result;
}

void JAIEntry::initSoundParameter(JAISound* param_1, JAISound** param_2,
                                  JAIActor* param_3, u32 param_4, u32 param_5,
                                  u8 param_6, void* param_7)
{
	param_1->setID(param_4);
	if (param_3) {
		param_1->mActor = param_3->mIdentity;
		if (param_3->mIdentity) {
			param_1->mActorTrans        = param_3->mTranslation;
			param_1->unk28              = param_3->unk8;
			param_1->mActorGroundNumber = param_3->mGroundNumber;
		} else {
			param_1->mActorTrans        = nullptr;
			param_1->unk28              = nullptr;
			param_1->mActorGroundNumber = param_3->mGroundNumber;
		}
	} else {
		param_1->mActor             = nullptr;
		param_1->mActorTrans        = nullptr;
		param_1->unk28              = nullptr;
		param_1->mActorGroundNumber = 0;
	}
	param_1->setMainSoundPPointer(param_2);
	param_1->mFadeCounter = param_5;
	param_1->unk4         = param_6;
	param_1->mInfo        = param_7;
	param_1->mWaitTimer   = 10;
	param_1->unk5         = JAIGlobalParameter::distanceParameterMoveTime;
	param_1->mAdjustPrio  = 0;
	param_1->mPlayGameFrameCounter = 0;
	if (param_2 == nullptr)
		return;
	*param_2 = param_1;
}

BOOL JAIEntry::checkSoundHandle(JAISound** sound, JAISound* soundParam) { }

void JAIEntry::checkAllSoundHandle(JAISound** sound) { }
