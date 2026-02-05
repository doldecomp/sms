#include "dolphin/gx/GXStruct.h"
#include "dolphin/gx/GXVerify.h"

#define GX_WRITE_U8(ub)  GXWGFifo.u8 = (u8)(ub)
#define GX_WRITE_U16(us) GXWGFifo.u16 = (u16)(us)
#define GX_WRITE_U32(ui) GXWGFifo.u32 = (u32)(ui)
#define GX_WRITE_F32(f)  GXWGFifo.f32 = (f32)(f);

// Write to GX Transform Unit (XF) register
// TODO: Pikmin2 has nicer macros here, refactor
#define GX_WRITE_XF_REG(addr, value)                                           \
	do {                                                                       \
		GX_WRITE_U8(GX_LOAD_XF_REG);                                           \
		GX_WRITE_U32(0x1000 + (addr));                                         \
		GX_WRITE_U32(value);                                                   \
	} while (0)

#define GX_WRITE_XF_REG_2(addr, value)                                         \
	do {                                                                       \
		GX_WRITE_U32(value);                                                   \
	} while (0)

#define GX_WRITE_XF_REG_F(addr, value)                                         \
	do {                                                                       \
		GX_WRITE_F32(value);                                                   \
	} while (0)

// Write to Blitting Processor (BP) register
// NOTE: inlining breaks without the (void) 0
// which is a verify macro for debug builds
#define GX_WRITE_BP_REG(value)                                                 \
	do {                                                                       \
		GX_WRITE_U8(GX_LOAD_BP_REG);                                           \
		GX_WRITE_U32(value);                                                   \
		(void)0;                                                               \
	} while (0)

// TODO: all of these are for GX_LOAD_CP_REG and
// this is a bloody mess, should probably steal whatever
// pikmin2 does
#define GX_WRITE_SOME_REG2(a, b, c, addr)                                      \
	do {                                                                       \
		long regAddr;                                                          \
		GX_WRITE_U8(a);                                                        \
		GX_WRITE_U8(b);                                                        \
		GX_WRITE_U32(c);                                                       \
		regAddr = addr;                                                        \
		if (regAddr >= 0 && regAddr < 4) {                                     \
			gx->indexBase[regAddr] = c;                                        \
		}                                                                      \
	} while (0)
#define GX_WRITE_SOME_REG3(a, b, c, addr)                                      \
	do {                                                                       \
		long regAddr;                                                          \
		GX_WRITE_U8(a);                                                        \
		GX_WRITE_U8(b);                                                        \
		GX_WRITE_U32(c);                                                       \
		regAddr = addr;                                                        \
		if (regAddr >= 0 && regAddr < 4) {                                     \
			gx->indexStride[regAddr] = c;                                      \
		}                                                                      \
	} while (0)
#define GX_WRITE_SOME_REG4(a, b, c, addr)                                      \
	do {                                                                       \
		long regAddr;                                                          \
		GX_WRITE_U8(a);                                                        \
		GX_WRITE_U8(b);                                                        \
		GX_WRITE_U32(c);                                                       \
		regAddr = addr;                                                        \
	} while (0)

#define GET_REG_FIELD(reg, size, shift)                                        \
	((int)((reg) >> (shift)) & ((1 << (size)) - 1))

#define SET_REG_FIELD(line, reg, size, shift, val)                             \
	do {                                                                       \
		(reg) = ((u32)(reg) & ~(((1 << (size)) - 1) << (shift)))               \
		        | ((u32)(val) << (shift));                                     \
	} while (0)

#define CHECK_GXBEGIN(line, name)                                              \
	ASSERTMSGLINE(line, !__GXinBegin,                                          \
	              "'" name "' is not allowed between GXBegin/GXEnd")

/* GXAttr.c */

void __GXCalculateVLim(void);
void __GXSetVCD(void);
void __GXSetVAT(void);

/* GXBump.c */

void __GXUpdateBPMask(void);
void __GXFlushTextureState(void);

/* GXFifo.c */

