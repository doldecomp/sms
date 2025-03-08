#ifndef BASE_PARAM_HPP
#define BASE_PARAM_HPP

#include <JSystem/JSupport/JSUMemoryInputStream.hpp>

class TParams;

class TBaseParam {
public:
	TBaseParam(TParams* params, unsigned short code, const char* paramName);
	virtual inline void load(JSUMemoryInputStream&);

	u16 keyCode;
	const char* name;
	TBaseParam* next;
};

#endif
