#include <MoveBG/MapObjCorona.hpp>
#include <System/DummyStrings.hpp>
#include <GC2D/GCConsole2.hpp>
#include <System/MarDirector.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Map/MapCollisionManager.hpp>
#include <Camera/CameraShake.hpp>
#include <Enemy/BathtubKiller.hpp>
#include <Enemy/Koopa.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/JMath.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoader.hpp>
#include <JSystem/JKernel/JKRFileLoader.hpp>
#include <JSystem/JUtility/JUTNameTab.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/MapCollisionEntry.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/WaterGun.hpp>
#include <System/Application.hpp>
#include <System/Particles.hpp>
#include <stdio.h>
#include <math.h>

// These game sound headers emit the lists present in this TU's static init.
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

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

// Binding level over a raw member read, worth +16 of low region in
// TBathtubGripParts::getRootJointMtx (batch 127).
static inline TBathtubGrip* MapObjCoronaGrip(const TBathtubGripParts* p)
{
	TBathtubGrip* grip = p->mGrip;
	return grip;
}

// TODO: The original has a larger stack frame; matrix lookup instructions match.
Mtx* TBathtubGripParts::getRootJointMtx() const
{
	int joint = MapObjCoronaGrip(this)->mJointIndices[mIndex];
	return (Mtx*)MapObjCoronaGrip(this)->getModel()->getAnmMtx(joint);
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

static inline J3DModel* BathtubGripModel(TBathtubGrip* self)
{
	return self->getModel();
}

// TODO: Match constructor register allocation and inlined initialization.
TBathtubGrip::TBathtubGrip(TBathtub* bathtub, f32 angle, MActorAnmData* anmData,
                         const char* name)
    : TMapObjBase(name)
{
	unk25C = new MActor(anmData);
	void* resource = JKRGetResource("/scene/map/map/stand_effect/stand_effect.bmd");
	u32 flags = 0x50050000;
	unk25C->setModel(new J3DModel(
	    J3DModelLoaderDataBase::load(resource, flags), 0, 1), flags);
	unk254 = 0;
	mBathtub = bathtub;
	unk24C = angle;
	initAndRegister("stand_break");
	calcRootMatrix();
	getModel()->calc();
	JUTNameTab* names = BathtubGripModel(this)->getModelData()->unkB0;
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
		unk138[0].set(*gpMarioPos);
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

// TODO: JGeometry::TMatrix34<T>::concat(a, b) in JGMatrix34.hpp has its
// indices transposed (a known open header item), so the correct body is
// parked here, as this batch may not edit that header. Once concat is fixed
// this becomes dst.concat(a, b).
// Fabricated, but the ROM's shape: every angle comparison in this TU computes
// `l + std::fmodf((r - l) + (t - l), r - l)` with l = -180, r = 180, and the
// two inline levels above std::fmodf are what make MWCC call the weak 0x5c
// copy the map records for this TU instead of expanding it. Same pair as
// koopajr.cpp's WrapDirectionF / WrapRadianF.
static inline f32 WrapDirectionF(f32 t, f32 l, f32 r)
{
	return l + std::fmodf((r - l) + (t - l), r - l);
}

static inline f32 WrapAngleDiffF(f32 a, f32 b)
{
	return WrapDirectionF(a - b, -180.0f, 180.0f);
}

static inline void ConcatMtx34(TSMtx34f& dst, const TSMtx34f& a,
    const TSMtx34f& b)
{
	dst.set(
	    // clang-format off
	    a.at(0, 0) * b.at(0, 0) + a.at(0, 1) * b.at(1, 0) + a.at(0, 2) * b.at(2, 0),
	    a.at(0, 0) * b.at(0, 1) + a.at(0, 1) * b.at(1, 1) + a.at(0, 2) * b.at(2, 1),
	    a.at(0, 0) * b.at(0, 2) + a.at(0, 1) * b.at(1, 2) + a.at(0, 2) * b.at(2, 2),
	    a.at(0, 3) + (a.at(0, 0) * b.at(0, 3) + a.at(0, 1) * b.at(1, 3) + a.at(0, 2) * b.at(2, 3)),

	    a.at(1, 0) * b.at(0, 0) + a.at(1, 1) * b.at(1, 0) + a.at(1, 2) * b.at(2, 0),
	    a.at(1, 0) * b.at(0, 1) + a.at(1, 1) * b.at(1, 1) + a.at(1, 2) * b.at(2, 1),
	    a.at(1, 0) * b.at(0, 2) + a.at(1, 1) * b.at(1, 2) + a.at(1, 2) * b.at(2, 2),
	    a.at(1, 3) + (a.at(1, 0) * b.at(0, 3) + a.at(1, 1) * b.at(1, 3) + a.at(1, 2) * b.at(2, 3)),

	    a.at(2, 0) * b.at(0, 0) + a.at(2, 1) * b.at(1, 0) + a.at(2, 2) * b.at(2, 0),
	    a.at(2, 0) * b.at(0, 1) + a.at(2, 1) * b.at(1, 1) + a.at(2, 2) * b.at(2, 1),
	    a.at(2, 0) * b.at(0, 2) + a.at(2, 1) * b.at(1, 2) + a.at(2, 2) * b.at(2, 2),
	    a.at(2, 3) + (a.at(2, 0) * b.at(0, 3) + a.at(2, 1) * b.at(1, 3) + a.at(2, 2) * b.at(2, 3))
	    // clang-format on
	);
}

void TBathtubGrip::calcRootMatrix()
{
	MtxPtr dst = getModel()->getBaseTRMtx();
	JGeometry::TPosition3<TMtx34f> local;
	MsMtxSetRotRPH(local, 0.0f, unk24C, 0.0f);
	local.setTrans(0.0f, 0.0f, 0.0f);
	ConcatMtx34(*(TSMtx34f*)dst, *(TSMtx34f*)mBathtub->getRootJointMtx(),
	    local);
}

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

// Binding level over a raw member read, worth +16 of low region in
// TBathtubGrip::control (batch 127).
static inline TBathtub* MapObjCoronaBathtub(const TBathtubGrip* p)
{
	TBathtub* bathtub = p->mBathtub;
	return bathtub;
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
			if (MapObjCoronaBathtub(this)->unk16C->resetGrip.get()) {
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

void TBathtub::loadAfter()
{
	SMS_LoadParticle("/scene/map/map/ms_lkp_yuge1.jpa", 0x1BE);
	SMS_LoadParticle("/scene/map/map/ms_kp_funsui.jpa", 0x1BF);
	SMS_LoadParticle("/scene/map/map/ms_kp_break_a.jpa", 0xF6);
	SMS_LoadParticle("/scene/map/map/ms_kp_break_b.jpa", 0xF7);
}

void TBathtub::hipdrop(const JGeometry::TVec3<f32>& pos)
{
	if (unk29A)
		return;
	else if (unk250 > getUnk16C()->hipdropRelease.get())
		return;
	// The direction is thrown away in the shipped build, as in quake().
	// The `else if` and the named `init` cost no code but carry this body
	// past MWCC's depth-1 inline budget, which is what keeps it a call from
	// both receiveMessage bodies, as retail has it.
	JGeometry::TVec3<f32> dir;
	const JGeometry::TVec3<f32>& init = getInitialPosition();
	dir.sub(pos, init);
	dir.y = 0.0f;
	dir.normalize();
	unk250 = getUnk16C()->hipdropRelease.get();
	unk258 = getUnk16C()->hipdropRecover.get();
	unk25C = getUnk16C()->hipdropRecover.get();
	unk254 = getUnk16C()->hipdropRelease.get();
	JDrama::TNameRefGen::search<TKoopa>("\x83\x4e\x83\x62\x83\x70")
	    ->stagger(false);
}

// Binding level worth +8 of low region, landing TBathtub::quake's frame at
// 0xa0 (batch 121).
static inline const JGeometry::TVec3<f32>&
MapObjCoronaGetInitialPosition(const TBathtub* p)
{
	const JGeometry::TVec3<f32>& initialPosition = p->getInitialPosition();
	return initialPosition;
}

void TBathtub::quake(const JGeometry::TVec3<f32>& pos)
{
	if (unk29A)
		return;
	// The direction is thrown away in the shipped build; only the inlined
	// inv_sqrt guard survives.
	// TODO: body exact, frame 0x98 against retail's 0xa0 (one 8-byte local).
	JGeometry::TVec3<f32> dir;
	dir.sub(pos, MapObjCoronaGetInitialPosition(this));
	dir.y = 0.0f;
	dir.normalize();
	unk24C = 300;
	unk250 = unk16C->quakeRelease.get();
	unk258 = unk16C->quakeRecover.get();
	unk25C = unk16C->quakeRecover.get();
	unk254 = unk16C->hipdropRelease.get();
	unk248 = unk16C->launchStopCount.get();
	TKoopa* koopa = JDrama::TNameRefGen::search<TKoopa>("\x83\x4e\x83\x62\x83\x70");
	gpCameraShake->startShake((EnumCamShakeMode)0x25, 1.0f);
	gpCameraShake->startShake((EnumCamShakeMode)0x26, 1.0f);
	SMSRumbleMgr->start(4, (f32*)nullptr);
	SMS_ThrowMario(JGeometry::TVec3<f32>(0.0f, 1.0f, 0.0f), 10.0f);
	koopa->getDown();
}

int TBathtub::getNumGripsDead() const
{
	int count = 0;
	for (int i = 0; i < 5; ++i)
		if (!unk168[i]->unk249)
			++count;
	return count;
}

// Unused
// TODO: inlined into receiveMessage, retail keeps &trampleRelease.value in r5
// and rereads the threshold through it for the store; a named const s32&
// (via get() or .value) and the guard-return shape do not reproduce it.
void TBathtub::trample(const JGeometry::TVec3<f32>&)
{
	if (!unk29A && unk250 <= unk16C->trampleRelease.value) {
		unk250 = unk16C->trampleRelease.value;
		unk258 = unk16C->trampleRecover.value;
		unk25C = unk16C->trampleRecover.value;
		unk254 = unk16C->hipdropRelease.value;
	}
}

// Unused
void TBathtub::liftMario(const JGeometry::TVec3<f32>&) { }

void TBathtub::tumble(f32 angle, f32 power)
{
	if (unk29A)
		return;
	f32 amount = power;
	amount *= 0.0001f;
	mAngleVel.add(JGeometry::TVec3<f32>(amount * JMACos(angle), 0.0f,
	    amount * -JMASin(angle)));
}

MtxPtr TBathtub::getTakingMtx()
{
	return mMActor->getModel()->getAnmMtx(mMarioJntIdx);
}

// Unused
MtxPtr TBathtub::getShineMtx()
{
	return mMActor->getModel()->getAnmMtx(mStarJntIdx);
}

// Unused
MtxPtr TBathtub::getShineEffectMtx()
{
	return unk29C->getModel()->getAnmMtx(mShineBodyJntIdx);
}

// Unused
MtxPtr TBathtub::getWaterMtx(int index)
{
	return mMActor->getModel()->getAnmMtx(mWaterJntIdx[index]);
}

MtxPtr TBathtub::getSubmarineMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mSubmarineJntIdx);
}

MtxPtr TBathtub::getPeachMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mDuckJntIdx);
}

