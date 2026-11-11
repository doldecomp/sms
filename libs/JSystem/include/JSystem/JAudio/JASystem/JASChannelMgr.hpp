#ifndef JASCHANNELMGR_HPP
#define JASCHANNELMGR_HPP

#include <JSystem/JAudio/JASystem/JASChannel.hpp>
#include <dolphin/types.h>

namespace JASystem {

/**
 * @brief Manages a group of related logical channels. Provides shared mixing
 * and filtering configs for the managed channels.
 */
class TChannelMgr {
public:
	void init();
	void stopAll();
	void stopAllRelease();
	BOOL checkGlobalRelease();
	TChannel* getLogicalChannel(u32 polyphony_key);
	void initAllocChannel(u32 capacity);
	TChannel* getListHead(u32 list_type);
	u32 countList(u32) const;
	int cutList(TChannel* channel);
	void addListHead(TChannel* channel, u32 list_type);
	void addListTail(TChannel* channel, u32 list_type);
	int receiveAllChannels(TChannelMgr* other);
	BOOL checkLimitStart(u32 polyphony_key);
	void checkLimitStop(TChannel* channel, u32 polyphony_key);

	enum {
		CHAN_LIST_FREE,
		CHAN_LIST_PLAYING,
		CHAN_LIST_RELEASE,
		CHAN_LIST_PENDING,
	};

public:
	/* 0x0 */ u32 mManagedChannels;
	/* 0x4 */ u32 mGlobalChannelBorrows;
	/* 0x8 */ TChannel* mFreeChannels;
	/* 0xC */ TChannel* mPlayingChannels;
	/* 0x10 */ TChannel* mReleaseChannels;
	/* 0x14 */ TChannel* mPendingChannels;
	/* 0x18 */ f32 mVolume;
	/* 0x1C */ f32 mPitch;
	/* 0x20 */ f32 mPan;
	/* 0x24 */ f32 mFxmix;
	/* 0x28 */ f32 mDolby;
	/* 0x2C */ s16 mFirCoeffs[8];
	/* 0x3C */ s16 mIirCoeffs[4];
	char unk44[0x8];
	/* 0x4C */ s16 mDistFilter;
	/* 0x4E */ u16 mMixConfig[6];
	/* 0x5A */ u8 mMixerDelaySamples[6];
	/* 0x60 */ u8 mMixerDelayMax;
	/* 0x61 */ u8 mFilterMode;
	/* 0x62 */ u8 mCalcTypes[3];
	/* 0x68 */ u32 mPriorityConfig;
	/* 0x6C */ u16 mPriorityTime;
	/* 0x70 */ BOOL mMaintainPoolSize;
};

} // namespace JASystem

#endif // JASCHANNELMGR_HPP
