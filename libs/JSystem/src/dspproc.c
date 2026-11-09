#include <JSystem/dspproc.h>
#include <JSystem/dsptask.h>
#include <dolphin/types.h>

extern u64 DSP_CreateMap();

void DSPSendCommands(u32*, u32) { }

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

void DSPWaitFinish() { }
void Dswap(u32, u32, u32) { }
void Dmix(u32, u32, u32, s16) { }
void Dcopy(u32, u32, u32) { }
void DloadBuffer1(u32, u32) { }
void DloadBuffer(u32, u32, u32) { }
void DsaveBuffer(u16, u32, u32) { }
void DsetLoopState(u32) { }
void DadpcmDec(u32, u32, u32, u32) { }
void DloadFilter(u32, u32) { }
void DcopyfromARAM(u32, u32, u32) { }
void DoscStart(u8, u16, u16) { }
void DoscStop(u8) { }
void DoscStore(u32) { }
void Dmixer(u32, u32) { }
void Dresampletest() { }
void Dadpcmtest(u32) { }

static volatile BOOL flag;
static u32 waitflag;
static u32 d_waitflag;

static void setup_callback(u16) { flag = 0; }

void DsetupTable(u32 chan_num, uintptr_t chan_buffer, uintptr_t render_tables,
                 uintptr_t adpcm_filters, uintptr_t fx_buffer)
{
	u32 table[5];
	table[0] = (chan_num & 0xFFFF) | 0x81000000;
	table[1] = chan_buffer;
	table[2] = render_tables;
	table[3] = adpcm_filters;
	table[4] = fx_buffer;
	flag     = 1;
	DSPSendCommands2(table, 5, setup_callback);
	while (flag)
		;
}

static u16 DSP_MIXERLEVEL = 0x4000;

void DsetMixerLevel(f32 level) { DSP_MIXERLEVEL = 4096.0f * level; }

void DsyncFrame(u32 sub_frames, uintptr_t out_pcm_left, uintptr_t out_pcm_right)
{
	u32 msgs[3];
	u32 cmd = (sub_frames & 0xff) << 0x10 | 0x82000000 | DSP_MIXERLEVEL;

	msgs[0] = cmd;
	msgs[1] = out_pcm_left;
	msgs[2] = out_pcm_right;
	DSPSendCommands2(msgs, 3, nullptr);
}

void wait_callback(u16) { }
void DwaitFrame() { }
void DiplSec(u32, void (*)(u16)) { }
void DagbSec(u32, void (*)(u16)) { }
void dummy_callback(u16) { }
