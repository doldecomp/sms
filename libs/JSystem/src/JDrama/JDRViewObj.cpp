#include <JSystem/JDrama/JDRViewObj.hpp>

void JDrama::TViewObj::testPerform(u32 cue, JDrama::TGraphics* graphics)
{
	if (((cue & CUE_MOVEMENT_GATE_A) != 0) && unkC.check(CUE_MOVEMENT_GATE_A)) {
		cue = cue & ~CUE_MOVE;
	}
	if (((cue & CUE_MOVEMENT_GATE_B) != 0) && unkC.check(CUE_MOVEMENT_GATE_B)) {
		cue = cue & ~CUE_MOVE;
	}
	cue &= ~unkC.get();
	if (cue) {
		perform(cue, graphics);
	}
}
