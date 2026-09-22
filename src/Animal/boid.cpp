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

// TODO: 96.1%. Frame 0x150 vs 0x1e0. Retail reloads mNeighborRadius after
// the first separation store (`lfs f27, 0x24(r28)`), so the sites read the
// member, not a `radius` local (cc32). The structural residue is the
// `d / d2 * r` product: retail copies the quotient through its own return
// slot (0x84 -> 0x110 -> 0xc0, six instructions per site) before the
// product's argument copy, i.e. its operator* took the quotient by
// reference. A TU-local `BoidMul(const TVec3& v, f32 s) { return v * s; }`
// at both sites reproduces those copies (96.1 -> 98.3, frame 0x168) but the
// frame stays 0x78 short and the third loop's six hoisted literals take
// callee-saved FPRs in another order (retail f27/f26/f23/f28/f25/f24), so it
// is not committed; the header-level operator* form is the known-open
// by-value-return class (JGVec3.hpp's comment above operator*). Rejected: an
// explicit `JGeometry::TVec3<f32>(d / d2)` temporary (93.1), `BoidMul2` with
// a named copy and `*=` (97.7), `BoidDiv(d, d2) * r` (90.3).
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

				if (d2 < mNeighborRadius * mNeighborRadius) {
					i->mSeparationForce += d / d2 * mNeighborRadius;
					j->mSeparationForce -= d / d2 * mNeighborRadius;

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
// Closed by re-pass II: the 4 extra bytes were `TUtil<f32>::one()`, which the
// rules card already prices at +4 -- `normalize()` is
// `setLength(*this, TUtil<f32>::one())`, so spelling the unit vector
// `force.setLength(1.0f)` drops that one level and lands retail's 0x48 frame
// and every displacement (99.6 -> 100.0). Only `calcGoalForce` wants it: the
// same respelling is -8 in `calcForces` (0xa0 -> 0x98, still 8 over, and
// 99.6 -> 99.5) and -8 in `calcBoids`, which is already 144 bytes short, so
// the spelling is per call site here.
// Inert on this function, measured in the same pass: a consumed
// `const TVec3<f32>& goal = mGoal.getPoint();` binding, the pointer form
// `const TVec3<f32>* goal = &mGoal.getPoint();`, `force.set(0,0,0)` for
// `zero()`, and splitting `force` into one local per branch with an early
// return (0x60). Worse: `force = mGraphGoal` for `force.set(mGraphGoal)`
// (97.3). Statement-by-statement pool triage: every `+=`/`-=`/`*=` site costs
// 0 bytes and the whole low region is the normalize expansion's 16.
TBoidLeader::calcGoalForce(const JGeometry::TVec3<f32>& pos) const
{
	JGeometry::TVec3<f32> force;
	if (mFlags & FLAG_USE_GRAPH_GOAL) {
		force.set(mGraphGoal);
		force -= pos;
		force.setLength(1.0f);
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

// `away` is declared at the top and assigned later so it sits directly
// under `force` in the named block, as retail's (cc32).  A TU-local raw
// getPoint at the mFleeTarget site is -0x10 of frame.
static inline const JGeometry::TVec3<f32>& BoidGetPoint(const TPathNode& node)
{
	if (node.unk0 != 0)
		return node.unk0->mPosition;

	return node.unk4;
}

// A direct-return level around the alignment product: it moves the
// operator's by-value argument below calcGoalForce's return slot, retail's
// pool order (cc32; a helper wrapping the whole `+=` or a scalar fork over
// mAlignmentStrength is not it).
static inline JGeometry::TVec3<f32> BoidAlignForce(const TBoidLeader* leader,
                                                   const TBoid* boid)
{
	return boid->mAlignmentForce * leader->mAlignmentStrength;
}

JGeometry::TVec3<f32> TBoidLeader::calcForces(const TBoid* boid) const
{
	JGeometry::TVec3<f32> force = boid->mSeparationForce;
	JGeometry::TVec3<f32> away;
	force += BoidAlignForce(this, boid);
	force += boid->mCohesionForce;
	force += calcGoalForce(boid->mPosition);

	if (force.squared() == 0.0f)
		return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

	force *= 0.01f * (5.0f * MsRandF() + 95.0f);
	force.normalize();

	if (0.0f < mFleeRadius) {
		f32 tmp = mFleeRadius;

		away = boid->mPosition;
		away -= BoidGetPoint(mFleeTarget);
		f32 d2 = away.squared();
		if (0.0f < d2 && d2 < tmp * tmp) {
			away.setLength(mFleeStrength);
			force = away;
		}
	}

	return force;
}
