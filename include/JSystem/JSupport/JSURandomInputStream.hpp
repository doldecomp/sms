#ifndef JSU_RANDOM_INPUT_STREAM_H
#define JSU_RANDOM_INPUT_STREAM_H

#include <types.h>
#include <JSystem/JSupport/JSUInputStream.hpp>

class JSURandomInputStream : public JSUInputStream {
public:
	virtual int getAvailable() const { return getLength() - getPosition(); }
	virtual int skip(s32 amount);
	virtual int readData(void* buf, s32 count)              = 0;
	virtual int getLength() const                           = 0;
	virtual int getPosition() const                         = 0;
	virtual int seekPos(s32 offset, JSUStreamSeekFrom from) = 0;

	int align(s32 alignment);
	u32 peek(void* buf, s32 len);
	int seek(s32 offset, JSUStreamSeekFrom from);

	// The ROM tests the drain condition through the two virtuals rather
	// than through getAvailable(), so the subtraction is spelled out here
	// (TCardSave::setMessageC). Inlined everywhere, so the map has no symbol.
	bool isDrained() const { return getLength() - getPosition() == 0; }

	u32 peekU32()
	{
		u32 i;
		peek(&i, sizeof(u32));
		return i;
	}
};

#endif
