#include <Animal/AnimalBase.hpp>

TAnimalBase::TAnimalBase(u32 actorType, const char* name)
    : TSpineEnemy(name)
{
	mActorType = actorType;
}

BOOL TAnimalBase::receiveMessage(THitActor* sender, u32 msg)
{
	return 0;
}

void TAnimalBase::calcRootMatrix() { }
