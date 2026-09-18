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

	// A raw read of the last slot, with neither the empty guard nor the
	// decrement pop() has: that is what retail expands inside
	// TSpineEnemy::doShortCut, and there is no map symbol for it anywhere, so
	// it has to stay expandable -- going through operator[] adds a level and
	// MWCC then emits an out-of-line copy instead.
	const T& top() const { return mData[mSize - 1]; }
	// fabricated and wrong
	T operator[](int index) const { return mData[index]; }

	// fabricated
	BOOL contain(const T& value) const
	{
		for (int i = 0; i < mSize; ++i)
			if (mData[mSize - 1 - i] == value)
				return true;

		return false;
	}

	// fabricated
	void clear() { mSize = 0; }

	// fabricated
	bool empty() const { return mSize == 0; }

	// fabricated
	int size() const { return mSize; }

	virtual ~TSolidStack() { }
};

#endif
