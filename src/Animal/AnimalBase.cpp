#include <Animal/AnimalBase.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>

TAnimalBase::TAnimalBase(u32 actorType, const char* name)
    : TSpineEnemy(name)
{
	mActorType = actorType;
}

BOOL TAnimalBase::receiveMessage(THitActor* sender, u32 msg)
{
	return FALSE;
}

void TAnimalBase::calcRootMatrix() { }

void TAnimalBase::loadAfter()
{
	// TODO: 99.7% - stack frame is 0x10 larger in the original (MWCC padding
	// from an unresolved inline); logic is otherwise byte-identical.
	TNameRef::loadAfter();
	if (mActorType == 0x800001)
		MSoundSESystem::MSRandPlay::registerTrans(MSD_SE_OBJ_KAMOME_SOLO,
		                                          &mPosition);
}

void TAnimalBase::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);

	s32 count;
	stream >> count;

	for (int i = 0; i < count - 1; ++i) {
		initNoLoad_(new TAnimalBase(mActorType, getName()));
	}
}
