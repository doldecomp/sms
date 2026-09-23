#include <Enemy/BossGesso.hpp>
#include <Enemy/BossGessoTentacle.hpp>
#include <Enemy/BossGessoPolDrop.hpp>
#include <Enemy/Conductor.hpp>
#include <Enemy/EffectObj.hpp>
#include <Enemy/NameKuri.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Map/Map.hpp>
#include <Map/PollutionManager.hpp>
#include <MoveBG/ItemManager.hpp>
#include <GC2D/GCConsole2.hpp>
#include <Player/Mario.hpp>
#include <Player/MarioAccess.hpp>
#include <Player/ModelWaterManager.hpp>
#include <Camera/CameraShake.hpp>
#include <Camera/CubeManagerBase.hpp>
#include <MarioUtil/DrawUtil.hpp>
#include <MarioUtil/ShadowUtil.hpp>
#include <MarioUtil/TexUtil.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/LightUtil.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <System/MSoundMainSide.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/ObjModel.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DCluster.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DMaterial.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

const char* bgeso_bastable[] = {
	nullptr,
	"/scene/bgeso/bas/bgeso_cannon.bas",
	nullptr,
	"/scene/bgeso/bas/bgeso_damage.bas",
	nullptr,
	"/scene/bgeso/bas/bgeso_eyedamage.bas",
	"/scene/bgeso/bas/bgeso_fly.bas",
	"/scene/bgeso/bas/bgeso_hit.bas",
	nullptr,
	"/scene/bgeso/bas/bgeso_limit.bas",
	"/scene/bgeso/bas/bgeso_osen.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/bgeso/bas/bgeso_roll_all_start.bas",
	nullptr,
	"/scene/bgeso/bas/bgeso_roll_start.bas",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"/scene/bgeso/bas/bgeso_wait.bas",
	"/scene/bgeso/bas/bgeso_wait2.bas",
	"/scene/bgeso/bas/bgeso_wait3.bas",
};

// TODO: 8 bytes in the map, i.e. `li r3, 0; blr`, so the debug name table this
// returned was stripped from the retail build and only the null return is
// left. The return type is a guess from the name.
static const char* getAttackModeStr(int) { return nullptr; }

// fabricated: a TBGTentacle predicate over the same state set as the one
// changeAllTentacleState() tests. Parked here rather than in
// BossGessoTentacle.hpp, which belongs to bgtentacle.cpp.
//
// The `int state` copy is load-bearing, not decoration: reading the member
// straight into the comparisons leaves all three tests separate in every
// expansion, while copying it first lets MWCC fold `state == 3 || state == 4`
// into retail's `subi 3; cmplwi 1` and hoist the `== 6` test ahead of it, at
// every inlined site (stopIfRoll, changeAttackMode and the five nerves) and
// in the unit's .data, which only matches with the fold. The comparison order
// stays 4, 6, 3: 6/3/4 and 3/4/6 both break the fold and the data again.
//
// TODO: the out-of-line TBossGesso::changeAllTentacleState (94.12) is the one
// body that must *not* fold -- retail emits 4, 6, 3 unmerged there from the
// same source and folds only in the expansions. No spelling found that splits
// them: a named local in that loop (93.9), reading getState() (93.9), the
// explicit three-compare chain (87.0, and it costs the expansions their
// materialised bool), a pointer-taking forwarding overload, and
// TBGTentacle::isThing() (order 6/3/4, no fold anywhere) were all tried.
// TODO: find the real name; `canTake` and `isAttacking` are the only named
// TBGTentacle predicates in the map and neither fits.
static inline BOOL isTentacleBusy(TBGTentacle* tentacle)
{
	int state = tentacle->mState;
	if (state == 4 || state == 6 || state == 3)
		return true;

	return false;
}

static BOOL isNozzleWater(THitActor* param_1)
{
	if (!param_1->isActorType(0x1000001))
		return FALSE;

	return gpModelWaterManager->checkParticleFlag((TWaterHitActor*)param_1,
	                                              0x40);
}

TBossGessoParams::TBossGessoParams(const char* path)
    : TSpineEnemyParams(path)
    , PARAM_INIT(mSLUnisonInter, 2000)
    , PARAM_INIT(mSLUnisonHoming, 200)
    , PARAM_INIT(mSLSingleHoming, 200)
    , PARAM_INIT(mSLBeakHoming, 200)
    , PARAM_INIT(mSLRegenFoot, 4800)
    , PARAM_INIT(mSLAmputeeTime, 1200)
    , PARAM_INIT(mSLRestTime, 360)
    , PARAM_INIT(mSLStunTime, 1200)
    , PARAM_INIT(mSLBeakDamageHeight, 150.0f)
    , PARAM_INIT(mSLBeakDamageRadius, 150.0f)
    , PARAM_INIT(mSLBeakLengthLimit, 1200.0f)
    , PARAM_INIT(mSLBeakLengthDamage, 600.0f)
    , PARAM_INIT(mSLBeakLengthPollute, 500.0f)
    , PARAM_INIT(mSLUnisonAttackSpeed, 2.0f)
    , PARAM_INIT(mSLDoubleAttackSpeed, 1.2f)
    , PARAM_INIT(mSLSkipRopeAttackSpeed, 1.0f)
    , PARAM_INIT(mSLSingleAttackLen, 2200.0f)
    , PARAM_INIT(mSLDoubleAttackLen, 1900.0f)
    , PARAM_INIT(mSLUnisonAttackLen, 1900.0f)
    , PARAM_INIT(mSLForceUnisonLen, 800.0f)
    , PARAM_INIT(mSLGuardLen, 500.0f)
    , PARAM_INIT(mSLTentacleStretch, 0.1f)
    , PARAM_INIT(mSLBeakStretch, 0.07f)
    , PARAM_INIT(mSLEyeDamageRadius, 100.0f)
    , PARAM_INIT(mSLEyeDamageHeight, 150.0f)
    , PARAM_INIT(mSLShootRadius, 8000.0f)
    , PARAM_INIT(mSLBlurJoint, 10)
    , PARAM_INIT(mSLBlurScale, 1.0f)
    , PARAM_INIT(mSLColumnScale, 3.0f)
    , PARAM_INIT(mSLSightAngle, 120.0f)
    , PARAM_INIT(mSLAmputeeWait, 720)
{
	TParams::load(mPrmPath);
}

TBGBeakHit::TBGBeakHit(TBossGesso* owner, const char* name)
    : TTakeActor(name)
    , mOwner(owner)
{
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	initHitActor(0x8000008, 1, -0x80000000, 0.0f, 0.0f,
	             mOwner->getSaveParam2()->mSLBeakDamageRadius.get(),
	             mOwner->getSaveParam2()->mSLBeakDamageHeight.get());
	offHitFlag(HIT_FLAG_NO_COLLISION);
	unkA4.zero();
}

MtxPtr TBGBeakHit::getTakingMtx() { return unk74; }

// TODO: fake
static inline JGeometry::TVec3<f32> fromPolar(f32 theta, f32 radius)
{
	return JGeometry::TVec3<f32>(radius * JMASSin(theta * (65536.0f / 360.0f)),
	                             0.0f,
	                             radius * JMASCos(theta * (65536.0f / 360.0f)));
}

// Binding level worth +8 of low region, landing TBGBeakHit::moveRequest's
// frame at 0x78 (batch 124).
static inline f32 BossgessoGetIntendedMag(const TMario* p)
{
	f32 intendedMag = p->getIntendedMag();
	return intendedMag;
}

static inline TBGBeakHit* BossgessoGetBeak(const TBossGesso* p);
static inline MActor* BossgessoGetMActor(const TLiveActor* p);

BOOL TBGBeakHit::moveRequest(const JGeometry::TVec3<f32>& where_to)
{
	TBossGessoParams* params = mOwner->getSaveParam2();

	unkA4 = fromPolar(gpMarioOriginal->getIntendedYaw(),
	                  BossgessoGetIntendedMag(gpMarioOriginal)
	                      * params->mSLBeakStretch.get());

	JGeometry::TVec3<f32> delta = mOwner->mPosition;
	delta -= where_to;
	delta.scale(0.001f);
	unkA4 += delta;

	mPosition = where_to;
	return FALSE;
}

BOOL TBGBeakHit::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->isActorType(0x1000001)) {
		if (!isNozzleWater(sender))
			return true;

		mOwner->gotEyeDamage();

		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &mPosition, 0,
		                             nullptr);
		return true;
	}

	if (mOwner->mAttackMode == 3)
		return false;

	if (mOwner->getLatestNerve() == &TNerveBGPollute::theNerve()
	    || mOwner->getLatestNerve() == &TNerveBGPolDrop::theNerve()
	    || mOwner->getLatestNerve() == &TNerveBGBeakDamage::theNerve())
		return false;

	if (sender->getActorType() == 0x80000001) {
		if (message == HIT_MESSAGE_TAKE) {
			TTakeActor* actor = (TTakeActor*)sender;
			if (actor->mHeldObject != nullptr && actor->mHeldObject != this)
				return false;

			mHolder = actor;

			if (mOwner->unk190.color.a != 0)
				mOwner->showMessage(0x25);

			mOwner->unk1A0 = 1;
			return true;
		}

		if (message == HIT_MESSAGE_THROWN || message == HIT_MESSAGE_UNK8) {
			// TODO: inlined from TBossGesso?
			JGeometry::TVec3<f32> delta = mPosition;
			TBossGesso* gesso           = mOwner;
			delta -= gesso->mPosition;
			f32 length = gesso->getSaveParam2()->mSLBeakLengthDamage.get();

			if (delta.length() >= length)
				mOwner->gotBeakDamage();

			mHolder = nullptr;
			return true;
		}
	}

	return false;
}

