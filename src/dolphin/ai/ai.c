#include <dolphin/ai.h>
#include <dolphin/gx.h>
#include <dolphin/hw_regs.h>
#include <dolphin/os.h>
#include <macros.h>

#include "../gx/__gx.h"

static AISCallback __AIS_Callback;
static AIDCallback __AID_Callback;
static u8* __CallbackStack;
static u8* __OldStack;
static BOOL __AI_init_flag;
static OSTime bound_32KHz;
static OSTime bound_48KHz;
static OSTime min_wait;
static OSTime max_wait;
static OSTime buffer;

struct STRUCT_TIMELOG {
	OSTime t_start;
	OSTime t1;
	OSTime t2;
	OSTime t3;
	OSTime t4;
	OSTime t_end;
};

#if DEBUG
struct STRUCT_TIMELOG profile;
#endif

static void __AI_set_stream_sample_rate(u32 rate);
static void __AIDHandler(__OSInterrupt interrupt, OSContext* context);
static void __AISHandler(__OSInterrupt interrupt, OSContext* context);
static void __AICallbackStackSwitch(void* cb);
static void __AI_SRC_INIT(void);

AIDCallback AIRegisterDMACallback(AIDCallback callback)
{
	AIDCallback old_callback;
	BOOL old;

	old_callback   = __AID_Callback;
	old            = OSDisableInterrupts();
	__AID_Callback = callback;
	OSRestoreInterrupts(old);
	return old_callback;
}

void AIInitDMA(u32 start_addr, u32 length)
{
	BOOL old;

	old           = OSDisableInterrupts();
	__DSPRegs[24] = (__DSPRegs[24] & 0xFFFFFC00) | (start_addr >> 16);
	__DSPRegs[25] = (__DSPRegs[25] & 0xFFFF001F) | (start_addr & 0xFFFF);
	ASSERTMSGLINE(0x12E, (length & 0x1F) == 0,
	              "AIStartDMA: length must be multiple of 32 bytes");
	__DSPRegs[27] = (__DSPRegs[27] & 0xFFFF8000) | ((length >> 5) & 0xFFFF);
	OSRestoreInterrupts(old);
}

void AIStartDMA(void) { __DSPRegs[27] = __DSPRegs[27] | 0x8000; }

void AIResetStreamSampleCount(void)
{
	__AIRegs[0] = (__AIRegs[0] & ~0x20) | 0x20;
}

inline void AISetStreamTrigger(u32 trigger) { __AIRegs[3] = trigger; }

void AISetStreamPlayState(u32 state)
{
	BOOL old;
	u8 vol_left;
	u8 vol_right;

	if (state != AIGetStreamPlayState()) {
		if (AIGetStreamSampleRate() == 0 && state == AI_STREAM_START) {
			vol_left  = AIGetStreamVolRight();
			vol_right = AIGetStreamVolLeft();
			AISetStreamVolRight(0);
			AISetStreamVolLeft(0);
			old = OSDisableInterrupts();
			__AI_SRC_INIT();
			SET_REG_FIELD(0, __AIRegs[0], 1, 5, 1);
			SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_START);
			OSRestoreInterrupts(old);
			AISetStreamVolLeft(vol_left);
			AISetStreamVolRight(vol_right);
			return;
		}
		SET_REG_FIELD(0x27F, __AIRegs[0], 1, 0, state);
	}
}

u32 AIGetStreamPlayState(void) { return __AIRegs[0] & 1; }

