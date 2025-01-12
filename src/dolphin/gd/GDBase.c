#include <dolphin/gd/GDBase.h>
#include <dolphin/os.h>
#include "__gd.h"

GDLObj* __GDCurrentDL          = NULL;
static GDOverflowCb overflowcb = NULL;

void GDInitGDLObj(GDLObj* dl, void* start, u32 length)
{
	dl->start  = start;
	dl->ptr    = start;
	dl->top    = (u8*)start + length;
	dl->length = length;
}

void GDFlushCurrToMem(void)
{
	DCFlushRange(__GDCurrentDL->start, __GDCurrentDL->length);
}

void GDPadCurr32(void)
{
	u32 n;

	n = (u32)__GDCurrentDL->ptr & 0x1f;
	if (((u32)__GDCurrentDL->ptr & 0x1f) != 0) {
		for (; n < 0x20; n = n + 1) {
			__GDWrite(0);
		}
	}
}

void GDOverflowed(void)
{
	if (overflowcb != NULL) {
		(*overflowcb)();
	}
}

void GDSetOverflowCallback(GDOverflowCb callback) { overflowcb = callback; }
