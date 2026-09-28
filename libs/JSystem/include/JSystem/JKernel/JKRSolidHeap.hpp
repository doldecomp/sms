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

	static JKRSolidHeap* createRoot(int, bool);
	static JKRSolidHeap* create(u32 size, JKRHeap* parent, bool errorFlag);
	void destroy();
	s32 adjustSize();
	void recordState(u32);
	void restoreState(u32);

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
	// Names from MKDD, which keeps recordState/restoreState in its binary.
	struct State {
		/* 0x00 */ u32 mId;
		/* 0x04 */ u32 mFreeSize;
		/* 0x08 */ void* mCurStart;
		/* 0x0C */ void* mCurEnd;
		/* 0x10 */ State* mNext;
	};
	/* 0x74 */ State* mStateList;
};

inline JKRSolidHeap* JKRCreateSolidHeap(u32 size, JKRHeap* parent,
                                        bool errorFlag)
{
	return JKRSolidHeap::create(size, parent, errorFlag);
}

#endif
