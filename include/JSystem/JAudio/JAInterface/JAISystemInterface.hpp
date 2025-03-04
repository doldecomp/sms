#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

class JAISound;
class JAISeqUpdateData;

class JAISystemInterface {
public:
	void startSeq(u32);
	void stopSeq(s32);
	bool checkSeqActiveFlag(u32);
	void setSeqData(JASystem::TTrack*, u8*, s32,
	                JASystem::Player::SEQ_PLAYMODE);

	void rootInit(JAISeqUpdateData*);
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
	void writePortApp(u32, u32, u16);
	void readPortApp(u32, u32, u16*);

	bool checkFileExsistence(char*);
	void* JAIouterP(void*);
	void* JAIouterSW(void*);

	// System port command
	static JASystem::Kernel::TPortCmd systemPortCmd;
};

#endif // JAISYSTEMINTERFACE_HPP
