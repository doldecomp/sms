#include <dolphin.h>
#include <dolphin/base/PPCArch.h>
#include <dolphin/os.h>
#include <macros.h>

#include "__os.h"

#define ENQUEUE_THREAD(thread, queue, link)                                    \
	do {                                                                       \
		OSThread* __prev = (queue)->tail;                                      \
		if (__prev == NULL) {                                                  \
			(queue)->head = (thread);                                          \
		} else {                                                               \
			__prev->link.next = (thread);                                      \
		}                                                                      \
		(thread)->link.prev = __prev;                                          \
		(thread)->link.next = 0;                                               \
		(queue)->tail       = (thread);                                        \
	} while (0);

#define DEQUEUE_THREAD(thread, queue, link)                                    \
	do {                                                                       \
		OSThread* __next = (thread)->link.next;                                \
		OSThread* __prev = (thread)->link.prev;                                \
		if (__next == NULL) {                                                  \
			(queue)->tail = __prev;                                            \
		} else {                                                               \
			__next->link.prev = __prev;                                        \
		}                                                                      \
		if (__prev == NULL) {                                                  \
			(queue)->head = __next;                                            \
		} else {                                                               \
			__prev->link.next = __next;                                        \
		}                                                                      \
	} while (0);

#define ENQUEUE_THREAD_PRIO(thread, queue, link)                               \
	do {                                                                       \
		OSThread* __prev;                                                      \
		OSThread* __next;                                                      \
		for (__next = (queue)->head;                                           \
		     __next && (__next->priority <= (thread)->priority);               \
		     __next = __next->link.next)                                       \
			;                                                                  \
                                                                               \
		if (__next == NULL) {                                                  \
			ENQUEUE_THREAD(thread, queue, link);                               \
		} else {                                                               \
			(thread)->link.next = __next;                                      \
			__prev              = __next->link.prev;                           \
			__next->link.prev   = (thread);                                    \
			(thread)->link.prev = __prev;                                      \
			if (__prev == NULL) {                                              \
				(queue)->head = (thread);                                      \
			} else {                                                           \
				__prev->link.next = (thread);                                  \
			}                                                                  \
		}                                                                      \
	} while (0);

#define DEQUEUE_HEAD(thread, queue, link)                                      \
	do {                                                                       \
		OSThread* __next = thread->link.next;                                  \
		if (__next == NULL) {                                                  \
			(queue)->tail = 0;                                                 \
		} else {                                                               \
			__next->link.prev = 0;                                             \
		}                                                                      \
		(queue)->head = __next;                                                \
	} while (0);

// which header should these go in?
extern unsigned char _stack_end[];
extern unsigned char _stack_addr[];

// .bss
static OSThreadQueue RunQueue[32];
static OSThread IdleThread;
static OSThread DefaultThread;
static struct OSContext IdleContext;
static volatile unsigned long RunQueueBits;
static volatile int RunQueueHint;
static long Reschedule;

#define ALIGN4(val) (((val) + 0x3) & ~0x3)
#define ALIGN8(val) (((val) + 0x7) & ~0x7)

// functions
static void OSInitMutexQueue(struct OSMutexQueue* queue);
static void __OSSwitchThread(OSThread* nextThread);
static int __OSIsThreadActive(OSThread* thread);
static void SetRun(OSThread* thread);
static void UnsetRun(OSThread* thread);
static OSThread* SetEffectivePriority(OSThread* thread, long priority);
static void UpdatePriority(OSThread* thread);
static OSThread* SelectThread(int yield);

