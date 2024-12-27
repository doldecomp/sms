#include <JSystem/JKernel/JKRAramHeap.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
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
