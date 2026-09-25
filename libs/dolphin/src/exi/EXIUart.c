#include "dolphin/exi.h"
#include "dolphin/os.h"

#define EXI_TX    0x800400u
#define EXI_MAGIC 0xa5ff005a

static s32 Chan;
static u32 Dev;
static u32 Enabled         = 0;
static u32 BarnacleEnabled = 0;

u32 InitializeUART(u32 baudRate)
{
	if (BarnacleEnabled == EXI_MAGIC) {
		return 0;
	}

	if (!(OSGetConsoleType() & OS_CONSOLE_DEVELOPMENT)) {
		Enabled = 0;
		return 2;
	} else {
		Chan    = 0;
		Dev     = 1;
		Enabled = EXI_MAGIC;
		return 0;
	}
}

u32 ReadUARTN(void* bytes, u32 length) { return 4; }

static int QueueLength(void)
{
	u32 cmd;

	if (!EXISelect(Chan, Dev, EXI_FREQ_8M))
		return -1;

	cmd = EXI_TX << 6;
	EXIImm(Chan, &cmd, 4, EXI_WRITE, NULL);
	EXISync(Chan);

	EXIImm(Chan, &cmd, 1, EXI_READ, NULL);
	EXISync(Chan);
	EXIDeselect(Chan);

	return 16 - (int)((cmd >> 24) & 0xff);
}

u32 WriteUARTN(void* buf, u32 len)
{
	u32 cmd;
	int qLen;
	long xLen;
	char* ptr;
	BOOL locked;
	u32 error;

	if (Enabled != EXI_MAGIC)
		return 2;

	locked = EXILock(Chan, Dev, 0);
	if (!locked) {
		return 0;
	}

	for (ptr = (char*)buf; ptr - (char*)buf < len; ptr++) {
		if (*ptr == '\n')
			*ptr = '\r';
	}

	error = 0;
	cmd   = (EXI_TX | 0x2000000) << 6;
	while (len) {
		qLen = QueueLength();
		if (qLen < 0) {
			error = 3;
			break;
		}

		if (qLen < 12 && qLen < len)
			continue;

		if (!EXISelect(Chan, Dev, EXI_FREQ_8M)) {
			error = 3;
			break;
		}

		EXIImm(Chan, &cmd, 4, EXI_WRITE, NULL);
		EXISync(Chan);

		while (qLen && len) {
			if (qLen < 4 && qLen < len)
				break;
			xLen = (len < 4) ? (long)len : 4;
			EXIImm(Chan, (void*)buf, xLen, EXI_WRITE, NULL);
			(u8*)buf += xLen;
			len -= xLen;
			qLen -= xLen;
			EXISync(Chan);
		}
		EXIDeselect(Chan);
	}

	EXIUnlock(Chan);
	return error;
}
