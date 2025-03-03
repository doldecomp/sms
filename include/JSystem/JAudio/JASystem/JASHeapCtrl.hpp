#ifndef JASHEAPCTRL_HPP
#define JASHEAPCTRL_HPP

namespace JASystem {

namespace Kernel {
	class THeap {
	public:
		THeap();
		void init();
		void* alloc(THeap*, unsigned long);
		void* allocHeapCheck(THeap*, unsigned long);
		void free();
		void* selfAlloc(THeap*, unsigned long, unsigned long);
		void initMotherHeap(unsigned long, unsigned long, unsigned char);
		void* getUnlockHeap();
		void checkAlloc();
		void selfInit(unsigned long, unsigned long, unsigned long);
		void setGroupHeap(THeap*);
		void cutDown();
		void garbageCollectionSt();
		void checkFreeHeapTotal();
		void checkFreeHeapLinear();
		void show(unsigned long);
		void moveChildren(long);
	};

	class TSolidHeap {
	public:
		TSolidHeap();
		TSolidHeap(unsigned char*, long);
		void init(unsigned char*, long);
		void* alloc(long);
		long getRemain();
		void freeLast();
		void freeAll();
	};

	extern void* global_id;

	void aramFinish(unsigned long);
	void dmaAramToAram(unsigned long, unsigned long, unsigned long);
	void dmaDramToDram(unsigned long, unsigned long, unsigned long);
} // namespace Kernel

namespace HeapCtrl {
	void init();
	void* alloc(unsigned long);
	void free(void*);
	void* getSystemHeap();
	void* getStudioHeap();
	void* getAramHeap();
	void* getWaveAramHeap();
	void* getSequenceHeap();
	void* getWaveHeap();
	void* getRegisterHeap();
	void* getSceneHeap();
	void* getSeqAramHeap();
	void* getSeqCacheHeap();
	void* getWaveCacheHeap();
	void* getStreamHeap();
	void* getStreamCacheHeap();
	void* getStreamAramHeap();
	void* getInfoHeap();
	void* getCommandHeap();
	void* getDriverHeap();
	void* getInterfaceHeap();
	void* getAudioSystemHeap();
	void* getAudioSystemAramHeap();
	void* getAudioSystemDramHeap();
	void* getAudioSystemWaveHeap();
	void* getAudioSystemWaveAramHeap();
} // namespace HeapCtrl

} // namespace JASystem

#endif // JASHEAPCTRL_HPP
