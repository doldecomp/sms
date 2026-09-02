#include <JSystem/JKernel/JKRThread.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JKernel/JKRMacro.hpp>
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
	mStackMemory  = JKRAllocFromHeap(mHeap, mStackSize, 32);
	mThreadRecord = (OSThread*)JKRAllocFromHeap(mHeap, sizeof(OSThread), 32);
	OSCreateThread(mThreadRecord, &JKRThread::start, this,
	               (void*)((u32)mStackMemory + mStackSize), mStackSize,
	               threadPrio, OS_THREAD_ATTR_DETACH);
	mMesgCount  = msgCount;
	mMesgBuffer = (OSMessage*)JKRAllocFromHeap(
	    mHeap, mMesgCount * sizeof(OSMessage), 0);
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

// NOTE: sTaskList is declared but not defined: the map does not list it, and a
// definition would add a JKRTask entry to __sinit_JKRThread_cpp, which the
// original does not have.

JKRTask::JKRTask()
    : JKRThread(0, 0, 0)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
}

JKRTask::~JKRTask() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

JKRTask* JKRTask::create()
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

void JKRTask::destroy() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

void* JKRTask::run()
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return nullptr;
}

void JKRTask::request(void (*func)(void*), void* param)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
}
