#ifndef JSUINPUTSTREAM_HPP
#define JSUINPUTSTREAM_HPP

class JSUInputStream {
public:
	virtual ~JSUInputStream();
	virtual void getAvailable() = 0;
	void read(void *, s32);
	virtual void skip(u16);
	u32 _000;
	u32 _004;
};

class JSURandomInputStream: public JSUInputStream {};

#endif