// Unused
MtxPtr TBathtub::getKoopaMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mKoopaJntIdx);
}

MtxPtr TBathtub::getKoopaJrMtxInDemo()
{
	return mMActor->getModel()->getAnmMtx(mJuniorJntIdx);
}

// TODO: retail calls hipdrop() from both hip-drop cases and only inlines
// trample(); we inline all three. hipdrop is instruction-exact on its own, so
// the difference is the per-caller inline budget, not its body.
BOOL TBathtub::receiveMessage(THitActor* sender, u32 message)
{
	switch (message) {
	case HIT_MESSAGE_HIP_DROP:
		hipdrop(*gpMarioPos);
		return true;
	case HIT_MESSAGE_SUPER_HIP_DROP:
		hipdrop(*gpMarioPos);
		return true;
	case HIT_MESSAGE_TRAMPLE:
		trample(*gpMarioPos);
		return true;
	case HIT_MESSAGE_PUSH_UP:
	default:
		return false;
	}
}

Mtx* TBathtub::getRootJointMtx() const
{
	if (unk29A)
		return (Mtx*)getModel()->getAnmMtx(0);
	return (Mtx*)getModel()->getBaseTRMtx();
}

// Named-local forks over the director and the shine actor: +0x60 and +8
// per site, landing TBathtub::perform's 0x168 frame (the shine fork at six
// of its seven sites; any six land it).
static inline TMarDirector* MapObjCoronaDirector()
{
	TMarDirector* d = gpMarDirector;
	return d;
}

