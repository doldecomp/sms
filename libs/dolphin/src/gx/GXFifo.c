#include <dolphin/base/PPCArch.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>
#include <stddef.h>

#include "__gx.h"

static struct __GXFifoObj* CPUFifo;
static struct __GXFifoObj* GPFifo;
static OSThread* __GXCurrentThread;
static GXBool CPGPLinked;
static BOOL GXOverflowSuspendInProgress;
static GXBreakPtCallback BreakPointCB;
static u32 __GXOverflowCount;

void* __GXCurrentBP;

static void __GXFifoReadEnable(void);
static void __GXFifoReadDisable(void);
static void __GXFifoLink(u8 arg0);
static void __GXWriteFifoIntEnable(u8 arg0, u8 arg1);
static void __GXWriteFifoIntReset(u8 arg0, u8 arg1);

static void GXOverflowHandler(s16 interrupt, OSContext* context)
{
	__GXOverflowCount++;
	__GXWriteFifoIntEnable(0, 1);
	__GXWriteFifoIntReset(1, 0);
	GXOverflowSuspendInProgress = TRUE;

	OSSuspendThread(__GXCurrentThread);
}

static void GXUnderflowHandler(s16 interrupt, OSContext* context)
{
	OSResumeThread(__GXCurrentThread);
	GXOverflowSuspendInProgress = FALSE;
	__GXWriteFifoIntReset(1U, 1U);
	__GXWriteFifoIntEnable(1U, 0U);
}

static void GXBreakPointHandler(__OSInterrupt interrupt, OSContext* context)
{
	OSContext exceptionContext;

	gx->cpEnable = gx->cpEnable & 0xFFFFFFDF;
	__cpReg[1]   = gx->cpEnable;
	if (BreakPointCB != NULL) {
		OSClearContext(&exceptionContext);
		OSSetCurrentContext(&exceptionContext);
		BreakPointCB();
		OSClearContext(&exceptionContext);
		OSSetCurrentContext(context);
	}
}

static void GXCPInterruptHandler(__OSInterrupt interrupt, OSContext* context)
{
	gx->cpStatus = __cpReg[0];
	if (GET_REG_FIELD(gx->cpEnable, 1, 3)
	    && GET_REG_FIELD(gx->cpStatus, 1, 1)) {
		GXUnderflowHandler(interrupt, context);
	}
	if (GET_REG_FIELD(gx->cpEnable, 1, 2)
	    && GET_REG_FIELD(gx->cpStatus, 1, 0)) {
		GXOverflowHandler(interrupt, context);
	}
	if (GET_REG_FIELD(gx->cpEnable, 1, 5)
	    && GET_REG_FIELD(gx->cpStatus, 1, 4)) {
		GXBreakPointHandler(interrupt, context);
	}
}

void GXInitFifoBase(GXFifoObj* fifo, void* base, u32 size)
{
	struct __GXFifoObj* realFifo = (struct __GXFifoObj*)fifo;

	realFifo->base  = base;
	realFifo->top   = (u8*)base + size - 4;
	realFifo->size  = size;
	realFifo->count = 0;
	GXInitFifoLimits(fifo, size - 0x4000, (size >> 1) & ~0x1F);
	GXInitFifoPtrs(fifo, base, base);
}

void GXInitFifoPtrs(GXFifoObj* fifo, void* readPtr, void* writePtr)
{
	struct __GXFifoObj* realFifo = (struct __GXFifoObj*)fifo;
	BOOL enabled;

	enabled         = OSDisableInterrupts();
	realFifo->rdPtr = readPtr;
	realFifo->wrPtr = writePtr;
	realFifo->count = (u8*)writePtr - (u8*)readPtr;
	if (realFifo->count < 0) {
		realFifo->count += realFifo->size;
	}
	OSRestoreInterrupts(enabled);
}

void GXInitFifoLimits(GXFifoObj* fifo, u32 hiWatermark, u32 loWatermark)
{
	struct __GXFifoObj* realFifo = (struct __GXFifoObj*)fifo;

	realFifo->hiWatermark = hiWatermark;
	realFifo->loWatermark = loWatermark;
}

