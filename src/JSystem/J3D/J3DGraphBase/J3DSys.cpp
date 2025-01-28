#include <JSystem/J3D/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JRenderer.hpp>
#include <dolphin/os.h>

#pragma opt_strength_reduction off

J3DSys j3dSys;

u32 j3dDefaultViewNo;

Mtx J3DSys::mCurrentMtx;
Vec J3DSys::mCurrentS;
Vec J3DSys::mParentS;
J3DTexCoordScaleInfo J3DSys::sTexCoordScaleTable[8];

extern void makeTexCoordTable();
extern void makeAlphaCmpTable();
extern void makeZModeTable();
extern void makeTevSwapTable();

J3DSys::J3DSys()
{
	makeTexCoordTable();
	makeTevSwapTable();
	makeAlphaCmpTable();
	makeZModeTable();
	mFlags = 0;
	MTXIdentity(mViewMtx);
	unk4C  = 1;
	unk50  = 0;
	unk38  = 0;
	unk40  = 0;
	unk44  = 0;
	unk48  = 0;
	unk54  = 0;
	unk104 = 0;
	unk108 = 0;
	unk10C = 0;
	unk110 = 0;
	unk114 = 0;
	unk118 = 0;
	unk100 = 0;
	unk11C = 0;

	for (u32 i = 0; i < 8; i++) {
		sTexCoordScaleTable[i].field_0x00 = 1;
		sTexCoordScaleTable[i].field_0x02 = 1;
		sTexCoordScaleTable[i].field_0x04 = 0;
		sTexCoordScaleTable[i].field_0x06 = 0;
	}
}

void J3DSys::loadPosMtxIndx(int id, u16 mtx_indx) const
{
	GXLoadPosMtxIndx(mtx_indx, id * 3);
}

void J3DSys::loadNrmMtxIndx(int id, u16 mtx_indx) const
{
	GXLoadNrmMtxIndx3x3(mtx_indx, id * 3);
}

void J3DSys::setTexCacheRegion(GXTexCacheSize size)
{
	const u32 kSize[] = {
		0x00008000,
		0x00020000,
		0x00080000,
		0x00000000,
	};
	const u32 kRegionNum[] = { 8, 4, 1, 0 };

	u32 regionNum      = kRegionNum[size];
	mTexCacheRegionNum = regionNum;

	if ((mFlags & 0x80000000) ? 1 : 0) {
		for (u32 i = 0; i < regionNum; i++) {
			if (!!(i & 1)) {
				GXInitTexCacheRegion(&mTexCacheRegion[i], GX_FALSE,
				                     i * kSize[size] + 0x80000, size,
				                     i * kSize[size], size);
				JRNLoadTexCached((GXTexMapID)i, i * kSize[size] + 0x80000, size,
				                 i * kSize[size], size);
			} else {
				GXInitTexCacheRegion(&mTexCacheRegion[i], GX_FALSE,
				                     i * kSize[size], size,
				                     i * kSize[size] + 0x80000, size);
				JRNLoadTexCached((GXTexMapID)i, i * kSize[size], size,
				                 i * kSize[size] + 0x80000, size);
			}
		}
	} else {
		for (u32 i = 0; i < regionNum; i++) {
			GXInitTexCacheRegion(&mTexCacheRegion[i], GX_FALSE, i * kSize[size],
			                     size, i * kSize[size] + 0x80000, size);
			JRNLoadTexCached((GXTexMapID)i, i * kSize[size], size,
			                 i * kSize[size] + 0x80000, size);
		}
	}
}

extern u8 j3dDefaultColorChanNum;
extern GXColor j3dDefaultColInfo;
extern GXColor j3dDefaultTevKColor;
extern GXColorS10 j3dDefaultTevColor;

struct TexCoordInfo {
	u8 genType;
	u8 genSrc;
	u8 mtx;
	u8 padding;
};

extern TexCoordInfo j3dDefaultTexCoordInfo[8];

