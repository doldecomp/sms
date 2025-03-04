#ifndef MSOUND_SCENE_HPP
#define MSOUND_SCENE_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

class MSSceneSE {
public:
	MSSceneSE(unsigned long);
	void frameLoop(unsigned long, Vec*, unsigned char);
	void sortMaxTrans(Vec*, unsigned char, unsigned char);
	void calcPosPanSR(Vec*, float);
	void calcPosPanLR(Vec*, float);
	void calcPosVolume(Vec*, float*, unsigned char);
};

#endif
