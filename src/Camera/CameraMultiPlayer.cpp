#include <Camera/CameraMultiPlayer.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CameraKindParam.hpp>
#include <Camera/cameralib.hpp>
#include <JSystem/JGeometry/JGUtil.hpp>

TCameraMultiPlayer::TCameraMultiPlayer(u8 param_1)
    : unk0(param_1)
    , unk1(0)
    , unk4(nullptr)
{
	unk4 = new TMultiPlayerData[param_1];
}

bool TCameraMultiPlayer::addPlayer(const JGeometry::TVec3<f32>* param_1,
                                   f32 param_2, f32 param_3)
{
	bool added;
	if (unk1 >= unk0)
		return false;

	TMultiPlayerData* data = &unk4[unk1];
	data->unk0             = param_1;
	data->unk4             = param_2;
	data->unk8             = param_3;
	unk1 += 1;

	return true;
}

// TODO: shouldn't be marked as inline
inline bool
TCameraMultiPlayer::removePlayer(const JGeometry::TVec3<f32>* param_1)
{
	bool found = false;
	int i;
	TMultiPlayerData* it = unk4;
	for (i = 0; i < unk1; ++i, ++it) {
		if (found == true || (found == false && it->unk0 == param_1)) {
			if (i != unk1 - 1)
				*it = *(it + 1);
			found = true;
		}
	}

	if (found != true)
		return found;

	unk1 -= 1;
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
	if (!unk2BC)
		return false;

	return unk2BC->addPlayer(param_1, param_2, param_3);
}

bool CPolarSubCamera::removeMultiPlayer(const JGeometry::TVec3<f32>* param_1)
{
	if (!unk2BC)
		return false;

	return unk2BC->removePlayer(param_1);
}

void CPolarSubCamera::ctrlMultiPlayerCamera_()
{
	int count = unk2BC->unk1;
	if (count <= 0) {
		unk80.unkC.set(unk148);
		unk80.unk18.set(unk124);
	} else {
		JGeometry::TVec3<f32> sum(0.0f, 0.0f, 0.0f);
		TMultiPlayerData* data = unk2BC->unk4;

		TMultiPlayerData* it = data;
		for (int i = 0; i < count; ++i, ++it)
			sum += *it->unk0;

		sum *= 1.0f / (f32)count;

		sum.y += unk68->mAtOffsetY;

		f32 maxSqDist       = 0.0f;
		TMultiPlayerData* a = data;
		for (int i = 0; i < count - 1; ++i, ++a) {
			TMultiPlayerData* b = a + 1;
			for (int j = i + 1; j < count; ++j, ++b) {
				f32 sq = a->unk0->squared(*b->unk0);
				if (sq > maxSqDist)
					maxSqDist = sq;
			}
		}

		f32 radius = MsClamp(1.5f * MsSqrtf(maxSqDist) + 300.0f,
		                     unk68->mDistMin, unk68->mDistMax);

		s16 angle = CLBLinearInbetween<s16>(
		    unk68->mXAngleMin, unk68->mXAngleMax,
		    CLBCalcRatio<f32>(unk68->mDistMin, unk68->mDistMax, radius));

		unk80.unkC.set(sum);
		CLBPolarToCross(sum, &unk80.unk18, radius, angle, 0);
	}

	calcPosAndAt_();
}
