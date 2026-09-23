#include <Enemy/MameGesso.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Player/MarioAccess.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <M3DUtil/MActor.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>

// rogue includes needed for matching sinit & bss
#include <JSystem/JAudio/JALibrary/JALModSe.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

// has to be here for rodata ordering
#include <Map/MapCollisionEntry.hpp>

f32 TMameGesso::mBaseHoseiTransY = -20.0f;

const char* mameGesso_bastable[] = {
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_land2.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_run_end2.bas",
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_run_start2.bas",
	"/scene/mameGesso/bas/mamegeso_turn1.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/mameGesso/bas/mamegeso_water_return1.bas",
};

static const char* UNUSED[] = {
	"mamegesso0", "mamegesso1", "mamegesso2", "mamegesso3", "mamegesso4",
	"mamegesso5", "mamegesso6", "mamegesso7", "mamegesso8", "mamegesso9",
};

TMameGessoSaveLoadParams::TMameGessoSaveLoadParams(const char* path)
    : TWalkerEnemyParams(path)
    , PARAM_INIT(mSLJumpWanderGravityY, 0.5f)
    , PARAM_INIT(mSLJumpWanderSpeed, 15.0f)
    , PARAM_INIT(mSLPickUpTime, 100)
    , PARAM_INIT(mSLThrownGravityY, 0.5f)
    , PARAM_INIT(mSLThrownVY, 50.0f)
    , PARAM_INIT(mSLThrownRateXZ, 0.5f)
    , PARAM_INIT(mSLGenerateInterval, 300)
    , PARAM_INIT(mSLWaitTimeOnGround, 50)
    , PARAM_INIT(mSLWaitTimeInWater, 200)
    , PARAM_INIT(mSLObjectRecoverTime, 1000)
    , PARAM_INIT(mSLObjectScale, 2.0f)
    , PARAM_INIT(mSLCollisionScale, 1.0f)
    , PARAM_INIT(mSLReturnJumpDist, 200.0f)
    , PARAM_INIT(mSLReturnJumpSp, 10.0f)
{
	TParams::load(mPrmPath);
}

TMameGessoManager::TMameGessoManager(const char* name)
    : TSmallEnemyManager(name)
{
}

void TMameGessoManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TMameGessoSaveLoadParams("/enemy/MameGesso.prm");
}

void TMameGessoManager::loadAfter() { JDrama::TNameRef::loadAfter(); }

TSmallEnemy* TMameGessoManager::createEnemyInstance() { return new TMameGesso; }

void TMameGessoManager::initSetEnemies() { }

// Binding level worth +8 of low region, landing TMameGessoManager::perform's
// frame at 0x48 (batch 124).
static inline bool MameGessoCheckLiveFlag(const TMameGesso* p, u32 i)
{
	bool liveFlag = p->checkLiveFlag(i);
	return liveFlag;
}

void TMameGessoManager::perform(u32 cue, JDrama::TGraphics* graphics)
{
	for (int i = 0; i < mObjNum; i++) {
		if (!(cue & CUE_MOVE))
			continue;
		TMameGesso* gesso = getObj(i);
		if (MameGessoCheckLiveFlag(gesso, LIVE_FLAG_DEAD) && gesso->unk1D2) {
			gesso->unk1CC += 1;
			if (gesso->unk1CC > gesso->unk194->mSLGenerateInterval.get()) {
				gesso->reset();
			}
		}
	}
	TSmallEnemyManager::perform(cue, graphics);
}

TMameGesso::TMameGesso(const char* name)
    : TWalkerEnemy(name)
    , unk1CC(0)
    , unk1D0(0)
    , unk1D1(0)
    , unk1D2(0)
    , unk1E0(0.0f)
    , unk1E4(0.0f)
    , unk1E8(0.0f)
    , unk1EC(1)
{
}

void TMameGesso::load(JSUMemoryInputStream& stream)
{
	TSpineEnemy::load(stream);
	unk1D2 = 1;
	reset();
}

void TMameGesso::init(TLiveManager* param_1)
{
	TWalkerEnemy::init(param_1);
	mActorType = 0x10000008;
	unk150     = 0x11;
	mSpine->initWith(&TNerveMameGessoGraphJumpWander::theNerve());
	unk194 = (TMameGessoSaveLoadParams*)getSaveParam();
	unk198 = new TMapCollisionMove;
	unk198->init(2, 0, 0, nullptr);
	onHitFlag(HIT_FLAG_NO_COLLISION);
	unk1D4 = mPosition;
}