static inline MActor* MapObjCoronaShine(const TBathtub* p)
{
	MActor* a = p->unk29C;
	return a;
}

void TBathtub::perform(u32 cue, JDrama::TGraphics* graphics)
{
	TMapObjBase::perform(cue, graphics);
	u32 calcCue = cue & 1;
	if (calcCue) {
		MTXCopy(mMActor->getModel()->getAnmMtx(mStarJntIdx),
		    MapObjCoronaShine(this)->getModel()->getBaseTRMtx());
		MapObjCoronaShine(this)->getModel()->setBaseScale(
		    JGeometry::TVec3<f32>(3.0f, 3.0f, 3.0f));
	}
	if (calcCue) {
		int time = MapObjCoronaDirector()->unk58;
		switch (getNumGripsDead()) {
		case 0:
			if (time >= 7200) {
				if (!(unk2A0 & 0x2))
					MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x1F, true);
				unk2A0 |= 0x2;
			} else if (time >= 3600) {
				if (!(unk2A0 & 0x1))
					MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x1E, true);
				unk2A0 |= 0x1;
			}
			break;
		case 1:
			if (!(unk2A0 & 0x4))
				MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x20, true);
			unk2A0 |= 0x4;
			break;
		case 2:
			if (!(unk2A0 & 0x8))
				MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x21, true);
			unk2A0 |= 0x8;
			break;
		case 3:
			if (!(unk2A0 & 0x10))
				MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x22, true);
			unk2A0 |= 0x10;
			break;
		case 4:
			if (!(unk2A0 & 0x8000))
				MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x2D, true);
			unk2A0 |= 0x8000;
			break;
		case 5:
			if (!(unk2A0 & 0x20))
				MapObjCoronaDirector()->getConsole()->startAppearBalloon(0x23, true);
			unk2A0 |= 0x20;
			break;
		}
	}
	if (cue & 0x2)
		unk29C->calc();
	if (cue & 0x4)
		MapObjCoronaShine(this)->viewCalc();
	if (cue & 0x200) {
		MtxPtr mtx = MapObjCoronaShine(this)->getModel()->getBaseTRMtx();
		JGeometry::TVec3<f32> pos;
		pos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
		MapObjCoronaShine(this)->setLightData(mGroundPlane, pos);
		MapObjCoronaShine(this)->entry();
	}
}

