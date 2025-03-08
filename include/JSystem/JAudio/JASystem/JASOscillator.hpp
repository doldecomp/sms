#ifndef JASOSCILLATOR_HPP
#define JASOSCILLATOR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TOscillator {
public:
	struct Osc_ {
		/* 0x00 */ u8 unk0;
		/* 0x04 */ f32 unk4;
		/* 0x08 */ s16* unk8;
		/* 0x0C */ s16* unkC;
		/* 0x10 */ f32 unk10;
		/* 0x14 */ f32 unk14;
	};

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
