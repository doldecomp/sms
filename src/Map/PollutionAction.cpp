#include <Map/PollutionLayer.hpp>

// rouge include needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>

f32 TPollutionLayer::mAreaMinRate         = 0.7f;
f32 TPollutionLayer::mSpreadArea          = 2000.0f;
u32 TPollutionLayer::mSpreadFrequency     = 3;
f32 TPollutionLayer::mFireArea            = 1600.0f;
u32 TPollutionLayer::mFireEffectWaitTime  = 20;
f32 TPollutionLayer::mThunderArea         = 1000.0f;
u32 TPollutionLayer::mThunderScaleRate    = 0; // UNUSED
f32 TPollutionLayer::mGlassWallArea       = 1000.0f;
u32 TPollutionLayer::mGlassWallScaleRate  = 0; // UNUSED
u32 TPollutionLayer::mGlassWallEffectTime = 120;

void TPollutionLayer::changeType(u16) { }

void TPollutionLayer::getPollutedPosNear(f32, JGeometry::TVec3<f32>*) { }

void TPollutionLayer::getPollutedPos(f32, JGeometry::TVec3<f32>*) { }

void TPollutionLayer::changeEffectScale(const JGeometry::TVec3<f32>&, f32) { }

void TPollutionLayer::spread() { }

void TPollutionLayer::electric() { }

void TPollutionLayer::glassWall() { }

void TPollutionLayer::fire() { }

void TPollutionLayer::action() { }