void TBathtub::control()
{
	if (unk29A) {
		if (getMActor()->curAnmEndsNext(0, nullptr)) {
			switch (unk294++) {
			case 0:
				startBck("bath_overturn2");
				break;
			case 1:
				startBck("bath_overturn3");
				break;
			}
		}
		JPABaseEmitter* shineEmitter
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0x128,
		        unk29C->getModel()->getAnmMtx(mShineBodyJntIdx), 1, this);
		if (shineEmitter)
			shineEmitter->setGlobalScale(
			    JGeometry::TVec3<f32>(3.0f, 3.0f, 3.0f));
		JPABaseEmitter* shineEmitter2
		    = gpMarioParticleManager->emitAndBindToMtxPtr(0x129,
		        unk29C->getModel()->getAnmMtx(mShineBodyJntIdx), 1, this);
		if (shineEmitter2)
			shineEmitter2->setGlobalScale(
			    JGeometry::TVec3<f32>(3.0f, 3.0f, 3.0f));

		MtxPtr starMtx = getMActor()->getModel()->getAnmMtx(mStarJntIdx);
		unk200.set(starMtx[0][3], starMtx[1][3], starMtx[2][3]);
		if (gpMSound->gateCheck(MSD_SE_SHINE_EXIST))
			MSoundSESystem::MSoundSE::startSoundActor(
			    MSD_SE_SHINE_EXIST, &unk200, 0, nullptr, 0, 4);
		calcRootMatrix();
		calcBathtubData();
		for (int i = 0; i < 30; ++i)
			unk164[i]->remove();
		TTakeActor* mario = (TTakeActor*)SMS_GetMarioHitActor();
		if (mario->receiveMessage(this, HIT_MESSAGE_TAKE))
			mHeldObject = mario;
		gpMarioOriginal->mFaceAngle.y = 0x7FFF;
		if (unk290 > 0)
			unk290--;
		return;
	}

	if (unk24C > 0)
		unk24C--;
	if (unk248 > 0)
		unk248--;
	if (unk250 > unk16C->hipdropRelease.get() || (marioIsOn() && !mHeldObject)) {
		f32 weight = unk16C->marioWeight.get();
		if (unk250 > 0)
			weight += unk16C->marioDropWeight.get();
		if (!(weight <= 0.0000000001f)) {
			static JGeometry::TVec3<f32> yDown(0.0f, -1.0f, 0.0f);
			JGeometry::TVec3<f32> lever;
			lever.sub(*gpMarioPos, getPosition());
			JGeometry::TVec3<f32> torque;
			torque.cross(lever, yDown);
			torque.scale(0.00000001f * weight);
			mAngleVel.add(torque);
		}
	}
	updatePosture_();
	calcRootMatrix();
	TMapObjBase::control();
	calcBathtubData();
	getModel()->calc();
	JGeometry::TVec3<f32> up;
	mBathtubData.unk18.getYDir(up);
	if (mBathtubData.unk0C.dot(up) > 0.995f)
		gpMarioParticleManager->emit(0x1BE, &unk1F4, 1, this);
	for (int i = 0; i < 5; ++i) {
		if (unk168[i]->unk248) {
			MtxPtr mtx = getMActor()->getModel()->getAnmMtx(mWaterJntIdx[i]);
			gpMarioParticleManager->emitAndBindToMtxPtr(0x1BF, mtx, 1, mtx);
		}
	}
	if (!mHeldObject) {
		setupCollisions_();
	} else {
		for (int i = 0; i < 30; ++i)
			unk164[i]->remove();
	}
}

// TODO: the frame is 0x150 in retail against 0x190 here (the quaternion sits
// at 0xf0, not 0x130), and retail keeps the normalised axis in a fifth saved
// FPR (f27). Angle spellings, `.value` and setLength(1.0f) are all inert; the
// excess is the inlined TQuat4 setRotate/rotate locals (see JGQuat4.hpp).
void TBathtub::calcBathtubData()
{
	const TPosition3f& mtx = *(TPosition3f*)getRootJointMtx();
	JGeometry::TVec3<f32> dir;
	mtx.getXDir(dir);
	mBathtubData.unk18.setXDir(dir);
	mtx.getYDir(dir);
	mBathtubData.unk18.setYDir(dir);
	mtx.getZDir(dir);
	mBathtubData.unk18.setZDir(dir);
	mtx.getTrans(mBathtubData.mPos);

	f32 upY  = mBathtubData.unk18.at(1, 1);
	f32 tilt = JGeometry::TUtil<f32>::sqrt(1.0f - upY * upY);
	mBathtubData.unk44
	    = mBathtubData.unk3C * JGeometry::max(unk16C->watermark.get(), tilt);
	mBathtubData.unk48 = unk16C->outerHeight.get();
	mBathtubData.unk0C.x = mBathtubData.unk18.at(0, 1);
	mBathtubData.unk0C.y = mBathtubData.unk18.at(1, 1);
	mBathtubData.unk0C.z = mBathtubData.unk18.at(2, 1);
	mBathtubData.unk58.zero();

	TKoopa* koopa
	    = JDrama::TNameRefGen::search<TKoopa>("\x83\x4e\x83\x62\x83\x70");
	if (koopa->effectsTumble() || unk24C > 0) {
		JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
		JGeometry::TVec3<f32> axis;
		axis.cross(up, mBathtubData.unk0C);
		axis.normalize();
		if (!axis.isZero()) {
			JGeometry::TQuat4<f32> shake;
			shake.setRotate(
			    axis, unk16C->maxAngle.get() * 6.2831855f / 360.0f);
			shake.rotate(mBathtubData.unk0C, mBathtubData.unk0C);
			mBathtubData.unk58.set(0.0f, unk16C->shake.get(), 0.0f);
		}
	} else {
		mBathtubData.unk0C.set(0.0f, 1.0f, 0.0f);
	}

	if (unk250 < unk254 / 2 && unk258 > 0)
		mBathtubData.unk64 = 1;
	else
		mBathtubData.unk64 = 0;
	mBathtubData.unk65 = unk29A;
	unk1F4 = mBathtubData.getThing();
}

