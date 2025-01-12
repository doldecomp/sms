#ifndef JUT_GAMEPAD_HPP
#define JUT_GAMEPAD_HPP

#include <types.h>
#include <dolphin/os.h>
#include <dolphin/pad.h>
#include <JSystem/JKernel/JKRDisposer.hpp>

class JUTGamePad : public JKRDisposer {
public:
	enum EPadPort {
		Port_unknown = -999,
		Port_Invalid = -1,
		Port1        = 0,
		Port2,
		Port3,
		Port4,
		PortRecorder
	};
};

#endif
