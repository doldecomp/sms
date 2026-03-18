#include <Player/MarioMain.hpp>

#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <JSystem/JMath.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/Watergun.hpp>
#include <Strategic/LiveActor.hpp>
#include <M3DUtil/InfectiousStrings.hpp>
#include <MSound/MSoundBGM.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <dolphin/mtx.h>
#include <fake_tgmath.h>

// MarioCheckCol: -inline deferred, functions in REVERSE address order.

// hitNormal: 0x80161A78, size 0x210
void TMario::hitNormal(THitActor* actor)
{
	u32 action = mAction;

	// Check action bit 12 (grounded/walking)
	if (action & 0x1000) {
		// Check speed < 0
		if (mVel.y < 0.0f) {
			// Check actor below mario
			if (actor->mPosition.y < mPosition.y) {
				// Check action range
				if ((action - 0x80000000) == 0x8A9) {
					// Call virtual receiveMessage(this, 1)
					u32* vtbl = *(u32**)actor;
					typedef BOOL (*MsgFunc)(THitActor*, THitActor*, u32);
					BOOL result = ((MsgFunc)vtbl[0x28])(actor, this, 1);
					if (result) {
						u32 actorType = *(u32*)((u8*)actor + 0x4C);
						if ((actorType - 0x08000000) <= 1) {
							setPlayerVelocity(0.0f);
							unk78 = unk78 & ~(1 << 8);
						}
					}
					return;
				}

				// Normal grounded hit
				keepDistance(*actor, 0.0f);
				return;
			}
		}
	}

	// Check flag bit 10
	if (checkFlag(0x400)) {
		if (actor->mPosition.y > mPosition.y) {
			u32* vtbl = *(u32**)actor;
			typedef void (*MsgFunc)(THitActor*, THitActor*, u32);
			((MsgFunc)vtbl[0x28])(actor, this, 3);
			return;
		}
	}

	// Check action ranges for kick/trample
	u32 act = mAction;
	if ((act - 0x80000000) == 0x456 ||
	    (act - 0x7C000000) == 0x45D ||
	    (act - 0x80000000) == 0x88A) {
		u32* vtbl = *(u32**)actor;
		typedef void (*MsgFunc)(THitActor*, THitActor*, u32);
		((MsgFunc)vtbl[0x28])(actor, this, 12);
		((MsgFunc)vtbl[0x28])(actor, this, 0);
	}

	// Check watergun nozzle state
	TWaterGun* wg = mWaterGun;
	u8 nozzleState = *(u8*)((u8*)wg + 0x1C84);
	if (nozzleState != 0)
		return;

	u8 emitState = *(u8*)((u8*)wg + 0x1C86);
	if (emitState == 0)
		return;

	// Create a hit position copy and push via receiveMessage(actor, 15)
	JGeometry::TVec3<f32> hitPos;
	hitPos.x = mPosition.x;
	hitPos.y = mPosition.y;
	hitPos.z = mPosition.z;
	hitPos.y += *(f32*)((u8*)0 + 0); // height offset
	// Store position to a temp area and call virtual
	u32* vtbl2 = *(u32**)actor;
	typedef void (*MsgFunc)(THitActor*, THitActor*, u32);
	((MsgFunc)vtbl2[0x28])(actor, this, 15);
}

