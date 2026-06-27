#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapWarp.hpp>
#include <Map/MapXlu.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapStaticObject.hpp>
#include <Map/MapEventMare.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>
#include <MoveBG/MapObjOption.hpp>
#include <MoveBG/MapObjWater.hpp>
#include <MoveBG/MapObjWave.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/EmitterViewObj.hpp>
#include <Camera/Camera.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

TMap* gpMap;

static void initMonte()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("マップグループ");

	TMapStaticObj* obj = new TMapStaticObj("水インダイレクト");
	obj->init("SeaIndirect");
	group->getChildren().push_back(obj);

	if (gpMarDirector->getCurrentStage() == 0
	    || gpMarDirector->getCurrentStage() == 2
	    || gpMarDirector->getCurrentStage() == 5
	    || gpMarDirector->getCurrentStage() == 6) {
		SMS_LoadParticle("/scene/map/pollution/ms_newfire_b.jpa", 0x1DC);
		SMS_LoadParticle("/scene/map/pollution/ms_newfire_a.jpa", 0x65);
	}

	if (gpMarDirector->getCurrentStage() == 1
	    || gpMarDirector->getCurrentStage() == 3
	    || gpMarDirector->getCurrentStage() == 5
	    || gpMarDirector->getCurrentStage() == 7) {
		SMS_LoadParticle("/scene/map/map/ms_monte_yuge.jpa", 0x156);
	}
}

static void initMare()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("マップグループ");

	if (gpMarDirector->getCurrentStage() == 5) {
		TMapStaticObj* gate = new TMapStaticObj("マーレ５ＥＸゲート");
		gate->init("Mare5ExGate");
		group->getChildren().push_back(gate);
	}

	if (gpMarDirector->getCurrentStage() == 0) {
		SMS_LoadParticle("/scene/map/map/ms_mare_objup_a.jpa",
		                 MAP_MAP_MS_MARE_OBJUP_A);
		SMS_LoadParticle("/scene/map/map/ms_mare_objup_b.jpa",
		                 MAP_MAP_MS_MARE_OBJUP_B);
	}

	if (gpMarDirector->getCurrentStage() != 0
	    && gpMarDirector->getCurrentStage() != 0) {
		for (int i = 1; i < 8; ++i)
			TMapObjBase::newAndInitBuildingCollisionWarp(i, nullptr)->setUp();
	}

	{
		TMareEventDepressWall* event
		    = new TMareEventDepressWall("イベント(マーレへこむ壁)");
		event->init1stEvent();
		group->getChildren().push_back(event);
	}
	{
		TMareEventDepressWall* event
		    = new TMareEventDepressWall("イベント(マーレへこむ壁)");
		event->init2ndEvent();
		group->getChildren().push_back(event);
	}
	{
		TMareEventDepressWall* event
		    = new TMareEventDepressWall("イベント(マーレへこむ壁)");
		event->init3rdEvent();
		group->getChildren().push_back(event);
	}
}

#pragma dont_inline on
static void initPinnaParco()
{
	J3DModel* model = new J3DModel(
	    gpMap->getModelManager()->getJointModel(0)->getModelData(), 0, 1);
	MActor* actor = new MActor(gpMap->getModelManager()->getMActorAnmData());
	actor->setModel(model, 0);
	TMapModelActor* mapModelActor = new TMapModelActor("ピンナ鏡用地形モデル");
	mapModelActor->setActor(actor);
	TMapObjBase::joinToGroup("鏡シーン", mapModelActor);
}
#pragma dont_inline off

static void initStageCommon()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >(
	        "インダイレクトシーン");
	JDrama::TNameRefGen::search<JDrama::TViewObjPtrListT<JDrama::TViewObj> >(
	    "マップグループ");

	if (gpMarDirector->getCurrentMap() == 4
	    || gpMarDirector->getCurrentMap() == 3
	    || gpMarDirector->getCurrentMap() == 0xD
	    || gpMarDirector->getCurrentMap() == 9
	    || gpMarDirector->getCurrentMap() == 5
	    || gpMarDirector->getCurrentMap() == 6
	    || gpMarDirector->getCurrentMap() == 0x14
	    || gpMarDirector->getCurrentMap() <= 1) {
		TMapStaticObj* sea = new TMapStaticObj("波（遠景）");
		sea->init("sea");

		TMapStaticObj* indirect = new TMapStaticObj("インダイレクト波");
		indirect->init("SeaIndirect");
		group->getChildren().push_back(indirect);

		TMapObjWaterFilter* filter
		    = new TMapObjWaterFilter("水中カメラフィルタ");
		filter->init();
		group->getChildren().push_back(filter);

		TMapObjSeaIndirect* sceneIndirect
		    = new TMapObjSeaIndirect("水中カメラインダイレクト");
		sceneIndirect->init();
		group->getChildren().push_back(sceneIndirect);
	}
	if (gpMarDirector->mMap == 2) {
		TMapObjSeaIndirect* sceneIndirect
		    = new TMapObjSeaIndirect("水中カメラインダイレクト");
		sceneIndirect->init();
		group->getChildren().push_back(sceneIndirect);
	}
}

