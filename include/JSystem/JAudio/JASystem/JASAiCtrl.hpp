#ifndef JASAICTRL_HPP
#define JASAICTRL_HPP

namespace JASystem {
namespace Kernel {

	// AI Control functions
	void registerDacCallback(void (*callback)(short*, long));
	short* mixMonoTrack(short* buffer, long sampleCount,
	                    short* (*mixCallback)(long));
	short* mixMonoTrackWide(short* buffer, long sampleCount,
	                        short* (*mixCallback)(long));
	short* mixExtraTrack(short* buffer, long sampleCount,
	                     short* (*mixCallback)(long));
	short* mixInterleaveTrack(short* buffer, long sampleCount,
	                          short* (*mixCallback)(long));
	short* (*getMixCallback(unsigned char* mode))(long);
	void registerMixCallback(short* (*callback)(long), unsigned char mode);
	unsigned long getCurrentVCounter();

	// AI Control variables
	extern void (*dacCallbackFunc)(short*, long);
	extern short* (*extMixCallback)(long);
	extern unsigned char extMixMode;
	extern unsigned char useRspMadep;
	extern unsigned char lastRspMadep;
	extern unsigned char vframeWorkRunning;
	extern unsigned long JASVframeCounter;
	extern unsigned long JASUniversalDacCounter;

	// System functions
	void init();
	void initSystem();
	void vframeWork();
	void updateDac();

} // namespace Kernel
} // namespace JASystem

#endif // JASAICTRL_HPP
