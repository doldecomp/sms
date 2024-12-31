#include <dolphin.h>
#include <dolphin/base/PPCArch.h>
#include <dolphin/os.h>
#include <macros.h>
#include <stdio.h>

// internal include
#include "__os.h"

OSErrorHandler __OSErrorTable[16];

void OSReport(char* msg, ...)
{
	va_list marker;
	va_start(marker, msg);
	vprintf(msg, marker);
	va_end(marker);
}

void OSPanic(char* file, int line, char* msg, ...)
{
	va_list marker;
	u32 i;
	u32* p;

	OSDisableInterrupts();
	va_start(marker, msg);
	vprintf(msg, marker);
	va_end(marker);
	OSReport(" in \"%s\" on line %d.\n", file, line);

	OSReport("\nAddress:      Back Chain    LR Save\n");
	for (i = 0, p = (u32*)OSGetStackPointer();
	     p && (u32)p != 0xffffffff && i++ < 16; p = (u32*)*p) {
		OSReport("0x%08x:   0x%08x    0x%08x\n", p, p[0], p[1]);
	}

	PPCHalt();
}

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler)
{
	OSErrorHandler oldHandler;

	oldHandler            = __OSErrorTable[error];
	__OSErrorTable[error] = handler;
	return oldHandler;
}

void __OSUnhandledException(unsigned char exception, struct OSContext* context,
                            unsigned long dsisr, unsigned long dar)
{
	if (!(context->srr1 & MSR_RI)) {
		OSReport("Non-recoverable Exception %d", exception);
	} else {
		if (__OSErrorTable[exception]) {
			OSDisableScheduler();
			__OSErrorTable[exception](exception, context, dsisr, dar);
			OSEnableScheduler();
			__OSReschedule();
			OSLoadContext(context);
		}
		if (exception == OS_EXCEPTION_DECREMENTER) {
			OSLoadContext(context);
		}
		OSReport("Unhandled Exception %d", exception);
	}
#if DEBUG
	OSReport("(%s)", __OSExceptionNames[exception]);
#endif
	OSReport("\n");
	OSDumpContext(context);
	OSReport("\nDSISR = 0x%08x                   DAR  = 0x%08x\n", dsisr, dar);
	OSReport("TB = 0x%016llx\n", OSGetTime());

	switch (exception) {
	case OS_EXCEPTION_DSI:
		OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
		         "invalid address 0x%x (read from DAR)\n",
		         context->srr0, dar);
		break;
	case OS_EXCEPTION_ISI:
		OSReport("\nAttempted to fetch instruction from invalid address 0x%x "
		         "(read from SRR0)\n",
		         context->srr0);
		break;
	case OS_EXCEPTION_ALIGNMENT:
		OSReport("\nInstruction at 0x%x (read from SRR0) attempted to access "
		         "unaligned address 0x%x (read from DAR)\n",
		         context->srr0, dar);
		break;
	case OS_EXCEPTION_PROGRAM:
		OSReport("\nProgram exception : Possible illegal instruction/operation "
		         "at or around 0x%x (read from SRR0)\n",
		         context->srr0, dar);
		break;
	// TODO: WTF?!
	case OS_EXCEPTION_MAX:
		OSReport("\n");
		OSReport("AI DMA Address =   0x%04x%04x\n", __DSPRegs[DSP_DMA_START_HI],
		         __DSPRegs[DSP_DMA_START_LO]);
		OSReport("ARAM DMA Address = 0x%04x%04x\n",
		         __DSPRegs[DSP_ARAM_DMA_MM_HI], __DSPRegs[DSP_ARAM_DMA_MM_LO]);
		OSReport("DI DMA Address =   0x%08x\n", __DIRegs[5]);
		break;
	}

	OSReport("\nLast interrupt (%d): SRR0 = 0x%08x  TB = 0x%016llx\n",
	         __OSLastInterrupt, __OSLastInterruptSrr0, __OSLastInterruptTime);

	PPCHalt();
}
