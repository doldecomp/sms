#include <Enemy/TypicalEnemy.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Walker.hpp>
#include <Enemy/Graph.hpp>
#include <Strategic/Spine.hpp>

#include <M3DUtil/InfectiousStrings.hpp>

TTypicalEnemy::TTypicalEnemy(const char* name)
    : TSpineEnemy(name)
{
	unk88 = new TWalker;
}

void TTypicalEnemy::init(TLiveManager* param_1)
{
	TSpineEnemy::init(param_1);
	unk124->init(gpConductor->getGraphByName("main"));
	mSpine->initWith(&TNerveTypicalGraphWander::theNerve());
	unk124->reset();
	goToShortestNextGraphNode();
}

TTypicalParams::TTypicalParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLMoveSpeed, 4.0f)
{
	TParams::load(mPrmPath);
}

TTypicalManager::TTypicalManager(const char* name)
    : TEnemyManager(name)
{
}

void TTypicalManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TTypicalParams("/enemy/typical.prm");
	TEnemyManager::load(stream);
}

TTypicalEnemy* TTypicalManager::createEnemyInstance()
{
	return new TTypicalEnemy("典型敵");
}

DEFINE_NERVE(TNerveTypicalGraphWander, TLiveActor)
{
	TTypicalEnemy* self = (TTypicalEnemy*)spine->getBody();

	if (spine->getUnk20() == 0) {
		self->goToRandomNextGraphNode();
	}

	if (self->isReachedToGoal()) {
		spine->pushRaw(&TNerveTypicalGraphWander::theNerve());
		return true;
	} else {
		self->walkToCurPathNode(self->getMarchSpeed(), self->getTurnSpeed(),
		                        0.0f);
		self->doShortCut();
		return false;
	}
}
