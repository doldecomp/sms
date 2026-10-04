#include <System/ProcessMeter.hpp>

TProcessMeter::TProcessMeter(s32 r4)
{
	_000 = 0x28;
#ifdef VERSION_GMSP01
	_004 = 0x1C2;
#else
	_004 = 0x1B8;
#endif
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
