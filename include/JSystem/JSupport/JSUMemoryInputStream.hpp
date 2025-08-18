#ifndef JSU_MEMORY_INPUT_STREAM_HPP
#define JSU_MEMORY_INPUT_STREAM_HPP

#include <JSystem/JSupport/JSURandomInputStream.hpp>

class JSUMemoryInputStream : public JSURandomInputStream {
public:
	JSUMemoryInputStream(const void* buffer, s32 size)
	{
		setBuffer(buffer, size);
	}

	// fabricated
	JSUMemoryInputStream() { setBuffer(nullptr, 0); }

	void setBuffer(const void* buffer, s32 size);
	virtual int readData(void* buf, s32 count);
	virtual int seekPos(s32 offset, JSUStreamSeekFrom from);
	virtual ~JSUMemoryInputStream() { }
	virtual int getLength() const { return mLength; }
	virtual int getPosition() const { return mPosition; }

	// fabricated
	bool isNotDrained() const { return getLength() - getPosition() != 0; }

	/* 0x8 */ const void* mBuffer;
	/* 0xC */ s32 mLength;
	/* 0x10 */ s32 mPosition;
};

#endif
