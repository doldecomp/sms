#include <JSystem/JDrama/JDRSmJ3DScn.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DDrawBuffer.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JDrama/JDRLighting.hpp>

using namespace JDrama;

TSmJ3DScn::TSmJ3DScn(const char* name, s32 draw_bufs)
    : TViewObjPtrListT(name)
{
	mLightMap = nullptr;

	mDrawBufferCount = draw_bufs;
	mDrawBuffers     = new J3DDrawBuffer*[mDrawBufferCount];
	for (int i = 0; i < mDrawBufferCount; ++i) {
		mDrawBuffers[i] = new J3DDrawBuffer(0x400);
	}
}

void TSmJ3DScn::perform(u32 param_1, TGraphics* param_2)
{
	if ((param_1 & 3)) {
		TViewObjPtrListT::perform(param_1, param_2);
	}

	if ((param_1 & 8)) {
		if (mLightMap)
			mLightMap->perform(param_1, param_2);

		MTXCopy(param_2->mViewMtx.mMtx, j3dSys.getViewMtx());
		j3dSys.drawInit();
		for (int i = 0; i < mDrawBufferCount; ++i)
			mDrawBuffers[i]->frameInit();
		j3dSys.mDrawBuffer[0] = mDrawBuffers[0];
		j3dSys.mDrawBuffer[1] = mDrawBuffers[1];
		TViewObjPtrListT::perform(param_1, param_2);
		j3dSys.unk4C = 3;
		mDrawBuffers[0]->draw();
		j3dSys.unk4C = 4;
		mDrawBuffers[1]->draw();
	}
}

void TSmJ3DScn::loadSuper(JSUMemoryInputStream& stream)
{
	TViewObjPtrListT::loadSuper(stream);
	mLightMap = new TLightMap;
	load(stream);
}
