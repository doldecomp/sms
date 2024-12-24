#ifndef JSU_OUTPUT_STREAM_HPP
#define JSU_OUTPUT_STREAM_HPP

#include <types.h>
#include <JSystem/JSupport/JSUIosBase.hpp>

class JSUOutputStream : public JSUIosBase {
public:
	virtual ~JSUOutputStream();
	int write(const void* buf, s32 size);
	virtual int skip(s32 amount, s8 val);
	virtual int writeData(const void* buf, s32 size) = 0;
};

#endif
