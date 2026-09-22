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

// TODO: 96.8%. Two residues, and the first is shared with sortMaxTrans:
// retail addresses the `getDistFromCamera(mMaxTrans[direction][rank])`
// argument as an *independently re-derived* indexed load while every other
// access to the same slot goes through one materialised address. In the
// out-of-line sortMaxTrans that is `lwzx r4, r25, r28` (row base + rank * 4)
// against our `lwz r4, 0(r26)`, and it is why retail keeps both `rank` (r27)
// and `rank << 2` (r28) in callee-saved registers where we let the scaled
// index die in r0. In frameLoop's inlined expansion it is the same shape with
// `rank` = 0 left unfolded (`li r0, 0; slwi r4, r0, 2; addi r24, r4, 0x428;
// add r24, r25, r24`), and it accounts for the whole register difference:
// retail saves r22-r31, we save r24-r31, because retail needs one register for
// the re-derived address and one more for the row base it no longer folds the
// 0x428 into.
// Measured, all worse than the plain subscript at every site (closure batch
// 120; sortMaxTrans 99.3% / 117 instructions is the best spelling found):
//   - `Vec** maxTrans = &mMaxTrans[direction][rank];` for the test, the store
//     and the recursion with the full subscript at the distance argument:
//     91.9%, 122 instructions, frame 0x40 -> 0x48;
//   - the same slot pointer at every site: 95.0%, 120 instructions;
//   - `Vec** maxTrans = mMaxTrans[direction];` (row pointer) mixed 84.5% /
//     everywhere 86.5%, both 121-122 instructions;
//   - a `getMaxTrans(u8, u8)` in-class accessor at the distance argument:
//     96.0% (the accessor's *value* CSEs into a callee-saved register and the
//     reload disappears entirely) -- but it is +8 of frameLoop's frame, which
//     is the only lever measured that moves frameLoop the right way (0x90 ->
//     0x98 against retail's 0xc0);
//   - a named `Vec* maxTrans` after the null test: 93.4%.
// Second residue: frameLoop's frame is 0xc0 against our 0x90. 8 of the 48
// bytes are the two extra callee-saved registers above; of the rest, retail
// has ~16 bytes of named locals declared *before* `listenerTrans` (which sits
// at 0x3c against our 0x20, with the u8 -> f32 magic-double temporaries above
// it at 0x58-0x6f against our 0x30-0x47) and ~24 more bytes of
// inline-expansion pool. The three UNUSED helpers below (calcPosVolume 0x134,
// calcPosPanLR 0x124, calcPosPanSR 0xf0, all still empty stubs) are the only
// candidates in the TU for locals that frameLoop declared and never read.
// cc28: `(int)rank` at the null test and the first store (only those two)
// puts `rank` in retail's r27 and leaves only the missing `lwzx` (99.4%); a
// `(int)rank` at the distance argument does produce retail's `lwzx r4, row,
// rank*4` but also in the depth-1 copy, where retail has none, and costs +8
// of frame -- so the re-derived index is real but reached some other way.
// Inert on both functions: a named `trans` distance, `!ptr`, `(u8)(rank + 1)`,
// row-pointer / value / reference accessors at the distance argument, and
// C-style top declarations of listenerTrans/angle/clampedAngle/direction in
// frameLoop. A TU-local `DistOf(Vec*)` wrapper over getDistFromCamera is +8 on
// frameLoop (0x98) but +8 on sortMaxTrans too.
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

// The three helpers below are dead code, not inlined bodies: nothing in the
// map's closure references them, and the target's frameLoop is 0x418 -- the
// same byte count as ours -- so there is no expanded copy hiding in it either.
// They are still compiled, so their literal requests shape the TU's .sdata2
// pool, and that pool is the only evidence about their contents beyond the
// map's sizes. Literals are allocated in code-generation order, which for
// `-inline deferred` is reverse source order, so these three run *before*
// sortMaxTrans and frameLoop and take the low ids:
//
//   @2051  0.0f        kept (frameLoop also wants it)
//   @2052  4 bytes     UNUSED -- only these helpers want it
//   @2053  3.1415927f  kept (frameLoop's upper clamp)
//   @2054  4 bytes     UNUSED
//   @2088  4 bytes     UNUSED
//   @2114  8 bytes     UNUSED (a double: an int->float magic or a real
//   @2115  8 bytes     UNUSED  double constant)
//   @2263  -3.1415927f, @2264 -1.0470928f, @2265 1.0470928f, @2267 the
//          u32->f64 magic -- all first requested by frameLoop
//
// So, reading the emission order calcPosPanSR, calcPosPanLR, calcPosVolume
// against the id clusters: the first of the three requests four 4-byte
// literals in the order 0.0f, ?, +pi, ?; a later one requests a single 4-byte
// literal; and the last requests two 8-byte doubles. Sizes to hit are 0xf0
// (60 instructions), 0x124 (73) and 0x134 (77).
//
// This also explains the one pool difference left in the unit: retail lays out
// +pi (@2053) before -pi (@2263) because +pi was already allocated by a dead
// helper, while our frameLoop -- which asks for -pi first, exactly as retail's
// does -- allocates -pi first because nothing asked earlier. Our clamp order
// is therefore right; the swap is entirely the missing helper bodies, and it
// would have to be fixed before this unit could be linked from source.
//
// TODO: bodies unreconstructed. The literal *values* of @2052/@2054/@2088 and
// the two doubles were dead-stripped and are not in the image, so anything
// written here would be invention; four literal slots and a byte size are not
// enough to pin 60-77 instructions.
void MSSceneSE::calcPosVolume(Vec* param_1, f32* param_2, u8 param_3) { }

void MSSceneSE::calcPosPanLR(Vec* param_1, f32 param_2) { }

void MSSceneSE::calcPosPanSR(Vec* param_1, f32 param_2) { }
