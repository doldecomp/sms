#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <macros.h>

JSUInputStream::~JSUInputStream() { }

u32 JSUInputStream::read(void* buf, s32 size)
{
	int len = readData(buf, size);
	if (len != size) {
		setState(EOF);
	}
	return len;
}

char* JSUInputStream::readString()
{
	u16 strLen;
	if (readData(&strLen, sizeof(strLen)) != sizeof(strLen)) {
		setState(EOF);
		return nullptr;
	}

	char* buf = new char[strLen + 1];

	int r;
	if (buf == nullptr) {
		r = skip(strLen);
	} else {
		r      = readData(buf, strLen);
		buf[r] = '\0';
	}

	if (r != strLen) {
		setState(EOF);
	}
	return buf;
}

char* JSUInputStream::readString(char* buf, u16 len)
{
	u16 strLen;
	if (readData(&strLen, sizeof(strLen)) != sizeof(strLen)) {
		buf[0] = '\0';
		setState(EOF);
		return nullptr;
	}

	s32 r = 0;
	if (strLen < len) {
		r      = readData(buf, strLen);
		buf[r] = '\0';
	} else {
		r      = readData(buf, len - 1);
		buf[r] = '\0';
		r += skip(strLen - (len - 1));
	}

	if (r != strLen) {
		setState(EOF);
	}

	return buf;
}

int JSUInputStream::skip(s32 amount)
{
	u8 _p;
	int i;

	for (i = 0; i < amount; i++) {
		if (readData(&_p, sizeof(_p)) != sizeof(_p)) {
			setState(EOF);
			break;
		}
	}

	return i;
}

/* JSURandomInputStream */

int JSURandomInputStream::skip(s32 amount)
{
	int s = seekPos(amount, JSUStreamSeekFrom_CUR);
	if (s != amount) {
		setState(EOF);
	}
	return s;
}

int JSURandomInputStream::align(s32 alignment)
{
	int pos     = getPosition();
	int aligned = ALIGN_NEXT(pos, alignment);
	int change;
	// NOTE: this is insane but it matches
	if ((change = aligned - pos) != 0) {
		int s = seekPos(aligned, JSUStreamSeekFrom_SET);
		if (s != change) {
			setState(EOF);
		}
	}

	return change;
}

u32 JSURandomInputStream::peek(void* buf, s32 len)
{
	int r   = 0; // NOTE: does not match unless we declare this first
	int pos = getPosition();
	// NOTE: this is read, but there's no inlining padding here so the code was
	// actually copy-pasted in the original?
	r = readData(buf, len);
	if (r != len) {
		setState(EOF);
	}
	if (r != 0) {
		seekPos(pos, JSUStreamSeekFrom_SET);
	}

	return r;
}

int JSURandomInputStream::seek(s32 offset, JSUStreamSeekFrom from)
{
	int s = seekPos(offset, from);
	clrState(EOF);
	return s;
}
