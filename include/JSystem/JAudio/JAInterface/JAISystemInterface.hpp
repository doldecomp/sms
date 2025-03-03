#ifndef JAISYSTEMINTERFACE_HPP
#define JAISYSTEMINTERFACE_HPP

#include <JSystem/JAudio/JASystem/JASystem.hpp>

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
	static void startSeq(unsigned long);
	static void stopSeq(long);
	static bool checkSeqActiveFlag(unsigned long);
	static void setSeqData(JASystem::TTrack*, unsigned char*, long,
	                       JASystem::Player::SEQ_PLAYMODE);

	// Track initialization and management
	static void rootInit(JAISeqUpdateData*);
	static void trackInit(JAISeqUpdateData*);
	static void outerInit(JAISeqUpdateData*, void*, unsigned long,
	                      unsigned short, unsigned char);
	static JASystem::TTrack* trackToSeqp(JAISound*, unsigned char);

	// Port parameter methods
	static void setSeqPortargsF32(JAISeqUpdateData*, unsigned long,
	                              unsigned char, float);
	static void setSeqPortargsU32(JAISeqUpdateData*, unsigned long,
	                              unsigned char, unsigned long);
	static void setSeqPortargsPS16(JAISeqUpdateData*, unsigned long,
	                               unsigned char, short*);
	static void setPortParameter(JASystem::Kernel::TPortArgs*,
	                             JASystem::TTrack*, unsigned long,
	                             unsigned long);
	static void setSePortParameter(JASystem::Kernel::TPortArgs*);
	static void setAudioFrameParameter(JASystem::Kernel::TPortArgs*);

	// Port I/O methods
	static void writePortApp(unsigned long, unsigned long, unsigned short);
	static void readPortApp(unsigned long, unsigned long, unsigned short*);

	// Utility methods
	static bool checkFileExsistence(char*);
	static void* JAIouterP(void*);
	static void* JAIouterSW(void*);

	// System port command
	static JASystem::Kernel::TPortCmd systemPortCmd;
};

#endif // JAISYSTEMINTERFACE_HPP
