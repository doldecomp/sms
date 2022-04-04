#include "dolphin.h"

#include "types.h"

TProcessMeter::TProcessMeter(s32 r4)
{
	this->_000 = 0x28;
	this->_004 = 0x1B8;
	this->_008 = 0x230;
	this->_00C = 4;
	this->_010 = r4;
	this->_014 = 0.0f;
	this->_018 = 0.0f;
	this->_01C = 0;
	this->_01E = 0;
}
