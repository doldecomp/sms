#ifndef ENEMY_POPO_HPP
#define ENEMY_POPO_HPP

#include <Enemy/WalkerEnemy.hpp>

// fabricated: declaration recovered from mario.MAP statics, layout unknown

class TPopo : public TWalkerEnemy {
public:
	TPopo(const char*); // declared only: suppresses the implicit default constructor
	// static members (map: .sdata)
	static u8 mRollSw;
	static u8 mTriggerSw;
	static f32 mTestAng_x;
	static f32 mTestAng_y;
	static f32 mNozzleOffsetZ;
	static u8 mCenterJntIndex;
	static u8 mMouthJntIndex;
	static u8 mRLegJntIndex;
	static u8 mLLegJntIndex;
	static u8 mRHandJntIndex;
	static u8 mLHandJntIndex;
	static f32 mTestBodyScale;
	static u8 mBrkFlag;
	static f32 mColOffsetY;
	static f32 mColMinVal;
	static u8 mLevelShootSw;
};

#endif