// GXFifoObj private data
struct __GXFifoObj {
	u8* base;
	u8* top;
	u32 size;
	u32 hiWatermark;
	u32 loWatermark;
	void* rdPtr;
	void* wrPtr;
	s32 count;
	u8 bind_cpu;
	u8 bind_gp;
};

void __GXSaveCPUFifoAux(struct __GXFifoObj* realFifo);
void __GXFifoInit(void);
void __GXInsaneWatermark(void);
void __GXCleanGPFifo(void);

/* GXGeometry.c */

void __GXSetDirtyState(void);
void __GXSendFlushPrim(void);
void __GXSetGenMode(void);

/* GXInit.c */

struct __GXData_struct {
	// total size: 0x4F4
	u16 vNum;                     // offset 0x0, size 0x2
	u16 bpSent;                   // offset 0x2, size 0x2
	u16 unk04; /* vNum? */        // offset 0x4, size 0x2
	u16 vLim;                     // offset 0x6, size 0x4
	u32 cpEnable;                 // offset 0x8, size 0x4
	u32 cpStatus;                 // offset 0xC, size 0x4
	u32 cpClr;                    // offset 0x10, size 0x4
	u32 vcdLo;                    // offset 0x14, size 0x4
	u32 vcdHi;                    // offset 0x18, size 0x4
	u32 vatA[8];                  // offset 0x1C, size 0x20
	u32 vatB[8];                  // offset 0x3C, size 0x20
	u32 vatC[8];                  // offset 0x5C, size 0x20
	u32 lpSize;                   // offset 0x7C, size 0x4
	u32 matIdxA;                  // offset 0x80, size 0x4
	u32 matIdxB;                  // offset 0x84, size 0x4
	u32 indexBase[4];             // offset 0x88, size 0x10
	u32 indexStride[4];           // offset 0x98, size 0x10
	u32 ambColor[2];              // offset 0xA8, size 0x8
	u32 matColor[2];              // offset 0xB0, size 0x8
	u32 suTs0[8];                 // offset 0xB8, size 0x20
	u32 suTs1[8];                 // offset 0xD8, size 0x20
	u32 suScis0;                  // offset 0xF8, size 0x4
	u32 suScis1;                  // offset 0xFC, size 0x4
	u32 tref[8];                  // offset 0x100, size 0x20
	u32 iref;                     // offset 0x120, size 0x4
	u32 bpMask;                   // offset 0x124, size 0x4
	u32 IndTexScale0;             // offset 0x128, size 0x4
	u32 IndTexScale1;             // offset 0x12C, size 0x4
	u32 tevc[16];                 // offset 0x130, size 0x40
	u32 teva[16];                 // offset 0x170, size 0x40
	u32 tevKsel[8];               // offset 0x1B0, size 0x20
	u32 cmode0;                   // offset 0x1D0, size 0x4
	u32 cmode1;                   // offset 0x1D4, size 0x4
	u32 zmode;                    // offset 0x1D8, size 0x4
	u32 peCtrl;                   // offset 0x1DC, size 0x4
	u32 cpDispSrc;                // offset 0x1E0, size 0x4
	u32 cpDispSize;               // offset 0x1E4, size 0x4
	u32 cpDispStride;             // offset 0x1E8, size 0x4
	u32 cpDisp;                   // offset 0x1EC, size 0x4
	u32 cpTexSrc;                 // offset 0x1F0, size 0x4
	u32 cpTexSize;                // offset 0x1F4, size 0x4
	u32 cpTexStride;              // offset 0x1F8, size 0x4
	u32 cpTex;                    // offset 0x1FC, size 0x4
	u8 cpTexZ;                    // offset 0x200, size 0x1
	u32 genMode;                  // offset 0x204, size 0x4
	GXTexRegion TexRegions[8];    // offset 0x208, size 0x80
	GXTexRegion TexRegionsCI[4];  // offset 0x288, size 0x40
	u32 nextTexRgn;               // offset 0x2C8, size 0x4
	u32 nextTexRgnCI;             // offset 0x2CC, size 0x4
	GXTlutRegion TlutRegions[20]; // offset 0x2D0, size 0x140
	GXTexRegion* (*texRegionCallback)(GXTexObj*,
	                                  GXTexMapID); // offset 0x410, size 0x4
	GXTlutRegion* (*tlutRegionCallback)(u32);      // offset 0x414, size 0x4
	GXAttrType nrmType;                            // offset 0x418, size 0x4
	u8 hasNrms;                                    // offset 0x41C, size 0x1
	u8 hasBiNrms;                                  // offset 0x41D, size 0x1
	u32 projType;                                  // offset 0x420, size 0x4
	float projMtx[6];                              // offset 0x424, size 0x18
	float vpLeft;                                  // offset 0x43C, size 0x4
	float vpTop;                                   // offset 0x440, size 0x4
	float vpWd;                                    // offset 0x444, size 0x4
	float vpHt;                                    // offset 0x448, size 0x4
	float vpNearz;                                 // offset 0x44C, size 0x4
	float vpFarz;                                  // offset 0x450, size 0x4
	u8 fgRange;                                    // offset 0x454, size 0x1
	float fgSideX;                                 // offset 0x458, size 0x4
	u32 tImage0[8];                                // offset 0x45C, size 0x20
	u32 tMode0[8];                                 // offset 0x47C, size 0x20
	u32 texmapId[16];                              // offset 0x49C, size 0x40
	u32 tcsManEnab;                                // offset 0x4DC, size 0x4
	u32 pad0;         // TODO: might go higher above
	GXPerf0 perf0;    // offset 0x4E0, size 0x4
	GXPerf1 perf1;    // offset 0x4E4, size 0x4
	u32 perfSel;      // offset 0x4E8, size 0x4
	u8 inDispList;    // offset 0x4F0, size 0x1
	u8 dlSaveContext; // offset 0x4F1, size 0x1
	u8 dirtyVAT;      // offset 0x4F2, size 0x1
	u32 dirtyState;   // offset 0x4F4, size 0x4
}; // size = 0x4F8

