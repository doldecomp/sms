#include <dolphin/base/PPCArch.h>
#include <dolphin/gx.h>
#include <dolphin/os.h>
#include <macros.h>
#include <stddef.h>

#include "__gx.h"

static GXDrawSyncCallback TokenCB;
static GXDrawDoneCallback DrawDoneCB;
static u8 DrawDone;
static OSThreadQueue FinishQueue;

void GXSetMisc(GXMiscToken token, u32 val)
{
	switch (token) {
	case GX_MT_XF_FLUSH:
		gx->unk04  = val;
		gx->vNum   = !gx->unk04;
		gx->bpSent = 1;
		if (gx->unk04) {
			gx->dirtyState |= 8;
		}
		break;
	case GX_MT_DL_SAVE_CONTEXT:
		gx->dlSaveContext = (val > 0);
		break;
	case GX_MT_NULL:
		break;
	default:
		break;
	}
}

void GXFlush(void)
{
	u32 i;

	if (gx->dirtyState) {
		__GXSetDirtyState();
	}
	for (i = 8; i > 0; i--) {
		GX_WRITE_U32(0);
	}
	PPCSync();
}

static inline void __GXAbortWait(u32 clocks)
{
	OSTime time0;
	OSTime time1;

	time0 = OSGetTime();
	do {
		time1 = OSGetTime();
	} while (time1 - time0 <= (clocks / 4));
}

void GXSetDrawSync(u16 token)
{
	BOOL enabled;
	u32 reg;

	enabled = OSDisableInterrupts();
	reg     = token | 0x48000000;
	GX_WRITE_BP_REG(reg);
	SET_REG_FIELD(0x16F, reg, 16, 0, token);
	SET_REG_FIELD(0x170, reg, 8, 24, 0x47);
	GX_WRITE_BP_REG(reg);
	GXFlush();
	OSRestoreInterrupts(enabled);
	gx->bpSent = 0;
}

void GXSetDrawDone(void)
{
	u32 reg;
	BOOL enabled;

	enabled = OSDisableInterrupts();
	reg     = 0x45000002;
	GX_WRITE_BP_REG(reg);
	GXFlush();
	DrawDone = 0;
	OSRestoreInterrupts(enabled);
}

void GXWaitDrawDone(void)
{
	BOOL enabled;

	enabled = OSDisableInterrupts();
	while (!DrawDone) {
		OSSleepThread(&FinishQueue);
	}
	OSRestoreInterrupts(enabled);
}

void GXDrawDone(void)
{
	GXSetDrawDone();
	GXWaitDrawDone();
}

void GXPixModeSync(void)
{
	GX_WRITE_BP_REG(gx->peCtrl);
	gx->bpSent = 0;
}

void GXPokeAlphaMode(GXCompare func, u8 threshold)
{
	u32 reg;

	reg        = (func << 8) | threshold;
	__peReg[3] = reg;
}

void GXPokeAlphaRead(GXAlphaReadMode mode)
{
	u32 reg;

	reg = 0;
	SET_REG_FIELD(0x26D, reg, 2, 0, mode);
	SET_REG_FIELD(0x26E, reg, 1, 2, 1);
	__peReg[4] = reg;
}

void GXPokeAlphaUpdate(GXBool update_enable)
{
	u32 reg;

	reg = __peReg[1];
	SET_REG_FIELD(0x27A, reg, 1, 4, update_enable);
	__peReg[1] = reg;
}

void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                     GXBlendFactor dst_factor, GXLogicOp op)
{
	u32 reg;

	reg = __peReg[1];
	SET_REG_FIELD(0x28C, reg, 1, 0,
	              (type == GX_BM_BLEND) || (type == GX_BM_SUBTRACT));
	SET_REG_FIELD(0x28D, reg, 1, 11, (type == GX_BM_SUBTRACT));
	SET_REG_FIELD(0x28F, reg, 1, 1, (type == GX_BM_LOGIC));
	SET_REG_FIELD(0x290, reg, 4, 12, op);
	SET_REG_FIELD(0x291, reg, 3, 8, src_factor);
	SET_REG_FIELD(0x292, reg, 3, 5, dst_factor);
	SET_REG_FIELD(0x293, reg, 8, 24, 0x41);
	__peReg[1] = reg;
}

