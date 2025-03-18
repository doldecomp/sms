#include <JSystem/JAudio/JASystem/JASAiCtrl.hpp>
#include <JSystem/JAudio/JASystem/JASCallback.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>
#include <JSystem/JAudio/JASystem/JASCalc.hpp>
#include <JSystem/JAudio/JASystem/JASDvdThread.hpp>
#include <JSystem/JAudio/JASystem/JASRate.hpp>
#include <JSystem/JAudio/JASystem/JASSystemHeap.hpp>
#include <JSystem/JAudio/JASystem/JASVload.hpp>
#include <JSystem/JAudio/JASystem/JASDSPBuf.hpp>
#include <JSystem/JAudio/JASystem/JASHardStream.hpp>
#include <JSystem/JAudio/JASystem/JASProbe.hpp>
#include <dolphin/ai.h>

namespace JASystem {
namespace Kernel {

	u32 JASUniversalDacCounter                = 0;
	static s16* lastRspMadep                  = 0;
	static s16* useRspMadep                   = 0;
	static int vframeWorkRunning              = 0;
	static void (*dacCallbackFunc)(s16*, s32) = nullptr;

	static u32 JASVframeCounter       = 0;
	static MixCallback extMixCallback = nullptr;
	static u8 extMixMode              = 0;

	void init()
	{
		resetCallback();
		initSystem();
		portCmdInit();
		Dvd::init();
		Calc::initSinfT();
	}

	static s16* dac[3];

	void initSystem()
	{
		for (int i = 0; i < 3; i++) {
			dac[i] = (s16*)allocFromSysDram(getDacSize() * 2);
			Calc::bzero(dac[i], getDacSize() * 2);
			DCStoreRange(dac[i], getDacSize() * 2);
		}
		AIInit(nullptr);
		AIInitDMA(u32(dac[2]), getDacSize() * 2);
		Vload::initVloadBuffers();
	}

	u32 getCurrentVCounter() { return JASUniversalDacCounter; }

	MixCallback getMixCallback(s32) { return extMixCallback; }

	void registerMixCallback(MixCallback callback, u8 mode)
	{
		extMixCallback = callback;
		extMixMode     = mode;
	}

	static void mixMonoTrack(s16* buffer, s32 sampleCount,
	                         MixCallback mixCallback);
	static void mixMonoTrackWide(s16* buffer, s32 sampleCount,
	                             MixCallback mixCallback);
	static void mixExtraTrack(s16* buffer, s32 sampleCount,
	                          MixCallback mixCallback);
	static void mixInterleaveTrack(s16* buffer, s32 sampleCount,
	                               MixCallback mixCallback);

	void vframeWork()
	{
		static u32 dacp = 0;
		JASVframeCounter++;

		s16* buf1 = DSPBuf::mixDSP(getDacSize() / 2);
		s16* buf2 = buf1 + getFrameSamples();
		Calc::imixcopy(buf2, buf1, dac[dacp], getDacSize() / 2);

		if (extMixCallback) {
			switch (extMixMode) {
			case 0:
				mixMonoTrack(dac[dacp], getDacSize() / 2, extMixCallback);
				break;
			case 1:
				mixMonoTrackWide(dac[dacp], getDacSize() / 2, extMixCallback);
				break;
			case 2:
				mixExtraTrack(dac[dacp], getDacSize() / 2, extMixCallback);
				break;
			case 3:
				mixInterleaveTrack(dac[dacp], getDacSize() / 2, extMixCallback);
				break;
			}
		}
		BOOL enable = OSDisableInterrupts();
		DCStoreRange(dac[dacp], getDacSize() * 2);
		OSRestoreInterrupts(enable);
		lastRspMadep = dac[dacp];
		dacp++;
		if (dacp == 3) {
			dacp = 0;
		}
		vframeWorkRunning = 0;
	}

	void updateDac()
	{
		if (!useRspMadep) {
			useRspMadep  = lastRspMadep;
			lastRspMadep = nullptr;
		}

		if (useRspMadep) {
			AIInitDMA((u32)useRspMadep, getDacSize() * 2);
			useRspMadep = nullptr;
		} else {
			JASUniversalDacCounter++;
		}

		if (!lastRspMadep && vframeWorkRunning == 0)
			vframeWork();

		HardStream::main();
		if (dacCallbackFunc)
			dacCallbackFunc(lastRspMadep, getDacSize() / 2);
	}

	void registerDacCallback(void (*callback)(s16*, s32)) { }

	inline s16 clamp_s32_to_s16(s32 v)
	{
		if (v < -0x8000)
			return -0x7fff;
		if (v > 0x7fff)
			return 0x7fff;
		return (s16)v;
	}

	static void mixMonoTrack(s16* buffer, s32 sampleCount,
	                         MixCallback mixCallback)
	{
		probeStart(5, "MONO-MIX");
		s16* src = mixCallback(sampleCount);
		if (!src)
			return;

		probeFinish(5);
		s16* destPtr = buffer;
		s16* srcPtr  = src;
		for (int samplesLeft = sampleCount; samplesLeft > 0; --samplesLeft) {
			*destPtr++ = clamp_s32_to_s16(*destPtr + *srcPtr);
			*destPtr++ = clamp_s32_to_s16(*destPtr + *srcPtr);
			++srcPtr;
		}
	}

	static void mixMonoTrackWide(s16* buffer, s32 sampleCount,
	                             MixCallback mixCallback)
	{
		probeStart(5, "MONO(W)-MIX");
		s16* src = mixCallback(sampleCount);
		if (!src) {
			return;
		}
		probeFinish(5);
		s16* destPtr = buffer;
		s16* srcPtr  = src;
		for (int samplesLeft = sampleCount; samplesLeft > 0; --samplesLeft) {
			*destPtr++ = clamp_s32_to_s16(*destPtr + *srcPtr);
			*destPtr++ = clamp_s32_to_s16(*destPtr - *srcPtr);
			++srcPtr;
		}
	}

	static void mixExtraTrack(s16* buffer, s32 sampleCount,
	                          MixCallback mixCallback)
	{
		probeStart(5, "DSPMIX");
		s16* src = mixCallback(sampleCount);
		if (!src)
			return;

		probeFinish(5);
		probeStart(6, "MIXING");
		s16* destPtr = buffer;
		s16* srcPtr  = src;
		s16* srcPtr2 = src + gFrameSamples;
		for (int samplesLeft = sampleCount; samplesLeft > 0; --samplesLeft) {
			*destPtr++ = clamp_s32_to_s16(*destPtr + *srcPtr2);
			*destPtr++ = clamp_s32_to_s16(*destPtr + *srcPtr);
			++srcPtr2;
			++srcPtr;
		}
		probeFinish(6);
	}

	static void mixInterleaveTrack(s16* buffer, s32 sampleCount,
	                               MixCallback mixCallback)
	{
		s16* src = mixCallback(sampleCount);
		if (!src)
			return;

		s16* destPtr = buffer;
		for (int i = 0; i < sampleCount * 2; i++) {
			*destPtr++ = clamp_s32_to_s16(*destPtr + *src);
			src++;
		}
	}

} // namespace Kernel
} // namespace JASystem
