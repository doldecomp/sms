#ifndef JGADGET_LIST_HPP
#define JGADGET_LIST_HPP

#include <JSystem/JGadget/std-memory.hpp>
#include <JSystem/JGadget/define.h>
#include <types.h>

namespace JGadget {

template <class T, class A = TAllocator<T> > class TList {
public:
	struct TNode_ {
		TNode_* pNext_;
		TNode_* pPrev_;
	};

	TNode_* CreateNode_(TNode_* pNext, TNode_* pPrev, const T& value)
	{
		// clang-format off
		JGADGET_ASSERT(pNext!=0);
		JGADGET_ASSERT(pPrev!=0);
		// clang-format on

		void* raw = mAllocator.AllocateRaw(sizeof(TNode_) + sizeof(T));

		if (raw == nullptr) {
			// clang-format off
			JGADGET_WARN("can't allocate memory");
			// clang-format on
			return nullptr;
		}

		TNode_* const ret = (TNode_*)raw;

		ret->pNext_ = pNext;
		ret->pPrev_ = pPrev;
		mAllocator.construct((T*)(ret + 1), value);
		return ret;
	}

	void DestroyNode_(TNode_* p)
	{
		// clang-format off
		JGADGET_ASSERT(p!=0);
		JGADGET_ASSERT(p!=&oEnd_);
		JGADGET_ASSERT(p->pNext_->pPrev_!=p);
		JGADGET_ASSERT(p->pPrev_->pNext_!=p);
		// clang-format on
		mAllocator.destroy((T*)(p + 1));
		mAllocator.DeallocateRaw(p);
	}

	void Initialize_()
	{
		oEnd_.pNext_ = &oEnd_;
		oEnd_.pPrev_ = &oEnd_;
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
		Confirm();
		clear();
		// clang-format off
		JGADGET_ASSERTWARN(empty());
		// clang-format on
	}

	iterator insert(iterator where, const T& what)
	{
		TNode_* p = where.p_;
		// clang-format off
		JGADGET_ASSERT(p!=0);
		// clang-format on

		TNode_* prev          = p->pPrev_;
		TNode_* const newNode = CreateNode_(p, prev, what);

		if (newNode == nullptr) {
			return end();
		}

		p->pPrev_    = newNode;
		prev->pNext_ = newNode;
		++mSize;
		return iterator(newNode);
	}

	iterator erase(iterator what)
	{
		// clang-format off
		JGADGET_ASSERT(!empty());
		// clang-format on

		TNode_* const p = what.p_;

		// clang-format off
		JGADGET_ASSERT(p!=0);
		// clang-format on

		// TODO: I think these should all use `p` instead of `what.p_`.
		// Changing them breaks Map's JointModelManager though.
		TNode_* const next      = what.p_->pNext_;
		what.p_->pPrev_->pNext_ = next;
		next->pPrev_            = p->pPrev_;
		DestroyNode_(p);
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

	iterator begin() { return iterator(oEnd_.pNext_); }
	iterator end() { return iterator(&oEnd_); }

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
		    : p_(node)
		{
		}

	public:
		iterator()
		    : p_(nullptr)
		{
		}

		T& operator*()
		{
			// clang-format off
			JGADGET_ASSERT(p_!=0);
			// clang-format on
			return *(T*)(p_ + 1);
		}
		// not in tp debug for some reason? You'd expect this to be there
		// and be emitted as one of the most comonly used functions
		// but it isn't
		T* operator->() { return (T*)(p_ + 1); }

		iterator& operator++()
		{
			p_ = p_->pNext_;
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
			return fst.p_ == snd.p_;
		}

		friend bool operator!=(iterator fst, iterator snd)
		{
			return !(fst == snd);
		}

	protected:
		TNode_* p_;
	};

private:
	bool Confirm()
	{
		// TODO
	}

	/* 0x0 */ TAllocator<T> mAllocator;
	/* 0x4 */ u32 mSize;
	/* 0x8 */ TNode_ oEnd_;
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
		T operator->() { return *(T*)this->Base::operator->(); }

		iterator& operator++()
		{
			this->Base::operator++();
			return *this;
		}
		iterator operator++(int) { return iterator(this->Base::operator++(0)); }

		friend bool operator==(iterator fst, iterator snd)
		{
			return fst.p_ == snd.p_;
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
