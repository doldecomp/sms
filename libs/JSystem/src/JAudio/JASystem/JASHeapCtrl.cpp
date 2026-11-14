#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <macros.h>
#include <dolphin/types.h>
#include <stdint.h>

namespace JASystem {

namespace Kernel {

	u32 global_id = 0;

	THeap::THeap()
	    : mBase(0)
	    , mEndOffset(0)
	    , mSize(0)
	{
		mId               = global_id++;
		unk0              = 0;
		unk1              = 1;
		mChildCount       = 0;
		mFirstChild       = 0;
		mParent           = 0;
		mNextSibling      = 0;
		mGroupOwner       = 0;
		mFirstGroupMember = 0;
		mNextGroupMember  = 0;
	}

	void* THeap::getUnlockHeap() { return 0; }

	void THeap::checkAlloc() { }

	void THeap::init() { }

	void THeap::selfInit(u32, u32, u32) { }

	BOOL THeap::selfAlloc(THeap* parent, u32 size, uintptr_t address)
	{
		if (mBase != 0 && mBase != (u8*)0xffffffff)
			return 0;

		mBase       = (u8*)address;
		mSize       = size;
		mEndOffset  = 0;
		unk0        = 0;
		unk1        = parent->unk1;
		mChildCount = 0;
		mFirstChild = nullptr;
		mParent     = parent;

		if (parent->mFirstChild == nullptr) {
			parent->mFirstChild = this;
			mNextSibling        = nullptr;
			parent->mEndOffset  = mSize + (mBase - parent->mBase);
		} else {
			THeap* it = parent->mFirstChild;
			if (mBase < parent->mFirstChild->mBase) {
				mNextSibling        = parent->mFirstChild;
				parent->mFirstChild = this;
			} else {
				for (;; it = it->mNextSibling) {
					if (!it->mNextSibling) {
						mNextSibling       = nullptr;
						it->mNextSibling   = this;
						parent->mEndOffset = mSize + (mBase - parent->mBase);
						break;
					}

					if (mBase < it->mNextSibling->mBase) {
						mNextSibling     = it->mNextSibling;
						it->mNextSibling = this;
						break;
					}
				}
			}
		}

		++parent->mChildCount;
		return 1;
	}

	void THeap::setGroupHeap(THeap*) { }

	void THeap::cutDown() { }

	void THeap::initMotherHeap(uintptr_t address, u32 size, u8 param_3)
	{
		mBase             = (u8*)ALIGN_NEXT(address, 0x20);
		mEndOffset        = 0;
		mSize             = size - (address & 0x1F);
		mId               = global_id++;
		unk0              = 1;
		unk1              = param_3;
		mChildCount       = 0;
		mFirstChild       = 0;
		mParent           = 0;
		mNextSibling      = 0;
		mGroupOwner       = 0;
		mFirstGroupMember = 0;
		mNextGroupMember  = 0;
	}

	BOOL THeap::allocHeapCheck(THeap* parent, u32 size)
	{
		u8* gapStart;
		THeap* it;
		THeap* bestNextSibling;
		u8* bestGapStart;
		u32 bestCandidateLeftover;
		u32 targetSize;

		targetSize = ALIGN_NEXT(size, 0x20);

		if (parent->mBase == 0)
			return false;

		if (mBase != 0 && mBase != (void*)0xffffffff)
			return false;

		if (parent->mSize - parent->mEndOffset < targetSize) {
			bestCandidateLeftover = 0xfffffff;
			bestNextSibling       = nullptr;
			gapStart              = parent->mBase;

			for (it = parent->mFirstChild;; it = it->mNextSibling) {
				if (!it)
					break;

				u32 availSpace = it->mBase - gapStart;
				if (availSpace >= targetSize
				    && availSpace - targetSize < bestCandidateLeftover) {
					bestNextSibling       = it;
					bestGapStart          = gapStart;
					bestCandidateLeftover = availSpace - targetSize;
				}
				gapStart = it->mBase + it->mSize;
			}

			if (bestNextSibling == nullptr)
				return false;

			if (bestNextSibling == parent->mFirstChild) {
				mNextSibling        = parent->mFirstChild;
				parent->mFirstChild = this;
			} else {
				for (THeap* it = parent->mFirstChild;; it = it->mNextSibling) {
					if (it->mNextSibling != bestNextSibling)
						continue;

					mNextSibling     = it->mNextSibling;
					it->mNextSibling = this;
					break;
				}
			}

			mBase       = bestGapStart;
			mSize       = targetSize;
			mEndOffset  = 0;
			unk0        = 0;
			unk1        = parent->unk1;
			mChildCount = 0;
			mFirstChild = 0;
			mParent     = parent;
			++parent->mChildCount;
			return true;
		}

		mBase       = parent->mBase + parent->mEndOffset;
		mSize       = targetSize;
		mEndOffset  = 0;
		unk0        = 0;
		unk1        = parent->unk1;
		mChildCount = 0;
		mFirstChild = 0;
		mParent     = parent;

		if (parent->mFirstChild == 0) {
			parent->mFirstChild = this;
			mNextSibling        = 0;
		} else {
			for (it = parent->mFirstChild;; it = it->mNextSibling) {
				if (it->mNextSibling != nullptr)
					continue;

				it->mNextSibling = this;
				break;
			}
		}

		mNextSibling = 0;
		parent->mEndOffset += targetSize;
		++parent->mChildCount;
		return true;
	}

