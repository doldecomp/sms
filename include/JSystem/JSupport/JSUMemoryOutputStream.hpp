#ifndef JSU_MEMORY_OUTPUT_STREAM_HPP
#define JSU_MEMORY_OUTPUT_STREAM_HPP

#include <JSystem/JSupport/JSURandomOutputStream.hpp>

class JSUMemoryOutputStream : public JSURandomOutputStream {
public:
	virtual int writeData(const void* buf, s32 count);
	virtual int seekPos(s32 offset, JSUStreamSeekFrom from);
	virtual ~JSUMemoryOutputStream() { }
	virtual int getLength() const { return mLength; }
	virtual int getPosition() const { return mPosition; }

	void setBuffer(void* buffer, s32 size);

public:
	const void* mBuffer;
	s32 mLength;
	s32 mPosition;
};

#endif
