#include <Player/MarioAccess.hpp>

#include <Strategic/LiveActor.hpp>
#include <Player/Mario.hpp>
#include <JSystem/JGeometry.hpp>

void* gpMarioAddress;
JGeometry::TVec3<f32>* gpMarioPos;
s16 *gpMarioAngleX, *gpMarioAngleY, *gpMarioAngleZ;
f32 *gpMarioSpeedX, *gpMarioSpeedY, *gpMarioSpeedZ;
s16* gpMarioLightID;
u32* gpMarioFlag;
f32* gpMarioThrowPower;
const TBGCheckData** gpMarioGroundPlane;

void SMS_SetMarioAccessParams()
{
	s16* angle;
	f32* speed;

	gpMarioAddress = gpMarioOriginal;
	gpMarioPos     = &gpMarioOriginal->mPosition;

	angle         = &gpMarioOriginal->mFaceAngle.x;
	gpMarioAngleX = angle;
	gpMarioAngleY = angle + 1;
	gpMarioAngleZ = angle + 2;

	speed         = &gpMarioOriginal->mVel.x;
	gpMarioSpeedX = speed;
	gpMarioSpeedY = speed + 1;
	gpMarioSpeedZ = speed + 2;

	gpMarioLightID = &gpMarioOriginal->mLightID;
	gpMarioFlag    = &gpMarioOriginal->mFlag;

	gpMarioThrowPower  = &gpMarioOriginal->mDeParams.mThrowPower.value;
	gpMarioGroundPlane = &gpMarioOriginal->mGroundPlane;
}

TYoshi* SMS_GetYoshi() { return gpMarioOriginal->mYoshi; }

bool SMS_AskJumpIntoWaterEffectExist()
{
	return gpMarioOriginal->askJumpIntoWaterEffectExist();
}

THitActor* SMS_GetMarioHitActor() { return (THitActor*)gpMarioAddress; }

TLiveActor* SMS_GetMarioLiveActor() { return (TLiveActor*)gpMarioAddress; }

bool SMS_IsMarioStatusTypeJumping()
{
	return gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_JUMPING);
}

bool SMS_IsMarioStatusTypeSwimming()
{
	return gpMarioOriginal->checkStatusType(MARIO_STATUS_FLAG_SWIMMING);
}

bool SMS_IsMarioStatusHipDrop()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_HIP_DROP) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusThrownDown()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_THROWN_DOWN) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioStatusElecDamage()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_ELECTRIC_DAMAGE) {
		return 1;
	} else {
		return 0;
	}
}

bool SMS_IsMarioHeadSlideAttack()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_CATCH
	    || gpMarioOriginal->mStatus == MARIO_STATUS_JUMP_CATCH) {
		return 1;
	} else {
		return 0;
	}
}

s16 SMS_GetMarioHP() { return gpMarioOriginal->mHealth; }

f32 SMS_GetMarioDamageRadius() { return gpMarioOriginal->mDamageRadius; }

f32 SMS_GetMarioGrLevel() { return gpMarioOriginal->mFloorPosition.y; }

f32 SMS_GetMarioGravity()
{
	return gpMarioOriginal->mJumpParams.mGravity.value;
}

TWaterGun* SMS_GetMarioWaterGun() { return gpMarioOriginal->mWaterGun; }

bool SMS_SendMessageToMario(THitActor* m, u32 mesg)
{
	if (gpMarioOriginal->receiveMessage(m, mesg))
		return true;

	return false;
}

void SMS_ThrowMario(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->throwMario(vec, f);
}

bool SMS_IsMarioTouchGround4cm()
{
	if (gpMarioOriginal->isTouchGround4cm())
		return true;
	else
		return false;
}

