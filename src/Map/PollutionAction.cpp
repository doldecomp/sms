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

void TPollutionLayer::changeType(u16 type) { mPollutionType = type; }

// TODO: 99.8%. Instruction-exact; frame 0xb0 vs 0x98, and every referenced
// slot (the three int-to-float magic pairs) is exactly 24 bytes higher in
// retail, so the whole residue is 24 dead bytes at the bottom of the local
// area. Same family as `TPollutionPos::isSame` in docs/catalog/frame-gaps.md
// ("The carrier has to be a callee with no matching out-of-line copy"): the
// inlined callees here are TPollutionPos::index/isInArea/getDepthWorld, whose
// out-of-line copies are emitted and already match, so the bytes cannot be
// theirs. TPollutionLayer::action() is 40 bytes short with the same shape.
// getPollutedPosNear's 24 dead low bytes are these two binding levels: each is
// +8 alone and the pair is +24 (batch 136). Named `u8* map` / `f32 marioY`
// locals only reach +8 together, so the real helpers are still unidentified.
// TODO: promote once they are.
static inline u8* PollutionMap(const TPollutionLayer* p)
{
	u8* map = p->mPollutionMap;
	return map;
}

static inline f32 PollutionMarioY()
{
	f32 y = SMS_GetMarioPos().y;
	return y;
}

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
				if (dest->y > PollutionMarioY())
					return false;
				if (PollutionMap(this)[mPos.index(texX, texZ)] != 0)
					return true;
			}
		}
	}
	return false;
}

bool TPollutionLayer::getPollutedPos(f32 range, JGeometry::TVec3<f32>* dest)
{
	for (int i = 0; i < 5; ++i) {
		dest->x = range * (MsRandF() - 0.5f) + SMS_GetMarioX();
		dest->y = SMS_GetMarioY();
		dest->z = range * (MsRandF() - 0.5f) + SMS_GetMarioZ();
		if (isPolluted(dest->x, dest->y, dest->z))
			return true;
	}
	return false;
}

void TPollutionLayer::changeEffectScale(const JGeometry::TVec3<f32>&, f32) { }

void TPollutionLayer::spread()
{
	JGeometry::TVec3<f32> scratch;
	JGeometry::TVec3<f32> scratch2;
	if (mSpreadTimer < mSpreadFrequency) {
		mSpreadTimer += 1;
	} else {
		mSpreadTimer = 0;
		JGeometry::TVec3<f32> spread;
		if (getPollutedPosNear(mSpreadArea, &spread))
			gpPollution->pollute(spread.x, spread.y, spread.z, 128.0f);
	}
}

// action()'s 40 dead low bytes are three uninitialised non-trivial TVec3
// locals of the callees it inlines: one here and two in spread(). They cost no
// instructions in either copy. The split matters -- three in spread() alone,
// or one in each of electric()/glassWall()/spread(), leaves the two live
// vectors 12 bytes high; only 1 + 2 puts every slot where retail has it.
// TODO: nothing reads them, so what retail declared here is still unknown.
void TPollutionLayer::electric()
{
	JGeometry::TVec3<f32> scratch;
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

// Exact. The scale vector has to be **one** function-scope local shared by
// both emitters: measured in closure batch 83, every per-site spelling costs
// two 12-byte slots instead of one (unnamed `TVec3(1.5f, 1.5f, 1.5f)`
// temporary, `TVec3(1.5f)`, a per-site named local, an uninitialised local plus
// `setAll`, and calling setGlobalDynamicsScale/setGlobalParticleScale
// separately all give frame 0x48 against retail's 0x30), while writing
// mGlobalDynamicsScale/mGlobalParticleScale directly gives 0x18. So retail
// binds the reference once and MWCC charges one slot for the object and one for
// the inlined setter's parameter binding.
//
// Retail calls fire() from action() at depth 1: the two guards are early
// returns (the same bytes as the nested ifs, two statements more), which takes
// the body over the depth-1 budget. Splitting the scale into field
// assignments or two setters per site also tips it but costs 24 bytes of
// frame.
void TPollutionLayer::fire()
{
	JGeometry::TVec3<f32> scale(1.5f, 1.5f, 1.5f);
	if (!getPollutedPosNear(mFireArea, &mEffectPositions[mCurEffectPosIndex]))
		return;

	mEffectTimer += 1;
	if (mEffectTimer <= mFireEffectWaitTime)
		return;

	SMSGetMSound()->startSoundSet(MSD_SE_EF_FIRE,
	                              &mEffectPositions[mCurEffectPosIndex], 0,
	                              0.0f, 0, 0, 4);
	if (JPABaseEmitter* em = gpMarioParticleManager->emit(
	        MAP_POLLUTION_MS_NEWFIRE_B, &mEffectPositions[mCurEffectPosIndex],
	        2, this)) {
		em->setGlobalScale(scale);
	}
	if (JPABaseEmitter* em = gpMarioParticleManager->emit(
	        MAP_POLLUTION_MS_NEWFIRE_A, &mEffectPositions[mCurEffectPosIndex],
	        0, this)) {
		em->setGlobalScale(scale);
	}
	mCurEffectPosIndex += 1;
	if (mCurEffectPosIndex >= mEffectPositionsCapacity)
		mCurEffectPosIndex = 0;
	mEffectTimer = 0;
}

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
	case POLLUTION_TYPE_SINK:
	case POLLUTION_TYPE_SLIP:
	case POLLUTION_TYPE_INSTAKILL:
	case POLLUTION_TYPE_SAFE:
	case POLLUTION_TYPE_UNK7:
	case POLLUTION_TYPE_UNK8:
		break;
	}

	JGeometry::TVec3<f32>* pos = &mEffectPositions[mCurEffectPosIndex];

	if (getPollutedPos(mSpreadArea, pos)) {
		if (getPollutionType() != POLLUTION_TYPE_FIRE
		    && getPollutionType() != POLLUTION_TYPE_UNK7) {
			if (mEffectTimer > 15) {
				gpMarioParticleManager->emitWithRotate(
				    PARTICLE_MS_RAKU_INDAWA,
				    &mEffectPositions[mCurEffectPosIndex], 0,
				    (s16)(0.005493164f * SMS_GetMarioAngleY()), 0, 2, nullptr);
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
