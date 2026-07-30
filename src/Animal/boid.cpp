#include <Animal/boid.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <dolphin/mtx.h>

TBoid::TBoid()
{
	mNeighbors = 0;
	mSpeed     = 0.0f;
	mPosition.zero();
	mRotation.zero();
	mHeading.set(0.0f, 0.0f, 1.0f);
	mSpeed = MsRandF();
}

TBoidLeader::TBoidLeader(int num, const char* name)
    : JDrama::TViewObj(name)
    , mNumBoids(num)
    , mBoids(new TBoid[num])
    , mTracer(nullptr)
    , mFlags(0)
    , mBaseSpeed(6.0f)
    , mNeighborRadius(150.0f)
    , mYawSpeed(2.0f)
    , mPitchSpeed(2.0f)
    , mMaxPitch(10.0f)
    , mAlignmentStrength(0.01f)
    , mGoal(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , mGoalStrength(1.0f)
    , unk58(0)
    , mFleeTarget(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , mFleeRadius(0.0f)
    , mFleeStrength(1.0f)
    , mGraphGoal(0.0f, 0.0f, 0.0f)
{
	mGoalOffset.zero();
	mFlags |= FLAG_SIMULATE;
}

void TBoidLeader::calcBoids()
{
	TBoid* i;
	TBoid* end;
	TBoid* j;

	if (mFlags & FLAG_SIMULATE) {
		end = mBoids + mNumBoids;

		for (TBoid* b = mBoids; b != end; ++b) {
			b->mSeparationForce.zero();
			b->mCohesionForce.zero();
			b->mAlignmentForce.zero();
			b->mNeighbors = 0;
		}

		for (i = mBoids; i != end; ++i) {
			for (j = i + 1; j != end; ++j) {
				JGeometry::TVec3<f32> d = i->mPosition;
				d -= j->mPosition;
				f32 d2 = d.squared();
				if (d2 < 0.001f)
					continue;

				f32 radius = mNeighborRadius;
				if (d2 < radius * radius) {
					i->mSeparationForce += d / d2 * radius;
					j->mSeparationForce -= d / d2 * radius;

					i->mAlignmentForce += j->mHeading;
					j->mAlignmentForce += i->mHeading;
					i->mCohesionForce += j->mPosition;
					j->mCohesionForce += i->mPosition;
					i->mNeighbors++;
					j->mNeighbors++;
				}
			}

			if (i->mNeighbors > 0) {
				f32 inv = 1.0f / i->mNeighbors;
				i->mCohesionForce *= inv;
				i->mCohesionForce -= i->mPosition;
				i->mCohesionForce.normalize();

				i->mAlignmentForce *= inv;
				f32 mag = VECMag(&i->mAlignmentForce);
				if (mag > 0.0f) {
					i->mAlignmentForce *= 1.0f / mag;
					i->mAlignmentForce -= i->mHeading;
					i->mAlignmentForce.normalize();
				}
			}
		}

		for (TBoid* b = mBoids; b != end; ++b) {
			JGeometry::TVec3<f32> force = calcForces(b);
			if (!force.isZero()) {
				if (force.y < -0.01f) {
					b->mRotation.x += mPitchSpeed;
					if (b->mRotation.x > mMaxPitch)
						b->mRotation.x = mMaxPitch;
				} else if (force.y > 0.01f) {
					b->mRotation.x -= mPitchSpeed;
					if (b->mRotation.x < -mMaxPitch)
						b->mRotation.x = -mMaxPitch;
				} else {
					b->mRotation.x *= 0.98f;
				}

				f32 targetYaw = MsGetRotFromZaxisY(force);
				f32 diff      = MsAngleDiff(targetYaw, b->mRotation.y);
				if (diff < -0.01f)
					diff = -mYawSpeed;
				else if (diff > 0.01f)
					diff = mYawSpeed;

				f32 newYaw = b->mRotation.y + diff;
				while (newYaw >= 360.0f)
					newYaw -= 360.0f;
				while (newYaw < 0.0f)
					newYaw += 360.0f;
				b->mRotation.y = newYaw;
			}

			Mtx m;
			MsMtxSetRotRPH(m, b->mRotation.x, b->mRotation.y, b->mRotation.z);
			b->mHeading.set(m[0][2], m[1][2], m[2][2]);
			VECNormalize(&b->mHeading, &b->mHeading);

			b->mPosition
			    += b->mHeading * (mBaseSpeed + b->mSpeed) * force.length();
		}
	}
}

void TBoidLeader::setGraph(TGraphWeb* web, const JGeometry::TVec3<f32>& pos)
{
	if (web == nullptr || web->isDummy())
		return;

	if (mTracer == nullptr)
		mTracer = new TGraphTracer();

	mTracer->setGraph(web);
	mTracer->setTo(web->findNearestNodeIndex(pos, -1));

	mGraphGoal.set(mTracer->getCurrentPos());

	mFlags |= FLAG_USE_GRAPH_GOAL;
}

void TBoidLeader::perform(u32 cue, JDrama::TGraphics* graphics)
{
	if (cue & CUE_CALC_ANIM) {
		updateGoal();
		calcBoids();
	}
}

void TBoidLeader::updateGoal()
{
	if (mFlags & FLAG_USE_GRAPH_GOAL) {
		JGeometry::TVec3<f32> dir = mTracer->getCurrentPos();
		dir -= mGraphGoal;
		if (dir.squared() < 10000.0f) {
			mTracer->moveToRandomNext();
		} else {
			VECNormalize(&dir, &dir);
			dir *= 0.9f * mBaseSpeed;
			mGraphGoal += dir;
		}
	}
}

JGeometry::TVec3<f32>
TBoidLeader::calcGoalForce(const JGeometry::TVec3<f32>& pos) const
{
	JGeometry::TVec3<f32> force;
	if (mFlags & FLAG_USE_GRAPH_GOAL) {
		force.set(mGraphGoal);
		force -= pos;
		force.normalize();
	} else {
		force.set(mGoal.getPoint());
		force += mGoalOffset;
		force -= pos;
		f32 len = force.length();
		if (0.0f < len) {
			force /= len;
			force *= mGoalStrength;
		} else {
			force.zero();
		}
	}
	return force;
}

JGeometry::TVec3<f32> TBoidLeader::calcForces(const TBoid* boid) const
{
	JGeometry::TVec3<f32> force = boid->mSeparationForce;
	force += boid->mAlignmentForce * mAlignmentStrength;
	force += boid->mCohesionForce;
	force += calcGoalForce(boid->mPosition);

	if (force.squared() == 0.0f)
		return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

	force *= 0.01f * (5.0f * MsRandF() + 95.0f);
	force.normalize();

	if (0.0f < mFleeRadius) {
		f32 tmp = mFleeRadius;

		JGeometry::TVec3<f32> away = boid->mPosition;
		away -= mFleeTarget.getPoint();
		f32 d2 = away.squared();
		if (0.0f < d2 && d2 < tmp * tmp) {
			away.setLength(mFleeStrength);
			force = away;
		}
	}

	return force;
}
