#include <dolphin/exi.h>
#include <dolphin/os.h>
#include <macros.h>

// internal include
#include "__os.h"

static struct SramControl Scb ATTRIBUTE_ALIGN(32);

static int ReadSram(void* buffer);
static void WriteSramCallback();
static int WriteSram(void* buffer, unsigned long offset, unsigned long size);
static void* LockSram(unsigned long offset);
static int UnlockSram(int commit, unsigned long offset);

static int ReadSram(void* buffer)
{
	int err;
	unsigned long cmd;

	DCInvalidateRange(buffer, 0x40);
	if (!EXILock(0, 1, NULL)) {
		return 0;
	}
	if (!EXISelect(0, 1, 3)) {
		EXIUnlock(0);
		return 0;
	}
	cmd = 0x20000100;
	err = 0;
	err |= !EXIImm(0, &cmd, 4, 1, 0);
	err |= !EXISync(0);
	err |= !EXIDma(0, buffer, 0x40, 0, NULL);
	err |= !EXISync(0);
	err |= !EXIDeselect(0);
	EXIUnlock(0);
	return !err;
}

static void WriteSramCallback()
{
	int unused;
	Scb.sync = WriteSram(&Scb.sram[Scb.offset], Scb.offset, 0x40 - Scb.offset);
	if (Scb.sync != 0) {
		Scb.offset = 0x40;
	}
}

static int WriteSram(void* buffer, unsigned long offset, unsigned long size)
{
	int err;
	unsigned long cmd;

	if (!EXILock(0, 1, WriteSramCallback)) {
		return 0;
	}
	if (!EXISelect(0, 1, 3)) {
		EXIUnlock(0);
		return 0;
	}
	offset <<= 6;
	cmd = ((offset + 0x100) | 0xA0000000);
	err = 0;
	err |= !EXIImm(0, &cmd, 4, 1, 0);
	err |= !EXISync(0);
	err |= !EXIImmEx(0, buffer, size, 1);
	err |= !EXIDeselect(0);
	EXIUnlock(0);
	return !err;
}

void __OSInitSram()
{
	Scb.locked = Scb.enabled = 0;
	Scb.sync                 = ReadSram(&Scb);
	Scb.offset               = 0x40;
}

static void* LockSram(unsigned long offset)
{
	int enabled;

	enabled = OSDisableInterrupts();
	if (Scb.locked) {
		OSRestoreInterrupts(enabled);
		return NULL;
	}
	Scb.enabled = enabled;
	Scb.locked  = 1;
	return &Scb.sram[offset];
}

OSSram* __OSLockSram() { return (OSSram*)LockSram(0); }

OSSramEx* __OSLockSramEx(void) { return (OSSramEx*)LockSram(0x14); }

static int UnlockSram(int commit, unsigned long offset)
{
	unsigned short* p;

	if (commit != 0) {
		if (offset == 0) {
			OSSram* sram = (OSSram*)Scb.sram;

			if (2u < (sram->flags & 3)) {
				sram->flags &= ~3;
			}

			sram->checkSum = sram->checkSumInv = 0;
			for (p = (unsigned short*)&sram->counterBias;
			     p < ((u16*)&Scb.sram[sizeof(OSSram)]); p++) {
				sram->checkSum += *p;
				sram->checkSumInv += ~(*p);
			}
		}
		if (offset < Scb.offset) {
			Scb.offset = offset;
		}
		Scb.sync
		    = WriteSram(&Scb.sram[Scb.offset], Scb.offset, 0x40 - Scb.offset);
		if (Scb.sync != 0) {
			Scb.offset = 0x40;
		}
	}
	Scb.locked = 0;
	OSRestoreInterrupts(Scb.enabled);
	return Scb.sync;
}

int __OSUnlockSram(int commit) { UnlockSram(commit, 0); }

int __OSUnlockSramEx(int commit) { UnlockSram(commit, 0x14); }

int __OSSyncSram() { return Scb.sync; }

int __OSReadROM(void* buffer, long length, long offset)
{
	int err;
	unsigned long cmd;

	DCInvalidateRange(buffer, length);
	if (EXILock(0, 1, NULL) == 0) {
		return 0;
	}
	if (EXISelect(0, 1, 3) == 0) {
		EXIUnlock(0);
		return 0;
	}
	cmd = offset << 6;
	err = 0;
	err |= !EXIImm(0, &cmd, 4, 1, 0);
	err |= !EXISync(0);
	err |= !EXIDma(0, buffer, length, 0, NULL);
	err |= !EXISync(0);
	err |= !EXIDeselect(0);
	EXIUnlock(0);
	return !err;
}

unsigned long OSGetSoundMode()
{
	struct OSSram* sram = __OSLockSram();
	unsigned long mode  = (sram->flags & 4) ? 1 : 0;

	__OSUnlockSram(0);
	return mode;
}

void OSSetSoundMode(unsigned long mode)
{
	struct OSSram* sram;
	int unused;

	mode *= 4;
	mode &= 4;
	sram = __OSLockSram();
	if (mode == (sram->flags & 4)) {
		__OSUnlockSram(0);
		return;
	}
	sram->flags &= 0xFFFFFFFB;
	sram->flags |= mode;
	__OSUnlockSram(1);
}

u32 OSGetProgressiveMode()
{
	OSSram* sram;
	u32 mode;

	sram = __OSLockSram();
	mode = (sram->flags & 0x80) >> 7;
	__OSUnlockSram(FALSE);
	return mode;
}

void OSSetProgressiveMode(u32 mode)
{
	char trash[0x2]; // TODO: intermediate vars or inlines?
	OSSram* sram;
	mode <<= 7;
	mode &= 0x80;

	sram = __OSLockSram();
	if (mode == (sram->flags & 0x80)) {
		__OSUnlockSram(FALSE);
		return;
	}

	sram->flags &= ~0x80;
	sram->flags |= mode;
	__OSUnlockSram(TRUE);
}

u16 OSGetWirelessID(s32 channel)
{
	OSSramEx* sram;
	u16 id;

	sram = __OSLockSramEx();
	id   = sram->wirelessPadID[channel];
	__OSUnlockSramEx(FALSE);
	return id;
}

void OSSetWirelessID(s32 channel, u16 id)
{
	OSSramEx* sram;

	sram = __OSLockSramEx();
	if (sram->wirelessPadID[channel] != id) {
		sram->wirelessPadID[channel] = id;
		__OSUnlockSramEx(TRUE);
		return;
	}

	__OSUnlockSramEx(FALSE);
}
