#ifndef JKR_STD_HEAP_HPP
#define JKR_STD_HEAP_HPP

#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>

class JKRStdHeap : public JKRHeap {
public:
	JKRStdHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);

	virtual ~JKRStdHeap();
	virtual void* alloc(u32 size, int alignment);
	virtual void free(void* ptr);
	virtual void freeTail();
	virtual void freeAll();
	virtual s32 resize(void* ptr, u32 size);
	virtual s32 getSize(void* ptr) { return OSReferentSize(ptr); }
	virtual s32 getFreeSize();
	virtual s32 getTotalFreeSize() { return getFreeSize(); }
	virtual u32 getHeapType() { return 'STDH'; }
	virtual bool check() { return OSCheckHeap(mHeapId) != -1; }
	virtual bool dump()
	{
		OSDumpHeap(mHeapId);
		return OSCheckHeap(mHeapId) != -1;
	}
	virtual void state_register(TState*, u32) const;
	virtual bool state_compare(const TState& fst, const TState& snd) const;

	static JKRStdHeap* create(u32 size, JKRHeap* parent, bool errorFlag);

private:
	/* 0x68 */ s32 mHeapId;
};

inline JKRStdHeap* JKRCreateStdHeap(u32 size, JKRHeap* parent, bool errorFlag)
{
	return JKRStdHeap::create(size, parent, errorFlag);
}

#endif
