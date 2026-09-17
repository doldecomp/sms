#include <MoveBG/MapObjBianco.hpp>

f32 TMapObjRootPakkun::mTremblePower = 15.0f;
f32 TMapObjRootPakkun::mTrembleAccel = 0.95f;
f32 TMapObjRootPakkun::mTrembleBrake = 0.98f;
u32 TMapObjRootPakkun::mTrembleTime  = 360;

f32 TBiancoWatermillVertical::mRotAccel         = 0.15f;
f32 TBiancoWatermillVertical::mRotSpeedDownRate = 0.005f;
f32 TBiancoWatermillVertical::mRotSpeedMax      = 3.0f;
f32 TBiancoWatermillVertical::mBridgeRotRate    = 0.03f;

f32 TBiancoMiniWindmill::mRotWaterAccel = 0.01f;
f32 TBiancoMiniWindmill::mFriction      = 0.01f;
f32 TBiancoMiniWindmill::mRotSpeedMax   = 10.0f;

f32 TLeafBoatRotten::mAlphaDownSpeed       = 0.5f;
f32 TLeafBoatRotten::mCollisionRemoveAlpha = 100.0f;
s16 TLeafBoatRotten::mRottenColor[4]       = { 100, 100, 180, 255 };
