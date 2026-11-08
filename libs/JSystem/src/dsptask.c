#include <JSystem/dsptask.h>
#include <JSystem/osdsp_task.h>
#include <JSystem/osdsp.h>
#include <dolphin/dsp.h>
#include <dolphin/types.h>

static void DspInitWork();

static void DspHandShake(void*)
{
	volatile u32 mail;
	char trash[0x8];

	do {
		mail = DSPCheckMailFromDSP();
	} while (!mail);
	DSPReadMailFromDSP();
	DSPCheckMailFromDSP();
	Dsp_Running_Start();
}

u8 jdsp[] __attribute__((aligned(32))) = {
#include "sms_jdsp.inc"
};

static DSPTaskInfo audio_task __attribute__((aligned(32)));

static u8 AUDIO_YIELD_BUFFER[8192] __attribute__((aligned(32)));

void DspBoot(void (*param_1)(void*))
{
	DspInitWork();
	audio_task.priority          = 0xf0;
	audio_task.iram_mmem_addr    = (u16*)OSPhysicalToCached(jdsp);
	audio_task.iram_length       = sizeof(jdsp);
	audio_task.iram_addr         = 0;
	audio_task.dram_mmem_addr    = (u16*)OSPhysicalToCached(AUDIO_YIELD_BUFFER);
	audio_task.dram_length       = sizeof(AUDIO_YIELD_BUFFER);
	audio_task.dram_addr         = 0;
	audio_task.dsp_init_vector   = 0;
	audio_task.dsp_resume_vector = 0x10;
	audio_task.init_cb           = DspHandShake;
	audio_task.res_cb            = (DSPCallback)nullptr;
	audio_task.done_cb           = (DSPCallback)nullptr;
	audio_task.req_cb            = param_1;
	DSPInit();
	DSPAddPriorTask(&audio_task);
}

int DSPSendCommands2(u32* param_1, u32 param_2, void (*callBack)(u16))
{
	s32 i;
	BOOL interruptFlag;
	s32 startWorkStatus;

	while (Dsp_Running_Check() == 0)
		;

	interruptFlag = OSDisableInterrupts();
	if (DSPCheckMailToDSP()) {
		OSRestoreInterrupts(interruptFlag);
		return -1;
	}

	DSPSendMailToDSP(param_2);
	DSPAssertInt();
	while (DSPCheckMailToDSP() != 0)
		;

	if (param_2 == 0)
		param_2 = 2;

	if (callBack != nullptr)
		startWorkStatus = DspStartWork(param_1[0], callBack);

	for (i = 0; i < param_2; i++) {
		DSPSendMailToDSP(param_1[i]);
		while (DSPCheckMailToDSP() != 0)
			;
	}

	OSRestoreInterrupts(interruptFlag);
	return startWorkStatus;
}

typedef struct {
	u16 field_0x0;
	u16 field_0x2;
	void (*field_0x4)(u16);
} TaskWorkStruct;

static TaskWorkStruct taskwork[16];

static void DspInitWork()
{
	for (int i = 0; i < 16; i++)
		taskwork[i].field_0x4 = (void (*)(u16))nullptr;
}

static u32 taskreadp;
static u32 taskwritep;

int DspStartWork(u32 param_1, void (*param_2)(u16))
{
	u32 taskWritePrev = taskwritep;
	u32 taskWriteCurr = taskWritePrev + 1;
	if ((taskWriteCurr & 0xf) == taskreadp)
		return 0;

	taskwork[taskWritePrev].field_0x0 = param_1 >> 0x10;
	taskwork[taskWritePrev].field_0x4 = param_2;
	taskwritep                        = taskWriteCurr & 0xf;
	return taskWriteCurr;
}

void DspFinishWork(u16 param_1)
{
	if (param_1 != taskwork[taskreadp].field_0x0)
		return;

	if (taskwork[taskreadp].field_0x4)
		taskwork[taskreadp].field_0x4(taskreadp);

	taskreadp = (taskreadp + 1) & 0xf;
}
