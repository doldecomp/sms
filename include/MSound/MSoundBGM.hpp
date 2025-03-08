#ifndef MSOUNDBGM_HPP
#define MSOUNDBGM_HPP

#include <dolphin/types.h>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JALibrary/JALList.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

template <typename T, typename U> class JALListVirtualNode;

class MSBgm : public JALListHioNode<MSBgm, u32> {
public:
	MSBgm();

	static JASystem::TTrack* smBgmInTrack[3];
	static f32 smMainVolume;

	static void init();
	static void startBGM(u32);
	static void stopBGM(u32, u32);
	static void stopTrackBGM(u8, u32);
	static void stopTrackBGMs(u8, u32);
	static void setVolume(u32, f32, u32, u8);
	static void setTrackVolume(u8, f32, u32, u8);
	static void setSeqTrackVolume(u8, u8, f32, u32);
	static void setPan(u8, f32, u32, u8);
	static void setDolby(u8, f32, u32, u8);
	static void setPause(u8, bool);
	static void setSeqTrackMute(u8, u8, bool);
	static void setSeqTRACKsMute(u8, bool, u16);
	static void setSeqTRACKsMuteH(JAISound*, bool, u16);
	static void setStageBgmYoshiPercussion(bool);
	static bool checkPlaying(u32);
	static u32 getSceneNo(u32);
	static JAISound* getHandle(u8);
	static JAISound* getJASTrack(JAISound*, u8);
};

#endif