void AISetDSPSampleRate(u32 rate)
{
	BOOL old;
	u32 play_state;
	u32 afr_state;
	u8 vol_left;
	u8 vol_right;

	if (rate != AIGetDSPSampleRate()) {
		__AIRegs[0] = (__AIRegs[0] & 0xFFFFFFBF);
		if (rate == AI_SAMPLERATE_32KHZ) {
			vol_left   = AIGetStreamVolLeft();
			vol_right  = AIGetStreamVolRight();
			play_state = AIGetStreamPlayState();
			afr_state  = AIGetStreamSampleRate();
			AISetStreamVolLeft(0U);
			AISetStreamVolRight(0U);
			old = OSDisableInterrupts();
			__AI_SRC_INIT();
			SET_REG_FIELD(0x2D8, __AIRegs[0], 1, 5, 1);
			SET_REG_FIELD(0x2D9, __AIRegs[0], 1, 1, afr_state);
			SET_REG_FIELD(0x2DA, __AIRegs[0], 1, 0, play_state);
			__AIRegs[0] |= 0x40;
			OSRestoreInterrupts(old);
			AISetStreamVolLeft(vol_left);
			AISetStreamVolRight(vol_right);
		}
	}
}

u32 AIGetDSPSampleRate(void) { return GET_REG_FIELD(__AIRegs[0], 1, 6) ^ 1; }

void AISetStreamSampleRate(u32 rate)
{
	if (rate == AI_SAMPLERATE_48KHZ) {
		__AI_set_stream_sample_rate(rate);
		return;
	}
#if DEBUG
	OSReport("AISetStreamSampleRate(): OBSOLETED. Only 48KHz streaming from "
	         "disk is supported!\n");
#endif
}

static void __AI_set_stream_sample_rate(u32 rate)
{
	BOOL old;
	u32 play_state;
	u8 vol_left;
	u8 vol_right;
	u32 dsp_src_state;

	if (rate != AIGetStreamSampleRate()) {
		play_state = AIGetStreamPlayState();
		vol_left   = AIGetStreamVolLeft();
		vol_right  = AIGetStreamVolRight();
		AISetStreamVolRight(0);
		AISetStreamVolLeft(0);
		dsp_src_state = __AIRegs[0] & 0x40;
		SET_REG_FIELD(0, __AIRegs[0], 1, 6, 0);
		old = OSDisableInterrupts();
		__AI_SRC_INIT();
		__AIRegs[0] |= dsp_src_state;
		SET_REG_FIELD(0x368, __AIRegs[0], 1, 5, 1);
		SET_REG_FIELD(0x369, __AIRegs[0], 1, 1, rate);
		OSRestoreInterrupts(old);
		AISetStreamPlayState(play_state);
		AISetStreamVolLeft(vol_left);
		AISetStreamVolRight(vol_right);
	}
}

u32 AIGetStreamSampleRate(void) { return GET_REG_FIELD(__AIRegs[0], 1, 1); }

void AISetStreamVolLeft(u8 vol)
{
	SET_REG_FIELD(0x3A3, __AIRegs[1], 8, 0, vol);
}

u8 AIGetStreamVolLeft(void) { return GET_REG_FIELD(__AIRegs[1], 8, 0); }

void AISetStreamVolRight(u8 vol)
{
	SET_REG_FIELD(0x3CC, __AIRegs[1], 8, 8, vol);
}

u8 AIGetStreamVolRight(void) { return (__AIRegs[1] & (0xFF << 8)) >> 8; }

void AIInit(u8* stack)
{
	if (__AI_init_flag != TRUE) {
		bound_32KHz = OSNanosecondsToTicks(31524);
		bound_48KHz = OSNanosecondsToTicks(42024);
		min_wait    = OSNanosecondsToTicks(42000);
		max_wait    = OSNanosecondsToTicks(63000);
		buffer      = OSNanosecondsToTicks(3000);
		AISetStreamVolRight(0);
		AISetStreamVolLeft(0);
		AISetStreamTrigger(0);
		AIResetStreamSampleCount();
		__AI_set_stream_sample_rate(AI_SAMPLERATE_48KHZ);
		AISetDSPSampleRate(AI_SAMPLERATE_32KHZ);
#if DEBUG
		OSReport("AIInit(): DSP is 32KHz\n");
#endif
		__AIS_Callback  = NULL;
		__AID_Callback  = NULL;
		__CallbackStack = stack;
		if (stack) {
			ASSERTMSGLINE(0x444, ((u32)stack & 7) != 0,
			              "AIInit: stack must be 8-byte aligned");
		}
		__OSSetInterruptHandler(5, __AIDHandler);
		__OSUnmaskInterrupts(0x04000000);
		__OSSetInterruptHandler(8, __AISHandler);
		__OSUnmaskInterrupts(0x800000);
		__AI_init_flag = TRUE;
	}
}

