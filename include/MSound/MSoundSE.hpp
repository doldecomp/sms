#ifndef MSOUND_SE_HPP
#define MSOUND_SE_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <JSystem/JSupport/JSUList.hpp>

#include <MSound/MSSetSound.hpp>

class JAIActor;

namespace MSoundSESystem {

class MSRandPlayVec {
public:
	MSRandPlayVec()
	    : unk0(0)
	    , unk4(0)
	    , unk8(0)
	    , unkC(0)
	    , unk10(this)
	    , unk20(0)
	{
	}
	~MSRandPlayVec() { }

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ u8 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32 unkC;
	/* 0x10 */ JSULink<MSRandPlayVec> unk10;
	/* 0x20 */ u32 unk20;
};

class MSRandPlay {
public:
	/* 0x0 */ JSULink<MSRandPlay> unk0;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ u16 unk14;
	/* 0x16 */ u16 unk16;

public:
	MSRandPlay(u32 param_1, s32 param_2, s32 param_3, f32 param_4, f32 param_5);

	/* 0x18 */ // vt
	virtual void randPlay(u32);

	static void construct(u32, s32, s32, f32, f32);
	static void createRandPlayVec(u32, u16);
	void createRandPlayVecDynamic(u16);
	static void registerTrans(u32, const Vec*);
	void registerTransDynamic(const Vec*);
	void startSeRandPlay(u32, u32);
	static JSUList<MSRandPlay> smList;

public:
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ s32 unk20;
	/* 0x24 */ s32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
};

class SeInfo {
public:
	static void* smSeSetting;
};

class MSoundSE {
public:
	MSoundSE();
	static void construct();
	static bool checkMonoSound(u32, JAIActor*);
	static void checkSoundArea(u32, const Vec&);
	static u32 getNewIDByGroundCode(u32, JAIActor*);
	static u32 getNewIDBySurfaceCode(u32, JAIActor*);
	static u32 getRandomID(u32);
	static void startSoundActor(u32, const Vec*, u32, JAISound**, u32, u8);
	static void startSoundActorInner(u32, JAISound**, JAIActor*, u32, u8);
	static void startSoundActorWithInfo(u32, const Vec*, Vec*, f32, u32, u32,
	                                    JAISound**, u32, u8);
	static void startSoundNpcActor(u32, const Vec*, u32, JAISound**, u32, u8);
	static void startSoundSystemSE(u32, u32, JAISound**, u32);
	static MSoundSE* mObj;
};

} // namespace MSoundSESystem

#endif
