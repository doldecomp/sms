#include <JSystem/JAudio/JAInterface/JAIEntrySe.hpp>
#include <JSystem/JAudio/JAInterface/JAIData.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>
#include <JSystem/JAudio/JAInterface/JAIConst.hpp>

void JAISeEntry::storeBuffer(JAISoundHandle* out_handle, JAIActor* actor,
                             u32 sound_id, u32 fade, u8 camera_idx, void* info)
{
	JAIData* data = unk0->unk0;
	if (!out_handle || !*out_handle || (*out_handle)->mSoundID != sound_id
	    || ((*out_handle)->mSoundID == sound_id
	        && (sound_id & 0xC00) == 0x800)) {
		if (checkSoundHandle(out_handle, sound_id, info))
			return;
	}

	u32 category = unk0->changeIDToCategory(sound_id);
	JAISound* it = data->mSeRegist[(u8)category].mUsedHead;

	JAIActor* actualActor = actor;
	if (!actor)
		actualActor = &JAIConst::nullActor;

	const void* uVar14 = actualActor->mIdentity;

	u8 bVar10 = 0;
	JAISound* local_88[16];

	u32 bVar9
	    = data->mCategoryInfoTable[unk0->mSoundScene][(sound_id >> 12) & 0xFF]
	          .mMaxSameSound;

	while (it != nullptr) {
		if (it->mActor == uVar14) {
			if (it->mSoundID == sound_id
			    && (unk0->getSoundSwBit(info) & 0x80000) == 0) {
				if ((sound_id & 0x800) == 0
				    && it->mState == SOUNDSTATE_Stopping) {
					it->mState = SOUNDSTATE_Playing;
					if (out_handle == nullptr)
						return;
					if (*out_handle != nullptr)
						return;
					it->mMainSoundPPointer = out_handle;
					*out_handle            = it;
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
		if (local_88[0]->getInfoPriority() > unk0->getSoundPrioity(info))
			return;
		if (local_88[0]->getInfoPriority() == unk0->getSoundPrioity(info)
		    && local_88[0]->mState == SOUNDSTATE_Stopping)
			return;
		unk0->releaseSeRegist(local_88[0]);
	}
	JAISound* controller
	    = unk0->getControllerHandle(&data->mSeRegist[(u8)category]);
	if (!controller) {
		if (out_handle)
			*out_handle = nullptr;
	} else {
		controller->setCustomParameterPointer(
		    unk0->getSeParametermeterPointer());
		if (controller->mCustomParameter == 0) {
			*out_handle = nullptr;
		} else {
			controller->mState = SOUNDSTATE_Stored;
			initSoundParameter(controller, out_handle, actualActor, sound_id,
			                   fade, camera_idx, info);
		}
	}
}
