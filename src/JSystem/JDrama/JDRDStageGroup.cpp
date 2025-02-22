#include <JSystem/JDrama/JDRDStageGroup.hpp>

using namespace JDrama;

#pragma dont_inline on
void TDStageGroup::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 8))
		return;

	TViewObjPtrListT::perform(param_1, param_2);
}
