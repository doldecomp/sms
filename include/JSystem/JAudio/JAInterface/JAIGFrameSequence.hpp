#ifndef JAIGFRAMESEQUENCE_HPP
#define JAIGFRAMESEQUENCE_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>

class JAIGFrameSequence {
public:
	static void checkEntriedSeq();
	static void checkPlayingSeqTrack(unsigned long trackID);
	static void checkPlayingSeq();
	static void checkStoppedSeq();
	static void checkStartedSeq();
	static void checkFadeoutSeq();
	static void checkReadSeq();
	static void checkSeqWave();
	static void checkDvdLoadArc(unsigned long arcID);
	static void stopSeq(JAISound* sound);
};

#endif // JAIGFRAMESEQUENCE_HPP
