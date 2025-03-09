#include "JSystem/JAudio/JASystem/JASTrackPort.hpp"

namespace JASystem {

void TTrackPort::init()
{
	for (int i = 0; i != 16; i++) {
		mImportFlag[i] = 0;
		mExportFlag[i] = 0;
		mValue[i]      = 0;
	}
}

void TTrackPort::readWriteCheck(u32 param, u8 param2) { }

} // namespace JASystem
