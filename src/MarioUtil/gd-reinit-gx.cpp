#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <types.h>

static u8 NullTexData[16] __attribute__((aligned(0x20))) = { 0 };
static Mtx IdentityMtx                                   = {
    1.0f, 0.0f, 0.0f, 0.0f, //
    0.0f, 1.0f, 0.0f, 0.0f, //
    0.0f, 0.0f, 1.0f, 0.0f, //
};

static GXColor ColorBlack = { 0, 0, 0, 0 };
static GXColor ColorWhite = { 0xff, 0xff, 0xff, 0xff };

static void ReInitGenMode();
static void ReInitGeometry();
static void ReInitLighting();
static void ReInitTransform();
static void ReInitTexture();
static void ReInitTevStages();
static void ReInitIndStages();
static void ReInitPixelProc();

extern "C" void ReInitializeGX()
{
	ReInitGenMode();
	ReInitGeometry();
	ReInitLighting();
	ReInitTransform();
	ReInitTexture();
	ReInitTevStages();
	ReInitIndStages();
	ReInitPixelProc();
}

static void ReInitGenMode()
{
	GXSetNumChans(0);
	GXSetNumTexGens(1);
	GXSetNumTevStages(1);
	GXSetNumIndStages(0);
	GXSetCullMode(GX_CULL_BACK);
	GXSetCoPlanar(GX_FALSE);
}

static void ReInitGeometry()
{
	const GXVtxAttrFmtList list[] = {
		{ GX_VA_POS, GX_POS_XYZ, GX_F32, 0 },
		{ GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0 },
		{ GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0 },
		{ GX_VA_CLR1, GX_CLR_RGBA, GX_RGBA8, 0 },
		{ GX_VA_TEX0, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX1, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX2, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX3, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX4, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX5, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX6, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_TEX7, GX_TEX_ST, GX_F32, 0 },
		{ GX_VA_NULL, (GXCompCnt)0, (GXCompType)0, 0 },
	};

	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	for (u32 i = 0; i < 8; ++i)
		GXSetVtxAttrFmtv((GXVtxFmt)i, list);
	GXSetLineWidth(6, GX_TO_ZERO);
	GXSetPointSize(6, GX_TO_ZERO);
	GXEnableTexOffsets(GX_TEXCOORD0, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD1, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD2, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD3, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD4, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD5, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD6, 0, 0);
	GXEnableTexOffsets(GX_TEXCOORD7, 0, 0);
}

static void ReInitLighting()
{
	GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE,
	              GX_AF_NONE);
	GXSetChanAmbColor(GX_COLOR0A0, ColorBlack);
	GXSetChanAmbColor(GX_COLOR1A1, ColorBlack);
	GXSetChanMatColor(GX_COLOR0A0, ColorWhite);
	GXSetChanMatColor(GX_COLOR1A1, ColorWhite);
}

static void ReInitTransform()
{

	GXLoadPosMtxImm(IdentityMtx, 0);
	GXLoadNrmMtxImm(IdentityMtx, 0);
	GXLoadTexMtxImm(IdentityMtx, 0x3c, GX_MTX3x4);
	GXLoadTexMtxImm(IdentityMtx, 0x7d, GX_MTX3x4);
	GXSetCurrentMtx(0);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX2, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX3, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD4, GX_TG_MTX2x4, GX_TG_TEX4, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD5, GX_TG_MTX2x4, GX_TG_TEX5, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD6, GX_TG_MTX2x4, GX_TG_TEX6, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordGen2(GX_TEXCOORD7, GX_TG_MTX2x4, GX_TG_TEX7, 0x3c, GX_FALSE,
	                  0x7d);
	GXSetTexCoordScaleManually(GX_TEXCOORD0, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD0, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD1, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD1, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD2, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD2, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD3, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD3, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD4, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD4, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD5, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD5, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD6, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD6, 0, 0);
	GXSetTexCoordScaleManually(GX_TEXCOORD7, 0, 0, 0);
	GXSetTexCoordCylWrap(GX_TEXCOORD7, 0, 0);
}

static void ReInitTexture()
{
	GXTexObj obj;
	GXInitTexObj(&obj, &NullTexData, 4, 4, GX_TF_IA8, GX_CLAMP, GX_CLAMP, 0);
	GXLoadTexObj(&obj, GX_TEXMAP0);
	GXLoadTexObj(&obj, GX_TEXMAP1);
	GXLoadTexObj(&obj, GX_TEXMAP2);
	GXLoadTexObj(&obj, GX_TEXMAP3);
	GXLoadTexObj(&obj, GX_TEXMAP4);
	GXLoadTexObj(&obj, GX_TEXMAP5);
	GXLoadTexObj(&obj, GX_TEXMAP6);
	GXLoadTexObj(&obj, GX_TEXMAP7);
}

static void ReInitTevStages()
{

	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD2, GX_TEXMAP2, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD3, GX_TEXMAP3, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD4, GX_TEXMAP4, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD5, GX_TEXMAP5, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD6, GX_TEXMAP6, GX_COLOR0A0);
	GXSetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD7, GX_TEXMAP7, GX_COLOR0A0);
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
	GXSetTevColor(GX_TEVREG0, ColorWhite);
	GXSetTevColor(GX_TEVREG1, ColorWhite);
	GXSetTevColor(GX_TEVREG2, ColorWhite);

	for (u32 i = 0; i < GX_MAX_TEVSTAGE; ++i) {
		GXSetTevOp((GXTevStageID)i, GX_REPLACE);
		GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_4);
		GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1);
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
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
}

static void ReInitIndStages()
{
	for (u32 i = 0; i < GX_MAX_TEVSTAGE; ++i)
		GXSetTevDirect((GXTevStageID)i);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE1, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE2, GX_ITS_1, GX_ITS_1);
	GXSetIndTexCoordScale(GX_INDTEXSTAGE3, GX_ITS_1, GX_ITS_1);
}

static void ReInitPixelProc()
{
	GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GXSetColorUpdate(GX_TRUE);
	GXSetAlphaUpdate(GX_TRUE);
	GXSetDither(GX_TRUE);
	GXSetFog(GX_FOG_NONE, 0.0f, 1.0f, 0.1f, 1.0f, ColorBlack);
	GXSetFogRangeAdj(GX_FALSE, 0, nullptr);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetDstAlpha(GX_FALSE, 0);
	GXSetZCompLoc(GX_TRUE);
}
