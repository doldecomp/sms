#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <macros.h>
#include <stdint.h>

JKRSolidHeap* JKRSolidHeap::createRoot(int param_1, bool errorFlag)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

JKRSolidHeap* JKRSolidHeap::create(u32 size, JKRHeap* parent, bool errorFlag)
{
	if (!parent)
		parent = sRootHeap;

	const u32 expHeapSize = ALIGN_NEXT(sizeof(JKRSolidHeap), 0x10);
	u32 alignedSize       = ALIGN_PREV(size, 0x10);

	void* ptr     = JKRHeap::alloc(alignedSize, 0x10, parent);
	void* dataPtr = (char*)ptr + expHeapSize;
	if (ptr == nullptr)
		return nullptr;

	return new (ptr)
	    JKRSolidHeap(dataPtr, alignedSize - expHeapSize, parent, errorFlag);
}

void JKRSolidHeap::destroy() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

JKRSolidHeap::JKRSolidHeap(void* data, u32 size, JKRHeap* parent,
                           bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag)
{
	mFreeSize  = mSize;
	mCurStart  = mStart;
	mCurEnd    = mEnd;
	mStateList = nullptr;
}

JKRSolidHeap::~JKRSolidHeap() { dispose(); }

s32 JKRSolidHeap::adjustSize()
{
	JKRHeap* parent = getParent();
	if (!parent) {
		return -1;
	}

	lock();
	u32 headerSize = (u32)mStart - (u32)this;
	u32 newSize    = ALIGN_NEXT((u32)mCurStart - (u32)mStart, 0x20);
	if (parent->resize(this, headerSize + newSize) != -1) {
		mFreeSize = 0;
		mSize     = newSize;
		mEnd      = (void*)((u32)mStart + mSize);
		mCurStart = mEnd;
		mCurEnd   = mEnd;
	}
	unlock();
	return headerSize + newSize;
}

void* JKRSolidHeap::alloc(u32 size, int alignment)
{
	lock();
	if (size < 4)
		size = 4;

	void* ret;

	if (alignment >= 0) {
		ret = allocFromHead(size, alignment < 4 ? 4 : alignment);
	} else {
		ret = allocFromTail(size, -alignment < 4 ? 4 : -alignment);
	}

	unlock();
	return ret;
}

void* JKRSolidHeap::allocFromHead(u32 size, int align)
{
	size = ALIGN_NEXT(size, align);

	void* ret = nullptr;

	char* alignedStart = (char*)ALIGN_NEXT((u32)mCurStart, align);
	u32 requiredSize   = (alignedStart - (char*)mCurStart) + size;
	if (requiredSize <= mFreeSize) {
		mCurStart = (char*)mCurStart + requiredSize;
		mFreeSize -= requiredSize;
		ret = alignedStart;
	} else {
		JUTWarningConsole_f("allocFromHead: cannot alloc memory (0x%x byte).\n",
		                    requiredSize);
		if (mErrorFlag == true && mErrorHandler != nullptr) {
			(*mErrorHandler)(this, size, align);
		}
	}
	return ret;
}

void* JKRSolidHeap::allocFromTail(u32 size, int align)
{
	size = ALIGN_NEXT(size, align);

	void* ret = nullptr;

	char* alignedEnd = (char*)ALIGN_PREV((u32)mCurEnd - size, align);
	u32 requiredSize = (char*)mCurEnd - alignedEnd;
	if (requiredSize <= mFreeSize) {
		mCurEnd = (char*)mCurEnd - requiredSize;
		mFreeSize -= requiredSize;
		ret = alignedEnd;
	} else {
		JUTWarningConsole_f("allocFromTail: cannot alloc memory (0x%x byte).\n",
		                    requiredSize);
		if (mErrorFlag == true && mErrorHandler != nullptr) {
			(*mErrorHandler)(this, size, align);
		}
	}
	return ret;
}

void JKRSolidHeap::free(void* ptr)
{
	JUTWarningConsole_f("free: cannot free memory block (%08x)\n", ptr);
}

