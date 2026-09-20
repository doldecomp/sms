#include <dolphin/exi.h>
#include <dolphin/os.h>
#include <macros.h>

// internal include
#include "__os.h"

static struct SramControl Scb ATTRIBUTE_ALIGN(32);

static int ReadSram(void* buffer);
static void WriteSramCallback();
static int WriteSram(void* buffer, u32 offset, u32 size);
static void* LockSram(u32 offset);
static int UnlockSram(int commit, u32 offset);

static int GetRTC(u32* rtc)
{
	int err;
	u32 cmd;

	if (EXILock(0, 1, NULL) == 0) {
		return 0;
	}
	if (EXISelect(0, 1, 3) == 0) {
		EXIUnlock(0);
		return 0;
	}
	cmd = 0x20000000;
	err = 0;
	err |= !EXIImm(0, &cmd, 4, 1, 0);
	err |= !EXISync(0);
	err |= !EXIImm(0, &cmd, 4, 0, 0);
	err |= !EXISync(0);
	err |= !EXIDeselect(0);
	EXIUnlock(0);
	rtc[0] = cmd;
	return !err;
}

int __OSGetRTC(u32* rtc)
{
	int err;
	u32 t0;
	u32 t1;
	int i;

	for (i = 0; i < 16; i++) {
		err = 0;
		err |= !GetRTC(&t0);
		err |= !GetRTC(&t1);
		if (err) {
			break;
		}
		if (t0 == t1) {
			rtc[0] = t0;
			return 1;
		}
	}
	return 0;
}

int __OSSetRTC(u32 rtc)
{
	int err;
	u32 cmd;

	if (EXILock(0, 1, NULL) == 0) {
		return 0;
	}
	if (EXISelect(0, 1, 3) == 0) {
		EXIUnlock(0);
		return 0;
	}
	cmd = 0xA0000000;
	err = 0;
	err |= !EXIImm(0, &cmd, 4, 1, 0);
	err |= !EXISync(0);
	err |= !EXIImm(0, &rtc, 4, 1, 0);
	err |= !EXISync(0);
	err |= !EXIDeselect(0);
	EXIUnlock(0);
	return !err;
}

static int ReadSram(void* buffer)
{
	int err;
	u32 cmd;

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

static int WriteSram(void* buffer, u32 offset, u32 size)
{
	int err;
	u32 cmd;

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

static void* LockSram(u32 offset)
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

static int UnlockSram(int commit, u32 offset)
{
	u16* p;

	if (commit != 0) {
		if (offset == 0) {
			OSSram* sram = (OSSram*)Scb.sram;

			if (2u < (sram->flags & 3)) {
				sram->flags &= ~3;
			}

			sram->checkSum = sram->checkSumInv = 0;
			for (p = (u16*)&sram->counterBias;
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

int __OSCheckSram()
{
	u16* p;
	u16 checkSum;
	u16 checkSumInv;
	struct OSSram* sram;
	int unused;

	ASSERTLINE(0x1A9, Scb.locked);

	checkSum = checkSumInv = 0;

	sram = (struct OSSram*)&Scb.sram[0];

	for (p = (void*)&sram->counterBias; p < (u16*)&Scb.sram[0x14]; p++) {
		checkSum += *p;
		checkSumInv += ~(*p);
	}

	return (sram->checkSum == checkSum && sram->checkSumInv == checkSumInv);
}

int __OSReadROM(void* buffer, s32 length, s32 offset)
{
	int err;
	u32 cmd;

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

static void __OSReadROMCallback(s32 chan)
{
	void (*callback)();

	EXIDeselect(chan);
	EXIUnlock(chan);
	callback = Scb.callback;
	if (callback) {
		Scb.callback = NULL;
		callback();
	}
}

int __OSReadROMAsync(void* buffer, s32 length, s32 offset, void (*callback)())
{
	int err;
	u32 cmd;

	ASSERTLINE(0x203, length <= 1024);
	ASSERTLINE(0x204, callback);
	DCInvalidateRange(buffer, length);
	Scb.callback = callback;
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
	err |= !EXIDma(0, buffer, length, 0, (void*)__OSReadROMCallback);
	return !err;
}

u32 OSGetSoundMode()
{
	struct OSSram* sram = __OSLockSram();
	u32 mode            = (sram->flags & 4) ? 1 : 0;

	__OSUnlockSram(0);
	return mode;
}

void OSSetSoundMode(u32 mode)
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

u32 OSGetVideoMode() { return 0; }

void OSSetVideoMode(u32 mode) { }

u8 OSGetLanguage(void)
{
	OSSram* sram = __OSLockSram();
	u8 language  = sram->language;

	__OSUnlockSram(0);
	return language;
}

void OSSetLanguage(u8 language)
{
	struct OSSram* sram = __OSLockSram();
	int unused;

	if (language == sram->language) {
		__OSUnlockSram(0);
		return;
	}
	sram->language = language;
	__OSUnlockSram(1);
}

u8 __OSGetBootMode()
{
	struct OSSram* sram = __OSLockSram();
	u8 ntd              = sram->ntd;
	__OSUnlockSram(0);
	return ntd & 0x80;
}

void __OSSetBootMode(u8 ntd)
{
	struct OSSram* sram;
	int unused;

	ntd &= 0x80;
	sram = __OSLockSram();
	if (ntd == (sram->ntd & 0x80U)) {
		__OSUnlockSram(0);
		return;
	}
	sram->ntd &= 0xFFFFFF7F;
	sram->ntd |= ntd;
	__OSUnlockSram(1);
}

u32 OSGetEuRgb60Mode(void)
{
	OSSram* sram;
	u32 on;

	sram = __OSLockSram();
	on   = (sram->ntd & 0x40) >> 6;
	__OSUnlockSram(0);
	return on;
}

void OSSetEuRgb60Mode(u32 on)
{
	char trash[0x1]; // TODO:
	OSSram* sram;

	ASSERTLINE(895, on == OS_EURGB60_OFF || on == OS_EURGB60_ON);
	on <<= 6;
	on &= 0x40;

	sram = __OSLockSram();
	if (on == (sram->ntd & 0x40)) {
		__OSUnlockSram(0);
	} else {
		sram->ntd &= ~0x40;
		sram->ntd |= on;
		__OSUnlockSram(1);
	}
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
