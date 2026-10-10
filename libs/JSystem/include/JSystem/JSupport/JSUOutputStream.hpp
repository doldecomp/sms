#ifndef JSU_OUTPUT_STREAM_HPP
#define JSU_OUTPUT_STREAM_HPP

#include <dolphin/types.h>
#include <JSystem/JSupport/JSUIosBase.hpp>

class JSUOutputStream : public JSUIosBase {
public:
	JSUOutputStream() { }
	virtual ~JSUOutputStream();
	int write(const void* buf, s32 size);
	void write(const char* str);
	virtual int skip(s32 amount, s8 val);
	virtual int writeData(const void* buf, s32 size) = 0;

	JSUOutputStream& operator<<(s8 p)
	{
		write(&p, sizeof(s8));
		return *this;
	}

	JSUOutputStream& operator<<(u8 p)
	{
		write(&p, sizeof(u8));
		return *this;
	}

	JSUOutputStream& operator<<(s16 p)
	{
		write(&p, sizeof(s16));
		return *this;
	}

	JSUOutputStream& operator<<(u16 p)
	{
		write(&p, sizeof(u16));
		return *this;
	}

	JSUOutputStream& operator<<(int p)
	{
		write(&p, sizeof(int));
		return *this;
	}

	JSUOutputStream& operator<<(unsigned int p)
	{
		write(&p, sizeof(unsigned int));
		return *this;
	}

	JSUOutputStream& operator<<(s32 p)
	{
		write(&p, sizeof(s32));
		return *this;
	}

	JSUOutputStream& operator<<(u32 p)
	{
		write(&p, sizeof(u32));
		return *this;
	}

	JSUOutputStream& operator<<(s64 p)
	{
		write(&p, sizeof(s64));
		return *this;
	}

	JSUOutputStream& operator<<(u64 p)
	{
		write(&p, sizeof(u64));
		return *this;
	}

	JSUOutputStream& operator<<(const char* p)
	{
		write(p);
		return *this;
	}
};

#endif