// TODO: frame 0xe8 against retail 0x138. Every named slot now sits exactly
// (up is declared before offset and never materialised), but retail's low
// region is 0xd0 of dead inline temporaries against our 0x7c.
void TBGBeakHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_MOVE) {
		mOwner->getJointTransByIndex(26, &mPosition);
		mPosition.y -= mDamageHeight * 0.5f;

		ensureTakeSituation();
		if (mHolder != nullptr && !unkA4.isZero()) {
			mPosition += unkA4;
			JGeometry::TVec3<f32> delta = mHolder->mPosition;
			delta += unkA4;
			mHolder->moveRequest(delta);
			unkA4.zero();
		}

		if (mHolder != nullptr) {
			JGeometry::TVec3<f32> us2mario = SMS_GetMarioPos();
			us2mario -= mOwner->mPosition;
			if (!us2mario.isZero())
				VECNormalize(&us2mario, &us2mario);
			else
				us2mario.set(0.0f, 0.0f, 1.0f);

			JGeometry::TVec3<f32> up(0.0f, 1.0f, 0.0f);
			JGeometry::TVec3<f32> offset = us2mario;
			JGeometry::TVec3<f32> perp;
			perp.cross(up, us2mario);
			if (!perp.isZero())
				VECNormalize(&perp, &perp);
			else
				perp.set(1.0f, 0.0f, 0.0f);

			unk74.mMtx[0][0] = perp.x;
			unk74.mMtx[1][0] = perp.y;
			unk74.mMtx[2][0] = perp.z;
			unk74.mMtx[0][1] = 0.0;
			unk74.mMtx[1][1] = 1.0;
			unk74.mMtx[2][1] = 0.0;
			unk74.mMtx[0][2] = us2mario.x;
			unk74.mMtx[1][2] = us2mario.y;
			unk74.mMtx[2][2] = us2mario.z;

			offset.scale(mDamageRadius);
			offset.add(mPosition);
			unk74.mMtx[0][3] = offset.x;
			unk74.mMtx[1][3] = offset.y;
			unk74.mMtx[2][3] = offset.z;

			JGeometry::TVec3<f32> ownerToUs = mPosition;
			ownerToUs -= mOwner->mPosition;
			f32 beakPullDist = ownerToUs.length();

			f32 lenPollute = mOwner->getSaveParam2()->mSLBeakLengthPollute.get();
			if (mOwner->unk190.color.a != 0 && beakPullDist >= lenPollute) {
				mHolder->receiveMessage(this, HIT_MESSAGE_UNK8);
			}

			f32 lenLimit = mOwner->getSaveParam2()->mSLBeakLengthLimit.get();
			if (beakPullDist >= lenLimit) {
				mHolder->receiveMessage(this, HIT_MESSAGE_UNK8);
				mOwner->gotBeakDamage();
			}
		}
	}
}

TBGEyeHit::TBGEyeHit(TBossGesso* owner, int joint_index, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mJointIndex(joint_index)
{
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	initHitActor(0x8000009, 1, 0x1000000, 0.0f, 0.0f,
	             mOwner->getSaveParam2()->mSLEyeDamageRadius.get(),
	             mOwner->getSaveParam2()->mSLEyeDamageHeight.get());
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBGEyeHit::receiveMessage(THitActor* sender, u32 message)
{
	if (mOwner->getAttackMode() == 3)
		return false;

	if (sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		mOwner->gotEyeDamage();
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		return true;
	}

	return mOwner->receiveMessage(sender, message);
}

void TBGEyeHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	THitActor::perform(cue, graphics);
	if (cue & CUE_CALC_ANIM)
		mOwner->getJointTransByIndex(mJointIndex, &mPosition);
}

TBGBodyHit::TBGBodyHit(TBossGesso* owner, int joint_index, const char* name)
    : THitActor(name)
    , mOwner(owner)
    , mJointIndex(joint_index)
{
	JDrama::TNameRefGen::search<TIdxGroupObj>("敵グループ")
	    ->getChildren()
	    .push_back(this);

	initHitActor(0x8000005, 1, -0x7f000000, 300.0f, 300.0f, 300.0f, 300.0f);
	offHitFlag(HIT_FLAG_NO_COLLISION);
}

BOOL TBGBodyHit::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		return true;
	}

	return mOwner->receiveMessage(sender, message);
}

void TBGBodyHit::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM)
		mOwner->getJointTransByIndex(mJointIndex, &mPosition);
	THitActor::perform(cue, graphics);
}

TBossGessoMtxCalc::TBossGessoMtxCalc(TBossGesso* owner)
    : M3UMtxCalcSIAnmBlendQuat(true)
    , mOwner(owner)
{
	mMotionBlendRatio = 0.0f;
}

void TBossGessoMtxCalc::joinAnm(int param_1)
{
	M3UMtxCalcSIAnmBlendQuat::joinAnm(
	    mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        param_1));
}

void TBossGessoMtxCalc::setAnm(int param_1)
{
	M3UMtxCalcSIAnmBlendQuat::setAnm(
	    mOwner->getActorKeeper()->getMActorAnmData()->getUnk2C()->getAnmPtr(
	        param_1));
}

void TBossGessoMtxCalc::calc(u16 param_1)
{
	M3UMtxCalcSIAnmBlendQuat::calc(param_1);
	if (param_1 != 26)
		return;

	TBGBeakHit* beak = BossgessoGetBeak(mOwner);
	if (beak != nullptr && beak->isTaken()) {
		MtxPtr mtx26     = mOwner->getModel()->getAnmMtx(param_1);
		mtx26[0][3]      = beak->mPosition.x;
		mtx26[1][3]      = beak->mPosition.y + 50.0f;
		mtx26[2][3]      = beak->mPosition.z;

		JGeometry::TVec3<f32> local_28 = beak->mPosition;
		local_28 -= mOwner->getPosition();

		f32 fVar4 = VECMag(&local_28);
		if (fVar4 > 0.0f)
			fVar4 = 300.0f / fVar4;
		else
			fVar4 = 1.0f;

		if (fVar4 > 1.0f)
			fVar4 = 1.0f;

		MTXCopy(mtx26, J3DSys::mCurrentMtx);
		mtx26[0][0] *= fVar4;
		mtx26[1][0] *= fVar4;
		mtx26[2][0] *= fVar4;
		mtx26[0][2] *= fVar4;
		mtx26[1][2] *= fVar4;
		mtx26[2][2] *= fVar4;
	} else {
		if (mOwner->mAttackMode == 3) {
			MtxPtr mtx26 = mOwner->getModel()->getAnmMtx(param_1);
			mtx26[0][0] *= 0.02f;
			mtx26[1][0] *= 0.02f;
			mtx26[2][0] *= 0.02f;
			MTXCopy(mtx26, J3DSys::mCurrentMtx);
		}
	}
}

TBGBinder::TBGBinder() { }

void TBGBinder::bind(TLiveActor* param_1)
{
	TBossGesso* gesso = (TBossGesso*)param_1;

	JGeometry::TVec3<f32> linearVel = gesso->mLinearVelocity;
	JGeometry::TVec3<f32> local_3c  = gesso->mPosition;
	local_3c += linearVel;

	if (gesso->isAirborne()) {
		JGeometry::TVec3<f32> local_48 = gesso->mVelocity;
		local_3c += local_48;
		local_48.y -= gesso->getGravityY();
		if (local_48.y < TLiveActor::mVelocityMinY)
			local_48.y = TLiveActor::mVelocityMinY;
		gesso->mVelocity = local_48;
	}

	if (gesso->getLatestNerve() == &TNerveBGDie::theNerve()
	    && gesso->getMActor()->checkCurBckFromIndex(6)) {

		gesso->mLinearVelocity = local_3c - gesso->mPosition;

		if (gpMarDirector->mMap != 9
		    && gesso->mPosition.y - local_3c.y > 0.0f) {

			// TODO: this is likely an inline where xyz are passed as separate
			// args
			f32 someZ = local_3c.z;
			const TBGCheckData* pTStack_4c;
			f32 dVar7 = gpMap->checkGround(local_3c.x,
			                               local_3c.y + gesso->getHeadHeight(),
			                               someZ, &pTStack_4c);
			dVar7 += 1.0f;
			const TBGCheckData* pTStack_50;
			f32 dVar8 = gpMap->checkGround(
			    local_3c.x, gesso->mPosition.y + gesso->getHeadHeight(), someZ,
			    &pTStack_50);
			dVar8 += 1.0f;

			if (dVar8 > dVar7) {
				TEffectColumWater* enemy
				    = (TEffectColumWater*)gpConductor->makeOneEnemyAppear(
				        gesso->mPosition, "エフェクト水柱マネージャー", 1);
				if (enemy) {
					f32 scale = gesso->getSaveParam2()->mSLColumnScale.get();
					JGeometry::TVec3<f32> local_5c;
					local_5c.x = scale;
					local_5c.y = scale;
					local_5c.z = scale;
					enemy->generate(gesso->mPosition, local_5c);
				}

				SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_DOWN_DIVE,
				                                &gesso->mPosition, 0, nullptr,
				                                0, 4);

				gesso->unk1A4 = 1.0f;
				SMSRumbleMgr->start(8, &gesso->unk1A4);
			}
		}
	} else {
		// TODO: this is likely an inline where xyz are passed as separate args
		f32 someY = local_3c.y;
		f32 someZ = local_3c.z;

		const TBGCheckData* local_60;
		f32 fVar1 = gpMap->checkGround(
		    local_3c.x, someY + gesso->getHeadHeight(), someZ, &local_60);
		fVar1 += 1.0f;

		f32 gessoY = gesso->mPosition.y;
		if (gessoY - someY > 0.0f) {
			const TBGCheckData* local_64;
			f32 dVar8 = gpMap->checkGround(
			    local_3c.x, gessoY + gesso->getHeadHeight(), someZ, &local_64);
			dVar8 += 1.0f;

			if (dVar8 > fVar1) {
				local_60 = local_64;
				fVar1    = dVar8;
			}
		}

		if (someY <= fVar1) {
			local_3c.y       = fVar1;
			gesso->mVelocity = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

			gesso->offLiveFlag(LIVE_FLAG_AIRBORNE);
		} else {
			gesso->onLiveFlag(LIVE_FLAG_AIRBORNE);
		}

		gesso->mGroundHeight = fVar1;
		gesso->mGroundPlane  = local_60;

		gesso->mLinearVelocity = local_3c - gesso->mPosition;
	}
}

TBGCork::TBGCork(TBossGesso* owner)
    : mOwner(owner)
    , unk4(nullptr)
    , unk8(nullptr)
    , unkC(0)
{
	unk4 = mOwner->getActorKeeper()->createMActor("bgeso_kolk.bmd", 0);
	unk8 = mOwner->getActorKeeper()->createMActor("bgeso_kolk_break.bmd", 0);
}

