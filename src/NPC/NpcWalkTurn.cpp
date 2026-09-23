#include <NPC/NpcBase.hpp>
#include <Strategic/Spine.hpp>
#include <System/MarDirector.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <NPC/NpcNerve.hpp>
#include <Camera/cameralib.hpp>

// The map emits JGeometry::TVec3<f32>::set<f32>(f32, f32, f32) as a local
// 16-byte instantiation for this TU and execWalk's inlined copy of isCanWalk
// reaches it with a `bl`, so retail has one inline level between isCanWalk and
// the unnamed vector's constructor: that puts `set` (three statements) at
// depth 4, where the allowance is two. Spelling the level as a squared-XZ
// helper reproduces the call (execWalk 95.8 -> 97.7 and the MISSING symbol is
// gone); a level *above* isCanWalk instead pushes TPathNode::getPoint() out of
// line too, which retail expands. Parked here rather than in a shared header
// because the map has no symbol for it.
static inline f32 NpcWalkTurnSquaredXZ(const JGeometry::TVec3<f32>& a,
                                       const JGeometry::TVec3<f32>& b)
{
	return JGeometry::TVec3<f32>(a.x - b.x, 0.0f, a.z - b.z).squared();
}

// TODO: in execWalk's inlined copy retail puts the point copy at 0x90 and
// the unnamed vector at 0x9c; ours has the vector at 0x90 and `target` at
// 0xa8. Passing getPoint() straight to a by-value helper parameter, an
// explicit temporary, getUnkF4(), and a set()-built vector were all inert or
// worse (95.8-97.8).
bool TBaseNPC::isCanWalk() const
{
	bool result = true;
	JGeometry::TVec3<f32> target = unkF4.getPoint();
	if (NpcWalkTurnSquaredXZ(target, mPosition) < CLBSquared(10.0f))
		result = false;
	return result;
}

