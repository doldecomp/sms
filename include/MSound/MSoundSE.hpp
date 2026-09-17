#ifndef MSOUND_SE_HPP
#define MSOUND_SE_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JSupport/JSUList.hpp>

class JAIActor;

namespace MSoundSESystem {

class MSRandPlayVec {
public:
	MSRandPlayVec()
	    : mTrans(0)
	    , mState(0)
	    , mWaitTime(0)
	    , mWaitTimer(0)
	    , unk10(this)
	    , mSound(nullptr)
	{
	}
	~MSRandPlayVec() { }

public:
	enum {
		STATE_CALC_WAIT = 0,
		STATE_WAIT      = 1,
		STATE_START     = 2,
		STATE_PLAYING   = 3
	};

	/* 0x0 */ const Vec* mTrans;
	/* 0x4 */ u8 mState;
	/* 0x8 */ s32 mWaitTime;
	/* 0xC */ s32 mWaitTimer;
	/* 0x10 */ JSULink<MSRandPlayVec> unk10;
	/* 0x20 */ JAISoundHandle mSound;
};

class MSRandPlay {
public:
	/* 0x0 */ JSULink<MSRandPlay> mLink;
	/* 0x10 */ MSRandPlayVec* mRandPlayVecs;
	/* 0x14 */ u16 mRandPlayVecMax;
	/* 0x16 */ u16 mRandPlayVecNum;

public:
	MSRandPlay(u32 sound_id, s32 wait_min, s32 wait_max, f32 curve_slope,
	           f32 plus_slope);

	/* 0x18 */ // vt
	virtual void randPlay(u32);

	static void construct(u32 sound_id, s32 wait_min, s32 wait_max,
	                      f32 curve_slope, f32 plus_slope);
	static void createRandPlayVec(u32 sound_id, u16 max_vecs);
	void createRandPlayVecDynamic(u16 max_vecs);
	static int registerTrans(u32 sound_id, const Vec* trans);
	int registerTransDynamic(const Vec* trans);
	static void startSeRandPlay(u32 sound_id, u32);
	static JSUList<MSRandPlay> smList;

public:
	/* 0x1C */ u32 mSoundID;
	/* 0x20 */ s32 mWaitMin;
	/* 0x24 */ s32 mWaitMax;
	/* 0x28 */ f32 mCurveSlope;
	/* 0x2C */ f32 mPlusSlope;
};

namespace SeInfo {
	// NOTE: SeInfo couldn't have been the name for this class, because that way
	// it's size is unknown at the point of smSeSetting declaration and so it
	// couldn not have been emitted into sbss
	// fabricated
	class Setting {
	public:
		Setting(f32 param_1, f32 param_2)
		    : unk0(param_1)
		    , unk4(param_2)
		{
		}

		/* 0x0 */ f32 unk0;
		/* 0x4 */ f32 unk4;
	};

	extern Setting smSeSetting;
} // namespace SeInfo

class MSoundSE {
public:
	MSoundSE();

	static void construct();
	static bool checkMonoSound(u32 id, JAIActor* actor);
	static bool checkSoundArea(u32, const Vec&);
	static u32 getNewIDByGroundCode(u32, JAIActor*);
	static u32 getNewIDBySurfaceCode(u32, JAIActor*);
	static u32 getRandomID(u32);

	static JAISound* startSoundActor(u32 id, const Vec* position, u32 ground_no,
	                                 JAISoundHandle* out_handle, u32 fade,
	                                 u8 camera_idx);
	static JAISound* startSoundActorInner(u32 id, JAISoundHandle* out_handle,
	                                      JAIActor* actor, u32 fade,
	                                      u8 camera_idx);
	static void startSoundActorWithInfo(u32 id, const Vec* position, Vec*, f32,
	                                    u32, u32 ground_no,
	                                    JAISoundHandle* out_handle, u32 fade,
	                                    u8 camera_idx);
	static void startSoundNpcActor(u32 id, const Vec* position, u32 ground_no,
	                               JAISoundHandle* out_handle, u32 fade,
	                               u8 camera_idx);
	static JAISound* startSoundSystemSE(u32 id, u32, JAISoundHandle* out_handle,
	                                    u32 fade);
	static MSoundSE* mObj;
};

} // namespace MSoundSESystem

#endif