void TBGCork::crush()
{
	if (unkC)
		return;

	unk8->setBckFromIndex(8);
	unk8->getModel()->setBaseTRMtx(unk4->getModel()->getBaseTRMtx());
	unkC = 1;
}

void TBGCork::perform(u32 cue, JDrama::TGraphics* graphics)
{
	MActor* cork;
	if (unkC == 0) {
		cork = unk4;
		if (cue & CUE_CALC_ANIM)
			cork->getModel()->setBaseTRMtx(mOwner->getModel()->getAnmMtx(27));
	} else {
		cork = unk8;
	}

	cork->perform(cue, graphics);
}

TBossGesso::TBossGesso(const char* name)
    : TSpineEnemy(name)
    , mBeak(nullptr)
    , mMtxCalc(nullptr)
    , mAttackMode(0)
    , mTimeInCurrentAttackMode(0)
    , unk178(nullptr)
    , unk17C(0)
    , mPolDrop(nullptr)
    , unk188(0.0f)
    , mCork(nullptr)
    , unk194(0)
    , unk195(0)
    , unk196(0)
    , unk198(0)
    , unk19C(0)
    , unk1A0(0)
    , unk1A1(0)
    , unk1A4(0.0f)
    , unk1A8(0)
    , unk1AC(0)
    , unk1AE(0)
{
	mBinder    = new TBGBinder;
	mTurnSpeed = 0.2f;
}

void TBossGesso::init(TLiveManager* param_1)
{
	mManager = param_1;
	mManager->manageActor(this);
	mMActorKeeper = new TMActorKeeper(mManager, 0xf);
	mMActor       = mMActorKeeper->createMActor("bgeso_body.bmd", 0);
	for (int i = 0; i < 4; ++i)
		mTentacles[i] = new TBGTentacle(this, 6, i);
	mBeak     = new TBGBeakHit(this);
	mLeftEye  = new TBGEyeHit(this, 7);
	mRightEye = new TBGEyeHit(this, 4);
	mBody     = new TBGBodyHit(this, 0);

	initHitActor(0x8000005, 5, -0x7f000000, 300.0f, 300.0f, 300.0f, 300.0f);
	onHitFlag(HIT_FLAG_NO_COLLISION);

	mSpine->initWith(&TNerveBGWait::theNerve());
	mMtxCalc = new TBossGessoMtxCalc(this);

	// Two getMActor binders land the 0x250 frame; the inlined `new` result
	// still sits 12 bytes high (0x234 against retail 0x228).
	// TODO: the remaining ~ mismatches are that allocation-order hole.
	BossgessoGetMActor(this)->setCalcForBck(mMtxCalc);

	BossgessoGetMActor(this)->calc();
	getMActor()->setLightType(LIGHT_TYPE_OBJECT);

	unk178   = getActorKeeper()->createMActor("bgeso_dirty_white.bmd", 0);
	mPolDrop = new TBGPolDrop;

	mPolDrop->setMActors(
	    getActorKeeper()->createMActor("bgeso_osenball.bmd", 0),
	    getActorKeeper()->createMActor("bgeso_osenball_white.bmd", 0));

	mCork = new TBGCork(this);

	J3DModel* model = getMActor()->getModel();
	if (!model->getSkinDeform()) {
		J3DSkinDeform* skinDeform = new J3DSkinDeform;
		model->setSkinDeform(skinDeform, J3D_DEFORM_ATTACH_FLAG_UNK_1);
	}

	reset();

	mHitPoints = getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;

	offLiveFlag(LIVE_FLAG_UNK100);
	getMActor()->offMakeDL();
	onLiveFlag(LIVE_FLAG_UNK8);
	mScaledBodyRadius = 330.0f;
	initAnmSound();
	unk190.color.r = 0;
	unk190.color.g = 0;
	unk190.color.b = 0;
	unk190.color.a = 0xE6;

	getMActor()
	    ->getModel()
	    ->getModelData()
	    ->getMaterialNodePointer(0)
	    ->getTevBlock()
	    ->setTevKColor(0, &unk190);

	const ResTIMG* image
	    = (const ResTIMG*)JKRGetResource("/scene/map/pollution/H_ma_rak.bti");

	if (image)
		SMS_ChangeTextureAll(getMActor()->getModel()->getModelData(),
		                     "H_ma_rak_dummy", *image);
}

void TBossGesso::rumblePad(int param_1, const JGeometry::TVec3<f32>& param_2)
{
	if (!SMS_IsMarioTouchGround4cm())
		return;

	JGeometry::TVec3<f32> delta = *gpMarioPos;
	delta -= param_2;
	f32 fVar2 = delta.length();
	f32 fVar1 = (3000.0f - fVar2) / 1000.0f;

	if (fVar1 < 0.0f)
		return;

	if (fVar1 > 1.0f)
		fVar1 = 1.0f;

	switch (param_1) {
	case 0:
		fVar1 *= 0.4f;
		break;
	case 1:
		fVar1 *= 0.7f;
		break;
	case 2:
		break;
	}

	unk1A4 = fVar1;
	SMSRumbleMgr->start(8, &unk1A4);
}

void TBossGesso::definiteRumble()
{
	unk1A4 = 1.0f;
	SMSRumbleMgr->start(8, &unk1A4);
}

void TBossGesso::continuousRumble()
{
	if (mBeak->mHolder != nullptr && mTimeInCurrentAttackMode % 4 == 0)
		rumblePad(1, mBeak->mPosition);
}

// Retail keeps this a `bl` inside perform, which needs the body to cost 15
// statements at depth one; four were missing. Naming the tentacle and the
// node, splitting length() into squared() and sqrt(), and declaring tipPos
// before assigning it (pass 167's TSolidStack::top() shape) are exactly those
// four, and none of them changes a single instruction here: 412 bytes and
// 107 instructions either way, while perform goes 82.2 -> 98.3.
// TODO: the body's own residue is one contraction: retail loads y and z before
// x and fuses `x * x` into `y * y` with an `fmadds` that our squared()/dot()
// spelling does not produce. TMario::wireMove has the identical residue, so it
// is a shared JGVec3 spelling question, not a local one. The literal pool
// agrees that retail expands nothing here: its 100000.0f is @7822, the highest
// id in the TU, because this function alone requests it, while ours lands it
// near perform's own literals.
f32 TBossGesso::lenFromToeToMario()
{
	f32 min = 100000.0f;

	for (int i = 0; i < 4; ++i) {
		TBGTentacle* tentacle = mTentacles[i];
		if (tentacle->isThing2())
			continue;

		TBGTentacle::TNode* node = tentacle->getLastNode();
		JGeometry::TVec3<f32> tipPos;
		tipPos = node->getPosition();

		f32 lenSq = tipPos.squared();
		f32 len   = JGeometry::TUtil<f32>::sqrt(lenSq);
		if (len < min)
			min = len;
	}

	return min;
}

void TBossGesso::showMessage(u32 param_1)
{
	u32 idx  = param_1 == 0x25 ? 3 : param_1 - 3;
	u32 flag = param_1 == 3 ? 0 : 1 << idx;

	if ((unk198 & flag) == 0)
		gpMarDirector->getConsole()->startAppearBalloon(param_1, true);

	unk198 |= flag;
}

// TODO: the map size is 0x11c (284 bytes) and this body compiles to less; the
// best candidates for the remainder are the CUE_MOVE rumble/timer block that
// follows it in perform and the CUE_CALC_ANIM pull-sound block above it, and
// neither reads as part of "check take message".
void TBossGesso::checkTakeMsg()
{
	if (unk1A0)
		return;

	if (is2ndFightNow())
		return;

	JGeometry::TVec3<f32> toMario = SMS_GetMarioPos();
	toMario -= mPosition;

	if (toMario.squared() < 4000000.0f) {
		unk19C++;
		if (unk19C >= 1200) {
			showMessage(4);
			unk1A0 = 1;
		}
	}
}

void TBossGesso::changeBck(int param_1)
{
	mMtxCalc->joinAnm(param_1);
	getMActor()->setFrameCtrlForBck(param_1);

	J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (ctrl != nullptr)
		unk188 = 10.0f / ctrl->getEnd();

	const char** table = getBasNameTable();
	setAnmSound(!table ? nullptr : table[param_1]);
}

// TODO: this inline is 99% incorrect, need to try harder =(
BOOL TBossGesso::inSightAngle(f32 a)
{
	if (inSight() < a)
		return TRUE;

	return FALSE;
}

// TODO: map 0x17c, ours 0x184. Naming the lower bound
// (`f32 lo = dVar9 - 180.0f;` passed to MsWrap) gives exactly 0x17c but costs
// doAttackSingle 93.29 -> 93.22, so it stays out. No spelling of this body
// (operator-, sub/set/assign/direct-init, SMS_DistanceFromMarioVec, a
// temporary `TVec3(...) -= mPosition`, MsAngleDiff, extra named floats; ~30
// variants, 2026-09-23) nor of inSightAngle (bare `return <`, ternary,
// inverted, BOOL local) expands MsGetRotFromZaxisY at depth 2: every caller
// still `bl`s it, so the shoot site and inSightAngle's 0x16c stay open on the
// shared header (see MathUtil.hpp).
f32 TBossGesso::inSight()
{
	JGeometry::TVec3<f32> local_90 = SMS_GetMarioPos();
	local_90 -= mPosition;
	f32 dVar9  = MsGetRotFromZaxisY(local_90);
	f32 dVar10 = MsWrap(mRotation.y, dVar9 - 180.0f, dVar9 + 180.0f);
	return fabsf(dVar9 - dVar10);
}

BOOL TBossGesso::is2ndFightNow() const
{
	if (gpMarDirector->unk7D == 4)
		return TRUE;

	return FALSE;
}

void TBossGesso::stopIfRoll()
{
	if (mAttackMode != 7)
		return;

	changeAttackMode(ASTATE_SINGLE);

	getSpine()->reset();
	mSpine->setNext(&TNerveBGWait::theNerve());

	changeAllTentacleState(0);
}