extern struct __GXData_struct* gx;
extern u16* __memReg;
extern u16* __peReg;
extern u16* __cpReg;
extern u32* __piReg;

void __GXInitGX();

/* GXMisc.c */

void __GXBypass(u32 reg);
u16 __GXReadPEReg(u32 reg);
void __GXPEInit(void);

/* GXSave.c */

void __GXShadowDispList(void* list, u32 nbytes);
void __GXShadowIndexState(u32 idx_reg, u32 reg_data);
void __GXPrintShadowState(void);

/* GXStubs.c */

void __GXSetRange(float nearz, float fgSideX);

/* GXTexture.c */

void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32* rowTiles,
                         u32* colTiles, u32* cmpTiles);
void __GXSetSUTexRegs(void);
void __GXGetSUTexSize(GXTexCoordID coord, u16* width, u16* height);
void __GXSetTmemConfig(int);

/* GXTransform.c */

void __GXSetMatrixIndex(GXAttr matIdxAttr);

/* GXVerifRAS.c */

void __GXVerifySU(void);
void __GXVerifyBUMP(void);
void __GXVerifyTEX(void);
void __GXVerifyTEV(void);
void __GXVerifyPE(void);

/* GXVerif.c */

typedef enum {
	GXWARN_INVALID_VTX_FMT    = 0,
	GXWARN_TEX_SIZE_INIT      = 1,
	GXWARN_SCISSOR_RECT_LEFT  = 2,
	GXWARN_SCISSOR_RECT_TOP   = 3,
	GXWARN_SCISSOR_RECT_RIGHT = 4,
	GXWARN_SCISSOR_RECT_BOT   = 5,
	GXWARN_SAMPLE_VALUE       = 6,
	GXWARN_BUMP_CMD           = 7,
	GXWARN_INVALID_INDIRECT   = 8,
	GXWARN_INDIRECT_MTX       = 9,
	GXWARN_IND_TEX_NO_INIT    = 10,
	GXWARN_IND_TEX_NO_SCALE   = 11,
	GXWARN_IND_TEX_BUMP       = 12,
	GXWARN_BUMP_ACCUMULATION  = 13,
	GXWARN_BUMP_ALPHA_EN      = 14,
	GXWARN_IND_DIR_MASK       = 15,
	GXWARN_TEV_TEX_REF        = 16,
	GXWARN_TEV_INV_TEX_COORD  = 17,
	GXWARN_IND_DIR_BOTH       = 18,
	GXWARN_TEX_CONFIG         = 19,
	GXWARN_TEX_BASE           = 20,
	GXWARN_TLUT_CONFIG        = 21,
	GXWARN_TEX_POW2           = 22,
	GXWARN_TEX_CLAMP          = 23,
	GXWARN_TEX_MIN_FILT       = 24,
	GXWARN_MIN_LOD            = 25,
	GXWARN_MAX_LOD            = 26,
	GXWARN_DIAG_LOD           = 27,
	GXWARN_TEX_ANISO          = 28,
	GXWARN_TEX_FIELD          = 29,
	GXWARN_TEX_MPEG           = 30,
	GXWARN_RND_CLR_INDX       = 31,
	GXWARN_TEV_ENV            = 32,
	GXWARN_TEV_INV_CHAN       = 33,
	GXWARN_TEV_NULL_TEX       = 34,
	GXWARN_TEV_NULL_TEX_A     = 35,
	GXWARN_TEV_DIRTY_REG      = 36,
	GXWARN_TEV_DIRTY_REG_A    = 37,
	GXWARN_TEV_CLR_CLAMP      = 38,
	GXWARN_TEV_A_CLAMP        = 39,
	GXWARN_ZTEX_OFFSET        = 40,
	GXWARN_ZTEX_INVALID       = 41,
	GXWARN_TEV_LAST_CLR       = 42,
	GXWARN_TEV_LAST_A         = 43,
	GXWARN_TEV_LAST_CLR_WRAP  = 44,
	GXWARN_TEV_LAST_A_WRAP    = 45,
	GXWARN_Z_BEFORE_T_A       = 46,
	GXWARN_BLEND_LOGICOP      = 47,
	GXWARN_DITHER_MODE        = 48,
	GXWARN_MULTISAMP0         = 49,
	GXWARN_MULTISAMP1         = 50,
	GXWARN_SAMP_ORDER         = 51,
	GXWARN_INVALID_TG_TYPE    = 52,
	GXWARN_XF_CTRL_UNINIT     = 53,
	GXWARN_XF_CTRL_INIT       = 54,
	GXWARN_INV_COLOR_TG_COMB  = 55,
	GXWARN_INV_NUM_COLORS     = 56,
	GXWARN_VTX_NO_GEOM        = 57,
	GXWARN_CLR_XF0_CP1        = 58,
	GXWARN_CLR_XF1_CP0        = 59,
	GXWARN_CLR_XF1_CP2        = 60,
	GXWARN_CLR_XF2_CPN1       = 61,
	GXWARN_CLR_XF2_CPN2       = 62,
	GXWARN_INV_IVS_CLR        = 63,
	GXWARN_NRM_XF0_CP1        = 64,
	GXWARN_NRM_XF0_CP3        = 65,
	GXWARN_NRM_XF1_CP0        = 66,
	GXWARN_NRM_XF1_CP3        = 67,
	GXWARN_NRM_XF3_CP1        = 68,
	GXWARN_NRM_XF3_CP0        = 69,
	GXWARN_INV_IVS_NRM        = 70,
	GXWARN_TEX_XFN_CPM        = 71,
	GXWARN_TEX_SRC_NPOS       = 72,
	GXWARN_TEX_SRC_NNRM       = 73,
	GXWARN_TEX_SRC_NCLR0      = 74,
	GXWARN_TEX_SRC_NCLR1      = 75,
	GXWARN_TEX_SRC_NNBT       = 76,
	GXWARN_TEX_SRC_NTEX       = 77,
	GXWARN_INV_TEX_SRC        = 78,
	GXWARN_INV_TG_ORDER       = 79,
	GXWARN_BM_INV_MTX_NDX     = 80,
	GXWARN_BM_INV_TEX         = 81,
	GXWARN_BM_INV_LIT_POS     = 82,
	GXWARN_BM_NO_NBT          = 83,
	GXWARN_INV_TEX_NUM        = 84,
	GXWARN_INV_TG_SRC         = 85,
	GXWARN_CLR_ADDR_UNINIT    = 86,
	GXWARN_CLR_MAT_UNINIT     = 87,
	GXWARN_CLR_AMB_UNINIT     = 88,
	GXWARN_CLR_INV_SPEC       = 89,
	GXWARN_CLR_NO_NRM         = 90,
	GXWARN_CLR_INV_MTX_NDX    = 91,
	GXWARN_VAL_INFINITY       = 92,
	GXWARN_VAL_NAN            = 93,
	GXWARN_VAL_SMALL          = 94,
	GXWARN_VAL_LARGE          = 95,
	GXWARN_MTX1_UNINIT        = 96,
	GXWARN_GM_UNINIT          = 97,
	GXWARN_TEX_XFN_SUM        = 98,
	GXWARN_CLR_XFN_SUM        = 99,
	GXWARN_INV_NUM_ANY_TEX    = 100,
	GXWARN_INV_NUM_REG_TEX    = 101,
	GXWARN_INV_NUM_BM_TEX     = 102,
	GXWARN_INV_NUM_CLR_TEX    = 103,
	GXWARN_INV_CLR_TEX        = 104,
	GXWARN_DUP_CLR_TEX        = 105,
	GXWARN_BM_INV_MTX_VAL     = 106,
	GXWARN_TEX_INV_MTX_VAL    = 107,
	GXWARN_LIT_INV_REG        = 108,
	GXWARN_CLR_INV_MTX_VAL    = 109,
	GXWARN_INV_MTX_VAL        = 110,
	GXWARN_ADDR_UNINIT        = 111,
	GXWARN_REG_UNINIT         = 112,
	GXWARN_MAX                = 113,
} GXWarnID;

