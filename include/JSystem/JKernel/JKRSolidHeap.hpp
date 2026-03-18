#ifndef JKR_SOLID_HEAP_HPP
#define JKR_SOLID_HEAP_HPP

#include <JSystem/JKernel/JKRHeap.hpp>
#include <dolphin/os.h>

class JKRSolidHeap : public JKRHeap {
public:
	JKRSolidHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);

	virtual ~JKRSolidHeap();
	virtual void* alloc(u32 size, int alignment);
	virtual void free(void* ptr);
	virtual void freeAll();
	virtual void freeTail();
	virtual s32 resize(void* ptr, u32 size);
	virtual s32 getSize(void* ptr);
	virtual s32 getFreeSize() { return mFreeSize; }
	virtual s32 getTotalFreeSize() { return getFreeSize(); }
	virtual u32 getHeapType() { return 'SLID'; }
	virtual bool check();
	virtual bool dump();
	virtual void state_register(TState*, u32) const;
	virtual bool state_compare(const TState& fst, const TState& snd) const;

	static JKRSolidHeap* create(u32 size, JKRHeap* parent, bool errorFlag);

	void* allocFromHead(u32 size, int align);
	void* allocFromTail(u32 size, int align);

	// fabricated
	static s32 getUsedSize_(JKRSolidHeap* solidHeap)
	{
		return solidHeap->mSize - solidHeap->getTotalFreeSize();
	}

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

inline JKRSolidHeap* JKRCreateSolidHeap(u32 size, JKRHeap* parent,
                                        bool errorFlag)
{
	return JKRSolidHeap::create(size, parent, errorFlag);
}

#endif
