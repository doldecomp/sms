#include <System/SnapTimeObj.hpp>
#include <System/TimeRec.hpp>

void TSnapTimeObj::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if ((unk14 & 1)) {
		if ((param_1 & 0x80) != 0) {
			TTimeRec::snapGxTimeStatic(0);
			TTimeRec::endTimer();
		}
		if ((param_1 & 8) != 0) {
			TTimeRec::startTimer(unk10);
			TTimeRec::snapGxTimeStatic(unk10);
		}
	}
}
