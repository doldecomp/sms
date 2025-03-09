#include <JSystem/JAudio/JASystem/JASAudioThread.hpp>
#include <JSystem/JAudio/JASystem/JASProbe.hpp>
#include <JSystem/JAudio/JASystem/JASAiCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASDSPBuf.hpp>
#include <JSystem/JAudio/JASystem/JASDriverIF.hpp>
#include <JSystem/JAudio/JASystem/JASDSPInterface.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASKernelDebug.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/dsptask.h>
#include <JSystem/dspproc.h>
#include <dolphin/dsp.h>
#include <dolphin/ai.h>

extern "C" void Console_printf(char*, ...) { }

namespace JASystem {
namespace AudioThread {
	OSThread jac_audioThread;
	u8 jac_audioStack[0x1000] __attribute__((aligned(0x20)));
	OSThread jac_dvdThread;
	u8 jac_dvdStack[0x1000] __attribute__((aligned(0x20)));

	static int isDSPBoot         = false;
	static u32 audioproc_mq_init = false;
	static volatile int intcount = 0;
	static int priority_set      = 0;
	static u32 jac_pri           = 0;
	static u32 dvd_pri           = 0;

	static OSMessageQueue audioproc_mq;
	static u8 msgbuf[0x40];

	void setDSPSyncCount(u32 count) { intcount = count; }

	u32 getDSPSyncCount() { return intcount; }

	static void syncAudio()
	{
		static u32 first = 1;

		if (first == 0)
			Kernel::probeFinish(4);

		first = 0;
		Kernel::probeStart(4, "UPDATE-DAC");

		if (audioproc_mq_init != 0)
			OSSendMessage(&audioproc_mq, 0, 0);
	}

	static void syncDSP(void* arg);

	static void* audioproc(void* arg)
	{
		OSInitFastCast();

		OSInitMessageQueue(&audioproc_mq, &msgbuf, 0x10);
		audioproc_mq_init = 1;
		Kernel::init();
		if (!isDSPBoot) {
			DspBoot(syncDSP);
			isDSPBoot = true;
		}
		Driver::init();
		AISetDSPSampleRate(Kernel::gAiSetting);
		AIRegisterDMACallback(syncAudio);
		AIStartDMA();
		for (;;) {
			int msg;

			OSReceiveMessage(&audioproc_mq, &msg, 1);

			switch (msg) {
			case 0:
				Kernel::updateDac();
				break;

			case 1:
				if (intcount == 0)
					return nullptr;
				--intcount;
				if (intcount == 0) {
					Kernel::probeFinish(7);
					DSPBuf::finishDSPFrame();
				} else {
					Kernel::probeStart(2, "SFR_DSP");
					DSPBuf::updateDSP();
					Kernel::probeFinish(2);
				}
				break;

			case 2:
				continue;

			case 3:
				OSExitThread(nullptr);
				break;
			}
		}
	}

	static void syncDSP(void* arg)
	{
		while (!DSPCheckMailFromDSP())
			;
		u32 mail = DSPReadMailFromDSP();
		if (mail >> 16 != DSPInterface::JAS_DSP_PREFIX)
			return;

		switch (mail & 0xFF00) {
		case 0xFF00:
			if (audioproc_mq_init)
				OSSendMessage(&audioproc_mq, (void*)1, 0);
			else
				DSPReleaseHalt();
			break;

		default:
			DspFinishWork(mail & 0xFFFF);
			break;
		}
	}

	void stop()
	{
		if (!audioproc_mq_init)
			return;

		if (!OSSendMessage(&audioproc_mq, (void*)3, 0))
			OSCancelThread(&jac_audioThread);
	}

	void setPriority(u8 audioPriority, u8 dvdPriority)
	{
		jac_pri      = audioPriority;
		priority_set = true;
		dvd_pri      = dvdPriority;
	}

	void start(JKRSolidHeap* heap, u32 aram_size, u32 flags)
	{
		if (priority_set == 0) {
			s32 pri = OSGetThreadPriority(OSGetCurrentThread());
			jac_pri = pri - 3;
			dvd_pri = jac_pri + 1;
		}
		Kernel::sysDramSetup(heap);
		Kernel::sysAramSetup(aram_size);
		Kernel::stackInit((u64*)jac_audioStack, 0x200);
		if (flags & 2) {
			OSCreateThread(&jac_audioThread, &audioproc, 0,
			               jac_audioStack + 0x1000, 0x1000, jac_pri, 1);
			OSResumeThread(&jac_audioThread);
		}

		Kernel::stackInit((u64*)jac_dvdStack, 0x200);
		if (flags & 1) {
			Dvd::dvdProcInit();
			OSCreateThread(&jac_dvdThread, &Dvd::dvdProc, 0,
			               jac_dvdStack + 0x1000, 0x1000, dvd_pri, 1);
			OSResumeThread(&jac_dvdThread);
		}
	}

} // namespace AudioThread
} // namespace JASystem
