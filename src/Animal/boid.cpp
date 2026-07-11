#include <Animal/boid.hpp>
#include <MarioUtil/RandomUtil.hpp>

TBoidLeader::TBoidLeader(int num, const char* name)
    : JDrama::TViewObj(name)
{
	mNumBoids = num;
	mBoids    = new TBoid[num];
	unk18     = 0;
	unk1C     = 0;
	unk20     = 6.0f;
	unk24     = 150.0f;
	unk28     = 2.0f;
	unk2C     = 2.0f;
	unk30     = 10.0f;
	unk34     = 0.01f;
	unk38     = 0;
	unk3C     = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	unk48     = 1.0f;
	unk58     = 0;
	unk5C     = 0;
	unk60     = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
	unk6C     = 0.0f;
	unk70     = 1.0f;
	unk74     = 0.0f;
	unk78     = 0.0f;
	unk7C     = 0.0f;
	unk4C     = JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f);
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
