#include <Animal/AnimalBase.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/RandomUtil.hpp>

TAnimalBase::TAnimalBase(u32 actorType, const char* name)
    : TSpineEnemy(name)
{
	mActorType = actorType;
}

BOOL TAnimalBase::receiveMessage(THitActor* sender, u32 msg) { return FALSE; }

void TAnimalBase::calcRootMatrix() { }

void TAnimalBase::resetRandomCurPathNode()
{
	TPathNode curNode = unkF4;
	if (curNode.unk0 != nullptr)
		return;

	JGeometry::TVec3<f32> pos = curNode.getPoint();
	pos.x += 1000.0f * (MsRandF() - 0.5f);
	pos.z += 1000.0f * (MsRandF() - 0.5f);

	if (mActorType == 0x800001) {
		f32 r;
		if (pos.y <= 1000.0f)
			r = MsRandF();
		else
			r = MsRandF() - 0.5f;
		pos.y += 1000.0f * r;
	} else {
		pos.y -= 250.0f * MsRandF();
	}

	setGoalPath(TPathNode(pos));
}

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
