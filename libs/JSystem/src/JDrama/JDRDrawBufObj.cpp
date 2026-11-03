#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>

using namespace JDrama;

TDrawBufObj::TDrawBufObj()
    : TViewObj("<DrawBufObj>")
    , mDrawBuffer(nullptr)
    , mDrawBufferSize(0)
    , mDrawFlags(J3DMaterialMode_All)
{
}

TDrawBufObj::TDrawBufObj(u32 draw_flags, u32 buffer_size, const char* name)
    : TViewObj(name)
    , mDrawBufferSize(buffer_size)
    , mDrawFlags(draw_flags)
{
	mDrawBuffer = new J3DDrawBuffer(mDrawBufferSize);
}

void TDrawBufObj::load(JSUMemoryInputStream& stream)
{
	TNameRef::load(stream);
	stream >> mDrawFlags;
	stream >> mDrawBufferSize;
	mDrawBuffer = new J3DDrawBuffer(mDrawBufferSize);
}

void TDrawBufObj::perform(u32 cue, TGraphics* graphics)
{
	if (cue & CUE_DRAW_INIT)
		mDrawBuffer->frameInit();

	if (cue & CUE_SET_DRAW_BUFFER) {
		if (mDrawFlags & J3DMaterialMode_OpaTexEdge)
			j3dSys.setDrawBuffer(mDrawBuffer, J3DSysDrawBuf_Opa);

		if (mDrawFlags & J3DMaterialMode_Xlu)
			j3dSys.setDrawBuffer(mDrawBuffer, J3DSysDrawBuf_Xlu);
	}

	if (cue & CUE_DRAW) {
		j3dSys.unk4C = mDrawFlags;
		mDrawBuffer->draw();
	}
}
