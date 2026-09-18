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
	// liveactor.o's.
	//
	// Header round 13 found half of the missing four bytes. The map has no
	// __dt__14JAIAnimeSoundFv anywhere, so nothing in the image calls a base
	// destructor for JAIAnimeSound; that destructor is in-class and trivial
	// (JAIAnimation.hpp used to declare it out of line, which made this body
	// `bl` a symbol that does not exist and cost the flag an extra register).
	// With it inlined we emit 0x5c / 23 instructions: prologue and r31 save,
	// `mr.`/`beq` null test, our vtable store, a bare `beq` for the base
	// subobject, JAIAnimeSound's vtable store, the `extsh. r4` delete branch,
	// epilogue.
	//
	// The map's 0x60 is 24 instructions, i.e. exactly one more. Round 13 read
	// that as the `addic. r0, r31, <off>` that __dt__24TSpineBase<10TLiveActor>Fv
	// (also 0x60) emits for an inlined base subobject at `this + 4`, and
	// looked for a second base or an intermediate class. **Header round 15
	// refutes that**:
	//   - __vt__9MAnmSound is 0x10, exactly the size of __vt__13JAIAnimeSound,
	//     and holds the usual two zero words plus startAnimSound and
	//     setSpeedModifySound. MWCC puts the vtable pointer after the
	//     introducing class's own data (hence the stores at 0x94), and a
	//     second polymorphic base would add a vtable segment, so MAnmSound
	//     adds no virtual of its own and has one polymorphic base at offset 0.
	//     There is no nonzero-offset subobject to find.
	//   - __ct__9MAnmSoundFP6MSound (0x44, byte-exact) calls
	//     __ct__13JAIAnimeSoundFv, stores the vtable at 0x94 and zero at 0x90,
	//     and **never stores its MSound* parameter**, so MAnmSound has no data
	//     members and the destructor body has nothing to destroy.
	// The one extra instruction is therefore a *fresh* null test before the
	// base vptr store instead of a reuse of the entry `mr.`'s cr0. Giving the
	// body `stop()` does produce exactly that tell (`cmplwi r30, 0` after the
	// `bl` clobbers cr0) but costs 29 instructions, and any cr0 clobber costs
	// an instruction of its own, so a one-instruction delta cannot come from
	// that route either. Open, with no candidate left in the map.
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
