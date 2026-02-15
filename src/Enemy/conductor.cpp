#include <Enemy/Conductor.hpp>
#include <Enemy/NpcSave.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Graph.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/AreaCylinder.hpp>
#include <Enemy/Generator.hpp>
#include <Enemy/EnemyTable.hpp>
#include <Map/Map.hpp>
#include <Map/PollutionManager.hpp>
#include <Player/MarioAccess.hpp>
#include <Camera/Camera.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <System/MarDirector.hpp>
#include <JSystem/JMath.hpp>

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
	unk10.push_back(param_1);
}

void TConductor::registerEnemyManager(TEnemyManager* param_1)
{
	unk20.push_back(param_1);
}

void TConductor::registerAloneActor(TLiveActor* param_1)
{
	unk30.push_back(param_1);
}

void TConductor::registerOtherObj(JDrama::TViewObj* param_1)
{
	unk40.push_back(param_1);
}

void TConductor::registerGenerator(TGenerator* param_1)
{
	unk60.push_back(param_1);
}

void TConductor::registerAreaCylinderManager(TAreaCylinderManager* param_1)
{
	unk50.push_back(param_1);
}

void TConductor::registerSDLModelData(SDLModelData* param_1)
{
	unk70.push_back(param_1);
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
			(*it)->createEnemies((*it)->getCapacity());

	unkF8 = (TAreaCylinderManager*)search("ナメクリ出現エリアマネージャー");
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

BOOL TConductor::isBossDefeated()
{
	switch (gpMarDirector->mMap) {
	case 2: {
		TLiveManager* mgr = getManagerByName("ヒノクリ２マネージャー");
		if (!mgr)
			return true;
		for (int i = 0; i < mgr->getObjNum(); ++i)
			if (!((TSpineEnemy*)mgr->getObj(i))->checkLiveFlag(LIVE_FLAG_UNK40))
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

int TConductor::makeEnemyAppear(const JGeometry::TVec3<f32>& param_1,
                                const char* param_2, int param_3, int param_4)
{
	TLiveManager* mgr = getManagerByName(param_2);

	if (!mgr)
		return 0;

	int result = 0;

	for (int i = 0; i < mgr->getObjNum(); ++i) {
		TLiveActor* actor = (TLiveActor*)mgr->getObj(i);
		if (actor->checkLiveFlag(LIVE_FLAG_DEAD)) {
			((TSpineEnemy*)actor)->resetToPosition(param_1);
			++result;
			if (result >= param_3)
				return result;
		}
	}

	if (param_4 == 0)
		return result;

	for (int i = 0; i < mgr->getObjNum(); ++i) {
		TLiveActor* actor = (TLiveActor*)mgr->getObj(i);
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && actor->checkLiveFlag(LIVE_FLAG_UNK800)
		    && actor->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			((TSpineEnemy*)actor)->resetToPosition(param_1);
			++result;
			if (result >= param_3)
				return result;
		}
	}

	if (param_4 == 1)
		return result;

	for (int i = 0; i < mgr->getObjNum(); ++i) {
		TLiveActor* actor = (TLiveActor*)mgr->getObj(i);
		if (!actor->checkLiveFlag(LIVE_FLAG_DEAD)
		    && !actor->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)
		    && actor->checkLiveFlag(LIVE_FLAG_UNK800)) {
			((TSpineEnemy*)actor)->resetToPosition(param_1);
			++result;
			if (result >= param_3)
				return result;
		}
	}

	return result;
}

TSpineEnemy*
TConductor::makeOneEnemyAppear(const JGeometry::TVec3<f32>& param_1,
                               const char* param_2, int param_3)
{
	TEnemyManager* mgr = (TEnemyManager*)getManagerByName(param_2);

	if (!mgr)
		return nullptr;

	if (TSpineEnemy* actor = (TSpineEnemy*)mgr->getActorByFlag(0x1)) {
		actor->resetToPosition(param_1);
		return actor;
	}

	if (param_3 == 0)
		return nullptr;

	if (TSpineEnemy* actor = (TSpineEnemy*)mgr->getActorByFlag(0x804)) {
		actor->resetToPosition(param_1);
		return actor;
	}

	if (param_3 == 1)
		return nullptr;

	TSpineEnemy* enemy = mgr->getObj(0);
	enemy->resetToPosition(param_1);
	return enemy;
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

void TConductor::genEnemyFromPollution()
{
	if (unkFC == 0)
		return;

	if (!unkF0)
		return;

	if (gpMarDirector->unk58 % unk84.mGenerateTime.get() != 1)
		return;

	TStageEnemyInfo* info = unkF0->getMatchedInfo(0x1);

	if (!info)
		return;

	TEnemyManager* mgr = (TEnemyManager*)getManagerByName(info->mManagerName);

	if (!mgr)
		return;

	JGeometry::TVec3<f32> targetPos = *gpMarioPos;
	f32 r                           = MsRandF(unk84.mGenerateRadiusMin.get(),
	                                          unk84.mGenerateRadiusMax.get());

	f32 theta = MsRandF() * 360 * (65536.0f / 360.0f);
	targetPos.x += r * JMASSin(theta);
	targetPos.z += r * JMASCos(theta);

	const TBGCheckData* data;
	targetPos.y = gpMap->checkGround(targetPos, &data) + 1.0f;

	if (!gpPollution->isPolluted(targetPos.x, targetPos.y, targetPos.z))
		return;

	if (unkF8 != nullptr) {
		TAreaCylinder* cyl = unkF8->getCylinderContains(targetPos);
		if (!cyl) {
			f32 f = unk84.mGenerateProp.get();
			if (MsRandF() > f)
				return;
		} else {
			if (MsRandF() > cyl->unk24)
				return;
		}
	} else {
		f32 f = unk84.mGenerateProp.get();
		if (MsRandF() > f)
			return;
	}

	TSpineEnemy* enemy = mgr->getFarOutEnemy();
	if (!enemy)
		return;

	enemy->resetToPosition(targetPos);
	enemy->moveObject();
	if (enemy->getModel())
		enemy->getModel()->entry();
}

void TConductor::clipAloneActors(JDrama::TGraphics* param_1)
{
	JGadget::TList<TLiveActor*>::iterator it = unk30.begin(), e = unk30.end();

	SetViewFrustumClipCheckPerspective(
	    gpCamera->getFovy(), gpCamera->getAspect(), param_1->getNearPlane(),
	    unk84.getEnemyFarClip());

	for (; it != e; ++it) {
		TLiveActor* actor = *it;
		if (!actor->checkLiveFlag(LIVE_FLAG_UNK100)) {
			actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else if (ViewFrustumClipCheck(param_1, &actor->mPosition, 300.0f)) {
			actor->offLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		} else {
			actor->onLiveFlag(LIVE_FLAG_CLIPPED_OUT);
		}
	}
}

void TConductor::clipGenerators(JDrama::TGraphics* param_1) { }

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
		{
			JGadget::TList<TLiveManager*>::iterator it = unk10.begin();
			if (i != 0) {
				for (; it != unk10.end(); ++it)
					if ((*it)->hasMapCollision())
						(*it)->testPerform(param_1, param_2);
			} else {
				for (; it != unk10.end(); ++it)
					if (!(*it)->hasMapCollision())
						(*it)->testPerform(param_1, param_2);
			}
		}

		if (param_1 & 2)
			clipAloneActors(param_2);

		{
			JGadget::TList<TLiveActor*>::iterator it = unk30.begin(),
			                                      e  = unk30.end();
			if (i != 0) {
				for (; it != e; ++it)
					if ((*it)->hasMapCollision())
						(*it)->testPerform(param_1, param_2);
			} else {
				for (; it != e; ++it)
					if (!(*it)->hasMapCollision())
						(*it)->testPerform(param_1, param_2);
			}
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
