#ifndef JSYSTEM_DSPTASK_H
#define JSYSTEM_DSPTASK_H

#include <dolphin/types.h>

void DspBoot(void (*)(void*));
int DSPSendCommands2(u32*, u32, void (*)(u16));
int DspStartWork(u32, void (*)(u16));
void DspFinishWork(u16);
void DsyncFrame2(u32, u32, u32);

#endif
