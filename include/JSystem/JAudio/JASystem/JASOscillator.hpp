#ifndef JASOSCILLATOR_HPP
#define JASOSCILLATOR_HPP

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
	extern void* FORCE_STOP_TABLE;
	extern void* relTableSampleCell;
	extern void* relTableSqRoot;
	extern void* relTableSquare;
} // namespace Driver

} // namespace JASystem

#endif // JASOSCILLATOR_HPP
