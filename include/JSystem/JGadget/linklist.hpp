#ifndef JGADGET_LINKLIST_HPP
#define JGADGET_LINKLIST_HPP

#include <types.h>
#include <JSystem/JUtility/JUTAssert.hpp>

namespace JGadget {

struct TLinkListNode {
	TLinkListNode()
	{
		mNext = nullptr;
		mPrev = nullptr;
	}

	TLinkListNode* getNext() const { return mNext; }
	TLinkListNode* getPrev() const { return mPrev; }

public:
	/* 0x0 */ TLinkListNode* mNext;
	/* 0x4 */ TLinkListNode* mPrev;
}; // Size: 0x8

struct TNodeLinkList {
	struct iterator {
		explicit iterator(TLinkListNode* pNode) { node = pNode; }
		iterator& operator=(const iterator& other)
		{
			node = other.node;
			return *this;
		}

		iterator& operator++()
		{
			node = node->getNext();
			return *this;
		}
		iterator& operator--()
		{
			node = node->getPrev();
			return *this;
		}
		iterator operator++(int)
		{
			const iterator old(*this);
			(void)++*this;
			return old;
		}
		iterator operator--(int)
		{
			const iterator old(*this);
			(void)--*this;
			return old;
		}
		friend bool operator==(iterator a, iterator b)
		{
			return a.node == b.node;
		}
		friend bool operator!=(iterator a, iterator b) { return !(a == b); }

		TLinkListNode* operator->() const { return node; }
		TLinkListNode& operator*() const { return *node; }

	public:
		/* 0x00 */ TLinkListNode* node;
	};

	struct const_iterator {
		explicit const_iterator(TLinkListNode* pNode) { node = pNode; }
		explicit const_iterator(iterator it) { node = it.node; }

		const_iterator& operator++()
		{
			node = node->getNext();
			return *this;
		}
		const_iterator& operator--()
		{
			node = node->getPrev();
			return *this;
		}
		const_iterator operator++(int)
		{
			const const_iterator old(*this);
			(void)++*this;
			return old;
		}
		const_iterator operator--(int)
		{
			const const_iterator old(*this);
			(void)--*this;
			return old;
		}
		friend bool operator==(const_iterator a, const_iterator b)
		{
			return a.node == b.node;
		}
		friend bool operator!=(const_iterator a, const_iterator b)
		{
			return !(a == b);
		}

		friend bool operator==(const_iterator a, iterator b)
		{
			return a.node == b.node;
		}
		friend bool operator!=(const_iterator a, iterator b)
		{
			return !(a == b);
		}

		const TLinkListNode* operator->() const { return node; }
		const TLinkListNode& operator*() const { return *node; }

	public:
		/* 0x00 */ TLinkListNode* node;
	};

	TNodeLinkList()
	    : ocObject_()
	{
		Initialize_();
	}
	~TNodeLinkList();

	void Initialize_()
	{
		count           = 0;
		ocObject_.mNext = &ocObject_;
		ocObject_.mPrev = &ocObject_;
	}

	iterator begin() { return iterator(ocObject_.getNext()); }
	const_iterator begin() const { return const_iterator(ocObject_.getNext()); }
	iterator end() { return iterator(&ocObject_); }
	const_iterator end() const
	{
		return const_iterator((TLinkListNode*)(&ocObject_));
	}
	u32 size() const { return count; }
	bool empty() const { return size() == 0; }

	iterator erase(JGadget::TNodeLinkList::iterator,
	               JGadget::TNodeLinkList::iterator);
	iterator erase(JGadget::TNodeLinkList::iterator);
	void splice(JGadget::TNodeLinkList::iterator, JGadget::TNodeLinkList&,
	            JGadget::TNodeLinkList::iterator);
	iterator Find(const JGadget::TLinkListNode*);
	iterator Insert(JGadget::TNodeLinkList::iterator, JGadget::TLinkListNode*);
	iterator Erase(JGadget::TLinkListNode*);
	void Remove(JGadget::TLinkListNode*);

	bool Iterator_isEnd_(const_iterator it) const
	{
		return it.node == &ocObject_;
	}
	template <typename Predicate>
	void Remove_if(Predicate predicate, TNodeLinkList& tList)
	{
		iterator it = begin();

		while (!Iterator_isEnd_(const_iterator(it))) {
			if (predicate(*it)) {
				iterator itPrev = it;
				++it;
				tList.splice(tList.end(), *this, itPrev);
			} else {
				++it;
			}
		}
	}

