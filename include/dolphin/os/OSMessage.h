#ifndef _DOLPHIN_OSMESSAGE_H_
#define _DOLPHIN_OSMESSAGE_H_

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSMessageQueue OSMessageQueue;
typedef void* OSMessage;

struct OSMessageQueue {
	struct OSThreadQueue queueSend;
	struct OSThreadQueue queueReceive;
	void* msgArray;
	long msgCount;
	long firstIndex;
	long usedCount;
};

#define OS_MESSAGE_NOBLOCK 0
#define OS_MESSAGE_BLOCK   1

void OSInitMessageQueue(struct OSMessageQueue* mq, void* msgArray,
                        long msgCount);
int OSSendMessage(struct OSMessageQueue* mq, void* msg, long flags);
int OSReceiveMessage(struct OSMessageQueue* mq, void* msg, long flags);
int OSJamMessage(struct OSMessageQueue* mq, void* msg, long flags);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSMESSAGE_H_