// hangPole: 0x801617E4, size 0x294
void TMario::hangPole(THitActor* actor)
{
	// Check action bit 20
	if (checkActionFlag(0x100000))
		return;

	// Check conditions for grabbing pole
	u8 canGrab = 0;
	if (*(u32*)((u8*)this + 0x6C) == 0) {
		if (!isMario()) {
			canGrab = 1;
		}
	}

	u8 shouldGrab;
	if (!canGrab) {
		shouldGrab = 0;
	} else {
		u32 action = mAction;
		u32 low9 = action & 0x1FF;
		if (low9 >= 0x80 && low9 <= 0x9F) {
			shouldGrab = 1;
		} else if (action & 0x200000) {
			shouldGrab = 1;
		} else {
			shouldGrab = 0;
		}
	}

	if ((u8)shouldGrab != 1) {
		// Normal hit - call virtual for height and keepDistance
		u32* vtbl = *(u32**)actor;
		typedef void (*HeightFunc)(THitActor*, f32);
		((HeightFunc)vtbl[0x2C])(actor, mPosition.y);
		keepDistance(actor->mPosition, *(f32*)((u8*)0 + 0), 0.0f);
		return;
	}

	// Distance check in XZ plane
	f32 actorZ = actor->mPosition.z;
	f32 marioZ = mPosition.z;
	f32 actorX = actor->mPosition.x;
	f32 dz = actorZ - marioZ;
	f32 marioX = mPosition.x;
	f32 dx = actorX - marioX;

	f32 distSqXZ = dx * dx + dz * dz;
	f32 distXZ = 0.0f;
	if (distSqXZ > 0.0f) {
		distXZ = sqrtf(distSqXZ);
	}

	f32 safeDistXZ = distXZ;
	if (0.0f == distXZ)
		safeDistXZ = 1.0f;

	f32 normZ = dz / safeDistXZ;
	f32 normX = dx / safeDistXZ;

	// Angle-based check
	u32 prevAction = mPrevAction;
	u16 faceAngle = (u16)mModelFaceAngle;
	u8 canCatch = 1;

	// sin/cos lookup
	u32 sinTbl = *(u32*)((u8*)0 + 0);
	u32 cosTbl = *(u32*)((u8*)0 + 0);
	s16 halfAngle = faceAngle >> *(u32*)((u8*)0 + 0);
	f32 sinVal = *(f32*)(sinTbl + (u32)((u16)halfAngle << 2));
	f32 cosVal = *(f32*)(cosTbl + (u32)((u16)halfAngle << 2));
	f32 catchRadius = *(f32*)((u8*)actor + 0x58);
	f32 dot = cosVal * normZ + sinVal * normX;
	f32 poleRadius = mBarParams.mCatchRadius.value;
	f32 poleHeight = mBarParams.mCatchAngle.value;

	if (prevAction & 0x100000)
		canCatch = 0;

	if (dot < poleHeight)
		canCatch = 0;

	if (safeDistXZ > catchRadius + poleRadius)
		canCatch = 0;

	// Check Y position
	f32 yCheck = *(f32*)((u8*)0 + 0);
	if (mPosition.y < yCheck + actor->mPosition.y)
		canCatch = 0;

	if ((u8)canCatch != 1) {
		// Fall through to normal hit
		u32* vtbl = *(u32**)actor;
		typedef void (*HeightFunc)(THitActor*, f32);
		((HeightFunc)vtbl[0x2C])(actor, mPosition.y);
		keepDistance(actor->mPosition, *(f32*)((u8*)0 + 0), 0.0f);
		return;
	}

	// Grab pole
	setPlayerVelocity(0.0f);
	*(u32*)((u8*)this + 0x68) = (u32)actor;
	mVel.y = 0.0f;
	mForwardVel = 0.0f;
	changePlayerStatus(0x10100341, 0, false);

	// Virtual receiveMessage(actor, mario, 5)
	u32* vtbl2 = *(u32**)actor;
	typedef void (*MsgFunc)(THitActor*, THitActor*, u32);
	((MsgFunc)vtbl2[0x28])(actor, this, 5);

	mHolderHeightDiff = mPosition.y - actor->mPosition.y;
}

