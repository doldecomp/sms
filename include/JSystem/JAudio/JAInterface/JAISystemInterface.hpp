#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASTrack.hpp>
#include <JSystem/JAudio/JASystem/JASCmdStack.hpp>

class JAISound;
class JAISeqUpdateData;

/**
 * JAISystemInterface - Interface between JAI and JASystem
 *
 * This class provides an interface layer between the high-level JAI audio
 * system and the lower-level JASystem implementation.
 */
class JAISystemInterface {
public:
	// Sequence control methods
	static void startSeq(u32);
	static void stopSeq(s32);
	static bool checkSeqActiveFlag(u32);
	static void setSeqData(JASystem::TTrack*, u8*, s32,
	                       JASystem::Player::SEQ_PLAYMODE);

	// Track initialization and management
	static void rootInit(JAISeqUpdateData*);
	static void trackInit(JAISeqUpdateData*);
	static void outerInit(JAISeqUpdateData*, void*, u32, u16, u8);
	static JASystem::TTrack* trackToSeqp(JAISound*, u8);

	// Port parameter methods
	static void setSeqPortargsF32(JAISeqUpdateData*, u32, u8, f32);
	static void setSeqPortargsU32(JAISeqUpdateData*, u32, u8, u32);
	static void setSeqPortargsPS16(JAISeqUpdateData*, u32, u8, s16*);
	static void setPortParameter(JASystem::Kernel::TPortArgs*,
	                             JASystem::TTrack*, u32, u32);
	static void setSePortParameter(JASystem::Kernel::TPortArgs*);
	static void setAudioFrameParameter(JASystem::Kernel::TPortArgs*);

	// Port I/O methods
	static void writePortApp(u32, u32, u16);
	static void readPortApp(u32, u32, u16*);

	// Utility methods
	static bool checkFileExsistence(char*);
	static void* JAIouterP(void*);
	static void* JAIouterSW(void*);

	// System port command
	static JASystem::Kernel::TPortCmd systemPortCmd;
};

#endif // JAISYSTEMINTERFACE_HPP
