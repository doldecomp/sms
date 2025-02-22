#ifndef JSU_INPUT_STREAM_H
#define JSU_INPUT_STREAM_H

#include <types.h>
#include <JSystem/JSupport/JSUIosBase.hpp>

class JSUInputStream : public JSUIosBase {
public:
	virtual ~JSUInputStream();
	virtual int getAvailable() const = 0;
	virtual int skip(s32 amount);
	virtual int readData(void* buf, s32 size) = 0;

	u32 read(void* buf, s32 size);
	char* read(char* buf);
	char* readString();
	char* readString(char* buf, u16 len);

	u32 read(s8& p) { return read(&p, sizeof(s8)); } /* @fabricated */
	u32 read(u8& p) { return read(&p, sizeof(u8)); }
	u32 read(bool& p) { return read(&p, sizeof(bool)); }
	u32 read(s16& p) { return read(&p, sizeof(s16)); } /* @fabricated */
	u32 read(u16& p) { return read(&p, sizeof(u16)); } /* @fabricated */
	u32 read(s32& p) { return read(&p, sizeof(s32)); } /* @fabricated */
	u32 read(u32& p) { return read(&p, sizeof(u32)); }
	u32 read(s64& p) { return read(&p, sizeof(s64)); } /* @fabricated */
	u32 read(u64& p) { return read(&p, sizeof(u64)); } /* @fabricated */

	u8 read8b()
	{
		u8 b;
		read(&b, sizeof(u8));
		return b;
	}

	u16 read16b()
	{
		u16 s;
		read(&s, sizeof(u16));
		return s;
	}

	u32 read32b()
	{
		u32 i;
		read(&i, sizeof(u32));
		return i;
	}

	/* @fabricated */
	s8 readS8()
	{
		s8 b;
		read(&b, sizeof(s8));
		return b;
	}

	u8 readU8()
	{
		u8 b;
		read(&b, sizeof(u8));
		return b;
	}

	s16 readS16()
	{
		s16 s;
		read(&s, sizeof(s16));
		return s;
	}

	u16 readU16()
	{
		u16 s;
		read(&s, sizeof(u16));
		return s;
	}

	s32 readS32()
	{
		s32 i;
		read(&i, sizeof(s32));
		return i;
	}

	u32 readU32()
	{
		u32 i;
		read(&i, sizeof(u32));
		return i;
	}

	/* @fabricated */
	f32 readF32()
	{
		f32 f;
		read(&f, sizeof(f32));
		return f;
	}

	JSUInputStream& operator>>(s8& p)
	{
		read(&p, sizeof(s8));
		return *this;
	}

	JSUInputStream& operator>>(u8& p)
	{
		read(&p, sizeof(u8));
		return *this;
	}

	JSUInputStream& operator>>(s16& p)
	{
		read(&p, sizeof(s16));
		return *this;
	}

	JSUInputStream& operator>>(u16& p)
	{
		read(&p, sizeof(u16));
		return *this;
	}

	JSUInputStream& operator>>(u32& p)
	{
		read(&p, sizeof(u32));
		return *this;
	}
};

#endif
