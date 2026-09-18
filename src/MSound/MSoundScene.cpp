#include <MSound/MSoundScene.hpp>
#include <MSound/MSound.hpp>
#include <macros.h>
#include <math.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

// Only the map's size (0xc00) and the leading .bss position are evidence: the
// final binary never reads it, and __sinit_MSoundScene_cpp only uses it as the
// base register for the chain nodes that follow. 256 matches MSSceneSE's
// mTrans[256], so this is the scratch position buffer those pointers index.
// TODO: dimensions unconfirmed.
Vec _posByCamera[256];

MSSceneSE::MSSceneSE(u32 param_1)
{
	for (int i = 0; i < ARRAY_COUNT(mTrans); ++i)
		mTrans[i] = nullptr;

	mUseRandPlay = 0;
}

void MSSceneSE::frameLoop(u32 sound_id, Vec* trans, u8 trans_num)
{
	if (MSGMSound->gateCheck(sound_id) && trans_num <= ARRAY_COUNT(mTrans)) {
		Vec* ptr = trans;
		for (u8 i = 0; i < trans_num; ++i) {
			mTrans[i] = ptr;
			++ptr;
		}

		MtxPtr listenerMtx = MSGMSound->mAudioCameras->nViewMtx;
		for (u8 i = 0; i < DIRECTION_NUM; ++i) {
			mAvgTrans[i].x = 0.0;
			mAvgTrans[i].y = 0.0;
			mAvgTrans[i].z = 0.0;
			for (int j = 0; j < MAX_TRANS_NUM; ++j)
				mMaxTrans[i][j] = nullptr;
		}

		for (u8 i = 0; i < trans_num; ++i) {
			Vec listenerTrans;
			MTXMultVec(listenerMtx, mTrans[i], &listenerTrans);

			f32 angle = atan2f(listenerTrans.x, listenerTrans.z);
			f32 clampedAngle;
			if (angle < -M_PI)
				clampedAngle = -M_PI;
			else if (angle > M_PI)
				clampedAngle = M_PI;
			else
				clampedAngle = angle;

			u8 direction;
			if (clampedAngle >= -M_PI && clampedAngle < -1.0470928f) {
				direction = DIRECTION_L;
			} else if (clampedAngle >= -1.0470928f
			           && clampedAngle < 1.0470928f) {
				direction = DIRECTION_R;
			} else {
				direction = DIRECTION_SR;
			}

			sortMaxTrans(mTrans[i], direction, 0);
		}

		for (u8 i = 0; i < DIRECTION_NUM; ++i) {
			u8 num;
			for (num = 0; num < MAX_TRANS_NUM; ++num) {
				if (mMaxTrans[i][num] == nullptr)
					break;
				mAvgTrans[i].x = mAvgTrans[i].x + mMaxTrans[i][num]->x;
				mAvgTrans[i].y = mAvgTrans[i].y + mMaxTrans[i][num]->y;
				mAvgTrans[i].z = mAvgTrans[i].z + mMaxTrans[i][num]->z;
			}

			if (num != 0) {
				mAvgTrans[i].x /= num;
				mAvgTrans[i].y /= num;
				mAvgTrans[i].z /= num;
			}
		}

		for (u8 i = 0; i < DIRECTION_NUM; ++i) {
			if (mMaxTrans[i][0] != nullptr) {
				if (!mUseRandPlay) {
					u32 id = sound_id + i;
					if (MSGMSound->gateCheck(id)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    id, &mAvgTrans[i], 0, nullptr, 0, 4);
					}
				} else {
					MSoundSESystem::MSRandPlay::startSeRandPlay(sound_id, i);
				}
			}
		}
	}
}

void MSSceneSE::sortMaxTrans(Vec* trans, u8 direction, u8 rank)
{
	if (mMaxTrans[direction][rank] == nullptr) {
		mMaxTrans[direction][rank] = trans;
		return;
	}

	if (MSGMSound->getDistFromCamera(mMaxTrans[direction][rank])
	    >= MSGMSound->getDistFromCamera(trans)) {
		if (rank + 1 < MAX_TRANS_NUM)
			sortMaxTrans(mMaxTrans[direction][rank], direction, rank + 1);
		mMaxTrans[direction][rank] = trans;
	} else {
		if (rank + 1 < MAX_TRANS_NUM)
			sortMaxTrans(trans, direction, rank + 1);
	}
}

void MSSceneSE::calcPosVolume(Vec* param_1, f32* param_2, u8 param_3) { }

void MSSceneSE::calcPosPanLR(Vec* param_1, f32 param_2) { }

void MSSceneSE::calcPosPanSR(Vec* param_1, f32 param_2) { }