void TMameGesso::moveObject()
{
	TWalkerEnemy::moveObject();
	if (mSpine->getCurrentNerve() == &TNerveMameGessoObject::theNerve()) {
		mPosition.y = unk1E4 - unk1E8;
	}
}

void TMameGesso::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
}

void TMameGesso::attackToMario()
{
	if (unk164 != 0)
		return;

	if (mSpine->getCurrentNerve() != &TNerveMameGessoObject::theNerve()) {
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
	}
}

void TMameGesso::calcRootMatrix()
{
	if (mSpine->getCurrentNerve() == &TNerveMameGessoObject::theNerve()) {
		if (!isEaten()) {
			getModel()->setBaseScale(mScaling);
			Mtx afStack_38;
			MsMtxSetRotRPH(afStack_38, mRotation.x, mRotation.y, mRotation.z);
			MtxPtr ptr       = afStack_38;
			afStack_38[0][3] = mPosition.x;
			afStack_38[1][3] = mPosition.y + unk1E0 + unk1E8;
			afStack_38[2][3] = mPosition.z;
			getModel()->setBaseTRMtx(ptr);
		}
	} else {
		TSpineEnemy::calcRootMatrix();
	}
}

void TMameGesso::rebirth() { }

void TMameGesso::reset()
{
	TWalkerEnemy::reset();

	TMsRange<int> interval(0, unk194->mSLGenerateInterval.get());

	unk1CC    = interval.rand();
	unk1D0    = 0;
	unk1E8    = 0.0f;
	unk1EC    = 1;
	mPosition = unk1D4;
	mPosition.y += 20.0f;
}

bool TMameGesso::isHitValid(u32 param_1)
{
	if (param_1 == 1)
		unk1D1 = 0;
	else
		unk1D1 = 1;

	if (mSpine->getCurrentNerve() == &TNerveMameGessoGraphJumpWander::theNerve()
	    && param_1 == 1) {
		unk1D0 = 1;
		mSpine->pushNerve(&TNerveSmallEnemyDie::theNerve());
		return false;
	}

	return true;
}

void TMameGesso::kill()
{
	if (unk184)
		if (mSpine->getCurrentNerve()
		    == &TNerveMameGessoGraphJumpWander::theNerve())
			mSpine->pushNerve(&TNerveSmallEnemyDie::theNerve());

	if (mSpine->getCurrentNerve() == &TNerveMameGessoGraphJumpWander::theNerve()
	    || mSpine->getCurrentNerve() == &TNerveMameGessoWait::theNerve()) {
		mSpine->pushNerve(&TNerveMameGessoDamage::theNerve());
		mSpine->pushNerve(&TNerveMameGessoTrample::theNerve());
	}

	if (mSpine->getCurrentNerve() == &TNerveMameGessoJitabata::theNerve())
		mSpine->pushNerve(&TNerveMameGessoTrample::theNerve());
}

f32 TMameGesso::getGravityY() const
{
	f32 result = mGravity;
	if (mSpine->getCurrentNerve() == &TNerveMameGessoObject::theNerve())
		result = unk194->mSLJumpWanderGravityY.get();

	if (mSpine->getCurrentNerve() == &TNerveMameGessoThrown::theNerve())
		result = unk194->mSLThrownGravityY.get();

	return result;
}

void TMameGesso::setWaitAnm()
{
	if (!mGroundPlane->isWaterSurface())
		setBckAnm(14);
	else
		setBckAnm(17);
	mHeadHeight = 150.0f;
}

void TMameGesso::setFreezeAnm()
{
	unk164 = 1;
	setBckAnm(15);
}

void TMameGesso::setDeadAnm() { unk164 = 1; }

void TMameGesso::behaveToWater(THitActor*)
{
	if (mSpine->getCurrentNerve() != &TNerveMameGessoGraphJumpWander::theNerve()
	    && mSpine->getCurrentNerve() != &TNerveMameGessoWait::theNerve())
		return;

	mSpine->pushNerve(&TNerveMameGessoDamage::theNerve());
}

void TMameGesso::behaveToTaken(THitActor*)
{
	if (mSpine->getCurrentNerve() != &TNerveMameGessoPickUp::theNerve())
		mSpine->pushNerve(&TNerveMameGessoPickUp::theNerve());
}

void TMameGesso::behaveToRelease()
{
	if (unk164 != 0
	    && mSpine->getCurrentNerve() != &TNerveMameGessoThrown::theNerve())
		mSpine->pushNerve(&TNerveMameGessoThrown::theNerve());
}

bool TMameGesso::isCollidMove(THitActor*)
{
	if (mSpine->getCurrentNerve() == &TNerveMameGessoGraphJumpWander::theNerve()
	    && !isAirborne())
		return false;
	else
		return true;
}

