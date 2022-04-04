#ifndef JSUMEMORYSTREAM_HPP
#define JSUMEMORYSTREAM_HPP

#include "JSystem/JSUInputStream.hpp"
#include "JSystem/JSUOutputStream.hpp"

class JSUMemoryInputStream: public JSURandomInputStream {
    public:
        void *buffer;
        s32 length;
        s32 position;
};

class JSUMemoryOutputStream: public JSURandomOutputStream {};

#endif
