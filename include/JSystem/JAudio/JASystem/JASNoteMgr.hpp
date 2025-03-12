#ifndef JASNOTEMGR_HPP
#define JASNOTEMGR_HPP

#include <dolphin/types.h>

namespace JASystem {

class TChannel;

namespace TTrack {
	class TNoteMgr {
	public:
		void init();
		void endProcess();
		void* getChannel(int);

	public:
		/* 0x00 */ TChannel* unk0[8];
		/* 0x20 */ u16 unk20[8];
		/* 0x30 */ int mBaseTime;
		/* 0x34 */ u8 mConnectCase;
		/* 0x35 */ u8 mLastNote;
		/* 0x36 */ u8 mBeforeTieMode;
	};
} // namespace TTrack

} // namespace JASystem

#endif // JASNOTEMGR_HPP
