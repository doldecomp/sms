#include <Animal/boid.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>
#include <dolphin/mtx.h>

TBoid::TBoid()
{
	unk48 = 0;
	unk4C = 0.0f;
	unk0.zero();
	unkC.zero();
	unk18.set(0.0f, 0.0f, 1.0f);
	unk4C = MsRandF();
}

TBoidLeader::TBoidLeader(int num, const char* name)
    : JDrama::TViewObj(name)
    , mNumBoids(num)
    , mBoids(new TBoid[num])
    , unk18(nullptr)
    , unk1C(0)
    , unk20(6.0f)
    , unk24(150.0f)
    , unk28(2.0f)
    , unk2C(2.0f)
    , unk30(10.0f)
    , unk34(0.01f)
    , unk38(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , unk48(1.0f)
    , unk58(0)
    , unk5C(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f))
    , unk6C(0.0f)
    , unk70(1.0f)
    , unk74(0.0f, 0.0f, 0.0f)
{
	unk4C.zero();
	unk1C |= 1;
}

void TBoidLeader::calcBoids()
{
	TBoid* i;
	TBoid* end;
	TBoid* j;

	if (unk1C & 1) {

		end = mBoids + mNumBoids;

		for (TBoid* b = mBoids; b != end; ++b) {
			b->unk24.zero();
			b->unk3C.zero();
			b->unk30.zero();
			b->unk48 = 0;
		}

		for (i = mBoids; i != end; ++i) {
			for (j = i + 1; j != end; ++j) {
				JGeometry::TVec3<f32> d = i->unk0;
				d -= j->unk0;
				f32 d2 = d.squared();
				if (d2 < 0.001f)
					continue;

				f32 radius = unk24;
				if (d2 < radius * radius) {
					i->unk24 += d / d2 * radius;
					j->unk24 -= d / d2 * radius;

					i->unk30 += j->unk18;
					j->unk30 += i->unk18;
					i->unk3C += j->unk0;
					j->unk3C += i->unk0;
					i->unk48++;
					j->unk48++;
				}
			}

			if (i->unk48 > 0) {
				f32 inv = 1.0f / i->unk48;
				i->unk3C *= inv;
				i->unk3C -= i->unk0;
				i->unk3C.normalize();

				i->unk30 *= inv;
				f32 mag = VECMag(&i->unk30);
				if (mag > 0.0f) {
					i->unk30 *= 1.0f / mag;
					i->unk30 -= i->unk18;
					i->unk30.normalize();
				}
			}
		}

		for (TBoid* b = mBoids; b != end; ++b) {
			JGeometry::TVec3<f32> force = calcForces(b);
			if (!force.isZero()) {
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
				f32 diff      = MsAngleDiff(targetYaw, b->unkC.y);
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
			VECNormalize(&b->unk18, &b->unk18);

			b->unk0 += b->unk18 * (unk20 + b->unk4C) * force.length();
		}
	}
}

void TBoidLeader::setGraph(TGraphWeb* web, const JGeometry::TVec3<f32>& pos)
{
	if (web == nullptr || web->isDummy())
		return;

	if (unk18 == nullptr)
		unk18 = new TGraphTracer();

	unk18->setGraph(web);
	unk18->setTo(web->findNearestNodeIndex(pos, -1));

	unk74.set(unk18->getCurrentPos());

	unk1C |= 4;
}

void TBoidLeader::perform(u32 flags, JDrama::TGraphics* graphics)
{
	if (flags & 2) {
		updateGoal();
		calcBoids();
	}
}

void TBoidLeader::updateGoal()
{
	if (unk1C & 4) {
		JGeometry::TVec3<f32> dir = unk18->getCurrentPos();
		dir -= unk74;
		if (dir.squared() < 10000.0f) {
			unk18->moveToRandomNext();
		} else {
			VECNormalize(&dir, &dir);
			dir *= 0.9f * unk20;
			unk74 += dir;
		}
	}
}

JGeometry::TVec3<f32>
TBoidLeader::calcGoalForce(const JGeometry::TVec3<f32>& pos) const
{
	JGeometry::TVec3<f32> force;
	if (unk1C & 4) {
		force.set(unk74);
		force -= pos;
		force.normalize();
	} else {
		force.set(unk38.getPoint());
		force += unk4C;
		force -= pos;
		f32 len = force.length();
		if (0.0f < len) {
			force /= len;
			force *= unk48;
		} else {
			force.zero();
		}
	}
	return force;
}

JGeometry::TVec3<f32> TBoidLeader::calcForces(const TBoid* boid) const
{
	JGeometry::TVec3<f32> force = boid->unk24;
	force += boid->unk30 * unk34;
	force += boid->unk3C;
	force += calcGoalForce(boid->unk0);

	if (force.squared() == 0.0f)
		return JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);

	force *= 0.01f * (5.0f * MsRandF() + 95.0f);
	force.normalize();

	if (0.0f < unk6C) {
		f32 tmp = unk6C;

		JGeometry::TVec3<f32> away = boid->unk0;
		away -= unk5C.getPoint();
		f32 d2 = away.squared();
		if (0.0f < d2 && d2 < tmp * tmp) {
			away.setLength(unk70);
			force = away;
		}
	}

	return force;
}
