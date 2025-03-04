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
	MSRandPlayVec();
	~MSRandPlayVec();
};

class MSRandPlay {
public:
	MSRandPlay(u32, s32, s32, f32, f32);
	void construct(u32, s32, s32, f32, f32);
	void createRandPlayVec(u32, u16);
	void createRandPlayVecDynamic(u16);
	void randPlay(u32);
	void registerTrans(u32, const Vec*);
	void registerTransDynamic(const Vec*);
	void startSeRandPlay(u32, u32);
	static JSUList<MSRandPlay> smList;
};

class SeInfo {
public:
	static void* smSeSetting;
};

class MSoundSE {
public:
	MSoundSE();
	void construct();
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
