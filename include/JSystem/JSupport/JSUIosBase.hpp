#ifndef JSU_IOS_BASE_H
#define JSU_IOS_BASE_H

#include <types.h>
#include <JSystem/JSupport/JSUStreamEnum.hpp>

class JSUIosBase {
public:
	inline JSUIosBase()
	    : mState(GOOD)
	{
	}

	virtual ~JSUIosBase() { }

	bool isGood() { return !mState; }
	void clrState(EIoState ioState) { mState &= ~ioState; }
	void setState(EIoState ioState) { mState |= ioState; }

	u8 mState;
};

#endif
