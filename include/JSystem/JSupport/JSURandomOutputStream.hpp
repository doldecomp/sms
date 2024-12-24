#ifndef JSU_RANDOM_OUTPUT_STREAM_HPP
#define JSU_RANDOM_OUTPUT_STREAM_HPP

#include <JSystem/JSupport/JSUOutputStream.hpp>

class JSURandomOutputStream : public JSUOutputStream {
public:
	virtual ~JSURandomOutputStream() { }

	virtual int getLength() const   = 0;
	virtual int getPosition() const = 0;
	virtual void seek(s32 offset, JSUStreamSeekFrom from);
	virtual int getAvailable() const { return getLength() - getPosition(); }
	virtual int seekPos(s32 offset, JSUStreamSeekFrom from) = 0;
};

#endif