void TBaseNPC::execWalk(bool param_1)
{
	if (mWalkForbidCount != 0 || SMSGetMarDirector()->isThing() || !isClean()
	    || checkActionFlag(NPC_ACTION_HAPPY)) {
		mMarchSpeed = 0.0f;
		mTurnSpeed  = 0.0f;
		return;
	}

	if (param_1 && checkUnk1DA(UNK1DA_FLAG_UNK1)) {
		f32 fVar1 = 4.0f;
		if (checkActionFlag(NPC_ACTION_RUN))
			fVar1 = 6.0f;

		SMS_GoRotate(mPosition, getUnkF4().getPoint(), fVar1, &mRotation.y);

		// TODO: retail makes *two* 12-byte copies of `direction` before
		// reading .z/.x (0xdc -> 0xfc -> 0x10c), and execWalk's frame only
		// reaches 0x130 with both. The source spelling that produces two
		// copies is unknown; a named copy plus one conversion temporary
		// reproduces the shape but is surely not what was written. New
		// evidence: retail's three objects sit in the *temp pool* in
		// ascending creation order, where a named local puts two of them in
		// the named region above it, so neither vector was a named local.
		// `MsGetRotFromZaxisY(TVec3(TVec3(getUnkF4().getPoint() -
		// mPosition)))` reproduces that placement at the same 97.7%, one
		// unnested temporary drops to 92.7% (frame 0x110), and a TU-local
		// copy of MsGetRotFromZaxisY taking its axis *by value* is 95.3%
		// (frame 0x120), so the by-value header spelling is ruled out.
		// Batch cc33 (after execUTurn closed with a by-value level over
		// MsGetRotFromZaxis): by-value TU-local levels here are all worse --
		// `a - b` returned by value (90.5%, frame exact but `bl sub`), named
		// or `-=` difference helpers (frame 0x120-0x148), an identity copy
		// level, and a by-value `NpcRotY(TVec3)` wrapper; retail's hoisted
		// `.x` load before the `.z == 0` test is not reproduced by any.
		// Frame now exact (0x130) via the SMSGetMarDirector() accessor at
		// the guard, which also lands the 0xfc copy on retail's slot; left
		// are the first temp (ours 0x108, retail 0xdc) and the last copy
		// (ours 0xec, retail 0x10c). Under the `: Vec(other)` copy ctor,
		// nested TVec3(TVec3(a - b)) elides the inner copy (95.4), and
		// named/assigned/by-value-wrapper spellings are all worse.
		JGeometry::TVec3<f32> direction = getUnkF4().getPoint();
		direction -= mPosition;
		JGeometry::TVec3<f32> copy;
		copy = direction;

		f32 angle = MsGetRotFromZaxisY(JGeometry::TVec3<f32>(copy));
		if (MsWrap(fabsf(mRotation.y - angle), 0.0f, 360.0f) < 0.001f)
			offUnk1DA(UNK1DA_FLAG_UNK1);

		return;
	}

	if (param_1) {
		EnumNpcAnmKind uVar4 = (EnumNpcAnmKind)unkD0->getCurrentAnmKind();

		f32 dVar11 = mIndividualParams->mSLMinMarchSpeed.get();
		f32 dVar12 = mIndividualParams->mMarchAccel.get();

		switch (uVar4) {
		case NPC_ANM_KIND_WALK:
			dVar11 = mIndividualParams->mMaxMarchSpeed.get();
			break;

		case NPC_ANM_KIND_RUN:
			dVar11 = mIndividualParams->mSLMaxRunSpeed.get();
			dVar12 = mIndividualParams->mSLRunAccel.get();
			if (checkActionFlag(NPC_ACTION_BURNING)) {
				f32 dVar10 = mPtrSaveNormal->mSLSmokeRunMagnif.get();
				dVar11 *= dVar10;
				dVar12 *= dVar10;
			}
			break;
		}

		CLBChaseGeneralConstantSpecifySpeed(&mMarchSpeed, dVar11, dVar12);
	} else {
		CLBChaseGeneralConstantSpecifySpeed(
		    &mMarchSpeed, 0.0f, mIndividualParams->mMarchDecrease.get());
	}

	if (mMarchSpeed < 0.001f)
		mTurnSpeed = mIndividualParams->mWaitTurnSpeed.get();
	else
		mTurnSpeed = mIndividualParams->mWalkTurnSpeed.get();

	if (isCanWalk())
		walkToCurPathNode(mMarchSpeed, mTurnSpeed, 0.0f);
}

// The compare really is `mRotation.y == targetYaw` (retail's `fcmpu cr0, f3,
// f0` puts the member first); reversing it costs nothing and is the ROM's
// operand order.
// A by-value level over MsGetRotFromZaxis materialises the struct-return
// slot's address before the difference temporary's (retail's r3 before r4)
// and loads the returned `.y` into f0 with an `fmr` into targetYaw's f2.
// fabricated
static inline JGeometry::TVec3<f32>
NpcUTurnRotFromZaxis(const JGeometry::TVec3<f32>& axis)
{
	return MsGetRotFromZaxis(axis);
}

bool TBaseNPC::execUTurn()
{
	f32 targetYaw = NpcUTurnRotFromZaxis(unkF4.getPoint() - mPosition).y;
	if (mRotation.y == targetYaw)
		return true;

	if (!isClean() || checkActionFlag(NPC_ACTION_HAPPY))
		return false;

	bool result = false;

	targetYaw   = MsWrap(targetYaw, 0.0f, 360.0f);
	mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);
	if (checkLiveFlag(LIVE_FLAG_UNK200000)) {
		if (mRotation.y > targetYaw)
			mRotation.y -= 360.0f;
	} else {
		if (mRotation.y < targetYaw)
			mRotation.y += 360.0f;
	}

	BOOL r = CLBChaseGeneralConstantSpecifySpeed(
	    &mRotation.y, targetYaw, mIndividualParams->mUTurnSpeed.get());
	mRotation.y = MsWrap(mRotation.y, 0.0f, 360.0f);
	if (!r)
		result = true;

	return result;
}

