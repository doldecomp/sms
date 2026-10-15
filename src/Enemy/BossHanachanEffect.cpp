
#include <Enemy/BossHanachan.hpp>
#include <Enemy/BossHanachanParts.hpp>
#include <Enemy/BossHanachanSub.hpp>
#include <Enemy/BossHanachanChangeSaveParams.hpp>
#include <Camera/CameraShake.hpp>
#include <Camera/SunMgr.hpp>
#include <Camera/cameralib.hpp>
#include <M3DUtil/MActor.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <System/Particles.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <macros.h>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

static const f32 sEmitSandFrameFoot[] = { 14.0f, 34.0f };
static const f32 sSnortStepFrames[]   = { 21.0f, 36.0f, 55.0f };

void TBossHanachan::staticLoadParticle()
{
	SMS_LoadParticle("ms_boha_sandsmo.jpa", PARTICLE_MS_BOHA_SANDSMO);
	SMS_LoadParticle("ms_boha_sand.jpa", PARTICLE_MS_BOHA_SAND);
	SMS_LoadParticle("ms_boha_jouki_r_a.jpa", PARTICLE_MS_BOHA_JOUKI_R_A);
	SMS_LoadParticle("ms_boha_jouki_r_b.jpa", PARTICLE_MS_BOHA_JOUKI_R_B);
	SMS_LoadParticle("ms_boha_jouki_l_a.jpa", PARTICLE_MS_BOHA_JOUKI_L_A);
	SMS_LoadParticle("ms_boha_jouki_l_b.jpa", PARTICLE_MS_BOHA_JOUKI_L_B);
	SMS_LoadParticle("ms_boha_hamon_a.jpa", PARTICLE_MS_BOHA_HAMON_A);
	SMS_LoadParticle("ms_boha_hamon_b.jpa", PARTICLE_MS_BOHA_HAMON_B);
	SMS_LoadParticle("ms_boha_crash_a.jpa", COLUMSAND_JPA_MS_BOHA_CRASH_A);
	SMS_LoadParticle("ms_boha_jouki2_r_a.jpa", PARTICLE_MS_BOHA_JOUKI2_R_A);
	SMS_LoadParticle("ms_boha_jouki2_r_b.jpa", PARTICLE_MS_BOHA_JOUKI2_R_B);
	SMS_LoadParticle("ms_boha_jouki2_l_a.jpa", PARTICLE_MS_BOHA_JOUKI2_L_A);
	SMS_LoadParticle("ms_boha_jouki2_l_b.jpa", PARTICLE_MS_BOHA_JOUKI2_L_B);
	SMS_LoadParticle("ms_boha_sandsmo_sl.jpa", PARTICLE_MS_BOHA_SANDSMO_SL);
	SMS_LoadParticle("ms_boha_sand_sl.jpa", PARTICLE_MS_BOHA_SAND_SL);
	SMS_LoadParticle("ms_boha_kizetsu.jpa", PARTICLE_MS_BOHA_KIZETSU);
}

// TODO: counter test, register allocation and stack frame still differ.
void TBossHanachan::emitParticle_()
{
	JGeometry::TVec3<f32> position;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	if (nerve == &TNerveBossHanachanDead::theNerve())
		return;
	f32 sandHeight = gpSunMgr->unk20;
	if (nerve != &TNerveBossHanachanSnort::theNerve()) {
		if (mHead->unk100->mWaterHitCounter > 0) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI2_R_A, mHead->unk114[1], 1, mHead);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI2_L_A, mHead->unk114[0], 1, mHead);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI2_R_B, mHead->unk114[1], 1, mHead);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI2_L_B, mHead->unk114[0], 1, mHead);
		}
	}
	if (nerve == &TNerveBossHanachanTumble::theNerve()
	    || (nerve == &TNerveBossHanachanDamage::theNerve()
	        && mMarchSpeed > 0.001f)) {
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
			position = unk178->mPoints[i].mPosition;
			if (position.y > sandHeight)
				gpMarioParticleManager->emitAndBindToPosPtr(
				    PARTICLE_MS_BOHA_SAND_SL, &mBodyParts[i]->unk154, 1,
				    mBodyParts[i]);
		}
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
			position = unk178->mPoints[i].mPosition;
			if (position.y > sandHeight)
				gpMarioParticleManager->emitAndBindToPosPtr(
				    PARTICLE_MS_BOHA_SANDSMO_SL, &mBodyParts[i]->unk154, 1,
				    mBodyParts[i]);
		}
	}
	if (nerve == &TNerveBossHanachanGraphWander::theNerve()) {
		for (int i = 0; i < ARRAY_COUNT(mBodyParts); ++i) {
			J3DFrameCtrl* frameCtrl
			    = mBodyParts[i]->getMActor()->getFrameCtrl(0);
			for (int j = 0; j < ARRAY_COUNT(sEmitSandFrameFoot); ++j) {
				if (frameCtrl->checkPass(sEmitSandFrameFoot[j])) {
					MtxPtr footMtx = mBodyParts[i]->unk118[j]->unk6C;
					const f32& probability
					    = unk1C0->mSLParticleProbability.get();
					if (MsRandF() < probability) {
						if (footMtx[1][3] < sandHeight) {
							MtxPtr surfaceMtx = mBodyParts[i]->unk14C[j];
							position.set(surfaceMtx[0][3], sandHeight,
							             surfaceMtx[2][3]);
							gpMarioParticleManager->emit(
							    PARTICLE_MS_BOHA_HAMON_A, &position, 0,
							    nullptr);
							gpMarioParticleManager->emit(
							    PARTICLE_MS_BOHA_HAMON_B, &position, 0,
							    nullptr);
						} else {
							position.set(footMtx[0][3], footMtx[1][3],
							             footMtx[2][3]);
							gpMarioParticleManager->emit(PARTICLE_MS_BOHA_SAND,
							                             &position, 0, nullptr);
							gpMarioParticleManager->emit(
							    PARTICLE_MS_BOHA_SANDSMO, &position, 0,
							    nullptr);
						}
					}
				}
			}
		}
	} else if (nerve == &TNerveBossHanachanSnort::theNerve()) {
		if (mHead->getMActor()->getFrameCtrl(0)->checkPass(134.0f)) {
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI_R_A, mHead->unk114[1], 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI_L_A, mHead->unk114[0], 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI_R_B, mHead->unk114[1], 0, nullptr);
			gpMarioParticleManager->emitAndBindToMtxPtr(
			    PARTICLE_MS_BOHA_JOUKI_L_B, mHead->unk114[0], 0, nullptr);
		}
	} else if (nerve == &TNerveBossHanachanDown::theNerve()) {
		gpMarioParticleManager->emitAndBindToMtxPtr(PARTICLE_MS_BOHA_KIZETSU,
		                                            mHead->unk108, 1, mHead);
	}
}

