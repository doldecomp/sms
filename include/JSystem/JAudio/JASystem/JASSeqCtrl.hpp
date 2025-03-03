#ifndef JASSEQCTRL_HPP
#define JASSEQCTRL_HPP

#include "types.h"

namespace JASystem {
class TSeqCtrl {
public:
	void init(void* data, unsigned long size);
	void callIntr(void* intr);
	void waitCountDown();
	unsigned short read16();
	unsigned short get16(unsigned long offset) const;
	unsigned int get24(unsigned long offset) const;
	unsigned int get32(unsigned long offset) const;
	unsigned int read24();
	unsigned int read32();
	void loopE();
	void retIntr();

private:
	// Member variables would go here
};
} // namespace JASystem

#endif // JASSEQCTRL_HPP
