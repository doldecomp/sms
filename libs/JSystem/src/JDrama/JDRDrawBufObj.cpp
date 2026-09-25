#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

using namespace JDrama;

TDrawBufObj::TDrawBufObj()
    : TViewObj("<DrawBufObj>")
    , mDrawBuffer(nullptr)
    , mDrawBufferSize(0)
    , unk18(7)
{
}

TDrawBufObj::TDrawBufObj(u32 param_1, u32 param_2, const char* name)
    : TViewObj(name)
    , mDrawBufferSize(param_2)
    , unk18(param_1)
{
	mDrawBuffer = new J3DDrawBuffer(mDrawBufferSize);
}

void TDrawBufObj::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	stream >> unk18;
	stream >> mDrawBufferSize;
	mDrawBuffer = new J3DDrawBuffer(mDrawBufferSize);
}

void TDrawBufObj::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT)
		mDrawBuffer->frameInit();

	if (cue & CUE_SET_DRAW_BUFFER) {
		if (unk18 & 3)
			j3dSys.setDrawBuffer(mDrawBuffer, 0);

		if (unk18 & 4)
			j3dSys.setDrawBuffer(mDrawBuffer, 1);
	}

	if (cue & CUE_DRAW) {
		j3dSys.unk4C = unk18;
		mDrawBuffer->draw();
	}
}