// Binding level worth +8 of low region, landing
// TBaseNPC::execTurnToFirstState's frame at 0x50 (batch 124).
static inline const JGeometry::TVec3<f32>&
NpcWalkTurnGetUnk1A0(const TBaseNPC* p)
{
	const JGeometry::TVec3<f32>& unk1A0 = p->getUnk1A0();
	return unk1A0;
}

// The `getUnk1A0()` level at the angle2 site alone is +8 with no instruction
// change (frame 0x48 -> 0x50, which is exact) and saturates there: at the
// compare site or the tail assignment it is +0 and costs 1-3 instructions.
// The turn speed is converted straight into the call's argument: a named
// `s16 angle3` takes a 4-byte slot at the bottom of the named block and pushes
// `angle1` down to 0x34.
bool TBaseNPC::execTurnToFirstState()
{
	if (mRotation.y == unk1A0.y)
		return true;

	bool result = false;

	s16 angle1 = CLBDegToShortAngle(mRotation.y);
	s16 angle2 = CLBDegToShortAngle(NpcWalkTurnGetUnk1A0(this).y);
	if (!CLBChaseGeneralConstantSpecifySpeed(
	        &angle1, angle2,
	        CLBDegToShortAngle(
	            mIndividualParams->mFirstStateTurnSpeed.get()))) {
		result      = true;
		mRotation.y = unk1A0.y;
	} else {
		mRotation.y = SHORTANGLE2DEG(angle1);
	}

	return result;
}

bool TBaseNPC::isNeedTurnToFirstState() const
{
	if (!isClean() || checkActionFlag(NPC_ACTION_HAPPY))
		return false;

	bool result = false;

	switch (getActorType()) {
	case 0x400001C:
	case 0x400001D:
	case 0x4000008:
		break;

	default:
		const TNerveBase<TLiveActor>* nerve = mSpine->getLatestNerve();
		if ((nerve == &TNerveNPCWaitMarioApproach::theNerve()
		     || nerve == &TNerveNPCTurnToMario::theNerve())
		    && (getActorType() == 0x4000006
		        || !checkActionFlag(NPC_ACTION_UNK800 | NPC_ACTION_UNK400
		                            | NPC_ACTION_UNK1))) {
			result = true;
		}
		break;
	}

	return result;
}

bool TBaseNPC::isTurnToMarioWhenTalk() const
{
	bool result = true;
	switch (mActorType) {
	case 0x4000007:
	case 0x4000008:
	case 0x400001A:
	case 0x400001B:
	case 0x400001D:
		result = false;
		break;

	default:
		if (checkActionFlag(NPC_ACTION_UNK800 | NPC_ACTION_UNK400
		                    | NPC_ACTION_UNK1))
			result = false;
		break;
	}
	return result;
}

bool TBaseNPC::isTurnToMarioWhenApproach() const
{
	if (!isClean() || checkActionFlag(NPC_ACTION_HAPPY))
		return false;

	bool result = true;

	switch (mActorType) {
	case 0x4000016:
	case 0x4000017:
	case 0x4000018:
		if (checkActionFlag(
		        NPC_ACTION_BURNING | NPC_ACTION_UNK2000 | NPC_ACTION_UNK1000
		        | NPC_ACTION_UNK800 | NPC_ACTION_UNK400 | NPC_ACTION_HAPPY
		        | NPC_ACTION_UNK40 | NPC_ACTION_UNK20 | NPC_ACTION_UNK10
		        | NPC_ACTION_RUN | NPC_ACTION_DANCE | NPC_ACTION_UNK2
		        | NPC_ACTION_UNK1))
			result = false;
		break;

	default:
		result = false;
		break;
	}
	return result;
}
