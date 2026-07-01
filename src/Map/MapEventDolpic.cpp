#include <Map/MapEventDolpic.hpp>
#include <Map/Map.hpp>
#include <Map/MapModel.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Camera/CameraShake.hpp>
#include <Player/MarioAccess.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <string.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

bool TDolpicEventBiancoGate::isFinishedAll() const
{
	if (stateIs(0))
		return true;
	return false;
}

bool TDolpicEventBiancoGate::control()
{
	unk20->mPosition.y += unk24;
	gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 1.0f);
	SMSRumbleMgr->start(0, (f32*)nullptr);
	{
		JGeometry::TVec3<f32>& pos = unk20->mPosition;
		gpPollution->clean(pos.x, pos.y, pos.z, 10000.0f);
	}
	if (unk20->mPosition.y >= unk20->mInitialPosition.y) {
		unk20->mPosition.y = unk20->mInitialPosition.y;
		unk20->setUpMapCollision(0);
		unk18 = 0;
		return true;
	}
	return false;
}

bool TDolpicEventBiancoGate::watch()
{
	if (TFlagManager::getInstance()->getBool(0x10384)) {
		unk20->appear();
		unk20->setUpMapCollision(1);
		return true;
	}
	return false;
}

void TDolpicEventBiancoGate::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	unk20 = JDrama::TNameRefGen::search<TMapObjBase>("dptKing");
	unk20->kill();
	unk20->mPosition.y -= 1800.0f;
}

TDolpicEventBiancoGate::TDolpicEventBiancoGate(const char* name)
    : TMapEvent(name)
    , unk20(nullptr)
    , unk24(3.0f)
{
}

bool TDolpicEventRiccoMammaGate::isFinishedAll() const
{
	if (stateIs(0))
		return true;
	return false;
}

void TDolpicEventRiccoMammaGate::rising()
{
	f32 scale = TMapObjBase::getJointScaleY(unk20) + unk34;

	TPosition3f mtx;
	mtx.identity();
	mtx.ref(1, 1) = scale;
	unk24->moveMtx(mtx.mMtx);

	gpCameraShake->keepShake(CAM_SHAKE_MODE_UNK5, 1.0f);
	SMSRumbleMgr->start(0, (f32*)nullptr);
	TMapObjBase::setJointScaleY(unk20, scale);
	TMapObjBase::setJointTransY(unk20, 300.0f * (1.0f - scale));
	gpMap->getRootJointModel()->getModel()->calc();
}

bool TDolpicEventRiccoMammaGate::control()
{
	if (unk44 < unk38 - unk3C && unk44 > unk40)
		rising();

	if (unk44 > unk40) {
		SMSRumbleMgr->start(0x13, (f32*)nullptr);
		gpMSound->startSoundActor(MSD_SE_OBJ_QUAKE, &unk48, 0, nullptr, 0, 4);
	}

	if (unk44 > 0) {
		unk44 -= 1;
		return false;
	}

	TMapObjBase::setJointScaleY(unk20, 1.0f);
	unk24->remove();
	unk28->setUp();
	gpMarDirector->fireEndDemoCamera();
	MSBgm::setTrackVolume(0, 1.0f, 15, 0);
	unk18 = 0;
	return true;
}

