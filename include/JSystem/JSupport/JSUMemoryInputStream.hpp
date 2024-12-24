#ifndef JSU_MEMORY_INPUT_STREAM_HPP
#define JSU_MEMORY_INPUT_STREAM_HPP

#include <JSystem/JSupport/JSURandomInputStream.hpp>

class JSUMemoryInputStream : public JSURandomInputStream {
public:
	void setBuffer(const void* buffer, s32 size);
	virtual int readData(void* buf, s32 count);
	virtual int seekPos(s32 offset, JSUStreamSeekFrom from);
	virtual ~JSUMemoryInputStream() { }
	virtual int getLength() const { return mLength; }
	virtual int getPosition() const { return mPosition; }

	const void* mBuffer;
	s32 mLength;
	s32 mPosition;
};

#endif