static void __AISHandler(__OSInterrupt interrupt, OSContext* context)
{
	OSContext exceptionContext;

	__AIRegs[0] |= 8;
	OSClearContext(&exceptionContext);
	OSSetCurrentContext(&exceptionContext);
	if (__AIS_Callback) {
		__AIS_Callback(__AIRegs[2]);
	}
	OSClearContext(&exceptionContext);
	OSSetCurrentContext(context);
}

static void __AIDHandler(__OSInterrupt interrupt, OSContext* context)
{
	OSContext exceptionContext;
	u16 tmp;

	tmp          = __DSPRegs[5];
	tmp          = (tmp & ~0xA0) | 8;
	__DSPRegs[5] = tmp;
	OSClearContext(&exceptionContext);
	OSSetCurrentContext(&exceptionContext);
	if (__AID_Callback) {
		if (__CallbackStack) {
			__AICallbackStackSwitch(__AID_Callback);
		} else {
			__AID_Callback();
		}
	}
	OSClearContext(&exceptionContext);
	OSSetCurrentContext(context);
}

static asm void __AICallbackStackSwitch(register void* cb)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	mflr r0
	stw r0, 0x4(r1)
	stwu r1, -0x18(r1)
	stw r31, 0x14(r1)
	mr r31, r3
	lis r5, __OldStack@ha
	addi r5, r5, __OldStack@l
	stw r1, 0x0(r5)
	lis r5, __CallbackStack@ha
	addi r5, r5, __CallbackStack@l
	lwz r1, 0x0(r5)
	subi r1, r1, 0x8
	mtlr r31
	blrl
	lis r5, __OldStack@ha
	addi r5, r5, __OldStack@l
	lwz r1, 0x0(r5)
	lwz r0, 0x1c(r1)
	lwz r31, 0x14(r1)
	addi r1, r1, 0x18
	mtlr r0
	blr
#endif // clang-format on
}

void __AI_SRC_INIT(void)
{
	OSTime rising_32khz = 0;
	OSTime rising_48khz = 0;
	OSTime diff         = 0;
	OSTime t1           = 0;
	OSTime temp;
	u32 temp0;
	u32 temp1;
	u32 done     = 0;
	u32 volume   = 0;
	u32 Init_Cnt = 0;
	u32 walking  = 0;

	walking  = 0;
	Init_Cnt = 0;
	temp     = 0;

#if DEBUG
	profile.t_start = OSGetTime();
#endif

	while (!done) {
		SET_REG_FIELD(0, __AIRegs[0], 1, 5, 1);
		SET_REG_FIELD(0, __AIRegs[0], 1, 1, 0);
		SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_START);
		temp0 = __AIRegs[2];
		while (temp0 == __AIRegs[2]) { }
		rising_32khz = OSGetTime();
		SET_REG_FIELD(0, __AIRegs[0], 1, 1, 1);
		SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_START);
		temp1 = __AIRegs[2];
		while (temp1 == __AIRegs[2]) { }
		rising_48khz = OSGetTime();
		diff         = rising_48khz - rising_32khz;
		SET_REG_FIELD(0, __AIRegs[0], 1, 1, 0);
		SET_REG_FIELD(0, __AIRegs[0], 1, 0, AI_STREAM_STOP);
		if (diff < bound_32KHz - buffer) {
			temp = min_wait;
			done = 1;
			Init_Cnt++;
		} else if (diff >= bound_32KHz + buffer
		           && diff < bound_48KHz - buffer) {
			temp = max_wait;
			done = 1;
			Init_Cnt++;
		} else {
			done    = 0;
			walking = 1;
			Init_Cnt++;
		}
	}
	while (rising_48khz + temp > OSGetTime()) { }
#if DEBUG
	profile.t_end = OSGetTime();
#endif
}
