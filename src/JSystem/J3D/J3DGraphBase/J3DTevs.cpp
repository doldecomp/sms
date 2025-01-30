#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JRenderer.hpp>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/gd.h>
#include <macros.h>

const J3DLightInfo j3dDefaultLightInfo = {
	0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0xff, 0xff,
	0xff, 0xff, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
};

const J3DTexCoordInfo j3dDefaultTexCoordInfo[8] = {
	{ GX_MTX2x4, GX_TG_TEX0, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX1, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX2, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX3, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX4, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX5, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX6, GX_IDENTITY },
	{ GX_MTX2x4, GX_TG_TEX7, GX_IDENTITY },
};

const J3DTexMtxInfo j3dDefaultTexMtxInfo = {
	0x01, 0x00, 0xFF, 0xFF, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo = {
	0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 1,
};

const J3DTevStageInfo j3dDefaultTevStageInfo = {
	0x04, 0x0A, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
	0x05, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
};

const J3DIndTevStageInfo j3dDefaultIndTevStageInfo = {};

const J3DFogInfo j3dDefaultFogInfo = {
	0, 0, 0x140, 0.0f, 0.0f, 0.0f, 0.0f, 0x00, 0x00, 0x00, 0x00,
};

const J3DNBTScaleInfo j3dDefaultNBTScaleInfo = { 0, 1.0f, 1.0f, 1.0f };

u8 GXTexMode0Ids[8]  = { 0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3 };
u8 GXTexMode1Ids[8]  = { 0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7 };
u8 GXTexImage0Ids[8] = { 0x88, 0x89, 0x8A, 0x8B, 0xA8, 0xA9, 0xAA, 0xAB };
u8 GXTexImage1Ids[8] = { 0x8C, 0x8D, 0x8E, 0x8F, 0xAC, 0xAD, 0xAE, 0xAF };
u8 GXTexImage2Ids[8] = { 0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3 };
u8 GXTexImage3Ids[8] = { 0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7 };
u8 GXTexTlutIds[8]   = { 0x98, 0x99, 0x9A, 0x9B, 0xB8, 0xB9, 0xBA, 0xBB };

const GXColor j3dDefaultColInfo = { 0xFF, 0xFF, 0xFF, 0xFF };
const GXColor j3dDefaultAmbInfo = { 0x32, 0x32, 0x32, 0x32 };
const u8 j3dDefaultColorChanNum = 0x01;
const J3DTevOrderInfo j3dDefaultTevOrderInfoNull
    = { GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL };
const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull
    = { GX_TEXCOORD_NULL, GX_TEXMAP_NULL };
const GXColorS10 j3dDefaultTevColor = { 0xFF, 0xFF, 0xFF, 0xFF };
const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo = {};
const GXColor j3dDefaultTevKColor              = { 0xFF, 0xFF, 0xFF, 0xFF };
const J3DTevSwapModeInfo j3dDefaultTevSwapMode = {};
const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable = { 0, 1, 2, 3 };
const J3DBlendInfo j3dDefaultBlendInfo
    = { GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP };
const J3DColorChanInfo j3dDefaultColorChanInfo
    = { 0, 0, 0, 2, 2, 0, 0xFF, 0xFF };
const u8 j3dDefaultTevSwapTableID = 0x1B;
const u16 j3dDefaultAlphaCmpID    = 0xE7;
const u16 j3dDefaultZModeID       = 0x17;

void J3DLoadArrayBasePtr(GXAttr attr, void* ptr) { }

void J3DSetVtxAttrFmtv(GXVtxFmt, GXVtxAttrFmtList*, bool) { }

void loadCullMode(u8) { }

void loadTexNo(u32 param_0, const u16& param_1) { }

void J3DLightObj::load(u32 id) const
{
	GXLightID light = (GXLightID)(1 << id);
	GDSetLightPos(light, mPos.x, mPos.y, mPos.z);
	GDSetLightAttn(light, mAttnA.x, mAttnA.y, mAttnA.z, mAttnK.x, mAttnK.y,
	               mAttnK.z);
	GXColor copy = mColor;
	GXColor* ptr = &copy;
	GDSetLightColor(light, ptr);
	GDSetLightDir(light, mDir.x, mDir.y, mDir.z);
}

void J3DTexMtx::calc()
{
	Mtx44 mtx2;
	Mtx44 mtx1;

	// clang-format off
	Mtx fixupMtx1 = {
    0.5,  0.0, 0.0, 0.5,
    0.0, -0.5, 0.0, 0.5,
    0.0,  0.0, 1.0, 0.0,
  };
	Mtx fixupMtx2 = {
    0.5,  0.0, 0.5, 0.0,
    0.0, -0.5, 0.5, 0.0,
    0.0,  0.0, 1.0, 0.0,
  };
	// clang-format on

	u32 useMayaFormat = (mTexMtxInfo.mInfo >> 7) & 1;
	u32 format        = mTexMtxInfo.mInfo & 0x7F;
	if (format == J3DTexMtxMode_Projmap || format == J3DTexMtxMode_ViewProjmap
	    || format == J3DTexMtxMode_EnvmapEffectMtx) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx1);
		}
		MTXConcat(mtx1, fixupMtx1, mtx1);
		J3DMtxProjConcat(mtx1, mTexMtxInfo.mEffectMtx, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_Envmap) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtx(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMaya(mTexMtxInfo.mSRT, mtx2);
		}

		MTXConcat(mtx2, fixupMtx1, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_EnvmapOldEffectMtx) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx1);
		}
		MTXConcat(mtx1, fixupMtx2, mtx1);
		J3DMtxProjConcat(mtx1, mTexMtxInfo.mEffectMtx, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_EnvmapOld) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
		}
		MTXConcat(mtx2, fixupMtx2, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_EnvmapBasic) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx2);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx2);
		}
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_ProjmapBasic
	           || format == J3DTexMtxMode_ViewProjmapBasic
	           || format == J3DTexMtxMode_Unknown5) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx1);
		}
		J3DMtxProjConcat(mtx1, mTexMtxInfo.mEffectMtx, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_Unknown4) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mtx1);
		}
		J3DMtxProjConcat(mtx1, mTexMtxInfo.mEffectMtx, mTotalMtx);
	} else {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mTexMtxInfo.mSRT, mTexMtxInfo.mCenter,
			                    mTotalMtx);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mTexMtxInfo.mSRT, mTotalMtx);
		}
	}
}

void J3DTexMtx::load(u32 id) const
{
	J3DGDLoadTexMtxImm((MtxPtr)&mTotalMtx, id * 3 + 0x1e,
	                   (GXTexMtxType)mTexMtxInfo.mProjection);
}

void loadDither(u8) { }

void loadNBTScale(J3DNBTScale& param_0) { }

u8 j3dTexCoordTable[7624];
u8 j3dTevSwapTableTable[1024];
u8 j3dAlphaCmpTable[768];
u8 j3dZModeTable[96];

void makeTexCoordTable() { }

void makeAlphaCmpTable() { }

void makeZModeTable() { }

void makeTevSwapTable() { }
