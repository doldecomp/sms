#ifndef JSYSTEM_DSPPROC_H
#define JSYSTEM_DSPPROC_H

#include <dolphin/types.h>
#include <stdint.h>

void DSPSendCommands(u32*, u32);
void DSPReleaseHalt();
void DSPWaitFinish();
void Dswap(u32, u32, u32);
void Dmix(u32, u32, u32, s16);
void Dcopy(u32, u32, u32);
void DloadBuffer1(u32, u32);
void DloadBuffer(u32, u32, u32);
void DsaveBuffer(u16, u32, u32);
void DsetLoopState(u32);
void DadpcmDec(u32, u32, u32, u32);
void DloadFilter(u32, u32);
void DcopyfromARAM(u32, u32, u32);
void DoscStart(u8, u16, u16);
void DoscStop(u8);
void DoscStore(u32);
void Dmixer(u32, u32);
void Dresampletest();
void Dadpcmtest(u32);
void DsetupTable(u32 chan_num, uintptr_t chan_buffer, uintptr_t render_tables,
                 uintptr_t adpcm_filters, uintptr_t fx_buffer);
void DsetMixerLevel(f32 level);
void DsyncFrame(u32 sub_frames, uintptr_t out_pcm_left,
                uintptr_t out_pcm_right);
void wait_callback(u16);
void DwaitFrame();
void DiplSec(u32, void (*)(u16));
void DagbSec(u32, void (*)(u16));
void dummy_callback(u16);

#endif
