#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include "dolphin/os.h"

JKRHeap* JKRHeap::sSystemHeap;
JKRHeap* JKRHeap::sCurrentHeap;
JKRHeap* JKRHeap::sRootHeap;
JKRHeapErrorHandler* JKRHeap::mErrorHandler;
void* JKRHeap::mCodeStart;
void* JKRHeap::mCodeEnd;
void* JKRHeap::mUserRamStart;
void* JKRHeap::mUserRamEnd;
u32 JKRHeap::mMemorySize;

JKRHeap::JKRHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag)
    : JKRDisposer()
    , mChildTree(this)
    , mDisposerList()
{
	OSInitMutex(&mMutex);
	mSize  = size;
	mStart = (u8*)data;
	mEnd   = ((u8*)data + size);
	if (parent == nullptr) {
		becomeSystemHeap();
		becomeCurrentHeap();
	} else {
		parent->mChildTree.appendChild(&mChildTree);
		if (sSystemHeap == sRootHeap)
			becomeSystemHeap();
		if (sCurrentHeap == sRootHeap)
			becomeCurrentHeap();
	}
	mErrorFlag = errorFlag;
	if (mErrorFlag == true && mErrorHandler == nullptr)
		mErrorHandler = JKRDefaultMemoryErrorRoutine;
}

JKRHeap::~JKRHeap()
{
	mChildTree.getParent()->removeChild(&mChildTree);
	JSUTree<JKRHeap>* nextRootHeap = sRootHeap->mChildTree.getFirstChild();
	if (sCurrentHeap == this)
		sCurrentHeap = !nextRootHeap ? sRootHeap : nextRootHeap->getObject();

	if (sSystemHeap == this)
		sSystemHeap = !nextRootHeap ? sRootHeap : nextRootHeap->getObject();
}

bool JKRHeap::initArena(char** outUserRamStart, u32* outUserRamSize,
                        int numHeaps)
{
	void* arenaLo = OSGetArenaLo();
	void* arenaHi = OSGetArenaHi();
	if (arenaLo == arenaHi) {
		return false;
	}
	void* arenaStart = OSInitAlloc(arenaLo, arenaHi, numHeaps);
	arenaHi          = (u8*)OSRoundDown32B(arenaHi);
	arenaLo          = (u8*)OSRoundUp32B(arenaStart);
	u8* start        = (u8*)OSPhysicalToCached(0);
	mCodeEnd         = (u8*)arenaLo;
	mCodeStart       = (u8*)start;
	mUserRamStart    = (u8*)arenaLo;
	mUserRamEnd      = (u8*)arenaHi;
	mMemorySize      = *(u32*)((start + 0x28));
	OSReport("%x %x %x %x %x\n", mCodeStart, mCodeEnd, mUserRamStart,
	         mUserRamEnd, mMemorySize);
	OSSetArenaLo(arenaHi);
	OSSetArenaHi(arenaHi);
	*outUserRamStart = (char*)arenaLo;
	*outUserRamSize  = (u32)arenaHi - (u32)arenaLo;
	OSReport("arenaLo=%x arenaHi=%x \n", arenaLo, arenaHi);
	return true;
}

JKRHeap* JKRHeap::becomeSystemHeap()
{
	JKRHeap* old = sSystemHeap;
	sSystemHeap  = this;
	return old;
}

JKRHeap* JKRHeap::becomeCurrentHeap()
{
	JKRHeap* old = sCurrentHeap;
	sCurrentHeap = this;
	return old;
}

void* JKRHeap::alloc(u32 byteCount, int padding, JKRHeap* heap)
{
	void* memory = nullptr;
	if (heap) {
		memory = heap->alloc(byteCount, padding);
	} else if (sCurrentHeap) {
		memory = sCurrentHeap->alloc(byteCount, padding);
	}
	return memory;
}

void JKRHeap::free(void* memory, JKRHeap* heap)
{
	if (heap != nullptr || (heap = findFromRoot(memory)) != nullptr)
		heap->free(memory);
}

void JKRHeap::freeAll()
{
	JUT_WARNING_F(417, !mInitFlag, "freeAll in heap %x", this);
	JSUListIterator<JKRDisposer> iterator;
	while (iterator = mDisposerList.getFirst(),
	       iterator != mDisposerList.getEnd()) {
		iterator.getObject()->~JKRDisposer();
	}
}

s32 JKRHeap::changeGroupID(u8 newGroupID) { return 0; }

