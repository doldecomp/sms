#include <JSystem/J3D/J3DGraphBase/J3DTevs.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DLightObj.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DTexMtx.hpp>
#include <JSystem/J3D/J3DGraphBase/Components/J3DNBTScale.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTransform.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/JRenderer.hpp>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/gd.h>
#include <types.h>
#include <macros.h>

u8 GXTexMode0Ids[8]  = { 0x80, 0x81, 0x82, 0x83, 0xA0, 0xA1, 0xA2, 0xA3 };
u8 GXTexMode1Ids[8]  = { 0x84, 0x85, 0x86, 0x87, 0xA4, 0xA5, 0xA6, 0xA7 };
u8 GXTexImage0Ids[8] = { 0x88, 0x89, 0x8A, 0x8B, 0xA8, 0xA9, 0xAA, 0xAB };
u8 GXTexImage1Ids[8] = { 0x8C, 0x8D, 0x8E, 0x8F, 0xAC, 0xAD, 0xAE, 0xAF };
u8 GXTexImage2Ids[8] = { 0x90, 0x91, 0x92, 0x93, 0xB0, 0xB1, 0xB2, 0xB3 };
u8 GXTexImage3Ids[8] = { 0x94, 0x95, 0x96, 0x97, 0xB4, 0xB5, 0xB6, 0xB7 };
u8 GXTexTlutIds[8]   = { 0x98, 0x99, 0x9A, 0x9B, 0xB8, 0xB9, 0xBA, 0xBB };

static u8 GX2HWFiltConv[6] = { 0x00, 0x04, 0x01, 0x05, 0x02, 0x06 };

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

void J3DLoadArrayBasePtr(GXAttr attr, void* ptr)
{
	u32 a = attr == GX_VA_NBT ? 1 : attr - 9;
	GXCmd1u8(8);
	GXCmd1u8(a + 0xA0);
	GXCmd1u32((u32)ptr & 0x7fffffff);
}

