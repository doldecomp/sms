#include <JSystem/dspproc.h>
#include <JSystem/dsptask.h>
#include <types.h>

extern u64 DSP_CreateMap();

void DSPReleaseHalt()
{
	u64 map = DSP_CreateMap();

	u32 hi = map >> 32;
	u32 lo = map & 0xffffffff;

	u32 msgs[2];
	msgs[0] = hi;
	msgs[1] = lo;
	DSPSendCommands2(msgs, 0, nullptr);
}

static volatile BOOL flag;

static void setup_callback(u16) { flag = 0; }

void DsetupTable(u32 param_1, u32 param_2, u32 param_3, u32 param_4,
                 u32 param_5)
{
	u32 table[5];
	table[0] = (param_1 & 0xFFFF) | 0x81000000;
	table[1] = param_2;
	table[2] = param_3;
	table[3] = param_4;
	table[4] = param_5;
	flag     = 1;
	DSPSendCommands2(table, 5, setup_callback);
	while (flag)
		;
}

static u16 DSP_MIXERLEVEL = 0x4000;

void DsetMixerLevel(float level) { DSP_MIXERLEVEL = 4096.0f * level; }

void DsyncFrame(u32 param_1, u32 param_2, u32 param_3)
{
	u32 msgs[3];
	u32 cmd = (param_1 & 0xff) << 0x10 | 0x82000000 | DSP_MIXERLEVEL;

	msgs[0] = cmd;
	msgs[1] = param_2;
	msgs[2] = param_3;
	DSPSendCommands2(msgs, 3, 0);
}
