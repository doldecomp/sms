#include <Enemy/Conductor.hpp>
#include <Enemy/NpcSave.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Generator.hpp>
#include <Camera/Camera.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <M3DUtil/SDLModel.hpp>
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
	JGadget::TList<TEnemyManager*>::iterator it, e;
	for (it = unk20.begin(), e = unk20.end(); it != e; ++it) {
		if ((*it)->search("ボスワンワンマネージャー") == nullptr)
			(*it)->killChildrenWithin(param_1, param_2);
	}
}

#pragma dont_inline on
void TConductor::genEnemyFromPollution() { }
#pragma dont_inline off

void TConductor::clipAloneActors(JDrama::TGraphics*) { }

void TConductor::clipGenerators(JDrama::TGraphics*) { }

JDrama::TNameRef* TConductor::searchF(u16 key, const char* name)
{
	if (JDrama::TNameRef* ref = JDrama::TNameRef::searchF(key, name))
		return ref;

	for (JGadget::TList<TLiveManager*>::iterator it = unk10.begin(),
	                                             e  = unk10.end();
	     it != e; ++it)
		if (JDrama::TNameRef* ref = (*it)->searchF(key, name))
			return ref;

	for (JGadget::TList<TLiveActor*>::iterator it = unk30.begin(),
	                                           e  = unk30.end();
	     it != e; ++it)
		if (JDrama::TNameRef* ref = (*it)->searchF(key, name))
			return ref;

	for (JGadget::TList<TAreaCylinderManager*>::iterator it = unk50.begin(),
	                                                     e  = unk50.end();
	     it != e; ++it)
		if (JDrama::TNameRef* ref = (*it)->searchF(key, name))
			return ref;

	for (JGadget::TList<JDrama::TViewObj*>::iterator it = unk40.begin(),
	                                                 e  = unk40.end();
	     it != e; ++it)
		if (JDrama::TNameRef* ref = (*it)->searchF(key, name))
			return ref;

	return nullptr;
}

void TConductor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if ((param_1 & 1) && gpMarDirector->unk124 == 0)
		genEnemyFromPollution();

	for (int i = 1; i >= 0; --i) {
		if (i != 0) {
			JGadget::TList<TLiveManager*>::iterator it, e;
			for (it = unk10.begin(), e = unk10.end(); it != e; ++it)
				if ((*it)->hasMapCollision())
					(*it)->testPerform(param_1, param_2);
		} else {
			JGadget::TList<TLiveManager*>::iterator it, e;
			for (it = unk10.begin(), e = unk10.end(); it != e; ++it)
				if (!(*it)->hasMapCollision())
					(*it)->testPerform(param_1, param_2);
		}

		if (param_1 & 2) {
			SetViewFrustumClipCheckPerspective(
			    gpCamera->mFovy, gpCamera->mAspect, param_2->unkE8,
			    unk84.mEnemyFarClip.get());

			JGadget::TList<TLiveActor*>::iterator it, e;
			for (it = unk30.begin(), e = unk30.end(); it != e; ++it) {
				TLiveActor* actor = *it;
				if (!actor->checkLiveFlag(0x100)) {
					actor->offLiveFlag(0x4);
				} else if (ViewFrustumClipCheck(param_2, actor->mPosition,
				                                300.0)) {
					actor->offLiveFlag(0x4);
				} else {
					actor->onLiveFlag(0x4);
				}
			}
		}

		if (i != 0) {
			JGadget::TList<TLiveActor*>::iterator it, e;
			for (it = unk30.begin(), e = unk30.end(); it != e; ++it)
				if ((*it)->hasMapCollision())
					(*it)->testPerform(param_1, param_2);
		} else {
			JGadget::TList<TLiveActor*>::iterator it, e;
			for (it = unk30.begin(), e = unk30.end(); it != e; ++it)
				if (!(*it)->hasMapCollision())
					(*it)->testPerform(param_1, param_2);
		}
	}

	{
		JGadget::TList<TGenerator*>::iterator it, e;
		for (it = unk60.begin(), e = unk60.end(); it != e; ++it)
			(*it)->testPerform(param_1, param_2);
	}

	{
		JGadget::TList<JDrama::TViewObj*>::iterator it, e;
		for (it = unk40.begin(), e = unk40.end(); it != e; ++it)
			(*it)->testPerform(param_1, param_2);
	}

	{
		JGadget::TList<TAreaCylinderManager*>::iterator it, e;
		for (it = unk50.begin(), e = unk50.end(); it != e; ++it)
			(*it)->testPerform(param_1, param_2);
	}

	if (param_1 & 0x200) {
		JGadget::TList<SDLModelData*>::iterator it, e;
		for (it = unk70.begin(), e = unk70.end(); it != e; ++it)
			(*it)->entrySDLModels();
	}

	unk80->perform(param_1, param_2);
}
