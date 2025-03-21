#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

class JAISound;
class JAISeqUpdateData {
public:
	/* 0x0 */ char unk0[0x4];
	/* 0x4 */ u32 unk4;
	/* 0x8 */ char unk8[0x40];
	/* 0x48 */ JAISound* unk48;
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
