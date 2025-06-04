#include <Enemy/Conductor.hpp>
#include <Enemy/NpcSave.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Enemy.hpp>
#include <System/MarDirector.hpp>

// rogue include
#include <M3DUtil/InfectiousStrings.hpp>

TConductor::TCondParams::TCondParams()
    : TParams("/conductor.prm")
    , PARAM_INIT(mEnemyFarClip, 5000.0f)
    , PARAM_INIT(mGenerateRadiusMax, 1000.0f)
    , PARAM_INIT(mGenerateRadiusMin, 200.0f)
    , PARAM_INIT(mGenerateTime, 300)
    , PARAM_INIT(mGenerateProp, 0.01f)
{
	TParams::load(mPrmPath);
}

TConductor::TConductor()
    : JDrama::TViewObj("コンダクター")
    , unk80(nullptr)
    , unkF0(0)
    , unkF4(nullptr)
    , unkF8(nullptr)
    , unkFC(1)
{
	unkF4 = new TNpcParams;
}

TConductor::~TConductor() { }

void TConductor::makeGraphGroup(void* param_1)
{
	unk80 = new TGraphGroup(param_1);
}

void TConductor::initGraphGroup() { unk80->initGraphGroup(); }

void TConductor::registerManager(TLiveManager* param_1)
{
	unk10.push_back2(param_1);
}

void TConductor::registerEnemyManager(TEnemyManager* param_1)
{
	unk20.push_back2(param_1);
}

void TConductor::registerAloneActor(TLiveActor* param_1)
{
	unk30.push_back2(param_1);
}

void TConductor::registerOtherObj(JDrama::TViewObj* param_1)
{
	unk40.push_back2(param_1);
}

void TConductor::registerGenerator(TGenerator* param_1)
{
	unk60.push_back2(param_1);
}

void TConductor::registerAreaCylinderManager(TAreaCylinderManager* param_1)
{
	unk50.push_back2(param_1);
}

void TConductor::registerSDLModelData(SDLModelData* param_1)
{
	unk70.push_back2(param_1);
}

void TConductor::registerEnemyInfoTable(TStageEnemyInfoTable* param_1)
{
	unkF0 = param_1;
}

void TConductor::init()
{
	JGadget::TList<TEnemyManager*>::iterator it, e;
	for (it = unk20.begin(), e = unk20.end(); it != e; ++it)
		if (!(*it)->search("ヒノクリ２マネージャー")
		    && !(*it)->search("ボスゲッソーマネージャー"))
			(*it)->createEnemies((*it)->getUnk10());

	unkF8 = (JDrama::TViewObj*)search("ナメクリ出現エリアマネージャー");
}

static void dummy(JDrama::TNameRef* v) { v->search("ナメクリマネージャー"); }

TGraphWeb* TConductor::getGraphByName(const char* name)
{
	if (!unk80)
		return nullptr;

	return unk80->getGraphByName(name);
}

TLiveManager* TConductor::getManagerByName(const char* name)
{
	u16 key = JDrama::TNameRef::calcKeyCode(name);

	JGadget::TList<TLiveManager*>::iterator it, e;
	for (it = unk10.begin(), e = unk10.end(); it != e; ++it)
		if ((*it)->searchF(key, name))
			return *it;

	return nullptr;
}

void TConductor::polluterExterminated() { }

bool TConductor::isBossDefeated()
{
	switch (gpMarDirector->mMap) {
	case 2: {
		TLiveManager* mgr = getManagerByName("ヒノクリ２マネージャー");
		if (!mgr)
			return true;
		for (int i = 0; i < mgr->objNum(); ++i)
			if (!((TSpineEnemy*)mgr->getObj(i))->checkLiveFlag(0x40))
				return false;

		return true;
	}

	case 3: {
		TEnemyManager* mgr
		    = (TEnemyManager*)getManagerByName("ボスゲッソーマネージャー");
		if (!mgr || mgr->countLivingEnemy() == 0)
			return true;

		return false;
	}
	}
}

void TConductor::conduct() { }

void TConductor::maskNFlagOfChildren(int, u32) { }

void TConductor::makeEnemyAppear(const JGeometry::TVec3<f32>&, const char*, int,
                                 int)
{
}

void TConductor::makeOneEnemyAppear(const JGeometry::TVec3<f32>&, const char*,
                                    int)
{
}

void TConductor::killEnemiesWithin(const JGeometry::TVec3<f32>& param_1,
                                   f32 param_2)
{
	JGadget::TList<TEnemyManager*>::iterator it;
	for (it = unk20.begin(); it != unk20.end(); ++it) {
		if ((*it)->search("ボスワンワンマネージャー") == nullptr)
			(*it)->killChildrenWithin(param_1, param_2);
	}
}

void TConductor::genEnemyFromPollution() { }

void TConductor::clipAloneActors(JDrama::TGraphics*) { }

void TConductor::clipGenerators(JDrama::TGraphics*) { }

JDrama::TNameRef* TConductor::searchF(u16, const char*) { }

void TConductor::perform(u32, JDrama::TGraphics*) { }
