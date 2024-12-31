#ifndef _DOLPHIN_OSEXCEPTION_H_
#define _DOLPHIN_OSEXCEPTION_H_

#include <dolphin/os/OSContext.h>
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

// TODO: this might be wrong
typedef enum {
	OS_EXCEPTION_SYSTEM_RESET,
	OS_EXCEPTION_MACHINE_CHECK,
	OS_EXCEPTION_DSI = 2,
	OS_EXCEPTION_ISI = 3,
	OS_EXCEPTION_EXTERNAL_INTERRUPT,
	OS_EXCEPTION_ALIGNMENT = 5,
	OS_EXCEPTION_PROGRAM   = 6,
	OS_EXCEPTION_FLOATING_POINT,
	OS_EXCEPTION_DECREMENTER,
	OS_EXCEPTION_SYSTEM_CALL,
	OS_EXCEPTION_TRACE,
	OS_EXCEPTION_PERFORMANCE_MONITOR,
	OS_EXCEPTION_BREAKPOINT,
	OS_EXCEPTION_SYSTEM_INTERRUPT,
	OS_EXCEPTION_THERMAL_INTERRUPT,
	OS_EXCEPTION_MAX,
} OSException;

typedef u8 __OSException;
typedef void (*__OSExceptionHandler)(__OSException exception,
                                     OSContext* context);

__OSExceptionHandler __OSSetExceptionHandler(__OSException exception,
                                             __OSExceptionHandler handler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);

#define OS_EXCEPTION_SAVE_GPRS(context)                                        \
	stw r0, OS_CONTEXT_R0(context);                                            \
	stw r1, OS_CONTEXT_R1(context);                                            \
	stw r2, OS_CONTEXT_R2(context);                                            \
	stmw r6, OS_CONTEXT_R6(context);                                           \
	mfspr r0, GQR1;                                                            \
	stw r0, OS_CONTEXT_GQR1(context);                                          \
	mfspr r0, GQR2;                                                            \
	stw r0, OS_CONTEXT_GQR2(context);                                          \
	mfspr r0, GQR3;                                                            \
	stw r0, OS_CONTEXT_GQR3(context);                                          \
	mfspr r0, GQR4;                                                            \
	stw r0, OS_CONTEXT_GQR4(context);                                          \
	mfspr r0, GQR5;                                                            \
	stw r0, OS_CONTEXT_GQR5(context);                                          \
	mfspr r0, GQR6;                                                            \
	stw r0, OS_CONTEXT_GQR6(context);                                          \
	mfspr r0, GQR7;                                                            \
	stw r0, OS_CONTEXT_GQR7(context);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSEXCEPTION_H_
