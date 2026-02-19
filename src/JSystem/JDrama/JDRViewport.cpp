#include <JSystem/JDrama/JDRViewport.hpp>
#include <dolphin/gx/GXCull.h>

using namespace JDrama;

TViewport::TViewport(const TRect& param_1, const char* name)
    : TViewObj(name)
    , unk10(param_1)
    , unk20(0)
{
}

void TViewport::perform(u32 param_1, TGraphics* param_2)
{
	if (!(param_1 & 0x88))
		return;

	param_2->setViewport(unk10, 0.0f, 1.0f);

	if (unk20 & 1)
		return;

	TRect rect(param_2->getUnk44());
	rect.intersect(unk10);
	param_2->mScissorRect = rect;
	GXSetScissor(param_2->mScissorRect.x1, param_2->mScissorRect.y1,
	             param_2->mScissorRect.getWidth(),
	             param_2->mScissorRect.getHeight());
}

void TViewport::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	stream.read(&unk10.x1, sizeof(int));
	stream.read(&unk10.y1, sizeof(int));
	stream.read(&unk10.x2, sizeof(int));
	stream.read(&unk10.y2, sizeof(int));
}