void __OSThreadInit()
{
	OSThread* thread = &DefaultThread;
	int prio;

	thread->state    = 2;
	thread->attr     = 1;
	thread->priority = thread->base = 0x10;
	thread->suspend                 = 0;
	thread->val                     = (void*)-1; // wut
	thread->mutex                   = 0;

	OSInitThreadQueue(&thread->queueJoin);
#ifdef DEBUG
	OSInitMutexQueue(&thread->queueMutex);
#else
	thread->queueMutex.head = thread->queueMutex.tail
	    = 0; // it got inlined? cant reproduce the inline...
#endif

	ASSERTLINE(282, PPCMfmsr() & MSR_FP);

	__gUnkThread1 = thread;
	OSClearContext(&thread->context);
	OSSetCurrentContext(&thread->context);
	thread->stackBase       = (unsigned char*)&_stack_addr;
	thread->stackEnd        = (unsigned long*)&_stack_end;
	*(u32*)thread->stackEnd = 0xDEADBABE;
	__gCurrentThread        = thread;
	RunQueueBits            = 0;
	RunQueueHint            = 0;

	for (prio = 0; prio <= 31; prio++) {
		OSInitThreadQueue(&RunQueue[prio]);
	}
	OSInitThreadQueue(&__OSActiveThreadQueue);

	ENQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);

	OSClearContext(&IdleContext);
	Reschedule = 0;
}

#if DEBUG
static void OSInitMutexQueue(struct OSMutexQueue* queue)
{
	queue->head = queue->tail = 0;
}
#endif

void OSInitThreadQueue(OSThreadQueue* queue) { queue->head = queue->tail = 0; }

OSThread* OSGetCurrentThread() { return __gCurrentThread; }

static void __OSSwitchThread(OSThread* nextThread)
{
	__gCurrentThread = nextThread;
	OSSetCurrentContext(&nextThread->context);
	OSLoadContext(&nextThread->context);
}

int OSIsThreadTerminated(OSThread* thread)
{
	return (thread->state == 8 || thread->state == 0) ? 1 : 0;
}

static int __OSIsThreadActive(OSThread* thread)
{
	OSThread* active;

	if (thread->state == 0) {
		return 0;
	}

	for (active = __OSActiveThreadQueue.head; active;
	     active = active->linkActive.next) {
		if (thread == active) {
			return 1;
		}
	}
	return 0;
}

s32 OSDisableScheduler(void)
{
	register int enabled;
	long count;

	enabled    = OSDisableInterrupts();
	count      = Reschedule;
	Reschedule = count + 1;
	OSRestoreInterrupts(enabled);
	return count;
}

s32 OSEnableScheduler(void)
{
	register int enabled;
	long count;

	enabled    = OSDisableInterrupts();
	count      = Reschedule;
	Reschedule = count - 1;
	OSRestoreInterrupts(enabled);
	return count;
}

static void SetRun(OSThread* thread)
{
	ASSERTLINE(469, !IsSuspended(thread->suspend));
	ASSERTLINE(470, thread->state == OS_THREAD_STATE_READY);

	ASSERTLINE(472, OS_PRIORITY_MIN <= thread->priority
	                    && thread->priority <= OS_PRIORITY_MAX);

	thread->queue = &RunQueue[thread->priority];

	ENQUEUE_THREAD(thread, thread->queue, link);

	RunQueueBits |= 1 << (0x1F - thread->priority);
	RunQueueHint = 1;
}

static void UnsetRun(OSThread* thread)
{
	OSThreadQueue* queue;

	ASSERTLINE(0x1ED, thread->state == OS_THREAD_STATE_READY);

	ASSERTLINE(0x1EF, OS_PRIORITY_MIN <= thread->priority
	                      && thread->priority <= OS_PRIORITY_MAX);
	ASSERTLINE(0x1F0, thread->queue == &RunQueue[thread->priority]);

	queue = thread->queue;

	DEQUEUE_THREAD(thread, queue, link);

	if (!queue->head) {
		RunQueueBits &= ~(1 << (0x1F - thread->priority));
	}
	thread->queue = NULL;
}

long __OSGetEffectivePriority(OSThread* thread)
{
	long priority = thread->base;
	struct OSMutex* mutex;

	for (mutex = thread->queueMutex.head; mutex; mutex = mutex->link.next) {
		OSThread* blocked = mutex->queue.head;
		if (blocked && blocked->priority < priority) {
			priority = blocked->priority;
		}
	}
	return priority;
}

static OSThread* SetEffectivePriority(OSThread* thread, long priority)
{
	ASSERTLINE(547, !IsSuspended(thread->suspend));

	switch (thread->state) {
	case 1:
		UnsetRun(thread);
		thread->priority = priority;
		SetRun(thread);
		break;
	case 4:
		DEQUEUE_THREAD(thread, thread->queue, link);
		thread->priority = priority;

		ENQUEUE_THREAD_PRIO(thread, thread->queue, link);

		if (thread->mutex) {
			ASSERTLINE(0x232, thread->mutex->thread);
			return thread->mutex->thread;
		}
		break;
	case 2:
		RunQueueHint     = 1;
		thread->priority = priority;
		break;
	}
	return 0;
}

