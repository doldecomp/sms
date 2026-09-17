#include <JSystem/JKernel/JKRAramHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <dolphin/os.h>
#include <limits.h>
#include <macros.h>

JSUList<JKRAramBlock> JKRAramHeap::sAramList;

JKRAramHeap::JKRAramHeap(u32 startAddress, u32 size)
{
	OSInitMutex(&mMutex);

	mHeap        = JKRHeap::findFromRoot(this);
	mSize        = ALIGN_PREV(size, 0x20);
	mHeadAddress = ALIGN_NEXT(startAddress, 0x20);
	mTailAddress = mHeadAddress + mSize;
	mGroupId     = -1;

	JKRAramBlock* block
	    = new (mHeap, 0) JKRAramBlock(mHeadAddress, 0, mSize, -1, false);
	sAramList.append(&block->mBlockLink);
}

JKRAramHeap::~JKRAramHeap()
{
	JSUListIterator<JKRAramBlock> iterator(sAramList.getFirst());
	while (iterator != sAramList.getEnd()) {
		delete (iterator++).getObject();
	}
}

s32 JKRAramHeap::changeGroupID(u8 newGroupId)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

JKRAramBlock* JKRAramHeap::alloc(u32 size,
                                 JKRAramHeap::EAllocMode allocationMode)
{
	lock();

	JKRAramBlock* block;
	if (allocationMode == JKRAramHeap::HEAD) {
		block = allocFromHead(size);
	} else {
		block = allocFromTail(size);
	}

	unlock();
	return block;
}

JKRAramBlock* JKRAramHeap::allocFromHead(u32 size)
{
	u32 alignedSize         = ALIGN_NEXT(size, 0x20);
	u32 bestFreeSize        = UINT_MAX;
	JKRAramBlock* bestBlock = nullptr;

	JSUList<JKRAramBlock>* list = &sAramList;
	for (JSUListIterator<JKRAramBlock> iterator = list;
	     iterator != list->getEnd(); ++iterator) {
		JKRAramBlock* block = iterator.getObject();
		if (block->mFreeSize < alignedSize)
			continue;
		if (bestFreeSize <= block->mFreeSize)
			continue;

		bestFreeSize = block->mFreeSize;
		bestBlock    = block;

		if (block->mFreeSize == alignedSize) {
			break;
		}
	}

	if (bestBlock) {
		return bestBlock->allocHead(alignedSize, mGroupId, this);
	}

	return nullptr;
}

JKRAramBlock* JKRAramHeap::allocFromTail(u32 size)
{
	u32 alignedSize         = ALIGN_NEXT(size, 0x20);
	JKRAramBlock* tailBlock = nullptr;

	JSUList<JKRAramBlock>* list            = &sAramList;
	JSUListIterator<JKRAramBlock> iterator = list->getLast();
	for (; iterator != list->getEnd(); --iterator) {
		JKRAramBlock* block = iterator.getObject();
		if (block->mFreeSize >= alignedSize) {
			tailBlock = block;
			break;
		}
	}

	if (tailBlock) {
		return tailBlock->allocTail(alignedSize, mGroupId, this);
	}

	return nullptr;
}

int JKRAramHeap::freeLevel(u8 level)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

int JKRAramHeap::freeGroup(u8 groupId)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

void JKRAramHeap::freeAll() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

void JKRAramHeap::freeTail() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }

u32 JKRAramHeap::getFreeSize()
{
	u32 maxFreeSize = 0;

	lock();
	JSUList<JKRAramBlock>* list            = &sAramList;
	JSUListIterator<JKRAramBlock> iterator = list->getFirst();
	for (; iterator != list->getEnd(); ++iterator) {
		if (iterator->getFreeSize() > maxFreeSize) {
			maxFreeSize = iterator->getFreeSize();
		}
	}
	unlock();
	return maxFreeSize;
}

u32 JKRAramHeap::getTotalFreeSize()
{
	u32 totalFreeSize = 0;

	lock();
	JSUList<JKRAramBlock>* list            = &sAramList;
	JSUListIterator<JKRAramBlock> iterator = list->getFirst();
	for (; iterator != list->getEnd(); ++iterator) {
		totalFreeSize += iterator->getFreeSize();
	}
	unlock();
	return totalFreeSize;
}

u32 JKRAramHeap::getUsedSize(u8 groupId)
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return 0;
}

bool JKRAramHeap::isEmpty()
{
	JUT_ASSERT_F(false, "UNIMPLEMENTED");
	return false;
}

void JKRAramHeap::dump() { JUT_ASSERT_F(false, "UNIMPLEMENTED"); }
