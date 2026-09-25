#ifndef _THP_THPREAD_H
#define _THP_THPREAD_H

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL CreateReadThread(OSPriority prio);
void ReadThreadStart(void);
void ReadThreadCancel(void);

// TODO: figure out if these returns THPBuffer * instead(DWARF info pls)
void* PopReadedBuffer(void);
void* PopFreeReadBuffer(void);
void* PopReadedBuffer2(void);
void* PushReadedBuffer(void* buffer);
void PushFreeReadBuffer(void* buffer);
void PushReadedBuffer2(void* buffer);

#ifdef __cplusplus
}
#endif

#endif
