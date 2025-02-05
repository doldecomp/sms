#ifndef _THP_THPVIDEODECODE_H
#define _THP_THPVIDEODECODE_H

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL CreateVideoDecodeThread(OSPriority, void*);
void VideoDecodeThreadStart(void);
void VideoDecodeThreadCancel(void);

void* PopFreeTextureSet(void);
void* PopDecodedTextureSet(s32 flags);

void PushFreeTextureSet(void* tex);
void PushDecodedTextureSet(void* tex);

#ifdef __cplusplus
}
#endif

#endif
