#include <System/SnapTimeObj.hpp>
#include <System/TimeRec.hpp>

// The DRAW_INIT block's `endTimer()` sits one inline level below this call
// site: adding a single forwarder above it moves the `JUtility::TColor`
// temporary that the inlined `TTimeRec::startTimer(u32)` stores and reloads
// from 0x34 to the ROM's 0x38 with the frame unchanged at 0x50, which is
// exactly the (+4 slot, 0 frame) lever header round 18 was missing. The level
// belongs in `include/System/TimeRec.hpp` -- most plausibly `endTimer()` (or
// the whole snap-and-end pair) forwarding to a second static -- but that
// header is shared with the source-linked `TLiveManager::perform`,
// `TObjManager::perform` and `TEnemyManager::perform`, so it is parked here
// under a TU-prefixed name and reported instead.
//
// Measured at this call site (colour slot, frame 0x50 throughout):
//   one level above `endTimer()`                      0x38  <- exact
//   the same level wrapping the whole DRAW_INIT block  0x38  <- also exact
//   one level above `startTimer(unk10)`, `(u32)` arg   0x2c
//   the same level taking `this`                       0x30
//   one level above `snapGxTimeStatic(0)`, one or two  0x34 (inert)
//   the DRAW block as one two-call wrapper             0x34, +1 instruction
//   a named `u32` for `unk10`, an early return, a
//     named `u16` for `unk14`, a copy of `cue`         0x34 (inert or worse)
//   a parked `static inline` taking `this` for `unk10` 0x34, 11 diffs
// Header-side history (all inside TimeRec.hpp, all rejected there): the trial
// table at `startTimer(u32)`'s declaration, plus round 18's compensating pair
// (the named `timeArray` in `endTimer` cancelled by removing it from the
// four-argument `startTimer`), which lands the slot but leaves +8 of frame.
static inline void SnapTimeObjEndTimer() { TTimeRec::endTimer(); }

void TSnapTimeObj::perform(u32 cue, JDrama::TGraphics*)
{
	if ((unk14 & 1)) {
		if ((cue & CUE_DRAW_INIT) != 0) {
			TTimeRec::snapGxTimeStatic(0);
			SnapTimeObjEndTimer();
		}
		if ((cue & CUE_DRAW) != 0) {
			TTimeRec::startTimer(unk10);
			TTimeRec::snapGxTimeStatic(unk10);
		}
	}
}
