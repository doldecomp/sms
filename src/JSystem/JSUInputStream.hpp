#ifndef JSUINPUTSTREAM_HPP
#define JSUINPUTSTREAM_HPP

class JSUInputStream {
public:
	void read(void *, s32);
	u32 _000;
	u32 _004;
};

class JSURandomInputStream: public JSUInputStream {};

#endif