static void UpdatePriority(OSThread* thread)
{
	long priority;

	while (1) {
		if (thread->suspend > 0) {
			break;
		}
		priority = __OSGetEffectivePriority(thread);
		if (thread->priority == priority) {
			break;
		}
		thread = SetEffectivePriority(thread, priority);
		if (thread == 0) {
			break;
		}
	}
}

void __OSPromoteThread(OSThread* thread, long priority)
{
	while (1) {
		if (thread->suspend > 0 || thread->priority <= priority) {
			break;
		}
		thread = SetEffectivePriority(thread, priority);
		if (thread == 0) {
			break;
		}
	}
}

static OSThread* SelectThread(int yield)
{
	struct OSContext* currentContext;
	OSThread* currentThread;
	OSThread* nextThread;
	long priority;
	OSThreadQueue* queue;

	if (Reschedule > 0) {
		return NULL;
	}

	currentContext = OSGetCurrentContext();
	currentThread  = OSGetCurrentThread();

	if (currentContext != &currentThread->context) {
		return NULL;
	}

	if (currentThread) {
		if (currentThread->state == 2) {
			if (yield == 0) {
				priority = __cntlzw(RunQueueBits);
				if (currentThread->priority <= priority)
					return NULL;
			}
			currentThread->state = 1;
			SetRun(currentThread);
		}
		if (!(currentThread->context.state & 2)
		    && (OSSaveContext(&currentThread->context) != 0)) {
			return NULL;
		}
	}

	__gCurrentThread = 0;

	if (RunQueueBits == 0) {
		OSSetCurrentContext(&IdleContext);
		do {
			OSEnableInterrupts();
			while (RunQueueBits == 0)
				;
			OSDisableInterrupts();
		} while (RunQueueBits == 0);
		OSClearContext(&IdleContext);
	}

	RunQueueHint = 0;
	priority     = __cntlzw(RunQueueBits);

	ASSERTLINE(0x2C6,
	           OS_PRIORITY_MIN <= priority && priority <= OS_PRIORITY_MAX);

	queue      = &RunQueue[priority];
	nextThread = queue->head;

	DEQUEUE_HEAD(nextThread, queue, link);

	ASSERTLINE(0x2C9, nextThread->priority == priority);

	if (!queue->head) {
		RunQueueBits &= ~(1 << (0x1F - priority));
	}
	nextThread->queue = 0;
	nextThread->state = 2;
	__OSSwitchThread(nextThread);
	return nextThread;
}

void __OSReschedule(void)
{
	if (RunQueueHint != 0) {
		SelectThread(0);
	}
}

void OSYieldThread(void)
{
	int enabled = OSDisableInterrupts();

	SelectThread(1);
	OSRestoreInterrupts(enabled);
}

int OSCreateThread(OSThread* thread, void* (*func)(void*), void* param,
                   void* stack, unsigned long stackSize, long priority,
                   unsigned short attr)
{
	int enabled;
	unsigned long sp;

	ASSERTMSGLINE(
	    0x31C, ((priority >= 0) && (priority <= 0x1F)),
	    "OSCreateThread(): priority out of range (0 <= priority <= 31).");

	// why check this for an assert just to check it again right after?
	if ((priority < 0) || (priority > 0x1F)) {
		return 0;
	}

	thread->state    = 1;
	thread->attr     = attr & 1U;
	thread->base     = priority;
	thread->priority = priority;
	thread->suspend  = 1;
	thread->val      = (void*)-1;
	thread->mutex    = 0;
	OSInitThreadQueue(&thread->queueJoin);
#ifdef DEBUG
	OSInitMutexQueue(&thread->queueMutex);
#else
	OSInitThreadQueue((void*)&thread->queueMutex); // why
#endif
	sp = (u32)stack;
	sp &= ~7;
	sp -= 8;
	((u32*)sp)[0] = 0;
	((u32*)sp)[1] = 0;
	OSInitContext(&thread->context, (u32)func, sp);
	thread->context.lr     = (unsigned long)&OSExitThread;
	thread->context.gpr[3] = (unsigned long)param;
	thread->stackBase      = stack;
	thread->stackEnd       = (void*)((unsigned int)stack - stackSize);
	*thread->stackEnd      = 0xDEADBABE;
	enabled                = OSDisableInterrupts();

	ASSERTMSG1LINE(0x33B, __OSIsThreadActive(thread) == 0L,
	               "OSCreateThread(): thread %p is still active.", thread);

	ENQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);

	OSRestoreInterrupts(enabled);
	return 1;
}

