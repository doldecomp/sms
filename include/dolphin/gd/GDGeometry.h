#ifndef _DOLPHIN_GDGEOMETRY_H_
#define _DOLPHIN_GDGEOMETRY_H_

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

// Command processor register IDs
#define CP_REG_MTXIDXA_ID     0x30 // Matrix index A
#define CP_REG_MTXIDXB_ID     0x40 // Matrix index B
#define CP_REG_VCD_LO_ID      0x50 // Vertex descriptor (lo)
#define CP_REG_VCD_HI_ID      0x60 // Vertex descriptor (hi)
#define CP_REG_VAT_GRP0_ID    0x70 // Vertex attribute table (group 0)
#define CP_REG_VAT_GRP1_ID    0x80 // Vertex attribute table (group 1)
#define CP_REG_VAT_GRP2_ID    0x90 // Vertex attribute table (group 2)
#define CP_REG_ARRAYBASE_ID   0xA0 // Vertex array start/base
#define CP_REG_ARRAYSTRIDE_ID 0xB0 // Vertex array stride

// clang-format off
#define CP_REG_VCD_LO(pnMtxIdx, txMtxIdxMask, posn, norm, col0, col1) \
    ( \
        ((u32)(pnMtxIdx)) << 0  | \
        ((u32)(txMtxIdxMask)) << 1  | \
        ((u32)(posn)) << 9  | \
        ((u32)(norm)) << 11 | \
        ((u32)(col0)) << 13 | \
        ((u32)(col1)) << 15   \
    )

#define CP_REG_VCD_HI(tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7) \
    ( \
        ((u32)(tex0)) << 0  | \
        ((u32)(tex1)) << 2  | \
        ((u32)(tex2)) << 4  | \
        ((u32)(tex3)) << 6  | \
        ((u32)(tex4)) << 8  | \
        ((u32)(tex5)) << 10 | \
        ((u32)(tex6)) << 12 | \
        ((u32)(tex7)) << 14   \
    )

#define CP_REG_VAT_GRP0(posCnt, posType, posFrac, nrmCnt, nrmType, c0Cnt, c0Type, c1Cnt, c1Type, tx0Cnt, tx0Type, tx0Frac, p12, nrmIdx3) \
    ( \
        ((u32)(posCnt)) << 0 | \
        ((u32)(posType)) << 1 | \
        ((u32)(posFrac)) << 4 | \
        ((u32)(nrmCnt)) << 9 | \
        ((u32)(nrmType)) << 10 | \
        ((u32)(c0Cnt)) << 13 | \
        ((u32)(c0Type)) << 14 | \
        ((u32)(c1Cnt)) << 17 | \
        ((u32)(c1Type)) << 18 | \
        ((u32)(tx0Cnt)) << 21 | \
        ((u32)(tx0Type)) << 22 | \
        ((u32)(tx0Frac)) << 25 | \
        ((u32)(p12)) << 30 | \
        ((u32)(nrmIdx3)) << 31 \
    )

#define CP_REG_VAT_GRP1(tx1Cnt, tx1Type, tx1Frac, tx2Cnt, tx2Type, tx2Frac, tx3Cnt, tx3Type, tx3Frac, tx4Cnt, tx4Type, p11) \
    ( \
        ((u32)(tx1Cnt)) << 0 | \
        ((u32)(tx1Type)) << 1 | \
        ((u32)(tx1Frac)) << 4 | \
        ((u32)(tx2Cnt)) << 9 | \
        ((u32)(tx2Type)) << 10 | \
        ((u32)(tx2Frac)) << 13 | \
        ((u32)(tx3Cnt)) << 18 | \
        ((u32)(tx3Type)) << 19 | \
        ((u32)(tx3Frac)) << 22 | \
        ((u32)(tx4Cnt)) << 27 | \
        ((u32)(tx4Type)) << 28 | \
        ((u32)(p11)) << 31 \
    )

#define CP_REG_VAT_GRP2(tx4Frac, tx5Cnt, tx5Type, tx5Frac, tx6Cnt, tx6Type, tx6Frac, tx7Cnt, tx7Type, tx7Frac) \
    ( \
        ((u32)(tx4Frac)) << 0 | \
        ((u32)(tx5Cnt)) << 5 | \
        ((u32)(tx5Type)) << 6 | \
        ((u32)(tx5Frac)) << 9 | \
        ((u32)(tx6Cnt)) << 14 | \
        ((u32)(tx6Type)) << 15 | \
        ((u32)(tx6Frac)) << 18 | \
        ((u32)(tx7Cnt)) << 23 | \
        ((u32)(tx7Type)) << 24 | \
        ((u32)(tx7Frac)) << 27 \
    )
