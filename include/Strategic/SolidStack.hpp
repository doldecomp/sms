#ifndef ENEMY_SOLID_STACK_HPP
#define ENEMY_SOLID_STACK_HPP

#include <dolphin/types.h>
#include <types.h>

template <class T> class TSolidStack {
	/* 0x0 */ int mCapacity;
	/* 0x4 */ int mSize;
	/* 0x8 */ T* mData;
	/* 0xC */ // vt
public:
	TSolidStack(int capacity)
	    : mCapacity(capacity)
	    , mSize(0)
	    , mData(nullptr)
	{
		mData = new T[mCapacity];
	}

	// matching
	T pop()
	{
		if (mSize <= 0)
			return mData[mSize];

		--mSize;
		return mData[mSize];
	}

	// fabricated
	void push(const T& value)
	{
		if (mSize < mCapacity) {
			mData[mSize] = value;
			++mSize;
		}
	}

	// fabricated
	void clear() { mSize = 0; }
	// fabricated
	bool empty() const { return mSize == 0; }

	virtual ~TSolidStack() { }
};

#endif
