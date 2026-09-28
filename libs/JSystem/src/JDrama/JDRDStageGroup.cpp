#include <JSystem/JDrama/JDRDStageGroup.hpp>

using namespace JDrama;

void TDStageGroup::perform(u32 cue, TGraphics* graphics)
{
	if (!(cue & CUE_DRAW))
		return;

	unk20.perform(cue, graphics);
	TViewObjPtrListT::perform(cue, graphics);
	unk20.perform(cue & ~CUE_DRAW | CUE_DRAW_STAGE_END, graphics);
}
