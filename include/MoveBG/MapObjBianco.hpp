#ifndef MOVE_BG_MAP_OBJ_BIANCO_HPP
#define MOVE_BG_MAP_OBJ_BIANCO_HPP

#include <MoveBG/MapObjBase.hpp>

// TODO: mark virtual methods as such

// fabricated: declaration recovered from mario.MAP statics, layout unknown
class TMapObjRootPakkun : public TMapObjBase {
public:
	static f32 mTremblePower;
	static f32 mTrembleAccel;
	static f32 mTrembleBrake;
	static u32 mTrembleTime;
};

// fabricated: declaration recovered from mario.MAP statics, layout unknown
class TBiancoWatermillVertical : public TMapObjBase {
public:
	static f32 mRotAccel;
	static f32 mRotSpeedDownRate;
	static f32 mRotSpeedMax;
	static f32 mBridgeRotRate;
};

// fabricated: declaration recovered from mario.MAP statics, layout unknown
class TBiancoMiniWindmill : public TMapObjBase {
public:
	static f32 mRotWaterAccel;
	static f32 mFriction;
	static f32 mRotSpeedMax;
};

// fabricated: declaration recovered from mario.MAP statics, layout unknown
class TLeafBoatRotten : public TMapObjBase {
public:
	static f32 mAlphaDownSpeed;
	static f32 mCollisionRemoveAlpha;
	static s16 mRottenColor[4];
};

#endif
