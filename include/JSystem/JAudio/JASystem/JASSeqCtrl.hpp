#ifndef JASSEQCTRL_HPP
#define JASSEQCTRL_HPP

#include <dolphin/types.h>

namespace JASystem {
class TSeqCtrl {
public:
	void init(void* data, u32 size);
	void callIntr(void* intr);
	void waitCountDown();
	u16 read16();
	u16 get16(u32 offset) const;
	u32 get24(u32 offset) const;
	u32 get32(u32 offset) const;
	u32 read24();
	u32 read32();
	void loopE();
	void retIntr();
};
} // namespace JASystem

#endif // JASSEQCTRL_HPP
