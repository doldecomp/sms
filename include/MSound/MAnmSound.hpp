#ifndef MANMSOUND_HPP
#define MANMSOUND_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include <JSystem/JAudio/JAInterface/JAIConst.hpp>
#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIAnimation.hpp>
#include <MSound/MSound.hpp>

class JAIActor;
class JAIAnimeFrameSoundData;

class MAnmSound : public JAIAnimeSound {
public:
	MAnmSound(MSound* sound);
	// In the class body, and not virtual: the map has __dt__9MAnmSoundFv as
	// an UNUSED 0x60 symbol of `Enemy.a enemyMario.cpp` rather than of
	// MAnmSound.cpp, so it is a header definition every using TU can emit,
	// and __vt__9MAnmSound has no destructor slot (its first two words are
	// the usual zero pair, then startAnimSound).
	// Open: ours lands in liveactor.o -- reached from the inlined
	// MAnmSoundNPC construction -- at 0x64, and enemyMario.o still lacks
	// it, so retail's enemyMario.cpp destroys an MAnmSound somewhere we do
	// not (its UNUSED `kill` is 0x4c against our 0x4 stub).
	~MAnmSound() { }

	virtual void startAnimSound(void* interface, u32 id,
	                            JAISoundHandle* out_handle, JAIActor* actor,
	                            u8 camera_idx);

	void animeLoop(Vec* position, f32 frame, f32 speed, u32 ground_no, u8);
	void initAnmSound(void* interface, u32 param_2, f32 frame);
	void setSpeedModifySound(JAISound* sound,
	                         JAIAnimeFrameSoundData* frame_data, f32 speed);
};

class MAnmSoundNPC : public MAnmSound {
public:
	MAnmSoundNPC(MSound* sound)
	    : MAnmSound(sound)
	    , unk98(JAIConst::random.get_uint8(255))
	{
	}

	virtual void startAnimSound(void* interface, u32 sound_id,
	                            JAISound** out_handle, JAIActor* actor,
	                            u8 camera_idx);

public:
	/* 0x98 */ u8 unk98;
};

#endif // MANMSOUND_HPP
