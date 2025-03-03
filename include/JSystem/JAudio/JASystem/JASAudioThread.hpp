#ifndef JASAUDIOTHREAD_HPP
#define JASAUDIOTHREAD_HPP

#include <dolphin/os.h>

class JKRSolidHeap;

namespace JASystem {
namespace AudioThread {
	extern unsigned char jac_pri;
	extern unsigned char priority_set;
	extern unsigned char dvd_pri;
	extern OSThread* jac_audioThread;
	extern unsigned char* jac_audioStack;
	extern OSThread* jac_dvdThread;
	extern unsigned char* jac_dvdStack;
	extern OSMessageQueue audioproc_mq;
	extern void* msgbuf;
	extern bool audioproc_mq_init;
	extern bool isDSPBoot;
	extern unsigned long intcount;

	void setPriority(unsigned char, unsigned char);
	void start(JKRSolidHeap*, unsigned long, unsigned long);
	void* audioproc(void*);
	void* syncDSP(void*);
	void setDSPSyncCount(unsigned long);
	unsigned long getDSPSyncCount();
	void syncAudio();
	void stop();
	void bootDSP();
} // namespace AudioThread
} // namespace JASystem

#endif // JASAUDIOTHREAD_HPP
