#include <JSystem/JKernel/JKRStdHeap.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <macros.h>

JKRStdHeap* JKRStdHeap::create(u32 size, JKRHeap* parent, bool errorFlag)
{
	if (!parent)
		parent = sRootHeap;

	const u32 expHeapSize = ALIGN_NEXT(sizeof(JKRStdHeap), 0x20);
	u32 alignedSize       = ALIGN_PREV(size, 0x20);

	void* ptr     = JKRHeap::alloc(alignedSize, 0x20, parent);
	void* dataPtr = (char*)ptr + expHeapSize;
	if (ptr == nullptr)
		return nullptr;

	return new (ptr)
	    JKRStdHeap(dataPtr, alignedSize - expHeapSize, parent, errorFlag);
}

JKRStdHeap::JKRStdHeap(void* data, u32 size, JKRHeap* parent, bool errorFlag)
    : JKRHeap(data, size, parent, errorFlag)
{
	mHeapId = OSCreateHeap(mStart, mEnd);
}

JKRStdHeap::~JKRStdHeap()
{
	dispose();
	if (mHeapId != -1)
		OSDestroyHeap(mHeapId);
}

void* JKRStdHeap::alloc(u32 size, int alignment)
{
	if (mHeapId == -1) {
		return 0;
	} else {
		void* res = OSAllocFromHeap(mHeapId, size);
		if (!res) {
			JUTWarningConsole_f("alloc: cannot alloc memory (0x%x byte).\n",
			                    size);
			if (mErrorFlag == true && mErrorHandler) {
				(*mErrorHandler)(this, size, alignment);
			}
		}
		return res;
	}
}

void JKRStdHeap::free(void* ptr)
{
	if (mHeapId == -1)
		return;
	if (mStart > ptr)
		return;
	if (ptr > mEnd)
		return;
	OSFreeToHeap(mHeapId, ptr);
}

void JKRStdHeap::freeAll()
{
	if (mHeapId == -1)
		return;

	JKRHeap::freeAll();
	OSDestroyHeap(mHeapId);
	mHeapId = OSCreateHeap(mStart, mEnd);
}

void JKRStdHeap::freeTail()
{
	JUTWarningConsole("freeTail: freeTail() is not work\n");
}

s32 JKRStdHeap::resize(void* ptr, u32 size)
{
	JUTWarningConsole_f("resize: cannot resize memory block (%08x: %d)\n");
	return -1;
}

s32 JKRStdHeap::getFreeSize()
{
	JUTWarningConsole("getFreeSize: cannot get free size\n");
	return -1;
}

void JKRStdHeap::state_register(TState* state, u32 param_1) const
{
	state->_0C        = param_1;
	state->mUsedSize  = 0;
	state->mCheckCode = 0;
}

bool JKRStdHeap::state_compare(const TState& fst, const TState& snd) const
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
