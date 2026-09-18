#include <MSound/MAnmSound.hpp>
#include <MSound/MSHandle.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

MAnmSound::MAnmSound(MSound* sound) { mData = nullptr; }

void MAnmSound::initAnmSound(void* interface, u32 param_2, f32 frame)
{
	initActorAnimSound(interface, param_2, frame);
}

void MAnmSound::animeLoop(Vec* position, f32 frame, f32 speed, u32 ground_no,
                          u8 param_5)
{
	if (mData != nullptr)
		setAnimSoundVec(JAIBasic::getInterface(), position, frame, speed,
		                ground_no, param_5);
}

void MAnmSound::startAnimSound(void* interface, u32 id,
                               JAISoundHandle* out_handle, JAIActor* actor,
                               u8 camera_idx)
{
	if (MSGMSound->gateCheck(id))
		MSoundSESystem::MSoundSE::startSoundActorInner(id, out_handle, actor, 0,
		                                               camera_idx);
}

void MAnmSound::setSpeedModifySound(JAISound* sound,
                                    JAIAnimeFrameSoundData* frame_data,
                                    f32 speed)
{
	if (MSound::getSwitch(sound->getID(), MSSeSwBit_AnimeSpeed,
	                      MSSeSwBit_AnimeSpeedShift))
		JAIAnimeSound::setSpeedModifySound(sound, frame_data, speed);
}

// Fabricated name; fully inlined into MAnmSoundMario::startAnimSound, which is
// its only user, so the map has no symbol for it.
static u32 get_thing(u32 param_1)
{
	u32 uVar1 = param_1 >> 30;
	u32 uVar2 = param_1 >> 12 & 0xF;

	if (uVar1 == 0)
		return uVar2;

	if (uVar1 == 2)
		return 0x10;

	if (uVar1 == 3)
		return 0x11;

	return 0xffffffff;
}

void MAnmSoundMario::startAnimSound(void* interface, u32 sound_id,
                                    JAISound** out_handle, JAIActor* actor,
                                    u8 camera_idx)
{
	if (MSGMSound->gateCheck(sound_id)) {
		u32 category = get_thing(sound_id);
		switch (category) {
		case 0:
			if ((actor->mGroundNumber & 0x1000) == 0x1000)
				return;
			break;

		// TODO: instructions and frame exact; r0 and r5 are swapped between
		// groundNo and voiceNo. Register-numbering residue only -- naming the
		// volume, the MSound pointer, a const or an s32 voiceNo all leave it
		// unchanged.
		case 7: {
			u32 groundNo = actor->mGroundNumber;
			u32 voiceNo  = (groundNo >> 24) & 0xF;
			MSGMSound->startMarioVoice(sound_id, voiceNo, groundNo >> 28);
			return;
		}
		}

		MSoundSESystem::MSoundSE::startSoundActorInner(sound_id, out_handle,
		                                               actor, 0, camera_idx);
	}
}

f32 MSMarioPosVolume::getDistFromMario(const Vec& pos)
{
	if (MSGMSound->cameraLooksAtMario()) {
		const Vec* mario = MSGMSound->unkAC[0].mPosition;
		return std::sqrtf(std::powf(pos.x - mario->x, 2.0f)
		                  + std::powf(pos.y - mario->y, 2.0f)
		                  + std::powf(pos.z - mario->z, 2.0f));
	}

	return 0.0f;
}

void MAnmSoundNPC::startAnimSound(void* interface, u32 sound_id,
                                  JAISound** out_handle, JAIActor* actor,
                                  u8 camera_idx)
{
	if (MSGMSound->gateCheck(sound_id)) {
		JAIAnimeSoundData* ptr = mData;

		if (ptr->mEntries[mDataCounter].unk10 & 0xFFFF0000) {
			if (ptr->mEntries[mDataCounter].unk10 & 0xFF000000) {
				u32 uVar5 = mLoopCount;
				u32 uVar6 = (uVar5 >> 24) + 1;
				if (uVar5 != 0) {
					u32 uVar3 = uVar5 + unk98 % uVar6;
					if (uVar3 % uVar6 != 0)
						return;
				}
			}

			if (ptr->mEntries[mDataCounter].unk10 & 0xFF0000) {
				u8 b = ptr->mEntries[mDataCounter].unk10 >> 16;
				b += 1;
				b *= JAIConst::random.get_ufloat_1();
				if (b != 0)
					return;
			}
		}

		if (MSoundSESystem::MSoundSE::checkMonoSound(sound_id, actor)) {
			MSoundSESystem::MSoundSE::startSoundActorInner(
			    sound_id, out_handle, actor, 0, camera_idx);

			if (*out_handle != nullptr
			    && !(ptr->mEntries[mDataCounter].unk10 & 0x8000)) {

				f32 dVar10 = 1.0f;

				f32 fVar11
				    = MSMarioPosVolume::getDistFromMario(*actor->mTranslation);

				if (fVar11 != 0.0f)
					dVar10 = MSHandle::calcVolume(
					    fVar11, 2000.0f, 600.0f,
					    ptr->mEntries[mDataCounter].unk10 >> 12 & 7, 8);

				(*out_handle)->setSeInterVolume(0, dVar10, 0, 0);
			}
		}
	}
}
