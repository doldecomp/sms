#ifndef JSUINPUTSTREAM_HPP
#define JSUINPUTSTREAM_HPP value

class JSUInputStream {
public:
	void read(void *, s32);
	u32 _000;
	u32 _004;
};

class JSURandomInputStream {
public:
	JSUInputStream _000;
};

class JSUMemoryInputStream {
public:
	JSURandomInputStream _000;
	void *buffer;
	s32 length;
	s32 position;
};

#endif
