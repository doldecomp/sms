#ifndef SYSTEM_PROCESS_METER_HPP
#define SYSTEM_PROCESS_METER_HPP

#include <dolphin/types.h>

class JUTRect;

class TProcessMeter {
public:
	TProcessMeter(s32);
	void draw(const JUTRect&);

	s32 _000;
	s32 _004;
	s32 _008;
	s32 _00C;
	s32 _010;
	f32 _014;
	f32 _018;
	s16 _01C;
	s16 _01E;
};

#endif
