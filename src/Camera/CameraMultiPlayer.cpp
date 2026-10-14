#include <Camera/CameraMultiPlayer.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>

TCameraMultiPlayer::TCameraMultiPlayer(u8 max_player_count)
    : mMaxPlayers(max_player_count)
    , mPlayerCount(0)
    , mPlayers(nullptr)
{
	mPlayers = new TMultiPlayerData[max_player_count];
}

bool TCameraMultiPlayer::addPlayer(const JGeometry::TVec3<f32>* param_1,
                                   f32 param_2, f32 param_3)
{
	bool added;
	if (mPlayerCount >= mMaxPlayers)
		return false;

	TMultiPlayerData* data = &mPlayers[mPlayerCount];
	data->unk0             = param_1;
	data->unk4             = param_2;
	data->unk8             = param_3;
	mPlayerCount += 1;

	return true;
}

// TODO: shouldn't be marked as inline
inline bool
TCameraMultiPlayer::removePlayer(const JGeometry::TVec3<f32>* param_1)
{
	bool found = false;
	int i;
	TMultiPlayerData* it = mPlayers;
	for (i = 0; i < mPlayerCount; ++i, ++it) {
		if (found == true || (found == false && it->unk0 == param_1)) {
			if (i != mPlayerCount - 1)
				*it = *(it + 1);
			found = true;
		}
	}

	if (found != true)
		return found;

	mPlayerCount -= 1;
	return found;
}

void CPolarSubCamera::createMultiPlayer(u8 param_1)
{
	if (!unk2BC)
		unk2BC = new TCameraMultiPlayer(param_1);
}

bool CPolarSubCamera::addMultiPlayer(const JGeometry::TVec3<f32>* param_1,
                                     f32 param_2, f32 param_3)
{
	bool result;
	if (!unk2BC)
		result = false;
	else
		result = unk2BC->addPlayer(param_1, param_2, param_3);

	return result;
}

bool CPolarSubCamera::removeMultiPlayer(const JGeometry::TVec3<f32>* param_1)
{
	if (!unk2BC)
		return false;

	return unk2BC->removePlayer(param_1);
}

void CPolarSubCamera::ctrlMultiPlayerCamera_()
{
	int count = unk2BC->mPlayerCount;
	if (count <= 0) {
		mCurrentTarget.mTarget.set(unk148);
		mCurrentTarget.unk18.set(unk124);
	} else {
		JGeometry::TVec3<f32> center(0.0f, 0.0f, 0.0f);

		{
			TMultiPlayerData* it = unk2BC->mPlayers;
			for (int i = 0; i < count; ++i, ++it)
				center += *it->unk0;
			center *= 1.0f / (f32)count;
		}

		center.y += mCurrentParams->mAtOffsetY;

		f32 maxSqDist = 0.0f;
		{
			int i;
			int j;
			TMultiPlayerData* it;
			TMultiPlayerData* jt;
			it = unk2BC->mPlayers;
			for (i = 0; i < count - 1; ++i, ++it) {
				jt = it + 1;
				for (j = i + 1; j < count; ++j, ++jt) {
					JGeometry::TVec3<f32> diff;
					diff.sub(*it->unk0, *jt->unk0);
					f32 x2 = diff.x * diff.x;
					f32 y2 = diff.y * diff.y;
					f32 z2 = diff.z * diff.z;
					f32 sq = x2 + y2 + z2;
					if (sq > maxSqDist)
						maxSqDist = sq;
				}
			}
		}

		f32 camDistance = 1.5f * MsSqrtf(maxSqDist) + 300.0f;
		camDistance     = MsClamp(camDistance, mCurrentParams->mDistMin,
		                          mCurrentParams->mDistMax);

		f32 ratio = CLBCalcRatio<f32>(mCurrentParams->mDistMin,
		                              mCurrentParams->mDistMax, camDistance);

		s16 camPitch = CLBLinearInbetween<s16>(
		    mCurrentParams->mXAngleMin, mCurrentParams->mXAngleMax, ratio);

		mCurrentTarget.mTarget.set(center);
		CLBPolarToCross(center, &mCurrentTarget.unk18, camDistance, camPitch,
		                0);
	}

	calcPosAndAt_();
}
