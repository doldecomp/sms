#ifndef JGADGET_VECTOR_HPP
#define JGADGET_VECTOR_HPP

#include <algorithm>
#include <memory>
#include <types.h>
#include <JSystem/JGadget/std-memory.hpp>

namespace JGadget {

template <typename T, typename Allocator = TAllocator<T> > class TVector {
	void DestroyElement_(T* pFirst, T* pLast)
	{
		// clang-format off
		JGADGET_ASSERT((pBegin_<=pFirst)&&(pFirst<pEnd_));
		JGADGET_ASSERT((pBegin_<=pLast)&&(pLast<=pEnd_));
		// clang-format on

		for (T* it = pFirst; it != pLast; ++it)
			mAllocator.destroy(it);
	}

	void DestroyElement_all_() { DestroyElement_(pBegin_, pEnd_); }

	size_t GetSize_extend_(size_t count) const
	{
		size_t sz     = size();
		size_t newSz  = sz + count;
		size_t newCap = (size_t)(sz * mGrowthMul) + mGrowthAdd;
		return newSz > newCap ? newSz : newCap;
	}

	struct TDestroyed_deallocate_ {
		TDestroyed_deallocate_(JGadget::TAllocator<T>& alloc, T* pointer)
		{
			mAllocator = &alloc;
			mPointer   = pointer;
		}

		void set(T* p) { mPointer = p; }

		~TDestroyed_deallocate_() { mAllocator->deallocate(mPointer, 0); }

		Allocator* mAllocator;
		T* mPointer;
	};

public:
	typedef T* iterator;
	typedef const T* const_iterator;

	TVector(const TAllocator<T>& alloc = TAllocator<T>())
	    : mAllocator(alloc)
	    , pBegin_(nullptr)
	    , pEnd_(pBegin_)
	    , mCapacity(0)
	    , mGrowthMul(2.0f)
	    , mGrowthAdd(0)
	{
	}
	TVector(size_t param_1, const T& param_2 = T(),
	        const TAllocator<T>& param_3 = TAllocator<T>())
	    : mAllocator(param_3)
	    , pBegin_(nullptr)
	    , pEnd_(pBegin_)
	    , mCapacity(0)
	    , mGrowthMul(2.0f)
	    , mGrowthAdd(0)
	{
		insert(end(), param_1, param_2);
	}

	TVector& operator=(const TVector& other)
	{
		if (this == &other)
			return *this;

		size_t ourSize   = size();
		size_t otherSize = other.size();
		iterator ourBeg  = begin();
		iterator ourEnd  = end();
		if (ourSize < otherSize) {
			if (mCapacity < otherSize) {
				T* newBegin = mAllocator.allocate(otherSize);

				if (!newBegin) {
					JGADGET_WARN("can't allocate memory");
					return *this;
				}

				TDestroyed_deallocate_ dealloc(mAllocator, newBegin);
				std::uninitialized_copy(pBegin_, pEnd_, newBegin);
				DestroyElement_all_();
				dealloc.set(pBegin_);
				pBegin_   = newBegin;
				pEnd_     = newBegin + otherSize;
				mCapacity = otherSize; // bug in TP
			} else {
				std::copy(ourBeg, ourBeg + ourSize, pBegin_);
				pEnd_
				    = std::uninitialized_copy(ourBeg + ourSize, ourEnd, pEnd_);
			}
		} else {
			iterator it = std::copy(ourBeg, ourEnd, pBegin_);
			DestroyElement_(it, ourEnd);
			pEnd_ = it;
		}

		return *this;
	}

	~TVector()
	{
		clear();
		// clang-format off
		JGADGET_ASSERT(size()==0);
		// clang-format on
		mAllocator.deallocate(pBegin_, mCapacity);
	}

	size_t size() const { return pBegin_ == nullptr ? 0 : pEnd_ - pBegin_; }
	size_t capacity() const { return mCapacity; }
	bool empty() const { return pBegin_ == pEnd_; }
	iterator begin() { return pBegin_; }
	const_iterator begin() const { return pBegin_; }
	iterator end() { return pEnd_; }
	const_iterator end() const { return pEnd_; }

	T& operator[](size_t u)
	{
		// clang-format off
		JGADGET_ASSERT(u<size());
		// clang-format on
		return pBegin_[u];
	}
	const T& operator[](size_t u) const
	{
		// clang-format off
		JGADGET_ASSERT(u<size());
		// clang-format on
		return pBegin_[u];
	}

	iterator insert(iterator where, const T& what)
	{
		size_t offset = where - pBegin_;
		insert(where, 1, what);
		return pBegin_ + offset;
	}

	void insert(iterator where, size_t how_many, const T& what)
	{
		if (!how_many)
			return;

		iterator it = InsertRaw(where, how_many);

		if (it == pEnd_) {
			JGADGET_WARN("can't allocate memory");
			return;
		}

		std::uninitialized_fill_n(it, how_many, what);
		// NOTE: non-standard: forgot to return the iterator
	}

	iterator erase(iterator where)
	{
		JGADGET_ASSERT(!empty());
		return erase(where, where + 1);
	}
	iterator erase(iterator pItFirst, iterator pItLast)
	{
		// clang-format off
		JGADGET_ASSERT((pBegin_<=pItFirst)&&(pItFirst<=pEnd_));
		JGADGET_ASSERT((pBegin_<=pItLast)&&(pItLast<=pEnd_));
		JGADGET_ASSERT(pItFirst<=pItLast);
		// clang-format on

		T* newEnd2 = pItFirst;
		T* newEnd  = std::copy(pItLast, pEnd_, newEnd2);
		DestroyElement_(newEnd, pEnd_);
		pEnd_ = newEnd;
		return newEnd2;
	}

	void push_back(const T& value) { insert(end(), value); }
	void pop_back() { erase(end() - 1); }

	void clear() { erase(begin(), end()); }

	void assign(size_t, const T&) { } // not present in any other game

	void resize(size_t new_size, const T& value = T())
	{
		size_t sz = size();
		if (new_size > sz) {
			insert(end(), new_size - sz, value);
		} else if (new_size != size()) {
			ResizeNotLarger(new_size);
		}
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= mCapacity)
			return;

		T* const newBegin = mAllocator.allocate(new_capacity);

		if (!newBegin) {
			JGADGET_WARN("can't allocate memory");
			return;
		}

		TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

		std::uninitialized_copy(pBegin_, pEnd_, newBegin);
		DestroyElement_all_();

		dealloc.set(pBegin_);

		const size_t size = pEnd_ - pBegin_;

		pEnd_     = newBegin + size;
		pBegin_   = newBegin;
		mCapacity = new_capacity;
	}

