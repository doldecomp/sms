#include <dolphin/gd/GDGeometry.h>
#include <dolphin/gd/GDBase.h>
#include <dolphin/os.h>
#include <macros.h>

void GDSetVtxDescv(const GXVtxDescList* attrPtr)
{
	u32 nnorms       = 0;
	u32 ncols        = 0;
	u32 ntexs        = 0;
	u32 pnMtxIdx     = GX_NONE;
	u32 txMtxIdxMask = 0;
	u32 posn         = GX_DIRECT;
	u32 norm         = GX_NONE;
	u32 col0         = GX_NONE;
	u32 col1         = GX_NONE;
	u32 tex0         = GX_NONE;
	u32 tex1         = GX_NONE;
	u32 tex2         = GX_NONE;
	u32 tex3         = GX_NONE;
	u32 tex4         = GX_NONE;
	u32 tex5         = GX_NONE;
	u32 tex6         = GX_NONE;
	u32 tex7         = GX_NONE;

	for (; attrPtr->attr != 0xff; ++attrPtr) {
		ASSERTMSGLINE(88,
		              attrPtr->attr >= GX_VA_PNMTXIDX
		                  && attrPtr->attr <= GX_VA_MAX_ATTR,
		              "GDSetVtxDescv: invalid attribute");
		ASSERTMSGLINE(92,
		              attrPtr->type >= GX_NONE && attrPtr->type <= GX_INDEX16,
		              "GDSetVtxDescv: invalid type");

		ASSERTMSGLINE(98,
		              attrPtr->attr >= GX_VA_PNMTXIDX
		                      && attrPtr->attr <= GX_VA_TEX7MTXIDX
		                  ? (attrPtr->type == 0 || attrPtr->type == 1)
		                  : TRUE,
		              "GDSetVtxDescv: invalid type for given attribute");

		switch (attrPtr->attr) {
		case GX_VA_PNMTXIDX:
			pnMtxIdx = attrPtr->type;
			break;
		case GX_VA_TEX0MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x1) | (attrPtr->type << 0);
			break;
		case GX_VA_TEX1MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x2) | (attrPtr->type << 1);
			break;
		case GX_VA_TEX2MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x4) | (attrPtr->type << 2);
			break;
		case GX_VA_TEX3MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x8) | (attrPtr->type << 3);
			break;
		case GX_VA_TEX4MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x10) | (attrPtr->type << 4);
			break;
		case GX_VA_TEX5MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x20) | (attrPtr->type << 5);
			break;
		case GX_VA_TEX6MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x40) | (attrPtr->type << 6);
			break;
		case GX_VA_TEX7MTXIDX:
			txMtxIdxMask = (txMtxIdxMask & ~0x80) | (attrPtr->type << 7);
			break;
		case GX_VA_POS:
			posn = attrPtr->type;
			break;
		case GX_VA_NRM:
			if (attrPtr->type != 0) {
				norm   = attrPtr->type;
				nnorms = 1;
			}
			break;
		case GX_VA_NBT:
			if (attrPtr->type != 0) {
				norm   = attrPtr->type;
				nnorms = 2;
			}
			break;
		case GX_VA_CLR0:
			col0 = attrPtr->type;
			ncols += col0 != GX_NONE;
			break;
		case GX_VA_CLR1:
			col1 = attrPtr->type;
			ncols += col1 != GX_NONE;
			break;
		case GX_VA_TEX0:
			tex0 = attrPtr->type;
			ntexs += tex0 != GX_NONE;
			break;
		case GX_VA_TEX1:
			tex1 = attrPtr->type;
			ntexs += tex1 != GX_NONE;
			break;
		case GX_VA_TEX2:
			tex2 = attrPtr->type;
			ntexs += tex2 != GX_NONE;
			break;
		case GX_VA_TEX3:
			tex3 = attrPtr->type;
			ntexs += tex3 != GX_NONE;
			break;
		case GX_VA_TEX4:
			tex4 = attrPtr->type;
			ntexs += tex4 != GX_NONE;
			break;
		case GX_VA_TEX5:
			tex5 = attrPtr->type;
			ntexs += tex5 != GX_NONE;
			break;
		case GX_VA_TEX6:
			tex6 = attrPtr->type;
			ntexs += tex6 != GX_NONE;
			break;
		case GX_VA_TEX7:
			tex7 = attrPtr->type;
			ntexs += tex7 != GX_NONE;
			break;
		}
	}

	GDWriteCPCmd(CP_REG_VCD_LO_ID,
	             CP_REG_VCD_LO(pnMtxIdx, txMtxIdxMask, posn, norm, col0, col1));
	GDWriteCPCmd(CP_REG_VCD_HI_ID,
	             CP_REG_VCD_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7));
	GDWriteXFCmd(XF_REG_INVERTEXSPEC_ID,
	             XF_REG_INVTXSPEC(ncols, nnorms, ntexs));
}

void GDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list)
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

	ASSERTMSGLINE(218, vtxfmt < GX_MAX_VTXFMT,
	              "GDSetVtxAttrFmtv: invalid vtx fmt");

	for (; list->attr != 0xff; ++list) {
		ASSERTMSGLINE(223, list->attr >= GX_VA_POS && list->attr <= GX_VA_TEX7,
		              "GDSetVtxAttrFmtv: invalid attribute");
		ASSERTMSGLINE(224, list->frac < 32,
		              "GDSetVtxAttrFmtv: invalid frac value");
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
				nrmCnt  = list->cnt;
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

	GDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP0_ID,
	             CP_REG_VAT_GRP0(posCnt, posType, posFrac, nrmCnt, nrmType,
	                             c0Cnt, c0Type, c1Cnt, c1Type, tx0Cnt, tx0Type,
	                             tx0Frac, 1, nrmIdx3));
	GDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP1_ID,
	             CP_REG_VAT_GRP1(tx1Cnt, tx1Type, tx1Frac, tx2Cnt, tx2Type,
	                             tx2Frac, tx3Cnt, tx3Type, tx3Frac, tx4Cnt,
	                             tx4Type, 1));
	GDWriteCPCmd(vtxfmt + CP_REG_VAT_GRP2_ID,
	             CP_REG_VAT_GRP2(tx4Frac, tx5Cnt, tx5Type, tx5Frac, tx6Cnt,
	                             tx6Type, tx6Frac, tx7Cnt, tx7Type, tx7Frac));
}

void GDSetArray(GXAttr attr, void* base_ptr, u8 stride)
{
	s32 cpAttr;

	if (attr == GX_VA_NBT) {
		cpAttr = GX_VA_TEX0MTXIDX;
	} else {
		cpAttr = attr - GX_VA_POS;
	}

	GDWriteCPCmd(cpAttr + CP_REG_ARRAYBASE_ID, OSCachedToPhysical(base_ptr));
	GDWriteCPCmd(cpAttr + CP_REG_ARRAYSTRIDE_ID, stride);
}

void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride)
{
	s32 cpAttr;

	if (attr == GX_VA_NBT) {
		cpAttr = GX_VA_TEX0MTXIDX;
	} else {
		cpAttr = attr - GX_VA_POS;
	}

	GDWriteCPCmd(cpAttr + CP_REG_ARRAYBASE_ID, base_ptr_raw);
	GDWriteCPCmd(cpAttr + CP_REG_ARRAYSTRIDE_ID, stride);
}

void GDSetCullMode(GXCullMode mode)
{
	static u8 cm2hw[4] = { 0, 2, 1, 3 };

	GDWriteBPCmd(0xfe00c000);
	GDWriteBPCmd(cm2hw[mode] << 14);
}

void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm)
{
	static u8 cm2hw[4] = { 0, 2, 1, 3 };

	GDWriteBPCmd(0xfe07fc3f);
	GDWriteBPCmd(BP_GEN_MODE(nTexGens, nChans, nTevs - 1, cm2hw[cm], nInds));

	GDWriteXFCmd(XF_REG_NUMCOLORS_ID, nChans);
	GDWriteXFCmd(XF_REG_NUMTEX_ID, nTexGens);
}
