#include <Enemy/BossHanachanSub.hpp>
#include <Player/ModelWaterManager.hpp>
#include <System/MarDirector.hpp>
#include <Map/Map.hpp>
#include <Map/MapData.hpp>
#include <Camera/cameralib.hpp>

f32 BHSCalcCentrifugalForce(const JGeometry::TVec3<f32>& head,
                            const JGeometry::TVec3<f32>& current,
                            const JGeometry::TVec3<f32>& previous,
                            f32 direction)
{
	JGeometry::TVec3<f32> headOffset(head.x - current.x, 0.0f,
	                                 head.z - current.z);
	f32 distanceSquared
	    = headOffset.x * headOffset.x + headOffset.z * headOffset.z;
	if (distanceSquared < 0.001f)
		return 0.0f;

	JGeometry::TVec3<f32> previousOffset(current.x - previous.x, 0.0f,
	                                     current.z - previous.z);
	if (previousOffset.x * previousOffset.x
	        + previousOffset.z * previousOffset.z
	    < 0.001f)
		return 0.0f;

	s16 headDirection = CLBDegToShortAngle(MsGetRotFromZaxisY(headOffset));
	s16 previousDirection
	    = CLBDegToShortAngle(MsGetRotFromZaxisY(previousOffset));
	if (headDirection == previousDirection)
		return 0.0f;

	int angleDifference = headDirection - previousDirection;
	f32 ratio           = (1.0f / 32768.0f) * CLBAbs<int>((s16)angleDifference);
	if (ratio >= 0.5f)
		return 0.0f;

	f32 force = distanceSquared * ratio;
	if (angleDifference < 0)
		force = -force;
	force *= JMASCos(headDirection - CLBDegToShortAngle(direction));
	return force;
}

void BHSCalcRevisionDistXZByRotateZ(f32 direction, f32 degreeZ,
                                    f32 revisionScale, f32* x, f32* z)
{
	JGeometry::TVec2<f32> offset(revisionScale * degreeZ, 0.0f);
	s16 angle = CLBDegToShortAngle(direction);
	f32 s     = JMASSin(angle);
	f32 c     = JMASCos(angle);
	*x        = offset.x * c + offset.y * s;
	*z        = -offset.x * s + offset.y * c;
}

// TODO: why TF are these defined here and aren't even weak? Mysterious.
void TWaterHitActor::onWaterHitCounter() { mWaterHitCounter = 0x3C; }

BOOL TWaterHitActor::receiveMessage(THitActor* sender, u32 message)
{
	BOOL result = false;

	if (message == HIT_MESSAGE_SPRAYED_BY_WATER) {
		if (SMSGetMarDirector()->isThing()) {
			mWaterHitCounter = 0;
		} else {
			onWaterHitCounter();
		}
		result = true;
	}

	return result;
}

TSphereLink::TSphereLink(u16 count, const JGeometry::TVec3<f32>& position,
                         f32 linkLength, f32 collisionRadius,
                         f32 velocityDamping, f32 gravity, f32 revisionScale,
                         f32 initialDirection)
{
	mPointCount       = count;
	mPoints           = new TSpherePoint[count];
	mVelocityDamping  = velocityDamping;
	mGravity          = gravity;
	mCollisionRadius  = collisionRadius;
	mRevisionScale    = revisionScale;
	mInitialDirection = initialDirection;
	f32 dx            = MsSin(mInitialDirection) * linkLength;
	f32 dz            = MsCos(mInitialDirection) * linkLength;
	for (int i = 0; i < mPointCount; ++i) {
		TSpherePoint& point = mPoints[i];
		JGeometry::TVec3<f32> initialPosition;
		if (i == 0) {
			initialPosition = position;
		} else {
			initialPosition = mPoints[i - 1].mPosition;
			initialPosition.x -= dx;
			initialPosition.z -= dz;
		}
		point.mPosition         = initialPosition;
		point.mPreviousPosition = point.mPosition;
		point.mVelocity.set(0.0f, 0.0f, 0.0f);
		point.mLinkLength = linkLength;
		point.mDegreeZ    = 0.0f;
	}
}

void TSphereLink::execMapCollision_(JGeometry::TVec3<f32>* position)
{
	gpMap->isTouchedOneWallAndMoveXZ(&position->x, position->y, &position->z,
	                                 mCollisionRadius);
	const TBGCheckData* ground;
	f32 height = gpMap->checkGroundIgnoreWaterSurface(
	    position->x, position->y + mCollisionRadius, position->z, &ground);
	if (!ground)
		return;
	if (!ground->isLegal())
		return;
	if (position->y < height)
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
		TSpherePoint& point          = mPoints[i];
		TSpherePoint& previous       = mPoints[i - 1];
		JGeometry::TVec3<f32> offset = point.mPosition - previous.mPosition;
		if (offset.isZero())
			offset.set(0.0f, 1.0f, 0.0f);
		else
			VECNormalize(&offset, &offset);
		offset.scale(point.mLinkLength);
		point.mPosition = previous.mPosition + offset;
		execMapCollision_(&point.mPosition);
	}

	for (int i = 0; i < mPointCount; ++i) {
		JGeometry::TVec3<f32> displacement
		    = mPoints[i].mPosition - mPoints[i].mPreviousPosition;
		mPoints[i].mVelocity         = displacement * mVelocityDamping;
		mPoints[i].mPreviousPosition = mPoints[i].mPosition;
	}
}

BOOL TSphereLink::setDegreeZAndRevisionPosXZ(int index, f32 degreeZ)
{
	TSpherePoint& point = mPoints[index];
	f32 oldDegreeZ      = point.mDegreeZ;
	BOOL changed        = FALSE;
	if (oldDegreeZ != degreeZ) {
		point.mDegreeZ = degreeZ;
		changed        = TRUE;
		f32 direction;
		if (index == 0)
			direction = mInitialDirection;
		else {
			JGeometry::TVec3<f32> offset
			    = mPoints[index - 1].mPosition - point.mPosition;
			direction = MsWrap(MsGetRotFromZaxisY(offset), 0.0f, 360.0f);
		}
		f32 dx, dz;
		BHSCalcRevisionDistXZByRotateZ(direction, degreeZ - oldDegreeZ,
		                               mRevisionScale, &dx, &dz);
		point.mPosition.x += dx;
		point.mPosition.z += dz;
	}
	return changed;
}
