#ifndef JSUINPUTSTREAM_HPP
#define JSUINPUTSTREAM_HPP

class JSUInputStream {
    public:
        virtual ~JSUInputStream();
        void read(void *, s32);
        virtual u32 getAvailable() = 0;
        virtual void skip(s32);
};

class JSURandomInputStream: public JSUInputStream {};

#endif
