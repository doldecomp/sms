#ifndef JGADGET_VECTOR_HPP
#define JGADGET_VECTOR_HPP

#include <algorithm>
#include <memory>
#include <types.h>
#include <JSystem/JGadget/std-memory.hpp>

namespace JGadget {

// T* is the iterator type
// std::__copy<void*, 1, 0>::copy(void**, void**, void**)
// std::uninitialized_fill_n<void**, size_t, void*>(void**, unsigned
// long, void* const&)

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

	TVector(const TAllocator<T>& alloc = TAllocator<T>())
	    : mAllocator(alloc)
	{
		pBegin_    = nullptr;
		pEnd_      = pBegin_;
		mCapacity  = 0;
		mGrowthMul = 2.0f;
		mGrowthAdd = 0;
	}
	TVector(size_t, const T&, const TAllocator<T>&);
	~TVector()
	{
		clear2();
		mAllocator.deallocate(pBegin_, mCapacity);
	}

	size_t size() const { return pBegin_ == nullptr ? 0 : pEnd_ - pBegin_; }
	size_t capacity() const { return mCapacity; }
	iterator begin() { return pBegin_; }
	iterator end() { return pEnd_; }

	T& operator[](size_t i) { return pBegin_[i]; }
	const T& operator[](size_t i) const { return pBegin_[i]; }

	iterator insert(iterator where, const T& what)
	{
		return insert(where, 1, what);
	}
	iterator insert(iterator where, size_t how_many, const T& what)
	{
		ptrdiff_t offset = where - pBegin_;
		iterator it      = InsertRaw(where, how_many);
		if (it != pEnd_)
			std::uninitialized_fill_n(it, how_many, what);
		return pBegin_ + offset;
	}

	iterator erase(iterator what) { }
	iterator erase(iterator b, iterator e)
	{
		T* newEnd2 = b;
		T* newEnd  = std::copy(e, pEnd_, newEnd2);
		DestroyElement_(newEnd, pEnd_);
		pEnd_ = newEnd;
		return newEnd2;
	}

	void clear() { erase(begin(), end()); }

	// fabricated TODO: figure out wtf is happening w/ the dtor
	void clear2() { clear(); }

	void assign(size_t, const T&) { }

	void resize(size_t new_size, const T& value = T())
	{
		size_t sz = size();
		if (new_size > sz) {
			// TODO: one more inline here?
			iterator it = end();
			if (new_size - sz > 0)
				insert(it, new_size - sz, value);
		} else if (new_size != size()) {
			// TODO: one more inline here too???
			erase(pBegin_ + new_size, pEnd_);
		}
	}

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= mCapacity)
			return;

		T* newBegin = mAllocator.allocate(new_capacity);

		if (!newBegin) {
			JGADGET_WARN("can't allocate memory");
			return;
		}

		TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

		std::uninitialized_copy(pBegin_, pEnd_, newBegin);
		DestroyElement_all_();

		dealloc.set(pBegin_);

		pEnd_     = newBegin + (pEnd_ - pBegin_);
		pBegin_   = newBegin;
		mCapacity = new_capacity;
	}

	iterator InsertRaw(T* where, size_t count)
	{
		T* where2 = where;

		if (!count)
			return where;

		if (mCapacity <= count + size()) {
			T* newEnd = where2 + count;
			if (newEnd < pEnd_) {
				T* split = pEnd_ - count;
				std::uninitialized_copy(split, pEnd_, pEnd_);
				std::copy_backward(where2, split, pEnd_);
				DestroyElement_(where2, newEnd);
				pEnd_ += count;
				return where;
			} else {
				std::uninitialized_copy(where2, pEnd_, newEnd);
				DestroyElement_(where2, pEnd_);
				pEnd_ += count;
				return where;
			}
		} else {
			size_t newCap = GetSize_extend_(count);
			T* newBegin   = mAllocator.allocate(newCap);

			if (newBegin == nullptr)
				return end();

			TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

			T* holeStart = std::uninitialized_copy(pBegin_, where2, newBegin);
			std::uninitialized_copy(where2, pEnd_, holeStart + count);
			DestroyElement_all_();

			dealloc.set(pBegin_);

			pEnd_     = newBegin + (pEnd_ - pBegin_ + count);
			pBegin_   = newBegin;
			mCapacity = newCap;

			return holeStart;
		}
	}
	void ResizeRaw(size_t new_size) { }

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
	void operator=(const JGadget::TVector_pointer_void&);
	~TVector_pointer_void();

	size_t size() const { return Base::size(); }

	iterator insert(iterator, const value_type&);
	iterator insert(iterator, size_t, const value_type&);

	iterator erase(iterator);
	iterator erase(iterator, iterator);

	void push_back(const value_type& value) { insert(end(), value); }

	void assign(size_t, const value_type&);
	void resize(size_t, const value_type&);
	void reserve(size_t);

	void InsertRaw(value_type*, size_t);
	void ResizeRaw(size_t);
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
