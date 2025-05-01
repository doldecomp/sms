#include <Map/MapEventSink.hpp>
#include <Map/PollutionManager.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/MapStaticObject.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/ItemManager.hpp>
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
	return unk1C->mChildren[0]->mChildren[i + unk24];
}

bool TMapEventSink::isBuried(int i) const { return !unk54[i - unk24]; }

f32 TMapEventSink::getSinkOffsetY() const
{
	return unk30->getMax().y - unk30->getMin().y;
}

TPollutionObj* TMapEventSink::getPollutionObj(int i)
{
	return (TPollutionObj*)gpPollution->getJointModel(unk60[i].unk0)
	    ->mChildren[unk60[i].unk2];
}

bool TMapEventSink::isFinishedAll() const
{
	for (int i = 0; i < mBuildingNum; ++i)
		if (!unk54[i])
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
	unk54[i] = true;
}

void TMapEventSink::finishControl()
{
	makeBuildingRecovered(unk28);
	unk2C = nullptr;
	unk30 = nullptr;
	unk28 = -1;
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
	unk1C->mActor->unk4->calc();
}

bool TMapEventSink::control()
{
	if (unk4C < unk40 - unk44 && unk4C > unk48)
		rising();

	unk5C[unk28]->moveTrans(unk30->getTransformInfo().mTranslate);

	if (unk4C > unk48
	    && (gpMarDirector->map != 2 || unk54[1 - unk24] == 0 || unk28 != 1)) {
		SMSRumbleMgr->start(0x13, (f32*)nullptr);
		if (gpMSound->gateCheck(0x3008))
			MSoundSESystem::MSoundSE::startSoundActor(0x3008, unk50[unk28], 0,
			                                          nullptr, 0, 4);
	}

	if (unk4C > 0) {
		--unk4C;
		return false;
	} else {
		return true;
	}
}

void TMapEventSink::startControl()
{
	unk18 = 2;
	unk2C = getBuilding(unk28);
	unk2C->alive();
	unk30 = getBuilding(unk28)->getJoint();
	unk34 = unk30->getTransformInfo().mTranslate.y;

	f32 dVar4;
	if (unk38 != 0.0f)
		dVar4 = unk38;
	else
		dVar4 = getSinkOffsetY();

	J3DTransformInfo& info = unk30->getTransformInfo();
	info.mTranslate.y -= dVar4;
	unk30->setTransformInfo(info);

	unk1C->mActor->unk4->calc();
	int iVar3 = (unk40 - unk44) - unk48;
	unk3C     = dVar4 / iVar3;
	unk4C     = unk40;

	unk5C[unk28]->moveTrans(unk30->getTransformInfo().mTranslate);
}

void TMapEventSink::initBuilding(int index, JSUMemoryInputStream& stream)
{
	u32 value;
	stream.read(&value, 4);
	unk60[index].unk0 = value;
	stream.read(&value, 4);
	unk60[index].unk2 = value;
	unk58[index]
	    = TMapObjBase::newAndInitBuildingCollisionWarp(index + 1, nullptr);
	unk5C[index]
	    = TMapObjBase::newAndInitBuildingCollisionMove(index + 1, nullptr);
}

void TMapEventSink::initWithBuildingNum(JSUMemoryInputStream& stream)
{
	u32 value;
	stream.read(&value, 4);
	unk24 = value;
	unk50 = new JGeometry::TVec3<f32>[mBuildingNum];
	unk58 = new TMapCollisionWarp*[mBuildingNum];
	unk5C = new TMapCollisionMove*[mBuildingNum];
	unk54 = new bool[mBuildingNum];
	unk60 = new Unk60Struct[mBuildingNum];
}

void TMapEventSink::load(JSUMemoryInputStream& stream)
{
	TMapEvent::load(stream);
	mBuildingNum = stream.readU32();
	initWithBuildingNum(stream);
	for (int i = 0; i < mBuildingNum; ++i) {
		unk54[i] = 0;
		getBuilding(i)->kill();
		initBuilding(i, stream);
	}

	if (gpMarDirector->map == 0) {
		mCleanedDegree = 30;
		unk38          = 200.0f;
	} else if (gpMarDirector->map == 2) {
		mCleanedDegree = 30;
	}
}

