#ifndef JSYSTEM_OSDSP_TASK_H
#define JSYSTEM_OSDSP_TASK_H

#include <dolphin/types.h>

void DsyncFrame2(u32, u32, u32);
BOOL Dsp_Running_Check();
void Dsp_Running_Start();

#endif
