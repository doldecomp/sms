#ifndef JASAUDIOTHREAD_HPP
#define JASAUDIOTHREAD_HPP

#include <dolphin/os.h>
#include <types.h>

class JKRSolidHeap;

namespace JASystem {
namespace AudioThread {
	extern u8 jac_pri;
	extern u8 priority_set;
	extern u8 dvd_pri;
	extern OSThread* jac_audioThread;
	extern u8* jac_audioStack;
	extern OSThread* jac_dvdThread;
	extern u8* jac_dvdStack;
	extern OSMessageQueue audioproc_mq;
	extern void* msgbuf;
	extern bool audioproc_mq_init;
	extern bool isDSPBoot;
	extern u32 intcount;

	void setPriority(u8, u8);
	void start(JKRSolidHeap*, u32, u32);
	void* audioproc(void*);
	void* syncDSP(void*);
	void setDSPSyncCount(u32);
	u32 getDSPSyncCount();
	void syncAudio();
	void stop();
	void bootDSP();
} // namespace AudioThread
} // namespace JASystem

#endif // JASAUDIOTHREAD_HPP