void TBathtub::setupCollisions_()
{
	JGeometry::TVec3<f32> xDir, zDir, d;
	mBathtubData.unk18.getXDir(xDir);
	mBathtubData.unk18.getZDir(zDir);
	d.sub(*gpMarioPos, mBathtubData.mPos);
	f32 lz = zDir.dot(d);
	f32 lx = xDir.dot(d);
	if (gpMarioPos->y < 300.0f || lx * lx + lz * lz < 0.01f) {
		for (int i = 0; i < 30; ++i)
			unk164[i]->remove();
		for (int i = 0; i < 5; ++i)
			unk168[i]->unk24B = 0;
		return;
	}

	f32 angle = atan2f(lx, lz);
	if (angle < 0.0f)
		angle += 6.2831855f;
	f32 slot = angle * 4.774648f;
	if (slot < 0.0f)
		slot += 30.0f;
	int base = ((int)(0.5f + slot - 1.0f) + 30) % 30;
	for (int i = 0; i < 2; ++i) {
		int index = (i + base) % 30;
		JGeometry::TPosition3<TMtx34f> local;
		local.setEularY((f32)(index + 1) * 6.2831855f / 30.0f - 3.1415927f);
		local.setTrans(0.0f, 0.0f, 0.0f);
		TSMtx34f mtx;
		ConcatMtx34(mtx, *(TSMtx34f*)getModel()->getBaseTRMtx(), local);
		unk164[index]->moveMtx(mtx);
		unk164[index]->setUp();
	}
	for (int i = 2; i < 30; ++i)
		unk164[(i + base) % 30]->remove();
	for (int i = 0; i < 5; ++i)
		unk168[i]->unk24B = 0;
	unk168[((int)(5.0f * angle / 6.2831855f) + 10) % 5]->unk24B = 1;
}

// Unused

// Unused; TODO: Recover callback behavior from the inlined demo setup.
namespace {
BOOL CameraDemoCallBack(u32, u32) { return false; }
}

// A named-local fork over the director accessor: +0x28 of low region at
// this function's three sites, and with the demo-camera flag named at
// function scope (+8 of named block) TBathtub::startDemo is exact.
static inline TMarDirector* MapObjCoronaMarDirector()
{
	TMarDirector* d = SMSGetMarDirector();
	return d;
}

void TBathtub::startDemo()
{
	if (unk29A)
		return;
	MSBgm::stopTrackBGMs(7, 10);
	if (!(unk2A0 & 0x20))
		MapObjCoronaMarDirector()->getConsole()->startAppearBalloon(0x23, true);
	unk2A0 |= 0x20;
	unk290 = 10;
	for (int i = 0; i < 5; ++i)
		unk168[i]->kill();
	unk168[2]->reset();
	unk168[2]->startBreak(0, 2, unk16C->animSpeed1.get());
	unkF8 |= MAP_OBJ_FLAG_UNK8;
	unkF8 &= ~MAP_OBJ_FLAG_UNK100;
	startBck("bath_overturn1");
	TTakeActor* mario = (TTakeActor*)SMS_GetMarioHitActor();
	if (mario->receiveMessage(this, HIT_MESSAGE_TAKE))
		mHeldObject = mario;
	gpMarioOriginal->mFaceAngle.y = 0x7FFF;
	JDrama::TFlagT<u16> flag(0);
	MapObjCoronaMarDirector()->fireStartDemoCamera("koopa_last2", &mPosition, -1,
	    mRotation.y, false, nullptr, 0, nullptr, flag);
	MapObjCoronaMarDirector()->fireStreamingMovie(0xE);
	JDrama::TNameRefGen::search<TKoopa>("\x83\x4e\x83\x62\x83\x70")
	    ->fall();
	unk29A = 1;
}

// Unused
void TBathtub::removeCollisions_()
{
	for (int i = 0; i < 30; ++i)
		unk164[i]->remove();
}

