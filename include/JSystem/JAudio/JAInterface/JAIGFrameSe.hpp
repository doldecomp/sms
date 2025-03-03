#ifndef JAIGFRAMSE_HPP
#define JAIGFRAMSE_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIActor.hpp>

class JAIGFrameSe {
public:
	static void checkNextFrameSe();
	static void sendPlayingSeCommand();
	static void setSeqMuteFromSeStart(JAISound* sound);
	static void clearSeqMuteFromSeStop(JAISound* sound);
	static void checkSeMovePara();
	static void sendSeAllParameter(JAISound* sound);
	static void releaseSeRegist(JAISound* sound);
};

#endif // JAIGFRAMSE_HPP
