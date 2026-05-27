#ifndef MSOUNDBGM_HPP
#define MSOUNDBGM_HPP

#include <MSound/MSSceneWave.hpp>
#include <JSystem/JAudio/JALibrary/JALList.hpp>
#include <JSystem/JAudio/JASystem/JASTrack.hpp>

class JAISound;

class MSBgm : public JALListHioNode<MSBgm, u32> {
public:
	MSBgm(int i)
	    : JALListHioNode<MSBgm, u32>("", i, this)
	    , unk14(nullptr)
	{
	}

	static MSBgm* smBgmInTrack[3];
	static f32 smMainVolume;
	static void init();
	static JAISound* startBGM(u32);
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
	static MS_SCENE_WAVE getSceneNo(u32);
	static JAISound* getHandle(u8);
	static JASystem::TTrack* getJASTrack(JAISound*, u8);

	// fabricated
	static void setAllTracksVolume(f32 param_1, u32 param_2)
	{
		for (u8 i = 0; i < 3; ++i)
			if (0x7 >> i & 1)
				MSBgm::setTrackVolume(i, param_1, param_2, 3);
	}

public:
	/* 0x14 */ JAISound* unk14;
};

// Exact order and names were pulled from mSound.asn
enum MSoundBGMId {
	MSD_BGM_DOLPIC          = 0x80010800,
	MSD_BGM_BIANCO          = 0x80010001,
	MSD_BGM_MAMMA           = 0x80010002,
	MSD_BGM_PINNAPACO_SEA   = 0x80010003,
	MSD_BGM_PINNAPACO       = 0x80010004,
	MSD_BGM_MARE_SEA        = 0x80010005,
	MSD_BGM_MONTEVILLAGE    = 0x80010006,
	MSD_BGM_SHILENA         = 0x80010007,
	MSD_BGM_RICCO           = 0x80010008,
	MSD_BGM_GET_SHINE       = 0x80010009,
	MSD_BGM_CHUBOSS         = 0x8001000A,
	MSD_BGM_MISS            = 0x8001000B,
	MSD_BGM_BOSS            = 0x8001000C,
	MSD_BGM_MAP_SELECT      = 0x8001000D,
	MSD_BGM_BOSSPAKU_DEMO   = 0x8001000E,
	MSD_BGM_MAIN_TITLE      = 0x8001000F,
	MSD_BGM_CHUBOSS2        = 0x80010010,
	MSD_BGM_EXTRA           = 0x80010011,
	MSD_BGM_DELFINO         = 0x80010012,
	MSD_BGM_MAREVILLAGE     = 0x80010013,
	MSD_BGM_CORONA          = 0x80010014,
	MSD_BGM_KAGEMARIO       = 0x80010015,
	MSD_BGM_CAMERA          = 0x80010016,
	MSD_BGM_MONTE_ONSEN     = 0x80010017,
	MSD_BGM_MECHAKUPPA      = 0x80010018,
	MSD_BGM_AIRPORT         = 0x80010019,
	MSD_BGM_UNDERGROUND     = 0x8001001A,
	MSD_BGM_TITLEBACK       = 0x8001001B,
	MSD_BGM_MONTE_NIGHT     = 0x8001001C,
	MSD_BGM_CASINO          = 0x8001001D,
	MSD_BGM_EVENT           = 0x8001001E,
	MSD_BGM_TIME_IVENT      = 0x8001001F,
	MSD_BGM_SKY_AND_SEA     = 0x80010020,
	MSD_BGM_MONTE_RESCUE    = 0x80010021,
	MSD_BGM_MERRY_GO_ROUND  = 0x80010022,
	MSD_BGM_SCENARIO_SELECT = 0x80010023,
	MSD_BGM_FANFARE_CASINO  = 0x80010024,
	MSD_BGM_FANFARE_RACE    = 0x80010025,
	MSD_BGM_CAMERA_KAGE     = 0x80010026,
	MSD_BGM_GAMEOVER        = 0x80010027,
	MSD_BGM_BOSSHANA_2ND3RD = 0x80010028,
	MSD_BGM_BOSSGESO_2DN3RD = 0x80010029,
	MSD_BGM_CHUBOSS_MANTA   = 0x8001002A,
	MSD_BGM_MONTE_LAST      = 0x8001002B,
	MSD_BGM_SHINE_APPEAR    = 0x8001002C,
	MSD_BGM_KUPPA           = 0x8001002D,
	MSD_BGM_MONTEMAN_RACE   = 0x8001002E,
	MSD_STR_SPACEWORLD      = 0x8001002F
};

#endif
