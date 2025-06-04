#include <Enemy/EnemyManager.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/Enemy.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/SDLModel.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TSpineEnemyParams::TSpineEnemyParams(const char* path)
    : TParams(path)
    , PARAM_INIT(mSLHeadHeight, 120.0f)
    , PARAM_INIT(mSLBodyRadius, 30.0f)
    , PARAM_INIT(mSLWallRadius, 50.0f)
    , PARAM_INIT(mSLClipRadius, 300.0f)
    , PARAM_INIT(mSLFarClip, 10000.0f)
    , PARAM_INIT(mSLHitPointMax, 1)
    , PARAM_INIT(mSLInstanceNum, 100)
    , PARAM_INIT(mSLActiveEnemyNum, 10)
{
	TParams::load(mPrmPath);
}

void TSharedMActorSet::init(MActorAnmData* param_1, J3DModelData* param_2,
                            const char* param_3, int param_4)
{
	unk4 = param_4;
	unk0 = new MActor*[unk4];

	f32 coeff = 1.0f / unk4;
	for (int i = 0; i < unk4; ++i) {
		J3DModel* model = new J3DModel(param_2, 0, 1);
		unk0[i]         = new MActor(param_1);
		unk0[i]->setModel(model, 0);
		unk0[i]->setBck(param_3);
		J3DFrameCtrl* ctrl = unk0[i]->getFrameCtrl(0);
		ctrl->setFrame(coeff * ctrl->getEndFrame() * i);
	}

	unk8 = unk0[0]->getCurAnmIdx(0);
}

void TSharedMActorSet::calcAnm() { }

void TSharedMActorSet::setScale(const JGeometry::TVec3<f32>&) { }

TEnemyManager::TEnemyManager(const char* name)
    : TLiveManager(name)
    , unk38(nullptr)
    , unk3C(1.0f)
    , unk40(nullptr)
    , unk44(0)
    , unk48(0)
    , unk4C(0xffffffff)
    , unk50(0)
{
	gpConductor->registerEnemyManager(this);
}

TEnemyManager::~TEnemyManager() { }

void TEnemyManager::createSharedMActorSet(const char** param_1)
{
	if (unk44 <= 0)
		return;

	u32 num = 0;
	for (int i = 0; param_1[i] != nullptr; ++i)
		++num;

	u32 prev = unk44;
	unk44    = num;
	unk40    = new TSharedMActorSet[unk44];

	for (int i = 0; i < unk44; ++i) {
		J3DModelData* modelData = getModelDataKeeper()->getNthData(0)->unk0;
		unk40[i].init(getMActorAnmData(), modelData, param_1[i], prev);
	}
}

TSharedMActorSet* TEnemyManager::getSharedMActorSet(int idx)
{
	if (!unk40)
		return nullptr;

	for (int i = 0; i < unk44; ++i)
		if (idx == unk40[i].unk8)
			return &unk40[i];

	return nullptr;
}

void TEnemyManager::load(JSUMemoryInputStream& stream)
{
	TLiveManager::load(stream);
	createModelData();
	stream.read(&unk44, 4);
}

TSpineEnemy* TEnemyManager::createEnemyInstance() { return nullptr; }

void TEnemyManager::createEnemy() { }

void TEnemyManager::createEnemies(int param_1)
{
	if (param_1 + unk14 > unk10)
		param_1 = unk10 - unk14;

	if (unk38 != nullptr && param_1 + unk14 > unk38->mSLInstanceNum.get())
		param_1 = unk38->mSLInstanceNum.get() - unk14;

	if (param_1 < 0)
		return;

	for (int i = 0; i < param_1; ++i) {
		TSpineEnemy* enemy = createEnemyInstance();

		if (!enemy)
			continue;

		JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")->add(enemy);

		enemy->init(this);
	}
}

void TEnemyManager::clipEnemies(JDrama::TGraphics* graphics)
{
	f32 fVar1;
	f32 fVar2;
	if (unk38 == nullptr) {
		fVar1 = 300.0f;
		fVar2 = gpConductor->unk84.mEnemyFarClip.get();
	} else {
		fVar2 = unk38->mSLFarClip.get();
		fVar1 = unk38->mSLClipRadius.get();
	}

	TLiveManager::clipActorsAux(graphics, fVar2, fVar1);
}

void TEnemyManager::setSharedFlags() { }

void TEnemyManager::updateAnmSoundShared() { }

void TEnemyManager::copyFromShared() { }

void TEnemyManager::performShared(u32, JDrama::TGraphics*) { }

void TEnemyManager::perform(u32, JDrama::TGraphics*) { }

void TEnemyManager::getNearestEnemy(const JGeometry::TVec3<f32>&) { }

void TEnemyManager::getDeadEnemy() { }

void TEnemyManager::getFarOutEnemy() { }

void TEnemyManager::killChildren() { }

void TEnemyManager::killChildrenWithin(const JGeometry::TVec3<f32>&, f32) { }

void TEnemyManager::killOtherEnemies() { }

int TEnemyManager::countLivingEnemy() const { }

void TEnemyManager::createCopyAnmMtx(int) { }

bool TEnemyManager::copyAnmMtx(TSpineEnemy*) { }
