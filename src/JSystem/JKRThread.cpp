
#include <JSystem/JKernel/JKRMacro.hpp>
#include <JSystem/JKernel/JKRThread.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

JSUList<JKRThread> JKRThread::sThreadList;

JKRThread::JKRThread(u32 stackSize, int msgCount, int threadPrio)
    : mLink(this)
{
	mHeap = JKRHeap::findFromRoot(this);
	if (mHeap == nullptr) {
		mHeap = JKRHeap::getSystemHeap();
	}

	mStackSize    = JKR_ALIGN32(stackSize);
	mStackMemory  = JKRHeap::alloc(mStackSize, 32, mHeap);
	mThreadRecord = JKRHeap::allocOne<OSThread>(32, mHeap);
	OSCreateThread(mThreadRecord, &JKRThread::start, this,
	               (void*)((u32)mStackMemory + mStackSize), mStackSize,
	               threadPrio, OS_THREAD_ATTR_DETACH);
	mMesgCount  = msgCount;
	mMesgBuffer = JKRHeap::allocArray<OSMessage>(mMesgCount, 0, mHeap);
	OSInitMessageQueue(&mMesgQueue, mMesgBuffer, mMesgCount);
	JKRThread::sThreadList.append(&mLink);
}

JKRThread::JKRThread(OSThread* threadRecord, int msgCount)
    : mLink(this)
{
	mHeap         = nullptr;
	mThreadRecord = threadRecord;
	mStackSize    = (u32)threadRecord->stackEnd - (u32)threadRecord->stackBase;
	mStackMemory  = threadRecord->stackBase;
	mMesgCount    = msgCount;
	mMesgBuffer   = (OSMessage*)JKRHeap::sSystemHeap->alloc(
        mMesgCount * sizeof(OSMessage), 4);
	OSInitMessageQueue(&mMesgQueue, mMesgBuffer, mMesgCount);
	JKRThread::sThreadList.append(&mLink);
}

JKRThread::~JKRThread()
{
	JKRThread::sThreadList.remove(&mLink);

	if (mHeap != nullptr) {
		if (!OSIsThreadTerminated(mThreadRecord)) {
			OSDetachThread(mThreadRecord);
			OSCancelThread(mThreadRecord);
		}

		JKRHeap::free(mStackMemory, mHeap);
		JKRHeap::free(mThreadRecord, mHeap);
	}

	JKRHeap::free(mMesgBuffer, nullptr);
}

void* JKRThread::start(void* thread)
{
	return static_cast<JKRThread*>(thread)->run();
}