void GXSetCPUFifo(GXFifoObj* fifo)
{
	struct __GXFifoObj* realFifo = (struct __GXFifoObj*)fifo;
	BOOL enabled                 = OSDisableInterrupts();

	CPUFifo = realFifo;
	if (CPUFifo == GPFifo) {
		u32 reg = 0;

		__piReg[3] = (u32)realFifo->base & 0x3FFFFFFF;
		__piReg[4] = (u32)realFifo->top & 0x3FFFFFFF;
		SET_REG_FIELD(0x294, reg, 21, 5,
		              ((u32)realFifo->wrPtr & 0x3FFFFFFF) >> 5);
		SET_REG_FIELD(0x295, reg, 1, 26, 0);
		__piReg[5] = reg;
		CPGPLinked = GX_TRUE;
		__GXWriteFifoIntReset(1, 1);
		__GXWriteFifoIntEnable(1, 0);
		__GXFifoLink(1);
	} else {
		u32 reg;

		if (CPGPLinked) {
			__GXFifoLink(0);
			CPGPLinked = GX_FALSE;
		}
		__GXWriteFifoIntEnable(0, 0);
		reg        = 0;
		__piReg[3] = (u32)realFifo->base & 0x3FFFFFFF;
		__piReg[4] = (u32)realFifo->top & 0x3FFFFFFF;
		SET_REG_FIELD(0x2B7, reg, 21, 5,
		              ((u32)realFifo->wrPtr & 0x3FFFFFFF) >> 5);
		SET_REG_FIELD(0x2B8, reg, 1, 26, 0);
		__piReg[5] = reg;
	}

	__sync();

	OSRestoreInterrupts(enabled);
}

void GXSetGPFifo(GXFifoObj* fifo)
{
	struct __GXFifoObj* realFifo = (struct __GXFifoObj*)fifo;
	BOOL enabled                 = OSDisableInterrupts();

	__GXFifoReadDisable();
	__GXWriteFifoIntEnable(0, 0);
	GPFifo = realFifo;

	__cpReg[16] = (u32)realFifo->base & 0xFFFF;
	__cpReg[18] = (u32)realFifo->top & 0xFFFF;
	__cpReg[24] = realFifo->count & 0xFFFF;
	__cpReg[26] = (u32)realFifo->wrPtr & 0xFFFF;
	__cpReg[28] = (u32)realFifo->rdPtr & 0xFFFF;
	__cpReg[20] = (u32)realFifo->hiWatermark & 0xFFFF;
	__cpReg[22] = (u32)realFifo->loWatermark & 0xFFFF;
	__cpReg[17] = ((u32)realFifo->base & 0x3FFFFFFF) >> 16;
	__cpReg[19] = ((u32)realFifo->top & 0x3FFFFFFF) >> 16;
	__cpReg[25] = realFifo->count >> 16;
	__cpReg[27] = ((u32)realFifo->wrPtr & 0x3FFFFFFF) >> 16;
	__cpReg[29] = ((u32)realFifo->rdPtr & 0x3FFFFFFF) >> 16;
	__cpReg[21] = (u32)realFifo->hiWatermark >> 16;
	__cpReg[23] = (u32)realFifo->loWatermark >> 16;

	__sync();

	if (CPUFifo == GPFifo) {
		CPGPLinked = GX_TRUE;
		__GXWriteFifoIntEnable(1, 0);
		__GXFifoLink(1);
	} else {
		CPGPLinked = GX_FALSE;
		__GXWriteFifoIntEnable(0, 0);
		__GXFifoLink(0);
	}
	__GXWriteFifoIntReset(1, 1);
	__GXFifoReadEnable();
	OSRestoreInterrupts(enabled);
}

#define SOME_MACRO1(fifo)                                                      \
	do {                                                                       \
		u32 temp = __cpReg[29] << 16;                                          \
		temp |= __cpReg[28];                                                   \
		fifo->rdPtr = OSPhysicalToCached(temp);                                \
	} while (0)

