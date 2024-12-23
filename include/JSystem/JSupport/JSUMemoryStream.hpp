#ifndef JSUMEMORYSTREAM_HPP
#define JSUMEMORYSTREAM_HPP

#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUOutputStream.hpp>

class JSUMemoryInputStream : public JSURandomInputStream {
public:
	void* buffer;
	s32 length;
	s32 position;
};

class JSUMemoryOutputStream : public JSURandomOutputStream { };

#endif
