#include <MoveBG/MapObjCorona.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <M3DUtil/MActor.hpp>
#include <Enemy/BathtubKiller.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <stdio.h>
#include <math.h>

// These game sound headers emit the lists present in this TU's static init.
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSSetSound.hpp>

TBathtubParams::TBathtubParams()
    : TParams("/MapObj/bathtub.prm")
    , PARAM_INIT(resetGrip, 0)
    , PARAM_INIT(trampleRelease, 10)
    , PARAM_INIT(trampleRecover, 10)
    , PARAM_INIT(quakeRelease, 500)
    , PARAM_INIT(quakeRecover, 500)
    , PARAM_INIT(hipdropRelease, 35)
    , PARAM_INIT(hipdropRecover, 35)
    , PARAM_INIT(breakCount0, 750)
    , PARAM_INIT(breakCount1, 710)
    , PARAM_INIT(breakCount2, 685)
    , PARAM_INIT(breakCount3, 655)
    , PARAM_INIT(launchStopCount, 1000)
    , PARAM_INIT(animSpeed0, 0.15f)
    , PARAM_INIT(animSpeed1, 0.15f)
    , PARAM_INIT(animSpeed2, 0.15f)
    , PARAM_INIT(animSpeed3, 0.15f)
    , PARAM_INIT(animSpeed4, 0.22f)
    , PARAM_INIT(shake, 0.0f)
    , PARAM_INIT(watermark, 0.3f)
    , PARAM_INIT(maxAngle, 35.0f)
    , PARAM_INIT(angleVelDamp, 0.93f)
    , PARAM_INIT(rebound, 0.0005f)
    , PARAM_INIT(shakeDamp, 0.93f)
    , PARAM_INIT(marioWeight, 0.01f)
    , PARAM_INIT(marioDropWeight, 5.0f)
    , PARAM_INIT(outerHeight, 20.0f)
{
	TParams::load(mPrmPath);
}

// Unused
TBathtubGripParts::TBathtubGripParts(const char* name, int index,
                                   TBathtubGrip* grip)
    : TLiveActor(name)
    , mGrip(grip)
    , mIndex(index)
{
}

// Unused
TBathtubGripPartsFragile::TBathtubGripPartsFragile(int index, TBathtubGrip* grip)
    : TBathtubGripParts("バスタブの足場の一部（弱点）", index, grip)
{
}

// Unused
TBathtubGripPartsHard::TBathtubGripPartsHard(int index, TBathtubGrip* grip)
    : TBathtubGripParts("バスタブの足場の一部（壊れない）", index, grip)
{
}

// TODO: The original has a larger stack frame; matrix lookup instructions match.
Mtx* TBathtubGripParts::getRootJointMtx() const
{
	int joint = mGrip->mJointIndices[mIndex];
	return (Mtx*)mGrip->getModel()->getAnmMtx(joint);
}

BOOL TBathtubGripPartsFragile::receiveMessage(THitActor* sender, u32 message)
{
	return mGrip->receiveMessage(sender, message);
}

BOOL TBathtubGripPartsHard::receiveMessage(THitActor* sender, u32 message)
{
	if (message == 3)
		message = 1;
	return mGrip->receiveMessage(sender, message);
}

void TBathtubGrip::kill()
{
	unk24A = 1;
	makeObjDead();
	removeCollisions_();
}

// Unused
void TBathtubGrip::reset()
{
	offLiveFlag(LIVE_FLAG_DEAD);
	unk248 = 0;
	unk24A = 0;
	unk249 = 1;
	unk24B = 0;
	startAnim(0);
	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
	if (ctrl) {
		ctrl->setFrame(0.0f);
		ctrl->setRate(0.0f);
	}
	unk250 = 1.0f;
	unk258 = 100;
	unk260 = 0;
}

