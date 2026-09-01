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

	static void* getState_(TState* state) { return getState_buf_(state); }
	static s32 getUsedSize(JKRSolidHeap* solidHeap)
	{
		return solidHeap->mSize - solidHeap->getTotalFreeSize();
	}

private:
	/* 0x68 */ u32 mFreeSize;
	/* 0x6c */ void* mCurStart;
	/* 0x70 */ void* mCurEnd;
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
