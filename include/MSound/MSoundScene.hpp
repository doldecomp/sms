#ifndef MSOUND_SCENE_HPP
#define MSOUND_SCENE_HPP

#include <dolphin/types.h>
#include <dolphin/mtx.h>

class MSSceneSE {
public:
	MSSceneSE(u32);
	void frameLoop(u32 sound_id, Vec* trans, u8 trans_num);
	void sortMaxTrans(Vec* trans, u8 direction, u8 rank);
	void calcPosPanSR(Vec*, f32);
	void calcPosPanLR(Vec*, f32);
	void calcPosVolume(Vec*, f32*, u8);

public:
	enum {
		DIRECTION_L   = 0,
		DIRECTION_R   = 1,
		DIRECTION_SR  = 2,
		DIRECTION_NUM = 3
	};

	enum { MAX_TRANS_NUM = 3 };

	/* 0x0 */ u8 mUseRandPlay;
	/* 0x4 */ Vec* mTrans[256];
	/* 0x404 */ Vec mAvgTrans[DIRECTION_NUM];
	/* 0x428 */ Vec* mMaxTrans[DIRECTION_NUM][MAX_TRANS_NUM];
};

#endif