// TODO: Match constructor register allocation and inlined initialization.
TBathtubGrip::TBathtubGrip(TBathtub* bathtub, f32 angle, MActorAnmData* anmData,
                         const char* name)
    : TMapObjBase(name)
{
	unk25C = new MActor(anmData);
	void* resource = JKRGetResource("/scene/map/map/stand_effect/stand_effect.bmd");
	u32 flags = 0x50050000;
	J3DModel* model = new J3DModel(
	    J3DModelLoaderDataBase::load(resource, flags), 0, 1);
	unk25C->setModel(model, flags);
	unk254 = 0;
	mBathtub = bathtub;
	unk24C = angle;
	initAndRegister("stand_break");
	calcRootMatrix();
	getModel()->calc();
	JUTNameTab* names = getModel()->getModelData()->getJointName();
	for (int i = 0; i < 17; ++i) {
		char jointName[0x10];
		char path[0x100];
		sprintf(jointName, "c%d", i + 1);
		sprintf(path, "/scene/mapObj/stand_break_%s.col", jointName);
		mJointIndices[i] = names->getIndex(jointName);
		mHardCollisions[i] = new TMapCollisionMove;
		mHardParts[i] = new TBathtubGripPartsHard(i, this);
		mHardCollisions[i]->init(path, 0, mHardParts[i]);
		if (i < 5) {
			sprintf(jointName, "b%d", i + 1);
			sprintf(path, "/scene/mapObj/stand_break_%s.col", jointName);
			mFragileCollisions[i] = new TMapCollisionMove;
			mFragileParts[i] = new TBathtubGripPartsFragile(i, this);
			mFragileCollisions[i]->init(path, 0, mFragileParts[i]);
		}
	}
	reset();
}

void TBathtubGrip::perform(u32 flags, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(flags, graphics);
	if (!unk260) {
		if (flags & 1) {
			MTXCopy(*getRootJointMtx(), unk25C->getModel()->getBaseTRMtx());
			if ((isCracking() || unk248)
			    && unk25C->curAnmEndsNext(0, nullptr))
				unk260 = 1;
		}
		unk25C->perform(flags, graphics);
	}
}

// Unused
bool TBathtubGrip::isCracking() const { return unk254 > 0; }

// Unused
void TBathtubGrip::startCrack()
{
	unk254 = 1;
	unk25C->setBck("stand_effect");
	unk25C->setBtk("stand_effect");
	unk25C->setBrk("stand_effect");
	J3DFrameCtrl* ctrl = unk25C->getFrameCtrl(0);
	if (ctrl)
		ctrl->setRate(0.5f * (12.0f * SMSGetAnmFrameRate()));
}

// Unused
void TBathtubGrip::startBreak(int animation, int delay, f32 speed)
{
	unk258 = delay;
	unk250 = speed;
	startCrack();
	unk248 = 1;
	unk249 = 0;
	if (animation == 0)
		animation = 1;
	else if (animation == 1)
		animation = 0;
	startAnim(animation);
}

// TODO: Match once the bathtub quake, hipdrop, and demo routines are recovered.
BOOL TBathtubGrip::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case 3: {
		if (unk248)
			return false;
		if (!unk249)
			return false;
		unk138[0] = *gpMarioPos;
		if (gpMSound->gateCheck(MSD_SE_OBJ_SUPERBLOCK_BREAK))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_OBJ_SUPERBLOCK_BREAK, &unk138[0], 0, nullptr, 0, 4);
		u16 count = mBathtub->getNumGripsDead();
		if (count == 4) {
			mBathtub->startDemo();
			return true;
		}
		mBathtub->quake(sender->mPosition);
		int delay;
		f32 speed;
		switch (count) {
		case 1:
			delay = mBathtub->unk16C->breakCount0.get();
			speed = mBathtub->unk16C->animSpeed1.get();
			break;
		case 2:
			delay = mBathtub->unk16C->breakCount1.get();
			speed = mBathtub->unk16C->animSpeed2.get();
			break;
		case 3:
			delay = mBathtub->unk16C->breakCount2.get();
			speed = mBathtub->unk16C->animSpeed3.get();
			break;
		case 4:
			delay = mBathtub->unk16C->breakCount3.get();
			speed = mBathtub->unk16C->animSpeed4.get();
			break;
		default:
			delay = mBathtub->unk16C->breakCount0.get();
			speed = mBathtub->unk16C->animSpeed0.get();
			break;
		}
		startBreak(count, delay, speed);
		return true;
	}
	case 1:
		mBathtub->hipdrop(sender->mPosition);
		return true;
	case 0:
		mBathtub->trample(sender->mPosition);
		return true;
	default:
		return false;
	}
}

Mtx* TBathtubGrip::getRootJointMtx() const
{
	return (Mtx*)getModel()->getBaseTRMtx();
}

void TBathtubGrip::calcRootMatrix() { }

// Unused
bool TBathtubGrip::marioIsOn() const { return false; }

// Unused
void TBathtubGrip::removeCollisions_()
{
	for (int i = 0; i < 17; ++i)
		mHardCollisions[i]->remove();
	for (int i = 0; i < 5; ++i)
		mFragileCollisions[i]->remove();
}

