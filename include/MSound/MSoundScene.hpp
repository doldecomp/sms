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
};

#endif
