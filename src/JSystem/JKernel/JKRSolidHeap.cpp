#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <macros.h>

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

JKRSolidHeap::JKRSolidHeap(void* data, u32 size, JKRHeap* parent,
                           bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag)
{
	mFreeSize = mSize;
	mCurStart = mStart;
	mCurEnd   = mEnd;
	unk74     = nullptr;
}

JKRSolidHeap::~JKRSolidHeap() { dispose(); }

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
	mFreeSize = mSize;
	mCurStart = mStart;
	mCurEnd   = mEnd;
	unk74     = nullptr;
	unlock();
}

void JKRSolidHeap::freeTail()
{
	lock();
	if (mCurEnd != mEnd)
		dispose(mCurEnd, mEnd);
	mFreeSize += (u8*)mEnd - (u8*)mCurEnd;
	mCurEnd = mEnd;
	// more stuff, unk74 has size 18
	for (UnknownStruct* s = unk74; s != nullptr; s = s->unk10) {
		s->unkC = mEnd;
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

void JKRSolidHeap::state_register(TState* state, u32 param_1) const
{
	state->_0C        = param_1;
	state->mUsedSize  = mSize - ((JKRSolidHeap*)this)->getTotalFreeSize();
	state->mCheckCode = ((u32)mCurEnd * 3) + (u32)mCurStart;
}

bool JKRSolidHeap::state_compare(const TState& fst, const TState& snd) const
{
	bool result = true;
	if (fst.mCheckCode != snd.mCheckCode) {
		result = false;
	}
	if (fst.mUsedSize != snd.mUsedSize) {
		result = false;
	}
	return result;
}
