#include <Enemy/HauntLeg.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const char* hauntleg_bastable[] = {
	nullptr,
	nullptr,
	nullptr,
};

THauntLegManager::~THauntLegManager() { }

THauntedObject::~THauntedObject() { }

THauntLeg::~THauntLeg() { }

THauntLegManager::THauntLegManager(const char* name)
    : TSmallEnemyManager(name)
{
	gpCurHauntLeg = nullptr;
}

void THauntLegManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TWalkerEnemyParams("/enemy/hauntLeg.prm");
}

TLiveActor* THauntLegManager::createEnemyInstance()
{
	return new THauntLeg("ゆうれい足");
}

void THauntLegManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "hauntleg.bmd", 0x11210000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

BOOL THauntedObject::receiveMessage(THitActor* sender, u32 message)
{
	if (message <= 1) {
		((TLiveActor*)mHolder)->kill();
		return TRUE;
	}
	if (message == 0xF)
		return TRUE;
	return FALSE;
}

void THauntLeg::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("hauntleg.bmd", 3);
}

void THauntLeg::reset()
{
	unk19C = nullptr;
	unk198 = 0;
	unk199 = 1;
	TWalkerEnemy::reset();
}

void THauntLeg::attackToMario()
{
	updateSquareToMario();
	if (mDistToMarioSquared < 10000.0f)
		sendAttackMsgToMario();
}

void THauntLeg::setDeadAnm()
{
	if (unk19C) {
		unk19C->receiveMessage(this, 6);
		mHolder     = nullptr;
		mHeldObject = nullptr;
	}
	unk194->mHitFlags |= HIT_FLAG_NO_COLLISION;
}

void THauntLeg::setWaitAnm() { setBckAnm(2); }

void THauntLeg::setWalkAnm() { setBckAnm(1); }

void THauntLeg::setRunAnm() { setBckAnm(1); }

void THauntLeg::setGenerateAnm() { setBckAnm(0); }

const char** THauntLeg::getBasNameTable() const { return hauntleg_bastable; }
