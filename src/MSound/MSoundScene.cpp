#include <MSound/MSoundScene.hpp>
#include <MSound/MSound.hpp>
#include <macros.h>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

MSSceneSE::MSSceneSE(u32 param_1)
{
	for (int i = 0; i < ARRAY_COUNT(unk4); ++i)
		unk4[i] = nullptr;

	unk0 = 0;
}

void MSSceneSE::frameLoop(u32 param_1, Vec* param_2, u8 param_3)
{
	if (MSGMSound->gateCheck(param_1) && param_3 <= 0x100) {
		Vec* ptr = param_2;
		for (u8 i = 0; i < param_3; ++i) {
			unk4[i] = ptr;
			++ptr;
		}

		MtxPtr m = MSGMSound->unk8->unk8;
		for (u8 i = 0; i < 3; ++i) {
			unk404[i].x = 0.0;
			unk404[i].y = 0.0;
			unk404[i].z = 0.0;
			for (int j = 0; j < 3; ++j)
				unk428[i][j] = nullptr;
		}

		for (u8 i = 0; i < param_3; ++i) {
			Vec local_84;
			MTXMultVec(m, unk4[i], &local_84);

			f32 fVar15 = atan2f(local_84.x, local_84.z);
			f32 fVar1;
			if (fVar15 < -M_PI)
				fVar1 = -M_PI;
			else if (fVar15 > M_PI)
				fVar1 = M_PI;
			else
				fVar1 = fVar15;

			u8 iVar11;
			if (fVar1 >= -M_PI && fVar1 < -1.0470928f) {
				iVar11 = 0;
			} else if (fVar1 >= -1.0470928f && fVar1 < 1.0470928f) {
				iVar11 = 1;
			} else {
				iVar11 = 2;
			}

			sortMaxTrans(unk4[i], iVar11, 0);
		}

		for (u8 i = 0; i < 3; ++i) {
			u8 j;
			for (j = 0; j < 3; ++j) {
				if (unk428[i][j] == 0x0)
					break;
				unk404[i].x = unk404[i].x + unk428[i][j]->x;
				unk404[i].y = unk404[i].y + unk428[i][j]->y;
				unk404[i].z = unk404[i].z + unk428[i][j]->z;
			}

			if (j != 0) {
				unk404[i].x /= j;
				unk404[i].y /= j;
				unk404[i].z /= j;
			}
		}

		for (u8 i = 0; i < 3; ++i) {
			if (unk428[i][0] != nullptr) {
				if (!unk0) {
					if (MSGMSound->gateCheck(param_1 + i)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    param_1 + i, &unk404[i], 0, nullptr, 0, 4);
					}
				} else {
					MSoundSESystem::MSRandPlay::startSeRandPlay(param_1, i);
				}
			}
		}
	}
}

void MSSceneSE::sortMaxTrans(Vec* param_1, u8 param_2, u8 param_3)
{
	if (unk428[param_2][param_3] == nullptr) {
		unk428[param_2][param_3] = param_1;
		return;
	}

	if (MSGMSound->getDistFromCamera(unk428[param_2][param_3])
	    >= MSGMSound->getDistFromCamera(param_1)) {
		if (param_3 + 1 < 3)
			sortMaxTrans(unk428[param_2][param_3], param_2, param_3 + 1);
		unk428[param_2][param_3] = param_1;
	} else {
		if (param_3 + 1 < 3)
			sortMaxTrans(param_1, param_2, param_3 + 1);
	}
}

void MSSceneSE::calcPosPanSR(Vec* param_1, f32 param_2) { }

void MSSceneSE::calcPosPanLR(Vec* param_1, f32 param_2) { }

void MSSceneSE::calcPosVolume(Vec* param_1, f32* param_2, u8 param_3) { }
