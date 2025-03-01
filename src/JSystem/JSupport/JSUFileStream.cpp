#include <JSystem/JSupport/JSUFileInputStream.hpp>

JSUFileInputStream::JSUFileInputStream(JKRFile* file)
{
	mFile     = file;
	mPosition = 0;
}

int JSUFileInputStream::readData(void* buf, s32 count)
{
	int result = 0;

	if (mFile->isAvailable()) {
		if (mPosition + (u32)count > mFile->getFileSize()) {
			count = mFile->getFileSize() - mPosition;
		}

		if (count > 0) {
			result = mFile->readData(buf, count, mPosition);
			mPosition += result;
		}
	}

	return result;
}

int JSUFileInputStream::seekPos(s32 offset, JSUStreamSeekFrom from)
{

	s32 oldPosition = mPosition;

	switch (from) {
	case JSUStreamSeekFrom_SET:
		mPosition = offset;
		break;

	case JSUStreamSeekFrom_END:
		mPosition = mFile->getFileSize() - offset;
		break;

	case JSUStreamSeekFrom_CUR:
		mPosition += offset;
		break;
	}

	if (mPosition < 0)
		mPosition = 0;

	if (mPosition > mFile->getFileSize())
		mPosition = mFile->getFileSize();

	return mPosition - oldPosition;
}
