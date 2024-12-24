#ifndef BASE_PARAM_HPP
#define BASE_PARAM_HPP

#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

class TBaseParam {
public:
	virtual void load(JSUMemoryInputStream&);

	u16 keyCode;
	char* name;
	TBaseParam* next;
};

#endif
