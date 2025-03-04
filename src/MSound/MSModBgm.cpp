#include <MSound/MSModBgm.hpp>

void MSModBgm::modBgm(u8 param1, u8 param2) { }

void MSModBgm::changeTempo(u8 param1, u8 param2) { }

void MSModBgm::loop() { }

f32 MSBgmXFade::scTiming[18] = {
	0.052632,   0.105263,   0.157895, 0.210526,   0.26315799, 0.315789,
	0.36842099, 0.42105299, 0.473684, 0.52631599, 0.578947,   0.63157898,
	0.684211,   0.73684198, 0.789474, 0.84210497, 0.894737,   0.947368,
};
f32 MSBgmXFade::scExp[18] = {
	0.0,      0.0,      0.030207,   0.063591, 0.100485, 0.14126,
	0.186324, 0.236126, 0.29116699, 0.351996, 0.419223, 0.49351999,
	0.57563,  0.666377, 0.766667,   0.877505, 1.0,      1.0,
};

void MSBgmXFade::xFadeBgm(f32 param) { }

void MSBgmXFade::xFadeBgmForce(f32 param) { }

f32 MSBgmXFade::getTimingForce(f32 param) { return 0.0f; }

void MSBgmXFade::getTiming(f32 param1, u32* param2) { }