	iterator InsertRaw(T* pIt, size_t count)
	{
		// clang-format off
		JGADGET_ASSERT((pBegin_<=pIt)&&(pIt<=pEnd_));
		// clang-format on

		if (count == 0)
			return pIt;

		if (mCapacity <= count + size()) {
			T* holeEnd = pIt + count;
			if (holeEnd < pEnd_) {
				T* split = pEnd_ - count;
				std::uninitialized_copy(split, pEnd_, pEnd_);
				std::copy_backward(pIt, split, pEnd_);
				DestroyElement_(pIt, holeEnd);
				pEnd_ += count;
				return pIt;
			} else {
				std::uninitialized_copy(pIt, pEnd_, holeEnd);
				DestroyElement_(pIt, pEnd_);
				pEnd_ += count;
				return pIt;
			}
		} else {
			size_t newCap = GetSize_extend_(count);
			T* newBegin   = mAllocator.allocate(newCap);

			if (!newBegin)
				return end();

			TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

			T* holeStart = std::uninitialized_copy(pBegin_, pIt, newBegin);
			std::uninitialized_copy(pIt, pEnd_, holeStart + count);
			DestroyElement_all_();

			dealloc.set(pBegin_);

			pEnd_     = newBegin + (pEnd_ - pBegin_ + count);
			pBegin_   = newBegin;
			mCapacity = newCap;

			return holeStart;
		}
	}
	T* ResizeRaw(size_t new_size)
	{
		size_t sz = size();
		if (new_size > sz) {
			return InsertRaw(end(), new_size - sz);
		} else if (new_size != size()) {
			ResizeNotLarger(new_size);
			return end();
		}
	}
	// fabricated but present in TP as Resize_notLarger_
	void ResizeNotLarger(size_t u)
	{
		size_t uSize = size();
		// clang-format off
		JGADGET_ASSERT(u<=uSize);
		// clang-format on
		erase(pBegin_ + u, pEnd_);
	}

private:
	Allocator mAllocator;
	T* pBegin_;
	T* pEnd_;
	size_t mCapacity;
	float mGrowthMul;
	size_t mGrowthAdd;
};

class TVector_pointer_void : public TVector<void*> {
	typedef TVector<void*> Base;
	typedef void* value_type;

public:
	typedef Base::iterator iterator;

	TVector_pointer_void(
	    const TAllocator<value_type>& = TAllocator<value_type>());
	TVector_pointer_void(size_t, const value_type&,
	                     const TAllocator<value_type>&);
	TVector_pointer_void& operator=(const JGadget::TVector_pointer_void&);
	~TVector_pointer_void();

	size_t size() const { return Base::size(); }

	iterator insert(iterator where, const value_type& what);
	void insert(iterator, size_t, const value_type&);

	iterator erase(iterator);
	iterator erase(iterator, iterator);

	void push_back(const value_type& value) { insert(end(), value); }

	void assign(size_t, const value_type&);
	void resize(size_t, const value_type&);
	void reserve(size_t);

	value_type* InsertRaw(value_type*, size_t);
	value_type* ResizeRaw(size_t);
};

template <class T> class TVector_pointer : public TVector_pointer_void {
	typedef TVector_pointer_void Base;
	typedef T* value_type;

public:
	typedef T** iterator;

	TVector_pointer() { }

	~TVector_pointer() { }

	iterator begin() { return iterator(Base::begin()); }
	iterator end() { return iterator(Base::end()); }
	size_t size() const { return Base::size(); }

	T& operator[](size_t i) { return *static_cast<T*>(Base::operator[](i)); }
	const T& operator[](size_t i) const
	{
		return *static_cast<T*>(Base::operator[](i));
	}

	void push_back(const value_type& value) { Base::push_back(value); }
};

} // namespace JGadget

#endif