void OSExitThread(OSThread* val)
{
	int enabled             = OSDisableInterrupts();
	OSThread* currentThread = OSGetCurrentThread();

	ASSERTMSGLINE(0x354, currentThread,
	              "OSExitThread(): current thread does not exist.");
	ASSERTMSGLINE(0x356, currentThread->state == 2,
	              "OSExitThread(): current thread is not running.");
	ASSERTMSGLINE(0x358, __OSIsThreadActive(currentThread) != 0,
	              "OSExitThread(): current thread is not active.");

	OSClearContext(&currentThread->context);
	if (currentThread->attr & 1) {
		DEQUEUE_THREAD(currentThread, &__OSActiveThreadQueue, linkActive);
		currentThread->state = 0;
	} else {
		currentThread->state = 8;
		currentThread->val   = val;
	}
	__OSUnlockAllMutex(currentThread);
	OSWakeupThread(&currentThread->queueJoin);
	RunQueueHint = 1;
#ifdef DEBUG
	__OSReschedule();
#else
	if (RunQueueHint != 0) {
		SelectThread(0);
	}
#endif
	OSRestoreInterrupts(enabled);
}

void OSCancelThread(OSThread* thread)
{
	int enabled = OSDisableInterrupts();

	ASSERTMSG1LINE(0x37E, __OSIsThreadActive(thread) != 0,
	               "OSExitThread(): thread %p is not active.", thread);

	switch (thread->state) {
	case 1:
		if (thread->suspend <= 0) {
			UnsetRun(thread);
		}
		break;
	case 2:
		RunQueueHint = 1;
		break;
	case 4:
		DEQUEUE_THREAD(thread, thread->queue, link);
		thread->queue = 0;
		if ((thread->suspend <= 0) && (thread->mutex)) {
			ASSERTLINE(0x391, thread->mutex->thread);
			UpdatePriority(thread->mutex->thread);
		}
		break;
	default:
		OSRestoreInterrupts(enabled);
		return;
	}
	OSClearContext(&thread->context);
	if (thread->attr & 1) {
		DEQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);
		thread->state = 0;
	} else {
		thread->state = 8;
	}
	__OSUnlockAllMutex(thread);
	OSWakeupThread(&thread->queueJoin);
	__OSReschedule();
	OSRestoreInterrupts(enabled);
}

int OSJoinThread(OSThread* thread, void* val)
{
	int enabled = OSDisableInterrupts();

	ASSERTMSG1LINE(0x3CA, __OSIsThreadActive(thread) != 0,
	               "OSJoinThread(): thread %p is not active.", thread);

	if (!(thread->attr & 1) && (thread->state != 8)
	    && (thread->queueJoin.head == NULL)) {
		OSSleepThread(&thread->queueJoin);
		if (__OSIsThreadActive(thread) == 0) {
			OSRestoreInterrupts(enabled);
			return 0;
		}
	}
	if (thread->state == 8) {
		if (val) {
			*(s32*)val = (s32)thread->val;
		}
		DEQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);
		thread->state = 0;
		OSRestoreInterrupts(enabled);
		return 1;
	}
	OSRestoreInterrupts(enabled);
	return 0;
}

void OSDetachThread(OSThread* thread)
{
	int enabled = OSDisableInterrupts();

	ASSERTMSG1LINE(0x3FC, __OSIsThreadActive(thread) != 0,
	               "OSDetachThread(): thread %p is not active.", thread);

	thread->attr |= 1;
	if (thread->state == 8) {
		DEQUEUE_THREAD(thread, &__OSActiveThreadQueue, linkActive);
		thread->state = 0;
	}
	OSWakeupThread(&thread->queueJoin);
	OSRestoreInterrupts(enabled);
}

