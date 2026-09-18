#ifndef JSU_OUTPUT_STREAM_HPP
#define JSU_OUTPUT_STREAM_HPP

#include <types.h>
#include <JSystem/JSupport/JSUIosBase.hpp>

class JSUOutputStream : public JSUIosBase {
public:
	virtual ~JSUOutputStream();
	int write(const void* buf, s32 size);

	// By value, mirroring JSUInputStream's read(u8&) family: the ROM gives
	// every write of the same scanned byte its own stack slot, which is what
	// the by-value parameter does at each inline expansion
	// (TCardSave::setMessageC). Inlined everywhere, so the map has no symbol.
	int write(u8 value) { return write(&value, sizeof(u8)); }

	virtual int skip(s32 amount, s8 val);
	virtual int writeData(const void* buf, s32 size) = 0;
};

#endif
