#ifndef JKR_FILE_H
#define JKR_FILE_H

#include <types.h>

class JKRFile
{
public:
  virtual ~JKRFile() {}

  virtual int open(const char*) = 0;
  virtual void close() = 0;
  virtual int readData(void*, s32, s32) = 0;
  virtual void writeData(const void*, s32, s32) = 0;
  virtual s32 getFileSize() const = 0;

  char unk4[0x14];
  u8 unk18;
};

#endif
