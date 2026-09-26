#ifndef _DOLPHIN_ARQ_H_
#define _DOLPHIN_ARQ_H_

#include <dolphin/ar.h>

#define ARQ_DMA_ALIGNMENT 32

#define ARQ_TYPE_MRAM_TO_ARAM ARAM_DIR_MRAM_TO_ARAM
#define ARQ_TYPE_ARAM_TO_MRAM ARAM_DIR_ARAM_TO_MRAM

#define ARQ_PRIORITY_LOW  0
#define ARQ_PRIORITY_HIGH 1

typedef void (*ARQCallback)(u32 pointerToARQRequest);

struct ARQRequest {
	/* 0x00 */ struct ARQRequest* next;
	/* 0x04 */ u32 owner;
	/* 0x08 */ u32 type;
	/* 0x0C */ u32 priority;
	/* 0x10 */ u32 source;
	/* 0x14 */ u32 dest;
	/* 0x18 */ u32 length;
	/* 0x1C */ ARQCallback callback;
};

typedef struct ARQRequest ARQRequest;

#ifdef __cplusplus
extern "C" {
#endif

void ARQInit(void);
void ARQReset(void);
void ARQPostRequest(struct ARQRequest* request, u32 owner, u32 type,
                    u32 priority, u32 source, u32 dest, u32 length,
                    ARQCallback callback);
void ARQRemoveRequest(struct ARQRequest* request);
void ARQRemoveOwnerRequest(u32 owner);
void ARQFlushQueue(void);
void ARQSetChunkSize(u32 size);
u32 ARQGetChunkSize(void);

#ifdef __cplusplus
}
#endif

#endif
