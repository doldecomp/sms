#ifndef JASAUDIOTHREAD_HPP
#define JASAUDIOTHREAD_HPP

#include <dolphin/os.h>
#include <dolphin/types.h>

class JKRSolidHeap;

namespace JASystem {
namespace AudioThread {
	extern OSThread jac_audioThread;
	extern u8 jac_audioStack[0x1000];
	extern OSThread jac_dvdThread;
	extern u8 jac_dvdStack[0x1000];

	void setPriority(u8 audio_prio, u8 dvd_prio);
	void start(JKRSolidHeap* heap, u32 aram_size, u32 flags);
	void setDSPSyncCount(u32);
	u32 getDSPSyncCount();
	void stop();
	void bootDSP();
} // namespace AudioThread
} // namespace JASystem

#endif // JASAUDIOTHREAD_HPP