static void initStage()
{
	if (gpMarDirector->getCurrentStage() > 9)
		return;

	initStageCommon();

	switch (gpMarDirector->getCurrentMap()) {
	case 1: { // Bianco
		if (gpMarDirector->getCurrentStage() == 5
		    || gpMarDirector->getCurrentStage() == 9)
			break;
		TMapObjBase::newAndInitBuildingCollisionWarp(1, nullptr)->setUp();
		TMapObjBase::newAndInitBuildingCollisionWarp(2, nullptr)->setUp();
		break;
	}
	case 2: // Ricco
		if (gpMarDirector->getCurrentStage() == 0)
			break;
		TMapObjBase::newAndInitBuildingCollisionWarp(1, nullptr)->setUp();
		TMapObjBase::newAndInitBuildingCollisionWarp(2, nullptr)->setUp();
		break;
	case 9: // Mare
		initMare();
		break;
	case 8: // Monte
		initMonte();
		break;
	case 6: // Pinna
		if (gpMarDirector->getCurrentStage() == 0)
			break;
		TMapObjBase::newAndInitBuildingCollisionWarp(1, nullptr)->setUp();
		break;
	case 5: // Sirena
		SMS_LoadParticle("/scene/mapObj/SandSteam.jpa", 0x6A);
		break;
	case 13: // Pinna Parco
		initPinnaParco();
		break;
	case 15: { // Option
		TMapObjOptionWall* wall = new TMapObjOptionWall("オプション用壁");
		wall->init();
		TMapObjBase::joinToGroup("マップグループ", wall);
		break;
	}
	}
}

void TMap::updateDelfino()
{
	int cube = gpCubeArea->unk1C;
	if (cube != mWarp->unk8) {
		if (cube != -1)
			mWarp->changeModel(cube);
		else if (gpMarDirector->getCurrentStage() != 0)
			mWarp->changeModel(3);
	}
}

void TMap::updateMonte()
{
	if (gpMarDirector->getCurrentStage() == 1
	    || gpMarDirector->getCurrentStage() == 3
	    || gpMarDirector->getCurrentStage() == 5
	    || gpMarDirector->getCurrentStage() == 7)
		gpMarioParticleManager->emit(0x156, &gpMapObjManager->unk44, 1, this);
}

static void updateRicco()
{
	static JGeometry::TVec3<f32> pos(1815.0f, 1500.0f, 1550.0f);
	SMSGetMSound()->startSoundActor(0x3000, &pos, 0, nullptr, 0, 4);
}

void TMap::update()
{
	switch (gpMarDirector->mMap) {
	case 3:
		updateRicco();
		break;

	case 8: // Monte
		updateMonte();
		break;

	case 7:
		updateDelfino();
		break;
	}

	if (gpMarDirector->unk124 != 0)
		return;

	if (gpCamera->isDemoCamera())
		return;

	if (gpMarDirector->getCurrentMap() == 0x39
	    || gpMarDirector->getCurrentMap() == 0x10)
		return;

	if (SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE))
		return;

	const JGeometry::TVec3<f32>& camPos = gpCamera->getUnk124();
	f32 height = gpMapObjWave->getHeight(camPos.x, camPos.y, camPos.z);
	if (height == gpCamera->getUnk124().y || gpCamera->getUnk124().y > height) {
		if (!unk20) {
			unk20 = 1;
			MSSeCallBack::setWaterCameraFir(false);
		}
	} else if (unk20) {
		unk20 = 0;
		MSSeCallBack::setWaterCameraFir(true);
	}
}

TBGCheckData* TMap::getIllegalCheckData()
{
	return &TMapCollisionData::mIllegalCheckData;
}

bool TMap::isInArea(f32 param_1, f32 param_2) const
{
	if (-mCollisionData->mGridExtentX < param_1
	    && param_1 < mCollisionData->mGridExtentX
	    && -mCollisionData->mGridExtentY < param_2
	    && param_2 < mCollisionData->mGridExtentY)
		return true;

	return false;
}