void J3DSetVtxAttrFmtv(GXVtxFmt vtxfmt, GXVtxAttrFmtList* list, bool param_3)
{
	u32 posCnt  = GX_POS_XYZ;
	u32 posType = GX_F32;
	u32 posFrac = 0;

	u32 nrmCnt  = GX_NRM_XYZ;
	u32 nrmType = GX_F32;
	u32 nrmIdx3 = 0;

	u32 c0Cnt  = GX_CLR_RGBA;
	u32 c0Type = GX_RGBA8;

	u32 c1Cnt  = GX_CLR_RGBA;
	u32 c1Type = GX_RGBA8;

	u32 tx0Cnt  = GX_TEX_ST;
	u32 tx0Type = GX_F32;
	u32 tx0Frac = 0;

	u32 tx1Cnt  = GX_TEX_ST;
	u32 tx1Type = GX_F32;
	u32 tx1Frac = 0;

	u32 tx2Cnt  = GX_TEX_ST;
	u32 tx2Type = GX_F32;
	u32 tx2Frac = 0;

	u32 tx3Cnt  = GX_TEX_ST;
	u32 tx3Type = GX_F32;
	u32 tx3Frac = 0;

	u32 tx4Cnt  = GX_TEX_ST;
	u32 tx4Type = GX_F32;
	u32 tx4Frac = 0;

	u32 tx5Cnt  = GX_TEX_ST;
	u32 tx5Type = GX_F32;
	u32 tx5Frac = 0;

	u32 tx6Cnt  = GX_TEX_ST;
	u32 tx6Type = GX_F32;
	u32 tx6Frac = 0;

	u32 tx7Cnt  = GX_TEX_ST;
	u32 tx7Type = GX_F32;
	u32 tx7Frac = 0;

	for (; list->attr != 0xff; ++list) {
		switch (list->attr) {
		case GX_VA_POS:
			posCnt  = list->cnt;
			posType = list->type;
			posFrac = list->frac;
			break;
		case GX_VA_NRM:
		case GX_VA_NBT:
			nrmType = list->type;
			if (list->cnt == 2) {
				nrmCnt  = GX_NRM_NBT;
				nrmIdx3 = 1;
			} else {
				if (param_3)
					nrmCnt = 1;
				else
					nrmCnt = list->cnt;
				nrmIdx3 = 0;
			}
			break;
		case GX_VA_CLR0:
			c0Cnt  = list->cnt;
			c0Type = list->type;
			break;
		case GX_VA_CLR1:
			c1Cnt  = list->cnt;
			c1Type = list->type;
			break;
		case GX_VA_TEX0:
			tx0Cnt  = list->cnt;
			tx0Type = list->type;
			tx0Frac = list->frac;
			break;
		case GX_VA_TEX1:
			tx1Cnt  = list->cnt;
			tx1Type = list->type;
			tx1Frac = list->frac;
			break;
		case GX_VA_TEX2:
			tx2Cnt  = list->cnt;
			tx2Type = list->type;
			tx2Frac = list->frac;
			break;
		case GX_VA_TEX3:
			tx3Cnt  = list->cnt;
			tx3Type = list->type;
			tx3Frac = list->frac;
			break;
		case GX_VA_TEX4:
			tx4Cnt  = list->cnt;
			tx4Type = list->type;
			tx4Frac = list->frac;
			break;
		case GX_VA_TEX5:
			tx5Cnt  = list->cnt;
			tx5Type = list->type;
			tx5Frac = list->frac;
			break;
		case GX_VA_TEX6:
			tx6Cnt  = list->cnt;
			tx6Type = list->type;
			tx6Frac = list->frac;
			break;
		case GX_VA_TEX7:
			tx7Cnt  = list->cnt;
			tx7Type = list->type;
			tx7Frac = list->frac;
		}
	}

	GDOverflowCheck(6);
	J3DGDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP0_ID,
	                CP_REG_VAT_GRP0(posCnt, posType, posFrac, nrmCnt, nrmType,
	                                c0Cnt, c0Type, c1Cnt, c1Type, tx0Cnt,
	                                tx0Type, tx0Frac, 1, nrmIdx3));
	GDOverflowCheck(6);
	J3DGDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP1_ID,
	                CP_REG_VAT_GRP1(tx1Cnt, tx1Type, tx1Frac, tx2Cnt, tx2Type,
	                                tx2Frac, tx3Cnt, tx3Type, tx3Frac, tx4Cnt,
	                                tx4Type, 1));
	GDOverflowCheck(6);
	J3DGDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP2_ID,
	                CP_REG_VAT_GRP2(tx4Frac, tx5Cnt, tx5Type, tx5Frac, tx6Cnt,
	                                tx6Type, tx6Frac, tx7Cnt, tx7Type,
	                                tx7Frac));
}

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

void J3DGDSetTexLookupMode(GXTexMapID id, GXTexWrapMode wrapS,
                           GXTexWrapMode wrapT, GXTexFilter minFilter,
                           GXTexFilter magFilter, f32 minLOD, f32 maxLOD,
                           f32 lodBias, u8 biasClamp, u8 edgeLOD,
                           GXAnisotropy maxAniso)
{
	// clang-format off
	u32 reg1 =
		(wrapS) << 0 |
		(wrapT) << 2 |
		(magFilter == GX_LINEAR) << 4 |
		(GX2HWFiltConv[minFilter]) << 5 |
		(!edgeLOD) << 8 |
		((u8)(lodBias * 32.0f)) << 9 |
		(maxAniso) << 19 |
		(biasClamp) << 21 |
		(GXTexMode0Ids[id] << 24);
	// clang-format on
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg1);

	// clang-format off
	u32 reg2 =
		((u8)(minLOD * 16.0f)) << 0 |
		((u8)(maxLOD * 16.0f)) << 8 |
		(GXTexMode1Ids[id] << 24);
	// clang-format on
	GDOverflowCheck(5);
	J3DGDWriteBPCmd(reg2);
}

