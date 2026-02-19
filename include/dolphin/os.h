#ifndef _DOLPHIN_OS_H_
#define _DOLPHIN_OS_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <dolphin/os/OSAlarm.h>
#include <dolphin/os/OSAlloc.h>
#include <dolphin/os/OSCache.h>
#include <dolphin/os/OSContext.h>
#include <dolphin/os/OSDC.h>
#include <dolphin/os/OSError.h>
#include <dolphin/os/OSException.h>
#include <dolphin/os/OSFont.h>
#include <dolphin/os/OSIC.h>
#include <dolphin/os/OSInterrupt.h>
#include <dolphin/os/OSL2.h>
#include <dolphin/os/OSLC.h>
#include <dolphin/os/OSMessage.h>
#include <dolphin/os/OSModule.h>
#include <dolphin/os/OSMutex.h>
#include <dolphin/os/OSReset.h>
#include <dolphin/os/OSResetSW.h>
#include <dolphin/os/OSRtc.h>
#include <dolphin/os/OSSerial.h>
#include <dolphin/os/OSStopwatch.h>
#include <dolphin/os/OSThread.h>
#include <dolphin/os/OSTime.h>
#include <dolphin/os/OSMemory.h>

// private macro, maybe shouldn't be defined here?
#define OFFSET(addr, align) (((u32)(addr) & ((align) - 1)))

u32 OSGetPhysicalMemSize(void);
u32 OSGetConsoleSimulatedMemSize(void);

// Upper words of the masks, since UIMM is only 16 bits
#define OS_CACHED_REGION_PREFIX   0x8000
#define OS_UNCACHED_REGION_PREFIX 0xC000
#define OS_PHYSICAL_MASK          0x3FFF

#define OS_BASE_CACHED   (OS_CACHED_REGION_PREFIX << 16)
#define OS_BASE_UNCACHED (OS_UNCACHED_REGION_PREFIX << 16)

typedef struct BI2Debug {
	/* 0x00 */ s32 debugMonSize;
	/* 0x04 */ s32 simMemSize;
	/* 0x08 */ u32 argOffset;
	/* 0x0C */ u32 debugFlag;
	/* 0x10 */ int trackLocation;
	/* 0x14 */ int trackSize;
	/* 0x18 */ u32 countryCode;
	/* 0x1C */ u8 unk[8];
	/* 0x24 */ u32 padSpec;
} BI2Debug;

u32 __OSPhysicalMemSize AT_ADDRESS(OS_BASE_CACHED | 0x0028);
volatile u32 __OSTVMode AT_ADDRESS(OS_BASE_CACHED | 0x00CC);
OSThread* __gUnkThread1 AT_ADDRESS(OS_BASE_CACHED | 0x00D8);
OSThreadQueue __OSActiveThreadQueue AT_ADDRESS(OS_BASE_CACHED | 0x00DC);
OSThread* __gCurrentThread AT_ADDRESS(OS_BASE_CACHED | 0x00E4);
u32 __OSSimulatedMemSize AT_ADDRESS(OS_BASE_CACHED | 0x00F0);
u32 __OSBusClock AT_ADDRESS(OS_BASE_CACHED | 0x00F8);
u32 __OSCoreClock AT_ADDRESS(OS_BASE_CACHED | 0x00FC);
unsigned int __gUnknown800030C0[2] AT_ADDRESS(OS_BASE_CACHED | 0x30C0);

#define OS_BUS_CLOCK   __OSBusClock
#define OS_CORE_CLOCK  __OSCoreClock
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)

#define OSTicksToSeconds(ticks)      ((ticks) / (OS_TIMER_CLOCK))
#define OSTicksToMilliseconds(ticks) ((ticks) / (OS_TIMER_CLOCK / 1000))
#define OSTicksToMicroseconds(ticks) ((ticks) * 8 / (OS_TIMER_CLOCK / 125000))
#define OSSecondsToTicks(sec)        ((sec) * (OS_TIMER_CLOCK))
#define OSMillisecondsToTicks(msec)  ((msec) * (OS_TIMER_CLOCK / 1000))
#define OSNanosecondsToTicks(nsec)   (((nsec) * (OS_TIMER_CLOCK / 125000)) / 8000)
#define OSMicrosecondsToTicks(usec)  (((usec) * (OS_TIMER_CLOCK / 125000)) / 8)

unsigned long OSGetConsoleType(void);
void OSInit(void);

void* OSGetArenaHi(void);
void* OSGetArenaLo(void);
void OSSetArenaHi(void*);
void OSSetArenaLo(void*);
void* OSAllocFromArenaLo(u32 size, u32 align);
void* OSAllocFromArenaHi(u32 size, u32 align);

u32 OSGetPhysicalMemSize(void);

void __OSPSInit();