// Binding level over the head joint's matrix: with the compiler-unrolled
// loop below it lands calcObjCollision's 0x58 frame.
static inline MtxPtr MameGessoHeadMtx(TMameGesso* p)
{
	MtxPtr mtx = p->mMActor->getModel()->getAnmMtx(1);
	return mtx;
}

void TMameGesso::calcObjCollision()
{
	mHeadHeight = 50.0f;

	f32 scale = unk194->mSLCollisionScale.get();
	scale *= mAttackRadius * mBodyScale;

	JGeometry::TVec3<f32> pos;
	MtxPtr mtx = MameGessoHeadMtx(this);
	pos.x = mtx[0][3];
	pos.y = mtx[1][3];
	pos.z = mtx[2][3];

	static const f32 xzTable[8] = {
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
	};

	for (int i = 0; i < 4; i++) {
		unk19C[i] = pos;
		unk19C[i].y += 90.0f;
		unk19C[i].x += scale * xzTable[i * 2];
		unk19C[i].z += scale * xzTable[i * 2 + 1];
	}
}

void TMameGesso::entryObjCollision()
{
	unk198->setVertexData(0, unk19C[2], unk19C[1], unk19C[0]);
	unk198->setVertexData(1, unk19C[0], unk19C[3], unk19C[2]);
}

BOOL TMameGesso::isInhibitedForceMove() { return true; }

bool TMameGesso::doKeepDistance()
{
	if (isBckAnm(16))
		return true;
	else
		return false;
}

void TMameGesso::checkMarioState() { }

const char** TMameGesso::getBasNameTable() const { return mameGesso_bastable; }

// TODO: instruction-exact at retail's frame 0xe8, but every stack object sits
// 4 bytes high (the low region is one 4-byte item too big). Inert: explicit
// `!= 0`/`== 0` at every call condition; `.get()` -> `.value` on the return
// jump params is -8 each, as is `getTime() == 0`.
DEFINE_NERVE(TNerveMameGessoGraphJumpWander, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (!spine->getTime()) {
		const TBGCheckData* groundPlane = self->getGroundPlane();
		if (groundPlane->isWaterSurface())
			self->setBckAnm(12);
		else
			self->setBckAnm(11);
	}

	if (self->isBckAnm(11) || self->isBckAnm(12)) {
		if (self->checkCurAnmEnd(0))
			self->setBckAnm(10);

		if (!self->isAirborne() && self->getCurAnmFrameNo(0) > 26.0f) {
			if (self->getGroundPlane()->isWaterSurface())
				self->generateEffectColumWater();

			if (self->unk1EC) {
				self->goToShortestNextGraphNode();
				f32 jumpWanderSpeed = self->unk194->mSLJumpWanderSpeed.value;
				JGeometry::TVec3<f32> local_4c = self->calcVelocityToJumpToY(
				    self->getUnk104().getPoint(), jumpWanderSpeed,
				    self->getGravityY());
				self->mPosition.y += 10.0f;
				self->setVelocity(local_4c);
				self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			} else {
				JGeometry::TVec3<f32> local_34(
				    self->getUnk104().getPoint().x - self->mPosition.x, 0.0f,
				    self->getUnk104().getPoint().z - self->mPosition.z);

				if (local_34.x == 0.0f && local_34.y == 0.0f
				    && local_34.z == 0.0f)
					local_34.x += 1.0f;

				MsVECNormalize(&local_34, &local_34);

				f32 returnJumpDist = self->unk194->mSLReturnJumpDist.get();
				f32 returnJumpSp   = self->unk194->mSLReturnJumpSp.get();

				local_34.x = local_34.x * returnJumpDist + self->mPosition.x;
				local_34.z = local_34.z * returnJumpDist + self->mPosition.z;
				local_34.y = self->mPosition.y;

				JGeometry::TVec3<f32> vel = self->calcVelocityToJumpToY(
				    local_34, returnJumpSp, self->getGravityY());
				self->mPosition.y += 2.0f;
				self->setVelocity(vel);
				self->onLiveFlag(LIVE_FLAG_AIRBORNE);
			}
		}
	}

	if (self->unk1EC != 0) {
		if (self->isReachedToGoal()) {
			if (self->isAirborne()) {
				// Retail evaluates isWaterSurface() here and does nothing
				// with it: the guarded statement was stripped, but the
				// inlined predicate's compare chain survives (all but its
				// last term).
				if (self->getGroundPlane()->isWaterSurface()) {
				}
			}
		} else {
			if (!self->isAirborne())
				self->walkBehavior(2, 1.0f);
			else
				self->walkBehavior(3, 1.0f);
		}
	} else {
		if (self->isReachedToGoal())
			self->unk1EC = 1;

		if (self->checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			self->onLiveFlag(LIVE_FLAG_UNK20000);
			spine->setNext(&TNerveSmallEnemyDie::theNerve());
		}

		self->walkBehavior(3, 1.0f);
	}

	if (!self->isAirborne()) {
		if (self->isBckAnm(10)) {
			if (self->getGroundPlane()->isWaterSurface()) {
				self->setBckAnm(9);
				self->generateEffectColumWater();
			} else {
				self->setBckAnm(8);
			}
		} else {
			if (self->checkCurAnmEnd(0)) {
				spine->pushAfterCurrent(&TNerveMameGessoWait::theNerve());
				return true;
			}
		}
	}

	return false;
}

