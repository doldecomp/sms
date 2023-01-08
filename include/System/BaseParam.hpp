#ifndef BASEPARAM_HPP
#define BASEPARAM_HPP

#include "dolphin.h"

#include "types.h"
#include "JSystem/JSUMemoryStream.hpp"

class TBaseParam {
public:
	virtual void load(JSUMemoryInputStream&);

	u16 keyCode;
	char *name;
	TBaseParam *next;
};

#endif
