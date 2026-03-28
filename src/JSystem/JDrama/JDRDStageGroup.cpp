#include <JSystem/JDrama/JDRDStageGroup.hpp>

using namespace JDrama;

void TDStageGroup::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 8))
		return;

	unk20.perform(param_1, param_2);
	TViewObjPtrListT::perform(param_1, param_2);
	unk20.perform(param_1 & ~0x8 | 0x100, param_2);
}
