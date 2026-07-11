#include <Animal/boid.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <dolphin/mtx.h>

TBoidLeader::TBoidLeader(int num, const char* name)
    : JDrama::TViewObj(name)
{
	mNumBoids  = num;
	mBoids     = new TBoid[num];
	unk18      = 0;
	unk1C      = 0;
	unk20      = 6.0f;
	unk24      = 150.0f;
	unk28      = 2.0f;
	unk2C      = 2.0f;
	unk30      = 10.0f;
	unk34      = 0.01f;
	unk38.unk0 = nullptr;
	unk38.unk4 = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	unk48      = 1.0f;
	unk58      = 0;
	unk5C.unk0 = nullptr;
	unk5C.unk4 = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	unk6C      = 0.0f;
	unk70      = 1.0f;
	unk74      = 0.0f;
	unk78      = 0.0f;
	unk7C      = 0.0f;
	unk4C      = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	unk1C |= 1;
}

TBoidLeader::~TBoidLeader() { }

void TBoidLeader::setGraph(TGraphWeb* web, const JGeometry::TVec3<f32>& pos)
{
	if (web == nullptr)
		return;
	if (web->isDummy())
		return;

	if (unk18 == nullptr)
		unk18 = new TGraphTracer();

	unk18->setGraph(web);
	unk18->setTo(web->findNearestNodeIndex(pos, 0xffffffff));

	// TODO: 86.8% - logic correct; the original keeps a separate TVec3 copy of
	// indexToPoint's by-value return that MWCC elides here (unsolved
	// copy-count).
	JGeometry::TVec3<f32> pt
	    = unk18->getGraph()->indexToPoint(unk18->getCurGraphIndex());
	unk74 = pt.x;
	unk78 = pt.y;
	unk7C = pt.z;

	unk1C |= 4;
}

JGeometry::TVec3<f32>
TBoidLeader::calcGoalForce(const JGeometry::TVec3<f32>& pos) const
{
	// TODO: logic-complete; by-value TVec3 return hits the unsolved MWCC
	// copy-count problem (same as getRotationFlyToDir in AnimalBase).
	JGeometry::TVec3<f32> force;
	if (unk1C & 4) {
		force.set(unk74, unk78, unk7C);
		force.sub(pos);
		force.setLength(1.0f);
	} else {
		const JGeometry::TVec3<f32>& node = unk38.getPoint();
		force.set(node.x + unk4C.x, node.y + unk4C.y, node.z + unk4C.z);
		force.sub(pos);
		f32 len = force.length();
		if (len > 0.0f) {
			force.scale(1.0f / len);
			force.scale(unk48);
		} else {
			force.set(0.0f, 0.0f, 0.0f);
		}
	}
	return force;
}

JGeometry::TVec3<f32> TBoidLeader::calcForces(const TBoid* boid) const
{
	// TODO: logic-complete; by-value TVec3 return hits the unsolved MWCC
	// copy-count problem (same as getRotationFlyToDir in AnimalBase).
	JGeometry::TVec3<f32> force(boid->unk24);

	JGeometry::TVec3<f32> heading(boid->unk30);
	heading.scale(unk34);
	force.add(heading);

	force.add(boid->unk3C);
	force.add(calcGoalForce(boid->unk0));

	if (force.squared() == 0.0f) {
		JGeometry::TVec3<f32> zero(0.0f, 0.0f, 0.0f);
		return zero;
	}

	force.scale(0.01f * (5.0f * MsRandF() + 95.0f));
	force.setLength(1.0f);

	if (unk6C > 0.0f) {
		JGeometry::TVec3<f32> away(boid->unk0);
		away.sub(unk5C.getPoint());
		f32 d2 = away.squared();
		if (d2 > 0.0f && d2 < unk6C * unk6C) {
			away.setLength(unk70);
			force = away;
		}
	}

	return force;
}

void TBoidLeader::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 2) {
		if (unk1C & 4) {
			JGeometry::TVec3<f32> dir
			    = unk18->getGraph()->indexToPoint(unk18->getCurGraphIndex());
			dir.x -= unk74;
			dir.y -= unk78;
			dir.z -= unk7C;
			if (dir.squared() < 10000.0f) {
				unk18->moveTo(unk18->getGraph()->getRandomNextIndex(
				    unk18->getCurGraphIndex(), unk18->getPrevIndex(),
				    0xffffffff));
			} else {
				PSVECNormalize(&dir, &dir);
				f32 step = 0.9f * unk20;
				dir.scale(step);
				unk74 += dir.x;
				unk78 += dir.y;
				unk7C += dir.z;
			}
		}
		calcBoids();
	}
}

