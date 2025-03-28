#include <JSystem/JUtility/JUTTexture.hpp>

#include <dolphin/gx.h>
#include <dolphin/os/OSCache.h>
#include <JSystem/JUtility/JUTPalette.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

JUTTexture::JUTTexture(int width, int height, GXTexFmt format)
{
	unk50 = 1;

	u32 size   = GXGetTexBufferSize(width, height, format, 0, 1);
	field_0x4c = new (0x20) u8[size + sizeof(ResTIMG)];

	ResTIMG* header = (ResTIMG*)field_0x4c;

	header->format          = format;
	header->alphaEnabled    = 0;
	header->width           = width;
	header->height          = height;
	header->wrapS           = 0;
	header->wrapT           = 0;
	header->isIndexTexture  = 0;
	header->colorFormat     = 0;
	header->numColors       = 0;
	header->paletteOffset   = 0;
	header->mipmapEnabled   = 0;
	header->doEdgeLod       = 0;
	header->biasClamp       = 0;
	header->maxAnisotropy   = 0;
	header->minFilter       = 1;
	header->magFilter       = 1;
	header->minLod          = 0;
	header->maxLod          = 0;
	header->mipmapCount     = 1;
	header->lodBias         = 0;
	header->imageDataOffset = sizeof(ResTIMG);

	mEmbPalette = nullptr;
	storeTIMG(header);

	DCFlushRange(mTexData, size);
}

JUTTexture::~JUTTexture()
{
	if (unk50)
		delete[] field_0x4c;
	if (mEmbPalette)
		delete mEmbPalette;
}

void JUTTexture::storeTIMG(ResTIMG const* param_0)
{
	if (param_0) {
		mTexInfo = param_0;
		mTexData = (void*)((int)mTexInfo + mTexInfo->imageDataOffset);

		if (mTexInfo->imageDataOffset == 0) {
			mTexData = (void*)((int)mTexInfo + 0x20);
		}

		if (mEmbPalette)
			delete mEmbPalette;

		mEmbPalette   = nullptr;
		field_0x2c    = nullptr;
		mTlutName     = 0;
		mFormat       = mTexInfo->format;
		mAlphaEnabled = mTexInfo->alphaEnabled;
		mWidth        = mTexInfo->width;
		mHeight       = mTexInfo->height;
		mWrapS        = mTexInfo->wrapS;
		mWrapT        = mTexInfo->wrapT;
		mMinFilter    = mTexInfo->minFilter;
		mMagFilter    = mTexInfo->magFilter;
		mMinLOD       = (s8)mTexInfo->minLod;
		mMaxLOD       = (s8)mTexInfo->maxLod;
		mLODBias      = mTexInfo->lodBias;

		if (mTexInfo->numColors == 0) {
			initTexObj();
		} else {
			GXTlut tlut = mTexInfo->numColors > 0x100 ? GX_BIGTLUT0 : GX_TLUT0;

			mEmbPalette = new JUTPalette(
			    tlut, (GXTlutFmt)mTexInfo->colorFormat,
			    (JUTTransparency)mTexInfo->alphaEnabled, mTexInfo->numColors,
			    (void*)(&mTexInfo->format + mTexInfo->paletteOffset));

			attachPalette(mEmbPalette);
		}
	}
}

void JUTTexture::attachPalette(JUTPalette* param_0)
{
	if (param_0 == nullptr && mEmbPalette != nullptr) {
		field_0x2c = mEmbPalette;
		initTexObj((GXTlut)field_0x2c->getTlutName());
	} else if (mTexInfo && mTexInfo->isIndexTexture) {
		field_0x2c = param_0;
		initTexObj((GXTlut)field_0x2c->getTlutName());
	}
}

void JUTTexture::initTexObj()
{
	GXBool mipmapEnabled = mTexInfo->mipmapEnabled != 0 ? GX_TRUE : GX_FALSE;
	u8* image            = ((u8*)mTexInfo);
	image += (mTexInfo->imageDataOffset ? mTexInfo->imageDataOffset : 0x20);
	GXInitTexObj(&mTexObj, image, mTexInfo->width, mTexInfo->height,
	             (GXTexFmt)mTexInfo->format, (GXTexWrapMode)mWrapS,
	             (GXTexWrapMode)mWrapT, mipmapEnabled);
	GXInitTexObjLOD(&mTexObj, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter,
	                mMinLOD / 8.0f, mMaxLOD / 8.0f, mLODBias / 100.0f,
	                mTexInfo->biasClamp, mTexInfo->doEdgeLod,
	                (GXAnisotropy)mTexInfo->maxAnisotropy);
}

void JUTTexture::initTexObj(GXTlut param_0)
{
	GXBool mipmapEnabled = mTexInfo->mipmapEnabled != 0 ? GX_TRUE : GX_FALSE;
	mTlutName            = param_0;
	u8* image            = ((u8*)mTexInfo);
	image += (mTexInfo->imageDataOffset ? mTexInfo->imageDataOffset : 0x20);
	GXInitTexObjCI(&mTexObj, image, mTexInfo->width, mTexInfo->height,
	               (GXCITexFmt)mTexInfo->format, (GXTexWrapMode)mWrapS,
	               (GXTexWrapMode)mWrapT, mipmapEnabled, mTlutName);
	GXInitTexObjLOD(&mTexObj, (GXTexFilter)mMinFilter, (GXTexFilter)mMagFilter,
	                mMinLOD / 8.0f, mMaxLOD / 8.0f, mLODBias / 100.0f,
	                mTexInfo->biasClamp, mTexInfo->doEdgeLod,
	                (GXAnisotropy)mTexInfo->maxAnisotropy);
}

void JUTTexture::load(GXTexMapID param_0)
{
	if (field_0x2c) {
		field_0x2c->load();
	}
	GXLoadTexObj(&mTexObj, param_0);
}
