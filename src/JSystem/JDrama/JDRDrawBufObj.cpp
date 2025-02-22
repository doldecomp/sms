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
	stream.read(&unk18, sizeof(u32));
	stream.read(&mDrawBufferSize, sizeof(u32));
	mDrawBuffer = new J3DDrawBuffer(mDrawBufferSize);
}

void TDrawBufObj::perform(u32 param_1, TGraphics* param_2)
{
	if ((param_1 & 0x80))
		mDrawBuffer->frameInit();

	if ((param_1 & 0x400)) {
		if ((unk18 & 3))
			j3dSys.mDrawBuffer[0] = mDrawBuffer;

		if ((unk18 & 4))
			j3dSys.mDrawBuffer[1] = mDrawBuffer;
	}

	if ((param_1 & 8)) {
		j3dSys.unk4C = unk18;
		mDrawBuffer->draw();
	}
}
