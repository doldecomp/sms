#include <dolphin.h>
#include <dolphin/base/PPCArch.h>
#include <dolphin/os.h>
#include <macros.h>

// internal header
#include "__os.h"

struct OSAlarmQueue {
	struct OSAlarm* head;
	struct OSAlarm* tail;
};

static struct OSAlarmQueue AlarmQueue;

// functions
static void SetTimer(struct OSAlarm* alarm);
static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler);
static void DecrementerExceptionCallback(register __OSException exception,
                                         register OSContext* context);
static void DecrementerExceptionHandler(__OSException exception,
                                        OSContext* context);

static void SetTimer(struct OSAlarm* alarm)
{
	OSTime delta = alarm->fire - __OSGetSystemTime();

	if (delta < 0) {
		PPCMtdec(0);
	} else if (delta < 0x80000000) {
		PPCMtdec((u32)delta);
	} else {
		PPCMtdec(0x7fffffff);
	}
}

void OSInitAlarm(void)
{
	if (__OSGetExceptionHandler(8) != DecrementerExceptionHandler) {
		AlarmQueue.head = AlarmQueue.tail = NULL;
		__OSSetExceptionHandler(8, DecrementerExceptionHandler);
	}
}

void OSCreateAlarm(OSAlarm* alarm) { alarm->handler = 0; }

static void InsertAlarm(OSAlarm* alarm, OSTime fire, OSAlarmHandler handler)
{
	OSAlarm* next;
	OSAlarm* prev;

	if (0 < alarm->period) {
		OSTime time = __OSGetSystemTime();

		fire = alarm->start;
		if (alarm->start < time) {
			fire += alarm->period * ((time - alarm->start) / alarm->period + 1);
		}
	}

	alarm->handler = handler;
	alarm->fire    = fire;

	for (next = AlarmQueue.head; next; next = next->next) {
		if (next->fire <= fire) {
			continue;
		}

		alarm->prev = next->prev;
		next->prev  = alarm;
		alarm->next = next;
		prev        = alarm->prev;

		if (prev) {
			prev->next = alarm;
		} else {
			AlarmQueue.head = alarm;
			SetTimer(alarm);
		}

		return;
	}

	alarm->next     = 0;
	prev            = AlarmQueue.tail;
	AlarmQueue.tail = alarm;
	alarm->prev     = prev;

	if (prev) {
		prev->next = alarm;
	} else {
		AlarmQueue.head = AlarmQueue.tail = alarm;
		SetTimer(alarm);
	}
}

void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler)
{
	BOOL enabled;
	enabled       = OSDisableInterrupts();
	alarm->period = 0;
	InsertAlarm(alarm, __OSGetSystemTime() + tick, handler);
	OSRestoreInterrupts(enabled);
}

void OSCancelAlarm(OSAlarm* alarm)
{
	OSAlarm* next;
	BOOL enabled;

	enabled = OSDisableInterrupts();

	if (alarm->handler == 0) {
		OSRestoreInterrupts(enabled);
		return;
	}

	next = alarm->next;
	if (next == 0) {
		AlarmQueue.tail = alarm->prev;
	} else {
		next->prev = alarm->prev;
	}
	if (alarm->prev) {
		alarm->prev->next = next;
	} else {
		AlarmQueue.head = next;
		if (next) {
			SetTimer(next);
		}
	}
	alarm->handler = 0;
	OSRestoreInterrupts(enabled);
}

static void DecrementerExceptionCallback(register __OSException exception,
                                         register OSContext* context)
{
	OSAlarm* alarm;
	OSAlarm* next;
	OSAlarmHandler handler;
	OSTime time;
	OSContext tmpCtx;

	time  = __OSGetSystemTime();
	alarm = AlarmQueue.head;
	if (alarm == 0) {
		OSLoadContext(context);
	}

	if (time < alarm->fire) {
		SetTimer(alarm);
		OSLoadContext(context);
	}

	next            = alarm->next;
	AlarmQueue.head = next;
	if (next == 0) {
		AlarmQueue.tail = 0;
	} else {
		next->prev = 0;
	}
	handler        = alarm->handler;
	alarm->handler = 0;
	if (0 < alarm->period) {
		InsertAlarm(alarm, 0, handler);
	}

	if (AlarmQueue.head) {
		SetTimer(AlarmQueue.head);
	}

	OSDisableScheduler();
	OSClearContext(&tmpCtx);
	OSSetCurrentContext(&tmpCtx);
	handler(alarm, context);
	OSClearContext(&tmpCtx);
	OSSetCurrentContext(context);
	OSEnableScheduler();
	__OSReschedule();
	OSLoadContext(context);
}

static asm void DecrementerExceptionHandler(register __OSException exception,
                                            register OSContext* context)
{
#ifdef __MWERKS__ // clang-format off
	nofralloc
	OS_EXCEPTION_SAVE_GPRS(context)
	b DecrementerExceptionCallback
#endif // clang-format on
}
