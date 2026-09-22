#include <Enemy/BossHanachan.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Enemy/BossHanachanSub.hpp>
#include <Camera/CameraShake.hpp>
#include <Camera/SunMgr.hpp>
#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>

static const f32 sEmitSandFrameFoot[2] = { 14.0f, 34.0f };
static const f32 sSnortStepFrames[3] = { 21.0f, 36.0f, 55.0f };

void TBossHanachan::staticLoadParticle()
{
	SMS_LoadParticle("ms_boha_sandsmo.jpa", 0x76);
	SMS_LoadParticle("ms_boha_sand.jpa", 0x77);
	SMS_LoadParticle("ms_boha_jouki_r_a.jpa", 0x78);
	SMS_LoadParticle("ms_boha_jouki_r_b.jpa", 0x79);
	SMS_LoadParticle("ms_boha_jouki_l_a.jpa", 0x7A);
	SMS_LoadParticle("ms_boha_jouki_l_b.jpa", 0x7B);
	SMS_LoadParticle("ms_boha_hamon_a.jpa", 0x7C);
	SMS_LoadParticle("ms_boha_hamon_b.jpa", 0x7D);
	SMS_LoadParticle("ms_boha_crash_a.jpa", 0x7E);
	SMS_LoadParticle("ms_boha_jouki2_r_a.jpa", 0x169);
	SMS_LoadParticle("ms_boha_jouki2_r_b.jpa", 0x16A);
	SMS_LoadParticle("ms_boha_jouki2_l_a.jpa", 0x16B);
	SMS_LoadParticle("ms_boha_jouki2_l_b.jpa", 0x16C);
	SMS_LoadParticle("ms_boha_sandsmo_sl.jpa", 0x16D);
	SMS_LoadParticle("ms_boha_sand_sl.jpa", 0x16E);
	SMS_LoadParticle("ms_boha_kizetsu.jpa", 0x16F);
}

