#include <System/SnapTimeObj.hpp>
#include <System/TimeRec.hpp>

// TODO: one stack slot away from exact. The `JUtility::TColor` temporary that
// the inlined `TTimeRec::startTimer(u32)` stores and reloads sits at 0x34; the
// ROM puts it at 0x38, with the same 0x50 frame and identical instructions, so
// four more bytes of inline-expansion temporaries belong below it. All levers
// are in the shared `include/System/TimeRec.hpp`, so they are reported rather
// than applied. Measured there: `endTimer()` is the only helper that
// contributes temporaries (12 bytes; dropping it moves the colour to 0x28),
// and its `instance()` fetch is 8 of them (`_instance` instead gives frame 0x48
// / colour 0x30). Worth zero: `snapGxTimeEnd()` over `snapGxTimeStatic(0)`,
// `instance()` inside `snapGxTimeStatic`, an extra `getInstance()` forwarder in
// `startTimer`, and any declaration order inside `startTimer`'s body. Worse: an
// extra level inside `crTimeAry()` (frame 0x58, colour 0x3c), a second
// `instance()` in `endTimer`, a `TColor` temporary or an unnamed `OSGetTick()`
// in `endTimer`, and a `TColor` argument at the `append` call.
void TSnapTimeObj::perform(u32 cue, JDrama::TGraphics*)
{
	if ((unk14 & 1)) {
		if ((cue & CUE_DRAW_INIT) != 0) {
			TTimeRec::snapGxTimeStatic(0);
			TTimeRec::endTimer();
		}
		if ((cue & CUE_DRAW) != 0) {
			TTimeRec::startTimer(unk10);
			TTimeRec::snapGxTimeStatic(unk10);
		}
	}
}
