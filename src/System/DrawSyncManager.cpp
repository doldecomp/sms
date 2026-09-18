#include <System/DrawSyncManager.hpp>
#include <dolphin/gx.h>

TDrawSyncManager* TDrawSyncManager::smInstance;

class TFifo {
public:
	TFifo(int capacity)
	    : mCapacity(capacity)
	    , mReadIdx(0)
	    , mWriteIdx(0)
	{
		mData = new void*[mCapacity + 1];
	}

	int getLoopIdx(u32 index)
	{
		if (index >= mCapacity + 1)
			index = 0;
		return index;
	}

	// fabricated
	int size()
	{
		if (mReadIdx <= mWriteIdx)
			return mWriteIdx - mReadIdx;
		else
			return mWriteIdx + mCapacity + 1 - mReadIdx;
	}

	// fabricated
	void advanceReadIdx() { mReadIdx = getLoopIdx(mReadIdx + 1); }

	// fabricated
	void* read() { return mData[getLoopIdx(mReadIdx + 1)]; }

	// fabricated
	void push(void* value)
	{
		mData[mWriteIdx] = value;
		mWriteIdx        = getLoopIdx(mWriteIdx + 1);
	}

private:
	/* 0x0 */ void** mData;
	/* 0x4 */ int mCapacity;
	/* 0x8 */ int mReadIdx;
	/* 0xC */ u32 mWriteIdx;
};

// TODO: 98.2%. Every instruction and the frame (0x40) are exact; the whole
// residue is a five-way rotation of the callee-saved set, with the inlined
// TDrawSyncManager constructor's values one step round from retail's:
// param_1 r27 (retail r31), param_2 r28 (r27), the `new` result r31 (r30),
// the reloaded `this` r29 (r28) and the constant 0 r30 (r29). Per
// docs/catalog/frame-gaps.md a pure rotation with relative order preserved is
// a member read that should go through an accessor; no member of
// TDrawSyncManager is read here, so the lever is more likely an inline level
// around the `new`/constructor pair.
TDrawSyncManager* TDrawSyncManager::start(u32 param_1, u32 param_2, s32 param_3)
{
	if (smInstance == nullptr)
		smInstance = new TDrawSyncManager(param_1, param_2, param_3);
	return smInstance;
}

void TDrawSyncManager::end() { }

void TDrawSyncManager::drawSyncCallback(u16 param_1)
{
	if (smInstance != nullptr)
		smInstance->drawSyncCallbackSub(param_1);
}

// TODO: 99.9%. Instruction-exact; frame 0x38 vs 0x30 and `msg` at 0x20 vs
// 0x1c, i.e. retail has one 4-byte local declared *before* `msg` (named locals
// grow down from the top, first-declared highest) plus the 4 bytes of
// alignment that makes. Rejected (closure batch 83): hoisting the fifo count
// to function scope as `u32 pending;` declared before `msg` -- it is
// register-allocated and changes nothing; a named `TFifo*` cannot be it either
// because retail re-reads 0x348(r30) at every use.
void* TDrawSyncManager::threadFunc(void* param_1)
{
	TDrawSyncManager* self = (TDrawSyncManager*)param_1;
	for (;;) {
		void* msg;
		OSReceiveMessage(&self->mMessageQueue, &msg, 1);
		if ((size_t)msg >= 0x80000000) {
			self->mFifo->push(msg);

			u32 iVar1 = self->mFifo->size();

			if (iVar1 == 2)
				GXEnableBreakPt(msg);
		} else {
			if ((size_t)msg >= 0x10000)
				break;

			self->mFifo->advanceReadIdx();

			u32 iVar1 = self->mFifo->size();

			if (iVar1 != 0) {
				if (iVar1 == 1)
					GXDisableBreakPt();
				else if (iVar1 >= 2)
					GXEnableBreakPt(self->mFifo->read());
			}
		}
	}
	return 0;
}

TDrawSyncManager::TDrawSyncManager(u32 param_1, u32 param_2, s32 param_3)
    : mCallbacks(param_1)
{
	mFlags = 0;
	OSCreateThread(&mProcessingThread, &threadFunc, this,
	               new u8[0x1000] + 0x1000, 0x1000, param_3, 0);
	OSInitMessageQueue(&mMessageQueue, new u8[0x50], 0x14);
	mFifo = new TFifo(param_2);
	OSResumeThread(&mProcessingThread);
}

TDrawSyncManager::~TDrawSyncManager() { }

// TODO: 99.6%. Instruction-exact and the frame is right (0x30); only the
// 8-byte TDrawSyncTokenRange temporary sits 4 bytes low (0x24 vs retail's
// 0x28, flush with the top of the local area). Measured (closure batch 83):
// naming the temporary (`TDrawSyncTokenRange range(...); mCallbacks[i] =
// range;`) puts it flush with the top but shrinks the frame to 0x28, so retail
// is a named local with 8 more bytes of dead low region *or* this unnamed form
// with 4 more; both need an object nothing here wants.
void TDrawSyncManager::setCallback(u32 param_1, u16 param_2, u16 param_3,
                                   TDrawSyncCallback* param_4)
{
	mCallbacks[param_1] = TDrawSyncTokenRange(param_2, param_3, param_4);
}

void TDrawSyncManager::drawSyncCallbackSub(u16 param_1)
{
	if (param_1 == 0) {
		if (!(mFlags & 2))
			OSSendMessage(&mMessageQueue, (void*)(size_t)param_1, 1);
		return;
	}

	for (TDrawSyncTokenRange* it = mCallbacks.begin(); it != mCallbacks.end();
	     ++it)
		if (it->mCallback != nullptr && it->mRangeStart <= param_1
		    && param_1 <= it->mRangeEnd) {
			it->mCallback->drawSyncCallback(param_1);
			if (!(mFlags & 2))
				OSSendMessage(&mMessageQueue, (void*)(size_t)param_1, 1);
			return;
		}
}

void TDrawSyncManager::pushBreakPoint()
{
	if (mFlags & 3)
		return;

	GXFlush();
	void* readPtr;
	void* writePtr;
	GXGetFifoPtrs(GXGetCPUFifo(), &readPtr, &writePtr);
	OSSendMessage(&mMessageQueue, writePtr, 1);
}
