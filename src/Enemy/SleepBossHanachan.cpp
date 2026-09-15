#include <Enemy/SleepBossHanachan.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/PacketUtil.hpp>
#include <MoveBG/Item.hpp>
#include <MoveBG/ItemManager.hpp>
#include <Strategic/MirrorActor.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Spine.hpp>
#include <System/FlagManager.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

#include <M3DUtil/InfectiousStrings.hpp>

static const char* sleepBossHanachan_bastable[2] = {
	"/scene/sleepBossHanachan/bas/demohanatyan_fall.bas", nullptr,
};

enum {
	SHINE_FLAG_UNK20000000 = 0x20000000,
	MIRROR_FLAG_HIDDEN = 1,
};

void TSleepBossHanachanManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "demohanatyan_model.bmd",
		  J3DMLF_MaterialPEFull | (1 << J3DMLF_TevStageNumShift), 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TSleepBossHanachan::init(TLiveManager* manager)
{
	initBase(manager, 3);
	mSpine->initWith(&TNerveSBH_SleepContinue::theNerve());
	initHitActor(ACTOR_TYPE_BOSS | 0x16, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	initAnmSound();
	mMActor->setBckFromIndex(1);
	setCurAnmSound();
	MsMtxSetXYZRPH(getModel()->getBaseTRMtx(), mPosition.x, mPosition.y,
	              mPosition.z, mRotation.x, mRotation.y, mRotation.z);
	mMirrorActor = new TMirrorActor("寝てるボスハナチャンin鏡");
	mMirrorActor->init(getModel(), 10);
}

void TSleepBossHanachan::calcRootMatrix() { }

const char** TSleepBossHanachan::getBasNameTable() const
{
	return sleepBossHanachan_bastable;
}

void TSleepBossHanachan::startFall(f32 x, f32 y, f32 z)
{
	TFlagManager* flagManager = TFlagManager::getInstance();
	flagManager->setBool(true, 0x5000B);
	mShinePosition.set(x, y, z);
	getMActor()->setBckFromIndex(0);
	setCurAnmSound();
	mSpine->setNext(&TNerveSBH_Fall::theNerve());
}

DEFINE_NERVE(TNerveSBH_SleepContinue, TLiveActor) { return FALSE; }

DEFINE_NERVE(TNerveSBH_Fall, TLiveActor)
{
	TSleepBossHanachan* boss = static_cast<TSleepBossHanachan*>(spine->getBody());
	JGeometry::TVec3<f32> pos;
	BOOL animationEnded = boss->getMActor()->curAnmEndsNext();
	if (animationEnded) {
		pos = boss->mShinePosition;
		TShine* shine = gpItemManager->makeShineAppearWithDemo(
		    "シャイン（ボス用）", "ボスシャインカメラ", pos.x, pos.y, pos.z);
		shine->onMapObjFlag(SHINE_FLAG_UNK20000000);
		boss->onLiveFlag(LIVE_FLAG_DEAD);
		TMirrorActor* mirror = boss->mMirrorActor;
		mirror->unk1A |= MIRROR_FLAG_HIDDEN;
		SMS_HideAllShapePacket(mirror->unk14);
		spine->pushAfterCurrent(&TNerveSBH_SleepContinue::theNerve());
		return TRUE;
	}
	return FALSE;
}