	template <typename Predicate> void remove_if(Predicate predicate)
	{
		TNodeLinkList list;
		Remove_if(predicate, list);
	}

public:
	/* 0x00 */ u32 count;
	/* 0x04 */ TLinkListNode ocObject_;
}; // Size: 0xC

template <typename T, int I> struct TLinkList : public TNodeLinkList {
	TLinkList()
	    : TNodeLinkList()
	{
	}

	struct iterator : TNodeLinkList::iterator {
		explicit iterator(TNodeLinkList::iterator iter)
		    : TNodeLinkList::iterator(iter)
		{
		}

		iterator& operator++()
		{
			TNodeLinkList::iterator::operator++();
			return *this;
		}
		iterator& operator--()
		{
			TNodeLinkList::iterator::operator--();
			return *this;
		}
		iterator operator++(int)
		{
			const iterator old(*this);
			++*this;
			return old;
		}
		iterator operator--(int)
		{
			const iterator old(*this);
			--*this;
			return old;
		}
		friend bool operator==(iterator a, iterator b)
		{
			return (TNodeLinkList::iterator&)a == (TNodeLinkList::iterator&)b;
		}
		friend bool operator!=(iterator a, iterator b) { return !(a == b); }

		T* operator->() const
		{
			return Element_getValue(TNodeLinkList::iterator::operator->());
		}
		T& operator*() const
		{
			T* p = operator->();
			JUT_ASSERT(541, p != nullptr);
			return *p;
		}
	};

	struct const_iterator : TNodeLinkList::const_iterator {
		explicit const_iterator(TNodeLinkList::const_iterator iter)
		    : TNodeLinkList::const_iterator((TNodeLinkList::iterator&)iter)
		{
		}
		explicit const_iterator(iterator iter)
		    : TNodeLinkList::const_iterator(iter)
		{
		}

		const_iterator& operator++()
		{
			TNodeLinkList::const_iterator::operator++();
			return *this;
		}
		const_iterator& operator--()
		{
			TNodeLinkList::const_iterator::operator--();
			return *this;
		}
		const_iterator operator++(int)
		{
			const const_iterator old(*this);
			++*this;
			return old;
		}
		const_iterator operator--(int)
		{
			const const_iterator old(*this);
			--*this;
			return old;
		}
		friend bool operator==(const_iterator a, const_iterator b)
		{
			return (TNodeLinkList::const_iterator&)a
			       == (TNodeLinkList::const_iterator&)b;
		}
		friend bool operator!=(const_iterator a, const_iterator b)
		{
			return !(a == b);
		}

		const T* operator->() const
		{
			return Element_getValue(
			    TNodeLinkList::const_iterator::operator->());
		}
		const T& operator*() const
		{
			T* p = operator->();
			JUT_ASSERT(586, p != nullptr);
			return *p;
		}
	};

	static const TLinkListNode* Element_getNode(const T* element)
	{
		JUT_ASSERT(753, element != NULL);
		return reinterpret_cast<const TLinkListNode*>(
		    reinterpret_cast<const char*>(element) - I);
	}
	static TLinkListNode* Element_getNode(T* element)
	{
		JUT_ASSERT(758, element != NULL);
		return reinterpret_cast<TLinkListNode*>(reinterpret_cast<char*>(element)
		                                        - I);
	}
	static const T* Element_getValue(const TLinkListNode* node)
	{
		JUT_ASSERT(763, node != NULL);
		return reinterpret_cast<const T*>(reinterpret_cast<const char*>(node)
		                                  + I);
	}
	static T* Element_getValue(TLinkListNode* node)
	{
		JUT_ASSERT(768, node != NULL);
		return reinterpret_cast<T*>(reinterpret_cast<char*>(node) + I);
	}

	iterator Insert(iterator iter, T* element)
	{
		return iterator(TNodeLinkList::Insert((TNodeLinkList::iterator&)iter,
		                                      Element_getNode(element)));
	}
	iterator Erase(T* element)
	{
		return iterator(TNodeLinkList::Erase(Element_getNode(element)));
	}

	iterator begin() { return iterator(TNodeLinkList::begin()); }
	const_iterator begin() const
	{
		return const_iterator(const_cast<TLinkList*>(this)->begin());
	}
	iterator end() { return iterator(TNodeLinkList::end()); }
	const_iterator end() const
	{
		return const_iterator(const_cast<TLinkList*>(this)->end());
	}
	T& front()
	{
		JUT_ASSERT(642, !empty());
		return *begin();
	}
	T& back()
	{
		JUT_ASSERT(652, !empty());
		return *--end();
	}
	void Push_front(T* element) { Insert(begin(), element); }
	void Push_back(T* element) { Insert(end(), element); }
	iterator Find(const T* element)
	{
		return iterator(TNodeLinkList::Find(Element_getNode(element)));
	}
	void Remove(T* element) { TNodeLinkList::Remove(Element_getNode(element)); }
	u32 size() { return count; }
	bool empty() { return size() == 0; }
};

} // namespace JGadget

#endif