// clang-format on

// Transform unit register IDs
#define XF_REG_ERROR_ID        0x1000
#define XF_REG_DIAGNOSTICS_ID  0x1001
#define XF_REG_STATE0_ID       0x1002
#define XF_REG_STATE1_ID       0x1003
#define XF_REG_CLOCK_ID        0x1004
#define XF_REG_CLIPDISABLE_ID  0x1005
#define XF_REG_PERF0_ID        0x1006
#define XF_REG_PERF1_ID        0x1007
#define XF_REG_INVERTEXSPEC_ID 0x1008
#define XF_REG_NUMCOLORS_ID    0x1009
#define XF_REG_DUALTEXTRAN_ID  0x1012
#define XF_REG_SCALEX_ID       0x101A
#define XF_REG_SCALEY_ID       0x101B
#define XF_REG_SCALEZ_ID       0x101C
#define XF_REG_OFFSETX_ID      0x101D
#define XF_REG_OFFSETY_ID      0x101E
#define XF_REG_OFFSETZ_ID      0x101F
#define XF_REG_NUMTEX_ID       0x103F
#define XF_REG_TEX0_ID         0x1040
#define XF_REG_TEX1_ID         0x1041
#define XF_REG_TEX2_ID         0x1042
#define XF_REG_TEX3_ID         0x1043
#define XF_REG_TEX4_ID         0x1044
#define XF_REG_TEX5_ID         0x1045
#define XF_REG_TEX6_ID         0x1046
#define XF_REG_TEX7_ID         0x1047
#define XF_REG_DUALTEX0_ID     0x1050
#define XF_REG_DUALTEX1_ID     0x1051
#define XF_REG_DUALTEX2_ID     0x1052
#define XF_REG_DUALTEX3_ID     0x1053
#define XF_REG_DUALTEX4_ID     0x1054
#define XF_REG_DUALTEX5_ID     0x1055
#define XF_REG_DUALTEX6_ID     0x1056
#define XF_REG_DUALTEX7_ID     0x1057

// clang-format off
#define XF_REG_INVTXSPEC(ncols, nnorms, ntexs) \
    ( \
        ((u32)(ncols)) << 0 | \
        ((u32)(nnorms)) << 2 | \
        ((u32)(ntexs)) << 4 \
    )

#define XF_REG_TEX(proj, form, tgType, row, embossRow, embossLit) \
    ( \
        ((u32)(proj)) << 1 | \
        ((u32)(form)) << 2 | \
        ((u32)(tgType)) << 4 | \
        ((u32)(row)) << 7 | \
        ((u32)(embossRow)) << 12 | \
        ((u32)(embossLit)) << 15 \
    )

#define XF_REG_DUALTEX(mtx, normalize) \
    ( \
        ((u32)(mtx)) << 0 | \
        ((u32)(normalize)) << 8 \
    )

#define BP_GEN_MODE(nTexGens, nChans, nTevs, p4, nInds) \
    ( \
        ((u32)(nTexGens)) << 0 | \
        ((u32)(nChans)) << 4 | \
        ((u32)(nTevs)) << 10 | \
        ((u32)(p4)) << 14 | \
        ((u32)(nInds)) << 16 \
    )

#define BP_LP_SIZE(lineWidth, pointSize, lineOffset, pointOffset, lineHalfAspect, p5) \
    ( \
        ((u32)(lineWidth)) << 0 | \
        ((u32)(pointSize)) << 8 | \
        ((u32)(lineOffset)) << 16 | \
        ((u32)(pointOffset)) << 19 | \
        ((u32)(lineHalfAspect)) << 22 | \
        ((u32)(p5)) << 24 \
    )
// clang-format on

void GDSetVtxDescv(const GXVtxDescList* attrPtr);
void GDSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list);
void GDSetArray(GXAttr attr, void* base_ptr, u8 stride);
void GDSetArrayRaw(GXAttr attr, u32 base_ptr_raw, u8 stride);
void GDPatchArrayPtr(void* base_ptr);
void GDSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
                      GXTexGenSrc src_param, u8 normalize, u32 postmtx);
void GDSetCullMode(GXCullMode mode);
void GDSetGenMode(u8 nTexGens, u8 nChans, u8 nTevs);
void GDSetGenMode2(u8 nTexGens, u8 nChans, u8 nTevs, u8 nInds, GXCullMode cm);
void GDSetLPSize(u8 lineWidth, u8 pointSize, GXTexOffset lineOffset,
                 GXTexOffset pointOffset, u8 lineHalfAspect);
void GDSetCoPlanar(u8 enable);

#ifdef __cplusplus
}
#endif

#endif
