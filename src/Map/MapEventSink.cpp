#include <Map/MapEventSink.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapStaticObject.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Enemy/GateKeeper.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MSoundMainSide.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <Jsystem/JDrama/JDRNameRefGen.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

u32 TMapEventSink::mCleanedDegree = 10;

TJointObj* TMapEventSink::getBuilding(int i) const
{
	return unk1C->getChild(0)->getChild(i + unk24);
}

bool TMapEventSink::isBuried(int i) const
{
	return !mIsBuildingRecovered[i - unk24];
}

f32 TMapEventSink::getSinkOffsetY() const
{
	return unk30->getMax().y - unk30->getMin().y;
}

// Binder over the pollution singleton.
static inline TPollutionManager* MapEventSinkPollution()
{
	TPollutionManager* m = gpPollution;
	return m;
}

TPollutionObj* TMapEventSink::getPollutionObj(int i)
{
	TMapEventSink::Unk60Struct* e = unk60;
	return MapEventSinkPollution()->getLayer(e[i].unk0)->getObj(e[i].unk2);
}

bool TMapEventSink::isFinishedAll() const
{
	for (int i = 0; i < mBuildingNum; ++i)
		if (!mIsBuildingRecovered[i])
			return false;

	return true;
}

void TMapEventSink::makeBuildingRecovered(int i)
{
	getBuilding(i)->alive();

	J3DTransformInfo& info = getBuilding(i)->getJoint()->getTransformInfo();
	info.mTranslate.y      = unk34;
	getBuilding(i)->getJoint()->setTransformInfo(info);

	unk5C[i]->remove();
	unk58[i]->setUp();
	mIsBuildingRecovered[i] = true;
}

void TMapEventSink::finishControl()
{
	makeBuildingRecovered(mRaisingBuildingIdx);
	unk2C               = nullptr;
	unk30               = nullptr;
	mRaisingBuildingIdx = -1;
	if (strcmp("イベント（リコゲート）", mName) == 0)
		MSMainProc::setBossLivesFlag2(false);
	else
		MSMainProc::setBossLivesFlag(false);
	TMapEvent::finishControl();
}

void TMapEventSink::rising()
{
	J3DTransformInfo& info = unk30->getTransformInfo();
	info.mTranslate.y += unk3C;
	unk30->setTransformInfo(info);
	unk1C->mActor->mModel->calc();
}

bool TMapEventSink::control()
{
	if (unk4C < unk40 - unk44 && unk4C > unk48)
		rising();

	const J3DTransformInfo& info = unk30->getTransformInfo();
	unk5C[mRaisingBuildingIdx]->moveTrans(JGeometry::TVec3<f32>(
	    info.mTranslate.x, info.mTranslate.y, info.mTranslate.z));

	if (unk4C > unk48
	    && (gpMarDirector->mMap != 2 || mIsBuildingRecovered[1 - unk24] == 0
	        || mRaisingBuildingIdx != 1)) {
		SMSRumbleMgr->start(0x13, (f32*)nullptr);
		SMSGetMSound()->startSoundActor(
		    MSD_SE_OBJ_QUAKE, unk50[mRaisingBuildingIdx], 0, nullptr, 0, 4);
	}

	if (unk4C > 0) {
		--unk4C;
		return false;
	} else {
		return true;
	}
}

// Binding level over a raw member read, worth +16 of low region in
// TMapEventSink::startControl (batch 127).
static inline MActor* MapEventSinkActor(const TJointModel* p)
{
	MActor* actor = p->mActor;
	return actor;
}

// Binding level over a raw member read, worth +16 of low region in
// TMapEventSink::startControl (batch 127).
static inline int MapEventSinkRaisingBuildingIdx(const TMapEventSink* p)
{
	int raisingBuildingIdx = p->mRaisingBuildingIdx;
	return raisingBuildingIdx;
}