TMapEventSink::TMapEventSink(const char* name)
    : TMapEvent(name)
{
	mBuildingNum   = 0;
	unk24          = 0;
	unk28          = -1;
	unk2C          = nullptr;
	unk30          = nullptr;
	unk34          = 0.0f;
	unk38          = 0.0f;
	unk3C          = 5.0f;
	unk40          = 600;
	unk44          = 0;
	unk48          = 0;
	unk4C          = 0;
	unk50          = nullptr;
	unk54          = nullptr;
	unk58          = nullptr;
	unk5C          = nullptr;
	unk60          = nullptr;
	mCleanedDegree = 10;
}

bool TMapEventSinkInPollution::watch()
{
	for (int i = 0; i < mBuildingNum; ++i) {
		if (!unk54[i]
		    && gpPollution->getLayer(unk60[i].unk0)
		           ->getObj(unk60[i].unk2)
		           ->isCleaned()) {
			unk28 = i;
			return true;
		}
	}
	return false;
}

void TMapEventSinkInPollution::initBuriedBuilding()
{
	for (int i = 0; i < mBuildingNum; ++i) {
		if (gpPollution->getLayer(unk60[i].unk0)
		        ->getObj(unk60[i].unk2)
		        ->isCleaned()) {
			makeBuildingRecovered(i);
		}
	}
}

void TMapEventSinkInPollution::loadAfter()
{
	TMapEventSink::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		TPollutionObj* obj
		    = gpPollution->getLayer(unk60[i].unk0)->getObj(unk60[i].unk2);
		gpPollution->getCounterObj().registerPollutionObj(obj, &obj->unk30);
	}
}

void TMapEventSinkInPollutionReset::getResetPollutionObj(int) { }

void TMapEventSinkInPollutionReset::makeBuildingRecovered(int i)
{
	TMapEventSinkInPollution::makeBuildingRecovered(i);
	gpPollution->getLayer(unk60[i].unk0)->getObj(unk60[i].unk2)->kill();
	gpPollution->getLayer(unk60[i].unk0)->getObj(unk60[i].unk2 + 1)->alive();
	gpPollution->getLayer(unk60[i].unk0)
	    ->getObj(unk60[i].unk2 + 1)
	    ->updateDepthMap();
}

void TMapEventSinkInPollutionReset::loadAfter()
{
	TMapEventSinkInPollution::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		gpPollution->getLayer(unk60[i].unk0)->getObj(unk60[i].unk2)->alive();
		gpPollution->getLayer(unk60[i].unk0)->getObj(unk60[i].unk2 + 1)->kill();
	}
}

void TMapEventSinkBianco::finishControl()
{
	char buffer[64];
	if (unk28 == 0) {
		TMapObjBase::setJointTransY(unk64, 0.0f);
		for (int i = 0; i < 6; ++i) {
			snprintf(buffer, 0x40, "バナナツリー（スケール） %d", i);
			TLiveActor* bananaTree
			    = (TLiveActor*)JDrama::TNameRefGen::getInstance()
			          ->getRootNameRef()
			          ->search(buffer);
			bananaTree->receiveMessage(gpModelWaterManager->unk2514[0], 0xf);
		}

		for (int i = 0; i < 7; ++i) {
			snprintf(buffer, 0x40, "落書き内%02d", i);
			TLiveActor* graffiti
			    = (TLiveActor*)JDrama::TNameRefGen::getInstance()
			          ->getRootNameRef()
			          ->search(buffer);
			graffiti->receiveMessage(gpModelWaterManager->unk2514[0], 0xf);
		}
	}

	TMapEventSinkInPollutionReset::finishControl();

	TPollutionManager* polman = gpPollution;
	for (int i = 0; i < polman->getJointModelNum(); ++i)
		polman->getLayer(i)->unk32 &= ~2;
}

void TMapEventSinkBianco::rising()
{
	TMapEventSinkInPollutionReset::rising();
	if (unk28 == 0)
		TMapObjBase::moveJoint(unk64, 0.0f, unk3C, 0.0f);
}

