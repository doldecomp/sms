#include <Enemy/EffectEnemy.hpp>
#include <MSound/MSound.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Player/MarioAccess.hpp>
#include <Strategic/ObjModel.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

void TEffectEnemyManager::load(JSUMemoryInputStream& stream)
{
	TSmallEnemyManager::load(stream);
	unk38 = new TWalkerEnemyParams("/enemy/moveFireEffect.prm");
}

void TEffectEnemyManager::loadAfter() { JDrama::TNameRef::loadAfter(); }

TSpineEnemy* TEffectEnemyManager::createEnemyInstance()
{
	return new TEffectEnemy;
}

void TEffectEnemyManager::initSetEnemies() { }

TEffectEnemy::TEffectEnemy(const char* name)
    : TWalkerEnemy(name)
    , unk194(0)
{
}

void TEffectEnemy::init(TLiveManager* manager)
{
	TWalkerEnemy::init(manager);
	mActorType = 0x10000005;
}

void TEffectEnemy::setMActorAndKeeper()
{
	mMActorKeeper = new TMActorKeeper(mManager, 1);
	mMActor       = mMActorKeeper->createMActor("default.bmd", 3);
}

void TEffectEnemy::kill()
{
	setDeadAnm();
	onLiveFlag(LIVE_FLAG_DEAD);
	onHitFilter(HIT_FILTER_NO_COLLISION);
}

void TEffectEnemy::forceKill()
{
	if (!(mGroundPlane->isIllegalData()
	      || (!mGroundPlane->isDeathPlane() && !mGroundPlane->isPool()
	          && !mGroundPlane->isWaterSurface())
	      || isAirborne() || checkLiveFlag(LIVE_FLAG_UNK10))
	    || !gpMap->isInArea(mPosition.x, mPosition.z)) {
		kill();
	}
}

void TEffectEnemy::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (!checkLiveFlag(LIVE_FLAG_DEAD)) {
		if (cue & CUE_MOVE)
			TWalkerEnemy::moveObject();

		if ((cue & CUE_CALC_ANIM) && !checkLiveFlag(LIVE_FLAG_CLIPPED_OUT)) {
			emitEffect();
		}

		THitActor::perform(cue, graphics);
	}
}

void TEffectEnemy::reset() { TWalkerEnemy::reset(); }

void TEffectEnemy::behaveToWater(THitActor* hit)
{
	if (mHitPoints > 1)
		TSmallEnemy::behaveToWater(hit);
	else
		kill();
}

void TEffectEnemy::sendAttackMsgToMario()
{
	switch (unk194) {
	case 0:
		SMS_SendMessageToMario(this, HIT_MESSAGE_UNKA);
		kill();
		break;
	case 1:
		SMS_SendMessageToMario(this, HIT_MESSAGE_UNK9);
		break;
	default:
		SMS_SendMessageToMario(this, HIT_MESSAGE_ATTACK);
		break;
	}
}

void TEffectEnemy::setDeadAnm()
{
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_OFF,
	                                            &mPosition, 0, nullptr);
	SMSGetMSound()->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition, 0,
	                                nullptr, 0, 4);
	onLiveFlag(LIVE_FLAG_UNK20000);
}

void TEffectEnemy::emitEffect()
{
	JGeometry::TVec3<f32> scale;
	VECScale(&mScaling, &scale, mHitPoints / (u8)getMaxHitPoints());

	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_C,
	                                            &mPosition, 3, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_A,
	                                            &mPosition, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_B,
	                                            &mPosition, 1, this);
	gpMarioParticleManager->emitAndBindToPosPtr(PARTICLE_MS_MOE_FIRE_D,
	                                            &mPosition, 1, this);
}
