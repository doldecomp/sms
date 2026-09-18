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
	// What emits it is constructing a derived MAnmSound inline: MWCC
	// synthesises the derived class's implicit destructor, which calls this
	// one out of line. TEnemyMario::initValues' `new MAnmSoundMario` is
	// enemyMario.cpp's copy and TLiveActor's inlined MAnmSoundNPC is
	// liveactor.o's. Both of ours come out at 0x64 against the map's 0x60,
	// so the body is still four bytes off somewhere in the base chain.
	~MAnmSound() { }

	virtual void startAnimSound(void* interface, u32 id,
	                            JAISoundHandle* out_handle, JAIActor* actor,
	                            u8 camera_idx);

	void animeLoop(Vec* position, f32 frame, f32 speed, u32 ground_no, u8);
	void initAnmSound(void* interface, u32 param_2, f32 frame);
	void setSpeedModifySound(JAISound* sound,
	                         JAIAnimeFrameSoundData* frame_data, f32 speed);
};

// The Marios' anime sound: TEnemyMario::initValues stores this vtable at 0x94
// (the ROM's `lis/addi MAnmSoundMario::__vtable` there is what identified the
// class), and its startAnimSound is the one that carries the sound-category
// switch -- MAnmSound's own is a bare gateCheck plus startSoundActorInner.
// The constructor has no symbol anywhere, so it is this in-class forwarder.
class MAnmSoundMario : public MAnmSound {
public:
	MAnmSoundMario(MSound* sound)
	    : MAnmSound(sound)
	{
	}

	virtual void startAnimSound(void* interface, u32 sound_id,
	                            JAISound** out_handle, JAIActor* actor,
	                            u8 camera_idx);
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