// Binding level worth +8 of low region, landing
// TNerveMameGessoDamage::execute's frame at 0x78 (batch 121).
static inline bool MameGessoIsAirborne(const TMameGesso* p)
{
	bool airborne = p->isAirborne();
	return airborne;
}

DEFINE_NERVE(TNerveMameGessoDamage, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		JGeometry::TVec3<f32> vel = self->getVelocity();

		vel.x = 0.0f;
		vel.z = 0.0f;

		self->setVelocity(vel);
		self->setBckAnm(13);
	}

	if (!MameGessoIsAirborne(self)
	    && self->getGroundPlane()->isWaterSurface()) {
		spine->pushAfterCurrent(&TNerveMameGessoObject::theNerve());
		return true;
	}

	if (self->checkCurAnmEnd(0)) {
		if (self->isBckAnm(13) && MameGessoIsAirborne(self))
			self->setBckAnm(0);

		if (!MameGessoIsAirborne(self)) {
			if (self->getGroundPlane()->isWaterSurface())
				spine->pushAfterCurrent(&TNerveMameGessoObject::theNerve());
			else
				spine->pushAfterCurrent(&TNerveMameGessoJitabata::theNerve());

			return true;
		}
	}
	return false;
}

// PathNode.hpp's getPoint() reaches the node's actor through getPosition();
// reading mPosition raw instead is low region here. Header round 27 measured
// the same change made in the header as a tree-wide wash, so it stays parked
// TU-locally.
static inline const JGeometry::TVec3<f32>& MameGessoGetPoint(const TPathNode& node)
{
	if (node.unk0 != 0)
		return node.unk0->mPosition;

	return node.unk4;
}

DEFINE_NERVE(TNerveMameGessoJitabata, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (spine->getTime() <= 2) {
		self->setFreezeAnm();
	} else {
		if (self->checkCurAnmEnd(0)) {
			if (self->isBckAnm(15)) {
				s32 freezeWait = self->unk194->mSLFreezeWait.value;
				if (spine->getTime() > freezeWait)
					self->setBckAnm(5);
			} else if (self->isBckAnm(5)) {
				// TODO: operator- inline is wrong here, too much stack frame
				if ((MameGessoGetPoint(self->unk104) - self->getPosition())
				        .length()
				    > 300.0f)
					self->unk1EC = 0;

				spine->pushAfterCurrent(&TNerveMameGessoWait::theNerve());
				self->unk164 = 0;
				return true;
			}
		}
	}

	return false;
}

DEFINE_NERVE(TNerveMameGessoTrample, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (spine->getTime() < 1) {
		if (self->unk1D1 != 0) {
			if (self->isBckAnm(15))
				self->setBckAnm(3);
			else
				self->setBckAnm(13);
		} else {
			if (self->isBckAnm(15))
				self->setBckAnm(3);
			else
				self->setBckAnm(4);
		}
	} else {
		if (self->checkCurAnmEnd(0))
			return true;
	}

	return false;
}

