#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <Enemy/BossWanwan.hpp>
#include <Camera/CameraShake.hpp>
#include <MarioUtil/RumbleMgr.hpp>
#include <Player/MarioAccess.hpp>
#include <M3DUtil/MActor.hpp>
#include <M3DUtil/MActorAnm.hpp>

void TBossWanwan::kill() { return; }
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
