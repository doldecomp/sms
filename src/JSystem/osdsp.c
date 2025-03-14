#include <dolphin/dsp.h>
#include "types.h"

DSPTaskInfo* __DSP_tmp_task;

extern DSPTaskInfo* DSP_prior_task;

DSPTaskInfo* DSPAddTask(DSPTaskInfo* task)
{
	if (DSP_prior_task == nullptr) {
		OSReport("Prior Task is not inited\n");
		return nullptr;
	}
	BOOL status = OSDisableInterrupts();
	__DSP_insert_task(task);
	task->state = 0;
	task->flags = 1;
	OSRestoreInterrupts(status);
	return task;
}

void DSPAddPriorTask(DSPTaskInfo* task)
{
	if (DSP_prior_task != nullptr) {
		OSReport("Already inited prior DSP task\n");
		return;
	}
	BOOL status    = OSDisableInterrupts();
	DSP_prior_task = task;
	task->state    = 0;
	task->flags    = 1;
	__DSP_boot_task(task);
	OSRestoreInterrupts(status);
}
