#ifndef JSU_MEMORY_STREAM_HPP
#define JSU_MEMORY_STREAM_HPP

#include <JSystem/JSupport/JSURandomInputStream.hpp>
#include <JSystem/JSupport/JSURandomOutputStream.hpp>


class JSUMemoryInputStream : public JSURandomInputStream {
public:
	void* buffer;
	s32 length;
	s32 position;
};

class JSUMemoryOutputStream : public JSURandomOutputStream { };

#endif
