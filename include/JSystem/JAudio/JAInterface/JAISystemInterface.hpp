#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

class JAISound;

struct JAISeqUpdateData {
	/* 0x0 */ u8 unk0;
	/* 0x0 */ u8 unk1;
	/* 0x0 */ u8 unk2;
	/* 0x0 */ u8 unk3;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	// The whole sequence. `outerInit` sends these six for the root slot.
	/* 0xC */ f32 mSeqVolume;
	/* 0x10 */ f32 mSeqPitch;
	/* 0x14 */ f32 mSeqFxmix;
	/* 0x18 */ f32 mSeqPan;
	/* 0x1C */ f32 mSeqDolby;
	/* 0x20 */ f32 mSeqTempo;
	// One entry per track. These hold the value last sent to each track, so
	// `checkPlayingSeqTrack` sends only what changed.
	/* 0x24 */ f32* mTrackVolume;
	/* 0x28 */ f32* mTrackPitch;
	/* 0x2C */ f32* mTrackFxmix;
	/* 0x30 */ f32* mTrackPan;
	/* 0x34 */ f32* mTrackDolby;
	/* 0x38 */ char unk38[0x8];
	// The sequence data in the load area, given to `setSeqData`.
	/* 0x40 */ u8* mSeqData;
	// One bit set per track that has an update to send.
	/* 0x44 */ u32* mTrackUpdate;
	/* 0x48 */ JAISound* mSound;

	struct FabricatedUnk4CStruct {
		/* 0x0 */ JASystem::TTrack* unk0;
		// TODO: fabricated union; setSeqPortargsF32/U32 write the port
		// arguments by index, so the original must have had an overlay
		// like this one.
		union {
			/* 0x4 */ JASystem::Kernel::TPortArgs unk4;
			/* 0x4 */ f32 unk4F32[10];
			/* 0x4 */ u32 unk4U32[10];
		};
		/* 0x2C */ JASystem::Kernel::TPortCmd unk2C;
	};
	/* 0x4C */ FabricatedUnk4CStruct* unk4C;
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

void setSeqPortargsF32(JAISeqUpdateData*, u32, u8, f32);
void setSeqPortargsU32(JAISeqUpdateData*, u32, u8, u32);
void setSeqPortargsPS16(JAISeqUpdateData*, u32, u8, s16*);
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
