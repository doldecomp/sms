#ifndef JKR_HEAP_H
#define JKR_HEAP_H

#include <types.h>
#include <dolphin/os/OSMutex.h>
#include <JSystem/JKernel/JKRDisposer.hpp>
#include <new>

typedef void JKRHeapErrorHandler(void*, u32, int);

class JKRHeap : public JKRDisposer {
public:
	enum EAllocMode {
		HEAPALLOC_Unk1 = 1,
	};

	// NOTE: half guesswork
	struct TState {
		TState(const JKRHeap* heap, u32 id, bool isCompareOnDestructed);
		TState(const JKRHeap* heap, bool isCompareOnDestructed);
		~TState();

		u32 getUsedSize() const { return mUsedSize; }
		u32 getCheckCode() const { return mCheckCode; }
		const JKRHeap* getHeap() const { return mHeap; }
		u32 getId() const { return mId; }
		bool isCompareOnDestructed() const { return mIsCompareOnDestructed; }

		/* 0x00 */ u32 mBuf;
		/* 0x04 */ u32 mUsedSize;
		/* 0x08 */ u32 mCheckCode;
		/* 0x0C */ u32 mId;
		/* 0x10 */ const JKRHeap* mHeap;
		/* 0x14 */ BOOL mIsCompareOnDestructed;
	};

public:
	JKRHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag);

	bool setErrorFlag(bool errorFlag);

	virtual ~JKRHeap();
	virtual void* alloc(u32 size, int alignment) = 0;
	virtual void free(void* ptr)                 = 0;
	virtual void freeAll();
	virtual void freeTail()                 = 0;
	virtual s32 resize(void* ptr, u32 size) = 0;
	virtual s32 getSize(void* ptr)          = 0;
	virtual s32 getFreeSize()               = 0;
	virtual s32 getTotalFreeSize()          = 0;
	virtual u32 getHeapType()               = 0;
	virtual bool check()                    = 0;
	virtual bool dump_sort() { return true; }
	virtual bool dump() = 0;
	virtual s32 changeGroupID(u8 newGroupId) { return 0; }
	virtual u8 getCurrentGroupId() { return 0; }
	virtual void state_register(JKRHeap::TState*, u32) const;
	virtual bool state_compare(JKRHeap::TState const&,
	                           JKRHeap::TState const&) const;
	virtual void state_dump(JKRHeap::TState const&) const;

	JKRHeap* becomeSystemHeap();
	JKRHeap* becomeCurrentHeap();
	void destroy();
	void fillFreeArea();
	static s32 resize(void*, u32, JKRHeap*);
	static s32 getSize(void*, JKRHeap*);
	static void fillMemory(u8*, u32, u8);
	static JKRHeapErrorHandler* setErrorHandler(JKRHeapErrorHandler*);

	void* getMaxFreeBlock();
	u32 getMaxAllocatableSize(int alignment);
	JKRHeap* find(void*) const;
	void dispose_subroutine(u32 begin, u32 end);
	bool dispose(void*, u32);
	void dispose(void*, void*);
	void dispose();

	void appendDisposer(JKRDisposer* disposer)
	{
		mDisposerList.append(&disposer->mPointerLinks);
	}

	void removeDisposer(JKRDisposer* disposer)
	{
		mDisposerList.remove(&disposer->mPointerLinks);
	}

	void* getStartAddr() const { return (void*)mStart; }
	void* getEndAddr() const { return (void*)mEnd; }
	u32 getHeapSize() const { return mSize; }
	bool getErrorFlag() const { return mErrorFlag; }
	void callErrorHandler(JKRHeap* heap, u32 size, int alignment)
	{
		if (mErrorHandler) {
			(*mErrorHandler)(heap, size, alignment);
		}
	}

	// TState related
	static void* getState_buf_(TState* state) { return &state->mBuf; }
	static void setState_u32ID_(TState* state, u32 id) { state->mId = id; }
	static void setState_uUsedSize_(TState* state, u32 usedSize)
	{
		state->mUsedSize = usedSize;
	}
	static void setState_u32CheckCode_(TState* state, u32 checkCode)
	{
		state->mCheckCode = checkCode;
	}

	void lock() const { OSLockMutex(const_cast<OSMutex*>(&mMutex)); }
	void unlock() const { OSUnlockMutex(const_cast<OSMutex*>(&mMutex)); }

	JKRHeap* getParent() { return mChildTree.getParent()->getObject(); }

	const JSUTree<JKRHeap>& getHeapTree() { return mChildTree; }

	void checkMemoryFilled(u8*, u32 size, u8);

	static bool initArena(char**, u32*, int);
	static void* alloc(u32 byteCount, int padding, JKRHeap* heap);
	static void copyMemory(void* dst, void* src, u32 size);
	static void free(void*, JKRHeap*);
	static void state_dumpDifference(const TState&, const TState&);
	static JKRHeap* findFromRoot(void*);

	static void* getCodeStart() { return mCodeStart; }

	static void* getCodeEnd() { return mCodeEnd; }

	static void* getUserRamStart() { return mUserRamStart; }

	static void* getUserRamEnd() { return mUserRamEnd; }

	static u32 getMemorySize() { return mMemorySize; }

	static JKRHeap* getCurrentHeap() { return sCurrentHeap; }

	static JKRHeap* getRootHeap() { return sRootHeap; }

	static JKRHeap* getSystemHeap() { return sSystemHeap; }

	static void* mCodeStart;
	static void* mCodeEnd;
	static void* mUserRamStart;
	static void* mUserRamEnd;
	static u32 mMemorySize;

	static JKRHeap* sSystemHeap;
	static JKRHeap* sCurrentHeap;
	static JKRHeap* sRootHeap;

	static JKRHeapErrorHandler* mErrorHandler;

protected:
	/* 0x18 */ OSMutex mMutex;
	/* 0x30 */ void* mStart;
	/* 0x34 */ void* mEnd;
	/* 0x38 */ u32 mSize;
	/* 0x3C */ JSUTree<JKRHeap> mChildTree;
	/* 0x58 */ JSUList<JKRDisposer> mDisposerList;
	/* 0x64 */ bool mErrorFlag;
	/* 0x65 */ bool mInitFlag;
	/* 0x66 */ u8 padding_0x6a[2];
};

inline JKRHeap* JKRGetCurrentHeap() { return JKRHeap::getCurrentHeap(); }

inline JKRHeap* JKRGetSystemHeap() { return JKRHeap::getSystemHeap(); }

inline JKRHeap* JKRGetRootHeap() { return JKRHeap::getRootHeap(); }

inline void* JKRAllocFromSysHeap(u32 size, int alignment)
{
	return JKRHeap::getSystemHeap()->alloc(size, alignment);
}

inline void* JKRAllocFromHeap(JKRHeap* heap, u32 size, int alignment)
{
	return JKRHeap::alloc(size, alignment, heap);
}

inline void JKRFree(void* pBuf) { JKRHeap::free(pBuf, nullptr); }

inline void JKRFreeToHeap(JKRHeap* heap, void* ptr)
{
	JKRHeap::free(ptr, heap);
}

inline void JKRFreeToSysHeap(void* buf) { JKRHeap::getSystemHeap()->free(buf); }

void JKRDefaultMemoryErrorRoutine(void*, u32, int);

void* operator new(size_t);
void* operator new(size_t, int);
void* operator new(size_t, JKRHeap*, int);

void* operator new[](size_t);
void* operator new[](size_t, int);
void* operator new[](size_t, JKRHeap*, int);

void operator delete(void*);
void operator delete[](void*);

#endif