void J3DSys::drawInit()
{
	GXSetMisc(GX_MT_DL_SAVE_CONTEXT, 0);
	GXInvalidateVtxCache();
	GXSetCurrentMtx(0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetCoPlanar(GX_FALSE);
	GXSetClipMode(GX_CLIP_ENABLE);
	GXSetColorUpdate(GX_TRUE);
	GXSetDither(GX_FALSE);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
	GXSetAlphaCompare(GX_ALWAYS, GX_FALSE, GX_AOP_AND, GX_ALWAYS, GX_FALSE);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, GX_FALSE);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_F32, GX_FALSE);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, GX_FALSE);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR1, GX_CLR_RGBA, GX_RGBA8, GX_FALSE);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX2, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX3, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX4, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX5, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX6, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX7, GX_CLR_RGBA, GX_RGBA4, '\a');
	GXSetNumIndStages(0);
	reinitTexture();
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE10, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE11, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE12, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE13, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE14, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);
	GXSetTevOrder(GX_TEVSTAGE15, GX_TEXCOORD_NULL, GX_TEXMAP_NULL,
	              GX_COLOR_NULL);

	u8 i;

	Mtx mtx;
	PSMTXIdentity(mtx);
	for (i = 0; i < 10; ++i) {
		GXLoadTexMtxImm(mtx, GX_TEXMTX0 + i * 3, GX_MTX3x4);
	}

	f32 mtx2[2][3] = {
		{ 0.5f, 0.0f, 0.0f },
		{ 0.0f, 0.5f, 0.0f },
	};
	for (i = 0; i < 3; ++i) {
		GXSetIndTexMtx((GXIndTexMtxID)(GX_ITM_0 + i), mtx2, GX_TRUE);
	}

	GXSetChanMatColor(GX_COLOR0A0, j3dDefaultColInfo);
	GXSetChanMatColor(GX_COLOR1A1, j3dDefaultColInfo);
	GXSetNumChans(j3dDefaultColorChanNum);

	GXSetNumTexGens(GX_TRUE);
	GXSetNumTevStages(GX_TRUE);

	GXChannelID ids[4] = { GX_COLOR0, GX_ALPHA0, GX_COLOR1, GX_ALPHA1 };
	for (int i = 0; i < 4; ++i) {
		GXSetChanCtrl(ids[i], GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_CLAMP,
		              GX_AF_NONE);
	}

	for (i = 0; i < 8; ++i) {
		u32 mtx              = j3dDefaultTexCoordInfo[i].mtx;
		GXTexGenSrc genSrc   = (GXTexGenSrc)j3dDefaultTexCoordInfo[i].genSrc;
		GXTexGenType genType = (GXTexGenType)j3dDefaultTexCoordInfo[i].genType;
		GXSetTexCoordGen2((GXTexCoordID)i, genType, genSrc, mtx, GX_FALSE,
		                  0x7d);
	}

	for (i = 0; i < 4; ++i) {
		GXSetIndTexCoordScale((GXIndTexStageID)i, GX_ITS_1, GX_ITS_1);
	}

	for (i = 0; i < 4; ++i) {
		GXSetTevKColor((GXTevKColorID)i, j3dDefaultTevKColor);
	}

	for (i = 0; i < 4; ++i) {
		GXSetTevColorS10((GXTevRegID)i, j3dDefaultTevColor);
	}

	for (i = 0; i < 16; ++i) {
		GXSetTevColorIn((GXTevStageID)i, GX_CC_RASC, GX_CC_ZERO, GX_CC_ZERO,
		                GX_CC_ZERO);
		GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
		GXSetTevAlphaIn((GXTevStageID)i, GX_CA_RASA, GX_CA_ZERO, GX_CA_ZERO,
		                GX_CA_ZERO);
		GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1,
		                GX_TRUE, GX_TEVPREV);
	}

	for (i = 0; i < 16; ++i) {
		GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
	}

	GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
	                      GX_CH_ALPHA);
	GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED,
	                      GX_CH_ALPHA);
	GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN,
	                      GX_CH_ALPHA);
	GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE,
	                      GX_CH_ALPHA);

	for (i = 0; i < 16; ++i) {
		GXSetTevIndirect((GXTevStageID)i, GX_INDTEXSTAGE0, GX_ITF_8,
		                 GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF, GX_ITW_OFF,
		                 GX_FALSE, GX_FALSE, GX_ITBA_OFF);
	}

	OSInitFastCast();
	setTexCacheRegion(GX_TEXCACHE_32K);
}

u8 NullTexData[16] __attribute__((aligned(32))) = { 0 };

void J3DSys::reinitTexture()
{
	GXTexObj GStack_24;

	GXInitTexObj(&GStack_24, &NullTexData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP,
	             GX_FALSE);
	GXLoadTexObj(&GStack_24, GX_TEXMAP0);
	GXLoadTexObj(&GStack_24, GX_TEXMAP1);
	GXLoadTexObj(&GStack_24, GX_TEXMAP2);
	GXLoadTexObj(&GStack_24, GX_TEXMAP3);
	GXLoadTexObj(&GStack_24, GX_TEXMAP4);
	GXLoadTexObj(&GStack_24, GX_TEXMAP5);
	GXLoadTexObj(&GStack_24, GX_TEXMAP6);
	GXLoadTexObj(&GStack_24, GX_TEXMAP7);
}
