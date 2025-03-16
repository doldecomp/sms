#ifndef JASTRACKPORT_HPP
#define JASTRACKPORT_HPP

#include <dolphin/types.h>

namespace JASystem {

class TTrackPort {
public:
	void init();
	void readWriteCheck(u32, u8);

	u16 readExport(u32 port)
	{
		mExportFlag[port] = 0;
		return mValue[port];
	}
	u16 readImport(u32 port)
	{
		mImportFlag[port] = 0;
		return mValue[port];
	}
	void writeExport(int port_num, u16 value)
	{
		mExportFlag[port_num] = 1;
		mValue[port_num]      = value;
	}

	u8 checkImport(int i) const { return mImportFlag[i]; }
	u8 checkExport(int i) const { return mExportFlag[i]; }

public:
	/* 0x00 */ u8 mImportFlag[16];
	/* 0x10 */ u8 mExportFlag[16];
	/* 0x20 */ u16 mValue[16];
};

} // namespace JASystem

#endif // JASTRACKPORT_HPP