void loadCullMode(u8) { }

void J3DLightObj::load(u32 id) const
{
	GXLightID light = (GXLightID)(1 << id);
	GDSetLightPos(light, mLightPosition.x, mLightPosition.y, mLightPosition.z);
	GDSetLightAttn(light, mCosAtten.x, mCosAtten.y, mCosAtten.z, mDistAtten.x,
	               mDistAtten.y, mDistAtten.z);
	GDSetLightColor(light, mColor);
	GDSetLightDir(light, mLightDirection.x, mLightDirection.y,
	              mLightDirection.z);
}

void J3DTexMtx::calc()
{
	Mtx44 mtx2;
	Mtx44 mtx1;

	// clang-format off
	Mtx fixupMtx1 = {
    0.5,  0.0, 0.5, 0.0,
    0.0, -0.5, 0.5, 0.0,
    0.0,  0.0, 1.0, 0.0,
  };
	Mtx fixupMtx2 = {
    0.5,  0.0, 0.0, 0.5,
    0.0, -0.5, 0.0, 0.5,
    0.0,  0.0, 1.0, 0.0,
  };
	// clang-format on

	u32 useMayaFormat = (mInfo >> 7) & 1;
	u32 format        = mInfo & 0x7F;
	if (format == J3DTexMtxMode_Projmap || format == J3DTexMtxMode_ViewProjmap
	    || format == J3DTexMtxMode_EnvmapEffectMtx) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtx(mSRT, mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMaya(mSRT, mtx1);
		}
		MTXConcat(mtx1, fixupMtx1, mtx1);
		J3DMtxProjConcat(mtx1, mEffectMtx, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_Envmap) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtx(mSRT, mCenter, mtx2);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMaya(mSRT, mtx2);
		}

		MTXConcat(mtx2, fixupMtx1, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_EnvmapOldEffectMtx) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mSRT, mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mSRT, mtx1);
		}
		MTXConcat(mtx1, fixupMtx2, mtx1);
		J3DMtxProjConcat(mtx1, mEffectMtx, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_EnvmapOld) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mSRT, mCenter, mtx2);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mSRT, mtx2);
		}
		MTXConcat(mtx2, fixupMtx2, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_EnvmapBasic) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mSRT, mCenter, mtx2);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mSRT, mtx2);
		}
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_ProjmapBasic
	           || format == J3DTexMtxMode_ViewProjmapBasic
	           || format == J3DTexMtxMode_Unknown5) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mSRT, mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mSRT, mtx1);
		}
		J3DMtxProjConcat(mtx1, mEffectMtx, mtx2);
		MTXConcat(mtx2, mViewMtx, mTotalMtx);
	} else if (format == J3DTexMtxMode_Unknown4) {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mSRT, mCenter, mtx1);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mSRT, mtx1);
		}
		J3DMtxProjConcat(mtx1, mEffectMtx, mTotalMtx);
	} else {
		if (useMayaFormat == 0) {
			J3DGetTextureMtxOld(mSRT, mCenter, mTotalMtx);
		} else if (useMayaFormat == 1) {
			J3DGetTextureMtxMayaOld(mSRT, mTotalMtx);
		}
	}
}

void J3DTexMtx::load(u32 id) const
{
	J3DGDLoadTexMtxImm((MtxPtr)&mTotalMtx, id * 3 + 0x1e,
	                   (GXTexMtxType)mProjection);
}