void TBossGesso::changeAttackMode(int new_mode)
{
	mAttackMode              = new_mode;
	mTimeInCurrentAttackMode = 0;
	switch (mAttackMode) {
	case ASTATE_SINGLE:
	case ASTATE_DOUBLE:
	case ASTATE_SKIP_ROPE:
		break;

	case ASTATE_GUARD: {
		static int idx[2] = { 1, 3 };
		for (int i = 0; i < 2; ++i)
			if (!isTentacleBusy(mTentacles[idx[i]]))
				mTentacles[idx[i]]->changeStateAndFixNodes(10);
		break;
	}

	case ASTATE_UNISON:
		changeAllTentacleState(0);
		break;

	case ASTATE_ROLL:
		if ((int)(MsRandF() * 100.0f) < 0x28)
			unk1A1 = true;
		else
			unk1A1 = false;
		changeAllTentacleState(0x8);
		getSpine()->reset();
		getSpine()->setNext(&TNerveBGRoll::theNerve());
		mSpine->pushAfterCurrent(&TNerveBGWait::theNerve());
		break;

	case ASTATE_SHOOT:
		if (mSpine->getLatestNerve() != &TNerveBGPolDrop::theNerve()
		    && mSpine->getLatestNerve() != &TNerveBGPollute::theNerve()
		    && mSpine->getLatestNerve() != &TNerveBGTentacleDamage::theNerve()
		    && mSpine->getLatestNerve() != &TNerveBGEyeDamage::theNerve()
		    && mSpine->getLatestNerve() != &TNerveBGBeakDamage::theNerve()) {
			mSpine->reset();
			mSpine->setNext(&TNerveBGPolDrop::theNerve());
			mSpine->pushAfterCurrent(&TNerveBGWait::theNerve());
			changeAllTentacleState(0x8);
		}
		break;

	case ASTATE_UNK6:
		mTentacles[0]->changeStateAndFixNodes(0x9);
		break;
	}
}

void TBossGesso::gotTentacleDamage()
{
	if (mSpine->getLatestNerve() == &TNerveBGBeakDamage::theNerve())
		return;

	mSpine->reset();
	mSpine->setNext(&TNerveBGTentacleDamage::theNerve());
	mSpine->pushAfterCurrent(&TNerveBGWait::theNerve());

	unk1A0 = 1;
}

void TBossGesso::gotEyeDamage()
{
	if (mSpine->getLatestNerve() == &TNerveBGEyeDamage::theNerve()
	    || mSpine->getLatestNerve() == &TNerveBGBeakDamage::theNerve()
	    || mSpine->getLatestNerve() == &TNerveBGPollute::theNerve())
		return;

	mSpine->reset();
	mSpine->setNext(&TNerveBGEyeDamage::theNerve());
	mSpine->pushAfterCurrent(&TNerveBGWait::theNerve());
}

void TBossGesso::gotBeakDamage()
{
	if (mSpine->getLatestNerve() == &TNerveBGTentacleDamage::theNerve())
		return;

	if (mHitPoints > 0)
		--mHitPoints;

	if (mHitPoints == 0) {
		mSpine->setNext(&TNerveBGDie::theNerve());
	} else {
		mSpine->setNext(&TNerveBGBeakDamage::theNerve());
		mSpine->pushAfterCurrent(&TNerveBGWait::theNerve());
	}

	unk1A0 = 1;
}

void TBossGesso::changeAllTentacleState(int param_1)
{
	for (int i = 0; i < TENTACLE_NUM; ++i)
		if (mTentacles[i]->mState != 5 && !isTentacleBusy(mTentacles[i]))
			mTentacles[i]->changeStateAndFixNodes(param_1);
}

void TBossGesso::forceAllTentacleState(int param_1)
{
	for (int i = 0; i < TENTACLE_NUM; ++i)
		mTentacles[i]->changeStateAndFixNodes(param_1);
}

void TBossGesso::startPollute()
{
	unk178->setBckFromIndex(4);
	unk17C = 1;
}

void TBossGesso::stopPollute()
{
	unk178->setBckFromIndex(-1);
	unk17C = 0;
}

void TBossGesso::launchPolDrop()
{
	if (mPolDrop->getUnk58())
		return;

	JGeometry::TVec3<f32> local_14;
	if (checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
		local_14 = getPosition();
		local_14.x += 1.0f;
	} else {
		getJointTransByIndex(26, &local_14);
	}

	JGeometry::TVec3<f32> VStack_20;
	SMSCalcJumpVelocityXZ(SMS_GetMarioPos(), local_14, 32.0f, 0.2f, &VStack_20);
	mPolDrop->launch(local_14, VStack_20);

	MtxPtr ptr = getModel()->getAnmMtx(27);
	gpMarioParticleManager->emitAndBindToMtxPtr(BGESO_JPA_MS_BOGE_CANON_B, ptr,
	                                            0, nullptr);
	gpMarioParticleManager->emitAndBindToMtxPtr(BGESO_JPA_MS_BOGE_CANON_A, ptr,
	                                            0, nullptr);

	unk195 += 1;
}

// Both btp spellings the nerves paste (index 1 held at frame 1.5, index 2
// at frame 0) as one flag-selected body; this is the map's 0x8c. Calling it
// at the eight nerve sites compiles to the same code but adds 0x18 of frame
// per site under the current changeBck spelling, so the sites stay pasted.
void TBossGesso::setEyeDamageBtp(int index)
{
	if (index != 0) {
		getMActor()->setBtpFromIndex(1);
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl->setFrame(1.5f);
		ctrl->setRate(0.0f);
		getMActor()->resetDL();
	} else {
		getMActor()->setBtpFromIndex(2);
		J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl->setFrame(0.0f);
		getMActor()->resetDL();
	}
}

BOOL TBossGesso::tentacleHeld() const
{
	for (int i = 0; i < TENTACLE_NUM; ++i)
		if (mTentacles[i]->mState == 3)
			return true;

	return false;
}

BOOL TBossGesso::tentacleAttack()
{
	for (int i = 0; i < TENTACLE_NUM; ++i)
		if (mTentacles[i]->mState == 2)
			return true;

	return false;
}

BOOL TBossGesso::beakHeld() const { return !!mBeak->mHolder; }

BOOL TBossGesso::tentacleWait()
{
	for (int i = 0; i < TENTACLE_NUM; ++i)
		if (mTentacles[i]->mState == 1)
			return true;

	return false;
}

const char** TBossGesso::getBasNameTable() const { return bgeso_bastable; }

BOOL TBossGesso::receiveMessage(THitActor* sender, u32 message)
{
	if (sender->getActorType() == 0x1000001
	    && message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		gpMarioParticleManager->emit(PARTICLE_MS_ENM_WATHIT, &sender->mPosition,
		                             0, nullptr);
		return true;
	}

	return false;
}

void TBossGesso::doAttackSingle()
{
	if (getLatestNerve() != &TNerveBGPollute::theNerve())
		stopPollute();

	if (gpMarDirector->unk58 < 0x1E0
	    || gpMarDirector->isTalkOrDemoModeNow())
		return;

	if (unk1A8 > 0) {
		unk1A8 -= 1;
		return;
	}

	if (is2ndFightNow()) {
		JGeometry::TVec3<f32> delta = SMS_GetMarioPos();

		if (20.0f + delta.y < mPosition.y)
			return;

		delta -= mPosition;

		if (delta.squared() > 3610000.0f)
			return;
	}

	for (int i = 0; i < 2; ++i) {
		static const int idxarray[2] = { 1, 3 };
		TBGTentacle* tentacle        = mTentacles[idxarray[i]];

		f32 sightAngle = getSaveParam2()->mSLSightAngle.get();
		if (inSightAngle(0.5f * sightAngle) && tentacle->mState == 0) {
			JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
			delta -= tentacle->getFirstNode()->getPosition();

			f32 attackLen = getSaveParam2()->mSLSingleAttackLen.get();
			if (delta.squared() < attackLen * attackLen) {
				tentacle->changeStateAndFixNodes(1);
				break;
			}
		}
	}

	if (mTentacles[3]->isThing2() && mTentacles[1]->isThing2()) {
		if (!mTentacles[2]->isThing2() || !mTentacles[0]->isThing2()) {
			if (mTimeInCurrentAttackMode
			        > getSaveParam2()->mSLUnisonInter.get()
			    && is2ndFightNow())
				changeAttackMode(ASTATE_ROLL);

			return;
		}
	}

	if (tentacleHeld())
		return;

	JGeometry::TVec3<f32> delta = SMS_GetMarioPos();

	f32 unisonAttackLen2 = getSaveParam2()->mSLUnisonAttackLen.get();
	unisonAttackLen2 *= unisonAttackLen2;

	f32 forceUnisonLen2 = getSaveParam2()->mSLForceUnisonLen.get();
	forceUnisonLen2 *= forceUnisonLen2;

	// TODO: retail makes a third getSaveParam() call here whose result is
	// never read; the fetch it belonged to was dead code. Keeping the bare
	// call reproduces the instruction.
	getSaveParam2();

	delta -= mPosition;

	if (mBeak->mHolder != nullptr
	    && !(mTentacles[3]->isThing2() && mTentacles[1]->isThing2())) {
		for (int i = 0; i < 2; ++i) {
			static const int idxarray[2] = { 1, 3 };
			TBGTentacle* tentacle        = mTentacles[idxarray[i]];

			if (tentacle->mState != 1 && tentacle->mState != 4
			    && tentacle->mState != 5 && tentacle->mState != 3
			    && tentacle->mState != 6)
				tentacle->changeStateAndFixNodes(1);
		}

		return;
	}

	f32 distToMario2 = delta.squared();

	if (distToMario2 < forceUnisonLen2
	    && gpMarioOriginal->isTouchGround4cm()) {
		changeAttackMode(ASTATE_UNISON);
		return;
	}

	if (distToMario2 < unisonAttackLen2
	    && gpMarioOriginal->isTouchGround4cm()) {
		if (mTimeInCurrentAttackMode
		    > getSaveParam2()->mSLUnisonInter.get()) {
			if (is2ndFightNow())
				changeAttackMode(ASTATE_ROLL);
			else
				changeAttackMode(ASTATE_UNISON);
		}

		return;
	}

	if (tentacleWait())
		return;

	if (mCork->unkC != 0 && unk195 < 3) {
		f32 shootRadius2 = getSaveParam2()->mSLShootRadius.get();
		shootRadius2 *= shootRadius2;

		// Retail compares inSight() directly here: this is the one site where
		// TVec3::sub expands and the `li 1 / li 0; cmpwi` BOOL stands without
		// an inSightAngle() level (91.5 -> 93.3).
		// TODO: retail also expands MsGetRotFromZaxisY at this site (41
		// missing instructions, frame 0x1a8 vs 0x280) and inside the UNUSED
		// inSightAngle (map 0x16c, ours 0xc4), while calling it at depth 3
		// from every inSightAngle site. Our MathUtil.hpp spelling costs too
		// much for depth 2: a TU-local probe copy costing 8-10 filler units
		// inlines at depth 2 but not 3 and lifts this function to 99.5 with
		// every other caller unchanged; the if/else spellings tried (single
		// or multiple returns) all stay out of line. A shared-header fix.
		if (mTimeInCurrentAttackMode > getSaveParam2()->mSLUnisonInter.get()
		    && distToMario2 < shootRadius2 && (inSight() < 30.0f ? TRUE : FALSE))
			changeAttackMode(ASTATE_SHOOT);

		return;
	}

	if (is2ndFightNow()) {
		f32 sightAngle = getSaveParam2()->mSLSightAngle.get();
		if (inSightAngle(0.5f * sightAngle)
		    && mTimeInCurrentAttackMode
		        > getSaveParam2()->mSLUnisonInter.get()) {
			changeAttackMode(ASTATE_ROLL);
			unk195 = 0;
		}
	}
}

