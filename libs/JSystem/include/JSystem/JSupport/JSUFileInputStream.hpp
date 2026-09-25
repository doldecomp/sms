#ifndef JSU_FILE_INPUT_STREAM_HPP
#define JSU_FILE_INPUT_STREAM_HPP

#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <JSystem/JKernel/JKRFile.hpp>

class JSUFileInputStream : public JSURandomInputStream {
public:
	JSUFileInputStream(JKRFile* file);
	virtual int readData(void* buf, s32 count);
	virtual int seekPos(s32 offset, JSUStreamSeekFrom from);
	virtual ~JSUFileInputStream() { }
	virtual int getLength() const { return mFile->getFileSize(); }
	virtual int getPosition() const { return mPosition; }

	JKRFile* mFile;
	s32 mPosition;
};

#endif
