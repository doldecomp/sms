#include <JSystem/JAudio/JAInterface/JAIEntry.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JAInterface/JAIGlobalParameter.hpp>

u32 JAIEntry::checkSoundHandle(JAISoundHandle* handle, u32 sound_id, void* info)
{
	u32 result = 0;

	if (handle) {
		if (*handle) {
			if ((sound_id & JAISoundID_TypeMask)
			    != ((*handle)->getID() & JAISoundID_TypeMask)) {
				(*handle)->stop(0);
			} else {
				if (unk0->getSoundPrioity((*handle)->mInfo)
				    <= unk0->getSoundPrioity(info))
					(*handle)->stop(0);
				else
					result = 1;
			}
		}
	}
	return result;
}

void JAIEntry::initSoundParameter(JAISound* sound, JAISoundHandle* out_handle,
                                  JAIActor* actor, u32 sound_id, u32 fade,
                                  u8 camera_idx, void* info)
{
	sound->setID(sound_id);
	if (actor) {
		sound->mActor = actor->mIdentity;
		if (actor->mIdentity) {
			sound->mActorTrans        = actor->mTranslation;
			sound->unk28              = actor->unk8;
			sound->mActorGroundNumber = actor->mGroundNumber;
		} else {
			sound->mActorTrans        = nullptr;
			sound->unk28              = nullptr;
			sound->mActorGroundNumber = actor->mGroundNumber;
		}
	} else {
		sound->mActor             = nullptr;
		sound->mActorTrans        = nullptr;
		sound->unk28              = nullptr;
		sound->mActorGroundNumber = 0;
	}
	sound->setMainSoundPPointer(out_handle);
	sound->mFadeCounter = fade;
	sound->mCameraIdx   = camera_idx;
	sound->mInfo        = info;
	sound->mWaitTimer   = 10;
	sound->unk5         = JAIGlobalParameter::distanceParameterMoveTime;
	sound->mAdjustPrio  = 0;
	sound->mPlayGameFrameCounter = 0;
	if (out_handle == nullptr)
		return;
	*out_handle = sound;
}

BOOL JAIEntry::checkSoundHandle(JAISoundHandle*, JAISound*) { }

void JAIEntry::checkAllSoundHandle(JAISoundHandle*) { }