bool TMapEventSinkBianco::control()
{
	if (unk28 == 0 && unk4C == unk7C) {
		gpItemManager->makeShineAppearWithTime("シャイン（坂上げ用）", 300,
		                                       unk50[unk28].x, unk50[unk28].y,
		                                       unk50[unk28].z, 0, 0x3C, 0x3C);
	}
	return TMapEventSinkInPollutionReset::control();
}

void TMapEventSinkBianco::startControl()
{
	switch (unk28) {
	case 0: {
		unk40 = 1320;
		unk44 = 120;
		unk48 = 600;
		unk38 = 1700.0f;
		unk7C = 360;
		JGeometry::TVec3<f32> zero;
		zero.zero();
		gpMarioParticleManager->emit(0x59, &zero, 0, nullptr);
		gpMarioParticleManager->emit(0x1E1, &zero, 2, nullptr);
		break;
	}
	case 1:
		unk40 = 60;
		unk44 = 120;
		unk48 = 120;
		unk38 = 1500.0f;
		break;
	}

	TMapEventSinkInPollutionReset::startControl();
	if (unk28 == 0) {
		SMS_ShowJoint(unk64->getMesh(), true);
		SMS_MarioWarpRequest(unk6C, unk78);
		JGeometry::TVec3<f32>& v = unk50[unk28];
		v.x                      = 7170.0f;
		v.y                      = 3675.0f;
		v.z                      = -185.0f;
		gpMarDirector->fireStartDemoCamera("bianco0_event0", nullptr, -1, 0.0f,
		                                   true, nullptr, 0, nullptr,
		                                   JDrama::TFlagT<u16>(0));
	}
}

bool TMapEventSinkBianco::watch()
{
	// TODO: mGateKeeper is likely TGateKeeperBase, which we don't have yet
}

void TMapEventSinkBianco::loadAfter()
{
	TMapEventSinkInPollutionReset::loadAfter();
	TMapStaticObj* ref = (TMapStaticObj*)JDrama::TNameRef::search(
	    "鏡内地形シャイン（坂上げ用）");

	unk64 = ref->getModelData()->getJointNodePointer(2);
	TMapObjBase::moveJoint(unk64, 0.0f, -1700.0f, 0.0f);
	SMS_ShowJoint(unk64->getMesh(), false);
	mGateKeeper = JDrama::TNameRef::search("ゲートキーパー");
}

// TODO: move this
extern u8 gParticleFlagLoaded[];
extern JPAResourceManager* gpResourceManager;

void TMapEventSinkBianco::load(JSUMemoryInputStream& stream)
{
	TMapEventSinkInPollutionReset::load(stream);
	stream.readString();
	stream.read(&unk6C.x, 4);
	stream.read(&unk6C.y, 4);
	stream.read(&unk6C.z, 4);
	int dummy;
	stream.read(&dummy, 4);
	stream.read(&unk78, 4);

	// TODO: these are inlines
	if (gParticleFlagLoaded[0x59] == 0) {
		gpResourceManager->load("/scene/map/map/ms_objup_slope_a.jpa", 0x59);
		gParticleFlagLoaded[0x59] = 1;
	}
	if (gParticleFlagLoaded[0x1E1] == 0) {
		gpResourceManager->load("/scene/map/map/ms_objup_slope_b.jpa", 0x1E1);
		gParticleFlagLoaded[0x1E1] = 1;
	}
}

void TMapEventSinkShadowMario::rising() { TMapEventSink::rising(); }

void TMapEventSinkShadowMario::raiseBuilding(int i)
{
	unk28 = i;
	startControl();
}

void TMapEventSinkShadowMario::loadAfter()
{
	TMapEventSink::loadAfter();
	for (int i = 0; i < mBuildingNum; ++i) {
		unk64[i] = JDrama::TNameRef::search(unk68[i]);
		// TODO: what is unk64?
		getBuilding(i);
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
	unk64 = new JDrama::TNameRef*[mBuildingNum];
	unk68 = new const char*[mBuildingNum];
}
