#include <Enemy/RiccoHook.hpp>
#include <Strategic/Spine.hpp>
#include <Enemy/Graph.hpp>
#include <JSystem/JMath.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

MtxPtr THookTake::getTakingMtx() { return nullptr; }

f32 THookTake::getRadiusAtY(f32 y) const
{
	return mOwner->getSaveLoadParam()->mSLHangRadius.get();
}

BOOL THookTake::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->mActorType == 0x80000001) {
		if (message == 5) {
			mHeldObject = (TTakeActor*)sender;
			return TRUE;
		}

		if (message == 7 || message == 8) {
			mHeldObject = nullptr;
			return TRUE;
		}
	}

	return FALSE;
}

void THookTake::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	if ((param_1 & 1) != 0) {
		mPosition = mOwner->getPosition();
		mPosition.y -= 900.0f;
	}

	THitActor::perform(param_1, param_2);

	if ((param_1 & 1) != 0 && mHeldObject != nullptr) {
		moveHeldObject();
	}
}

// @non-matching -- the issue seems to stem from the JDrama TNameRefGen
// search/push_back calls.
THookTake::THookTake(TRiccoHook* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
{
	initHitActor(0x400000BB, 1, -0x80000000,
	             mOwner->getSaveLoadParam()->mSLHitRadius.get(),
	             mOwner->getSaveLoadParam()->mSLHitHeight.get(),
	             mOwner->getSaveLoadParam()->mSLHitRadius.get(),
	             mOwner->getSaveLoadParam()->mSLHitHeight.get());

	JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")
	    ->getChildren()
	    .push_back(this);
}

TRiccoHook::TRiccoHook(const char* name)
    : TSpineEnemy(name)
    , mHookTake(nullptr)
    , mTimer(0)
{
}

// @non-matching - stack issues, maybe caused by THookTake ctor?
void TRiccoHook::init(TLiveManager* manager)
{
	TSpineEnemy::init(manager);
	mSpine->initWith(&TNerveRHGraphWander::theNerve());
	onHitFlag(HIT_FLAG_NO_COLLISION);
	mHookTake = new THookTake(this);
	unk124->reset();
	goToShortestNextGraphNode();
	mMarchSpeed = getSaveLoadParam()->mSLMoveSpeed.get();
	mTurnSpeed  = 10.0f;
	onLiveFlag(LIVE_FLAG_UNK10);
}

void TRiccoHook::kill() { }

BOOL TRiccoHook::receiveMessage(THitActor* sender, u32 message)
{
	return FALSE;
}

void TRiccoHook::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	TSpineEnemy::perform(param_1, param_2);
	mHookTake->perform(param_1, param_2);
	if ((param_1 & 1) && mTimer > 0) {
		mTimer--;
	}
}

THookParams::THookParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLHitHeight, 900.0f)
    , PARAM_INIT(mSLHitRadius, 120.0f)
    , PARAM_INIT(mSLHangRadius, 30.0f)
    , PARAM_INIT(mSLMoveSpeed, 4.0f)
{
	TParams::load(mPrmPath);
}

TRiccoHookManager::TRiccoHookManager(const char* name)
    : TEnemyManager(name)
{
}

void TRiccoHookManager::createModelData()
{
	static const TModelDataLoadEntry entry[2]
	    = { { "riccohook.bmd", 0x10000000, 0 }, { 0 } };
	createModelDataArray(entry);
}

void TRiccoHookManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new THookParams("/enemy/riccohook.prm");
	TEnemyManager::load(stream);
}

TSpineEnemy* TRiccoHookManager::createEnemyInstance() { return nullptr; }

// Same "fake" inline as seen in walkerEnemy.cpp's moveObject func
// Can't find any other way to get the * 1.0f's to emit
static inline JGeometry::TVec3<f32> polarXZ(f32 theta, f32 radius)
{
	f32 c = radius * JMACos(theta);
	f32 s = radius * JMASin(theta);
	return JGeometry::TVec3<f32>(s, 0.0f, c);
}

DEFINE_NERVE(TNerveRHGraphWander, TLiveActor)
{
	TRiccoHook* self = (TRiccoHook*)spine->getBody();

	if (spine->getTime() == 0) {
		f32 y                        = self->getRotation().y;
		JGeometry::TVec3<f32>& polar = polarXZ(y, 1.0f);

		self->goToDirectedNextGraphNode(polar);
	}

	if (vecdist(self->unk104.getPoint(), self->getPosition()) < 10.0f) {
		TGraphNode& node = self->unk124->getCurrent();

		if (node.checkFlag(0x800)) {
			self->mTimer = node.getRailNode()->mPitch;
		}

		spine->pushAfterCurrent(&TNerveRHGraphWander::theNerve());
		return true;
	} else {
		if (self->unk124->unk0 == nullptr || self->unk124->unk0->isDummy()) {
			return false;
		}

		if (self->mTimer > 0) {
			return false;
		}

		JGeometry::TVec3<f32> dPos = self->getUnkF4().getPoint();
		dPos.sub(self->getPosition());
		PSVECNormalize(&dPos, &dPos);
		dPos.scale(self->getMarchSpeed());
		self->mPosition.add(dPos);
		return false;
	}
}
