#include <Enemy/BossHanachanSub.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/MarDirector.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Camera/cameralib.hpp>

f32 BHSCalcCentrifugalForce(const JGeometry::TVec3<f32>& first,
                          const JGeometry::TVec3<f32>& middle,
                          const JGeometry::TVec3<f32>& last, f32 degreeY)
{
	JGeometry::TVec3<f32> firstDelta;
	firstDelta.sub(first, middle);
	f32 force = firstDelta.x * firstDelta.x + firstDelta.z * firstDelta.z;
	if (force < 0.001f)
		return 0.0f;

	JGeometry::TVec3<f32> secondDelta;
	secondDelta.sub(middle, last);
	if (secondDelta.x * secondDelta.x + secondDelta.z * secondDelta.z < 0.001f)
		return 0.0f;

	s16 firstAngle = CLBDegToShortAngle(MsGetRotFromZaxisY(firstDelta));
	s16 secondAngle = CLBDegToShortAngle(MsGetRotFromZaxisY(secondDelta));
	if (firstAngle == secondAngle)
		return 0.0f;

	int difference = firstAngle - secondAngle;
	s16 wrappedDifference = difference;
	int magnitude
	    = wrappedDifference >= 0 ? wrappedDifference : -wrappedDifference;
	f32 ratio = (1.0f / 32768.0f) * magnitude;
	if (ratio >= 0.5f)
		return 0.0f;
	force *= ratio;
	if (difference < 0)
		force = -force;
	force *= JMASCos(firstAngle - CLBDegToShortAngle(degreeY));
	return force;
}

// TODO: 94.0%. Two residues, both of which also show up where this body is
// inlined into TSphereLink::setDegreeZAndRevisionPosXZ:
//  - retail loads the surviving `distance.z` zero literal *before* the sine
//    table `lfsx` and we load it after (one swapped pair, everything else in
//    order), and at the inlined site the same swap comes with an f2/f3
//    permutation on the following member read -- the known-open FPR
//    permutation class;
//  - frame 0x50 against retail's 0x58.
// Measured inert: `distance` spelled `TVec3 d; d.set(...)`, spelled as three
// field assignments. Measured worse: declaring `sine` before `cosine`
// (94.0 -> 92.6, it permutes f3/f4 as well), writing the zero term as
// `sine * distance.z` (93.5), putting the `distance.z` terms first (86.7).
void BHSCalcRevisionDistXZByRotateZ(f32 degreeY, f32 degreeZ, f32 scale,
                                  f32* resultX, f32* resultZ)
{
	JGeometry::TVec3<f32> distance(scale * degreeZ, 0.0f, 0.0f);
	s16 angle = CLBDegToShortAngle(degreeY);
	f32 cosine = JMASCos(angle);
	f32 sine = JMASSin(angle);
	*resultX = distance.x * cosine + distance.z * sine;
	*resultZ = -distance.x * sine + distance.z * cosine;
}

void TWaterHitActor::onWaterHitCounter() { mWaterHitCounter = 60; }

BOOL TWaterHitActor::receiveMessage(THitActor*, u32 message)
{
	BOOL result = FALSE;
	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (gpMarDirector->isThing())
			mWaterHitCounter = 0;
		else
			onWaterHitCounter();
		result = TRUE;
	}
	return result;
}

TSphereLink::TSphereLink(u16 count, const JGeometry::TVec3<f32>& start,
                         f32 length, f32 radius, f32 velocityScale, f32 gravity,
                         f32 rotationMoveScale, f32 degreeY)
    : mPointCount(count)
    , mPoints(new TSpherePoint[count])
    , mVelocityScale(velocityScale)
    , mGravity(gravity)
    , mCollisionRadius(radius)
    , mRotationMoveScale(rotationMoveScale)
    , mHeadDegreeY(degreeY)
{
	s16 angle = (65536.0f / 360.0f) * mHeadDegreeY;
	f32 offsetX = JMASSin(angle) * length;
	f32 offsetZ = JMASCos(angle) * length;
	for (int i = 0; i < mPointCount; ++i) {
		TSpherePoint* point = &mPoints[i];
		JGeometry::TVec3<f32> position;
		if (i == 0) {
			position = start;
		} else {
			position = mPoints[i - 1].mPosition;
			position.x -= offsetX;
			position.z -= offsetZ;
		}
		point->mPosition = position;
		point->mPreviousPosition = point->mPosition;
		point->mVelocity.set(0.0f, 0.0f, 0.0f);
		point->mLength = length;
		point->mDegreeZ = 0.0f;
	}
}

