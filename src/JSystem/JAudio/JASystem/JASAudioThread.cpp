#include <JSystem/JAudio/JASystem/JASAudioThread.hpp>

namespace JASystem {
namespace AudioThread {
	u8 jac_pri                = 0;
	u8 priority_set           = 0;
	u8 dvd_pri                = 0;
	OSThread* jac_audioThread = nullptr;
	u8* jac_audioStack        = nullptr;
	OSThread* jac_dvdThread   = nullptr;
	u8* jac_dvdStack          = nullptr;
	OSMessageQueue audioproc_mq;
	void* msgbuf           = nullptr;
	bool audioproc_mq_init = false;
	bool isDSPBoot         = false;
	u32 intcount           = 0;

	void setPriority(u8 audioPriority, u8 dvdPriority) { }

	void start(JKRSolidHeap* heap, u32 stackSize, u32 dvdStackSize) { }

	void* audioproc(void* arg) { return nullptr; }

	void* syncDSP(void* arg) { return nullptr; }

	void setDSPSyncCount(u32 count) { }

	u32 getDSPSyncCount() { return 0; }

	void syncAudio() { }

	void stop() { }

	void bootDSP() { }
} // namespace AudioThread
} // namespace JASystem