// Unused
void TBathtubGrip::setupCollisions_()
{
	for (int i = 0; i < 17; ++i) {
		mHardCollisions[i]->moveMtx(*mHardParts[i]->getRootJointMtx());
		mHardCollisions[i]->setUp();
	}
	for (int i = 0; i < 5; ++i) {
		mFragileCollisions[i]->moveMtx(*mFragileParts[i]->getRootJointMtx());
		mFragileCollisions[i]->setUp();
	}
}

// TODO: Match the stack frame used by the inlined grip helpers.
void TBathtubGrip::control()
{
	calcRootMatrix();
	TMapObjBase::control();
	if (unk24A) {
		removeCollisions_();
		return;
	}
	mMActor->calcAnm();
	if (unk24B)
		setupCollisions_();
	if (unk248) {
		if (animIsFinished()) {
			if (mBathtub->unk16C->resetGrip.get()) {
				reset();
				return;
			}
			kill();
			return;
		}
		J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
		if (ctrl)
			ctrl->setRate(0.5f * (unk250 * SMSGetAnmFrameRate()));
		((JGeometry::TPosition3<TMtx34f>*)mFragileParts[0]->getRootJointMtx())
		    ->getTrans(unk138[1]);
		if (gpMSound->gateCheck(MSD_SE_OBJ_STAND_BREAK))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_OBJ_STAND_BREAK, &unk138[1], 0, nullptr, 0, 4);
		SMSRumbleMgr->start(8, &unk138[0]);
		return;
	}
	J3DFrameCtrl* ctrl = mMActor->getFrameCtrl(0);
	ctrl->setRate(0.0f);
	if (isCracking()) {
		ctrl->setFrame(1.0f);
		if (++unk254 > unk258) {
			unk248 = 1;
			unk254 = 0;
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0xF6, *getRootJointMtx(), 0, this);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0xF7, *getRootJointMtx(), 0, this);
		}
	} else {
		ctrl->setFrame(0.0f);
	}
}

void TBathtub::loadAfter() { }

void TBathtub::hipdrop(const JGeometry::TVec3<f32>&) { }

void TBathtub::quake(const JGeometry::TVec3<f32>&) { }

int TBathtub::getNumGripsDead() const
{
	int count = 0;
	for (int i = 0; i < 5; ++i)
		if (!unk168[i]->unk249)
			++count;
	return count;
}

// Unused
void TBathtub::trample(const JGeometry::TVec3<f32>&)
{
	if (!unk29A && unk250 <= unk16C->trampleRelease.get()) {
		unk250 = unk16C->trampleRelease.get();
		unk258 = unk16C->trampleRecover.get();
		unk25C = unk16C->trampleRecover.get();
		unk254 = unk16C->hipdropRelease.get();
	}
}

// Unused
void TBathtub::liftMario(const JGeometry::TVec3<f32>&) { }

void TBathtub::tumble(f32, f32) { }

MtxPtr TBathtub::getTakingMtx()
{
	return mMActor->getModel()->getAnmMtx(mMarioJntIdx);
}

// Unused
MtxPtr TBathtub::getShineMtx() { return nullptr; }

// Unused
MtxPtr TBathtub::getShineEffectMtx() { return nullptr; }

// Unused
MtxPtr TBathtub::getWaterMtx(int) { return nullptr; }

MtxPtr TBathtub::getSubmarineMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mSubmarineJntIdx);
}

MtxPtr TBathtub::getPeachMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mDuckJntIdx);
}

// Unused
MtxPtr TBathtub::getKoopaMtxInDemo() { return nullptr; }

MtxPtr TBathtub::getKoopaJrMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mJuniorJntIdx);
}

BOOL TBathtub::receiveMessage(THitActor* sender, u32 message) { return false; }

Mtx* TBathtub::getRootJointMtx() const
{
	if (unk29A)
		return (Mtx*)getModel()->getAnmMtx(0);
	return (Mtx*)getModel()->getBaseTRMtx();
}

void TBathtub::perform(u32 cue, JDrama::TGraphics* graphics) { }

void TBathtub::control() { }

void TBathtub::calcBathtubData() { }

void TBathtub::setupCollisions_() { }

// Unused

// Unused; TODO: Recover callback behavior from the inlined demo setup.
namespace {
BOOL CameraDemoCallBack(u32, u32) { return false; }
}

void TBathtub::startDemo() { }

void TBathtub::removeCollisions_() { }