// checkCollision: 0x80160480, size 0x135C
void TMario::checkCollision()
{
	// Check action bit 19
	if (checkActionFlag(0x80000))
		return;

	// Check yoshi state
	TYoshi* yoshi = mYoshi;
	u8 yoshiState = *(u8*)yoshi;
	if (yoshiState == 6 || yoshiState == 2) {
		f32 marioY = mPosition.y;
		f32 yoshiFloorY = *(f32*)((u8*)yoshi + 0x24);
		if (yoshiFloorY <= marioY) {
			f32 extra = *(f32*)((u8*)0 + 0);
			if (marioY < extra + yoshiFloorY) {
				// XZ distance
				f32 yoshiZ = *(f32*)((u8*)yoshi + 0x28);
				f32 marioZ = mPosition.z;
				f32 yoshiX = *(f32*)((u8*)yoshi + 0x20);
				f32 marioX = mPosition.x;
				f32 dz = yoshiZ - marioZ;
				f32 dx = yoshiX - marioX;
				f32 distSq = dx * dx + dz * dz;
				f32 dist = 0.0f;
				if (distSq > 0.0f)
					dist = sqrtf(distSq);

				// Additional checks
				u32 action = mAction;
				if (action & 0x1000) {
					if (*(u32*)((u8*)this + 0x6C) != 0)
						goto afterYoshi;
					if (mVel.y >= 0.0f)
						goto afterYoshi;
					f32 yoshiSurfY = *(f32*)((u8*)yoshi + 0x24);
					if (yoshiSurfY >= mPosition.y)
						goto afterYoshi;
					if (action == 0x89C)
						goto afterYoshi;
					if ((action - 0x02000000) == 0x8B8)
						goto afterYoshi;
					if (action == 0x883)
						goto afterYoshi;
					if (dist >= *(f32*)((u8*)0 + 0))
						goto afterYoshi;

					// Copy yoshi pos
					mPosition.x = *(f32*)((u8*)yoshi + 0x20);
					mPosition.y = *(f32*)((u8*)yoshi + 0x24);
					mPosition.z = *(f32*)((u8*)yoshi + 0x28);

					TYoshi* yoshi2 = mYoshi;
					s16 yoshiAngle = *(s16*)((u8*)yoshi2 + 0x70);
					mModelFaceAngle = yoshiAngle;
					mFaceAngle.z = mModelFaceAngle;

					// HAS_FLUDD check
					if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
						TWaterGun* wg = mWaterGun;
						u8 ntype = wg->mCurrentNozzle;
						*(u32*)((u8*)this + 0x3E8) = ntype;

						TWaterGun* wg2 = mWaterGun;
						TNozzleBase* nozzle = wg2->getCurrentNozzle();
						u32 waterA = *(u32*)((u8*)nozzle + 0xCC);
						u32 waterB = *(u32*)((u8*)nozzle + 0xBC);
						f32 ratio = (f32)((s32)waterA / (s32)waterB);
						*(f32*)((u8*)this + 0x3EC) = ratio;
					}

					TYoshi* yoshi3 = mYoshi;
					*(u32*)((u8*)yoshi3 + 0) = 0; // ride
					mState |= 0x8000;

					if (checkFlag(MARIO_FLAG_HAS_FLUDD)) {
						mWaterGun->changeNozzle(3, true);
					}

					changePlayerStatus(0x0C400201, 0, false);
					return;
				}
			}
		}
	}

afterYoshi:
	// Set attack area
	setNormalAttackArea();

	// Iterate over collision actors
	u16 numActors = *(u16*)((u8*)this + 0x48);
	for (u16 i = 0; i < numActors; i++) {
		THitActor** actorList = *(THitActor***)((u8*)this + 0x44);
		THitActor* actor = actorList[i];

		u32 actorFlags = *(u32*)((u8*)actor + 0x4C);

		// Check actor flag bit 26
		if (!(actorFlags & 0x04000000))
			continue;

		// Check mario flags for special collision handling
		if (!checkFlag(0x80000)) {
			if (!checkActionFlag(0x2000)) {
				if (checkActionFlag(0x1000)) {
					if ((mAction - 0x80000000) != 0x8A9) {
						if (mVel.y < 0.0f) {
							if (actor->mPosition.y < mPosition.y) {
								if (trampleExec(actor)) {
									keepDistance(*actor, 0.0f);
								}
							}
						}
					}
				}
			}
		}

		// Hit normal processing
		keepDistance(actor->mPosition, 0.0f, 0.0f);

		// Check hit result flags
		u32 hitFlags = *(u32*)((u8*)actor + 0xF0);
		if (!(hitFlags & 0x100000))
			continue;

		// Process hit reaction
		hitNormal(actor);
		// Store actor and change status
		*(THitActor**)((u8*)this + 0x384) = actor;
		changePlayerStatus(0x383, 0, false);
	}
}