// Binding level worth +16 of low region, landing TBossGesso::doAttackShoot's
// frame at 0x98 (batch 124).
static inline TBossGessoParams* BossgessoGetSaveParam2(const TBossGesso* p)
{
	TBossGessoParams* saveParam2 = p->getSaveParam2();
	return saveParam2;
}

static inline TBGBeakHit* BossgessoGetBeak(const TBossGesso* p)
{
	TBGBeakHit* beak = p->mBeak;
	return beak;
}

static inline TItemManager* BossgessoGetItemManager()
{
	TItemManager* manager = gpItemManager;
	return manager;
}

// Mario's ear is 75 units above his feet.  Retail copies the vector into a
// second slot before the getInCubeNo() call, which is this by-value return.
static inline JGeometry::TVec3<f32> BossgessoMarioEarPos()
{
	JGeometry::TVec3<f32> earPos = SMS_GetMarioPos();
	earPos.y += 75.0f;
	return earPos;
}

static inline TMap* BossgessoGetMap()
{
	TMap* map = gpMap;
	return map;
}

static inline TMarDirector* BossgessoGetMarDirector()
{
	TMarDirector* director = gpMarDirector;
	return director;
}

static inline J3DModel* BossgessoGetModel(const TBossGesso* p)
{
	J3DModel* model = p->getModel();
	return model;
}

// +8 of pool per site. Two sites close TBossGesso::init and
// TNerveBGDie's frames (both were 0x18 short).
static inline MActor* BossgessoGetMActor(const TLiveActor* p)
{
	MActor* actor = p->getMActor();
	return actor;
}

// A bare-return fork over getModel() is the +4 rung this TU's nerve pools
// need (the binder BossgessoGetModel above is +8): one site closed
// TNerveBGPollute and TNerveBGTug.
static inline J3DModel* BossgessoModelOf(const TBossGesso* p)
{
	return p->getModel();
}

static inline MSound* BossgessoGetMSound()
{
	MSound* sound = SMSGetMSound();
	return sound;
}

static inline TMarioParticleManager* BossgessoGetParticleManager()
{
	TMarioParticleManager* manager = gpMarioParticleManager;
	return manager;
}

// +8 of low region at its one expansion in TNerveBGPollute (the named
// TStack_24 there is still 4 bytes low: one 4-byte inline level short).
static inline MActor* BossgessoGetUnk178(const TBossGesso* p)
{
	MActor* actor = p->unk178;
	return actor;
}

static inline int BossgessoGetUnk195(const TBossGesso* p) { return p->unk195; }

static inline TMario* BossgessoGetMario()
{
	TMario* mario = gpMarioOriginal;
	return mario;
}

static inline TTakeActor* BossgessoGetBeakHolder(const TBossGesso* p)
{
	TTakeActor* holder = BossgessoGetBeak(p)->mHolder;
	return holder;
}

void TBossGesso::doAttackDouble()
{
	if (BossgessoGetBeak(this)->mHolder != nullptr || tentacleHeld()) {
		changeAttackMode(ASTATE_SKIP_ROPE);
		return;
	}

	JGeometry::TVec3<f32> delta = mPosition;
	delta -= SMS_GetMarioPos();

	// Declare sightAngle first so it takes f30 and the squared length f29,
	// matching retail. The leftover 4-byte TVec3 slot (0x80 vs 0x84) is the
	// known-open `a = b - c` allocation-order residue.
	f32 sightAngle;
	f32 doubleAttackLen2 = BossgessoGetSaveParam2(this)->mSLDoubleAttackLen.get();
	doubleAttackLen2 *= doubleAttackLen2;

	sightAngle = BossgessoGetSaveParam2(this)->mSLSightAngle.get();
	BOOL inSight = inSightAngle(0.5f * sightAngle);
	if (inSight
	    && delta.squared() < doubleAttackLen2) {

		for (int i = 0; i < 2; ++i) {
			static const int idxarray[2] = { 0, 2 };
			TBGTentacle* tentacle        = mTentacles[idxarray[i]];
			if (tentacle->mState == 0)
				tentacle->changeStateAndFixNodes(1);
		}
	}

	if (mBeak->mHolder != nullptr)
		return;

	if (!mTentacles[3]->isThing2() || !mTentacles[1]->isThing2())
		changeAttackMode(ASTATE_SINGLE);
}

void TBossGesso::doAttackSkipRope()
{
	if (BossgessoGetBeakHolder(this) != nullptr) {
		changeAttackMode(ASTATE_SKIP_ROPE);
		return;
	}

	if (BossgessoGetBeak(this)->mHolder == nullptr && !tentacleHeld()) {
		changeAttackMode(ASTATE_SINGLE);
		return;
	}

	f32 sightAngle = BossgessoGetSaveParam2(this)->mSLSightAngle.get();
	if (inSightAngle(0.5f * sightAngle)) {
		for (int i = 0; i < 2; ++i) {
			static const int idxarray[2] = { 0, 2 };
			TBGTentacle* tentacle        = getTentacle(idxarray[i]);
			if (tentacle->mState != 2 && tentacle->mState != 1
			    && tentacle->mState != 4 && tentacle->mState != 5
			    && tentacle->mState != 3 && tentacle->mState != 6) {
				tentacle->changeStateAndFixNodes(1);
			}
		}
	}
}

void TBossGesso::doAttackUnison()
{
	if (mBeak->mHolder != nullptr) {
		changeAttackMode(ASTATE_SKIP_ROPE);
		return;
	}

	JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
	delta -= mPosition;

	f32 unisonAttackLen2 = BossgessoGetSaveParam2(this)->mSLUnisonAttackLen.get();
	unisonAttackLen2 *= unisonAttackLen2;

	f32 sightAngle = BossgessoGetSaveParam2(this)->mSLSightAngle.get();
	if (inSightAngle(0.5f * sightAngle)
	    && BossgessoGetMario()->isTouchGround4cm()
	    && delta.squared() < unisonAttackLen2) {

		BOOL bVar3 = true;
		for (int i = 0; i < TENTACLE_NUM; ++i) {
			TBGTentacle* tentacle = mTentacles[i];
			if (i == mTimeInCurrentAttackMode / 8 && tentacle->mState == 0) {
				tentacle->changeStateAndFixNodes(1);
				bVar3 = false;
			}
		}

		if (bVar3 && mTimeInCurrentAttackMode > 300)
			changeAttackMode(ASTATE_SINGLE);
	} else {
		for (int i = 0; i < TENTACLE_NUM; ++i) {
			TBGTentacle* tentacle = mTentacles[i];
			if (tentacle->mState == 1)
				return;

			if (!tentacle->isThing2() && tentacle->mState != 0
			    && tentacle->mState != 2 && tentacle->mState != 5)
				tentacle->changeStateAndFixNodes(2);
		}

		changeAttackMode(ASTATE_SINGLE);
	}
}

void TBossGesso::doAttackShoot()
{
	if (mBeak->mHolder != nullptr) {
		changeAttackMode(ASTATE_SINGLE);
		return;
	}

	if (mTimeInCurrentAttackMode > 120
	    && getSpine()->getLatestNerve() != &TNerveBGPolDrop::theNerve()) {
		changeAttackMode(ASTATE_SINGLE);
		return;
	}

	f32 sightAngle = BossgessoGetSaveParam2(this)->mSLSightAngle.get();
	if (!inSightAngle(0.5f * sightAngle))
		return;

	JGeometry::TVec3<f32> delta = *gpMarioPos;
	delta -= mPosition;

	f32 singleAttackLen2 = getSaveParam2()->mSLSingleAttackLen.get();
	singleAttackLen2 *= singleAttackLen2;

	if (delta.squared() < singleAttackLen2)
		changeAttackMode(ASTATE_SINGLE);
}

// Retail inlines this whole body into moveObject's ASTATE_GUARD case while
// keeping doAttackShoot a `bl`; the lever is the statement budget, not bytes.
// doAttackShoot sat at exactly 14 and needed one more statement (the early
// return below), and this body sat at 16 and needed two fewer -- the two
// nested `isThing2` guards collapse into one `||` inside the positive
// condition. moveObject 85.0 -> 97.8.
// TODO: the out-of-line copy is still 584 bytes against the map's 580.
void TBossGesso::doAttackGuard()
{
	if (mBeak->mHolder != nullptr) {
		changeAttackMode(ASTATE_SKIP_ROPE);
		return;
	}

	f32 sightAngle = BossgessoGetSaveParam2(this)->mSLSightAngle.get();
	if (!inSightAngle(0.5f * sightAngle))
		return;

	JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
	delta -= mPosition;

	f32 guardLen = getSaveParam2()->mSLGuardLen.get();
	if (guardLen * guardLen < delta.squared()
	    || (mTentacles[3]->isThing2() && mTentacles[1]->isThing2())) {
		changeAllTentacleState(0);
		changeAttackMode(ASTATE_SINGLE);
	}
}

