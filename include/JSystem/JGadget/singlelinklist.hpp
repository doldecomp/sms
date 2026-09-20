#ifndef JGADGET_SINGLELINKLIST_HPP
#define JGADGET_SINGLELINKLIST_HPP

#include <types.h>

namespace JGadget {

struct TSingleLinkListNode {
	TSingleLinkListNode() { mNext = nullptr; }

public:
	/* 0x0 */ TSingleLinkListNode* mNext;
};

class TSingleNodeLinkList {
	void Initialize_()
	{
		count = 0;
		mHead = nullptr;
		mTail = &mHead;
	}

public:
	TSingleNodeLinkList() { Initialize_(); }

	class iterator {
	public:
		explicit iterator(TSingleLinkListNode** param_1) { unk0 = param_1; }
		iterator& operator=(const iterator& other)
		{
			unk0 = other.unk0;
			return *this;
		}

		// NOTE: no user-declared copy constructor. The map's
		// __ct__Q37JGadget19TSingleNodeLinkList8iteratorFRCQ37JGadget19TSingleNodeLinkList8iterator
		// is the compiler-generated one. A user-declared copy ctor makes MWCC
		// construct by-value arguments in place; the original instead copies
		// them bitwise through an extra stack temporary, which only happens
		// when the copy constructor is implicit.

		iterator& operator++()
		{
			unk0 = &(*unk0)->mNext;
			return *this;
		}
		iterator operator++(int)
		{
			const iterator old(*this);
			(void)++*this;
			return old;
		}

		friend bool operator==(iterator a, iterator b)
		{
			return a.unk0 == b.unk0;
		}
		friend bool operator!=(iterator a, iterator b) { return !(a == b); }

		TSingleLinkListNode* operator->() const { return *unk0; }
		TSingleLinkListNode& operator*() const { return **unk0; }

	public:
		/* 0x0 */ TSingleLinkListNode** unk0;
	};

	iterator begin() { return iterator(&mHead); }
	iterator end() { return iterator(mTail); }

	template <class F> void Remove_if(F, TSingleNodeLinkList&);
	void Remove(JGadget::TSingleLinkListNode*);
	iterator Insert(JGadget::TSingleNodeLinkList::iterator,
	                JGadget::TSingleLinkListNode*);
	void Find(const JGadget::TSingleLinkListNode*);
	void reverse();
	void swap(JGadget::TSingleNodeLinkList&);
	void splice(JGadget::TSingleNodeLinkList::iterator,
	            JGadget::TSingleNodeLinkList&,
	            JGadget::TSingleNodeLinkList::iterator,
	            JGadget::TSingleNodeLinkList::iterator);
	void splice(JGadget::TSingleNodeLinkList::iterator,
	            JGadget::TSingleNodeLinkList&,
	            JGadget::TSingleNodeLinkList::iterator);
	void splice(JGadget::TSingleNodeLinkList::iterator,
	            JGadget::TSingleNodeLinkList&);
	void clear();
	void erase(JGadget::TSingleNodeLinkList::iterator,
	           JGadget::TSingleNodeLinkList::iterator);
	void erase(JGadget::TSingleNodeLinkList::iterator);
	~TSingleNodeLinkList();

public:
	/* 0x0 */ int count;
	/* 0x4 */ TSingleLinkListNode* mHead;
	/* 0x8 */ TSingleLinkListNode** mTail;
};

template <class T, int I> class TSingleLinkList : public TSingleNodeLinkList {
public:
	TSingleLinkList()
	    : TSingleNodeLinkList()
	{
	}

	static TSingleLinkListNode* Element_getNode(T* element)
	{
		(void)element; // Debug-only assert
		return reinterpret_cast<TSingleLinkListNode*>(
		    reinterpret_cast<char*>(element) - I);
	}
	static T* Element_getValue(TSingleLinkListNode* node)
	{
		(void)node; // Debug-only assert
		return reinterpret_cast<T*>(reinterpret_cast<char*>(node) + I);
	}

	class iterator : public TSingleNodeLinkList::iterator {
	public:
		explicit iterator(TSingleNodeLinkList::iterator iter)
		    : TSingleNodeLinkList::iterator(iter)
		{
		}

		iterator& operator++()
		{
			TSingleNodeLinkList::iterator::operator++();
			return *this;
		}
		iterator operator++(int)
		{
			const iterator old(*this);
			++*this;
			return old;
		}

		friend bool operator==(iterator a, iterator b)
		{
			return (TSingleNodeLinkList::iterator&)a
			       == (TSingleNodeLinkList::iterator&)b;
		}
		friend bool operator!=(iterator a, iterator b) { return !(a == b); }

		T* operator->() const
		{
			return Element_getValue(
			    TSingleNodeLinkList::iterator::operator->());
		}
		T& operator*() const { return *operator->(); }
	};

	iterator begin() { return iterator(TSingleNodeLinkList::begin()); }
	iterator end() { return iterator(TSingleNodeLinkList::end()); }

	iterator Insert(iterator iter, T* element)
	{
		return iterator(TSingleNodeLinkList::Insert(
		    (TSingleNodeLinkList::iterator&)iter, Element_getNode(element)));
	}

	void Push_back(T* element) { Insert(end(), element); }

	~TSingleLinkList() { }
};

} // namespace JGadget

#endif
