#ifndef JAIANIMATION_HPP
#define JAIANIMATION_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JAudio/JAInterface/JAIBasic.hpp>

class JAIActor;

struct JAIAnimeFrameSoundData {
	/* 0x00 */ u32 mSoundID;
	/* 0x04 */ f32 mStartFrame;
	/* 0x08 */ f32 mEndFrame;
	/* 0x0C */ f32 mPitch;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u8 mVolume;
	/* 0x15 */ s8 mPitchScale;
	/* 0x16 */ u8 unk16;
	/* 0x17 */ u8 mPan;
	/* 0x18 */ s8 mVolumeScale;
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
		/* 0x4 */ JAISoundHandle mSoundHandle;
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

	virtual void startAnimSound(void* interface, u32 id,
	                            JAISoundHandle* out_handle, JAIActor* actor,
	                            u8 flag);
	virtual void setSpeedModifySound(JAISound* sound,
	                                 JAIAnimeFrameSoundData* data, f32 speed);

	void setAnimSound(JAIBasic*, f32, f32, u8);
	void setAnimSoundVec(JAIBasic* interface, Vec* position, f32 frame,
	                     f32 speed, u32 ground_no, u8 param4);
	void setAnimSoundActor(JAIBasic* interface, JAIActor* actor, f32 frame,
	                       f32 param2, u8 param3);
	void playActorAnimSound(JAIBasic* interface, JAIActor* actor, f32 param,
	                        u8 flag);
	void initActorAnimSound(void* interface, u32 param, f32 frame);
	void initActorAnimSound(void*, JAIActor, u32, f32);
	void stop();
};

#endif // JAIANIMATION_HPP