DEFINE_NERVE(TNerveMameGessoPickUp, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (spine->getTime() == 0 && self->unk164 == 0)
		return true;

	if (self->checkCurAnmEnd(0)
	    && spine->getTime() > self->getSaveLoadParam()->mSLPickUpTime.get()) {
		self->unk164 = 0;
		self->attackToMario();
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveMameGessoThrown, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		TMameGessoSaveLoadParams* params = self->getSaveLoadParam();

		// The ROM groups the throw as `rate * (power * sin)`, not
		// `rate * power * sin`: the first fmuls multiplies the power by the
		// table value and only the second brings the rate in. Named
		// power/rate/z/x with component stores put the VY load after the
		// x store as in the ROM (97.1 -> 99.8).
		// TODO: frame 0x68 against 0x70 (the ROM has 8 bytes above `vel`)
		// and power/cosine swapped between f2 and f3. Tried: every order of
		// power/rate/angle, named s16/int angle (frame -8), named sin/cos,
		// a named VY (+8 frame but `vel` 4 high), `vel` at function scope,
		// params declared after `vel`, `set`/ctor/temporary forms, raw
		// `*gpMarioAngleY`, product-returning helpers (frame +0x10..+0x20),
		// a component-setter helper.
		JGeometry::TVec3<f32> vel;
		f32 power = *gpMarioThrowPower;
		f32 rate = params->mSLThrownRateXZ.get();
		f32 z = rate * (power * JMASCos(SMS_GetMarioAngleY()));
		f32 x = rate * (power * JMASSin(SMS_GetMarioAngleY()));
		vel.x = x;
		vel.y = params->mSLThrownVY.get();
		vel.z = z;
		self->setVelocity(vel);

		self->mPosition.y += 2.0f;
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
	}

	if (!self->isAirborne()) {
		if (self->getGroundPlane()->isWaterSurface()) {
			spine->pushAfterCurrent(&TNerveMameGessoObject::theNerve());
			self->unk164 = 0;
			return true;
		}

		if (!self->isBckAnm(1))
			self->setBckAnm(1);

		if (self->checkCurAnmEnd(0) && self->isBckAnm(1)) {
			self->unk164 = 0;
			return true;
		}
	}

	return false;
}

// Binding level worth +16 of low region, landing
// TNerveMameGessoObject::execute's frame at 0x48 (batch 124).
static inline bool MameGessoIsBckAnm(const TMameGesso* p, int i)
{
	bool bckAnm = p->isBckAnm(i);
	return bckAnm;
}

DEFINE_NERVE(TNerveMameGessoObject, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (SMS_IsMarioStatusTypeSwimming()) {
		self->offHitFlag(HIT_FLAG_NO_COLLISION);
		self->unk190 = self->unk154 * 2.5f;
		self->expandCollision();
	} else {
		self->onHitFlag(HIT_FLAG_NO_COLLISION);
	}

	if (spine->getTime() == 0) {
		self->setBckAnm(2);
		self->onHitFlag(1);
		self->calcObjCollision();
		self->entryObjCollision();
		self->generateEffectColumWater();
		self->unk1E0 = 0.0f;
		self->unk1E4 = self->mGroundHeight;
		self->unk1E8 = 80.0f;
	}

	if (self->checkCurAnmEnd(0) && MameGessoIsBckAnm(self, 2))
		self->setBckAnm(16);

	if (spine->getTime() > self->unk194->mSLObjectRecoverTime.get()) {
		if (self->checkCurAnmEnd(0)) {
			if (MameGessoIsBckAnm(self, 16)) {
				self->setBckAnm(18);
			} else if (MameGessoIsBckAnm(self, 18)) {
				self->calcObjCollision();
				self->entryObjCollision();
				self->offHitFlag(HIT_FLAG_NO_COLLISION);
				self->mPosition.y = self->unk1E4;
				self->unk1E8      = 0.0f;
				spine->pushAfterCurrent(&TNerveMameGessoWait::theNerve());
				return true;
			}
		}

		if (self->unk1E0 < 0.0f)
			self->unk1E0 += 1.0f;
	} else if (self->unk1E0 > TMameGesso::mBaseHoseiTransY) {
		self->unk1E0 -= 1.0f;
	}

	if (!SMS_IsMarioStatusTypeSwimming()) {
		self->calcObjCollision();
		self->entryObjCollision();
	}

	return false;
}

// Binding level over a raw member read, worth +8 of low region in
// TNerveMameGessoWait::execute (batch 127).
static inline s16 MameGessoInstanceIndex(const TMameGesso* p)
{
	s16 instanceIndex = p->mInstanceIndex;
	return instanceIndex;
}

DEFINE_NERVE(TNerveMameGessoWait, TLiveActor)
{
	TMameGesso* self = (TMameGesso*)spine->getBody();

	if (spine->getTime() == 0)
		self->setWaitAnm();

	if (self->checkCurAnmEnd(0)) {
		int wait = self->getGroundPlane()->isWaterSurface()
		               ? self->unk194->mSLWaitTimeInWater.get()
		               : self->unk194->mSLWaitTimeOnGround.get();
		if (spine->getTime() > wait + MameGessoInstanceIndex(self) * 10) {
			spine->pushAfterCurrent(
			    &TNerveMameGessoGraphJumpWander::theNerve());
			return true;
		}
	}

	return false;
}
