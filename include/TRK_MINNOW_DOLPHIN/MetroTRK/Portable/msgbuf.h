#ifndef METROTRK_PORTABLE_MSGBUF_H
#define METROTRK_PORTABLE_MSGBUF_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"

#ifdef __cplusplus
extern "C" {
#endif

DSError TRKSetBufferPosition(TRKBuffer* msg, u32 pos);
void* TRKGetBuffer(int);
void TRKResetBuffer(TRKBuffer* msg, BOOL keepData);
void* TRKGetBuffer(int idx);
DSError TRKGetFreeBuffer(int* msgID, TRKBuffer** outMsg);

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_MSGBUF_H */