void TMapEventSink::startControl()
{
	unk18 = 2;
	unk2C = getBuilding(getRaisingBuildingIdx());
	unk2C->alive();
	unk30 = getBuilding(getRaisingBuildingIdx())->getJoint();

	J3DTransformInfo& info = unk30->getTransformInfo();
	unk34                  = info.mTranslate.y;

	f32 dVar4;
	if (unk38 != 0.0f)
		dVar4 = unk38;
	else
		dVar4 = getSinkOffsetY();

	info.mTranslate.y -= dVar4;
	unk30->setTransformInfo(info);

	MapEventSinkActor(unk1C)->mModel->calc();
	int iVar3 = (unk40 - unk44) - unk48;
	unk3C     = dVar4 / iVar3;
	unk4C     = unk40;

	unk5C[MapEventSinkRaisingBuildingIdx(this)]->setUpTrans(
	    JGeometry::TVec3<f32>(info.mTranslate.x, info.mTranslate.y,
	                          info.mTranslate.z));
}

void TMapEventSink::initBuilding(int index, JSUMemoryInputStream& stream)
{
	u32 value;
	stream >> value;
	unk60[index].unk0 = value;
	stream >> value;
	unk60[index].unk2 = value;
	unk58[index]
	    = TMapObjBase::newAndInitBuildingCollisionWarp(index + 1, nullptr);
	unk5C[index]
	    = TMapObjBase::newAndInitBuildingCollisionMove(index + 1, nullptr);
}

void TMapEventSink::initWithBuildingNum(JSUMemoryInputStream& stream)
{
	u32 value;
	stream >> value;
	unk24                = value;
	unk50                = new JGeometry::TVec3<f32>[mBuildingNum];
	unk58                = new TMapCollisionWarp*[mBuildingNum];
	unk5C                = new TMapCollisionMove*[mBuildingNum];
	mIsBuildingRecovered = new bool[mBuildingNum];
	unk60                = new Unk60Struct[mBuildingNum];
}

void TMapEventSink::load(JSUMemoryInputStream& stream)
{
	TMapEvent::load(stream);
	mBuildingNum = stream.readU32();
	initWithBuildingNum(stream);
	for (int i = 0; i < mBuildingNum; ++i) {
		mIsBuildingRecovered[i] = false;
		getBuilding(i)->kill();
		initBuilding(i, stream);
	}

	if (gpMarDirector->mMap == 0) {
		mCleanedDegree = 30;
		unk38          = 200.0f;
	} else if (gpMarDirector->mMap == 2) {
		mCleanedDegree = 30;
	}
}

TMapEventSink::TMapEventSink(const char* name)
    : TMapEvent(name)
{
	mBuildingNum         = 0;
	unk24                = 0;
	mRaisingBuildingIdx  = -1;
	unk2C                = nullptr;
	unk30                = nullptr;
	unk34                = 0.0f;
	unk38                = 0.0f;
	unk3C                = 5.0f;
	unk40                = 600;
	unk44                = 0;
	unk48                = 0;
	unk4C                = 0;
	unk50                = nullptr;
	mIsBuildingRecovered = nullptr;
	unk58                = nullptr;
	unk5C                = nullptr;
	unk60                = nullptr;
	mCleanedDegree       = 10;
}

bool TMapEventSinkInPollution::watch()
{
	for (int i = 0; i < mBuildingNum; ++i) {
		if (!mIsBuildingRecovered[i] && getPollutionObj(i)->isCleaned()) {
			mRaisingBuildingIdx = i;
			return true;
		}
	}
	return false;
}

void TMapEventSinkInPollution::initBuriedBuilding()
{
	for (int i = 0; i < mBuildingNum; ++i)
		if (getPollutionObj(i)->isCleaned())
			makeBuildingRecovered(i);
}

void TMapEventSinkInPollution::loadAfter()
{
	TMapEventSink::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		gpPollution->getCounterObj().registerPollutionObj(
		    getPollutionObj(i), &getPollutionObj(i)->mCounter);
	}
}

TPollutionObj* TMapEventSinkInPollutionReset::getResetPollutionObj(int i)
{
	return gpPollution->getLayer(unk60[i].unk0)->getObj(unk60[i].unk2 + 1);
}

void TMapEventSinkInPollutionReset::makeBuildingRecovered(int i)
{
	TMapEventSinkInPollution::makeBuildingRecovered(i);
	getPollutionObj(i)->kill();
	getResetPollutionObj(i)->alive();
	getResetPollutionObj(i)->updateDepthMap();
}

