#ifndef OS_DOLPHIN_DOLPHIN_TRK_GLUE_H
#define OS_DOLPHIN_DOLPHIN_TRK_GLUE_H

#include "dolphin/os.h"
#include "PowerPC_EABI_Support/MetroTRK/trk.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*DBCommFunc)();
typedef int (*DBCommInitFunc)(void*, __OSInterruptHandler);
typedef int (*DBCommReadFunc)(u8*, int);
typedef int (*DBCommWriteFunc)(const u8*, int);

typedef struct DBCommTable {
	DBCommInitFunc initialize_func;
	DBCommFunc init_interrupts_func;
	DBCommFunc shutdown_func;
	DBCommFunc peek_func;
	DBCommReadFunc read_func;
	DBCommWriteFunc write_func;
	DBCommFunc open_func;
	DBCommFunc close_func;
	DBCommFunc pre_continue_func;
	DBCommFunc post_stop_func;
} DBCommTable;

DSError InitializeProgramEndTrap(void);
DSError TRKInitializeIntDrivenUART(u32 param_0, u32 param_1, u32 param_2,
                                   void* param_3);

void UnreserveEXI2Port(void);
void ReserveEXI2Port(void);
int TRKPollUART(void);
UARTError TRKReadUARTN(void* bytes, u32 length);
UARTError TRKWriteUARTN(const void* bytes, u32 length);
void TRKLoadContext(OSContext* ctx, u32 r4);
int InitMetroTRKCommTable(int hwId);
void EnableEXI2Interrupts(void);
void TRK_board_display(char* str);

#ifdef __cplusplus
}
#endif

#endif /* OS_DOLPHIN_DOLPHIN_TRK_GLUE_H */
