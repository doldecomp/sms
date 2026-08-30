#include <JSystem/J3D/J3DGraphAnimator/J3DMaterialAnm.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DTexGenBlocks.hpp>
#include <JSystem/J3D/J3DGraphBase/Blocks/J3DColorBlocks.hpp>
#include <macros.h>

void J3DMatColorAnm::calc(GXColor* out) const
{
	mAnmColor->getColor(mAnmIndex, out);
}

void J3DTexMtxAnm::calc(J3DTextureSRTInfo* out) const
{
	mAnmTransform->getTransform(mAnmIndex, out);
}

void J3DTexNoAnm::calc(u16* out) const
{
	mAnmTexPattern->getTexNo(mAnmIndex, out);
}

void J3DTevColorAnm::calc(GXColorS10* out) const
{
	mAnmTevReg->getTevColorReg(mAnmIndex, out);
}

void J3DTevKColorAnm::calc(GXColor* out) const
{
	mAnmTevReg->getTevKonstReg(mAnmIndex, out);
}

void J3DMaterialAnm::initialize()
{
	for (u32 i = 0; i < ARRAY_COUNT(mMatColorAnm); ++i)
		mMatColorAnm[i] = nullptr;
	for (u32 i = 0; i < ARRAY_COUNT(mTexNoAnm); ++i)
		mTexNoAnm[i] = nullptr;
	for (u32 i = 0; i < ARRAY_COUNT(mTevColorAnm); ++i)
		mTevColorAnm[i] = nullptr;
	for (u32 i = 0; i < ARRAY_COUNT(mTevKColorAnm); ++i)
		mTevKColorAnm[i] = nullptr;
	for (u32 i = 0; i < ARRAY_COUNT(mTexMtxAnm); ++i)
		mTexMtxAnm[i] = nullptr;
}

void J3DMaterialAnm::calc(J3DMaterial* pMaterial) const
{
	for (u32 i = 0; i < ARRAY_COUNT(mMatColorAnm); i++)
		if (mMatColorAnm[i] != NULL)
			mMatColorAnm[i]->calc(&pMaterial->getMatColor(i)->color);

	for (u32 i = 0; i < ARRAY_COUNT(mTexNoAnm); i++) {
		if (mTexNoAnm[i] != NULL) {
			u16 texNo;
			mTexNoAnm[i]->calc(&texNo);
			pMaterial->setTexNo(i, texNo);
		}
	}

	for (u32 i = 0; i < 3; i++)
		if (mTevColorAnm[i] != NULL)
			mTevColorAnm[i]->calc(&pMaterial->getTevColor(i)->color);

	for (u32 i = 0; i < ARRAY_COUNT(mTevKColorAnm); i++)
		if (mTevKColorAnm[i] != NULL)
			mTevKColorAnm[i]->calc(&pMaterial->getTevKColor(i)->color);

	for (u32 i = 0; i < ARRAY_COUNT(mTexMtxAnm); i++)
		if (mTexMtxAnm[i] != NULL)
			mTexMtxAnm[i]->calc(&pMaterial->getTexMtx(i)->mSRT);

	// TODO: more inlines? we may never know
	char trash[0x8];
}
