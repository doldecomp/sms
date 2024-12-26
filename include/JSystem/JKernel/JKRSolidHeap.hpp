#ifndef JKR_STD_HEAP_HPP
#define JKR_STD_HEAP_HPP

#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>

class JKRSolidHeap : public JKRHeap {
public:
	JKRSolidHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);

	virtual ~JKRSolidHeap();                                 // _08
	virtual void* alloc(u32 size, int alignment);            // _10
	virtual void free(void* ptr);                            // _14
	virtual void freeTail();                                 // _18
	virtual void freeAll();                                  // _1C
	virtual s32 resize(void* ptr, u32 size);                 // _20
	virtual s32 getSize(void* ptr);                          // _24
	virtual s32 getFreeSize() { return mFreeSize; }          // _28
	virtual u32 getHeapType() { return 'SLID'; }             // _30
	virtual s32 getTotalFreeSize() { return getFreeSize(); } // _2C
	virtual bool check();                                    // _34
	virtual bool dump_sort() { return true; }                // _38
	virtual bool dump();                                     // _3C
	virtual s32 changeGroupID(u8 groupId) { return 0; }      // _40
	virtual u8 getCurrentGroupId() { return 0; }             // _44 (weak)
	virtual void state_register(TState*, u32) const;         // _48
	virtual bool state_compare(const TState& fst,
	                           const TState& snd) const; // _4C

	static JKRSolidHeap* create(u32 size, JKRHeap* parent, bool errorFlag);

	void* allocFromHead(u32 size, int align);
	void* allocFromTail(u32 size, int align);

private:
	// _00     = VTBL
	// _00-_68 = JKRHeap
	u32 mFreeSize;   // _68
	void* mCurStart; // _6c
	void* mCurEnd;   // _70
	struct UnknownStruct {
		char unk0[0x4];
		char unk4[0x4];
		char unk8[0x4];
		void* unkC;
		UnknownStruct* unk10;
	};
	UnknownStruct* unk74;
};

inline JKRSolidHeap* JKRCreateStdHeap(u32 size, JKRHeap* parent, bool errorFlag)
{
	return JKRSolidHeap::create(size, parent, errorFlag);
}

#endif
