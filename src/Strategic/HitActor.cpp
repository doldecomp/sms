#include <Strategic/HitActor.hpp>

THitActor::THitActor(const char* name)
    : JDrama::TActor(name)
{
}

void THitActor::initHitActor(u32, u16, int, float, float, float, float) { }

void THitActor::perform(u32, JDrama::TGraphics*) { }

f32 THitActor::calcEntryRadius() { }
