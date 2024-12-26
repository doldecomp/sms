#ifndef JKR_EXP_HEAP_HPP
#define JKR_EXP_HEAP_HPP

#include <JSystem/JKernel/JKRHeap.hpp>

class JKRExpHeap : public JKRHeap {
public:
	class CMemBlock {
	public:
		CMemBlock* allocBack(u32, u8, u8, u8, u8);
		CMemBlock* allocFore(u32, u8, u8, u8, u8);
		void* free(JKRExpHeap*);
		static CMemBlock* getHeapBlock(void*);
		void initiate(CMemBlock*, CMemBlock*, u32, u8, u8);

		void newGroupId(u8 groupId) { mGroupID = groupId; }
		bool isValid() const { return mUsageHeader == 'HM'; }
		bool _isTempMemBlock() const { return (mFlags & 0x80); }
		int getAlignment() const { return mFlags & 0x7f; }
		void* getContent() const { return (void*)(this + 1); }
		CMemBlock* getPrevBlock() const { return mPrev; }
		CMemBlock* getNextBlock() const { return mNext; }
		u32 getSize() const { return mAllocatedSpace; }
		u8 getGroupId() const { return mGroupID; }
		static CMemBlock* getBlock(void* data)
		{
			return (CMemBlock*)((u32)data + -0x10);
		}

		u16 mUsageHeader;    // _00
		u8 mFlags;           // _02, a|bbbbbbb = a=temp, b=aln
		u8 mGroupID;         // _03
		int mAllocatedSpace; // _04
		CMemBlock* mPrev;    // _08
		CMemBlock* mNext;    // _0C
	};

	JKRExpHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);

	virtual ~JKRExpHeap();                                     // _08
	virtual void* alloc(u32 size, int alignment);              // _10
	virtual void free(void* ptr);                              // _14
	virtual void freeTail();                                   // _18
	virtual void freeAll();                                    // _1C
	virtual s32 resize(void* ptr, u32 size);                   // _20
	virtual s32 getSize(void* ptr);                            // _24
	virtual s32 getFreeSize();                                 // _28
	virtual u32 getHeapType() { return 'EXPH'; }               // _30 (weak)
	virtual s32 getTotalFreeSize();                            // _2C
	virtual bool check();                                      // _34
	virtual bool dump_sort();                                  // _38
	virtual bool dump();                                       // _3C
	virtual s32 changeGroupID(u8 groupId);                     // _40
	virtual u8 getCurrentGroupId() { return mCurrentGroupID; } // _44 (weak)
	virtual void state_register(TState*, u32) const;           // _48
	virtual bool state_compare(const TState& fst,
	                           const TState& snd) const; // _4C

	void* allocFromHead(u32 size, int align);
	void* allocFromHead(u32 size);
	void* allocFromTail(u32 size, int align);
	void* allocFromTail(u32 size);
	void appendUsedList(CMemBlock*);
	static JKRExpHeap* create(u32 size, JKRHeap* parent, bool errorFlag);
	static JKRExpHeap* create(void*, u32, JKRHeap*, bool);
	static JKRExpHeap* createRoot(int, bool);
	int freeGroup(u8 groupID);
	void joinTwoBlocks(CMemBlock*);
	void recycleFreeBlock(CMemBlock*);
	void removeFreeBlock(CMemBlock*);
	void setFreeBlock(CMemBlock*, CMemBlock*, CMemBlock*);
	void destroy();

	// unused/inlined:
	void removeUsedBlock(CMemBlock*);
	bool isEmpty();
	s32 getUsedSize(u8 groupId) const;

	CMemBlock* getHeadUsedList() const { return mHeadUsedList; }
	void setAllocationMode(EAllocMode mode) { mCurrentAllocMode = mode; }

	static s32 getUsedSize_(JKRExpHeap* expHeap)
	{
		// s32 totalFreeSize = expHeap->getTotalFreeSize();
		return expHeap->mSize - expHeap->getTotalFreeSize();
	}
	static u32 getState_(TState* state)
	{
		return getState_buf_(state);
	} // might instead be a pointer to a next state?

private:
	// _00     = VTBL
	// _00-_68 = JKRHeap
	u8 mCurrentAllocMode;     // _68
	u8 mCurrentGroupID;       // _69
	u8 mIsRoot;               // _6A
	void* _70;                // _6C
	u32 _74;                  // _70
	CMemBlock* mHead;         // _74, free list
	CMemBlock* mTail;         // _78, free list
	CMemBlock* mHeadUsedList; // _8C
	CMemBlock* mTailUsedList; // _80
};

inline JKRExpHeap* JKRCreateExpHeap(u32 size, JKRHeap* parent, bool errorFlag)
{
	return JKRExpHeap::create(size, parent, errorFlag);
}

#endif