// TODO: 98.3%, frame 0xc8 vs 0x90, no missing/extra/opcode differences.
// Batch 136 measured the whole lever curve for the 56 bytes: a TU-local
// binding over any one of mHead, unk178, mChangeParams, mBodies[i]->mFeet[],
// mBodies[i]->mLegMtx[] or gpSunMgr->unk20 is +8 alone, but in combination the
// cost is 24 + 8n, so exactly *five* of them reach 0xc8 and every five-subset
// tried gives the identical result. It is not worth committing: at frame-exact
// the operand mismatches only fall from 103 to 90 and two residues stay, and
// both are known-open. (1) The named `position` vector sits at 0x6c where
// retail has it at 0x70 -- the same 4-byte pool-order shift emitCamShake_
// below has, and no 8-byte lever can pay for 4. (2) The callee-saved rotation:
// retail keeps `this` in r31 and the nerve in r30 (we have them swapped) and
// permutes the three loop counters with it; that is the `this`-vs-pool-base
// swap of docs/catalog/frame-gaps.md, "pass 131". Fix the 4 bytes first --
// it is shared with emitCamShake_, so one construct explains both.
// Batch 145 measured the declaration knob here and it is inert: a shared
// `int i` hoisted to function scope, `int i` plus `int foot` hoisted, a
// block-scoped `int i` for the first two loops and `position` declared above
// `waterHeight` all give exactly the same 103 markers. Declaration order
// cannot reach a `this`-versus-local swap (frame-gaps.md, "batch 145").
// cc28: the rotation is also inert to the nerve fetched through a TU-local
// reference out-parameter level, a direct-return fork, a binder, raw
// `mSpine->getLatestNerve()` and a named `self` receiver (103 markers each).
// emitCamShake_'s 4-byte shift was closed by a bool wrapper plus params forks.
void TBossHanachan::emitParticle_()
{
	const TNerveBase<TLiveActor>* nerve = getLatestNerve();
	if (nerve == &TNerveBossHanachanDead::theNerve())
		return;
	f32 waterHeight = gpSunMgr->unk20;
	JGeometry::TVec3<f32> position;
	if (nerve != &TNerveBossHanachanSnort::theNerve()
	    && mHead->unk100->getWaterHitCounter() > 0) {
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x169, mHead->mRightNoseMtx, 1, mHead);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x16B, mHead->mLeftNoseMtx, 1, mHead);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x16A, mHead->mRightNoseMtx, 1, mHead);
		gpMarioParticleManager->emitAndBindToMtxPtr(
		    0x16C, mHead->mLeftNoseMtx, 1, mHead);
	}
	if (nerve == &TNerveBossHanachanTumble::theNerve()
	    || (nerve == &TNerveBossHanachanDamage::theNerve()
	        && mMarchSpeed > 0.001f)) {
		for (int i = 0; i < 8; ++i) {
			position = unk178->mPoints[i].mPosition;
			if (position.y > waterHeight)
				gpMarioParticleManager->emitAndBindToPosPtr(
				    0x16E, &mBodies[i]->unk154, 1, mBodies[i]);
		}
		for (int i = 0; i < 8; ++i) {
			position = unk178->mPoints[i].mPosition;
			if (position.y > waterHeight)
				gpMarioParticleManager->emitAndBindToPosPtr(
				    0x16D, &mBodies[i]->unk154, 1, mBodies[i]);
		}
	}
	if (nerve == &TNerveBossHanachanGraphWander::theNerve()) {
		for (int i = 0; i < 8; ++i) {
			J3DFrameCtrl* ctrl = mBodies[i]->mMActor->getFrameCtrl(0);
			for (int foot = 0; foot < 2; ++foot) {
				if (ctrl->checkPass(sEmitSandFrameFoot[foot])) {
					MtxPtr mtx = mBodies[i]->mFeet[foot]->mJointMtx;
					if (MsRandF()
					    < mChangeParams->mSLParticleProbability.get()) {
						if (mtx[1][3] < waterHeight) {
							MtxPtr leg = mBodies[i]->mLegMtx[foot];
							position.set(leg[0][3], waterHeight, leg[2][3]);
							gpMarioParticleManager->emit(0x7C, &position, 0,
							                            nullptr);
							gpMarioParticleManager->emit(0x7D, &position, 0,
							                            nullptr);
						} else {
							position.set(mtx[0][3], mtx[1][3], mtx[2][3]);
							gpMarioParticleManager->emit(0x77, &position, 0,
							                            nullptr);
							gpMarioParticleManager->emit(0x76, &position, 0,
							                            nullptr);
						}
					}
				}
			}
		}
	} else if (nerve == &TNerveBossHanachanSnort::theNerve()) {
		if (mHead->mMActor->getFrameCtrl(0)->checkPass(134.0f)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x78, mHead->mRightNoseMtx, 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x7A, mHead->mLeftNoseMtx, 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x79, mHead->mRightNoseMtx, 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    0x7B, mHead->mLeftNoseMtx, 0, nullptr);
		}
	} else if (nerve == &TNerveBossHanachanDown::theNerve()) {
		gpMarioParticleManager->emitAndBindToMtxPtr(0x16F, mHead->unk108, 1,
		                                             mHead);
	}
}

// TODO: 99.9%, frame 0x38 vs retail 0x40; all 77 instructions match and every
// r1 displacement in retail is 8 higher, so the residue is one small object at
// the bottom of the local area. Batch 74 traced it to MSound::startSoundActor:
// retail called that wrapper here rather than spelling the gate check out, and
// the wrapper bound the callee's JAISound* return to a named local. Switch the
// tail below to `gpMSound->startSoundActor(0x2884, &mSandPillarPosition, 0,
// nullptr, 0, 4)` once the header change lands (it needs ten other call sites
// converted to the raw spelling first). See docs/catalog/frame-gaps.md,
// "Closure batch 74".
void TBossHanachan::emitOneTimeSandPillar_(TBossHanachanPartsBody* part)
{
	onLiveFlag(0x10000);
	MtxPtr mtx = part->unk108;
	mSandPillarPosition.set(mtx[0][3], mPosition.y, mtx[2][3]);
	const TLiveActor* sand = part->getSandActor_();
	if (sand) {
		mSandPillarPosition.x = 0.5f * (mSandPillarPosition.x + sand->mPosition.x);
		mSandPillarPosition.z = 0.5f * (mSandPillarPosition.z + sand->mPosition.z);
		mSandPillarPosition.y = sand->mPosition.y;
	}
	gpMarioParticleManager->emit(0x7E, &mSandPillarPosition, 0, nullptr);
	MtxPtr base = mSandPillarActor->getModel()->getBaseTRMtx();
	base[0][3] = mSandPillarPosition.x;
	base[1][3] = mSandPillarPosition.y;
	base[2][3] = mSandPillarPosition.z;
	mSandPillarActor->setBckFromIndex(0x25);
	mSandPillarActor->setBtkFromIndex(2);
	mSandPillarActor->setBrkFromIndex(2);
	gpCameraShake->startShake((EnumCamShakeMode)8, 1.0f);
	gpMSound->startSoundActor(0x2884, &mSandPillarPosition);
}