void loadTexNo(u32 param_1, const u16& param_2)
{
	ResTIMG* img = &j3dSys.getTexture()->mResources[param_2];
	J3DSys::sTexCoordScaleTable[param_1].field_0x00 = img->width;
	J3DSys::sTexCoordScaleTable[param_1].field_0x02 = img->height;
	J3DGDSetTexImgAttr((GXTexMapID)param_1, img->width, img->height,
	                   (GXTexFmt)(img->format & 0xf));
	J3DGDSetTexImgPtr((GXTexMapID)param_1, (u8*)img + img->imageDataOffset);

	J3DGDSetTexLookupMode(
	    (GXTexMapID)param_1, (GXTexWrapMode)img->wrapS,
	    (GXTexWrapMode)img->wrapT, (GXTexFilter)img->minFilter,
	    (GXTexFilter)img->magFilter, img->minLod * 0.125f, img->maxLod * 0.125f,
	    img->lodBias * 0.01f, img->biasClamp, img->doEdgeLod,
	    (GXAnisotropy)img->maxAnisotropy);

	if (img->isIndexTexture == true) {
		GXTlutSize tlutSize = img->numColors > 16 ? GX_TLUT_256 : GX_TLUT_16;

		J3DGDLoadTlut((u8*)img + img->paletteOffset, (param_1 << 15) + 0xC0000,
		              tlutSize);
		J3DGDSetTexTlut((GXTexMapID)param_1, (param_1 << 15) + 0xC0000,
		                (GXTlutFmt)img->colorFormat);
	}
}

void loadDither(u8) { }

void loadNBTScale(J3DNBTScale& param_0)
{
	if (param_0.mbHasScale == true) {
		j3dSys.setNBTScale(&param_0.mScale);
	} else {
		j3dSys.setNBTScale(nullptr);
	}
}

u8 j3dTexCoordTable[7624];
u8 j3dTevSwapTableTable[1024];
u8 j3dAlphaCmpTable[768];
u8 j3dZModeTable[96];

void makeTexCoordTable()
{
	u8 texMtx[] = {
		GX_TEXMTX0, GX_TEXMTX1, GX_TEXMTX2, GX_TEXMTX3, GX_TEXMTX4,  GX_TEXMTX5,
		GX_TEXMTX6, GX_TEXMTX7, GX_TEXMTX8, GX_TEXMTX9, GX_IDENTITY,
	};

	u8* table = j3dTexCoordTable;
	for (u32 i = 0; i < 11; ++i) {
		for (u32 j = 0; j < 21; ++j) {
			for (int k = 0; k < ARRAY_COUNT(texMtx); ++k) {
				u32 idx = j * 11 + i * 0xe7 + k;

				table[idx * 3 + 0] = i;
				table[idx * 3 + 1] = j;
				table[idx * 3 + 2] = texMtx[k];
			}
		}
	}
}

void makeAlphaCmpTable()
{
	u8* table = j3dAlphaCmpTable;
	for (u32 i = 0; i != 8; ++i) {
		for (int j = 0; j != 4; ++j) {
			for (u32 k = 0; k != 8; ++k) {
				u32 idx = i * 32 + j * 8 + k;

				table[idx * 3 + 0] = i;
				table[idx * 3 + 1] = j;
				table[idx * 3 + 2] = k;
			}
		}
	}
}

void makeZModeTable()
{
	u8* table = j3dZModeTable;
	for (int i = 0; i < 2; ++i) {
		for (u32 j = 0; j < 8; ++j) {
			for (int k = 0; k < 2; ++k) {
				u32 idx = j * 2 + i * 16 + k;

				table[idx * 3 + 0] = i;
				table[idx * 3 + 1] = j;
				table[idx * 3 + 2] = k;
			}
		}
	}
}

void makeTevSwapTable()
{
	u8* table = j3dTevSwapTableTable;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				for (int l = 0; l < 4; ++l) {
					u32 idx = i * 64 + j * 16 + k * 4 + l;

					table[idx * 4 + 0] = i;
					table[idx * 4 + 1] = j;
					table[idx * 4 + 2] = k;
					table[idx * 4 + 3] = l;
				}
			}
		}
	}
}
