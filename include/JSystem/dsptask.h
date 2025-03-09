#ifndef JSYSTEM_DSPTASK_H
#define JSYSTEM_DSPTASK_H

#include <dolphin/types.h>

void DspHandShake(void*);
void DspBoot(void (*)(void*));
void DSPSendCommands2(u32*, u32, void (*)(u16));
void DspInitWork();
void DspStartWork(u32, void (*)(u16));
void DspFinishWork(u16);

#endif
