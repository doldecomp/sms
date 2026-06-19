#ifndef JAIANIMATION_HPP
#define JAIANIMATION_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

class JAIActor;

struct JAIAnimeFrameSoundData {
	/* 0x00 */ u32 unk0;
	/* 0x04 */ f32 unk4;
	/* 0x08 */ f32 unk8;
	/* 0x0C */ f32 unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u8 unk14;
	/* 0x15 */ s8 unk15;
	/* 0x16 */ u8 unk16;
	/* 0x17 */ u8 unk17;
	/* 0x18 */ s8 unk18;
	/* 0x19 */ u8 unk19[7];
};

// Fabricated
// Basically, contents of a .bas sound (binary animated sound?)
struct JAIAnimeSoundData {
	/* 0x00 */ u16 mCount;
	/* 0x02 */ u8 unk2[6];
	/* 0x08 */ JAIAnimeFrameSoundData mEntries[];
};

class JAIAnimeSound {
public:
	struct Slot {
		/* 0x0 */ u8 mIsPlaying;
		/* 0x4 */ JAISound* mSound;
		/* 0x8 */ JAIAnimeFrameSoundData* mData;
	};

	/* 0x00 */ Slot mSlots[8];
	/* 0x60 */ u32 unk60;
	/* 0x64 */ u32 unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ u32 unk6C;
	/* 0x70 */ void* unk70[2];
	/* 0x78 */ u32 mDataCounterInc;
	/* 0x7C */ u32 mDataCounterLimit;
	/* 0x80 */ u32 mDataCounter;
	/* 0x84 */ u32 mLoopCount;
	/* 0x88 */ f32 mCurrentTime;
	/* 0x8C */ u8 unk8C[0x4];

	/* 0x90 */ JAIAnimeSoundData* mData;
	/* 0x94 */ // vtable

public:
	JAIAnimeSound();
	~JAIAnimeSound();

	virtual void startAnimSound(void* data, u32 id, JAISound** sound,
	                            JAIActor* actor, u8 flag);
	virtual void setSpeedModifySound(JAISound* sound,
	                                 JAIAnimeFrameSoundData* data, f32 speed);

	void setAnimSound(JAIBasic* basic, f32 param1, f32 param2, u8 param3);
	void setAnimSoundVec(JAIBasic* basic, Vec* pos, f32 param1, f32 param2,
	                     u32 param3, u8 param4);
	void setAnimSoundActor(JAIBasic* basic, JAIActor* actor, f32 param1,
	                       f32 param2, u8 param3);
	void playActorAnimSound(JAIBasic* basic, JAIActor* actor, f32 param,
	                        u8 flag);
	void initActorAnimSound(void* data, u32 param, f32 value);
	void initActorAnimSound(void* data, JAIActor actor, u32 param, f32 value);
	void stop();
};

#endif // JAIANIMATION_HPP
