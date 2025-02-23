#ifndef JGADGET_LIST_HPP
#define JGADGET_LIST_HPP

#include <JSystem/JGadget/allocator.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <types.h>

namespace JGadget {

template <class T, class A = TAllocator<T> > class TList {
public:
	struct TNode_ {
		TNode_* mNext;
		TNode_* mPrev;
		char mValue[sizeof(T)];
	};

	TNode_* CreateNode_(TNode_* next, TNode_* prev, const T& value)
	{
		TNode_* ret = (TNode_*)mAllocator.AllocateRaw(sizeof(TNode_));

		if (!ret)
			return nullptr;

		ret->mNext = next;
		ret->mPrev = prev;

		T* casted = (T*)&ret->mValue;
		mAllocator.construct(casted, value);

		return ret;
	}

	void DestroyNode_(TNode_* node)
	{
		mAllocator.destroy((T*)&node->mValue);
		mAllocator.DeallocateRaw(node);
	}

public:
	class iterator;

	TList(const TAllocator<T>& alloc)
	    : mAllocator(alloc)
	    , mSize(0)
	{
		mSentinel.mNext = &mSentinel;
		mSentinel.mPrev = &mSentinel;
	}

	~TList() { clear(); }

	iterator insert(iterator where, const T& what)
	{
		TNode_* curr    = where.mNode;
		TNode_* prev    = curr->mPrev;
		TNode_* newNode = CreateNode_(curr, prev, what);

		if (!newNode) {
			return iterator(&mSentinel);
		} else {
			curr->mPrev = newNode;
			prev->mNext = newNode;
			++mSize;
			return iterator(newNode);
		}
	}

	iterator erase(iterator what)
	{
		TNode_* curr       = what.mNode;
		TNode_* next       = curr->mNext;
		curr->mPrev->mNext = next;
		next->mPrev        = curr->mPrev;
		DestroyNode_(curr);
		--mSize;
		return iterator(next);
	}

	iterator erase(iterator start, iterator end)
	{
		while (start != end) {
			start = erase(start);
		}
		return start;
	}

	void clear() { erase(begin(), end()); }

	iterator begin() { return mSentinel.mNext; }
	iterator end() { return &mSentinel; }

	class iterator {
		friend class TList;

	public:
		iterator()
		    : mNode(nullptr)
		{
		}

		iterator(TNode_* node)
		    : mNode(node)
		{
		}

		T& operator*() { return *(T*)mNode->mValue; }
		T* operator->() { return (T*)mNode->mValue; }

		iterator operator++()
		{
			mNode = mNode->mNext;
			return *this;
		}

		iterator operator++(int)
		{
			iterator copy = *this;
			mNode         = mNode->mNext;
			return copy;
		}

		friend bool operator==(iterator fst, iterator snd)
		{
			return fst.mNode == snd.mNode;
		}

		friend bool operator!=(iterator fst, iterator snd)
		{
			return !(fst == snd);
		}

	protected:
		TNode_* mNode;
	};

private:
	/* 0x0 */ TAllocator<T> mAllocator;
	/* 0x4 */ u32 mSize;
	/* 0x8 */ TNode_ mSentinel;
};

class TList_pointer_void : public TList<void*> {
	typedef TList<void*> Base;
	typedef void* value_type;

public:
	TList_pointer_void(
	    const TAllocator<value_type>& = TAllocator<value_type>());
	TList_pointer_void(u32, const value_type&,
	                   const JGadget::TAllocator<value_type>&);
	~TList_pointer_void();

	iterator insert1234(iterator a, value_type const& b)
	{
		return Base::insert(a, b);
	}

	iterator insert(iterator, value_type const&);
	iterator insert(iterator, u32, value_type const&);
	iterator erase(iterator);
	iterator erase(iterator, iterator);
	void remove(const value_type&);
	void assign(u32, const value_type&);
	void resize(u32, const value_type&);
	void unique();
	void operator=(const JGadget::TList_pointer_void&);
};

template <class T> class TList_pointer : TList_pointer_void {
	typedef TList_pointer_void Base;

public:
	class iterator : Base::iterator {
		friend class TList_pointer;
		typedef Base::iterator Base;

	public:
		iterator() { }

		iterator(Base it)
		    : Base(it)
		{
		}

		T& operator*() { return (T&)this->Base::operator*(); }
		T* operator->() { return (T*)this->Base::operator->(); }

		iterator operator++() { return iterator(this->Base::operator++()); }
		iterator operator++(int) { return iterator(this->Base::operator++(0)); }

		friend bool operator==(iterator fst, iterator snd)
		{
			return fst.mNode == snd.mNode;
		}

		friend bool operator!=(iterator fst, iterator snd)
		{
			return !(fst == snd);
		}
	};

	iterator begin() { return iterator(Base::begin()); }
	iterator end() { return iterator(Base::end()); }

	iterator insert(iterator where, const T& what)
	{
		return Base::insert(where, what);
	}

	void push_front(const T& what) { insert(begin(), what); }
	void push_back(const T& what) { insert(end(), what); }
};

}; // namespace JGadget

#endif
