#include <Animal/fishoid.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/MarioAccess.hpp>

TRealoid::TRealoid(const char* name)
    : TSpineEnemy(name)
{
	unk150 = 0;
	mLiveFlag |= 0x38;
}

TRealoid::~TRealoid() { }

TRealoidActor::TRealoidActor(MActor* actor)
    : TTakeActor("boid")
{
	unk70 = actor;
	unk74 = 0;
}

TRealoidActor::~TRealoidActor() { }

MtxPtr TRealoidActor::getTakingMtx() { return unk78; }

void TRealoidActor::checkHitActors()
{
	if (unk74 & 6)
		return;

	THitActor** it  = mCollisions;
	THitActor** end = mCollisions + mColCount;
	for (; it != end; ++it) {
		if ((*it)->getActorType() == 0x80000001)
			SMS_SendMessageToMario(this, 0xE);
	}
}

void TRealoidActor::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (unk74 & 6)
		return;

	THitActor::perform(flags, graphics);
	if (!(unk74 & 1))
		unk70->perform(flags, graphics);
}
