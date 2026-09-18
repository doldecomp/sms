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

// TODO: UNUSED 0x48 in the map, ours 0x44 -- one instruction short.  The named
// `added` result, a positive `mPlayerCount < mMaxPlayers` guard, `mPlayers +
// mPlayerCount` and `mPlayerCount = mPlayerCount + 1` are all codegen-neutral
// here.  A named `int index = mPlayerCount` with `mPlayerCount = index + 1`
// goes the wrong way (0x40) and costs addMultiPlayer its match, so the extra
// instruction is not an index temporary.
bool TCameraMultiPlayer::addPlayer(const JGeometry::TVec3<f32>* param_1,
                                   f32 param_2, f32 param_3)
{
	bool added = false;
	if (mPlayerCount >= mMaxPlayers)
		return added;

	TMultiPlayerData* data = &mPlayers[mPlayerCount];
	data->unk0             = param_1;
	data->unk4             = param_2;
	data->unk8             = param_3;
	mPlayerCount += 1;
	added = true;

	return added;
}

// UNUSED 0x88 in the map, and now size-exact as a plain method that MWCC both
// emits and expands into CPolarSubCamera::removeMultiPlayer byte for byte.
// The lever was the tail: the earlier `if (found != true) return found;
// mPlayerCount -= 1; return found;` is one statement and two instructions more
// (0x90), and that one statement puts the body over MWCC's 14-statement
// depth-1 budget, so `inline` was needed to keep the caller (which then left
// the symbol undefined).  `if (found == true) mPlayerCount -= 1;` is two
// statements and still emits retail's `clrlwi; cmplwi 1; bnelr` in the
// expansion; a plain `if (found)` emits `clrlwi.; beqlr` instead and is 0x84.
// `int i;` as its own declaration does not count toward the budget and is what
// gives retail's r6/r5 split between the index and the cursor.
bool
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

	if (found == true)
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

// The inner loop of ctrlMultiPlayerCamera_ goes through this helper rather than
// spelling the subtraction out: the extra inline level is what gives retail's
// register allocation there (x's difference in f4 and y's in f3, ours had them
// swapped).  Written per component because `TVec3 diff; diff.sub(a, b);` as a
// local of an *inlined* callee is given a stack home and read back (seven
// instructions retail does not have) where the same two statements in the
// caller's own body are scalar-replaced.  The squares are named so the
// products stay three `fmuls` (fp_contract fuses products of locals).
//
// TODO: with a dead `JGeometry::TVec3<f32> diff;` declared here the caller's
// frame is 0x60 exactly, with no instruction change -- so retail's 28 bytes of
// dead low region below the MsSqrtf slot are this helper's reserved locals
// (MWCC reserves an inlined callee's slots even when its values live in
// registers).  Left out because an unused local is not evidence on its own;
// the real body is presumably the copy-and-subtract form above in a spelling
// that our compiler scalar-replaces.
static inline f32 sqDistance(const JGeometry::TVec3<f32>& a,
                             const JGeometry::TVec3<f32>& b)
{
	f32 x2 = (a.x - b.x) * (a.x - b.x);
	f32 y2 = (a.y - b.y) * (a.y - b.y);
	f32 z2 = (a.z - b.z) * (a.z - b.z);
	return x2 + y2 + z2;
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
					f32 sq = sqDistance(*it->unk0, *jt->unk0);
					if (sq > maxSqDist)
						maxSqDist = sq;
				}
			}
		}

		// TODO: retail's fmadds writes camDistance straight into f31, the
		// register MsClamp then clamps in place; ours computes it in f0 and
		// copies (one extra `fmr f31, f0`).  A single-expression
		// `MsClamp(1.5f * MsSqrtf(...) + 300.0f, min, max)` moves the copy
		// rather than removing it (the clamp then runs in f0), and the
		// if/else-if spelling reloads mDistMin in both arms (+2).
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
