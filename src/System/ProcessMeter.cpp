#include <System/ProcessMeter.hpp>
#include <version.h>

TProcessMeter::TProcessMeter(s32 r4)
{
	_000 = 0x28;
	_004 = VERSION_SELECT(0x1C2, GMSJ01(0x1B8), D38J01(0x1B8));
	_008 = 0x230;
	_00C = 4;
	_010 = r4;
	_014 = 0.0f;
	_018 = 0.0f;
	_01C = 0;
	_01E = 0;
}

// UNUSED
void TProcessMeter::draw(const JUTRect&) { }