	void* THeap::alloc(THeap* parent, u32 size)
	{
		if (!allocHeapCheck(parent, size))
			return nullptr;

		return mBase;
	}

	BOOL THeap::free()
	{
		THeap* it;

		if (mBase == 0)
			return false;

		for (it = mFirstChild; it != nullptr;) {
			THeap* next = it->mNextSibling;
			it->free();
			it = next;
		}
		mFirstChild = nullptr;

		for (it = mFirstGroupMember; it != nullptr;) {
			THeap* next = it->mNextGroupMember;
			it->free();
			it = next;
		}
		mFirstGroupMember = nullptr;

		if (mParent != 0) {
			if (mParent->mFirstChild == this) {
				mParent->mFirstChild = mNextSibling;
				if (mNextSibling == nullptr)
					mParent->mEndOffset = 0;
			} else {
				for (it = mParent->mFirstChild;; it = it->mNextSibling) {
					if (it == nullptr) {
						mBase = 0;
						return false;
					}

					if (it->mNextSibling != this)
						continue;

					it->mNextSibling = mNextSibling;

					if (mNextSibling == nullptr)
						mParent->mEndOffset
						    = (it->mBase + it->mSize) - mParent->mBase;

					break;
				}
			}
			--mParent->mChildCount;
		}

		if (mGroupOwner != 0) {
			if (mGroupOwner->mFirstGroupMember == this) {
				mGroupOwner->mFirstGroupMember = mNextGroupMember;
			} else {
				for (it = mGroupOwner->mFirstGroupMember;;
				     it = it->mNextGroupMember) {
					if (it == nullptr)
						return false;

					if (it->mNextGroupMember != this)
						continue;

					it->mNextGroupMember = mNextGroupMember;

					break;
				}
			}
			mGroupOwner      = 0;
			mNextGroupMember = 0;
		}

		mBase = 0;
		return true;
	}

	void THeap::garbageCollectionSt() { }

	void THeap::checkFreeHeapTotal() { }

	void THeap::checkFreeHeapLinear() { }

	void THeap::show(u32) { }

	void THeap::moveChildren(s32) { }

	void aramFinish(u32) { }

	void dmaAramToAram(u32, u32, u32) { }

	void dmaDramToDram(u32, u32, u32) { }

	TSolidHeap::TSolidHeap()
	    : mStart(0)
	    , mCurrent(0)
	    , mSize(0)
	    , mAllocCount(0)
	    , mLastAlloc(0)
	{
	}

	TSolidHeap::TSolidHeap(u8* mem_start, s32 mem_size)
	{
		init(mem_start, mem_size);
	}

	void* TSolidHeap::alloc(s32 size)
	{
		u32 alignedSize = ALIGN_NEXT(size, 0x20);
		if (mStart == 0)
			return 0;

		u8* result = nullptr;
		if (mCurrent + alignedSize <= mStart + mSize) {
			result = mCurrent;
			mCurrent += alignedSize;
		} else {
			return result;
		}

		mAllocCount++;
		mLastAlloc = result;
		return result;
	}

	void TSolidHeap::freeLast() { }

	void TSolidHeap::freeAll() { }

	void TSolidHeap::init(u8* mem_start, s32 mem_size)
	{
		mAllocCount = 0;
		if (!mem_start) {
			mSize      = 0;
			mCurrent   = 0;
			mLastAlloc = 0;
		} else {
			mStart     = (u8*)ALIGN_NEXT((uintptr_t)mem_start, 0x20);
			mCurrent   = mStart;
			mSize      = mem_size - ((uintptr_t)mem_start & 0x1F);
			mLastAlloc = 0;
		}
	}

	u32 TSolidHeap::getRemain() { return mSize - (mCurrent - mStart); }

} // namespace Kernel

} // namespace JASystem
