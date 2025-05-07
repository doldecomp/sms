#ifndef JGADGET_SINGLELINKLIST_HPP
#define JGADGET_SINGLELINKLIST_HPP

#include <types.h>

namespace JGadget {

class TSingleLinkListNode {
public:
	TSingleLinkListNode()
	    : mNext(nullptr)
	{
	}

public:
	/* 0x0 */ TSingleLinkListNode* mNext;
};

class TSingleNodeLinkList {
	void Initialize_()
	{
		unk0       = 0;
		unk4.mNext = nullptr;
		unk8       = &unk4;
	}

public:
	TSingleNodeLinkList() { Initialize_(); }

	class iterator {
	public:
		iterator(TSingleLinkListNode** param_1)
		    : unk0(param_1)
		{
		}

		iterator(const iterator& other)
		    : unk0(other.unk0)
		{
		}

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

	iterator begin() { return iterator(&unk4.mNext); }
	iterator end() { return iterator(&unk8->mNext); }

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
	/* 0x0 */ int unk0;
	/* 0x4 */ TSingleLinkListNode unk4;
	/* 0x8 */ TSingleLinkListNode* unk8;
};

template <class T, int I> class TSingleLinkList : public TSingleNodeLinkList {
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

public:
	class iterator {
	public:
		iterator(TSingleNodeLinkList::iterator bs)
		    : base(bs)
		{
		}

		// fabricated

		iterator& operator++()
		{
			++base;
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
			return a.base == b.base;
		}
		friend bool operator!=(iterator a, iterator b) { return !(a == b); }

		T* operator->() const { return Element_getValue(base.operator->()); }
		T& operator*() const { return *operator->(); }

	public:
		TSingleNodeLinkList::iterator base;
	};

	iterator begin() { return iterator(TSingleNodeLinkList::begin()); }
	iterator end() { return iterator(TSingleNodeLinkList::end()); }

	iterator Insert(iterator iter, T* element)
	{
		return iterator(
		    TSingleNodeLinkList::Insert(iter.base, Element_getNode(element)));
	}

	void Push_back(T* element) { Insert(end(), element); }

	~TSingleLinkList() { }
};

} // namespace JGadget

#endif