bool TDolpicEventRiccoMammaGate::watch()
{
	if (!TFlagManager::getInstance()->getBool(unk2C)) {
		SMS_ShowJoint(unk20->getMesh(), true);
		TMapObjBase::setJointScaleY(unk20, unk34);

		TPosition3f mtx;
		mtx.identity();
		mtx.ref(1, 1) = unk34;
		(void)&mtx; // unused?!

		unk24->setUp();

		unk44 = unk38;

		if (unk2C == 0x50001) {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "マニ屋上げデモカメラ", &unk48, -1, 0.0f, false, nullptr, 0,
			    nullptr, JDrama::TFlagT<u16>(0));
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_MANIYA_A, &unk48, 0,
			                             this);
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_MANIYA_B, &unk48, 2,
			                             this);
			gpPollution->getLayer(0)->startDecay();
		} else {
			SMSGetMarDirector()->fireStartDemoCamera(
			    "灯台上げデモカメラ", &unk48, -1, 0.0f, false, nullptr, 0,
			    nullptr, JDrama::TFlagT<u16>(0));
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_TOUDAI_A, &unk48, 0,
			                             this);
			gpMarioParticleManager->emit(MAP_MAP_MS_OBJUP_TOUDAI_B, &unk48, 2,
			                             this);
			gpPollution->getLayer(1)->startDecay();
		}

		SMS_MarioWarpRequest(unk54, unk60);
		return true;
	}

	return false;
}

void TDolpicEventRiccoMammaGate::loadAfter()
{
	JDrama::TNameRef::loadAfter();
	unk38 = 720;
	unk3C = 120;
	unk40 = 120;
	unk34 = 1.0f / (f32)((unk38 - unk3C) - unk40);
	if (unk30) {
		unk28->setUp();
		unk18 = 0;
		if (unk2C == 0x50001)
			gpPollution->getCounterLayer().offLayer(0);
		else
			gpPollution->getCounterLayer().offLayer(1);
	}
}

void TDolpicEventRiccoMammaGate::load(JSUMemoryInputStream& stream)
{
	TMapEvent::load(stream);
	stream.readString();
	stream >> unk54.x >> unk54.y >> unk54.z;
	f32 unused;
	stream >> unused;
	stream >> unk60;

	int idx;
	if (strcmp("イベント（リコゲート）", getName()) == 0) {
		unk2C = 0x50001; // the tanuki house
		idx   = 0;
	} else {
		unk2C = 0x50002; // the lighthouse
		idx   = 1;
	}

	unk24 = TMapObjBase::newAndInitBuildingCollisionMove(idx + 1, nullptr);
	unk28 = TMapObjBase::newAndInitBuildingCollisionWarp(idx + 1, nullptr);
	if (TFlagManager::getInstance()->getBool(unk2C)) {
		unk20 = getBuilding(idx + 1)->getJoint();
		TMapObjBase::setJointScaleY(unk20, 0.008f);
		TMapObjBase::setJointTransY(unk20, 295.0f);
		gpMap->getRootJointModel()->getModel()->calc();
		if (unk2C == 0x50001) {
			unk48.x = -10500.0f;
			unk48.y = 300.0f;
			unk48.z = 2003.0f;
			SMS_LoadParticle("/scene/map/map/ms_objup_maniya_a.jpa",
			                 MAP_MAP_MS_OBJUP_MANIYA_A);
			SMS_LoadParticle("/scene/map/map/ms_objup_maniya_b.jpa",
			                 MAP_MAP_MS_OBJUP_MANIYA_B);
		} else {
			unk48.x = 9915.0f;
			unk48.y = 300.0f;
			unk48.z = -7565.0f;
			SMS_LoadParticle("/scene/map/map/ms_objup_toudai_a.jpa",
			                 MAP_MAP_MS_OBJUP_TOUDAI_A);
			SMS_LoadParticle("/scene/map/map/ms_objup_toudai_b.jpa",
			                 MAP_MAP_MS_OBJUP_TOUDAI_B);
		}
		unk30 = false;
	} else {
		unk30 = true;
	}
}

TDolpicEventRiccoMammaGate::TDolpicEventRiccoMammaGate(const char* name)
    : TMapEvent(name)
    , unk20(nullptr)
    , unk24(nullptr)
    , unk28(nullptr)
    , unk2C(0)
    , unk30(false)
    , unk34(0.0f)
    , unk38(0)
    , unk3C(0)
    , unk40(0)
    , unk44(0)
    , unk60(0.0f)
    , unk54(0.0f, 0.0f, 0.0f)
    , unk48(0.0f, 0.0f, 0.0f)
{
}
