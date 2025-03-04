#ifndef JGADGET_VECTOR_HPP
#define JGADGET_VECTOR_HPP

#include <algorithm>
#include <memory>
#include <types.h>
#include <JSystem/JGadget/allocator.hpp>

namespace JGadget {

// T* is the iterator type
// std::__copy<void*, 1, 0>::copy(void**, void**, void**)
// std::uninitialized_fill_n<void**, size_t, void*>(void**, unsigned
// long, void* const&)

template <typename T, typename Allocator = TAllocator<T> > class TVector {
	void DestroyElement_(T* b, T* e)
	{
		for (T* it = b; it != e; ++it)
			mAllocator.destroy(it);
	}

	void DestroyElement_all_() { DestroyElement_(mBegin, mEnd); }

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
		mBegin     = nullptr;
		mEnd       = mBegin;
		mCapacity  = 0;
		mGrowthMul = 2.0f;
		mGrowthAdd = 0;
	}
	TVector(size_t, const T&, const TAllocator<T>&);
	~TVector()
	{
		clear2();
		mAllocator.deallocate(mBegin, mCapacity);
	}

	size_t size() const { return mBegin == nullptr ? 0 : mEnd - mBegin; }
	size_t capacity() const { return mCapacity; }
	iterator begin() { return mBegin; }
	iterator end() { return mEnd; }

	T& operator[](size_t i) { return mBegin[i]; }
	const T& operator[](size_t i) const { return mBegin[i]; }

	iterator insert(iterator where, const T& what)
	{
		return insert(where, 1, what);
	}
	iterator insert(iterator where, size_t how_many, const T& what)
	{
		ptrdiff_t offset = where - mBegin;
		iterator it      = InsertRaw(where, how_many);
		if (it != mEnd)
			std::uninitialized_fill_n(it, how_many, what);
		return mBegin + offset;
	}

	iterator erase(iterator what) { }
	iterator erase(iterator b, iterator e)
	{
		T* newEnd2 = b;
		T* newEnd  = std::copy(e, mEnd, newEnd2);
		DestroyElement_(newEnd, mEnd);
		mEnd = newEnd;
		return newEnd2;
	}

	void clear() { erase(begin(), end()); }

	// fabricated TODO: figure out wtf is happening w/ the dtor
	void clear2() { clear(); }

	void assign(size_t, const T&) { }

	void resize(size_t new_size, const T& value = T()) { }

	void reserve(size_t new_capacity)
	{
		if (new_capacity <= mCapacity)
			return;

		T* newBegin = mAllocator.allocate(new_capacity);

		if (!newBegin)
			return;

		TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

		std::uninitialized_copy(mBegin, mEnd, newBegin);
		DestroyElement_all_();

		dealloc.set(mBegin);

		mEnd      = newBegin + (mEnd - mBegin);
		mBegin    = newBegin;
		mCapacity = new_capacity;
	}

	iterator InsertRaw(T* where, size_t count)
	{
		T* where2 = where;

		if (!count)
			return where;

		if (mCapacity <= count + size()) {
			T* newEnd = where2 + count;
			if (newEnd < mEnd) {
				T* split = mEnd - count;
				std::uninitialized_copy(split, mEnd, mEnd);
				std::copy_backward(where2, split, mEnd);
				DestroyElement_(where2, newEnd);
				mEnd += count;
				return where;
			} else {
				std::uninitialized_copy(where2, mEnd, newEnd);
				DestroyElement_(where2, mEnd);
				mEnd += count;
				return where;
			}
		} else {
			size_t newCap = GetSize_extend_(count);
			T* newBegin   = mAllocator.allocate(newCap);

			if (newBegin == nullptr)
				return end();

			TDestroyed_deallocate_ dealloc(mAllocator, newBegin);

			T* holeStart = std::uninitialized_copy(mBegin, where2, newBegin);
			std::uninitialized_copy(where2, mEnd, holeStart + count);
			DestroyElement_all_();

			dealloc.set(mBegin);

			mEnd      = newBegin + (mEnd - mBegin + count);
			mBegin    = newBegin;
			mCapacity = newCap;

			return holeStart;
		}
	}
	void ResizeRaw(size_t new_size) { }

private:
	Allocator mAllocator;
	T* mBegin;
	T* mEnd;
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
	typedef T* iterator;

	TVector_pointer() { }

	~TVector_pointer() { }

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
