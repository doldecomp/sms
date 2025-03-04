#ifndef MANMSOUND_HPP
#define MANMSOUND_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIAnimation.hpp>
#include <MSound/MSound.hpp>

class JAIActor;
class JAIAnimeFrameSoundData;

class MAnmSound : public JAIAnimeSound {
public:
	MAnmSound(MSound* sound);

	virtual void startAnimSound(void* ptr, u32 ul, JAISound** sound,
	                            JAIActor* actor, u8 uc);

	void animeLoop(Vec* vec, f32 f1, f32 f2, u32 ul, u8 uc);
	void initAnmSound(void* ptr, u32 ul, f32 f);
	void setSpeedModifySound(JAISound* sound, JAIAnimeFrameSoundData* data,
	                         f32 f);
};

class MAnmSoundNPC : public MAnmSound {
public:
	MAnmSoundNPC(MSound* sound);

	virtual void startAnimSound(void* ptr, u32 ul, JAISound** sound,
	                            JAIActor* actor, u8 uc);
};

#endif // MANMSOUND_HPP
