#include <Map/PollutionLayer.hpp>
#include <Map/PollutionManager.hpp>
#include <Player/MarioAccess.hpp>
#include <System/EmitterViewObj.hpp>
#include <System/Particles.hpp>
#include <MSound/MSound.hpp>
#include <MSound/SoundEffects.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <stdlib.h>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

f32 TPollutionLayer::mAreaMinRate         = 0.7f;
f32 TPollutionLayer::mSpreadArea          = 2000.0f;
s32 TPollutionLayer::mSpreadFrequency     = 3;
f32 TPollutionLayer::mFireArea            = 1600.0f;
s32 TPollutionLayer::mFireEffectWaitTime  = 20;
f32 TPollutionLayer::mThunderArea         = 1000.0f;
u32 TPollutionLayer::mThunderScaleRate    = 0; // UNUSED
f32 TPollutionLayer::mGlassWallArea       = 1000.0f;
u32 TPollutionLayer::mGlassWallScaleRate  = 0; // UNUSED
s32 TPollutionLayer::mGlassWallEffectTime = 120;

void TPollutionLayer::changeType(u16) { }

bool TPollutionLayer::getPollutedPosNear(f32 range, JGeometry::TVec3<f32>* dest)
{
	TPollutionPos& pos = mPos;
	for (int i = 0; i < 5; ++i) {
		f32 x   = (MsRandF() - 0.5f) * (mAreaMinRate + MsRandF());
		dest->x = x * range + SMS_GetMarioPos().x;
		f32 z   = (MsRandF() - 0.5f) * (mAreaMinRate + MsRandF());
		dest->z = z * range + SMS_GetMarioPos().z;

		if (isInArea(dest->x, 0.0f, dest->z)) {
			int texX = getTexPosS(dest->x);
			int texZ = getTexPosS(dest->z);
			if (pos.isInArea(texX, texZ)) {
				dest->y = pos.getDepthWorld(texX, texZ);
				if (dest->y > SMS_GetMarioPos().y)
					return false;
				if (mPollutionMap[mPos.index(texX, texZ)] != 0)
					return true;
			}
		}
	}
	return false;
}

bool TPollutionLayer::getPollutedPos(f32 range, JGeometry::TVec3<f32>* dest)
{
	for (int i = 0; i < 5; ++i) {
		dest->x = range * (MsRandF() - 0.5f) + gpMarioPos->x;
		dest->y = gpMarioPos->y;
		dest->z = range * (MsRandF() - 0.5f) + gpMarioPos->z;
		if (isPolluted(dest->x, dest->y, dest->z))
			return true;
	}
	return false;
}

void TPollutionLayer::changeEffectScale(const JGeometry::TVec3<f32>&, f32) { }

void TPollutionLayer::spread()
{
	if (mSpreadTimer < mSpreadFrequency) {
		mSpreadTimer += 1;
	} else {
		mSpreadTimer = 0;
		JGeometry::TVec3<f32> spread;
		if (getPollutedPosNear(mSpreadArea, &spread))
			gpPollution->stamp(1, spread.x, spread.y, spread.z, 128.0f);
	}
}

void TPollutionLayer::electric()
{
	if (getPollutedPosNear(mThunderArea,
	                       &mEffectPositions[mCurEffectPosIndex])) {
		mEffectTimer += 1;
		if (mEffectTimer > 15) {
			SMSGetMSound()->startSoundSet(MSD_SE_EF_ELEC,
			                              &mEffectPositions[mCurEffectPosIndex],
			                              0, 0.0f, 0, 0, 4);
			gpMarioParticleManager->emit(MAP_POLLUTION_MS_THUNDER_S,
			                             &mEffectPositions[mCurEffectPosIndex],
			                             0, this);
			mCurEffectPosIndex += 1;
			if (mCurEffectPosIndex >= mEffectPositionsCapacity)
				mCurEffectPosIndex = 0;
			mEffectTimer = 0;
		}
	}
}

void TPollutionLayer::glassWall()
{
	JGeometry::TVec3<f32> pos;
	if (getPollutedPos(mGlassWallArea, &pos)) {
		static s32 counter = 0;
		if (counter < mGlassWallEffectTime)
			counter += 1;
		else
			counter = 0;
	}
}

#pragma dont_inline on
void TPollutionLayer::fire()
{
	if (getPollutedPosNear(mFireArea, &mEffectPositions[mCurEffectPosIndex])) {
		mEffectTimer += 1;
		if (mEffectTimer > mFireEffectWaitTime) {
			SMSGetMSound()->startSoundSet(MSD_SE_EF_FIRE,
			                              &mEffectPositions[mCurEffectPosIndex],
			                              0, 0.0f, 0, 0, 4);
			if (JPABaseEmitter* em = gpMarioParticleManager->emit(
			        MAP_POLLUTION_MS_NEWFIRE_B,
			        &mEffectPositions[mCurEffectPosIndex], 2, this)) {
				em->setScale(JGeometry::TVec3<f32>(1.5f, 1.5f, 1.5f));
			}
			if (JPABaseEmitter* em = gpMarioParticleManager->emit(
			        MAP_POLLUTION_MS_NEWFIRE_A,
			        &mEffectPositions[mCurEffectPosIndex], 0, this)) {
				em->setScale(JGeometry::TVec3<f32>(1.5f, 1.5f, 1.5f));
			}
			mCurEffectPosIndex += 1;
			if (mCurEffectPosIndex >= mEffectPositionsCapacity)
				mCurEffectPosIndex = 0;
			mEffectTimer = 0;
		}
	}
}
#pragma dont_inline off

void TPollutionLayer::action()
{
	if (getPlaneType() != 0)
		return;

	switch (mPollutionType) {
	case POLLUTION_TYPE_FIRE:
		fire();
		break;
	case POLLUTION_TYPE_ELECTRIC:
		electric();
		break;
	case POLLUTION_TYPE_GLASS_WALL:
		glassWall();
		break;
	}

	int tries = 0;
	bool ok;
	JGeometry::TVec3<f32>* pos = &mEffectPositions[mCurEffectPosIndex];

	f32 area = mSpreadArea;

	while (true) {
		pos->x = area * (MsRandF() - 0.5f) + gpMarioPos->x;
		pos->y = gpMarioPos->y;
		pos->z = area * (MsRandF() - 0.5f) + gpMarioPos->z;
		if (isPolluted(pos->x, pos->y, pos->z)) {
			ok = true;
			break;
		}
		tries += 1;
		if (tries >= 5) {
			ok = false;
			break;
		}
	}

	if (ok) {
		if (getPollutionType() != POLLUTION_TYPE_FIRE
		    && getPollutionType() != POLLUTION_TYPE_UNK7) {
			if (mEffectTimer > 15) {
				gpMarioParticleManager->emitWithRotate(
				    PARTICLE_MS_RAKU_INDAWA,
				    &mEffectPositions[mCurEffectPosIndex], 0,
				    (s16)(0.005493164f * (f32)*gpMarioAngleY), 0, 2, nullptr);
				mCurEffectPosIndex += 1;
				if (mCurEffectPosIndex >= mEffectPositionsCapacity)
					mCurEffectPosIndex = 0;
				mEffectTimer = 0;
			}
			mEffectTimer += 1;
		}

		if (mFlags & FLAG_CAN_SPREAD)
			spread();
	}
}
