#include <MSound/MAnmSound.hpp>
#include <MSound/MSHandle.hpp>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

MAnmSound::MAnmSound(MSound* sound) { mData = nullptr; }

void MAnmSound::animeLoop(Vec* param_1, f32 param_2, f32 param_3, u32 param_4,
                          u8 param_5)
{
	if (mData != nullptr)
		setAnimSoundVec(JAIBasic::basic, param_1, param_2, param_3, param_4,
		                param_5);
}

void MAnmSound::initAnmSound(void* param_1, u32 param_2, f32 param_3)
{
	initActorAnimSound(param_1, param_2, param_3);
}

void MAnmSound::setSpeedModifySound(JAISound* param_1,
                                    JAIAnimeFrameSoundData* param_2,
                                    f32 param_3)
{
	if (MSound::getSwitch(param_1->getUnk8(), 0x100000, 0x14))
		JAIAnimeSound::setSpeedModifySound(param_1, param_2, param_3);
}

// TODO: find a home for this
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

void MAnmSound::startAnimSound(void* param_1, u32 param_2, JAISound** param_3,
                               JAIActor* param_4, u8 param_5)
{
	if (MSGMSound->gateCheck(param_2)) {
		switch (get_thing(param_2)) {
		case 0:
			if ((param_4->unkC & 0x1000) == 0x1000)
				return;
			break;

		case 7: {
			u32 bVar2 = param_4->unkC >> 24;
			u32 a     = bVar2 & 0xF;
			u8 b      = bVar2 >> 4;
			MSGMSound->startMarioVoice(param_2, a, b);
			return;
		}
		}

		MSoundSESystem::MSoundSE::startSoundActorInner(param_2, param_3,
		                                               param_4, 0, param_5);
	}
}

void MAnmSoundNPC::startAnimSound(void* param_1, u32 param_2,
                                  JAISound** param_3, JAIActor* param_4,
                                  u8 param_5)
{
	if (MSGMSound->gateCheck(param_2)) {
		UnkStruct* ptr = mData;

		if (ptr[mDataCounter].unk18 & 0xFFFF0000) {
			if (ptr[mDataCounter].unk18 & 0xFF000000) {
				u32 uVar5 = mLoopCount;
				u32 uVar6 = (uVar5 >> 24) + 1;
				if (uVar5 != 0) {
					u32 uVar3 = uVar5 + (unk98 - (unk98 / uVar6) * uVar6);
					if (uVar3 - (uVar3 / uVar6) * uVar6 != 0)
						return;
				}
			}

			if (ptr[mDataCounter].unk18 & 0xFF0000) {
				u8 b = ptr[mDataCounter].unk18 >> 16;
				b += 1;
				b *= JAIConst::random.get_ufloat_1();
				if (b != 0)
					return;
			}
		}

		if (MSoundSESystem::MSoundSE::checkMonoSound(param_2, param_4)) {
			MSoundSESystem::MSoundSE::startSoundActorInner(param_2, param_3,
			                                               param_4, 0, param_5);

			if (*param_3 != nullptr && !(ptr[mDataCounter].unk18 & 0x8000)) {

				f32 dVar10 = 1.0f;

				const Vec* pfVar7 = param_4->unk4;
				f32 fVar11;
				if (MSGMSound->cameraLooksAtMario()) {
					const Vec* pVVar8 = MSGMSound->unkAC[0].unk0;
					fVar11
					    = std::sqrtf(std::powf(pfVar7->x - pVVar8->x, 2.0f)
					                 + std::powf(pfVar7->y - pVVar8->y, 2.0f)
					                 + std::powf(pfVar7->z - pVVar8->z, 2.0f));
				} else {
					fVar11 = 0.0f;
				}

				if (fVar11 != 0.0f)
					dVar10 = MSHandle::calcVolume(
					    fVar11, 2000.0f, 600.0f,
					    ptr[mDataCounter].unk18 >> 12 & 7, 8);

				(*param_3)->setSeInterVolume(0, dVar10, 0, 0);
			}
		}
	}
}
