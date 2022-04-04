#include "dolphin.h"
#include "types.h"

#include "JSystem/JSUList.hpp"

JSUPtrLink::JSUPtrLink(void* val)
{
	this->list  = NULL;
	this->value = val;
	this->prev  = NULL;
	this->next  = NULL;
}

JSUPtrLink::~JSUPtrLink()
{
	if (this->list) {
		this->list->remove(this);
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
	JSUPtrLink* p = this->first;

	for (int i = 0; i < this->length; i++) {
		p->list = 0;
		p       = p->next;
	}
}

void JSUPtrList::initiate()
{
	this->first  = NULL;
	this->last   = NULL;
	this->length = 0;
}

bool JSUPtrList::append(JSUPtrLink* node)
{
	bool result;

	result = (node->list == NULL);
	if (!result) {
		result = node->list->remove(node);
	}
	if (result) {
		if (this->length == 0) {
			node->list   = this;
			node->prev   = NULL;
			node->next   = NULL;
			this->last   = node;
			this->first  = node;
			this->length = 1;
		} else {
			node->list       = this;
			node->prev       = this->last;
			node->next       = NULL;
			this->last->next = node;
			this->last       = node;
			this->length     = this->length + 1;
		}
	}

	return result;
}

bool JSUPtrList::prepend(JSUPtrLink* node)
{
	bool result;

	result = (node->list == NULL);
	if (!result) {
		result = node->list->remove(node);
	}
	if (result) {
		if (this->length == 0) {
			node->list   = this;
			node->prev   = NULL;
			node->next   = NULL;
			this->last   = node;
			this->first  = node;
			this->length = 1;
		} else {
			node->list        = this;
			node->prev        = NULL;
			node->next        = this->first;
			this->first->prev = node;
			this->first       = node;
			this->length      = this->length + 1;
		}
	}

	return result;
}

bool JSUPtrList::insert(JSUPtrLink* before, JSUPtrLink* ptr)
{
	bool result;

	if (before == this->first) {
		return this->prepend_(ptr);
	} else if (before == NULL) {
		return this->append_(ptr); // inlined
	}

	if (before->list != this) {
		return false;
	}

	result = (NULL == ptr->list);
	if (!result) {
		result = ptr->list->remove(ptr);
	}

	if (result) {
		JSUPtrLink* prev = before->prev;
		ptr->list        = this;
		ptr->prev        = prev;
		ptr->next        = before;
		prev->next       = ptr;
		before->prev     = ptr;
		this->length++;
	}

	return result;
}

bool JSUPtrList::remove(JSUPtrLink* node)
{
	bool isSameList = (node->list == this);

	if (isSameList) {
		if (this->length == 1) {
			this->first = NULL;
			this->last  = NULL;
		} else if (node == this->first) {
			node->next->prev = NULL;
			this->first      = node->next;
		} else if (node == this->last) {
			node->prev->next = NULL;
			this->last       = node->prev;
		} else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}

		node->list = NULL;
		this->length--;
	}

	return isSameList;
}
