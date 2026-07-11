#include <Animal/boid.hpp>
#include <MarioUtil/RandomUtil.hpp>

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

TBoid::TBoid()
{
	unk48 = 0;
	unk4C = 0.0f;
	unk0.set(0.0f, 0.0f, 0.0f);
	unkC.set(0.0f, 0.0f, 0.0f);
	unk18 = 0.0f;
	unk1C = 0.0f;
	unk20 = 1.0f;
	unk4C = MsRandF();
}