#define __GX_WARN(id) (__gxVerif->cb(GX_WARN_SEVERE, (id), __gxvWarnings[(id)]))
#define __GX_WARNF(id, ...)                                                    \
	do {                                                                       \
		sprintf(__gxvDummyStr, __gxvWarnings[(id)], __VA_ARGS__);              \
		__gxVerif->cb(GX_WARN_SEVERE, (id), __gxvDummyStr);                    \
	} while (0)

#define __GX_WARN2(level, id) (__gxVerif->cb(level, (id), __gxvWarnings[(id)]))
#define __GX_WARN2F(level, id, ...)                                            \
	do {                                                                       \
		sprintf(__gxvDummyStr, __gxvWarnings[(id)], __VA_ARGS__);              \
		__gxVerif->cb(level, (id), __gxvDummyStr);                             \
	} while (0)

struct __GXVerifyData {
	// total size: 0x13F8
	GXVerifyCallback cb;        // offset 0x0, size 0x4
	GXWarningLevel verifyLevel; // offset 0x4, size 0x4
	u32 xfRegs[80];             // offset 0x8, size 0x140
	u32 xfMtx[256];             // offset 0x148, size 0x400
	u32 xfNrm[96];              // offset 0x548, size 0x180
	u32 xfDMtx[256];            // offset 0x6C8, size 0x400
	u32 xfLight[128];           // offset 0xAC8, size 0x200
	u32 rasRegs[256];           // offset 0xCC8, size 0x400
	u8 xfRegsDirty[80];         // offset 0x10C8, size 0x50
	u8 xfMtxDirty[256];         // offset 0x1118, size 0x100
	u8 xfNrmDirty[96];          // offset 0x1218, size 0x60
	u8 xfDMtxDirty[256];        // offset 0x1278, size 0x100
	u8 xfLightDirty[128];       // offset 0x1378, size 0x80
};

extern struct __GXVerifyData* __gxVerif;
extern char* __gxvWarnings[113];
extern char __gxvDummyStr[256];

void __GXVerifyGlobal(void);
void __GXVerifyCP(GXVtxFmt fmt);
void __GXVerifyState(GXVtxFmt vtxfmt);

/* GXVerifXF.c */

void __GXVerifyXF(void);
