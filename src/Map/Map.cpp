#include <Map/Map.hpp>
#include <Map/MapCollisionData.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapWarp.hpp>
#include <Map/MapXlu.hpp>
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
#include <Map/MapCollisionEntry.hpp>

TMap* gpMap;

// UNUSED, 0x5c in the map: inlined into initStage().
static void initOption()
{
	TMapObjOptionWall* wall = new TMapObjOptionWall("オプション用壁");
	wall->init();
	TMapObjBase::joinToGroup("マップグループ", wall);
}

// UNUSED, 0x48 in the map: inlined into initStage().
static void initSirena()
{
	if (SMSGetMarDirector()->getCurrentStage() == 0)
		return;
	TMapObjBase::newAndInitBuildingCollisionWarp(1, nullptr)->setUp();
}

static void initMonte()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >(
	        "インダイレクトシーン");

	TMapStaticObj* obj = new TMapStaticObj("水インダイレクト");
	obj->init("SeaIndirect");
	group->getChildren().push_back(obj);

	if (SMSGetMarDirector()->getCurrentStage() == 0
	    || SMSGetMarDirector()->getCurrentStage() == 2
	    || SMSGetMarDirector()->getCurrentStage() == 5
	    || SMSGetMarDirector()->getCurrentStage() == 6) {
		SMS_LoadParticle("/scene/map/pollution/ms_newfire_b.jpa", 0x1DC);
		SMS_LoadParticle("/scene/map/pollution/ms_newfire_a.jpa", 0x65);
	}

	if (SMSGetMarDirector()->getCurrentStage() == 1
	    || SMSGetMarDirector()->getCurrentStage() == 3
	    || SMSGetMarDirector()->getCurrentStage() == 5
	    || SMSGetMarDirector()->getCurrentStage() == 7) {
		SMS_LoadParticle("/scene/map/map/ms_monte_yuge.jpa", 0x156);
	}
}

// TODO: every instruction matches; frame 0x138 against retail's 0x148. The
// three events share one `event` slot as in retail, and the director reads go
// through SMSGetMarDirector() (the `!= 0 && != 0` pair is +8 each, the first
// two are inert), leaving a uniform 0x10 of low region. Inert: a named warp
// local in the loop, split declarations, a group alias at the gate.
static void initMare()
{
	JDrama::TViewObjPtrListT<JDrama::TViewObj>* group
	    = JDrama::TNameRefGen::search<
	        JDrama::TViewObjPtrListT<JDrama::TViewObj> >("マップグループ");

	if (SMSGetMarDirector()->getCurrentStage() == 5) {
		TMapStaticObj* gate = new TMapStaticObj("マーレ５ＥＸゲート");
		gate->init("Mare5ExGate");
		group->getChildren().push_back(gate);
	}

	if (SMSGetMarDirector()->getCurrentStage() == 0) {
		SMS_LoadParticle("/scene/map/map/ms_mare_objup_a.jpa",
		                 MAP_MAP_MS_MARE_OBJUP_A);
		SMS_LoadParticle("/scene/map/map/ms_mare_objup_b.jpa",
		                 MAP_MAP_MS_MARE_OBJUP_B);
	}

	if (SMSGetMarDirector()->getCurrentStage() != 0
	    && SMSGetMarDirector()->getCurrentStage() != 0) {
		for (int i = 1; i < 8; ++i)
			TMapObjBase::newAndInitBuildingCollisionWarp(i, nullptr)->setUp();
	}

	TMareEventDepressWall* event
	    = new TMareEventDepressWall("イベント（マーレへこむ壁）");
	event->init1stEvent();
	group->getChildren().push_back(event);

	event = new TMareEventDepressWall("イベント（マーレへこむ壁）");
	event->init2ndEvent();
	group->getChildren().push_back(event);

	event = new TMareEventDepressWall("イベント（マーレへこむ壁）");
	event->init3rdEvent();
	group->getChildren().push_back(event);
}

// Pragma residue (sweep 360): protects initStage() (99.94 -> 55.8) and, less
// visibly, initMonte/initMare/initStageCommon and this unit's data (100 ->
// 90.3). initPinnaParco is exact and static, and the map keeps it out of line,
// but it is only 6 statements against a depth-1 budget of 14.
// TODO (cc38): three statements short (fillers). Naming either constructor
// argument tips it but hoists the argument above its `__nw__`
// (initPinnaParco 63%).
#pragma dont_inline on
// Binding level worth +8 of low region, landing initPinnaParco's frame at
// 0x50 (batch 121).
static inline TMapModelManager* MapGetModelManager(TMap* p)
{
	TMapModelManager* modelManager = p->getModelManager();
	return modelManager;
}

static void initPinnaParco()
{
	J3DModel* model = new J3DModel(
	    MapGetModelManager(gpMap)->getJointModel(0)->getModelData(), 0, 1);
	MActor* actor = new MActor(gpMap->getModelManager()->getMActorAnmData());
	actor->setModel(model, 0);
	TMapModelActor* mapModelActor = new TMapModelActor("ピンナ鏡用地形モデル");
	mapModelActor->setActor(actor);
	TMapObjBase::joinToGroup("鏡シーン", mapModelActor);
}
#pragma dont_inline off

// UNUSED, 0x58 in the map: inlined into initStage().
static void initPinnaBeach()
{
	SMS_LoadParticle("/scene/mapObj/SandSteam.jpa", 0x6A);
}

