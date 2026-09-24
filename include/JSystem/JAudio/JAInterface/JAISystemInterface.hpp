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
		/* 0x4 */ PTR32(s16) mArgsAsPS16[10];
		/* 0x4 */ u32 mArgsAsU32[10];
	};
	/* 0x2C */ JASystem::Kernel::TPortCmd mCmd;
};

struct JAISeqUpdateData {
	/* 0x0 */ u8 mPauseMode;
	/* 0x1 */ u8 mPauseVolume;
	/* 0x2 */ bool mPrepareFlag;
	/* 0x3 */ bool mLoadingFlag;
	/* 0x4 */ u32 mTrackInitFlags;
	/* 0x8 */ u32 unk8;
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
	/* 0x44 */ u32* mTrackUpdate;
	/* 0x48 */ JAISound* mSound;
	/* 0x4C */ JAIPlayerParameter* mPlayerParams;
};

struct JAISeqParameter;

namespace JAISystemInterface {

BOOL startSeq(u32);
BOOL stopSeq(s32);
BOOL checkSeqActiveFlag(u32);
int setSeqData(JASystem::TTrack*, u8*, s32, JASystem::Player::SEQ_PLAYMODE);

JAISeqParameter* rootInit(JAISeqUpdateData*);
void trackInit(JAISeqUpdateData*);
void outerInit(JAISeqUpdateData*, void*, u32, u16, u8);
JASystem::TTrack* trackToSeqp(JAISound*, u8);

void setSeqPortargsF32(JAISeqUpdateData* sud, u32 track_no, u8 arg_no,
                       f32 value);
void setSeqPortargsU32(JAISeqUpdateData* sud, u32 track_no, u8 arg_no,
                       u32 value);
void setSeqPortargsPS16(JAISeqUpdateData* sud, u32 track_no, u8 arg_no,
                        s16* value);
void setPortParameter(JASystem::Kernel::TPortArgs*, JASystem::TTrack*, u32,
                      u32);
void setSePortParameter(JASystem::Kernel::TPortArgs*);
void setAudioFrameParameter(JASystem::Kernel::TPortArgs*);
BOOL writePortApp(u32, u32, u16);
BOOL readPortApp(u32, u32, u16*);

BOOL checkFileExsistence(char*);
void* JAIouterP(void*);
void* JAIouterSW(void*);

extern JASystem::Kernel::TPortCmd systemPortCmd;

} // namespace JAISystemInterface

#endif // JAISYSTEMINTERFACE_HPP
