#ifndef MSSETSOUNDTL_HPP
#define MSSETSOUNDTL_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

class MSSetSoundMember;
class MSSetSoundGrp;

// TODO: one more parent
template <typename T> class MSSetSoundTL {
public:
	MSSetSoundTL(u32, const char*, T*, u8, u8, u8, u8, f32, u8, f32, f32, f32,
	             f32, f32, s32, f32, s32, f32, f32, f32, bool);
	~MSSetSoundTL();

	virtual void frameLoopDyna();

	void startSoundSetDyna(u32 param1, const Vec* param2, u32 param3,
	                       f32 param4, u32 param5, u32 param6, u8 param7,
	                       MSSetSoundGrp* param8);
};

class MSSetSoundGrp : public MSSetSoundTL<MSSetSoundGrp> {
public:
	MSSetSoundGrp();

	void startSoundSetGrp(u32 param1, const Vec* param2, u32 param3, f32 param4,
	                      u32 param5, u32 param6, u8 param7);
};

class MSSetSound : public MSSetSoundTL<MSSetSound> {
public:
	MSSetSound();

	static MSSetSound* smSetSound;
	void init();
	void startSoundSet(u32 param1, const Vec* param2, u32 param3, f32 param4,
	                   u32 param5, u32 param6, u8 param7);
};

#endif // MSSETSOUNDTL_HPP
