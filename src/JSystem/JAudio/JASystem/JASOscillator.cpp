#include <JSystem/JAudio/JASystem/JASOscillator.hpp>

namespace JASystem {

bool TOscillator::release() { }

void TOscillator::initStart() { }

void TOscillator::init() { }

void TOscillator::getOffsetNoCount() { }

bool TOscillator::forceStop() { }

f32 TOscillator::getOffset() { }

void TOscillator::calc(short* param) { }

namespace Driver {
	u32 FORCE_STOP_TABLE[3] = { 0xf, 0xf, 0x0 };
	f32 relTableSampleCell[17]
	    = { 1.0f,        0.970489f,   0.781274f,   0.54628098f, 0.39979199f,
		    0.28931499f, 0.21210399f, 0.15747599f, 0.112613f,   0.081789598f,
		    0.0579852f,  0.0436415f,  0.0308237f,  0.0237129f,  0.0152593f,
		    0.00915555f, 0.0f };
	f32 relTableSqRoot[17]
	    = { 1.0f,        0.878906f,    0.765625f, 0.660156f,  0.5625f,
		    0.472656f,   0.390625f,    0.316406f, 0.25f,      0.191406f,
		    0.140625f,   0.097656198f, 0.0625f,   0.0351562f, 0.015625f,
		    0.00390625f, 0.0f };
	f32 relTableSquare[17]
	    = { 1.0f,        0.96824598f, 0.935414f, 0.90138799f, 0.86602497f,
		    0.82915598f, 0.790569f,   0.75f,     0.707107f,   0.66143799f,
		    0.61237198f, 0.559017f,   0.5f,      0.43301299f, 0.353553f,
		    0.25f,       0.0f };
} // namespace Driver

} // namespace JASystem