// TODO: Match the local-space position helpers and their stack layout.
bool TBathtub::allowsTumble() const
{
	JGeometry::TVec3<f32> pos = *gpMarioPos;
	f32 angle;
	if (getNearGrip(pos, 18.0f, &angle)) {
		JGeometry::TVec3<f32> relative;
		relative.sub(pos, mBathtubData.mPos);
		JGeometry::TVec3<f32> local;
		mBathtubData.unk18.mult(relative, local);
		local.y = 0.0f;
		f32 distance = local.length();
		if (distance < 4200.0f)
			return false;
		if (distance > 4700.0f) {
			if (mBathtubData.unk18.at(1, 1) > 0.99f) {
				TBathtubKillerManager* manager
				    = JDrama::TNameRefGen::search<TBathtubKillerManager>(
				        "バスタブキラーマネージャー");
				u32 status = SMS_GetMarioStatus();
				if (status == MARIO_STATUS_HIP_DROP)
					return false;
				if (status == MARIO_STATUS_ROCKET)
					return false;
				if (status == MARIO_STATUS_ROCKET_LANDING)
					return false;
				const TWaterGun* gun = gpMarioOriginal->mWaterGun;
				if (gun) {
					TNozzleTrigger* nozzle = (TNozzleTrigger*)gun->getCurrentNozzle();
					if (nozzle && nozzle->getNozzleKind() == 1
					    && nozzle->unk388 > 0.0f)
						return false;
				}
				return manager->countActiveKillers() == 0;
			}
			return false;
		}
		return true;
	}
	return false;
}

void TBathtub::calcRootMatrix() { }

// Unused; TODO: Recover quaternion and direction helpers from their callers.
void QuatRotate(JGeometry::TQuat4<f32>&, const JGeometry::TVec3<f32>&) { }

namespace {
void getDir(MtxPtr, const JGeometry::TVec3<f32>&) { }

void getDir(MtxPtr, const JGeometry::TVec3<f32>&,
            const JGeometry::TVec3<f32>&) { }
}

// Unused
u8 TBathtub::getNearJuncture(const JGeometry::TVec3<f32>&) const { return 0; }

// TODO: Match the matrix projection helpers and local stack layout.
bool TBathtub::getNearGrip(const JGeometry::TVec3<f32>& pos, f32 tolerance,
                          f32* gripAngle) const
{
	const TPosition3f& matrix = *(TPosition3f*)getRootJointMtx();
	JGeometry::TVec3<f32> x, z, origin, relative;
	matrix.getXDir(x);
	matrix.getZDir(z);
	matrix.getTrans(origin);
	relative.sub(pos, origin);
	f32 angle = (360.0f / 65536.0f) * matan(z.dot(relative), x.dot(relative));
	f32 nearest = 360.0f;
	int index = 0;
	for (int i = 0; i < 5; ++i) {
		f32 difference = fabsf(-180.0f + std::fmodf(
		    360.0f + ((unk150[i] - angle) - -180.0f), 360.0f));
		if (difference < nearest) {
			index = i;
			nearest = difference;
		}
	}
	if (nearest < tolerance) {
		*gripAngle = unk150[index];
		return true;
	}
	return false;
}

u8 TBathtub::getNextJuncture(const JGeometry::TVec3<f32>&,
                             const JGeometry::TVec3<f32>&) const
{
	return 0;
}

u8 TBathtub::getNextGrip(const JGeometry::TVec3<f32>&,
                         const JGeometry::TVec3<f32>&, f32, f32*) const
{
	return 0;
}

// Unused
void TBathtub::showMessage(u32) { }

void TBathtub::updatePosture_() { }

void TBathtub::load(JSUMemoryInputStream&) { }

// TODO: The original calls SMatrix33R's constructor for mBathtubData.unk18.
// Correct that shared matrix type together with its water-physics consumers.
TBathtub::TBathtub(const char* name)
    : TMapObjBase(name)
    , unk164(nullptr)
    , unk290(0)
{
	unk16C = new TBathtubParams;
	unk1D8 = 0.0f;
	unk1DC = 0.0f;
	unk1E0 = 0.0f;
	unk1E4 = 1.0f;
	mPosition.x = mPosition.y = mPosition.z = 0.0f;
	unk1E8 = unk1EC = unk1F0 = 0.0f;
	unk250 = 0;
	unk254 = 1;
	unk258 = 0;
	unk25C = 1;
	unk248 = 0;
	unk298 = 0;
	unk23C = unk240 = unk244 = 0.0f;
	unk299 = 0;
	unk29A = 0;
	unk2A0 = 0;
	unk294 = 0;
}

// Unused
bool TBathtub::isKillerLaunchable() const { return false; }

int TBathtub::getNumKillerLaunchable() const { return 0; }

bool TBathtub::isKillerAttackable() const { return false; }

// Unused
bool TBathtub::isBreaking() const { return false; }

int TBathtub::getNumKillerBurstable() const { return 0; }

TBathtub::~TBathtub() { }
