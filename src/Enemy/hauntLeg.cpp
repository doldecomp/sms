#include <Enemy/HauntLeg.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/TakeActor.hpp>
#include <Strategic/Strategy.hpp>
#include <Enemy/Walker.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
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
	return new THauntLeg("ハントレッグ");
}

void THauntLegManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "hauntleg.bmd", 0x10220000, 0 },
	};
	createModelDataArray(entry);
}

BOOL THauntedObject::receiveMessage(THitActor* sender, u32 message)
{
	if (message <= 1) {
		mHaunter->kill();
		return TRUE;
	}
	if (message == 0xF)
		return TRUE;
	return FALSE;
}

void THauntLeg::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);

	mActorType = 0x10000025;
	unk150     = 0x3A;
	mHitFlags |= 0x60000000;
	((TWalker*)mBinder)->setMode(1);
	unk130 = 2;
	mMActor->setJointCallback(1, HauntLegCallback);

	unk194 = new THauntedObject("ハントオブジェクト");

	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(unk194);
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

// TODO: not reconstructed. The map records 0x170 (368 bytes). The body
// rewrites the joint matrix of the leg's current node from gpCurHauntLeg's
// position and its angle field at 0x1AC, but only while the Haunt nerve is
// current; otherwise it leaves the matrix alone. Writing it needs THauntLeg's
// tail fields at 0x1A0..0x1B0 named first.
int HauntLegCallback(J3DNode* node, int param)
{
	if (param == 0 && gpCurHauntLeg != nullptr
	    && gpCurHauntLeg->mSpine->getCurrentNerve()
	           == &TNerveHauntLegHaunt::theNerve()) {
	}
	return 1;
}

BOOL THauntLeg::isCollidMove(THitActor* other)
{
	if (mSpine->getCurrentNerve() != &TNerveHauntLegHaunt::theNerve()
	    && unk198 == 0 && !(mLiveFlag & LIVE_FLAG_CLIPPED_OUT)
	    && ((other->mActorType & 0xFFFF0000) == 0x20000000
	        || (other->mActorType & 0xFFFF0000) == 0x40000000)) {
		if (((TTakeActor*)other)->mHolder == nullptr || other != unk19C) {
			unk19C = other;
			mSpine->setNext(&TNerveHauntLegHaunt::theNerve());
		}
	}
	return FALSE;
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

// TODO: not reconstructed. The map records 0x2f4 (756 bytes). Only the
// nerve's destructor is expected to match until this body is written.
DEFINE_NERVE(TNerveHauntLegHaunt, TLiveActor) { return FALSE; }
