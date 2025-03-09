#ifndef JASTRACKPORT_HPP
#define JASTRACKPORT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrackPort {
public:
	void init();
	void readWriteCheck(u32, u8);

public:
	/* 0x00 */ u8 mImportFlag[16];
	/* 0x10 */ u8 mExportFlag[16];
	/* 0x20 */ u16 mValue[16];
};

} // namespace JASystem

#endif // JASTRACKPORT_HPP
