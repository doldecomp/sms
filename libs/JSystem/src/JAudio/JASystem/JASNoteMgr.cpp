#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <dolphin/types.h>

namespace JASystem {

void TTrack::TNoteMgr::init()
{
	mBaseTime      = 0;
	mConnectCase   = 0;
	mLastNote      = 0;
	mBeforeTieMode = 0;
	for (int i = 0; i < 8; i++) {
		mVoices[i]           = nullptr;
		mVoiceGenerations[i] = 0;
	}
}

void TTrack::TNoteMgr::endProcess()
{
	if (mBaseTime == 0xffffffff)
		return;

	if (mConnectCase != 0)
		return;

	mVoices[0] = nullptr;
}

TChannel* TTrack::TNoteMgr::getChannel(int voice_slot)
{
	TChannel* channel = mVoices[voice_slot];
	if (!channel)
		return nullptr;

	if (mVoiceGenerations[voice_slot] != channel->mGeneration) {
		mVoices[voice_slot] = nullptr;
		return nullptr;
	}
	return channel;
}

} // namespace JASystem