// UNUSED, 0x64 in the map: inlined into initStage().
static void initBianco()
{
	if (SMSGetMarDirector()->getCurrentStage() == 0)
		return;
	TMapObjBase::newAndInitBuildingCollisionWarp(1, nullptr)->setUp();
	TMapObjBase::newAndInitBuildingCollisionWarp(2, nullptr)->setUp();
}

// UNUSED, 0x6c in the map: inlined into initStage().
static void initDolpic()
{
	if (SMSGetMarDirector()->getCurrentStage() != 5
	    && SMSGetMarDirector()->getCurrentStage() != 9) {
		TMapObjBase::newAndInitBuildingCollisionWarp(1, nullptr)->setUp();
		TMapObjBase::newAndInitBuildingCollisionWarp(2, nullptr)->setUp();
	}
}

// TODO: 99.6%. Every instruction matches; retail's frame is 0x78 larger, with
// all push_back temporaries 0x74 higher (an unidentified low-region block).
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
		TMapStaticObj* obj = new TMapStaticObj("波（遠景）");
		obj->init("sea");

		obj = new TMapStaticObj("インダイレクト波");
		obj->init("SeaIndirect");
		group->getChildren().push_back(obj);

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

// TODO: every instruction matches; the frame is 0x18 short (0x58 vs 0x70).
// The director accessor bought 0x20; naming the warp objects and restoring
// the UNUSED per-area helpers are inert.
static void initStage()
{
	if (SMSGetMarDirector()->getCurrentStage() > 9)
		return;

	initStageCommon();

	switch (SMSGetMarDirector()->getCurrentMap()) {
	case 1:
		initDolpic();
		break;
	case 2:
		initBianco();
		break;
	case 9: // Mare
		initMare();
		break;
	case 8: // Monte
		initMonte();
		break;
	case 6:
		initSirena();
		break;
	case 5:
		initPinnaBeach();
		break;
	case 13: // Pinna Parco
		initPinnaParco();
		break;
	case 15:
		initOption();
		break;
	}
}

void TMap::updateDelfino()
{
	TMapWarp* warp = mWarp;
	int cube       = gpCubeArea->unk1C;
	if (cube != warp->unk8) {
		if (cube != -1)
			warp->changeModel(cube);
		else if (SMSGetMarDirector()->getCurrentStage() != 0)
			warp->changeModel(3);
	}
}

void TMap::updateMonte()
{
	if (SMSGetMarDirector()->getCurrentStage() == 1
	    || SMSGetMarDirector()->getCurrentStage() == 3
	    || SMSGetMarDirector()->getCurrentStage() == 5
	    || SMSGetMarDirector()->getCurrentStage() == 7)
		gpMarioParticleManager->emit(MAP_MAP_MS_MONTE_YUGE,
		                             &gpMapObjManager->unk44, 1, this);
}

static void updateRicco()
{
	static JGeometry::TVec3<f32> pos(1815.0f, 1500.0f, 1550.0f);
	SMSGetMSound()->startSoundActor(0x3000, &pos, 0, nullptr, 0, 4);
}

void TMap::update()
{
	switch (SMSGetMarDirector()->getCurrentMap()) {
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

	if (SMSGetMarDirector()->unk124 != 0 || gpCamera->isDemoCamera())
		return;

	if (SMSGetMarDirector()->getCurrentMap() == 0x39
	    || SMSGetMarDirector()->getCurrentMap() == 0x10
	    || SMS_CheckMarioFlag(MARIO_FLAG_VISIBLE))
		return;

	const JGeometry::TVec3<f32>& camPos = SMSGetCamera()->getUnk124();
	f32 height = gpMapObjWave->getHeight(camPos.x, camPos.y, camPos.z);
	if (height == SMSGetCamera()->getUnk124().y || SMSGetCamera()->getUnk124().y > height) {
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

bool TMap::isTouchedOneWall(const JGeometry::TVec3<f32>& pos, f32 radius) const
{
	return isTouchedOneWall(pos.x, pos.y, pos.z, radius);
}

// TODO: frame exact but the wall record and the spilled x/z parameters sit
// 4 bytes low (one 4-byte pool item missing below them). Inert: a named or
// ternary result, an if/return pair, local copies of x/z; a TU-local
// forwarding helper costs 14% of match.
bool TMap::isTouchedOneWall(f32 x, f32 y, f32 z, f32 radius) const
{
	return isTouchedOneWallAndMoveXZ(&x, y, &z, radius);
}

bool TMap::isTouchedOneWallAndMoveXZ(f32* x, f32 y, f32* z, f32 radius) const
{
	TBGWallCheckRecord record(*x, y, *z, radius, 1, 0);

	int r = mCollisionData->checkWalls(&record);
	if (r != 0 ? true : false) {
		const JGeometry::TVec3<f32>& center = record.mCenter;
		*x = center.x;
		*z = center.z;
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

f32 TMap::checkGroundExactY(const JGeometry::TVec3<f32>& pos,
                            const TBGCheckData** result) const
{
	return checkGroundExactY(pos.x, pos.y, pos.z, result);
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

void TMap::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		update();
		mCollisionData->initMoveCollision();
		mWarp->watchToWarp();
	}

	if (cue & CUE_ENTRY) {
		if ((cue & CUE_SEMITRANSPARENT_PRIO_2)) {
			if (!mXlu->changeXluJoint(1))
				return;
		} else if ((cue & CUE_SEMITRANSPARENT_PRIO_1)) {
			if (!mXlu->changeXluJoint(0))
				return;
		} else {
			mXlu->changeNormalJoint();
		}
	}

	if (cue & CUE_DRAW)
		draw(cue, graphics);

	mModelManager->perform(cue, graphics);
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

