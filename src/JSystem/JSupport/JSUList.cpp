#include <JSystem/JSupport/JSUList.hpp>

JSUPtrLink::JSUPtrLink(void* val)
{
	mPtrList = nullptr;
	mData    = val;
	mPrev    = nullptr;
	mNext    = nullptr;
}

JSUPtrLink::~JSUPtrLink()
{
	if (mPtrList) {
		mPtrList->remove(this);
	}
}

JSUPtrList::JSUPtrList(bool b)
{
	if (b) {
		initiate();
	}
}

JSUPtrList::~JSUPtrList()
{
	JSUPtrLink* p = mHead;

	for (int i = 0; i < mLinkCount; i++) {
		p->mPtrList = 0;
		p           = p->mNext;
	}
}

void JSUPtrList::initiate()
{
	mHead      = nullptr;
	mTail      = nullptr;
	mLinkCount = 0;
}

bool JSUPtrList::append(JSUPtrLink* node)
{
	bool result;

	result = (node->mPtrList == nullptr);
	if (!result) {
		result = node->mPtrList->remove(node);
	}
	if (result) {
		if (mLinkCount == 0) {
			node->mPtrList = this;
			node->mPrev    = nullptr;
			node->mNext    = nullptr;
			mTail          = node;
			mHead          = node;
			mLinkCount     = 1;
		} else {
			node->mPtrList = this;
			node->mPrev    = mTail;
			node->mNext    = nullptr;
			mTail->mNext   = node;
			mTail          = node;
			mLinkCount     = mLinkCount + 1;
		}
	}

	return result;
}

bool JSUPtrList::prepend(JSUPtrLink* node)
{
	bool result;

	result = (node->mPtrList == nullptr);
	if (!result) {
		result = node->mPtrList->remove(node);
	}
	if (result) {
		if (mLinkCount == 0) {
			node->mPtrList = this;
			node->mPrev    = nullptr;
			node->mNext    = nullptr;
			mTail          = node;
			mHead          = node;
			mLinkCount     = 1;
		} else {
			node->mPtrList = this;
			node->mPrev    = nullptr;
			node->mNext    = mHead;
			mHead->mPrev   = node;
			mHead          = node;
			mLinkCount     = mLinkCount + 1;
		}
	}

	return result;
}

bool JSUPtrList::insert(JSUPtrLink* before, JSUPtrLink* ptr)
{
	bool result;

	if (before == mHead) {
		return prepend(ptr);
	} else if (before == nullptr) {
		return append(ptr);
	}

	if (before->mPtrList != this) {
		return false;
	}

	result = (nullptr == ptr->mPtrList);
	if (!result) {
		result = ptr->mPtrList->remove(ptr);
	}

	if (result) {
		JSUPtrLink* mPrev = before->mPrev;
		ptr->mPtrList     = this;
		ptr->mPrev        = mPrev;
		ptr->mNext        = before;
		mPrev->mNext      = ptr;
		before->mPrev     = ptr;
		mLinkCount++;
	}

	return result;
}

bool JSUPtrList::remove(JSUPtrLink* node)
{
	bool isSameList = (node->mPtrList == this);

	if (isSameList) {
		if (mLinkCount == 1) {
			mHead = nullptr;
			mTail = nullptr;
		} else if (node == mHead) {
			node->mNext->mPrev = nullptr;
			mHead              = node->mNext;
		} else if (node == mTail) {
			node->mPrev->mNext = nullptr;
			mTail              = node->mPrev;
		} else {
			node->mPrev->mNext = node->mNext;
			node->mNext->mPrev = node->mPrev;
		}

		node->mPtrList = nullptr;
		mLinkCount--;
	}

	return isSameList;
}
