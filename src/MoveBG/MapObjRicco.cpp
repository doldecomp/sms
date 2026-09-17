#include <MoveBG/MapObjRicco.hpp>

u32 TCraneRotY::mWaitTime = 120;

f32 TCraneUpDown::mRotSpeed = 0.1f;
u32 TCraneUpDown::mWaitTime = 120;

f32 TRiccoWatermill::mRotAccel              = 1.0f;
f32 TRiccoWatermill::mRotSpeedMaxUp         = 3.0f;
f32 TRiccoWatermill::mRotSpeedMaxDown       = 1.0f;
f32 TRiccoWatermill::mRotDown               = 0.05f;
f32 TRiccoWatermill::mSubmarineMoveRate     = 0.5f;
f32 TRiccoWatermill::mSubmarineMaxTransY    = 750.0f;
f32 TRiccoWatermill::mSubmarineBottomTransY = -950.0f;
u32 TRiccoWatermill::mWaitTime              = 600;

f32 TFruitLauncher::mObjSpeedXZ    = 1.0f;
f32 TFruitLauncher::mObjSpeedY     = 20.0f;
u32 TFruitLauncher::mFruitLiveTime = 4800;