#define SOME_MACRO2(fifo)                                                      \
	do {                                                                       \
		u32 temp = __cpReg[25] << 16;                                          \
		temp |= __cpReg[24];                                                   \
		fifo->count = temp;                                                    \
	} while (0)

void GXGetFifoPtrs(GXFifoObj* fifo, void** readPtr, void** writePtr)
{
	struct __GXFifoObj* realFifo = (struct __GXFifoObj*)fifo;

	if (realFifo == CPUFifo) {
		realFifo->wrPtr = OSPhysicalToCached(__piReg[5] & 0xFBFFFFFF);
	}
	if (realFifo == GPFifo) {
		SOME_MACRO1(realFifo);
		SOME_MACRO2(realFifo);
	} else {
		realFifo->count = (u8*)realFifo->wrPtr - (u8*)realFifo->rdPtr;
		if (realFifo->count < 0) {
			realFifo->count += realFifo->size;
		}
	}
	*readPtr  = realFifo->rdPtr;
	*writePtr = realFifo->wrPtr;
}

void* __GXCurrentBP;

void GXEnableBreakPt(void* break_pt)
{
	BOOL enabled = OSDisableInterrupts();

	__GXFifoReadDisable();

	__cpReg[30]   = (u32)break_pt;
	__cpReg[31]   = ((u32)break_pt >> 16) & 0x3FFF;
	gx->cpEnable  = (gx->cpEnable & 0xFFFFFFFD) | 2;
	gx->cpEnable  = (gx->cpEnable & 0xFFFFFFDF) | 0x20;
	__cpReg[1]    = gx->cpEnable;
	__GXCurrentBP = break_pt;
	__GXFifoReadEnable();
	OSRestoreInterrupts(enabled);
}

void GXDisableBreakPt(void)
{
	BOOL enabled = OSDisableInterrupts();

	gx->cpEnable  = gx->cpEnable & 0xFFFFFFFD;
	gx->cpEnable  = gx->cpEnable & 0xFFFFFFDF;
	__cpReg[1]    = gx->cpEnable;
	__GXCurrentBP = NULL;
	OSRestoreInterrupts(enabled);
}

void __GXFifoInit(void)
{
	__OSSetInterruptHandler(0x11, GXCPInterruptHandler);
	__OSUnmaskInterrupts(0x4000);
	__GXCurrentThread           = OSGetCurrentThread();
	GXOverflowSuspendInProgress = FALSE;
	CPUFifo                     = NULL;
	GPFifo                      = NULL;
}

static void __GXFifoReadEnable(void)
{
	SET_REG_FIELD(0, gx->cpEnable, 1, 0, 1);
	__cpReg[1] = gx->cpEnable;
}

static void __GXFifoReadDisable(void)
{
	SET_REG_FIELD(0, gx->cpEnable, 1, 0, 0);
	__cpReg[1] = gx->cpEnable;
}

static void __GXFifoLink(u8 en)
{
	SET_REG_FIELD(0x4B0, gx->cpEnable, 1, 4, (en != 0) ? 1 : 0);
	__cpReg[1] = gx->cpEnable;
}

static void __GXWriteFifoIntEnable(u8 hiWatermarkEn, u8 loWatermarkEn)
{
	SET_REG_FIELD(0x4C6, gx->cpEnable, 1, 2, hiWatermarkEn);
	SET_REG_FIELD(0x4C7, gx->cpEnable, 1, 3, loWatermarkEn);
	__cpReg[1] = gx->cpEnable;
}

static void __GXWriteFifoIntReset(u8 hiWatermarkClr, u8 loWatermarkClr)
{
	SET_REG_FIELD(0x4DE, gx->cpClr, 1, 0, hiWatermarkClr);
	SET_REG_FIELD(0x4DF, gx->cpClr, 1, 1, loWatermarkClr);
	__cpReg[2] = gx->cpClr;
}

GXFifoObj* GXGetCPUFifo(void) { return (GXFifoObj*)CPUFifo; }