// TODO: instructions match; retail's frame is 0x10 larger (0x88) in the low
// inline-temporary region below getLocalPos's return temporary.
bool TBathtub::allowsTumble() const
{
	JGeometry::TVec3<f32> pos = SMS_GetMarioPos();
	f32 angle;
	if (getNearGrip(pos, 18.0f, &angle)) {
		JGeometry::TVec3<f32> local = mBathtubData.getLocalPos(pos);
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
					TNozzleTrigger* nozzle
					    = (TNozzleTrigger*)gun->getCurrentNozzle();
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

void TBathtub::calcRootMatrix()
{
	if (unk29A) {
		TPosition3f* mtx = (TPosition3f*)getModel()->getBaseTRMtx();
		MsMtxSetRotRPH(*mtx, 0.0f, mRotation.y, 0.0f);
		mtx->setTrans(mPosition);
	} else {
		TPosition3f& mtx = *(TPosition3f*)getModel()->getBaseTRMtx();
		mtx.setQuat(mQuat);
		mtx.setTrans(mPosition);
	}
}

// Unused; TODO: Recover quaternion and direction helpers from their callers.
// Integrates angular velocity `w` into `q` over one frame.
void QuatRotate(JGeometry::TQuat4<f32>& q, const JGeometry::TVec3<f32>& w)
{
	JGeometry::TQuat4<f32> dq;
	dq.xyz().scale(0.5f, w);
	dq.w = 0.0f;
	dq.mul(q);
	q.x += dq.x;
	q.y += dq.y;
	q.z += dq.z;
	q.w += dq.w;
	q.normalize();
}

// A setter level around the found-angle store: +4 of low region, the
// rung that lands TBathtub::getNearGrip's inlined getDir block.
static inline void MapObjCoronaSetAngle(f32* out, f32 v)
{
	*out = v;
}

// Binding level over the matrix cast: +8 of low region, landing
// getNextGrip / getNextJuncture frames (0x148 / 0x130).
static inline const TPosition3f& MapObjCoronaMatrix(MtxPtr mtx)
{
	const TPosition3f& matrix = *(TPosition3f*)mtx;
	return matrix;
}

namespace {
/// Yaw of `pos` inside the frame of `mtx`, measured about the matrix's Y axis.
s16 getDir(MtxPtr mtx, const JGeometry::TVec3<f32>& pos)
{
	const TPosition3f& matrix = *(TPosition3f*)mtx;
	JGeometry::TVec3<f32> x, z, origin, relative;
	matrix.getXDir(x);
	matrix.getZDir(z);
	matrix.getTrans(origin);
	relative.sub(pos, origin);
	f32 dz = z.dot(relative);
	f32 dx = x.dot(relative);
	return matan(dz, dx);
}

/// Same, but the point is first pushed sideways by the part of `offset` that
/// is tangential to the radius, so a moving target leads the grip it will
/// reach rather than the one it stands on.
s16 getDir(MtxPtr mtx, const JGeometry::TVec3<f32>& pos,
           const JGeometry::TVec3<f32>& offset)
{
	const TPosition3f& matrix = MapObjCoronaMatrix(mtx);
	JGeometry::TVec3<f32> x, z, origin, relative, radial, tangent;
	matrix.getXDir(x);
	matrix.getZDir(z);
	matrix.getTrans(origin);
	relative.sub(pos, origin);
	radial.setLength(relative, 1.0f);
	tangent.scaleAdd(-radial.dot(offset), radial, offset);
	relative.add(tangent);
	return matan(z.dot(relative), x.dot(relative));
}
}

// Unused
f32 TBathtub::getNearJuncture(const JGeometry::TVec3<f32>& pos) const
{
	s16 dir     = getDir(*getRootJointMtx(), pos);
	f32 angle   = (360.0f / 65536.0f) * dir;
	f32 nearest = 360.0f;
	int index   = 0;
	for (int i = 0; i < 5; ++i) {
		f32 difference = fabsf(WrapAngleDiffF(unk13C[i], angle));
		if (difference < nearest) {
			index   = i;
			nearest = difference;
		}
	}
	return unk13C[index];
}

// TODO: 99.9%, frame exact. Retail converts dir into a volatile and lands
// only the product `k * dir` in angle's f28; `angle *= k` converts straight
// into f28. Every product spelling (`k * dir`, `dir * k`, `angle = angle * k`,
// `k * angle`, an unnamed getDir, an f32 dir) gets that shape but numbers the
// magic double, constant and conversion f1/f2/f0 against retail's f2/f0/f1
// (5 differences), so the compound spelling stays.
bool TBathtub::getNearGrip(const JGeometry::TVec3<f32>& pos, f32 tolerance,
                          f32* gripAngle) const
{
	s16 dir     = getDir(*getRootJointMtx(), pos);
	f32 angle   = dir;
	angle *= 360.0f / 65536.0f;
	f32 nearest = 360.0f;
	int index = 0;
	for (int i = 0; i < 5; ++i) {
		f32 difference = fabsf(WrapAngleDiffF(unk150[i], angle));
		if (difference < nearest) {
			index = i;
			nearest = difference;
		}
	}
	if (nearest < tolerance) {
		MapObjCoronaSetAngle(gripAngle, unk150[index]);
		return true;
	}
	return false;
}

// TODO: 99.5%, frame exact. Retail computes the z dot (0xbc block) before the
// x dot inside the inlined getDir, and converts dir into f1 before scaling
// into f28 (getNextGrip too). Inert: named dz/dx (breaks inlining), x/z
// declaration and getter order, relative.dot(v), `k * dir`.
f32 TBathtub::getNextJuncture(const JGeometry::TVec3<f32>& pos,
                              const JGeometry::TVec3<f32>& offset) const
{
	s16 dir     = getDir(*getRootJointMtx(), pos, offset);
	f32 angle   = dir;
	angle *= 360.0f / 65536.0f;
	f32 nearest = 360.0f;
	int index   = 0;
	for (int i = 0; i < 5; ++i) {
		f32 difference = fabsf(WrapAngleDiffF(unk13C[i], angle));
		if (difference < nearest) {
			index   = i;
			nearest = difference;
		}
	}
	return unk13C[index];
}

u8 TBathtub::getNextGrip(const JGeometry::TVec3<f32>& pos,
                         const JGeometry::TVec3<f32>& offset, f32 tolerance,
                         f32* gripAngle) const
{
	s16 dir     = getDir(*getRootJointMtx(), pos, offset);
	f32 angle   = dir;
	angle *= 360.0f / 65536.0f;
	f32 nearest = 360.0f;
	int index   = 0;
	for (int i = 0; i < 5; ++i) {
		f32 difference = fabsf(WrapAngleDiffF(unk150[i], angle));
		if (difference < nearest) {
			index   = i;
			nearest = difference;
		}
	}
	if (nearest < tolerance) {
		*gripAngle = unk150[index];
		return true;
	}
	return false;
}

// Unused; TODO: no call site survives, so the bit-per-message shape is a
// guess constrained only by the map's 0x64.
void TBathtub::showMessage(u32 message)
{
	if (!(unk2A0 & (1 << message)))
		gpMarDirector->getConsole()->startAppearBalloon(message, true);
	unk2A0 |= 1 << message;
}

// TODO: retail keeps `excess` as an unfused fmuls/fsubs (ours fuses to
// fnmsubs), and mQuat.mul(limit, mQuat) reads limit.w * q.x first, the
// term order of TQuat4::mul(other); the rest is FPR numbering.
void TBathtub::updatePosture_()
{
	static JGeometry::TVec3<f32> yDown(0.0f, 1.0f, 0.0f);

	if (unk250 == 0) {
		f32 rate;
		if (unk258 == 0) {
			rate = 1.0f;
		} else {
			unk258--;
			rate = 1.0f - (f32)unk258 / (f32)unk25C;
		}
		JGeometry::TVec3<f32> up;
		mQuat.getYDir(up);
		JGeometry::TVec3<f32> axis;
		axis.cross(yDown, up);
		axis.normalize();
		f32 rebound = unk16C->rebound.value;
		axis.scale(rebound * (rate * -acosf(yDown.dot(up))));
		mAngleVel.scaleAdd(unk16C->angleVelDamp.value, mAngleVel, axis);
	} else {
		unk250--;
	}

	QuatRotate(mQuat, mAngleVel);

	JGeometry::TVec3<f32> up;
	mQuat.getYDir(up);
	f32 angle = acosf(yDown.dot(up));
	f32 excess = angle - unk16C->maxAngle.value * 0.017453292f;
	if (excess > 0.0f) {
		JGeometry::TQuat4<f32> limit;
		limit.setRotate(up, yDown, excess / angle);
		mQuat.mul(limit, mQuat);
	}
	mQuat.normalize();
}

// Binding level over a raw member read, worth +16 of low region in
// TBathtub::load (batch 127).
// The grip angle for index i. Spelling the whole expression as one level
// puts the product in a volatile FPR and the quotient straight into the
// named local's callee-saved one, as retail does (batch 356).
static inline f32 MapObjCoronaGripAngle(int i)
{
	return 360.0f * (0.5f + (f32)i) / 5.0f;
}

static inline MActor* MapObjCoronaUnk29C(const TBathtub* p)
{
	MActor* v29C = p->unk29C;
	return v29C;
}

void TBathtub::load(JSUMemoryInputStream& stream)
{
	unk24C = 0;
	TMapObjBase::load(stream);
	mPosition.x = mInitialPosition.x;
	mPosition.y = mInitialPosition.y;
	mPosition.z = mInitialPosition.z;

	unk164 = new TMapCollisionMove*[30];
	for (int i = 0; i < 30; ++i) {
		unk164[i] = new TMapCollisionMove;
		const char* path = nullptr;
		switch (i % 6) {
		case 0:
			path = "/scene/mapObj/bath_col_inside3.col";
			break;
		case 1:
			path = "/scene/mapObj/bath_col_inside2.col";
			break;
		case 2:
			path = "/scene/mapObj/bath_col_inside1.col";
			break;
		case 3:
			path = "/scene/mapObj/bath_col_inside6.col";
			break;
		case 4:
			path = "/scene/mapObj/bath_col_inside5.col";
			break;
		case 5:
			path = "/scene/mapObj/bath_col_inside4.col";
			break;
		}
		unk164[i]->init(path, 0, this);
		unk164[i]->setUp();
	}

	mBathtubData.mPos.x = mInitialPosition.x;
	mBathtubData.mPos.y = mInitialPosition.y;
	mBathtubData.mPos.z = mInitialPosition.z;
	// TODO: the store order proves mBathtubData.unk18 is a
	// JGeometry::TRotation3<TMatrix33<SMatrix33R<f32> > >: with the row-major
	// base, identity()'s chained assignments write [2][1], [2][0], [1][2],
	// [1][0], [0][2], [0][1] and then the diagonal, exactly as retail does,
	// and the 4-byte weak __ct__Q29JGeometry13SMatrix33R<f>Fv the map lists
	// for this TU appears. The declaration lives in Map/BathWaterManager.hpp,
	// which this batch may not touch; changing it also transposes every
	// at()/ref() in BathWaterManager, GCConsole2 and the bathtub units.
	mBathtubData.unk18.identity();
	mBathtubData.unk3C = 3000.0f;
	mBathtubData.unk40 = 3600.0f;
	mBathtubData.unk44 = mBathtubData.unk3C * sinf(0.27925268f);
	mBathtubData.unk4C = mBathtubData.unk50 = mBathtubData.unk54 = 0.0f;
	mBathtubData.unk58.zero();
	mBathtubData.unk48 = 100.0f;
	mBathtubData.unk64 = 0;
	mBathtubData.unk0C.set(0.0f, 1.0f, 0.0f);

	unk168 = new TBathtubGrip*[5];
	unk138 = new MActorAnmData;
	unk138->init("scene/map/map/stand_effect", nullptr);
	for (int i = 0; i < 5; ++i) {
		f32 angle     = MapObjCoronaGripAngle(i);
		f32 gripAngle = angle - 180.0f;
		unk168[i % 5] = new TBathtubGrip(this, gripAngle, unk138,
		    "\x89\xf3\x82\xea\x82\xa9\x82\xaf\x82\xcc\x83\x6f\x83"
		    "\x58\x83\x5e\x83\x75\x82\xcc\x8e\xe6\x82\xc1\x8e\xe8");
		unk168[i % 5]->appear();
		unk13C[i] = -180.0f + angle;
		unk150[i] = -180.0f + 360.0f * (f32)i / 5.0f;
	}

	JUTNameTab* names = getModel()->getModelData()->getJointName();
	mMarioJntIdx     = names->getIndex("mario");
	mStarJntIdx      = names->getIndex("star");
	mWaterJntIdx[0]  = names->getIndex("water4");
	mWaterJntIdx[1]  = names->getIndex("water5");
	mWaterJntIdx[2]  = names->getIndex("water1");
	mWaterJntIdx[3]  = names->getIndex("water2");
	mWaterJntIdx[4]  = names->getIndex("water3");
	mDuckJntIdx      = names->getIndex("ahiru");
	mSubmarineJntIdx = names->getIndex("submarin");
	mJuniorJntIdx    = names->getIndex("Jr");
	mKoopaJntIdx     = names->getIndex("koopa");

	MActorAnmData* shineAnm = new MActorAnmData;
	shineAnm->init("/scene/map/map/shine", nullptr);
	unk29C = new MActor(shineAnm);
	void* resource = JKRGetResource("/scene/map/map/shine/shine_3bai.bmd");
	unk29C->setModel(
	    new J3DModel(J3DModelLoaderDataBase::load(resource, 0x10000000), 0, 1),
	    0x10000000);
	mShineBodyJntIdx
	    = MapObjCoronaUnk29C(this)->getModel()->getModelData()->getJointName()->getIndex("body");
	unk298 = 1;
}

TBathtub::TBathtub(const char* name)
    : TMapObjBase(name)
    , unk164(nullptr)
    , unk290(0)
{
	TBathtubParams* params = new TBathtubParams;
	unk16C                 = params;
	mQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
	mPosition.x = mPosition.y = mPosition.z = 0.0f;
	mAngleVel.zero();
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
bool TBathtub::isKillerLaunchable() const
{
	if (unk29A)
		return false;
	TKoopa* koopa = JDrama::TNameRefGen::search<TKoopa>("\x83\x4e\x83\x62\x83\x70");
	if (!koopa->allowsLaunch())
		return false;
	return isKillerAttackable();
}

int TBathtub::getNumKillerLaunchable() const
{
	if (!isKillerLaunchable())
		return 0;
	int num = getNumGripsDead() + 1;
	if (num < 2)
		num = 2;
	if (num > 4)
		num = 4;
	return num;
}

bool TBathtub::isKillerAttackable() const { return unk248 <= 0; }

// Unused
bool TBathtub::isBreaking() const
{
	for (int i = 0; i < 5; ++i)
		if (unk168[i]->unk248)
			return true;
	return false;
}

int TBathtub::getNumKillerBurstable() const
{
	if (!isKillerLaunchable())
		return 0;
	int dead = getNumGripsDead();
	if (dead >= 4)
		return 8;
	if (!allowsTumble() && unk250 == 0 && unk258 == 0) {
		switch (dead) {
		case 1:
			return 4;
		case 2:
			return 6;
		case 3:
			return 8;
		case 4:
			return 8;
		}
	}
	return 0;
}

TBathtub::~TBathtub() { }
