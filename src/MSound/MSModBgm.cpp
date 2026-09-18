#include <MSound/MSModBgm.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

JAISound* MSModBgm::modBgm(u8 param_1, u8 param_2)
{
	switch (param_1) {
	case 0:
	case 1:
		unk0 = true;
		break;

	default:
		break;
	}

	// TODO: 96.5%. Retail shares one zero register between the two
	// statements of each reset: `li r31, 0` (the value that is both
	// `sound` and the return value) is also the source of the `stb` into
	// unk0, twice. We materialise 0 into r0 for the byte store and into
	// r31 for the pointer, so we are two instructions long. Measured with
	// no effect: swapping the two assignments (both sites), `unk0 = 0`,
	// `unk0 = sound` after the null assignment, `bool`/`u8`/integer-zero
	// spellings, and an early `return nullptr`. MWCC always sinks the
	// pointer assignment below the member store, so the fix has to make
	// the store's source depend on `sound`.
	JAISound* sound = MSBgm::getHandle(param_2);
	if (!sound) {
		sound = nullptr;
		unk0  = false;
	} else {
		switch (unk4) {
		case 0:
			sound->setTempoProportion(1.3f, 10);
			sound->setPitch(1.3f, 10, 0);
			break;

		case 5:
			sound->setTempoProportion(0.3f, 180);
			sound->setPitch(0.2f, 180, 0);
			break;

		case 180:
			sound->stop(1);
			sound = nullptr;
			unk0  = false;
			break;
		}
	}
	return sound;
}

void MSModBgm::loop()
{
	switch (unk0) {
	case true:
		unk4 += 1;
		break;

	case false:
	default:
		unk4 = 0;
		return;
	}
	unk0 = 0;
}

void MSModBgm::changeTempo(u8 param_1, u8 param_2)
{
	JAISound* sound = MSBgm::getHandle(param_2);
	if (sound) {
		f32 fVar1 = 1.0f;
		u32 uVar2 = 5;
		switch (param_1) {
		case 0:
			fVar1 = 1.07894f;
			break;

		case 1:
			fVar1 = 1.15789f;
			break;

		case 2:
			fVar1 = 1.2f;
			uVar2 = 20;
			break;

		case 3:
			fVar1 = 1.0f;
			break;
		}

		sound->setTempoProportion(fVar1, uVar2);
	}
}

f32 MSBgmXFade::scTiming[18] = {
	0.052632f,   0.105263f,   0.157895f, 0.210526f,   0.26315799f, 0.315789f,
	0.36842099f, 0.42105299f, 0.473684f, 0.52631599f, 0.578947f,   0.63157898f,
	0.684211f,   0.73684198f, 0.789474f, 0.84210497f, 0.894737f,   0.947368f,
};
f32 MSBgmXFade::scExp[18] = {
	0.0f,      0.0f,      0.030207f,   0.063591f, 0.100485f, 0.14126f,
	0.186324f, 0.236126f, 0.29116699f, 0.351996f, 0.419223f, 0.49351999f,
	0.57563f,  0.666377f, 0.766667f,   0.877505f, 1.0f,      1.0f,
};

void MSBgmXFade::xFadeBgm(f32 param_1)
{
	// TODO: 99.4%. Two independent residues, both localised:
	//  * frame 0x30 vs 0x38. No slot is referenced, so this is the dead
	//    low region: an uninitialised **non-trivial 8-byte local of
	//    getTiming** (the only inlined callee) takes the frame to 0x38
	//    with no instruction change, and getTiming is UNUSED so its map
	//    size is unaffected. Nothing in MSBgmXFade names such an object,
	//    so it is left out rather than fabricated. Caller-side levers
	//    measured: `u32 timing; getTiming(param_1, &timing)` +0 (a scalar
	//    written by an inlined callee is dropped), named `f32` volumes
	//    regress (+4 instructions).
	//  * f0/f1 swap inside the inlined getTiming: retail parks `unk0` in
	//    f1 (the vacated float-parameter register) and the per-iteration
	//    `scTiming[i]` in f0; we do the opposite, which flips all four
	//    `fcmpo` operand pairs. Naming the table element, reading `unk0`
	//    raw in the conditions and every operand-order permutation of the
	//    two comparisons were measured: none moves the allocation.
	//    Probably a longer live range for `unk0` in the real body (see
	//    getTiming's 0x60-vs-0x94 size gap below).
	// Closure re-pass (batch 161): under the UNUSED-callee carrier rule
	// both carriers are *legal* -- `getTiming` and `getTimingForce` are
	// UNUSED, so neither has an out-of-line copy that a dead local could
	// break, and a dead uninitialised local costs no instructions and no
	// map size.  What is still missing is only a candidate object: a
	// non-trivial class local of 4 or 8 bytes buys +8 either way, and
	// nothing in a BGM crossfade timing search names one.  The most likely
	// source of it is `getTiming`'s missing `param_2` output (0x60 against
	// the map's 0x94), so reconstructing that body is the way in, not a
	// lever hunt.
	u8 tmp = getTiming(param_1, nullptr);

	bool b = tmp >= 1 && tmp <= 16;
	if (b) {
		MSBgm::setTrackVolume(0, scExp[17 - tmp], 2, 0);
		MSBgm::setTrackVolume(1, scExp[tmp], 2, 0);
	}
	unk0 = param_1;
}

void MSBgmXFade::xFadeBgmForce(f32 param_1)
{
	u8 tmp = getTimingForce(param_1);
	if (tmp != 0xff) {
		MSBgm::setTrackVolume(0, scExp[17 - tmp], 0, 0);
		MSBgm::setTrackVolume(1, scExp[tmp], 0, 0);
		unk0 = param_1;
	}
}

// TODO: map size 0x94, ours 0x60: thirteen instructions of the optional
// `param_2` output are missing. It is invisible at the only call site
// (xFadeBgm passes nullptr, so the stores fold away), so there is no
// evidence for its contents; spelling it as two null-checked stores also
// pushes the body out of line, which the UNUSED marker forbids.
u8 MSBgmXFade::getTiming(f32 param_1, u32* param_2)
{
	f32 f1 = unk0;
	for (u8 i = 0; i < 18; ++i) {
		if (param_1 > scTiming[i] && f1 <= scTiming[i])
			return i;
		if (param_1 < scTiming[i] && scTiming[i] >= f1)
			return i;
	}

	return 0xff;
}

// TODO: an uninitialised non-trivial **4-byte** local here takes
// MSBgmXFade::xFadeBgmForce from 99.8% to exact (frame 0x28 -> 0x30) with
// no instruction change and no size change to this UNUSED body. Left out
// for want of a candidate object; same family as getTiming's 8 bytes.
u8 MSBgmXFade::getTimingForce(f32 param_1)
{
	for (u8 i = 0; i < 17; ++i)
		if (param_1 >= scTiming[i] && param_1 < scTiming[i + 1])
			return i;

	return 0xff;
}
