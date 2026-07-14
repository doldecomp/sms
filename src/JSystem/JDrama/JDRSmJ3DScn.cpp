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
	for (int i = 0; i < mDrawBufferCount; ++i)
		mDrawBuffers[i] = new J3DDrawBuffer(0x400);
}

void TSmJ3DScn::perform(u32 cue, TGraphics* graphics)
{
	if (cue & (CUE_MOVE | CUE_CALC_ANIM)) {
		TViewObjPtrListT::perform(cue, graphics);
	}

	if (cue & CUE_DRAW) {
		if (mLightMap)
			mLightMap->perform(CUE_LIGHT, graphics);

		MTXCopy(graphics->mViewMtx, j3dSys.getViewMtx());
		j3dSys.drawInit();
		for (int i = 0; i < mDrawBufferCount; ++i)
			mDrawBuffers[i]->frameInit();

		j3dSys.setDrawBuffer(mDrawBuffers[0], 0);
		j3dSys.setDrawBuffer(mDrawBuffers[1], 1);
		TViewObjPtrListT::perform(cue | (CUE_CALC_VIEW | CUE_ENTRY), graphics);
		j3dSys.setUnk4C(3);
		mDrawBuffers[0]->draw();
		j3dSys.setUnk4C(4);
		mDrawBuffers[1]->draw();
	}
}

void TSmJ3DScn::loadSuper(JSUMemoryInputStream& stream)
{
	TViewObjPtrListT::loadSuper(stream);
	mLightMap = new TLightMap;
	load(stream);
}
