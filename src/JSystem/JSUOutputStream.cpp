#include <JSystem/JSupport/JSUOutputStream.hpp>
#include <JSystem/JSupport/JSURandomOutputStream.hpp>

JSUOutputStream::~JSUOutputStream() { }

int JSUOutputStream::write(const void* buf, s32 size)
{
	int len = writeData(buf, size);
	if (len != size) {
		setState(EOF);
	}
	return len;
}

int JSUOutputStream::skip(s32 amount, s8 val)
{
  int i;
	for (i = 0; i < amount; ++i) {
		u32 r = writeData(&val, 1);
    if (r != 1) {
      setState(EOF);
      break;
    }
	}
  return i;
}

void JSURandomOutputStream::seek(s32 offset, JSUStreamSeekFrom from)
{
  seekPos(offset, from);
  clrState(EOF);
}
