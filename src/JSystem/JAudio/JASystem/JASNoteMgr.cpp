#include <JSystem/JAudio/JASystem/JASNoteMgr.hpp>
#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <types.h>

namespace JASystem {

namespace TTrack {

	void TNoteMgr::init()
	{
		mBaseTime      = 0;
		mConnectCase   = 0;
		mLastNote      = 0;
		mBeforeTieMode = 0;
		for (int i = 0; i < 8; i++) {
			unk0[i]  = nullptr;
			unk20[i] = 0;
		}
	}

	void TNoteMgr::endProcess()
	{
		if (mBaseTime == 0xffffffff)
			return;

		if (mConnectCase != 0)
			return;

		unk0[0] = nullptr;
	}

	void* TNoteMgr::getChannel(int index)
	{
		TChannel* channel = unk0[index];
		if (!channel)
			return NULL;

		if (unk20[index] != channel->unkC6) {
			unk0[index] = nullptr;
			return NULL;
		}
		return channel;
	}

} // namespace TTrack

} // namespace JASystem
