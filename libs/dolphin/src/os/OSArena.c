#include <dolphin.h>
#include <dolphin/os.h>
#include <macros.h>

#define ROUND(n, a) (((u32)(n) + (a) - 1) & ~((a) - 1))
#define TRUNC(n, a) (((u32)(n)) & ~((a) - 1))

static void* __OSArenaHi;
static void* __OSArenaLo = (void*)-1;

void* OSGetArenaHi()
{
	ASSERTMSGLINE(0x37, (u32)__OSArenaLo != -1,
	              "OSGetArenaHi(): OSInit() must be called in advance.");
	ASSERTMSGLINE(0x39, (u32)__OSArenaLo <= (u32)__OSArenaHi,
	              "OSGetArenaHi(): invalid arena (hi < lo).");
	return __OSArenaHi;
}

void* OSGetArenaLo()
{
	ASSERTMSGLINE(0x49, (u32)__OSArenaLo != -1,
	              "OSGetArenaLo(): OSInit() must be called in advance.");
	ASSERTMSGLINE(0x4B, (u32)__OSArenaLo <= (u32)__OSArenaHi,
	              "OSGetArenaLo(): invalid arena (hi < lo).");
	return __OSArenaLo;
}

void OSSetArenaHi(void* newHi) { __OSArenaHi = newHi; }

void OSSetArenaLo(void* newLo) { __OSArenaLo = newLo; }
