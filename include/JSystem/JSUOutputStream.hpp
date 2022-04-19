#ifndef JSUOUTPUTSTREAM_HPP
#define JSUOUTPUTSTREAM_HPP

#include "dolphin.h"

class JSUOutputStream {
    public:
        virtual ~JSUOutputStream();
        void write(void const *, s32);
        virtual void skip(u32, s8);
};

class JSURandomOutputStream: public JSUOutputStream {};

#endif
