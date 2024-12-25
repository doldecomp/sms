#ifndef JKR_FILE_H
#define JKR_FILE_H

#include <types.h>
#include <JSystem/JKernel/JKRDisposer.hpp>

class JKRFile : public JKRDisposer {
public:
	JKRFile() { mFileOpen = false; }
	virtual ~JKRFile() { }

	virtual bool open(const char*)               = 0;
	virtual bool close()                         = 0;
	virtual int readData(void*, s32, s32)        = 0;
	virtual int writeData(const void*, s32, s32) = 0;
	virtual s32 getFileSize() const              = 0;

	bool isAvailable() { return mFileOpen; }

protected:
	bool mFileOpen;
};

#endif
