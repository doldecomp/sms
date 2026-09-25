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

	virtual ~JKRExpHeap();
	virtual void* alloc(u32 size, int alignment);
	virtual void free(void* ptr);
	virtual void freeTail();
	virtual void freeAll();
	virtual s32 resize(void* ptr, u32 size);
	virtual s32 getSize(void* ptr);
	virtual s32 getFreeSize();
	virtual u32 getHeapType() { return 'EXPH'; }
	virtual s32 getTotalFreeSize();
	virtual bool check();
	virtual bool dump_sort();
	virtual bool dump();
	virtual s32 changeGroupID(u8 groupId);
	virtual u8 getCurrentGroupId() { return mCurrentGroupID; }
	virtual void state_register(TState*, u32) const;
	virtual bool state_compare(const TState& fst, const TState& snd) const;

	void* allocFromHead(u32 size, int align);
	void* allocFromHead(u32 size);
	void* allocFromTail(u32 size, int align);
	void* allocFromTail(u32 size);
	void appendUsedList(CMemBlock*);
	static JKRExpHeap* create(u32 size, JKRHeap* parent, bool errorFlag);
	static JKRExpHeap* create(void*, u32, JKRHeap*, bool);
	static JKRExpHeap* createRoot(int, bool);
	int freeLevel(u8 level);
	int freeGroup(u8 groupID);
	int countGroup(u8 groupID);
	s32 getTotalUsedSize() const;
	void DBshow();
	void joinTwoBlocks(CMemBlock*);
	void recycleFreeBlock(CMemBlock*);
	void removeFreeBlock(CMemBlock*);
	void setFreeBlock(CMemBlock*, CMemBlock*, CMemBlock*);
	void destroy();

	void removeUsedBlock(CMemBlock*);
	bool isEmpty();
	s32 getUsedSize(u8 groupId) const;

	CMemBlock* getHeadUsedList() const { return mHeadUsedList; }
	void setAllocationMode(EAllocMode mode) { mCurrentAllocMode = mode; }

	static s32 getUsedSize_(JKRExpHeap* expHeap)
	{
		return expHeap->mSize - expHeap->getTotalFreeSize();
	}
	static void* getState_(TState* state) { return getState_buf_(state); }

private:
	/* 0x68 */ u8 mCurrentAllocMode;
	/* 0x69 */ u8 mCurrentGroupID;
	/* 0x6A */ u8 mIsRoot;
	/* 0x6C */ void* _70;
	/* 0x70 */ u32 _74;
	/* 0x74 */ CMemBlock* mHead;
	/* 0x78 */ CMemBlock* mTail;
	/* 0x8C */ CMemBlock* mHeadUsedList;
	/* 0x80 */ CMemBlock* mTailUsedList;
};

inline JKRExpHeap* JKRCreateExpHeap(u32 size, JKRHeap* parent, bool errorFlag)
{
	return JKRExpHeap::create(size, parent, errorFlag);
}

#endif
