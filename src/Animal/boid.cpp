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

// TODO: 95.5%. Frame 0x150 vs 0x1e0. Every remaining structural difference is
// the same shape at the three `d / d2 * radius` / `*= 1.0f / mag` sites: retail
// copies the operator's result *through its own return slot* (0x84 -> 0x110 ->
// 0xc0, six instructions per site) where we copy the by-value parameter temp
// straight into the next operator's. That is the batch-119 "by-value return"
// geometry -- retail's JGeometry operators return TVec3 by value and ours return
// a reference -- so it cannot be fixed here (JGVec3.hpp is a shared header and
// the by-value return was rejected project-wide). Declaring `radius` before `d2`
// as a C-style block declaration is what puts d2 in f27 and radius in f22 as
// retail does (measured: 175 -> 170 differing operands). Rejected: an explicit
// `JGeometry::TVec3<f32>(d / d2)` temporary at the two separation sites (adds
// four instructions and 0x20 of frame, 93.1).
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
				f32 radius;
				f32 d2 = d.squared();
				if (d2 < 0.001f)
					continue;

				radius = mNeighborRadius;
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
// TODO: frame 0x50 vs 0x48 -- we are 8 bytes *too big* and the returned vector's
// temp sits at 0x38 where retail has it at 0x34 (4 extra low bytes and 4 extra
// bytes of pad above it). Every instruction matches. One inline level or one
// named scalar too many; `f32 len` is the only candidate in the body.
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

// TODO: frame 0xa0 vs 0x90 -- 16 bytes too big. Slot triage: the calcGoalForce
// sret slot is at 0x38 in *both* builds, while the three other 12-byte vector
// temps are all exactly 12 bytes higher in ours (0x44/0x50/0x70 against retail's
// 0x28/0x44/0x64), i.e. retail's pool has one more 12-byte entry at the bottom
// and ours is hoisted. Same per-statement pool geometry as calcBoids above.
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