void JKRSolidHeap::freeAll()
{
	lock();
	JKRHeap::freeAll();
	mFreeSize  = mSize;
	mCurStart  = mStart;
	mCurEnd    = mEnd;
	mStateList = nullptr;
	unlock();
}

void JKRSolidHeap::freeTail()
{
	lock();
	if (mCurEnd != mEnd)
		dispose(mCurEnd, mEnd);
	mFreeSize += (u8*)mEnd - (u8*)mCurEnd;
	mCurEnd = mEnd;
	for (State* state = mStateList; state != nullptr; state = state->mNext) {
		state->mCurEnd = mEnd;
	}
	unlock();
}

s32 JKRSolidHeap::resize(void* ptr, u32 size)
{
	JUTWarningConsole_f("resize: cannot resize memory block (%08x: %d)\n", ptr,
	                    size);
	return -1;
}

s32 JKRSolidHeap::getSize(void* ptr)
{
	JUTWarningConsole_f("getSize: cannot get memory block size (%08x)\n", ptr);
	return -1;
}

void JKRSolidHeap::recordState(u32 id) { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

void JKRSolidHeap::restoreState(u32 id)
{
	State* state = mStateList;
	lock();
	if (id != 0) {
		while (state != nullptr && id != state->mId) {
			state = state->mNext;
		}
	}
	if (state != nullptr) {
		if (state->mCurStart != mCurStart) {
			dispose(state->mCurStart, mCurStart);
		}
		if (state->mCurEnd != mCurEnd) {
			dispose(mCurEnd, state->mCurEnd);
		}
		mFreeSize  = state->mFreeSize;
		mCurStart  = state->mCurStart;
		mCurEnd    = state->mCurEnd;
		mStateList = state->mNext;
	}
	unlock();
}

bool JKRSolidHeap::check()
{
	lock();

	u32 checkedFreeSize = mFreeSize + ((u8*)mCurStart - (u8*)mStart)
	                      + ((u8*)mEnd - (u8*)mCurEnd);
	bool valid = true;
	if (checkedFreeSize != mSize) {
		valid = false;
		JUTWarningConsole_f("check: bad total memory block size (%08X, %08X)\n",
		                    mSize, checkedFreeSize);
	}

	unlock();
	return valid;
}

bool JKRSolidHeap::dump()
{
	JUTReportConsole("\nJKRSolidHeap dump\n");
	bool ret = check();
	lock();
	u32 checkedFreeSize = mFreeSize + ((u8*)mCurStart - (u8*)mStart)
	                      + ((u8*)mEnd - (u8*)mCurEnd);
	JUTReportConsole("attr  address:   size\n");
	JUTReportConsole_f("head %08x: %08x\n", mStart,
	                   (u8*)mCurStart - (u8*)mStart);
	JUTReportConsole_f("tail %08x: %08x\n", mCurEnd, (u8*)mEnd - (u8*)mCurEnd);

	float pcnt = 100.0f * ((float)checkedFreeSize / (float)mSize);
	JUTReportConsole_f("%d / %d bytes (%6.2f%%) used\n", checkedFreeSize, mSize,
	                   pcnt);
	unlock();
	return ret;
}

void JKRSolidHeap::state_register(TState* p, u32 id) const
{
	JUT_ASSERT(p != nullptr);
	JUT_ASSERT(p->getHeap() == this);

	setState_u32ID_(p, id);
	setState_uUsedSize_(p, getUsedSize((JKRSolidHeap*)this));

	// TODO: r28 is copy-pasted from TP debug but still not enough stack
	char trash[0x4];
	void* r28     = getState_(p);
	u32 checkCode = (uintptr_t)mCurStart;
	checkCode += (uintptr_t)mCurEnd * 3;
	setState_u32CheckCode_(p, checkCode);
}

bool JKRSolidHeap::state_compare(const TState& fst, const TState& snd) const
{
	bool result = true;
	if (fst.getCheckCode() != snd.getCheckCode()) {
		result = false;
	}
	if (fst.getUsedSize() != snd.getUsedSize()) {
		result = false;
	}
	return result;
}
