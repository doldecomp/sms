#include <dolphin.h>
#include <dolphin/dvd.h>
#include <macros.h>

#include "__dvd.h"

static struct {
	/* 0x00 */ struct DVDCommandBlock* next;
	/* 0x04 */ struct DVDCommandBlock* prev;
} WaitingQueue[4];

static struct DVDCommandBlock* PopWaitingQueuePrio(long prio);

void __DVDClearWaitingQueue(void)
{
	unsigned long i;
	struct DVDCommandBlock* q;

	for (i = 0; i < 4; i++) {
		q       = (struct DVDCommandBlock*)&WaitingQueue[i].next;
		q->next = q;
		q->prev = q;
	}
}

int __DVDPushWaitingQueue(long prio, struct DVDCommandBlock* block)
{
	int enabled               = OSDisableInterrupts();
	struct DVDCommandBlock* q = (struct DVDCommandBlock*)&WaitingQueue[prio];

	q->prev->next = block;
	block->prev   = q->prev;
	block->next   = q;
	q->prev       = block;
	OSRestoreInterrupts(enabled);
	return 1;
}

inline static struct DVDCommandBlock* PopWaitingQueuePrio(long prio)
{
	struct DVDCommandBlock* tmp;
	int enabled;
	struct DVDCommandBlock* q;

	enabled = OSDisableInterrupts();
	q       = (struct DVDCommandBlock*)&WaitingQueue[prio];
	ASSERTLINE(0x54, q->next != q);
	tmp             = q->next;
	q->next         = tmp->next;
	tmp->next->prev = q;
	OSRestoreInterrupts(enabled);
	tmp->next = 0;
	tmp->prev = 0;
	return tmp;
}

struct DVDCommandBlock* __DVDPopWaitingQueue(void)
{
	unsigned long i;
	int enabled;
	struct DVDCommandBlock* q;

	enabled = OSDisableInterrupts();
	for (i = 0; i < 4; i++) {
		q = (struct DVDCommandBlock*)&WaitingQueue[i];
		if (q->next != q) {
			return PopWaitingQueuePrio(i);
		}
	}
	OSRestoreInterrupts(enabled);
	return NULL;
}

int __DVDCheckWaitingQueue(void)
{
	unsigned long i;
	int enabled;
	struct DVDCommandBlock* q;

	enabled = OSDisableInterrupts();
	for (i = 0; i < 4; i++) {
		q = (struct DVDCommandBlock*)&WaitingQueue[i];
		if (q->next != q) {
			return 1;
		}
	}
	OSRestoreInterrupts(enabled);
	return 0;
}

int __DVDDequeueWaitingQueue(struct DVDCommandBlock* block)
{
	int enabled;
	struct DVDCommandBlock* prev;
	struct DVDCommandBlock* next;

	enabled = OSDisableInterrupts();
	prev    = block->prev;
	next    = block->next;
	if (prev == NULL || next == NULL) {
		OSRestoreInterrupts(enabled);
		return 0;
	}
	prev->next = next;
	next->prev = prev;
	OSRestoreInterrupts(enabled);
	return 1;
}
