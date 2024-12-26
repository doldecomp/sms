#ifndef JKR_STD_HEAP_HPP
#define JKR_STD_HEAP_HPP

#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>

class JKRStdHeap : public JKRHeap {
public:
	JKRStdHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);

	virtual ~JKRStdHeap();                                         // _08
	virtual void* alloc(u32 size, int alignment);                  // _10
	virtual void free(void* ptr);                                  // _14
	virtual void freeTail();                                       // _18
	virtual void freeAll();                                        // _1C
	virtual s32 resize(void* ptr, u32 size);                       // _20
	virtual s32 getSize(void* ptr) { return OSReferentSize(ptr); } // _24
	virtual s32 getFreeSize();                                     // _28
	virtual u32 getHeapType() { return 'STDH'; }                   // _30
	virtual s32 getTotalFreeSize() { return getFreeSize(); }       // _2C
	virtual bool check() { return OSCheckHeap(mHeapId) != -1; }    // _34
	virtual bool dump_sort();                                      // _38
	virtual bool dump()                                            // _3C
	{
		OSDumpHeap(mHeapId);
		return OSCheckHeap(mHeapId) != -1;
	}
	virtual s32 changeGroupID(u8 groupId);           // _40
	virtual u8 getCurrentGroupId() { return 0; }     // _44 (weak)
	virtual void state_register(TState*, u32) const; // _48
	virtual bool state_compare(const TState& fst,
	                           const TState& snd) const; // _4C

	static JKRStdHeap* create(u32 size, JKRHeap* parent, bool errorFlag);

private:
	// _00     = VTBL
	// _00-_68 = JKRHeap
	s32 mHeapId; // _68
};

inline JKRStdHeap* JKRCreateStdHeap(u32 size, JKRHeap* parent, bool errorFlag)
{
	return JKRStdHeap::create(size, parent, errorFlag);
}

#endif