const TBGCheckData* TMap::intersectLine(const JGeometry::TVec3<f32>& param_1,
                                        const JGeometry::TVec3<f32>& param_2,
                                        bool param_3,
                                        JGeometry::TVec3<f32>* param_4) const
{
	mCollisionData->intersectLine(param_1, param_2, param_3, param_4);
}

bool TMap::isTouchedOneWall(f32 x, f32 y, f32 z, f32 radius) const
{
	return isTouchedOneWallAndMoveXZ(&x, y, &z, radius);
}

bool TMap::isTouchedOneWallAndMoveXZ(f32* x, f32 y, f32* z, f32 radius) const
{
	TBGWallCheckRecord record(*x, y, *z, radius, 1, 0);

	int r = mCollisionData->checkWalls(&record);
	if (r != 0 ? true : false) {
		*x = record.mCenter.x;
		*z = record.mCenter.z;
		return true;
	} else {
		return false;
	}
}

bool TMap::isTouchedWallsAndMoveXZ(TBGWallCheckRecord* record) const
{
	return mCollisionData->checkWalls(record) != 0 ? true : false;
}

f32 TMap::checkRoofIgnoreWaterThrough(f32 x, f32 y, f32 z,
                                      const TBGCheckData** result) const
{
	return mCollisionData->checkRoof(
	    x, y, z, TMapCollisionData::IGNORE_WATER_THROUGH, result);
}

f32 TMap::checkRoof(f32 x, f32 y, f32 z, const TBGCheckData** result) const
{
	return mCollisionData->checkRoof(x, y, z, 0, result);
}

f32 TMap::checkRoof(const JGeometry::TVec3<f32>& pos,
                    const TBGCheckData** param_2) const
{
	return mCollisionData->checkRoof(pos.x, pos.y, pos.z, 0, param_2);
}

f32 TMap::checkGroundIgnoreWaterThrough(f32 x, f32 y, f32 z,
                                        const TBGCheckData** result) const
{
	return mCollisionData->checkGround(
	    x, y, z, TMapCollisionData::IGNORE_WATER_THROUGH, result);
}

f32 TMap::checkGroundIgnoreWaterSurface(f32 x, f32 y, f32 z,
                                        const TBGCheckData** result) const
{
	return mCollisionData->checkGround(
	    x, y, z, TMapCollisionData::IGNORE_WATER_SURFACE, result);
}

f32 TMap::checkGroundIgnoreWaterSurface(const JGeometry::TVec3<f32>& pos,
                                        const TBGCheckData** result) const
{
	return mCollisionData->checkGround(
	    pos.x, pos.y, pos.z, TMapCollisionData::IGNORE_WATER_SURFACE, result);
}

f32 TMap::checkGroundExactY(f32 x, f32 y, f32 z,
                            const TBGCheckData** result) const
{
	return mCollisionData->checkGround(x, y - -78.0f, z, 0, result);
}

f32 TMap::checkGround(const JGeometry::TVec3<f32>& pos,
                      const TBGCheckData** result) const
{
	return mCollisionData->checkGround(pos.x, pos.y, pos.z, 0, result);
}

f32 TMap::checkGround(f32 x, f32 y, f32 z, const TBGCheckData** result) const
{
	return mCollisionData->checkGround(x, y, z, 0, result);
}

void TMap::changeModel(s16 param_1) const { mWarp->changeModel(param_1); }

void TMap::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if (param_1 & 1) {
		update();
		mCollisionData->initMoveCollision();
		mWarp->watchToWarp();
	}

	if (param_1 & 0x200) {
		if ((param_1 & 0x2000000)) {
			if (!mXlu->changeXluJoint(1))
				return;
		} else if ((param_1 & 0x4000000)) {
			if (!mXlu->changeXluJoint(0))
				return;
		} else {
			mXlu->changeNormalJoint();
		}
	}

	if (param_1 & 8)
		draw(param_1, param_2);

	mModelManager->perform(param_1, param_2);
}

void TMap::loadAfter()
{
	JDrama::TViewObj::loadAfter();
	initStage();
}

void TMap::load(JSUMemoryInputStream& stream)
{
	JDrama::TViewObj::load(stream);
	mXlu->init(stream);
	mModelManager->init();
	mCollisionData->init(stream);
	mWarp->initModel();
	mWarp->init(stream);
	mModelManager->mCollision->setUp();
}

TMap::TMap(const char* name)
    : JDrama::TViewObj(name)
{
	mCollisionData = new TMapCollisionData;
	mModelManager  = new TMapModelManager("地形モデル管理");
	mWarp          = new TMapWarp;
	mXlu           = new TMapXlu;
	unk20          = 0;

	gpMap = this;
}

TMap::~TMap() { }
