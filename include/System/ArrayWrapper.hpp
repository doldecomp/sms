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

	T* begin() const { return mData; }
	T* end() const { return mData + mSize; }
	u32 size() const { return mSize; }
	T& operator[](int idx) const { return mData[idx]; }

public:
	/* 0x0 */ T* mData;
	/* 0x4 */ u32 mSize;
};

} // namespace

#endif
