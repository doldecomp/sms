#ifndef MSOUND_SCENE_HPP
#define MSOUND_SCENE_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

class MSSceneSE {
public:
	MSSceneSE(u32);
	void frameLoop(u32, Vec*, u8);
	void sortMaxTrans(Vec*, u8, u8);
	void calcPosPanSR(Vec*, f32);
	void calcPosPanLR(Vec*, f32);
	void calcPosVolume(Vec*, f32*, u8);

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ Vec* unk4[256];
	/* 0x404 */ Vec unk404[3];
	/* 0x428 */ Vec* unk428[3][3];
};

#endif
