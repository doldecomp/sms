#include <System/SnapTimeObj.hpp>
#include <System/TimeRec.hpp>

void TSnapTimeObj::perform(u32 cue, JDrama::TGraphics*)
{
	if ((unk14 & 1)) {
		if ((cue & CUE_DRAW_INIT) != 0) {
			TTimeRec::snapGXTimeSt(0);
			TTimeRec::snapCPUTime(0);
		}
		if ((cue & CUE_DRAW) != 0) {
			TTimeRec::snapCPUTime(JUtility::TColor(unk10));
			TTimeRec::snapGXTimeSt(unk10);
		}
	}
}
