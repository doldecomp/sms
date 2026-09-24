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
	void* read()
	{
		int idx = getLoopIdx(mReadIdx + 1);
		return mData[idx];
	}

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

// TODO (closure batch 152): the fourth non-exact symbol in this object is the
// weak JGadget::TVector<TDrawSyncTokenRange>::insert, at 99.2% and
// instruction-exact: `this` and the count parameter are swapped (retail r30/
// r29, ours r29/r30) and our frame is 0x20 *larger* than retail's 0x100, so it
// wants a lever removed rather than added. It lives in JGadget's shared
// header, so it is parked here as a report item.

TDrawSyncManager* TDrawSyncManager::start(u32 param_1, u32 param_2, s32 param_3)
{
	if (smInstance == nullptr)
		smInstance = new TDrawSyncManager(param_1, param_2, param_3);
	return smInstance;
}

// UNUSED, 0x80 in the map: the counterpart of start(), never called by the
// shipped game. The guard is what the size says -- with a bare
// `delete smInstance; smInstance = nullptr;` this compiles to 0x78, and the
// whole destructor below is expanded inline here, so the two sizes move
// together (every destructor body measured lands end() exactly 8 bytes below
// it).
void TDrawSyncManager::end()
{
	if (smInstance != nullptr) {
		delete smInstance;
		smInstance = nullptr;
	}
}

void TDrawSyncManager::drawSyncCallback(u16 param_1)
{
	if (smInstance != nullptr)
		smInstance->drawSyncCallbackSub(param_1);
}

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
	u8* stack = new u8[0x1000];
	OSCreateThread(&mProcessingThread, &threadFunc, this, stack + 0x1000, 0x1000,
	               param_3, 0);
	u8* buffer = new u8[0x14 * sizeof(OSMessage)];
	OSInitMessageQueue(&mMessageQueue, buffer, 0x14);
	mFifo = new TFifo(param_2);
	OSResumeThread(&mProcessingThread);
}

// UNUSED, 0x88 in the map, of which 0x68 is the inlined TVector destructor of
// mCallbacks; the remaining 0x20 is this body. The shutdown message is the one
// piece of direct evidence in the TU: threadFunc() leaves its loop only for a
// message in [0x10000, 0x80000000), a range neither drawSyncCallbackSub() (a
// u16 token) nor pushBreakPoint() (a FIFO pointer) can produce, so that range
// exists purely to stop the thread -- and signalling it then joining is the
// only pair that reproduces 0x20 exactly. Two size-equal alternatives are
// rejected for having no evidence behind their stores:
// `OSCancelThread(&mProcessingThread); delete mFifo; mFifo = nullptr;` plus
// either `smInstance = nullptr;` or `mFlags = 0;`. Note that nothing here
// frees mFifo, the thread stack or the message-queue buffer; any body that
// does overshoots 0x88.
TDrawSyncManager::~TDrawSyncManager()
{
	OSSendMessage(&mMessageQueue, (void*)0x10000, 1);
	OSJoinThread(&mProcessingThread, nullptr);
}

// TODO: 99.6%. Instruction-exact and the frame is right (0x30); only the
// 8-byte TDrawSyncTokenRange temporary sits 4 bytes low (0x24 vs retail's
// 0x28, flush with the top of the local area). Measured (closure batch 83):
// naming the temporary (`TDrawSyncTokenRange range(...); mCallbacks[i] =
// range;`) puts it flush with the top but shrinks the frame to 0x28, so retail
// is a named local with 8 more bytes of dead low region *or* this unnamed form
// with 4 more; both need an object nothing here wants.
// Closure 217 pinned which of the two retail is: the named form's frame is
// exactly the top of the temporary (0x28), the unnamed form's is that rounded
// up (temp top 0x2c, frame 0x30), and retail's 0x28/0x30 has no padding, so
// retail is the *named* form plus 8 bytes of pool below it. Every +8 lever
// tried on `mCallbacks` measured +16 instead (frame 0x38): a TU-local
// reference binder, the same as a pointer binder, a bare direct-return fork,
// and a `getCallbacks()` member accessor binder in the class -- the vector is
// 0x18 bytes and the binder appears to be priced by the bound object, not by
// the return type, so the missing item is a 8-byte object, not a vector
// binding.
// Closure 220 measured the 8 bytes exactly: they are *two 4-byte dead named
// locals*, and the whole ladder is now known. On the named-temporary form
// (frame 0x28, object 0x20) every dead named scalar declared after `range`
// raises the object 4 and the first of them raises the frame 8, so two of them
// are retail's 0x28/0x30 byte-for-byte (100.0%); one is 0x24/0x30, two u16s
// pack into one word (0x24/0x30), and one dead 8-byte object is 0x30/0x38.
// Two dead `u32` locals in the inlined TDrawSyncTokenRange constructor are the
// same 100.0%. All of these are nonsensical temporaries, so none is
// committable. Measured free instead (no slot at all): named pointer/reference
// binders over `mCallbacks.begin()` or the element, in any declaration order,
// singly or in pairs; the constructor written with assignments instead of an
// initialiser list; a `setRange`/`set` helper inside the constructor
// (inlined-callee *parameters* reserve nothing, only named locals do, and the
// helper also costs drawSyncCallbackSub). What is missing is a real value,
// two words wide, that this body computes and never uses.
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
