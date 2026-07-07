#include <Animal/AnimalBase.hpp>
#include <MSound/MSoundSE.hpp>

TAnimalBase::TAnimalBase(u32 actorType, const char* name)
    : TSpineEnemy(name)
{
	mActorType = actorType;
}

TAnimalBase::~TAnimalBase() { }

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
		MSoundSESystem::MSRandPlay::registerTrans(0x3813, &mPosition);
}

void TAnimalBase::load(JSUMemoryInputStream& stream)
{
	// TODO: 95.6% - logic matches; residual is register allocation cascading
	// from the same 16-byte MWCC stack-padding seen in loadAfter (likely a
	// shared inline in this class yet to be identified).
	TSpineEnemy::load(stream);

	int count;
	stream.read(&count, 4);

	int n = count;
	for (int i = 0; i < n - 1; ++i) {
		initNoLoad_(new TAnimalBase(mActorType, getName()));
	}
}
