#include <Enemy/BossHanachan.hpp>
#include <Enemy/DemoBossHanachan.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SleepBossHanachan.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/JAudio/JALibrary/JALSystem.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JStage/JSGActor.hpp>
#include <JSystem/JStage/JSGObject.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/ObjManager.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/TakeActor.hpp>
#include <System/FlagManager.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>

void TSleepBossHanachanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "demohanatyan_model.bmd",
		  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TSleepBossHanachan::init(TLiveManager* param_1)
{
	initBase(param_1, 3);
	mSpine->initWith(&TNerveSBH_SleepContinue::theNerve());
	initHitActor(ACTOR_TYPE_SLEEP_BOSS_HANACHAN, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFilter(HIT_FILTER_NO_COLLISION);
	initAnmSound();
	getMActor()->setBckFromIndex(1);
	setCurAnmSound();
	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	               mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	unk15C = new TMirrorActor("寝てるボスハナチャンin鏡");
	unk15C->init(getModel(), 10);
}

void TSleepBossHanachan::calcRootMatrix() { }

static const char* sleepBossHanachan_bastable[2] = {
	"/scene/sleepBossHanachan/bas/demohanatyan_fall.bas",
	nullptr,
};

const char** TSleepBossHanachan::getBasNameTable() const
{
	return sleepBossHanachan_bastable;
}

void TSleepBossHanachan::startFall(f32 shine_x, f32 shine_y, f32 shine_z)
{
	TFlagManager::getInstance()->setBool(true, MSF_WIGGLER_FALLING);
	mShinePosition.set(shine_x, shine_y, shine_z);
	getMActor()->setBckFromIndex(0);
	setCurAnmSound();
	mSpine->setNext(&TNerveSBH_Fall::theNerve());
}

DEFINE_NERVE(TNerveSBH_SleepContinue, TLiveActor) { return false; }

DEFINE_NERVE(TNerveSBH_Fall, TLiveActor)
{
	TSleepBossHanachan* self = (TSleepBossHanachan*)spine->getBody();
	if (self->getMActor()->curAnmEndsNext()) {
		JGeometry::TVec3<f32> pos = self->mShinePosition;
		TShine* shine             = gpItemManager->makeShineAppearWithDemo(
            "シャイン（ボス用）", "ボスシャインカメラ", pos.x, pos.y, pos.z);
		shine->onMapObjFlag(TMapObjBase::MAP_OBJ_FLAG_UNK20000000);
		self->onLiveFlag(LIVE_FLAG_DEAD);
		TMirrorActor* mirror = self->unk15C;
		mirror->unk1A |= 1;
		SMS_HideAllShapePacket(mirror->unk14);
		spine->pushAfterCurrent(&TNerveSBH_SleepContinue::theNerve());
		return true;
	}
	return false;
}