void TMapEventSinkInPollutionReset::loadAfter()
{
	TMapEventSinkInPollution::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		getPollutionObj(i)->alive();
		getResetPollutionObj(i)->kill();
	}
}

void TMapEventSinkBianco::finishControl()
{
	char buffer[64];
	if (mRaisingBuildingIdx == 0) {
		TMapObjBase::setJointTransY(unk64, 0.0f);
		for (int i = 0; i < 6; ++i) {
			snprintf(buffer, 0x40, "バナナツリー（スケール） %d", i);
			JDrama::TNameRefGen::search<TLiveActor>(buffer)->receiveMessage(
			    gpModelWaterManager->unk2514[0], HIT_MESSAGE_SPRAYED_BY_WATER);
		}

		for (int i = 0; i < 7; ++i) {
			snprintf(buffer, 0x40, "落書き内%02d", i);
			JDrama::TNameRefGen::search<TLiveActor>(buffer)->receiveMessage(
			    gpModelWaterManager->unk2514[0], HIT_MESSAGE_SPRAYED_BY_WATER);
		}
	}

	TMapEventSinkInPollutionReset::finishControl();

	for (int i = 0; i < gpPollution->getJointModelNum(); ++i)
		gpPollution->getLayer(i)->stopDecay();
}

static inline int MapEventSinkRaisingIdx(const TMapEventSink* p)
{
	int i = p->mRaisingBuildingIdx;
	return i;
}

// Binder over the bell joint.
static inline J3DJoint* MapEventSinkBiancoJoint(const TMapEventSinkBianco* p)
{
	J3DJoint* joint = p->unk64;
	return joint;
}

void TMapEventSinkBianco::rising()
{
	TMapEventSinkInPollutionReset::rising();
	if (MapEventSinkRaisingIdx(this) == 0)
		TMapObjBase::moveJoint(MapEventSinkBiancoJoint(this), 0.0f, unk3C,
		                       0.0f);
}

// Binding level over a raw member read, worth +16 of low region in
// TMapEventSinkBianco::control (batch 127).
static inline JGeometry::TVec3<f32>* MapEventSinkUnk50(const TMapEventSinkBianco* p)
{
	JGeometry::TVec3<f32>* v50 = p->unk50;
	return v50;
}

bool TMapEventSinkBianco::control()
{
	if (mRaisingBuildingIdx == 0 && unk4C == unk7C) {
		gpItemManager->makeShineAppearWithTime(
		    "シャイン（坂上げ用）", 300, MapEventSinkUnk50(this)[mRaisingBuildingIdx].x,
		    MapEventSinkUnk50(this)[mRaisingBuildingIdx].y, MapEventSinkUnk50(this)[mRaisingBuildingIdx].z, 0, 0x3C,
		    0x3C);
	}
	return TMapEventSinkInPollutionReset::control();
}

void TMapEventSinkBianco::startControl()
{
	switch (mRaisingBuildingIdx) {
	case 0: {
		unk40 = 1320;
		unk44 = 120;
		unk48 = 600;
		unk38 = 1700.0f;
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		unk7C = 360;

		gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_SLOPE_A, &zero, 0,
		                             nullptr);
		gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_SLOPE_B, &zero, 2,
		                             nullptr);
		break;
	}

	case 1:
		unk40 = 600;
		unk44 = 120;
		unk48 = 120;
		unk38 = 1500.0f;
		break;
	}

	TMapEventSinkInPollutionReset::startControl();

	if (mRaisingBuildingIdx == 0) {
		SMS_ShowJoint(unk64->getMesh(), true);
		SMS_MarioWarpRequest(unk6C, unk78);
		SMSGetMSound()->startSoundSystemSE(MSD_SE_SY_CLEAR_SIGN_BIG, 0,
		                                   nullptr, 0);
		// TODO: 99.9%. Only the frame is short by 8 bytes: retail keeps the
		// zero vector at 0x2c where we put it at 0x24, so one 8-byte inline
		// temp is missing below it. A reference binding on this array element
		// is inert (measured).
		unk50[mRaisingBuildingIdx].set(7170.0f, 3675.0f, -185.0f);
		SMSGetMarDirector()->fireStartDemoCamera(
		    "bianco0_event0", nullptr, -1, 0.0f, true, nullptr, 0, nullptr,
		    JDrama::TFlagT<u16>(0));
	}
}

