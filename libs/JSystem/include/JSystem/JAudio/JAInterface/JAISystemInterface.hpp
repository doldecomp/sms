#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

class JAISound;

struct JAIPlayerParameter {
	/* 0x0 */ JASystem::TTrack* mTrack;
	union {
		/* 0x4 */ JASystem::Kernel::TPortArgs mArgs;
		/* 0x4 */ f32 mArgsAsF32[10];
		/* 0x4 */ s16* mArgsAsPS16[10];
		/* 0x4 */ u32 mArgsAsU32[10];
	};
	/* 0x2C */ JASystem::Kernel::TPortCmd mCmd;
};

struct JAISeqUpdateData {
	// fabricated
	enum RequestFlag {
		REQUEST_LOAD            = 0x00000001,
		REQUEST_STOP            = 0x00000002,
		REQUEST_TEMPO           = 0x00000004,
		REQUEST_PORT_DATA       = 0x00000010,
		REQUEST_TRACK_MUTE      = 0x00000020,
		REQUEST_TRACK_VOLUME    = 0x00000040,
		REQUEST_TRACK_PAN       = 0x00000080,
		REQUEST_TRACK_DOLBY     = 0x00000100,
		REQUEST_TRACK_PITCH     = 0x00000200,
		REQUEST_TRACK_FXMIX     = 0x00000800,
		REQUEST_TRACK_PORT_DATA = 0x00001000,
		REQUEST_VOLUME          = 0x00040000,
		REQUEST_PAN             = 0x00080000,
		REQUEST_PITCH           = 0x00100000,
		REQUEST_FXMIX           = 0x00200000,
		REQUEST_DOLBY           = 0x00400000,
		REQUEST_TRACK_INTERRUPT = 0x00800000,
	};

	/* 0x0 */ u8 mPauseMode;
	/* 0x1 */ u8 mPauseVolume;
	/* 0x2 */ bool mPrepareFlag;
	/* 0x3 */ bool mLoadingFlag;
	/* 0x4 */ u32 mTrackInitFlags;
	/* 0x8 */ u32 mRequestFlags;
	/* 0xC */ f32 mSeqVolume;
	/* 0x10 */ f32 mSeqPitch;
	/* 0x14 */ f32 mSeqFxmix;
	/* 0x18 */ f32 mSeqPan;
	/* 0x1C */ f32 mSeqDolby;
	/* 0x20 */ f32 mSeqTempo;
	/* 0x24 */ f32* mTrackVolume;
	/* 0x28 */ f32* mTrackPitch;
	/* 0x2C */ f32* mTrackFxmix;
	/* 0x30 */ f32* mTrackPan;
	/* 0x34 */ f32* mTrackDolby;
	/* 0x38 */ char unk38[0x8];
	/* 0x40 */ u8* mSeqData;
	/* 0x44 */ u32* mTrackPendingFlags;
	/* 0x48 */ JAISound* mSound;
	/* 0x4C */ JAIPlayerParameter* mTrackCommands;
};

struct JAISeqParameter;

namespace JAISystemInterface {

BOOL startSeq(u32 seq_handle);
BOOL stopSeq(s32 seq_handle);
BOOL checkSeqActiveFlag(u32 seq_handle);
int setSeqData(JASystem::TTrack* track, u8* seq_data, s32 size,
               JASystem::Player::SEQ_PLAYMODE play_mode);

JAISeqParameter* rootInit(JAISeqUpdateData* update_data);
void trackInit(JAISeqUpdateData* update_data);
void outerInit(JAISeqUpdateData* update_data, void* track_ptr, u32 player_index,
               u16 outer_param_mask, u8 param_5);
JASystem::TTrack* trackToSeqp(JAISound* sound, u8 track_no);

void setSeqPortargsF32(JAISeqUpdateData* update_data, u32 player_index,
                       u8 arg_index, f32 value);
void setSeqPortargsU32(JAISeqUpdateData* update_data, u32 player_index,
                       u8 arg_index, u32 value);
void setSeqPortargsPS16(JAISeqUpdateData* update_data, u32 player_index,
                        u8 arg_index, s16* value);
void setPortParameter(JASystem::Kernel::TPortArgs* args,
                      JASystem::TTrack* track, u32 outer_param_mask,
                      u32 value_index);
void setSePortParameter(JASystem::Kernel::TPortArgs* args);
void setAudioFrameParameter(JASystem::Kernel::TPortArgs*);
BOOL writePortApp(u32 seq_handle, u32 port_route, u16 value);
BOOL readPortApp(u32 seq_handle, u32 port_route, u16* value);

BOOL checkFileExsistence(char* path);
void* JAIouterP(void*);
void* JAIouterSW(void*);

extern JASystem::Kernel::TPortCmd systemPortCmd;

} // namespace JAISystemInterface

#endif // JAISYSTEMINTERFACE_HPP