void TBossHanachan::emitOneTimeSandPillar_(TBossHanachanPartsBody* body)
{
	onLiveFlag(LIVE_FLAG_UNK10000);
	unk1A0.set(body->unk108[0][3], mPosition.y, body->unk108[2][3]);
	const TLiveActor* sand = body->getSandActor_();
	if (sand) {
		unk1A0.x = 0.5f * (unk1A0.x + sand->mPosition.x);
		unk1A0.z = 0.5f * (unk1A0.z + sand->mPosition.z);
		unk1A0.y = sand->mPosition.y;
	}
	gpMarioParticleManager->emit(COLUMSAND_JPA_MS_BOHA_CRASH_A, &unk1A0, 0,
	                             nullptr);
	MtxPtr mtx = unk19C->getModel()->getBaseTRMtx();
	mtx[0][3]  = unk1A0.x;
	mtx[1][3]  = unk1A0.y;
	mtx[2][3]  = unk1A0.z;
	unk19C->setBckFromIndex(37);
	unk19C->setBtkFromIndex(2);
	unk19C->setBrkFromIndex(2);
	gpCameraShake->startShake(CAM_SHAKE_MODE_UNK8, 1.0f);
	SMSGetMSound()->startSoundActor(MSD_SE_BS_HANA_TUMBLE, &unk1A0, 0, nullptr,
	                                0, 4);
}

void TBossHanachan::emitCamShake_()
{
	if (gpMarDirector->mState == TMarDirector::STATE_UNK1)
		return;
	const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
	J3DFrameCtrl* frameCtrl = mBodyParts[0]->getMActor()->getFrameCtrl(0);
	bool marioOnGround      = SMS_IsMarioTouchGround4cm();
	if (nerve == &TNerveBossHanachanGraphWander::theNerve()) {
		f32 distance = MsSqrtf(mDistToMarioSquared);
		f32 strength;
		if (distance <= unk1BC->mSLCamShakeMaxDist.get())
			strength = 1.0f;
		else if (distance >= unk1BC->mSLCamShakeZeroDist.get())
			strength = 0.0f;
		else
			strength = MsClamp(CLBCalcRatio(unk1BC->mSLCamShakeZeroDist.get(),
			                                unk1BC->mSLCamShakeMaxDist.get(),
			                                distance),
			                   0.0f, 1.0f);
		for (int i = 0; i < ARRAY_COUNT(sEmitSandFrameFoot); ++i) {
			if (frameCtrl->checkPass(sEmitSandFrameFoot[i])) {
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNKB, strength);
				if (marioOnGround)
					for (int j = 0; j < ARRAY_COUNT(mBodyParts); ++j)
						SMSRumbleMgr->start(8, (Vec*)&mBodyParts[j]->unk154);
			}
		}
	} else if (nerve == &TNerveBossHanachanSnort::theNerve()) {
		for (int i = 0; i < ARRAY_COUNT(sSnortStepFrames); ++i) {
			if (frameCtrl->checkPass(sSnortStepFrames[i])) {
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNKA, 1.0f);
				if (marioOnGround)
					SMSRumbleMgr->start(8, (f32*)nullptr);
			}
		}
	} else if (nerve == &TNerveBossHanachanGetUp::theNerve()) {
		switch (mBodyParts[0]->mCurAnm) {
		case BOSS_HANACHAN_ANM_UNK9:
		case BOSS_HANACHAN_ANM_UNK12:
			if (frameCtrl->checkPass(9.0f)) {
				gpCameraShake->startShake(CAM_SHAKE_MODE_UNKC, 1.0f);
				if (marioOnGround)
					SMSRumbleMgr->start(8, (f32*)nullptr);
				if (checkLiveFlag(LIVE_FLAG_UNK20000))
					MSBgm::stopTrackBGM(1, 30);
			}
			break;
		}
	}
}
