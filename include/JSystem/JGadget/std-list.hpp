#ifndef JGADGET_LIST_HPP
#define JGADGET_LIST_HPP

#include <JSystem/JGadget/std-memory.hpp>
#include <types.h>

namespace JGadget {

template <class T, class A = TAllocator<T> > class TList {
public:
	struct TNode_ {
		TNode_* mNext;
		TNode_* mPrev;
	};

	TNode_* CreateNode_(TNode_* next, TNode_* prev, const T& value)
	{
		// Debug assert leftovers confirmed through TP. Don't touch.
		(void)0;
		(void)0;

		void* raw = mAllocator.AllocateRaw(sizeof(TNode_) + sizeof(T));

		if (raw == nullptr) {
			(void)0; // debug assert
			return nullptr;
		}

		// const meme, don't touch
		TNode_* const ret = (TNode_*)raw;

		ret->mNext = next;
		ret->mPrev = prev;

		mAllocator.construct((T*)(ret + 1), value);

		return ret;
	}

	void DestroyNode_(TNode_* node)
	{
		// a whole bunch of debug assertin
		(void)0;
		(void)0;
		(void)0;
		(void)0;
		mAllocator.destroy((T*)(node + 1));
		mAllocator.DeallocateRaw(node);
	}

	void Initialize_()
	{
		mSentinel.mNext = &mSentinel;
		mSentinel.mPrev = &mSentinel;
	}

public:
	class iterator;

	TList(const TAllocator<T>& alloc = TAllocator<T>())
	    : mAllocator(alloc)
	    , mSize(0)
	{
		Initialize_();
	}

	~TList()
	{
		clear();
		(void)0; // debug assert
	}

	iterator insert(iterator where, const T& what)
	{
		TNode_* curr = where.mNode;
		(void)0; // debug assert
		TNode_* prev = curr->mPrev;

		// const meme, don't touch
		TNode_* const newNode = CreateNode_(curr, prev, what);

		if (newNode == nullptr)
			return end();

		curr->mPrev = newNode;
		prev->mNext = newNode;
		++mSize;
		return iterator(newNode);
	}

	iterator erase(iterator what)
	{
		TNode_* const next       = what.mNode->mNext;
		TNode_* const curr       = what.mNode;
		what.mNode->mPrev->mNext = next;
		next->mPrev              = curr->mPrev;
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

	iterator begin() { return iterator(mSentinel.mNext); }
	iterator end() { return iterator(&mSentinel); }

	// TODO: TP debug has const_iterators, do we have them too?
	void begin() const;
	void end() const;

	void push_front(const T& what) { insert(begin(), what); }
	void push_back(const T& what) { insert(end(), what); }

	size_t size() const { return mSize; }
	bool empty() const { return size() == 0; }

	class iterator {
		friend class TList;

		iterator(TNode_* node)
		    : mNode(node)
		{
		}

	public:
		iterator()
		    : mNode(nullptr)
		{
		}

		T& operator*()
		{
			(void)0; // debug assert
			return *(T*)(mNode + 1);
		}
		// not in tp debug for some reason? You'd expect this to be there
		// and be emitted as one of the most comonly used functions
		// but it isn't
		T* operator->() { return (T*)(mNode + 1); }

		iterator operator++()
		{
			mNode = mNode->mNext;
			return *this;
		}

		iterator operator++(int)
		{
			iterator copy = *this;
			this->operator++();
			return copy;
		}

		// TODO: TP debug has operator-- and even does funky inheritance from
		// std::iterator, do we need it too?

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

		T operator*() { return (T)this->Base::operator*(); }
		T operator->() { return (T)this->Base::operator->(); }

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