void TBossGesso::doAttackRoll()
{
	if (getLatestNerve() == &TNerveBGRoll::theNerve())
		return;

	changeAllTentacleState(0);
	changeAttackMode(ASTATE_SINGLE);
}

// TODO: 97.8%, frame 0xe0 against 0xe8. At the ASTATE_GUARD expansion retail
// calls SMS_GetMarioPos out of line (its only bl in the TU, hence the weak
// copy) and multiplies 0.5f * sightAngle after inSight() returns: the
// inSightAngle -> inSight chain has one more inline level in retail (the map
// sizes agree: inSightAngle 0x16c vs our 0xc4, inSight 0x17c vs 0x184).
// Dropping the sightAngle local or wrapping the guard body were inert.
void TBossGesso::moveObject()
{
	TLiveActor::moveObject();

	if (mSpine->getLatestNerve() == &TNerveBGDie::theNerve())
		return;

	if (mSpine->getLatestNerve() == &TNerveBGPolDrop::theNerve())
		return;

	if (mSpine->getLatestNerve() == &TNerveBGPollute::theNerve())
		return;

	if (mSpine->getLatestNerve() == &TNerveBGBeakDamage::theNerve())
		return;

	switch (mAttackMode) {
	case ASTATE_SINGLE:
		doAttackSingle();
		break;

	case ASTATE_DOUBLE:
		doAttackDouble();
		break;

	case ASTATE_SKIP_ROPE:
		doAttackSkipRope();
		break;

	case ASTATE_UNISON:
		doAttackUnison();
		break;

	case ASTATE_SHOOT:
		doAttackShoot();
		break;

	case ASTATE_GUARD:
		doAttackGuard();
		break;

	case ASTATE_ROLL:
		doAttackRoll();
		break;
	}

	if (mSpine->getLatestNerve() != &TNerveBGTentacleDamage::theNerve()
	    && mSpine->getLatestNerve() != &TNerveBGBeakDamage::theNerve()
	    && mSpine->getLatestNerve() != &TNerveBGTug::theNerve()) {

		// TODO: inline?
		BOOL bVar4;
		if (mTentacles[0]->mState == 3)
			bVar4 = true;
		else if (mTentacles[1]->mState == 3)
			bVar4 = true;
		else if (mTentacles[2]->mState == 3)
			bVar4 = true;
		else if (mTentacles[3]->mState == 3)
			bVar4 = true;
		else
			bVar4 = false;

		if (bVar4 || mBeak->mHolder != nullptr)
			mSpine->pushNerve(&TNerveBGTug::theNerve());
	}

	mTimeInCurrentAttackMode += 1;
}

void TBossGesso::reset()
{
	for (int i = 0; i < TENTACLE_NUM; ++i) {
		mTentacles[i]->resetAllNodes(mPosition);
		mTentacles[i]->getFirstNode()->onUnk24();
	}

	if (SMSGetMarDirector()->getCurrentMap() == 3
	    && SMSGetMarDirector()->getCurrentStage() == 0)
		changeAttackMode(ASTATE_UNK6);
	else
		changeAttackMode(ASTATE_SINGLE);

	calcRootMatrix();
	getMActor()->getModel()->calc();
}

void TBossGesso::calcRootMatrix()
{
	if (getLatestNerve() == &TNerveBGDie::theNerve()
	    && getMActor()->checkCurBckFromIndex(6)) {
		mRotation = MsGetRotFromZaxis(mVelocity);
		MtxPtr mA = getModel()->getBaseTRMtx();

		MsMtxSetXYZRPH(mA, mPosition.x, mPosition.y, mPosition.z, mRotation.x,
		               mRotation.y, mRotation.z);

		Mtx local_50;

		MsMtxSetRotX(local_50, 90.0f);

		MTXConcat(mA, local_50, mA);

		getModel()->setBaseScale(mScaling);
		return;
	}

	TSpineEnemy::calcRootMatrix();
}

void TBossGesso::performInContainer(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		if (JDrama::TNameRefGen::search<THitActor>("container") == nullptr) {
			changeAttackMode(ASTATE_SINGLE);
		} else if (mTentacles[0]->mState != 4) {
			JGeometry::TVec3<f32> pos(11603.0f, 2114.3f, 2411.4f);
			mTentacles[0]->mNodes[0].setPosition(pos);
			pos.x = 11510.0f;
			mTentacles[0]->mNodes[1].setPosition(pos);
		}
	}

	mTentacles[0]->testPerform(cue, graphics);
}

void TBossGesso::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		if (mBeak->getHolder() != nullptr || tentacleHeld()) {
			if (gpMarioOriginal->mIntendedMag > 0.1f) {
				SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_PULL, &mPosition,
				                                0, nullptr, 0, 4);
			}
		}
	}

	if (cue & CUE_MOVE) {
		checkTakeMsg();

		if (mBeak->getHolder() != nullptr
		    && mTimeInCurrentAttackMode % 4 == 0) {
			rumblePad(1, mBeak->mPosition);
		}

		if (unk1AC > 0)
			unk1AC--;

		if (unk1AE > 0)
			unk1AE--;
	}

	if (mAttackMode == 6) {
		performInContainer(cue, graphics);
		return;
	}

	if (cue & CUE_ENTRY) {
		if (getLatestNerve() == &TNerveBGBeakDamage::theNerve()) {
			SMS_AddDamageFogEffect(mMActor->getModel()->getModelData(),
			                       mPosition, graphics);
		} else {
			SMS_ResetDamageFogEffect(mMActor->getModel()->getModelData());
		}

		mMActor->getModel()
		    ->getModelData()
		    ->getMaterialNodePointer(0)
		    ->getTevBlock()
		    ->setTevKColor(0, &unk190);
	}

	TSpineEnemy::perform(cue, graphics);
	mPolDrop->testPerform(cue, graphics);

	if (mLiveFlag & LIVE_FLAG_DEAD)
		return;

	if (cue & CUE_CALC_ANIM) {
		if (unk194 > 0) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    BGESO_JPA_MS_BOGE_WASH, getModel()->getAnmMtx(0), 1, this);
			unk194--;
		}
	}

	mCork->perform(cue, graphics);

	if (cue & CUE_CALC_VIEW) {
		TCircleShadowRequest request;

		MtxPtr joint = mMActor->getModel()->getAnmMtx(1);

		JGeometry::TVec3<f32> shadowPos;
		shadowPos.x      = joint[0][3];
		shadowPos.y      = mPosition.y;
		shadowPos.z      = joint[2][3];
		request.mPosition = shadowPos;

		JGeometry::TVec3<f32> right(joint[0][0], joint[1][0], joint[2][0]);
		JGeometry::TVec3<f32> front(joint[0][2], joint[1][2], joint[2][2]);

		request.mRadiusX = VECMag(right);
		request.mRadiusZ = VECMag(front);
		request.mRadiusX *= mScaledBodyRadius;
		request.mRadiusZ *= mScaledBodyRadius;
		request.mShadowType = getShadowType();
		request.mRotationY  = mRotation.y;

		gpBindShadowManager->request(request, getActorType());
	}

	mBeak->testPerform(cue, graphics);
	mLeftEye->testPerform(cue, graphics);
	mRightEye->testPerform(cue, graphics);
	mBody->testPerform(cue, graphics);

	if (cue & CUE_MOVE) {
		mMtxCalc->advanceMotionBlend(-unk188);
	}

	if (unk17C) {
		if (cue & CUE_CALC_ANIM) {
			MTXCopy(mMActor->getModel()->getBaseTRMtx(),
			        unk178->getModel()->getBaseTRMtx());
			unk178->calcAnm();
		}

		if (cue & CUE_ENTRY)
			gpPollution->stampModel(unk178->getModel());
	}

	if (cue & CUE_CALC_ANIM) {
		if (mLiveFlag & LIVE_FLAG_CLIPPED_OUT) {
			for (int i = 0; i < TENTACLE_NUM; ++i) {
				if (mTentacles[i]->mState != 4)
					mTentacles[i]->mNodes[0].setPosition(mPosition);
			}
		} else {
			static const int idxarray[] = { 2, 3, 5, 6 };

			for (int i = 0; i < TENTACLE_NUM; ++i) {
				if (mTentacles[i]->mState == 4)
					continue;

				JGeometry::TVec3<f32> trans;
				if (getJointTransByIndex(idxarray[i], &trans) >= 0)
					mTentacles[i]->mNodes[0].setPosition(trans);
			}
		}
	}

	for (int i = 0; i < TENTACLE_NUM; ++i) {
		if (cue & CUE_ENTRY) {
			if (getLatestNerve() == &TNerveBGBeakDamage::theNerve()) {
				mTentacles[i]->unk2C->offMakeDL();
				SMS_AddDamageFogEffect(
				    mTentacles[i]->unk2C->getModel()->getModelData(), mPosition,
				    graphics);
			} else {
				SMS_ResetDamageFogEffect(
				    mTentacles[i]->unk2C->getModel()->getModelData());
			}
		}

		mTentacles[i]->testPerform(cue, graphics);
	}

	if (cue & CUE_CALC_ANIM) {
		if (mMActor->checkCurBckFromIndex(14)
		    || mMActor->checkCurBckFromIndex(15)) {
			f32 len = lenFromToeToMario();
			SMSGetMSound()->startSoundActorWithInfo(MSD_SE_BS_GESO_ROLL,
			                                        &mPosition, nullptr, len, 0,
			                                        0, nullptr, 0, 4);
		}
	}

	if (cue & CUE_MOVE) {
		if (mBeak->mHolder != nullptr && unk190.color.a == 0) {
			// TODO: retail `bl`s showMessage here and inlines it at the
			// other two call sites, and showMessage is byte-exact at five
			// statements, so this site must sit at depth four -- three
			// inline levels of perform are still missing. Forcing the call
			// takes perform 98.3 -> 99.2. With showMessage(3) the flag is
			// 0, so the balloon always appears, which is why spelling the
			// console call directly here was byte-identical.
			if (!(isTentacleBusy(mTentacles[1])
			      && isTentacleBusy(mTentacles[3]))) {
				showMessage(3);
			}
		}
	}
}