long OSResumeThread(OSThread* thread)
{
	int enabled = OSDisableInterrupts();
	long suspendCount;

	ASSERTMSG1LINE(0x419, __OSIsThreadActive(thread) != 0,
	               "OSResumeThread(): thread %p is not active.", thread);
	ASSERTMSG1LINE(0x41B, thread->state != 8,
	               "OSResumeThread(): thread %p is terminated.", thread);

	suspendCount = thread->suspend--;
	if (thread->suspend < 0) {
		thread->suspend = 0;
	} else if (thread->suspend == 0) {
		switch (thread->state) {
		case 1:
			thread->priority = __OSGetEffectivePriority(thread);
			SetRun(thread);
			break;
		case 4:
			ASSERTLINE(0x42A, thread->queue);
			DEQUEUE_THREAD(thread, thread->queue, link);
			thread->priority = __OSGetEffectivePriority(thread);
			ENQUEUE_THREAD_PRIO(thread, thread->queue, link);
			if (thread->mutex) {
				UpdatePriority(thread->mutex->thread);
			}
		}
		__OSReschedule();
	}
	OSRestoreInterrupts(enabled);
	return suspendCount;
}

long OSSuspendThread(OSThread* thread)
{
	int enabled = OSDisableInterrupts();
	long suspendCount;

	ASSERTMSG1LINE(0x44C, __OSIsThreadActive(thread) != 0,
	               "OSSuspendThread(): thread %p is not active.", thread);
	ASSERTMSG1LINE(0x44E, thread->state != 8,
	               "OSSuspendThread(): thread %p is terminated.", thread);

	suspendCount = thread->suspend++;
	if (suspendCount == 0) {
		switch (thread->state) {
		case 2:
			RunQueueHint  = 1;
			thread->state = 1;
			break;
		case 1:
			UnsetRun(thread);
			break;
		case 4:
			DEQUEUE_THREAD(thread, thread->queue, link);
			thread->priority = 0x20;
			ENQUEUE_THREAD(thread, thread->queue, link);
			if (thread->mutex) {
				ASSERTLINE(0x463, thread->mutex->thread);
				UpdatePriority(thread->mutex->thread);
			}
			break;
		}
		__OSReschedule();
	}
	OSRestoreInterrupts(enabled);
	return suspendCount;
}

void OSSleepThread(OSThreadQueue* queue)
{
	int enabled             = OSDisableInterrupts();
	OSThread* currentThread = OSGetCurrentThread();

	ASSERTMSGLINE(0x484, currentThread,
	              "OSSleepThread(): current thread does not exist.");
	ASSERTMSG1LINE(0x486, __OSIsThreadActive(currentThread) != 0,
	               "OSSleepThread(): current thread %p is not active.",
	               currentThread);
	ASSERTMSG1LINE(0x488, currentThread->state == 2,
	               "OSSleepThread(): current thread %p is not running.",
	               currentThread);
	ASSERTMSG1LINE(0x48A, currentThread->suspend <= 0,
	               "OSSleepThread(): current thread %p is suspended.",
	               currentThread);

	currentThread->state = 4;
	currentThread->queue = queue;
	ENQUEUE_THREAD_PRIO(currentThread, queue, link);
	RunQueueHint = 1;
	__OSReschedule();
	OSRestoreInterrupts(enabled);
}

void OSWakeupThread(OSThreadQueue* queue)
{
	int enabled = OSDisableInterrupts();

	while (queue->head) {
		OSThread* thread = queue->head;

		DEQUEUE_HEAD(thread, queue, link);

		ASSERTLINE(0x4A7, __OSIsThreadActive(thread));
		ASSERTLINE(0x4A8, thread->state != OS_THREAD_STATE_MORIBUND);
		ASSERTLINE(0x4A9, thread->queue == queue);
		thread->state = 1;
		if (thread->suspend <= 0) {
			SetRun(thread);
		}
	}
	__OSReschedule();
	OSRestoreInterrupts(enabled);
}

long OSGetThreadPriority(OSThread* thread) { return thread->base; }