// TODO: 93.8%, and the only function keeping MarioAccess.cpp from being
// source-linked. The original loads mHolder twice -- once into r0 for the null
// test, once into r3 to dereference -- where MWCC gives us one load reused.
// The body is otherwise exact: our 17 instructions are the target's 18 minus
// that second `lwz r3, 0x68(r3)`, and the BOOL->bool tail (clrlwi/neg/subic/
// subfe/clrlwi, same shape as SMS_IsMarioOnYoshi's) is already right.
//
// Rejected spellings (all measured; the last batch in a scratch TU with the
// real 0x68/0x4c offsets, so the offsets are not the reason):
//   * short-circuit && on the raw member (93.8%, best, what is below);
//   * a separate null check then else-if (90.0%);
//   * assigning the comparison to the result (54.7%);
//   * casting the holder before the member access (93.8%, identical codegen);
//   * isTaken() on the left of the && (73.6%: the BOOL ternary materialises a
//     bool and adds a cmpwi, which the target does not have);
//   * getHolder() on either side or both, getActorType(), isActorType(),
//     u32/BOOL/pointer-returning isTaken() spellings, a base-class cast of
//     gpMarioOriginal, a const base for the test, nested ifs, goto, do/while,
//     a single-case switch, ||-inverted logic, non-short-circuit &, and a
//     TU-local helper taking the holder by value or by const& (deferred
//     definition included, which does not change CSE): every one of these
//     compiles to the same 17 instructions, i.e. MWCC always CSEs the two
//     member loads.
// The only spelling that does reproduce two loads is a reference local or a
// const-reference accessor (`TTakeActor* const& h = gpMarioOriginal->mHolder`),
// but MWCC then keeps the *address* and emits `lwzu r0, 0x68(r3)` + `lwz r3,
// 0(r3)`: right structure, wrong addressing mode. A scan of every .s file in
// the ROM found 29 sites with this load/test/reload shape, and each of the
// other 28 is explained by an intervening `bl`, an intervening store to the
// same member, or index arithmetic (RumbleMgr's `mCtrlMgr[i]` with i folded to
// 0) -- none of which exists here. Next idea worth trying: something that puts
// a real call or store between the test and the dereference, i.e. the null test
// may belong to a *different* inlined helper than the type test.
bool SMS_IsMarioOnWire()
{
	bool ret;
	if (gpMarioOriginal->mHolder
	    && gpMarioOriginal->mHolder->mActorType == 0x40000098)
		ret = true;
	else
		ret = false;

	return !!ret;
}

bool SMS_IsMarioOpeningDoor()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_DOOR_OPEN_R
	    || gpMarioOriginal->mStatus == MARIO_STATUS_DOOR_OPEN_L) {
		return true;
	} else {
		return false;
	}
}

bool SMS_IsMarioOnYoshi() { return gpMarioOriginal->onYoshi(); }

bool SMS_IsMarioDashing()
{
	return !!gpMarioOriginal->checkFlag(MARIO_FLAG_FLUDD_EMITTING);
}

void SMS_MarioMoveRequest(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->moveRequest(vec);
}

void SMS_MarioWarpRequest(const JGeometry::TVec3<float>& vec, float f)
{
	gpMarioOriginal->warpRequest(vec, f);
}

void SMS_FlowMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->flowMove(vec);
}

void SMS_WindMoveMario(const JGeometry::TVec3<float>& vec)
{
	gpMarioOriginal->windMove(vec);
}

u32 SMS_GetMarioStatus() { return gpMarioOriginal->mStatus; }

const TBGCheckData* SMS_GetMarioGrPlane()
{
	return gpMarioOriginal->mGroundPlane;
}

const TBGCheckData* SMS_GetMarioWlPlane()
{
	return gpMarioOriginal->mWallPlane;
}

const TBGCheckData* SMS_GetMarioRfPlane()
{
	return gpMarioOriginal->mRoofPlane;
}

u32 SMS_GetMarioStatus(THitActor* actor) { return ((TMario*)actor)->mStatus; }

bool SMS_IsMarioFencing() { return gpMarioOriginal->isFencing(); }

bool SMS_IsMarioRoofing() { return gpMarioOriginal->isRoofing(); }
