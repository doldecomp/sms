#include <Enemy/EffectEnemy.hpp>
#include <Strategic/Spine.hpp>
#include <System/Particles.hpp>
#include <M3DUtil/MActor.hpp>
#include <Map/Map.hpp>
#include <Player/MarioAccess.hpp>
#include <MSound/MSound.hpp>
#include <MSound/MSoundSE.hpp>
#include <MSound/SoundEffects.hpp>
#include <Strategic/ObjModel.hpp>
#include <Map/MapCollisionData.hpp>

// rogue includes needed for matching sinit & bss
#include <M3DUtil/InfectiousStrings.hpp>
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
	return new TEffectEnemy("エフェクト敵");
}

void TEffectEnemyManager::initSetEnemies() { }

TEffectEnemy::TEffectEnemy(const char* name)
    : TWalkerEnemy(name)
{
	unk194 = 0;
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
	mLiveFlag |= LIVE_FLAG_DEAD;
	mHitFlags |= HIT_FLAG_NO_COLLISION;
}

void TEffectEnemy::forceKill()
{
	// Both paths fall into a single kill() call, as the original does.
	if (mGroundPlane->isIllegalData()
	    || !(mGroundPlane->isDeathPlane() || mGroundPlane->isPool()
	         || mGroundPlane->isWaterSurface())
	    || isAirborne() || checkLiveFlag(LIVE_FLAG_UNK10)) {
		if (gpMap->isInArea(mPosition.x, mPosition.z))
			return;
	}

	kill();
}

void TEffectEnemy::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (mLiveFlag & LIVE_FLAG_DEAD)
		return;

	if (cue & 1)
		TWalkerEnemy::moveObject();

	if ((cue & 2) && !(mLiveFlag & LIVE_FLAG_CLIPPED_OUT)) {
		JGeometry::TVec3<f32> scale;
		// Both are u8 (getMaxHitPoints() narrows the param), so this is an
		// int division; reading the hit points through the accessor rather
		// than the field is what puts the dividend in the higher register.
		VECScale(&mScaling, &scale,
		         (f32)(getHitPoints() / getMaxHitPoints()));

		gpMarioParticleManager->emitAndBindToPosPtr(0x1ED, &mPosition, 3, this);
		gpMarioParticleManager->emitAndBindToPosPtr(0x135, &mPosition, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(0x136, &mPosition, 1, this);
		gpMarioParticleManager->emitAndBindToPosPtr(0x137, &mPosition, 1, this);
	}

	THitActor::perform(cue, graphics);
}

void TEffectEnemy::reset() { TWalkerEnemy::reset(); }

void TEffectEnemy::behaveToWater(THitActor* water)
{
	if (mHitPoints > 1) {
		TSmallEnemy::behaveToWater(water);
		return;
	}
	kill();
}

void TEffectEnemy::sendAttackMsgToMario()
{
	switch (unk194) {
	case 0:
		SMS_SendMessageToMario(this, 0xA);
		kill();
		break;
	case 1:
		SMS_SendMessageToMario(this, 9);
		break;
	default:
		SMS_SendMessageToMario(this, 0xE);
		break;
	}
}

void TEffectEnemy::setDeadAnm()
{
	gpMarioParticleManager->emitAndBindToPosPtr(0x8B, &mPosition, 0, nullptr);
	gpMSound->startSoundActor(MSD_SE_BS_WANWAN_TO_COOL, &mPosition);
	onLiveFlag(LIVE_FLAG_UNK20000);
}

// UNUSED, 0xf4 in the map, between setDeadAnm and the destructor. No call site
// in this TU is large enough to hold it (perform is 0x150), so it is either
// dead or inlined somewhere that is not reconstructed yet.
// TODO: incorrect size (empty stub).
void TEffectEnemy::emitEffect() { }
