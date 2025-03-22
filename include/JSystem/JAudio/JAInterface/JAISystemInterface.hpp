#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

class JAISound;
class JAISeqUpdateData {
public:
	/* 0x0 */ u8 unk0;
	/* 0x0 */ u8 unk1;
	/* 0x0 */ u8 unk2;
	/* 0x0 */ u8 unk3;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32* unk24;
	/* 0x28 */ f32* unk28;
	/* 0x2C */ f32* unk2C;
	/* 0x30 */ f32* unk30;
	/* 0x34 */ f32* unk34;
	/* 0x38 */ char unk38[0xC];
	/* 0x44 */ u32* unk44;
	/* 0x48 */ JAISound* unk48;
	/* 0x4C */ void* unk4C;
};

namespace JAISystemInterface {

BOOL startSeq(u32);
BOOL stopSeq(s32);
BOOL checkSeqActiveFlag(u32);
int setSeqData(JASystem::TTrack*, u8*, s32, JASystem::Player::SEQ_PLAYMODE);

void* rootInit(JAISeqUpdateData*);
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

bool checkFileExsistence(char*);
void* JAIouterP(void*);
void* JAIouterSW(void*);

extern JASystem::Kernel::TPortCmd systemPortCmd;

} // namespace JAISystemInterface

#endif // JAISYSTEMINTERFACE_HPP
