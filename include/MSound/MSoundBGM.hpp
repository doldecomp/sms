#ifndef MSOUNDBGM_HPP
#define MSOUNDBGM_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

template <typename T> class JALList;

template <typename T, typename U> class JALListS;

template <typename T, typename U> class JALListVirtualNode;

class MSBgm {
public:
	static JASystem::TTrack* smBgmInTrack[];
	static f32 smMainVolume;

	void init();
	void startBGM(u32);
	void stopBGM(u32, u32);
	void stopTrackBGM(u8, u32);
	void stopTrackBGMs(u8, u32);
	void setVolume(u32, f32, u32, u8);
	void setTrackVolume(u8, f32, u32, u8);
	void setSeqTrackVolume(u8, u8, f32, u32);
	void setPan(u8, f32, u32, u8);
	void setDolby(u8, f32, u32, u8);
	void setPause(u8, bool);
	void setSeqTrackMute(u8, u8, bool);
	void setSeqTRACKsMute(u8, bool, u16);
	void setSeqTRACKsMuteH(JAISound*, bool, u16);
	void setStageBgmYoshiPercussion(bool);
	bool checkPlaying(u32);
	u32 getSceneNo(u32);
	JAISound* getHandle(u8);
	JAISound* getJASTrack(JAISound*, u8);
};

#endif
