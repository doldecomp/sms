#include <GC2D/Menu.hpp>
#include <JSystem/J2D/J2DOrthoGraph.hpp>
#include <JSystem/J2D/J2DScreen.hpp>
#include <dolphin/gx.h>

void TMenuBase::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 0x8) {
		J2DOrthoGraph orthoGraph(param_2->getUnk54());
		orthoGraph.setup2D();
		unk10->draw(0, 0, &orthoGraph);
		const JUTRect& rect = param_2->getUnk64();
		GXSetScissor(rect.x1, rect.y1, rect.getWidth(), rect.getHeight());
	}
}

TMenuPlane::TMenuPlane(const TMarioGamePad*, J2DPane*, u32, u32)
    : JDrama::TViewObj("<TMenuPlane>")
{
}

void TMenuPlane::perform(u32, JDrama::TGraphics*) { }

void TFlashPane::perform(u32, JDrama::TGraphics*) { }
