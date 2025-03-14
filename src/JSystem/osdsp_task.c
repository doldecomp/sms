#include <JSystem/osdsp_task.h>
#include <JSystem/dspproc.h>
#include <dolphin/dsp.h>
#include <dolphin/os.h>
#include <types.h>

static u32 sync_stack[3];
static volatile u8 DSP_prior_yield;
static s32 AUDIO_UPDATE_REQUEST;
DSPTaskInfo* DSP_prior_task;

extern DSPTaskInfo* __DSP_curr_task;
extern DSPTaskInfo* __DSP_first_task;

extern "C" void Console_printf(const char*, ...);

static void Dsp_Update_Request();

extern "C" void __DSPHandler(int interrupt, OSContext* context)
{
	char trash[0x2D8]; // TODO: wtf is this???

	u16 temp     = __DSPRegs[5];
	temp         = temp & ~0x28;
	temp         = temp | 0x80;
	__DSPRegs[5] = temp;

	if (DSP_prior_yield == 1 || DSP_prior_yield == 0)
		__DSP_curr_task = DSP_prior_task;

	while (DSPCheckMailFromDSP() == 0)
		;

	u32 mail = DSPReadMailFromDSP();

	if ((__DSP_curr_task->flags & 2) && mail == 0xDCD10002)
		mail = 0xDCD10003;

	switch (mail) {
	case 0xDCD10000:
		__DSP_curr_task->state = 1;
		if (__DSP_curr_task == DSP_prior_task) {
			DSP_prior_yield = 1;
		}
		if (__DSP_curr_task->init_cb != nullptr) {
			__DSP_curr_task->init_cb(__DSP_curr_task);
		}
		break;
	case 0xDCD10001:
		__DSP_curr_task->state = 1;
		if (__DSP_curr_task == DSP_prior_task) {
			DSP_prior_yield = 1;
			Dsp_Update_Request();
		}
		if (__DSP_curr_task->res_cb != nullptr)
			__DSP_curr_task->res_cb(__DSP_curr_task);
		Console_printf("Audio Resumed\n");
		break;
	case 0xDCD10002:
		Console_printf("Yield Handler\n");

		DSPSendMailToDSP(0xCDD10001);
		while (DSPCheckMailToDSP() != 0)
			;
		__DSP_curr_task->state = 2;
		if (__DSP_curr_task->next == nullptr && AUDIO_UPDATE_REQUEST) {
			__DSP_exec_task(__DSP_curr_task, DSP_prior_task);
			AUDIO_UPDATE_REQUEST = 0;
			__DSP_curr_task      = DSP_prior_task;
		} else {
			__DSP_exec_task(__DSP_curr_task, __DSP_curr_task->next);
			__DSP_curr_task = __DSP_curr_task->next;
		}
		break;
	case 0xDCD10003:
		Console_printf("Done DSP Task  %x \n", __DSP_curr_task);
		if (__DSP_curr_task->done_cb != nullptr)
			__DSP_curr_task->done_cb(__DSP_curr_task);
		DSPSendMailToDSP(0xCDD10001);
		while (DSPCheckMailToDSP() != 0)
			;
		__DSP_curr_task->state = 3;
		if (__DSP_curr_task->next == nullptr) {
			__DSP_exec_task(nullptr, DSP_prior_task);
			__DSP_remove_task(__DSP_curr_task);
			__DSP_curr_task = DSP_prior_task;
		} else {
			__DSP_exec_task(nullptr, __DSP_curr_task->next);
			__DSP_curr_task = __DSP_curr_task->next;
			__DSP_remove_task(__DSP_curr_task->prev);
		}
		break;
	case 0xDCD10004:
		if (__DSP_curr_task->req_cb != nullptr)
			__DSP_curr_task->req_cb(__DSP_curr_task);
		break;
	case 0xDCD10005:
		if (__DSP_first_task == nullptr || AUDIO_UPDATE_REQUEST) {
			DSPSendMailToDSP(0xCDD10003);
			while (DSPCheckMailToDSP() != 0)
				;
			AUDIO_UPDATE_REQUEST = 0;
			__DSP_curr_task      = DSP_prior_task;
			Dsp_Update_Request();
		} else {
			Console_printf("Audio Yield Start\n");
			DSP_prior_yield = 3;
			DSPSendMailToDSP(0xCDD10001);
			while (DSPCheckMailToDSP() != 0)
				;
			__DSP_exec_task(DSP_prior_task, __DSP_first_task);
			__DSP_curr_task = __DSP_first_task;
			Console_printf("Audio Yield Finish\n");
		}
		break;
	default:
		OSPanic(
		    __FILE__, 0x10B,
		    "__DSPHandler(): Unknown msg from DSP 0x%08X - task sync failed!\n",
		    mail);
		break;
	}
}

void DsyncFrame2(u32 param_1, u32 param_2, u32 param_3)
{
	if (DSP_prior_yield != 1) {
		Console_printf("Yield中です\n");
		sync_stack[0]        = param_1;
		sync_stack[1]        = param_2;
		sync_stack[2]        = param_3;
		AUDIO_UPDATE_REQUEST = 1;
		return;
	}
	DsyncFrame(param_1, param_2, param_3);
	AUDIO_UPDATE_REQUEST = 0;
}

static void Dsp_Update_Request()
{
	if (AUDIO_UPDATE_REQUEST) {
		DsyncFrame2(sync_stack[0], sync_stack[1], sync_stack[2]);
	}
}

BOOL Dsp_Running_Check() { return DSP_prior_yield != 1 ? FALSE : TRUE; }

void Dsp_Running_Start() { DSP_prior_yield = 1; }
