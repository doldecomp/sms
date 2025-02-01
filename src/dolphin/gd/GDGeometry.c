#include <dolphin/gd/GDGeometry.h>
#include <dolphin/os.h>
#include "__gd.h"

void GDSetVtxDescv(const GXVtxDescList* attrPtr)
{
	u32 nnorms;
	u32 ncols;
	u32 ntexs;
	u32 pnMtxIdx;
	u32 txMtxIdxMask;
	u32 posn;
	u32 norm;
	u32 col0;
	u32 col1;
	u32 tex0;
	u32 tex1;
	u32 tex2;
	u32 tex3;
	u32 tex4;
	u32 tex5;
	u32 tex6;
	u32 tex7;

	nnorms       = 0;
	ncols        = 0;
	ntexs        = 0;
	pnMtxIdx     = 0;
	txMtxIdxMask = 0;
	posn         = 1;
	norm         = 0;
	col0         = 0;
	col1         = 0;
	tex0         = 0;
	tex1         = 0;
	tex2         = 0;
	tex3         = 0;
	tex4         = 0;
	tex5         = 0;
	tex6         = 0;
	tex7         = 0;
	for (; attrPtr->attr != 0xff; ++attrPtr) {
		switch (attrPtr->attr) {
		case GX_VA_PNMTXIDX:
			pnMtxIdx = attrPtr->type;
			break;
		case GX_VA_TEX0MTXIDX:
			txMtxIdxMask = txMtxIdxMask & 0xfffffffe | attrPtr->type;
			break;
		case GX_VA_TEX1MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xfffffffd | (attrPtr->type & 0x7fffffff) << 1;
			break;
		case GX_VA_TEX2MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xfffffffb | (attrPtr->type & 0x3fffffff) << 2;
			break;
		case GX_VA_TEX3MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xfffffff7 | (attrPtr->type & 0x1fffffff) << 3;
			break;
		case GX_VA_TEX4MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xffffffef | (attrPtr->type & 0xfffffff) << 4;
			break;
		case GX_VA_TEX5MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xffffffdf | (attrPtr->type & 0x7ffffff) << 5;
			break;
		case GX_VA_TEX6MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xffffffbf | (attrPtr->type & 0x3ffffff) << 6;
			break;
		case GX_VA_TEX7MTXIDX:
			txMtxIdxMask
			    = txMtxIdxMask & 0xffffff7f | (attrPtr->type & 0x1ffffff) << 7;
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
			col0  = attrPtr->type;
			ncols = ncols + (col0 != 0);
			break;
		case GX_VA_CLR1:
			col1  = attrPtr->type;
			ncols = ncols + (col1 != 0);
			break;
		case GX_VA_TEX0:
			tex0  = attrPtr->type;
			ntexs = ntexs + (tex0 != 0);
			break;
		case GX_VA_TEX1:
			tex1  = attrPtr->type;
			ntexs = ntexs + (tex1 != 0);
			break;
		case GX_VA_TEX2:
			tex2  = attrPtr->type;
			ntexs = ntexs + (tex2 != 0);
			break;
		case GX_VA_TEX3:
			tex3  = attrPtr->type;
			ntexs = ntexs + (tex3 != 0);
			break;
		case GX_VA_TEX4:
			tex4  = attrPtr->type;
			ntexs = ntexs + (tex4 != 0);
			break;
		case GX_VA_TEX5:
			tex5  = attrPtr->type;
			ntexs = ntexs + (tex5 != 0);
			break;
		case GX_VA_TEX6:
			tex6  = attrPtr->type;
			ntexs = ntexs + (tex6 != 0);
			break;
		case GX_VA_TEX7:
			tex7  = attrPtr->type;
			ntexs = ntexs + (tex7 != 0);
			break;
		}
	}
	// clang-format off
	GDWriteCPCmd(0x50,
		pnMtxIdx |
		txMtxIdxMask << 1 |
		posn << 9 |
		norm << 0xb |
		col0 << 0xd |
		col1 << 0xf
	);
	GDWriteCPCmd(0x60,
		tex0 |
		tex1 << 2 |
		tex2 << 4 |
		tex3 << 6 |
		tex4 << 8 |
		tex5 << 10 |
		tex6 << 0xc |
		tex7 << 0xe
	);
	GDWriteXFCmd(0x1008,
		ncols |
		nnorms << 2 |
		ntexs << 4
	);
	// clang-format on
}

void GDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list)
{
	u32 posCnt  = 1;
	u32 posType = 4;
	u32 posFrac = 0;
	u32 nrmCnt  = 0;
	u32 nrmType = 4;
	u32 nrmIdx3 = 0;
	u32 c0Cnt   = 1;
	u32 c0Type  = 5;
	u32 c1Cnt   = 1;
	u32 c1Type  = 5;
	u32 tx0Cnt  = 1;
	u32 tx0Type = 4;
	u32 tx0Frac = 0;
	u32 tx1Cnt  = 1;
	u32 tx1Type = 4;
	u32 tx1Frac = 0;
	u32 tx2Cnt  = 1;
	u32 tx2Type = 4;
	u32 tx2Frac = 0;
	u32 tx3Cnt  = 1;
	u32 tx3Type = 4;
	u32 tx3Frac = 0;
	u32 tx4Cnt  = 1;
	u32 tx4Type = 4;
	u32 tx4Frac = 0;
	u32 tx5Cnt  = 1;
	u32 tx5Type = 4;
	u32 tx5Frac = 0;
	u32 tx6Cnt  = 1;
	u32 tx6Type = 4;
	u32 tx6Frac = 0;
	u32 tx7Cnt  = 1;
	u32 tx7Type = 4;
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
				nrmCnt  = 1;
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
	// clang-format off
	GDWriteCPCmd(vtxfmt + 0x70,
		posCnt |
		posType << 1 |
		posFrac << 4 |
		nrmCnt << 9 |
		nrmType << 10 |
		c0Cnt << 13 |
		c0Type << 14 |
		c1Cnt << 17 |
		c1Type << 18 |
		tx0Cnt << 21 |
		tx0Type << 22 |
		tx0Frac << 25 |
		1 << 30 |
		nrmIdx3 << 31
	);
	GDWriteCPCmd(vtxfmt + 0x80,
		tx1Cnt |
		tx1Type << 1 |
		tx1Frac << 4 |
		tx2Cnt << 9 |
		tx2Type << 10 |
		tx2Frac << 13 |
		tx3Cnt << 18 |
		tx3Type << 19 |
		tx3Frac << 22 |
		tx4Cnt << 27 |
		tx4Type << 28 |
		1 << 31
	);
	GDWriteCPCmd(vtxfmt + 0x90,
		tx4Frac |
		tx5Cnt << 5 |
		tx5Type << 6 |
		tx5Frac << 9 |
		tx6Cnt << 14 |
		tx6Type << 15 |
		tx6Frac << 18 |
		tx7Cnt << 23 |
		tx7Type << 24 |
		tx7Frac << 27
	);
	// clang-format on
}

void GDSetArray(GXAttr attr, void* base_ptr, u8 stride)
{
	s32 cpAttr;

	if (attr == 0x19) {
		cpAttr = 1;
	} else {
		cpAttr = attr - 9;
	}
	GDWriteCPCmd(cpAttr + 0xa0, OSCachedToPhysical(base_ptr));
	GDWriteCPCmd(cpAttr + 0xb0, stride);
}

void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride)
{
	s32 cpAttr;

	if (attr == GX_VA_NBT) {
		cpAttr = 1;
	} else {
		cpAttr = attr - 9;
	}
	GDWriteCPCmd(cpAttr + 0xa0, base_ptr_raw);
	GDWriteCPCmd(cpAttr + 0xb0, stride);
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
	// clang-format off
	GDWriteBPCmd(
		((u32)(nTexGens)) |
		((u32)(nChans)) << 4 |
		((u32)(nTevs - 1)) << 10 |
		((u32)(cm2hw[cm])) << 14 |
		((u32)(nInds)) << 16
	);
	// clang-format on
	GDWriteXFCmd(0x1009, nChans);
	GDWriteXFCmd(0x103f, nTexGens);
}