TBossGessoManager::TBossGessoManager(const char* name)
    : TEnemyManager(name)
{
}

void TBossGessoManager::createModelData()
{
	static const TModelDataLoadEntry entry[] = {
		{ "bgeso_body.bmd", 0x10300000, 0 },
		{ "bgeso_hand.bmd", 0x10240000, 0 },
		{ "bgeso_shand.bmd", 0x200000, 0 },
		{ "bgeso_dirty_white.bmd", 0x10220000, 0 },
		{ "bgeso_osenball.bmd", 0x10220000, 0 },
		{ "bgeso_osenball_white.bmd", 0x10220000, 0 },
		{ "bgeso_kolk.bmd", 0x10220000, 0 },
		{ "bgeso_kolk_break.bmd", 0x10220000, 0 },
		{ nullptr, 0, 0 },
	};
	createModelDataArray(entry);
}

void TBossGessoManager::initJParticle()
{
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_a.jpa", 0x8c);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_b.jpa", 0x8d);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_c.jpa", 0x8e);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_attack_d.jpa", 0x8f);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_blur.jpa", 0x90);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_blur_f.jpa", 0x91);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_blur_j.jpa", 0x92);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_canon_a.jpa", 0x93);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_canon_b.jpa", 0x94);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_damage_a.jpa", 0x95);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_damage_b.jpa", 0x96);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hit_a.jpa", 0x97);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hit_b.jpa", 0x98);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hit_c.jpa", 0x99);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_hitdown.jpa", 0x9a);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_jump.jpa", 0x9b);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_kizetsu.jpa", 0x9c);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_kizetsu_r.jpa", 0x9d);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_odanhit_a.jpa", 0x9e);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_odanhit_b.jpa", 0x9f);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_osen.jpa", 0xa0);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_ase.jpa", 0x138);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_namida.jpa", 0x139);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_kiseki.jpa", 0x13a);
	SMS_LoadParticle("/scene/bgeso/jpa/ms_boge_wash.jpa", 0x13b);
}

void TBossGessoManager::load(JSUMemoryInputStream& stream)
{
	unk38 = new TBossGessoParams("/enemy/bossgesso.prm");
	TEnemyManager::load(stream);
	initJParticle();
}

// TODO: frame 0xd0 against retail 0xa0, all low-region temps. The pasted
// changeBck's accessor spelling is the knob (raw mMActorKeeper in joinAnm
// gives 0xa0 here) but it breaks Tug/Eye/Die, which match with it as is.
DEFINE_NERVE(TNerveBGWait, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		if (self->getHitPoints() == 1) {
			self->changeBck(27);
		} else if (self->getHitPoints() == 2) {
			self->changeBck(26);
		} else {
			self->changeBck(25);
		}

		self->setGoalPathMario();

		self->getMActor()->setBtpFromIndex(2);

		J3DFrameCtrl* ctrl3 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl3->setFrame(0.0f);
		self->getMActor()->resetDL();
	}

	JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
	delta -= self->mPosition;
	f32 len   = delta.length();
	f32 fVar2;
	if (len > 800.0f)
		fVar2 = 1.0f;
	else
		fVar2 = 3000.0f / len;
	self->walkToCurPathNode(0.0f, fVar2 * self->getTurnSpeed(), 0.0f);

	return false;
}

DEFINE_NERVE(TNerveBGEyeDamage, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(5);

		if (BossgessoGetBeakHolder(self) == nullptr
		    && self->mAttackMode != 2) {
			self->changeAttackMode(TBossGesso::ASTATE_UNISON);
			self->changeAllTentacleState(1);
		}

		self->getMActor()->setBtpFromIndex(1);
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl->setFrame(1.5f);
		ctrl->setRate(0.0f);
		self->getMActor()->resetDL();
	}

	if (self->getMActor()->curAnmEndsNext()) {
		self->getMActor()->setBtpFromIndex(2);
		J3DFrameCtrl* ctrl = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl->setFrame(0.0f);
		self->getMActor()->resetDL();
		return true;
	}

	BossgessoGetParticleManager()->emitAndBindToMtxPtr(
	    BGESO_JPA_MS_BOGE_NAMIDA, BossgessoGetModel(self)->getAnmMtx(7), 1,
	    self);
	BossgessoGetParticleManager()->emitAndBindToMtxPtr(
	    BGESO_JPA_MS_BOGE_NAMIDA, BossgessoGetModel(self)->getAnmMtx(4), 1,
	    self);

	if (self->unk1AE == 0) {
		self->unk1AE = 0x78;
		SMSGetMSound()->startSoundActor(MSD_SE_BS_GESO_WATER_HIT,
		                                &self->mPosition, 0, nullptr, 0, 4);
	}

	if (self->unk190.color.a != 0) {
		self->unk190.color.a -= 1;
		self->unk194 = 5;
	}

	return false;
}

DEFINE_NERVE(TNerveBGBeakDamage, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(7);
		self->getMActor()->setBtkFromIndex(0);
		self->changeAllTentacleState(8);

		J3DFrameCtrl* ctrl4 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTK);
		ctrl4->setRate(1.0f);
		ctrl4->setFrame(0.0f);

		self->getMActor()->setBtpFromIndex(1);

		J3DFrameCtrl* ctrl3 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl3->setFrame(1.5f);
		ctrl3->setRate(0.0f);

		self->getMActor()->resetDL();

		if (gpMarDirector->mMap == 3 || gpMarDirector->mMap == 59) {
			MSBgm::stopBGM(MSD_BGM_BOSS, 10);
			MSMainProc::setBossNotDamagedFlag(false);
		}
	}

	if (spine->getTime() == 1)
		self->mCork->crush();

	if (spine->getTime() == 12) {
		BossgessoGetParticleManager()->emitAndBindToMtxPtr(
		    BGESO_JPA_MS_BOGE_HIT_A, BossgessoGetModel(self)->getAnmMtx(27), 0,
		    nullptr);
		BossgessoGetParticleManager()->emitAndBindToMtxPtr(
		    BGESO_JPA_MS_BOGE_HIT_C, self->getModel()->getAnmMtx(27), 0,
		    nullptr);
		BossgessoGetParticleManager()->emitAndBindToMtxPtr(
		    BGESO_JPA_MS_BOGE_HIT_B, self->getModel()->getAnmMtx(27), 0,
		    nullptr);
	}

	if (spine->getTime() == 18) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_BOGE_BEAK_HIT, 1.0f);
		self->rumblePad(1, self->mPosition);
	}

	if (spine->getTime() == 234) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_BOGE_TUMBLE, 1.0f);
		self->rumblePad(2, self->mPosition);
	}

	if (spine->getTime() == 510) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_BOGE_LAND, 1.0f);
		self->rumblePad(1, self->mPosition);
	}

	if (spine->getTime() == 510 || spine->getTime() == 236) {
		JGeometry::TVec3<f32> local_28;
		self->getJointTransByIndex(47, &local_28);
		const TBGCheckData* data;
		local_28.y = BossgessoGetMap()->checkGround(local_28.x, local_28.y + 500.0f,
		                                local_28.z, &data);

		BossgessoGetParticleManager()->emit(BGESO_JPA_MS_BOGE_HITDOWN, &local_28, 0,
		                             nullptr);
	}

	if (spine->getTime() == 40) {
		BossgessoGetParticleManager()->emitAndBindToMtxPtr(
		    BGESO_JPA_MS_BOGE_KIZETSU, BossgessoGetModel(self)->getAnmMtx(7), 0,
		    nullptr);
		BossgessoGetParticleManager()->emitAndBindToMtxPtr(
		    BGESO_JPA_MS_BOGE_KIZETSU_R, BossgessoGetModel(self)->getAnmMtx(4), 0,
		    nullptr);
	}

	if (self->getMActor()->curAnmEndsNext()) {

		self->forceAllTentacleState(0);

		J3DFrameCtrl* ctrl4 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTK);
		ctrl4->setRate(0.0f);
		ctrl4->setFrame(0.0f);

		spine->pushAfterCurrent(&TNerveBGPollute::theNerve());
		if (gpMarDirector->mMap == 3 || gpMarDirector->mMap == 59) {
			MSBgm::startBGM(MSD_BGM_BOSSGESO_2DN3RD);

			// Outside every sound cube the boss theme plays on a muted
			// track; inside one the Sirena hotel (map 59) hands the volume
			// back to the cube fader.
			if (gpCubeSoundChange != nullptr
			    && gpCubeSoundChange->getInCubeNo(BossgessoMarioEarPos())
			           == -1) {
				MSBgm::setTrackVolume(1, 0.0f, 0, 0);
			} else if (gpMarDirector->mMap == 59
			           && MSStageCubeFade::smInstance != nullptr) {
				MSStageCubeFade::smInstance->setBgmVolumeForce();
			}
		}

		return true;
	}

	return false;
}

