#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

void JSUMemoryInputStream::setBuffer(const void* buffer, s32 size)
{
	mBuffer   = buffer;
	mLength   = size;
	mPosition = 0;
}

int JSUMemoryInputStream::readData(void* buf, s32 count)
{
	if (mPosition + count > mLength) {
		count = mLength - mPosition;
	}

	if (count > 0) {
		memcpy(buf, (char*)mBuffer + mPosition, count);
		mPosition += count;
	}

	return count;
}

int JSUMemoryInputStream::seekPos(s32 offset, JSUStreamSeekFrom from)
{
	s32 oldPosition = mPosition;

	switch (from) {
	case JSUStreamSeekFrom_SET:
		mPosition = offset;
		break;

	case JSUStreamSeekFrom_END:
		mPosition = mLength - offset;
		break;

	case JSUStreamSeekFrom_CUR:
		mPosition += offset;
		break;
	}

	if (mPosition < 0)
		mPosition = 0;

	if (mPosition > mLength)
		mPosition = mLength;

	return mPosition - oldPosition;
}

void JSUMemoryOutputStream::setBuffer(void* buffer, s32 size)
{
	mBuffer   = buffer;
	mLength   = size;
	mPosition = 0;
}

int JSUMemoryOutputStream::writeData(const void* buf, s32 count)
{
	if (mPosition + count > mLength) {
		count = mLength - mPosition;
	}

	if (count > 0) {
		memcpy((char*)mBuffer + mPosition, buf, count);
		mPosition += count;
	}

	return count;
}

int JSUMemoryOutputStream::seekPos(s32 offset, JSUStreamSeekFrom from)
{
	s32 oldPosition = mPosition;

	switch (from) {
	case JSUStreamSeekFrom_SET:
		mPosition = offset;
		break;

	case JSUStreamSeekFrom_END:
		mPosition = mLength - offset;
		break;

	case JSUStreamSeekFrom_CUR:
		mPosition += offset;
		break;
	}

	if (mPosition < 0)
		mPosition = 0;

	if (mPosition > mLength)
		mPosition = mLength;

	return mPosition - oldPosition;
}