void TSphereLink::execMapCollision_(JGeometry::TVec3<f32>* position)
{
	gpMap->isTouchedOneWallAndMoveXZ(&position->x, position->y, &position->z,
	                                mCollisionRadius);
	const TBGCheckData* ground;
	f32 height = gpMap->checkGroundIgnoreWaterSurface(
	    position->x, position->y + mCollisionRadius, position->z, &ground);
	if (ground && (ground->isIllegalData() == true ? false : true)
	    && position->y < height)
		position->y = height;
}

void TSphereLink::moveHead(const JGeometry::TVec3<f32>& position)
{
	for (int i = 0; i < mPointCount; ++i) {
		mPoints[i].mPosition.y += mGravity;
		mPoints[i].mPosition += mPoints[i].mVelocity;
	}
	mPoints[0].mPosition = position;
	execMapCollision_(&mPoints[0].mPosition);
	for (int i = 1; i < mPointCount; ++i) {
		TSpherePoint* point = &mPoints[i];
		TSpherePoint* previous = &mPoints[i - 1];
		JGeometry::TVec3<f32> offset = point->mPosition - previous->mPosition;
		if (offset.isZero())
			offset.set(0.0f, 1.0f, 0.0f);
		else
			PSVECNormalize(offset, offset);
		offset.scale(point->mLength);
		point->mPosition = previous->mPosition + offset;
		execMapCollision_(&point->mPosition);
	}
	for (int i = 0; i < mPointCount; ++i) {
		// TODO: retail `bl`s TVec3::sub here and expands operator-=, i.e. the
		// difference is at inline depth 4, which only a named local reaches
		// (nested in the product's argument it lands one lower and MWCC
		// `bl`s operator-= instead). Retail's three copies -- operator-'s
		// by-value operand, the named difference, operator*'s by-value
		// operand -- also say the difference was named. Writing it that way
		// (plus a named `mVelocityScale`, which retail keeps in a
		// callee-saved FPR across the loop) does reach the `bl` and moves
		// the frame 16 bytes towards the ROM's 0x108, but retail then
		// assigns operator*'s operand slot straight into mVelocity while we
		// materialise operator*'s by-value return as a fourth copy, so the
		// function scores 85.7 -> 84.9 and it was reverted. The missing
		// piece is why MWCC elides that return copy for retail and not for
		// us; JGVec3.hpp's `operator*` returning a reference like
		// `operator-`/`operator+` do is the obvious candidate and is a
		// shared-header question.
		mPoints[i].mVelocity
		    = (mPoints[i].mPosition - mPoints[i].mPreviousPosition) * mVelocityScale;
		mPoints[i].mPreviousPosition = mPoints[i].mPosition;
	}
}

// TODO: 98.2% and frame-exact. The `mPoints[index - 1].mPosition - ...` sub
// receiver sits at 0x5c where retail has 0x40 and its copy at 0x70 against
// 0x74, and the tail carries the inlined BHSCalcRevisionDistXZByRotateZ swap
// described above. Retail's `lfs f2, 0xc(r31)` member read ranks *below* the
// computed product where ours ranks above it, which is the "a member read must
// be a named local" rule -- but `point->mPosition.x += offsetX` has nowhere to
// put the name without changing the store.
BOOL TSphereLink::setDegreeZAndRevisionPosXZ(int index, f32 degreeZ)
{
	BOOL changed = FALSE;
	TSpherePoint* point = &mPoints[index];
	f32 oldDegreeZ = point->mDegreeZ;
	if (oldDegreeZ != degreeZ) {
		point->mDegreeZ = degreeZ;
		changed = TRUE;
		f32 degreeY;
		if (index == 0) {
			degreeY = mHeadDegreeY;
		} else {
			JGeometry::TVec3<f32> axis
			    = mPoints[index - 1].mPosition - point->mPosition;
			degreeY = MsWrap(MsGetRotFromZaxisY(axis), 0.0f, 360.0f);
		}
		f32 offsetX, offsetZ;
		BHSCalcRevisionDistXZByRotateZ(degreeY, degreeZ - oldDegreeZ,
		                             mRotationMoveScale, &offsetX, &offsetZ);
		point->mPosition.x += offsetX;
		point->mPosition.z += offsetZ;
	}
	return changed;
}