// TODO: frame 0x80 against retail 0x78. Dropping getMActor() at
// curAnmEndsNext is 0 (the accessor decays). The +8 lives in the
// pasted changeBck, whose getMActor()->mMActor rung disagrees in
// sign across the nine callers.
DEFINE_NERVE(TNerveBGTentacleDamage, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(3);
	}

	if (spine->getTime() == 30 || spine->getTime() == 304) {
		JGeometry::TVec3<f32> local_28;
		self->getJointTransByIndex(47, &local_28);
		const TBGCheckData* data;
		local_28.y = gpMap->checkGround(local_28.x, local_28.y + 500.0f,
		                                local_28.z, &data);

		gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_HITDOWN, &local_28, 0,
		                             nullptr);
	}

	if (spine->getTime() == 10) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_BOGE_TE_HIT, 1.0f);
		self->rumblePad(1, self->mPosition);
	}

	if (spine->getTime() == 304) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_BOGE_LAND, 1.0f);
		self->rumblePad(1, self->mPosition);
	}

	if (self->getMActor()->curAnmEndsNext()) {
		if (!self->tentacleHeld())
			return true;

		spine->pushAfterCurrent(&TNerveBGTentacleDamage::theNerve());
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGTug, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(5);
	}

	gpMarioParticleManager->emitAndBindToMtxPtr(
	    BGESO_JPA_MS_BOGE_ASE, BossgessoModelOf(self)->getAnmMtx(47), 1, self);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    BGESO_JPA_MS_BOGE_NAMIDA, self->getModel()->getAnmMtx(7), 1, self);
	gpMarioParticleManager->emitAndBindToMtxPtr(
	    BGESO_JPA_MS_BOGE_NAMIDA, self->getModel()->getAnmMtx(4), 1, self);

	if (self->mBeak->mHolder != nullptr) {
		JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
		delta -= self->mPosition;
		// Read raw: the const f32& from TParamT::get() is 8 bytes of frame.
		// TODO: frame exact now, but delta still sits 4 bytes low (0xa0
		// against retail's 0xa4); 4 bytes of low pool are still missing.
		f32 lim = self->getSaveParam2()->mSLBeakLengthDamage.value;

		if (delta.length() >= lim) {
			self->getMActor()->setBtpFromIndex(1);

			J3DFrameCtrl* ctrl3 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
			ctrl3->setFrame(1.5f);
			ctrl3->setRate(0.0f);

			self->getMActor()->resetDL();
			if (!self->getMActor()->checkCurBckFromIndex(9))
				self->changeBck(9);
		}
	}

	if (self->getMActor()->curAnmEndsNext()) {
		if (self->tentacleHeld())
			self->changeBck(5);
		else
			return true;
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed() * 10.0f, 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBGDie, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(2);

		// Two getMActor binders land the low region exactly.
		// TODO: frame 0x1b8 against retail's 0x1c0: every slot below 0x190
		// matches, but retail keeps an unreferenced 12 bytes under local_24
		// (0x19c in retail, 0x190 here), which also pushes the int->float
		// conversion temp from 0x1a0 to 0x1a8.
		BossgessoGetMActor(self)->setBtpFromIndex(1);

		J3DFrameCtrl* ctrl3 = BossgessoGetMActor(self)->getFrameCtrl(ANM_TYPE_BTP);
		ctrl3->setFrame(1.5f);
		ctrl3->setRate(0.0f);

		self->getMActor()->resetDL();

		if (BossgessoGetMarDirector()->mMap == 3 || BossgessoGetMarDirector()->mMap == 59) {
			MSBgm::stopTrackBGMs(7, 10);
			MSMainProc::setBossLivesFlag(false);
		} else if (BossgessoGetMarDirector()->mMap == 9) {
			MSBgm::stopTrackBGM(1, 10);
			MSMainProc::setBossLivesFlagOnlyFlag(false);
		}

		if (BossgessoGetMarDirector()->mMap == 9) {
			gpMarDirector->fireStartDemoCamera("bgeso_fall_camera3", nullptr,
			                                   -1, 0.0f, true, nullptr, 0,
			                                   nullptr, JDrama::TFlagT<u16>(0));
		} else if (!self->is2ndFightNow()) {
			gpMarDirector->fireStartDemoCamera("bgeso_fall_camera", nullptr, -1,
			                                   0.0f, true, nullptr, 0, nullptr,
			                                   JDrama::TFlagT<u16>(0));
		} else {
			gpMarDirector->fireStartDemoCamera("bgeso_fall_camera2", nullptr,
			                                   -1, 0.0f, true, nullptr, 0,
			                                   nullptr, JDrama::TFlagT<u16>(0));
		}

		if (BossgessoGetMarDirector()->mMap == 3 || BossgessoGetMarDirector()->mMap == 59) {
			BossgessoGetItemManager()->makeShineAppearWithDemo(
			    "シャイン（ボス用）", "ボスシャインカメラ", self->mPosition.x,
			    6000.0f + self->mPosition.y, self->mPosition.z);
		}

		TNameKuriManager* nameKuriMgr
		    = JDrama::TNameRefGen::search<TNameKuriManager>(
		        "ナメクリマネージャー");
		if (nameKuriMgr)
			nameKuriMgr->killChildren();

		BossgessoGetMSound()->startSoundActor(MSD_SE_BS_GESO_MHIT_NOBOICE,
		                                &self->mPosition, 0, nullptr, 0, 4);
	}

	if (BossgessoGetMarDirector()->mMap == 9 && spine->getTime() >= 740
	    && spine->getTime() <= 750) {
		BossgessoGetMSound()->startSoundActor(MSD_SE_OBJ_QUAKE, &self->mPosition, 0,
		                                nullptr, 0, 4);

		if (spine->getTime() == 745) {
			self->unk1A4 = 1.0f;
			SMSRumbleMgr->start(8, &self->unk1A4);
		}
	}

	if (self->getMActor()->checkCurBckFromIndex(2)
	    && self->getMActor()->curAnmEndsNext()) {

		self->changeBck(6);
		self->changeAllTentacleState(8);

		JGeometry::TVec3<f32> local_24;
		local_24.set(self->getPosition().x, -5000.0f,
		             7000.0f + self->getPosition().z);

		self->setGoalPath(local_24);

		self->mVelocity = self->calcVelocityToJumpToY(local_24, 50.0f,
		                                              self->getGravityY());
		self->onLiveFlag(LIVE_FLAG_AIRBORNE);
		return false;
	}

	if (self->isReachedToGoal()) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		self->mVelocity       = zero;
		self->mLinearVelocity = zero;
		self->onLiveFlag(LIVE_FLAG_UNK10);
	}

	if (self->isReachedToGoal() && BossgessoGetMarDirector()->unk124 != 3) {

		self->changeAllTentacleState(0);
		self->kill();

		THitActor* block = JDrama::TNameRefGen::search<THitActor>(
		    "マーレボスゲッソー用ブロック");

		if (block != nullptr) {
			block->receiveMessage(self, HIT_MESSAGE_ATTACK);
			MSBgm::stopTrackBGM(1, 10);
			MSBgm::setTrackVolume(0, 1.0f, 5, 0);
		}

		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGPollute, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(10);
		BossgessoGetUnk178(self)->setBckFromIndex(4);
		self->unk17C = 1;
		self->changeAllTentacleState(0);
		self->getMActor()->setBtpFromIndex(2);

		J3DFrameCtrl* ctrl3 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl3->setFrame(0.0f);
		self->getMActor()->resetDL();
	}

	if (spine->getTime() == 90) {
		gpMarioParticleManager->emitAndBindToSRTMtxPtr(
		    BGESO_JPA_MS_BOGE_OSEN, BossgessoModelOf(self)->getAnmMtx(27), 0,
		    nullptr);
	}

	if (spine->getTime() == 226) {
		JGeometry::TVec3<f32> TStack_24;
		self->getJointTransByIndex(1, &TStack_24);
		gpMarioParticleManager->emit(BGESO_JPA_MS_BOGE_JUMP, &TStack_24, 0,
		                             nullptr);
		self->unk190.color.a = 230;
	}

	if (spine->getTime() == 230) {
		gpCameraShake->startShake(CAM_SHAKE_MODE_BOGE_LAND, 1.0f);
		self->rumblePad(2, self->mPosition);
	}

	if (self->getMActor()->curAnmEndsNext()) {
		self->unk178->setBckFromIndex(-1);
		self->unk17C = 0;
		return true;
	}

	return false;
}

DEFINE_NERVE(TNerveBGPolDrop, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();

	if (spine->getTime() == 0) {
		self->changeBck(1);
		self->changeAllTentacleState(0);
		self->getMActor()->setBtpFromIndex(2);

		J3DFrameCtrl* ctrl3 = self->getMActor()->getFrameCtrl(ANM_TYPE_BTP);
		ctrl3->setFrame(0.0f);
		self->getMActor()->resetDL();
	}

	J3DFrameCtrl* ctrl0 = self->getMActor()->getFrameCtrl(ANM_TYPE_BCK);
	if (83.0f < ctrl0->getFrame() && ctrl0->getFrame() < 87.0f) {
		self->launchPolDrop();
	}

	if (self->getMActor()->curAnmEndsNext()) {
		JGeometry::TVec3<f32> delta = SMS_GetMarioPos();
		delta -= self->mPosition;

		f32 shootRadius2 = BossgessoGetSaveParam2(self)->mSLShootRadius.value;
		shootRadius2 *= shootRadius2;
		f32 singleAttackLen2
		    = BossgessoGetSaveParam2(self)->mSLSingleAttackLen.get();
		singleAttackLen2 *= singleAttackLen2;

		if (BossgessoGetUnk195(self) < 3) {
			f32 len = delta.squared();
			if (singleAttackLen2 <= len && len < shootRadius2) {
				spine->pushAfterCurrent(&TNerveBGPolDrop::theNerve());
			}
		}

		return true;
	}

	self->walkToCurPathNode(0.0f, self->getTurnSpeed(), 0.0f);
	return false;
}

DEFINE_NERVE(TNerveBGRoll, TLiveActor)
{
	TBossGesso* self = (TBossGesso*)spine->getBody();
	MActor* mactor   = self->getMActor();

	if (self->unk1A1 == 0) {
		if (spine->getTime() == 0)
			self->changeBck(19);

		if (mactor->curAnmEndsNext()) {
			if (mactor->checkCurBckFromIndex(14)) {
				if (self->unk196 < 3) {
					self->changeBck(14);
					self->unk196 += 1;
				} else {
					self->changeBck(18);
					self->unk196 = 0;
				}
			} else {
				if (mactor->checkCurBckFromIndex(19))
					self->changeBck(14);
				else
					return true;
			}
		}

		return false;
	} else {
		if (spine->getTime() == 0)
			self->changeBck(17);

		if (mactor->curAnmEndsNext()) {
			if (mactor->checkCurBckFromIndex(15)) {
				if (self->unk196 < 3) {
					self->changeBck(15);
					self->unk196 += 1;
				} else {
					self->changeBck(16);
					self->unk196 = 0;
				}
			} else {
				if (mactor->checkCurBckFromIndex(17))
					self->changeBck(15);
				else
					return true;
			}
		}
	}

	return false;
}
