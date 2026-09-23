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

// TODO: incorrect size (map 0x24, i.e. the same nine instructions as
// SMS_IsMarioStatusHipDrop) if MARIO_STATUS_WAIT is not the constant; the
// SMS_IsMarioStatus<X> family all compare mStatus to one exact status, and
// WAIT is the only "waiting" status in the enum.
bool SMS_IsMarioStatusWaiting()
{
	if (gpMarioOriginal->mStatus == MARIO_STATUS_WAIT) {
		return 1;
	} else {
		return 0;
	}
}

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

// TODO: body unknown. The map gives 0x24, the same nine instructions as
// SMS_IsMarioStatusHipDrop, so this is `mStatus == MARIO_STATUS_HIP_DROP_END`
// -- but that enumerator's value is not recoverable: nothing in the ROM
// compares a status against 0x8008AA (the value next to HIP_DROP's 0x8008A9),
// and no other unexplained `subis rX, rY, 0x80` constant exists. Left empty
// rather than guessed.
bool SMS_IsMarioStatusHipDropEnd() { }

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

// TODO: body unknown, for the same reason as SMS_IsMarioStatusHipDropEnd:
// MARIO_STATUS_HIP_DROP_END has no recoverable value. The map's 0x28 is one
// instruction more than SMS_IsMarioOpeningDoor's two-status compare, which
// this would otherwise be with the global load replaced by the argument.
bool SMS_IsStatusHipDropOrHipDropEnd(THitActor* actor) { }

// TODO: incorrect size (map 0x38, ours 0x30). isWearingCap() lives in
// MarioDraw.cpp so it stays a `bl`, and 0x38 is 14 instructions: 7 of frame
// (mflr/stw/stwu + lwz/addi/mtlr/blr), the gpMarioOriginal load, the call and
// five left over. `!bool` is clrlwi/cntlzw/srwi (three, what we get); five is
// the clrlwi/neg/subic/subfe/clrlwi normalise that `!!` emits (cf.
// SMS_IsMarioDashing). Writing `!!isWearingCap()` is size-exact and
// semantically backwards, so it is rejected as a fakematch. The other body
// that lands on 14 is a direct `!mCap->isModelActive(E_CAP_MODEL_HAT)` --
// global load, mCap load, argument, call, three-instruction negate -- but
// which model index is a guess, so the readable spelling is kept.
bool SMS_IsMarioNoCap() { return !gpMarioOriginal->isWearingCap(); }

// The aloha shirt is MARIO_FLAG_HAS_SHIRT; the map's 0x34 is the same
// thirteen instructions as SMS_IsMarioDashing, the other checkFlag wrapper.
bool SMS_IsMarioWearingAloha()
{
	return !!gpMarioOriginal->checkFlag(MARIO_FLAG_HAS_SHIRT);
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

// TODO: the offset is a guess. The map's 0xc forces exactly the three
// instructions of SMS_GetMarioHP (global load, 16-bit member load, blr), and
// unk122 is the only unnamed halfword adjacent to mHealth, but naming it
// mHealthMax belongs in Mario.hpp and was not done here.
s16 SMS_GetMarioHPMax() { return gpMarioOriginal->unk122; }

f32 SMS_GetMarioDamageRadius() { return gpMarioOriginal->mDamageRadius; }

f32 SMS_GetMarioGrLevel() { return gpMarioOriginal->mFloorPosition.y; }

f32 SMS_GetMarioGravity()
{
	return gpMarioOriginal->mJumpParams.mGravity.value;
}

TWaterGun* SMS_GetMarioWaterGun() { return gpMarioOriginal->mWaterGun; }

// TODO: incorrect size (map 0x24). The wrapper is the same size as
// TMario::getJumpIntoWaterModelData (UNUSED 0x24, MarioDraw.cpp), which is
// what gets inlined here; that body is still an empty stub, and the return
// type is unknown (TMarioEffect's own getJumpIntoWaterModelData returns int).
void SMS_GetMarioJumpIntoWaterModelData()
{
	gpMarioOriginal->getJumpIntoWaterModelData();
}

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

// TODO: incorrect size (map 0x34). TMario::isSpeedZero is UNUSED at 0x38 in
// MarioMove.cpp and still an empty stub there, so the inlined body -- and this
// wrapper's return type -- cannot be reached from this TU yet.
void SMS_IsMarioSpeedZero() { gpMarioOriginal->isSpeedZero(); }

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
// Closure batch 103 tried the documented CSE breakers and none of them works
// here: a pointer-to-const parameter on a TU-local inline (the
// AGENT_MATCHING_TIPS "const on an inline's pointer parameter also defeats
// CSE" lever) applied to the null test, to the type test, and to both; the
// const member `TTakeActor::isTaken()` on the left of the && (73.6%, the BOOL
// still materialises *and* the load is still merged); and a `TTakeActor* const&`
// reference parameter of an inline, which folds the address into the load
// offset but still merges. Every variant is 17 instructions to retail's 18, so
// the missing instruction really is a second `lwz 0x68`. Next idea: an
// intervening *store*, i.e. the type test may live in a helper that also writes
// a member (nothing in this 18-instruction body can be that store, so more
// likely the whole predicate lived somewhere else and was inlined here).
// Closure batch 115 added the batch-110 binding level (a TU-local `static
// inline TTakeActor* MarioAccessHolder(const TMario*)` binding the fetch, used
// on both sides of the &&): codegen is byte-identical to the raw spelling, so
// a binding level is not a CSE breaker either. What is left untried is a
// spelling in which the two loads are genuinely different memory to MWCC.
// Batch cc22, all inert or worse: a `TTakeActor**` to the member (19
// instructions), a `(u32)` null test, a ternary (82.4), two nested ifs with a
// false default (76.1), and a named `TMario*` receiver (identical).
// Batch c-link4, all 17 instructions: TU-local levels over the mario
// pointer for the null test or the type test (`const TMario*`/`TTakeActor*`,
// getHolder() inside), BOOL-valued `!= nullptr` tests, and `== TRUE` isTaken().
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

// TODO: body unknown. The map's 0x14 is five instructions, i.e. a global
// load, index scaling and one indexed load, but no member of TMario is named
// or shaped like a "work" array and nothing in the ROM calls this.
void SMS_GetMarioWork(int index) { }

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

// Same shape as SMS_IsMarioTouchGround4cm(), one instruction shorter
// (map 0x40 against 0x44) because the argument replaces the gpMarioOriginal
// load.
bool SMS_IsMarioTouchGround4cm(THitActor* actor)
{
	if (((TMario*)actor)->isTouchGround4cm())
		return true;
	else
		return false;
}

// Same shape as SMS_GetMarioStatus(THitActor*), which the map also gives as
// 0x8: one load off the argument and a blr.
f32 SMS_GetMarioSpeedY(THitActor* actor) { return ((TMario*)actor)->mVel.y; }

bool SMS_IsMarioFencing() { return gpMarioOriginal->isFencing(); }

bool SMS_IsMarioRoofing() { return gpMarioOriginal->isRoofing(); }
