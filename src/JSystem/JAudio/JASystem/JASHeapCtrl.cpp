#include <JSystem/JAudio/JASystem/JASHeapCtrl.hpp>
#include <macros.h>
#include <types.h>

namespace JASystem {

namespace Kernel {

	u32 global_id = 0;

	THeap::THeap()
	    : mBase(0)
	    , unkC(0)
	    , unk10(0)
	{
		unk4  = global_id++;
		unk0  = 0;
		unk1  = 1;
		unk2  = 0;
		unk14 = 0;
		unk18 = 0;
		mNext = 0;
		unk20 = 0;
		unk24 = 0;
		unk28 = 0;
	}

	void* THeap::getUnlockHeap() { return 0; }

	void THeap::checkAlloc() { }

	void THeap::init() { }

	void THeap::selfInit(u32, u32, u32) { }

	BOOL THeap::selfAlloc(THeap* param_1, u32 param_2, u32 param_3)
	{
		if (mBase != 0 && mBase != (u8*)0xffffffff)
			return 0;

		mBase = (u8*)param_3;
		unk10 = param_2;
		unkC  = 0;
		unk0  = 0;
		unk1  = param_1->unk1;
		unk2  = 0;
		unk14 = nullptr;
		unk18 = param_1;

		if (param_1->unk14 == nullptr) {
			param_1->unk14 = this;
			mNext          = nullptr;
			param_1->unkC  = unk10 + (mBase - param_1->mBase);
		} else {
			THeap* it = param_1->unk14;
			if (mBase < param_1->unk14->mBase) {
				mNext          = param_1->unk14;
				param_1->unk14 = this;
			} else {
				for (;; it = it->mNext) {
					if (!it->mNext) {
						mNext         = nullptr;
						it->mNext     = this;
						param_1->unkC = unk10 + (mBase - param_1->mBase);
						break;
					}

					if (mBase < it->mNext->mBase) {
						mNext     = it->mNext;
						it->mNext = this;
						break;
					}
				}
			}
		}

		++param_1->unk2;
		return 1;
	}

	void THeap::setGroupHeap(THeap*) { }

	void THeap::cutDown() { }

	void THeap::initMotherHeap(u32 param_1, u32 param_2, u8 param_3)
	{
		mBase = (u8*)ALIGN_NEXT(param_1, 0x20);
		unkC  = 0;
		unk10 = param_2 - (param_1 & 0x1F);
		unk4  = global_id++;
		unk0  = 1;
		unk1  = param_3;
		unk2  = 0;
		unk14 = 0;
		unk18 = 0;
		mNext = 0;
		unk20 = 0;
		unk24 = 0;
		unk28 = 0;
	}

	BOOL THeap::allocHeapCheck(THeap* param_1, u32 param_2)
	{
		u8* uVar3;
		THeap* it;
		THeap* bestCandidate;
		u8* in_r11;
		u32 bestCandidateLeftover;
		u32 targetSize;

		targetSize = ALIGN_NEXT(param_2, 0x20);

		if (param_1->mBase == 0)
			return false;

		if (mBase != 0 && mBase != (void*)0xffffffff)
			return false;

		if (param_1->unk10 - param_1->unkC < targetSize) {
			bestCandidateLeftover = 0xfffffff;
			bestCandidate         = nullptr;
			uVar3                 = param_1->mBase;

			for (it = param_1->unk14;; it = it->mNext) {
				if (!it)
					break;

				u32 availSpace = it->mBase - uVar3;
				if (availSpace >= targetSize
				    && availSpace - targetSize < bestCandidateLeftover) {
					bestCandidate         = it;
					in_r11                = uVar3;
					bestCandidateLeftover = availSpace - targetSize;
				}
				uVar3 = it->mBase + it->unk10;
			}

			if (bestCandidate == nullptr)
				return false;

			if (bestCandidate == param_1->unk14) {
				mNext          = param_1->unk14;
				param_1->unk14 = this;
			} else {
				for (THeap* it = param_1->unk14;; it = it->mNext) {
					if (it->mNext != bestCandidate)
						continue;

					mNext     = it->mNext;
					it->mNext = this;
					break;
				}
			}

			mBase = in_r11;
			unk10 = targetSize;
			unkC  = 0;
			unk0  = 0;
			unk1  = param_1->unk1;
			unk2  = 0;
			unk14 = 0;
			unk18 = param_1;
			++param_1->unk2;
			return true;
		}

		mBase = param_1->mBase + param_1->unkC;
		unk10 = targetSize;
		unkC  = 0;
		unk0  = 0;
		unk1  = param_1->unk1;
		unk2  = 0;
		unk14 = 0;
		unk18 = param_1;

		if (param_1->unk14 == 0) {
			param_1->unk14 = this;
			mNext          = 0;
		} else {
			for (it = param_1->unk14;; it = it->mNext) {
				if (it->mNext != nullptr)
					continue;

				it->mNext = this;
				break;
			}
		}

		mNext = 0;
		param_1->unkC += targetSize;
		++param_1->unk2;
		return true;
	}

