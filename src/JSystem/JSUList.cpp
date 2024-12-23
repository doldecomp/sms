#include <JSystem/JSupport/JSUList.hpp>

JSUPtrLink::JSUPtrLink(void* val)
{
	this->mPtrList = nullptr;
	this->mData    = val;
	this->mPrev    = nullptr;
	this->mNext    = nullptr;
}

JSUPtrLink::~JSUPtrLink()
{
	if (this->mPtrList) {
		this->mPtrList->remove(this);
	}
}

JSUPtrList::JSUPtrList(bool b)
{
	if (b) {
		this->initiate();
	}
}

JSUPtrList::~JSUPtrList()
{
	JSUPtrLink* p = this->mHead;

	for (int i = 0; i < this->mLinkCount; i++) {
		p->mPtrList = 0;
		p           = p->mNext;
	}
}

void JSUPtrList::initiate()
{
	this->mHead      = nullptr;
	this->mTail      = nullptr;
	this->mLinkCount = 0;
}

bool JSUPtrList::append(JSUPtrLink* node)
{
	bool result;

	result = (node->mPtrList == nullptr);
	if (!result) {
		result = node->mPtrList->remove(node);
	}
	if (result) {
		if (this->mLinkCount == 0) {
			node->mPtrList   = this;
			node->mPrev      = nullptr;
			node->mNext      = nullptr;
			this->mTail      = node;
			this->mHead      = node;
			this->mLinkCount = 1;
		} else {
			node->mPtrList     = this;
			node->mPrev        = this->mTail;
			node->mNext        = nullptr;
			this->mTail->mNext = node;
			this->mTail        = node;
			this->mLinkCount   = this->mLinkCount + 1;
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
		if (this->mLinkCount == 0) {
			node->mPtrList   = this;
			node->mPrev      = nullptr;
			node->mNext      = nullptr;
			this->mTail      = node;
			this->mHead      = node;
			this->mLinkCount = 1;
		} else {
			node->mPtrList     = this;
			node->mPrev        = nullptr;
			node->mNext        = this->mHead;
			this->mHead->mPrev = node;
			this->mHead        = node;
			this->mLinkCount   = this->mLinkCount + 1;
		}
	}

	return result;
}

bool JSUPtrList::insert(JSUPtrLink* before, JSUPtrLink* ptr)
{
	bool result;

	if (before == this->mHead) {
		return this->prepend(ptr);
	} else if (before == nullptr) {
		return this->append(ptr);
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
		this->mLinkCount++;
	}

	return result;
}

bool JSUPtrList::remove(JSUPtrLink* node)
{
	bool isSameList = (node->mPtrList == this);

	if (isSameList) {
		if (this->mLinkCount == 1) {
			this->mHead = nullptr;
			this->mTail = nullptr;
		} else if (node == this->mHead) {
			node->mNext->mPrev = nullptr;
			this->mHead        = node->mNext;
		} else if (node == this->mTail) {
			node->mPrev->mNext = nullptr;
			this->mTail        = node->mPrev;
		} else {
			node->mPrev->mNext = node->mNext;
			node->mNext->mPrev = node->mPrev;
		}

		node->mPtrList = nullptr;
		this->mLinkCount--;
	}

	return isSameList;
}
