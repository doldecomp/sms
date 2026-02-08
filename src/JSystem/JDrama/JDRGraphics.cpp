#include <JSystem/JDrama/JDRGraphics.hpp>
#include <dolphin/vi.h>
#include <dolphin/gx.h>

using namespace JDrama;

void TGraphics::setViewport(const TRect& param_1, float param_2, float param_3)
{
	mViewportRect = param_1;

	if (mRenderMode.field_rendering) {
		GXSetViewportJitter(mViewportRect.x1, mViewportRect.y1,
		                    mViewportRect.getWidth(), mViewportRect.getHeight(),
		                    param_2, param_3, VIGetNextField());
	} else {
		GXSetViewport(mViewportRect.x1, mViewportRect.y1,
		              mViewportRect.getWidth(), mViewportRect.getHeight(),
		              param_2, param_3);
	}
}
