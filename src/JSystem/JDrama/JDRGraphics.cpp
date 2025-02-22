#include <JSystem/JDrama/JDRGraphics.hpp>
#include <dolphin/vi.h>
#include <dolphin/gx.h>

using namespace JDrama;

void TGraphics::setViewport(const TRect& param_1, float param_2, float param_3)
{
	unk54 = param_1;

	if (unk8.field_rendering) {
		GXSetViewportJitter(unk54.x1, unk54.y1, unk54.getWidth(),
		                    unk54.getHeight(), param_2, param_3,
		                    VIGetNextField());
	} else {
		GXSetViewport(unk54.x1, unk54.y1, unk54.getWidth(), unk54.getHeight(),
		              param_2, param_3);
	}
}
