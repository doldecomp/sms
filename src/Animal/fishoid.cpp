#include <Animal/fishoid.hpp>

TRealoid::TRealoid(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= 0x38;
}

TRealoidActor::TRealoidActor(MActor* actor)
    : TTakeActor("boid")
{
	unk70 = actor;
	unk74 = 0;
}