bool TMapEventSinkBianco::watch()
{
	if (!mIsBuildingRecovered[0]
	    && mGateKeeper->checkLiveFlag(LIVE_FLAG_DEAD)) {
		mRaisingBuildingIdx = 0;
		for (int i = 0; i < gpPollution->getJointModelNum(); ++i)
			gpPollution->getLayer(i)->startDecay();
		return true;
	}

	for (int i = 1; i < mBuildingNum; ++i) {
		if (!mIsBuildingRecovered[i]) {
			if (getPollutionObj(i)->isCleaned()) {
				mRaisingBuildingIdx = i;
				return true;
			}
		}
	}

	return false;
}

// TODO: 56.7%. Retail's only structural difference is that the inlined
// TMapEventSinkInPollutionReset::loadAfter() keeps its own base call as
// `bl TMapEventSinkInPollution::loadAfter()` (one `bl`, one alive/kill loop),
// while MWCC expands ours at depth two and duplicates the registerPollutionObj
// loop. Both bases are instruction-exact on their own, so this is an inline
// refusal at depth two, not a wrong callee.
void TMapEventSinkBianco::loadAfter()
{
	TMapEventSinkInPollutionReset::loadAfter();

	TMapStaticObj* ref = JDrama::TNameRefGen::search<TMapStaticObj>("鏡内地形");
	unk64              = ref->getModelData()->getJointNodePointer(2);
	TMapObjBase::moveJoint(unk64, 0.0f, -1700.0f, 0.0f);
	SMS_ShowJoint(unk64->getMesh(), false);
	mGateKeeper
	    = JDrama::TNameRefGen::search<TGateKeeperBase>("ゲートキーパー");
}

void TMapEventSinkBianco::load(JSUMemoryInputStream& stream)
{
	TMapEventSinkInPollutionReset::load(stream);
	stream.readString();
	stream >> unk6C.x >> unk6C.y >> unk6C.z;
	s32 dummy;
	stream >> dummy;
	stream >> unk78;

	SMS_LoadParticle("/scene/map/map/ms_objup_slope_a.jpa",
	                 MAP_MAP_MS_OBJUP_SLOPE_A);
	SMS_LoadParticle("/scene/map/map/ms_objup_slope_b.jpa",
	                 MAP_MAP_MS_OBJUP_SLOPE_B);
}

// Binder over the raising building's placement.
static inline JGeometry::TVec3<f32>*
MapEventSinkRaisingPos(const TMapEventSinkShadowMario* p)
{
	int idx                    = p->mRaisingBuildingIdx;
	JDrama::TPlacement* obj    = p->unk64[idx];
	JGeometry::TVec3<f32>* pos = &obj->mPosition;
	return pos;
}

void TMapEventSinkShadowMario::rising()
{
	TMapEventSink::rising();
	MapEventSinkRaisingPos(this)->y += unk3C;
}

void TMapEventSinkShadowMario::raiseBuilding(int i)
{
	mRaisingBuildingIdx = i;
	startControl();
}

void TMapEventSinkShadowMario::loadAfter()
{
	TMapEventSink::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		unk64[i] = JDrama::TNameRefGen::search<JDrama::TPlacement>(unk68[i]);
		TJointObj* obj = getBuilding(i);
		unk64[i]->mPosition.y
		    -= obj->getJoint()->getMax().y - obj->getJoint()->getMin().y;
	}
}

void TMapEventSinkShadowMario::initBuilding(int i, JSUMemoryInputStream& stream)
{
	TMapEventSink::initBuilding(i, stream);
	unk68[i] = stream.readString();
}

void TMapEventSinkShadowMario::initWithBuildingNum(JSUMemoryInputStream& stream)
{
	TMapEventSink::initWithBuildingNum(stream);
	unk64 = new JDrama::TPlacement*[mBuildingNum];
	unk68 = new const char*[mBuildingNum];
}
