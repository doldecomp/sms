#ifndef JASOSCILLATOR_HPP
#define JASOSCILLATOR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TOscillator {
public:
	void release();
	void initStart();
	void init();
	void getOffsetNoCount();
	void forceStop();
	void getOffset();
	void calc(short*);
};

namespace Driver {
	extern u32 FORCE_STOP_TABLE[];
	extern f32 relTableSampleCell[];
	extern f32 relTableSqRoot[];
	extern f32 relTableSquare[];
} // namespace Driver

} // namespace JASystem

#endif // JASOSCILLATOR_HPP
