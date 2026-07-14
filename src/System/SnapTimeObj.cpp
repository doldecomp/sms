#include <System/SnapTimeObj.hpp>
#include <System/TimeRec.hpp>

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