u8 JKRHeap::getCurrentGroupId() { return 0; }

JKRHeap* JKRHeap::findFromRoot(void* ptr)
{
	if (sRootHeap != nullptr)
		return sRootHeap->find(ptr);

	return nullptr;
}

JKRHeap* JKRHeap::find(void* memory) const
{
	if ((mStart <= memory) && (memory <= mEnd)) {
		if (mChildTree.getNumChildren() != 0) {
			for (JSUTreeIterator<JKRHeap> iterator(mChildTree.getFirstChild());
			     iterator != mChildTree.getEndChild(); ++iterator) {
				JKRHeap* result = iterator->find(memory);
				if (result) {
					return result;
				}
			}
		}
		return const_cast<JKRHeap*>(this);
	}
	return nullptr;
}

void JKRHeap::dispose_subroutine(u32 begin, u32 end)
{
	JSUListIterator<JKRDisposer> last_iterator;
	JSUListIterator<JKRDisposer> next_iterator;
	JSUListIterator<JKRDisposer> iterator;
	for (iterator = mDisposerList.getFirst();
	     iterator != mDisposerList.getEnd(); iterator = next_iterator) {
		if ((void*)begin <= iterator.getObject()
		    && iterator.getObject() < (void*)end) {
			iterator.getObject()->~JKRDisposer();
			if (last_iterator == nullptr) {
				next_iterator = mDisposerList.getFirst();
			} else {
				next_iterator = last_iterator;
				next_iterator++;
			}
		} else {
			last_iterator = iterator;
			next_iterator = iterator;
			next_iterator++;
		}
	}
}

bool JKRHeap::dispose(void* memory, u32 size)
{
	u32 begin = (u32)memory;
	u32 end   = (u32)memory + size;
	dispose_subroutine(begin, end);
	return false;
}

void JKRHeap::dispose(void* begin, void* end)
{
	dispose_subroutine((u32)begin, (u32)end);
}

void JKRHeap::dispose()
{
	JSUListIterator<JKRDisposer> iterator;
	while (iterator = mDisposerList.getFirst(),
	       iterator != mDisposerList.getEnd()) {
		iterator->~JKRDisposer();
	}
}

void JKRHeap::copyMemory(void* dst, void* src, u32 size)
{
	u32 count = (size + 3) / 4;

	u32* dst_32 = (u32*)dst;
	u32* src_32 = (u32*)src;
	while (count > 0) {
		*dst_32 = *src_32;
		dst_32++;
		src_32++;
		count--;
	}
}

void JKRDefaultMemoryErrorRoutine(void* heap, u32 size, int alignment)
{
	OSErrorLine(694, "abort\n");
}

void* operator new(u32 byteCount)
{
	return JKRHeap::alloc(byteCount, 4, nullptr);
}
void* operator new(u32 byteCount, int alignment)
{
	return JKRHeap::alloc(byteCount, alignment, nullptr);
}
void* operator new(u32 byteCount, JKRHeap* heap, int alignment)
{
	return JKRHeap::alloc(byteCount, alignment, heap);
}

void* operator new[](u32 byteCount)
{
	return JKRHeap::alloc(byteCount, 4, nullptr);
}
void* operator new[](u32 byteCount, int alignment)
{
	return JKRHeap::alloc(byteCount, alignment, nullptr);
}
void* operator new[](u32 byteCount, JKRHeap* heap, int alignment)
{
	return JKRHeap::alloc(byteCount, alignment, heap);
}

// this is not needed without the other pragma and asm bs
void operator delete(void* memory) { JKRHeap::free(memory, nullptr); }
void operator delete[](void* memory) { JKRHeap::free(memory, nullptr); }

void JKRHeap::state_register(JKRHeap::TState* p, u32) const
{
#line 1132
	JUT_ASSERT(p != 0);
	JUT_ASSERT(p->getHeap() == this);
}

bool JKRHeap::state_compare(const JKRHeap::TState& r1,
                            const JKRHeap::TState& r2) const
{
#line 1141
	JUT_ASSERT(r1.getHeap() == r2.getHeap());
	return (r1.getCheckCode() == r2.getCheckCode());
}

void JKRHeap::state_dump(const TState& state) const
{
	JUT_LOG_F(1165, "check-code : 0x%08x", state.getCheckCode());
	JUT_LOG_F(1166, "id         : 0x%08x", state.getId());
	JUT_LOG_F(1167, "used size  : %u", state.getUsedSize());
}
