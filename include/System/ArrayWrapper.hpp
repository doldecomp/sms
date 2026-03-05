#ifndef SYSTEM_ARRAY_WRAPPER_HPP
#define SYSTEM_ARRAY_WRAPPER_HPP

#include <dolphin/types.h>
#include <types.h>

// They did a dum-dum here, anon namespace should never be used in headers.
namespace {

template <class T> class ArrayWrapper {
public:
	ArrayWrapper()
	    : mData(nullptr)
	    , mSize(0)
	{
	}

	ArrayWrapper(T* data, u32 size)
	    : mData(data)
	    , mSize(size)
	{
	}

	void set(T* data, u32 size)
	{
		mData = data;
		mSize = size;
	}

	T* begin() { return mData; }
	T* end() { return mData + mSize; }
	const T* begin() const { return mData; }
	const T* end() const { return mData + mSize; }
	s32 size() const { return mSize; }
	const T& operator[](int idx) const { return mData[idx]; }
	T& operator[](int idx) { return mData[idx]; }
	const T& front() const { return mData[0]; }
	T& front() { return mData[0]; }
	const T& back() const { return mData[mSize - 1]; }
	T& back() { return mData[mSize - 1]; }

public:
	/* 0x0 */ T* mData;
	/* 0x4 */ u32 mSize;
};

} // namespace

#endif
