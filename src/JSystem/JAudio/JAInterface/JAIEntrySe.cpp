#include <JSystem/JAudio/JAInterface/JAIEntrySe.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

void JAISeEntry::storeBuffer(JAISound** sound, JAIActor* param_2, u32 param_3,
                             u32 param_4, u8 param_5, void* param_6)
{
	JAIData* data = unk0->unk0;
	if (!sound || !*sound || (*sound)->mSoundID != param_3
	    || ((*sound)->mSoundID == param_3 && (param_3 & 0xC00) == 0x800)) {
		if (checkSoundHandle(sound, param_3, param_6))
			return;
	}

	u32 category = unk0->changeIDToCategory(param_3);
	JAISound* it = data->mSeRegist[(u8)category].mUsedHead;

	JAIActor* actor = param_2;
	if (!param_2)
		actor = &JAIConst::nullActor;

	const void* uVar14 = actor->mIdentity;

	u8 bVar10 = 0;
	JAISound* local_88[16];

	u32 bVar9
	    = data->mCategoryInfoTable[unk0->mSoundScene][(param_3 >> 12) & 0xFF]
	          .mMaxSameSound;

	while (it != nullptr) {
		if (it->mActor == uVar14) {
			if (it->mSoundID == param_3
			    && (unk0->getSoundSwBit(param_6) & 0x80000) == 0) {
				if ((param_3 & 0x800) == 0
				    && it->mState == SOUNDSTATE_Stopping) {
					it->mState = SOUNDSTATE_Playing;
					if (sound == nullptr)
						return;
					if (*sound != nullptr)
						return;
					it->mMainSoundPPointer = sound;
					*sound                 = it;
					return;
				}
				it->stop(0);
				it     = nullptr;
				bVar10 = 0xff;
			} else {
				if (bVar10 == 0) {
					local_88[bVar10] = it;
				} else {
					if (local_88[0]->getInfoPriority()
					    < it->getInfoPriority()) {
						local_88[bVar10] = it;
					} else {
						for (u32 i = 0; i < bVar10; ++i)
							local_88[i + 1] = local_88[i];
						local_88[0] = it;
					}
				}
				it = it->mNextSound;
				++bVar10;
			}
		} else {
			it = it->mNextSound;
		}
	}

	if (bVar10 == bVar9) {
		if (local_88[0]->getInfoPriority() > unk0->getSoundPrioity(param_6))
			return;
		if (local_88[0]->getInfoPriority() == unk0->getSoundPrioity(param_6)
		    && local_88[0]->mState == SOUNDSTATE_Stopping)
			return;
		unk0->releaseSeRegist(local_88[0]);
	}
	JAISound* controller
	    = unk0->getControllerHandle(&data->mSeRegist[(u8)category]);
	if (!controller) {
		if (sound)
			*sound = nullptr;
	} else {
		controller->setCustomParameterPointer(
		    unk0->getSeParametermeterPointer());
		if (controller->mCustomParameter == 0) {
			*sound = nullptr;
		} else {
			controller->mState = SOUNDSTATE_Stored;
			initSoundParameter(controller, sound, actor, param_3, param_4,
			                   param_5, param_6);
		}
	}
}