void GXPokeColorUpdate(GXBool update_enable)
{
	u32 reg;

	reg = __peReg[1];
	SET_REG_FIELD(0x2A0, reg, 1, 3, update_enable);
	__peReg[1] = reg;
}

void GXPokeDstAlpha(GXBool enable, u8 alpha)
{
	u32 reg = 0;

	SET_REG_FIELD(0x2AB, reg, 8, 0, alpha);
	SET_REG_FIELD(0x2AC, reg, 1, 8, enable);
	__peReg[2] = reg;
}

void GXPokeDither(GXBool dither)
{
	u32 reg;

	reg = __peReg[1];
	SET_REG_FIELD(0x2B8, reg, 1, 2, dither);
	__peReg[1] = reg;
}

void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable)
{
	u32 reg = 0;

	SET_REG_FIELD(0x2C3, reg, 1, 0, compare_enable);
	SET_REG_FIELD(0x2C4, reg, 3, 1, func);
	SET_REG_FIELD(0x2C5, reg, 1, 4, update_enable);
	__peReg[0] = reg;
}

void GXPeekARGB(u16 x, u16 y, u32* color)
{
	u32 addr = (u32)OSPhysicalToUncached(0x08000000);

	SET_REG_FIELD(0x2DC, addr, 10, 2, x);
	SET_REG_FIELD(0x2DD, addr, 10, 12, y);
	SET_REG_FIELD(0x2DE, addr, 2, 22, 0);
	*color = *(u32*)addr;
}

void GXPeekZ(u16 x, u16 y, u32* z)
{
	u32 addr = (u32)OSPhysicalToUncached(0x08000000);

	SET_REG_FIELD(0x2F0, addr, 10, 2, x);
	SET_REG_FIELD(0x2F1, addr, 10, 12, y);
	SET_REG_FIELD(0x2F2, addr, 2, 22, 1);
	*z = *(u32*)addr;
}

GXDrawSyncCallback GXSetDrawSyncCallback(GXDrawSyncCallback cb)
{
	GXDrawSyncCallback oldcb;
	BOOL enabled;

	oldcb   = TokenCB;
	enabled = OSDisableInterrupts();
	TokenCB = cb;
	OSRestoreInterrupts(enabled);
	return oldcb;
}

static void GXTokenInterruptHandler(__OSInterrupt interrupt, OSContext* context)
{
	u16 token;
	OSContext exceptionContext;
	u32 reg;

	token = __peReg[7];
	if (TokenCB != NULL) {
		OSClearContext(&exceptionContext);
		OSSetCurrentContext(&exceptionContext);
		TokenCB(token);
		OSClearContext(&exceptionContext);
		OSSetCurrentContext(context);
	}
	reg = __peReg[5];
	SET_REG_FIELD(0, reg, 1, 2, 1);
	__peReg[5] = reg;
}

static void GXFinishInterruptHandler(__OSInterrupt interrupt,
                                     OSContext* context)
{
	OSContext exceptionContext;
	u32 reg;

	reg = __peReg[5];
	SET_REG_FIELD(0, reg, 1, 3, 1);
	__peReg[5] = reg;
	DrawDone   = 1;
	if (DrawDoneCB != NULL) {
		OSClearContext(&exceptionContext);
		OSSetCurrentContext(&exceptionContext);
		DrawDoneCB();
		OSClearContext(&exceptionContext);
		OSSetCurrentContext(context);
	}
	OSWakeupThread(&FinishQueue);
}

void __GXPEInit(void)
{
	u32 reg;
	__OSSetInterruptHandler(0x12, GXTokenInterruptHandler);
	__OSSetInterruptHandler(0x13, GXFinishInterruptHandler);
	OSInitThreadQueue(&FinishQueue);
	__OSUnmaskInterrupts(0x2000);
	__OSUnmaskInterrupts(0x1000);
	reg = __peReg[5];
	SET_REG_FIELD(0, reg, 1, 2, 1);
	SET_REG_FIELD(0, reg, 1, 3, 1);
	SET_REG_FIELD(0, reg, 1, 0, 1);
	SET_REG_FIELD(0, reg, 1, 1, 1);
	__peReg[5] = reg;
}