typedef struct OSCalendarTime {
	/*0x00*/ int sec;
	/*0x04*/ int min;
	/*0x08*/ int hour;
	/*0x0C*/ int mday;
	/*0x10*/ int mon;
	/*0x14*/ int year;
	/*0x18*/ int wday;
	/*0x1C*/ int yday;
	/*0x20*/ int msec;
	/*0x24*/ int usec;
} OSCalendarTime;

#include <dolphin/dvd.h>

typedef struct OSBootInfo_s {
	// total size: 0x40
	DVDDiskID DVDDiskID;        // offset 0x0, size 0x20
	unsigned long magic;        // offset 0x20, size 0x4
	unsigned long version;      // offset 0x24, size 0x4
	unsigned long memorySize;   // offset 0x28, size 0x4
	unsigned long consoleType;  // offset 0x2C, size 0x4
	void* arenaLo;              // offset 0x30, size 0x4
	void* arenaHi;              // offset 0x34, size 0x4
	void* FSTLocation;          // offset 0x38, size 0x4
	unsigned long FSTMaxLength; // offset 0x3C, size 0x4
} OSBootInfo;

OSTick OSGetTick(void);
OSTime OSGetTime(void);
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);
BOOL OSEnableInterrupts(void);
BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);

#define OS_CONSOLE_MASK        0xf0000000
#define OS_CONSOLE_RETAIL      0x00000000
#define OS_CONSOLE_DEVELOPMENT 0x10000000
#define OS_CONSOLE_TDEV        0x20000000

#define OS_CONSOLE_RETAIL4     0x00000004
#define OS_CONSOLE_RETAIL3     0x00000003
#define OS_CONSOLE_RETAIL2     0x00000002
#define OS_CONSOLE_RETAIL1     0x00000001
#define OS_CONSOLE_TDEVHW4     0x20000007
#define OS_CONSOLE_TDEVHW3     0x20000006
#define OS_CONSOLE_TDEVHW2     0x20000005
#define OS_CONSOLE_TDEVHW1     0x20000004
#define OS_CONSOLE_DEVHW4      0x10000007
#define OS_CONSOLE_DEVHW3      0x10000006
#define OS_CONSOLE_DEVHW2      0x10000005
#define OS_CONSOLE_DEVHW1      0x10000004
#define OS_CONSOLE_MINNOW      0x10000003
#define OS_CONSOLE_ARTHUR      0x10000002
#define OS_CONSOLE_PC_EMULATOR 0x10000001
#define OS_CONSOLE_EMULATOR    0x10000000

#define OS_SOUND_MODE_MONO   0
#define OS_SOUND_MODE_STEREO 1

u32 OSGetSoundMode(void);
void OSSetSoundMode(u32 mode);

void OSReport(char*, ...);
void OSPanic(char* file, int line, char* msg, ...);

#define OSErrorLine(line, ...) OSPanic(__FILE__, line, __VA_ARGS__)

#define OSRoundUp32B(x)   (((u32)(x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32)(x)) & ~(32 - 1))

void* OSPhysicalToCached(u32 paddr);
void* OSPhysicalToUncached(u32 paddr);
u32 OSCachedToPhysical(void* caddr);
u32 OSUncachedToPhysical(void* ucaddr);
void* OSCachedToUncached(void* caddr);
void* OSUncachedToCached(void* ucaddr);
#if !DEBUG
#define OSPhysicalToCached(paddr)                                              \
	((void*)((u32)(OS_BASE_CACHED + (u32)(paddr))))
#define OSPhysicalToUncached(paddr)                                            \
	((void*)((u32)(OS_BASE_UNCACHED + (u32)(paddr))))
#define OSCachedToPhysical(caddr)    ((u32)((u32)(caddr) - OS_BASE_CACHED))
#define OSUncachedToPhysical(ucaddr) ((u32)((u32)(ucaddr) - OS_BASE_UNCACHED))
#define OSCachedToUncached(caddr)                                              \
	((void*)((u8*)(caddr) + (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#define OSUncachedToCached(ucaddr)                                             \
	((void*)((u8*)(ucaddr) - (OS_BASE_UNCACHED - OS_BASE_CACHED)))
#endif

static inline void OSInitFastCast(void)
{
#ifdef __MWERKS__ // clang-format off
	asm {
		li r3, 4
		oris r3, r3, 4
		mtspr GQR2, r3
		li r3, 5
		oris r3, r3, 5
		mtspr GQR3, r3
		li r3, 6
		oris r3, r3, 6
		mtspr GQR4, r3
		li r3, 7
		oris r3, r3, 7
		mtspr GQR5, r3
	}
#endif // clang-format on
}

// NOTE: some code is compile as-if for the regular 750, so we occasionally
// gotta ifdef out paired singles stuff in SDK headers
#ifdef GEKKO

inline void OSf32tos8(register f32* f, register s8* out)
{
#ifdef __MWERKS__ // clang-format off
	asm {
		lfs    f1, 0(f)
		psq_st f1, 0(out), 0x1, 4
	}
#endif // clang-format on
}

#endif

#ifdef __cplusplus
}
#endif

#endif
