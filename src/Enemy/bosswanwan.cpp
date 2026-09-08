#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>
#include <Strategic/ObjManager.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/FlagManager.hpp>
#include <System/MarDirector.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Application.hpp>

void TBossWanwan::kill() { return; }

BOOL TBossWanwan::receiveMessage(THitActor* sender, u32 message)
{
	u32 actorType = sender->getActorType();
	if (actorType == 0x80000001) {
		return false;
	} else if (actorType == 0x1000001) {
		// fabricated
		if (!this->msInvincible) {
			gpMarioParticleManager->emit(0xE7, &sender->mPosition, 0, 0);
			if (this->mHitPoints == 0) {
				if (true) {
					if (gpMSound->gateCheck(0x28d1)) {
						MSoundSESystem::MSoundSE::startSoundActor(
						    0x28d1, &this->mPosition, 0, nullptr, 0, 4);
					}
				}
			} else if (this->mHitPoints == 1) {
				MtxPtr iVar3 = this->getModel()->getAnmMtx(1);
				gpMarioParticleManager->emitAndBindToMtxPtr(
				    0xb0, (iVar3 + 0x58) + 0x30, 0, 0);
				if (gpMSound->gateCheck(0x28c5)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x28c5, &this->mPosition, 0, nullptr, 0, 4);
				}

			} else {
				if (gpMSound->gateCheck(0x28be)) {
					MSoundSESystem::MSoundSE::startSoundActor(
					    0x28be, &this->mPosition, 0, nullptr, 0, 4);
				}
			}

			if (this->mHitPoints != 0) {
				this->mHitPoints--;
			}

			this->mWaterHitCount++;
			return true;
		} else {
			return true;
		}

	} else {
		if (actorType == 0x4000005a) {
			sender->receiveMessage(this, HIT_MESSAGE_HIP_DROP);
			this->mHitPoints = 0;
			this->mWaterHitCount++;
			if (!this->unk0) {
				this->unk0 = true;
			}

			MtxPtr mtx = this->getModel()->getAnmMtx(1); // fabricated index
			gpMarioParticleManager->emitAndBindToMtxPtr(0xB0, mtx, 0, nullptr);

			if (gpMSound->gateCheck(0x28c5)) {
				MSoundSESystem::MSoundSE::startSoundActor(
				    0x28c5, &this->mPosition, 0, nullptr, 0, 4);
			}
		}
	}

	return TSpineEnemy::receiveMessage(sender, message);
}

void TBossWanwan::shakeCamera(int shakeType)
{
	if (!SMS_IsMarioTouchGround4cm()) {
		return;
	}

	f32 dist = this->mDistToMarioSquared;

	if (0.0f < dist) {
		// TODO: This is a bit tricky. There's still some problems here
		f64 r = __frsqrte(dist);
		dist  = dist * (0.5 * r * -(dist * (r * r) - 3.0));
	}

	TBWParams* params  = (TBWParams*)this->getSaveParam();
	f32 shakeLengthMax = params->mSLShakeLengthMax.get();

	TBWParams* params2    = (TBWParams*)this->getSaveParam();
	f32 shakeLengthMaxHP0 = params2->mSLShakeLengthMaxHP0.get();

	f32 ratio = 1.0;
	if (this->mMActor->checkCurBckFromIndex(0) == 0) {
		TBWParams* params3 = (TBWParams*)this->getSaveParam();
		ratio = (f32)this->mHitPoints / (f32)params3->mSLBWHitPointMax.get();
	}

	f32 effectiveDist
	    = shakeLengthMax * ratio + shakeLengthMaxHP0 * (1.0f - ratio);

	f32 delta = effectiveDist - dist;

	if (delta < 0.0f) {
		return;
	}

	f32 power = delta / effectiveDist;
	if (1.0f < power) {
		power = 1.0f;
	}

	power *= ratio;

	gpCameraShake->startShake((EnumCamShakeMode)shakeType, power);
	SMSRumbleMgr->start(8, &this->mPosition);
}

TSpineEnemy* TBossWanwanManager::createEnemyInstance()
{
	return new TBossWanwan();
}

void TBossWanwanManager::createModelData() { }