	void* THeap::alloc(THeap* param_1, u32 param_2)
	{
		if (!allocHeapCheck(param_1, param_2))
			return nullptr;

		return mBase;
	}

	BOOL THeap::free()
	{
		THeap* it;

		if (mBase == 0)
			return false;

		for (it = unk14; it != nullptr;) {
			THeap* next = it->mNext;
			it->free();
			it = next;
		}
		unk14 = nullptr;

		for (it = unk24; it != nullptr;) {
			THeap* next = it->unk28;
			it->free();
			it = next;
		}
		unk24 = nullptr;

		if (unk18 != 0) {
			if (unk18->unk14 == this) {
				unk18->unk14 = mNext;
				if (mNext == nullptr)
					unk18->unkC = 0;
			} else {
				for (it = unk18->unk14;; it = it->mNext) {
					if (it == nullptr) {
						mBase = 0;
						return false;
					}

					if (it->mNext != this)
						continue;

					it->mNext = mNext;

					if (mNext == nullptr)
						unk18->unkC = (it->mBase + it->unk10) - unk18->mBase;

					break;
				}
			}
			--unk18->unk2;
		}

		if (unk20 != 0) {
			if (unk20->unk24 == this) {
				unk20->unk24 = unk28;
			} else {
				for (it = unk20->unk24;; it = it->unk28) {
					if (it == nullptr)
						return false;

					if (it->unk28 != this)
						continue;

					it->unk28 = unk28;

					break;
				}
			}
			unk20 = 0;
			unk28 = 0;
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
	    , mEnd(0)
	    , mSize(0)
	    , unkC(0)
	    , unk10(0)
	{
	}

	TSolidHeap::TSolidHeap(u8* param_1, s32 param_2) { init(param_1, param_2); }

	void* TSolidHeap::alloc(s32 size)
	{
		u32 alignedSize = ALIGN_NEXT(size, 0x20);
		if (mStart == 0)
			return 0;

		u8* result = nullptr;
		if (mEnd + alignedSize <= mStart + mSize) {
			result = mEnd;
			mEnd += alignedSize;
		} else {
			return result;
		}

		unkC++;
		unk10 = result;
		return result;
	}

	void TSolidHeap::freeLast() { }

	void TSolidHeap::freeAll() { }

	void TSolidHeap::init(u8* mem_start, s32 mem_size)
	{
		unkC = 0;
		if (!mem_start) {
			mSize = 0;
			mEnd  = 0;
			unk10 = 0;
		} else {
			mStart = (u8*)ALIGN_NEXT((u32)mem_start, 0x20);
			mEnd   = mStart;
			mSize  = mem_size - ((u32)mem_start & 0x1F);
			unk10  = 0;
		}
	}

	u32 TSolidHeap::getRemain() { return mSize - (mEnd - mStart); }

} // namespace Kernel

} // namespace JASystem