void TBoidLeader::calcBoids()
{
	// TODO: logic-complete; nested pair loop over the flock. The by-value
	// calcForces call and heavy TVec3 math leave MWCC copy-count residuals.
	if (unk1C & 1) {
		TBoid* end = mBoids + mNumBoids;

		for (TBoid* b = mBoids; b != end; ++b) {
			b->unk24.set(0.0f, 0.0f, 0.0f);
			b->unk3C.set(0.0f, 0.0f, 0.0f);
			b->unk30.set(0.0f, 0.0f, 0.0f);
			b->unk48 = 0;
		}

		for (TBoid* i = mBoids; i != end; ++i) {
			for (TBoid* j = i + 1; j != end; ++j) {
				JGeometry::TVec3<f32> d;
				d.sub(i->unk0, j->unk0);
				f32 d2 = d.squared();
				if (d2 < 0.001f)
					continue;

				f32 radius = unk24;
				if (d2 < radius * radius) {
					JGeometry::TVec3<f32> sep = d;
					sep.div(d2);
					sep.scale(radius);
					i->unk24.add(sep);

					JGeometry::TVec3<f32> sep2 = d;
					sep2.div(d2);
					sep2.scale(radius);
					j->unk24.sub(sep2);

					i->unk30.add(j->unk18);
					j->unk30.add(i->unk18);
					i->unk3C.add(j->unk0);
					j->unk3C.add(i->unk0);
					i->unk48++;
					j->unk48++;
				}
			}

			if (i->unk48 > 0) {
				f32 inv = 1.0f / i->unk48;
				i->unk3C.scale(inv);
				i->unk3C.sub(i->unk0);
				i->unk3C.setLength(1.0f);

				i->unk30.scale(inv);
				f32 mag = PSVECMag(&i->unk30);
				if (mag > 0.0f) {
					i->unk30.scale(1.0f / mag);
					i->unk30.sub(i->unk18);
					i->unk30.setLength(1.0f);
				}
			}
		}

		for (TBoid* b = mBoids; b != end; ++b) {
			JGeometry::TVec3<f32> force = calcForces(b);
			if (force.squared() != 0.0000038146973f) {
				if (force.y < -0.01f) {
					b->unkC.x += unk2C;
					if (b->unkC.x > unk30)
						b->unkC.x = unk30;
				} else if (force.y > 0.01f) {
					b->unkC.x -= unk2C;
					if (b->unkC.x < -unk30)
						b->unkC.x = -unk30;
				} else {
					b->unkC.x *= 0.98f;
				}

				f32 targetYaw = MsGetRotFromZaxisY(force);
				f32 diff      = targetYaw
				           - MsWrap(b->unkC.y, targetYaw - 180.0f,
				                    targetYaw + 180.0f);
				if (diff < -0.01f)
					diff = -unk28;
				else if (diff > 0.01f)
					diff = unk28;

				f32 newYaw = b->unkC.y + diff;
				while (newYaw >= 360.0f)
					newYaw -= 360.0f;
				while (newYaw < 0.0f)
					newYaw += 360.0f;
				b->unkC.y = newYaw;
			}

			Mtx m;
			MsMtxSetRotRPH(m, b->unkC.x, b->unkC.y, b->unkC.z);
			b->unk18.set(m[0][2], m[1][2], m[2][2]);
			PSVECNormalize(&b->unk18, &b->unk18);

			f32 speed                  = force.length();
			JGeometry::TVec3<f32> step = b->unk18;
			step.scale(unk20 + b->unk4C);
			step.scale(speed);
			b->unk0.add(step);
		}
	}
}

TBoid::TBoid()
{
	unk48 = 0;
	unk4C = 0.0f;
	unk0.set(0.0f, 0.0f, 0.0f);
	unkC.set(0.0f, 0.0f, 0.0f);
	unk18.set(0.0f, 0.0f, 1.0f);
	unk4C = MsRandF();
}