// Direct-return levels (cc28): with the grounded test through a bool
// wrapper, the params fork at three of the four distance sites is what puts
// MsSqrtf's `volatile float y` on retail's 0x40 at frame 0x78 (the old +16
// binder at one site was 4 bytes off in pool order).
static inline TBossHanachanCommonSaveParams*
BossHanachanEffectCommonParams(const TBossHanachan* p)
{
	return p->mCommonParams;
}

static inline bool BossHanachanEffectGround4cm()
{
	return SMS_IsMarioTouchGround4cm();
}

// The hoisted `int j` is what puts the outer counter in r25 and the inner in
// r26 as retail does; declaring it inside the inner `for` swaps them.
void TBossHanachan::emitCamShake_()
{
	if (gpMarDirector->mState == 1)
		return;
	const TNerveBase<TLiveActor>* nerve = getLatestNerve();
	J3DFrameCtrl* ctrl = mBodies[0]->mMActor->getFrameCtrl(0);
	bool grounded = BossHanachanEffectGround4cm();
	if (nerve == &TNerveBossHanachanGraphWander::theNerve()) {
		f32 distance = MsSqrtf(mDistToMarioSquared);
		f32 ratio;
		if (distance <= mCommonParams->mSLCamShakeMaxDist.get())
			ratio = 1.0f;
		else if (distance >= BossHanachanEffectCommonParams(this)
		                         ->mSLCamShakeZeroDist.get())
			ratio = 0.0f;
		else
			ratio = MsClamp(
			    CLBCalcRatio(
			        BossHanachanEffectCommonParams(this)->mSLCamShakeZeroDist.get(),
			        BossHanachanEffectCommonParams(this)->mSLCamShakeMaxDist.get(),
			        distance),
			    0.0f, 1.0f);
		int j;
		for (int i = 0; i < 2; ++i) {
			if (ctrl->checkPass(sEmitSandFrameFoot[i])) {
				gpCameraShake->startShake((EnumCamShakeMode)0xB, ratio);
				if (grounded)
					for (j = 0; j < 8; ++j)
						SMSRumbleMgr->start(8, &mBodies[j]->unk154);
			}
		}
	} else if (nerve == &TNerveBossHanachanSnort::theNerve()) {
		for (int i = 0; i < 3; ++i) {
			if (ctrl->checkPass(sSnortStepFrames[i])) {
				gpCameraShake->startShake((EnumCamShakeMode)0xA, 1.0f);
				if (grounded)
					SMSRumbleMgr->start(8, (f32*)nullptr);
			}
		}
	} else if (nerve == &TNerveBossHanachanGetUp::theNerve()) {
		switch (mBodies[0]->mCurrentAnm) {
		case BOSS_HANACHAN_ANM_UNK9:
		case BOSS_HANACHAN_ANM_UNK12:
			if (ctrl->checkPass(9.0f)) {
				gpCameraShake->startShake((EnumCamShakeMode)0xC, 1.0f);
				if (grounded)
					SMSRumbleMgr->start(8, (f32*)nullptr);
				if (checkLiveFlag(LIVE_FLAG_UNK20000))
					MSBgm::stopTrackBGM(1, 30);
			}
			break;
		}
	}
}
