#ifndef BASEPARAM_HPP
#define BASEPARAM_HPP

#include <dolphin.h>

#include "types.h"

class TBaseParam {
public:
	u16 keyCode;
	char *name;
	TBaseParam *next;
};

#endif
