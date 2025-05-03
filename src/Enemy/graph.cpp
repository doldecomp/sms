#include <Enemy/Graph.hpp>
#include <Enemy/Spline.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <types.h>

// rogue include
#include <M3DUtil/InfectiousStrings.hpp>

TGraphNode::TGraphNode()
    : unk0(nullptr)
    , unk4(0)
    , unk8(0.0f)
    , unkC(-0x7FFF)
{
}

void TGraphNode::getPoint(Vec* v) const
{
	v->x = unk0->unk0.x;
	v->y = unk0->unk0.y;
	v->z = unk0->unk0.z;
}

TSplineRail::TSplineRail(const TGraphWeb* param_1)
    : unk0(nullptr)
    , unk4(0)
{
	TRailNode* node = param_1->unk0[0].unk0;

	BOOL appendMorePoints;
	if (node->unk6 > 2
	    || (node->unk14 != param_1->unk8 - 1
	        && node->unk16 != param_1->unk8 - 1))
		appendMorePoints = false;
	else if (node->unk6 > 2 || (node->unk14 != 0 && node->unk16 != 0))
		appendMorePoints = false;
	else
		appendMorePoints = true;

	// TODO: need an S16Vec -> Vec inline of some sort...
	if (appendMorePoints) {
		unk4 = 1;
		unk0 = new TSplinePath(param_1->unk8 + 3);

		TRailNode* lastNode = param_1->unk0[param_1->unk8 - 1].unk0;
		JGeometry::TVec3<f32> lastV;
		lastV.set(lastNode->unk0);
		unk0->setPoint(0, lastV);

		for (int i = 0; i < param_1->unk8; ++i) {
			TRailNode* node = param_1->unk0[i + 1].unk0;
			JGeometry::TVec3<f32> v;
			v.set(node->unk0);
			unk0->setPoint(i + 1, v);
		}

		TRailNode* firstNode = param_1->unk0[0].unk0;
		JGeometry::TVec3<f32> firstV;
		firstV.set(firstNode->unk0);
		unk0->setPoint(param_1->unk8 + 1, firstV);

		TRailNode* firstNode2 = param_1->unk0[1].unk0;
		JGeometry::TVec3<f32> firstV2;
		firstV2.set(firstNode2->unk0);
		unk0->setPoint(param_1->unk8 + 2, firstV2);

	} else {
		unk0 = new TSplinePath(param_1->unk8);

		for (int i = 0; i < param_1->unk8; ++i) {
			TRailNode* node = param_1->unk0[i].unk0;
			JGeometry::TVec3<f32> v;
			v.set(node->unk0);
			unk0->setPoint(i, v);
		}
	}

	unk0->getPoint(0.0f);
}

void TSplineRail::wrapT(f32) { }

void TSplineRail::getNthT(int) { }

void TSplineRail::getPosition(f32) { }

void TSplineRail::getPosAndRot(f32, JGeometry::TVec3<f32>*,
                               JGeometry::TVec3<f32>*)
{
}

static TRailNode grDummyRail[3];

TGraphWeb::TGraphWeb(TRailNode* param_1, const char* param_2, int param_3)
    : unk0(nullptr)
    , unk4(param_1)
    , unk8(0)
    , unkC(param_2)
    , unk10(-1)
    , unk14(nullptr)
{
	if (!param_1) {
		param_1 = grDummyRail;
		unk4    = grDummyRail;
	}

	if (param_3 < 1) {
		int i = 0;
		while (param_1[i].unk6 > 0)
			++i;
		unk8 = i;
	} else {
		unk8 = param_3;
	}

	unk0 = new TGraphNode[unk8];

	for (int i = 0; i < unk8; ++i)
		unk0[i].unk0 = &param_1[i];

	translateNodes(param_1);
	if (param_2[0] == 'S' && param_2[1] == '_')
		unk14 = new TSplineRail(this);
}

TGraphWeb::~TGraphWeb() { }

void TGraphWeb::filterRailNode(u32, const TRailNode*, const TRailNode*,
                               TRailNode*) const
{
}

void TGraphWeb::translateNodes(TRailNode*) { }

void TGraphWeb::getAimToGoalNextIndex(int, int, u32, f32) const { }

void TGraphWeb::getShortestNextIndex(int, int, u32) const { }

void TGraphWeb::getRandomNextIndex(int, int, u32) const { }

void TGraphWeb::getEscapeFromMarioIndex(int, int, const JGeometry::TVec3<f32>&,
                                        u32) const
{
}

void TGraphWeb::getAimToDirNextIndex(int, int, const JGeometry::TVec3<f32>&,
                                     const JGeometry::TVec3<f32>&, u32) const
{
}

void TGraphWeb::getRandomButDirLimited(int, int, const JGeometry::TVec3<f32>&,
                                       const JGeometry::TVec3<f32>&, f32,
                                       u32) const
{
}

void TGraphWeb::getEscapeDirLimited(int, int, const JGeometry::TVec3<f32>&,
                                    const JGeometry::TVec3<f32>&, f32,
                                    u32) const
{
}

void TGraphWeb::findNearestNodeIndex(const JGeometry::TVec3<f32>&, u32) const {
}

void TGraphWeb::findFarthestNodeIndex(const JGeometry::TVec3<f32>&, u32) const
{
}

void TGraphWeb::findNearestVisibleIndex(const JGeometry::TVec3<f32>&, f32, f32,
                                        f32, u32) const
{
}

void TGraphWeb::findNearestNodeIndexCheckY(const JGeometry::TVec3<f32>&, f32,
                                           u32) const
{
}

void TGraphWeb::getNodeIndexInXZRange(const JGeometry::TVec3<f32>&, f32,
                                      u32) const
{
}

void TGraphWeb::calcGraphDirection(int) { }

void TGraphWeb::initGoalIndex(const Vec&) { }

void TGraphWeb::attachToGround() { }

void TGraphWeb::isOnePath() const { }

void TGraphWeb::startIsEnd() const { }

void TGraphWeb::indexToPoint(int) const { }

void TGraphWeb::perform(u32, JDrama::TGraphics*) { }

BOOL TGraphWeb::isDummy() const { }

void TGraphWeb::getNearestPosOnGraphLink(const JGeometry::TVec3<f32>&) const { }

void TGraphWeb::getNeighborNodeIndexByFlag(int, int, u32) const { }

void TGraphWeb::getDesignatedNodeIndex(u32, int, f32) const { }

TGraphGroup::TGraphGroup(void*) { }

TGraphGroup::~TGraphGroup() { }

void TGraphGroup::initGraphGroup() { }

void TGraphGroup::getGraphByName(const char*) { }

void TGraphGroup::perform(u32, JDrama::TGraphics*) { }

TGraphTracer::TGraphTracer()
    : unk0(nullptr)
    , unk4(-1)
    , unk8(-1)
    , unkC(100.0f)
    , unk10(1.0f)
    , unk14(0.0f)
{
}

void TGraphTracer::setParamFromGraph()
{
	if (unk4 >= 0) {
		unk10 = (u16)unk0->unk0[unk4].unk0->unkC * (1.0f / 65535.0f);
	} else {
		unk10 = 0.0f;
	}

	if (unk8 >= 0)
		unkC = (u16)unk0->unk0[unk8].unk0->unkE * 0.01f;
}

void TGraphTracer::setTo(int param_1)
{
	unk8 = -1;
	unk4 = param_1;
	setParamFromGraph();
}

int TGraphTracer::moveTo(int param_1)
{
	if (param_1 < 0)
		return param_1;

	unk8 = unk4;
	unk4 = param_1;
	setParamFromGraph();
	return param_1;
}

f32 TGraphTracer::calcSplineSpeed(f32 param_1)
{
	if (unk0->unk14 == nullptr)
		return 0.001f;

	if (unk8 < 0)
		return 0.001f;

	Vec v1;
	unk0->unk0[unk4].getPoint(&v1);
	Vec v2;
	unk0->unk0[unk8].getPoint(&v2);

	JGeometry::TVec3<f32> diff;
	diff.x     = v1.x - v2.x;
	diff.y     = v1.y - v2.y;
	diff.z     = v1.z - v2.z;
	f32 fVar13 = VECMag(&diff);

	f32 fVar1;
	f32 fVar2;
	if (unk0->unk14->unk4 && unk8 == unk0->unk8 - 1 && unk4 == 0) {
		fVar1 = unk0->unk14->unk0->mParametrization[0];
		fVar2 = unk0->unk14->unk0->mParametrization[1];
	} else if (unk0->unk14->unk4 && unk8 == 0 && unk4 == unk0->unk8 - 1) {
		fVar1 = unk0->unk14->unk0->mParametrization[unk8 + 1];
		fVar2 = unk0->unk14->unk0->mParametrization[unk8];
	} else {
		u32 uVar10 = unk8;
		if (unk0->unk14->unk4)
			uVar10 += 1;
		fVar1     = unk0->unk14->unk0->mParametrization[uVar10];
		u32 uVar7 = unk4;
		if (unk0->unk14->unk4)
			uVar7 += 1;
		fVar2 = unk0->unk14->unk0->mParametrization[uVar7];
	}

	return param_1 * (fVar2 - fVar1) / fVar13;
}

bool TGraphTracer::traceSpline(f32 param_1)
{
	if (!unk0->unk14)
		return false;

	f32 dVar8 = unk14;
	f32 dVar9 = dVar8 + param_1;

	f32 dVar10;
	if (unk0->unk14->unk4 && unk8 == unk0->unk8 - 1 && unk4 == 0) {
		dVar10 = unk0->unk14->unk0->mParametrization[unk8 + 1];
	} else if (unk0->unk14->unk4 && unk8 == 0 && unk4 == unk0->unk8 - 1) {
		dVar10 = unk0->unk14->unk0->mParametrization[unk8];
	} else {
		u32 uVar7 = unk4;
		if (unk0->unk14->unk4)
			uVar7 += 1;
		dVar10 = unk0->unk14->unk0->mParametrization[uVar7];
	}

	bool result;
	if ((param_1 >= 0.0f && dVar8 <= dVar10 && dVar10 <= dVar9)
	    || (param_1 < 0.0f && dVar9 <= dVar10 && dVar10 <= dVar8)) {
		result = true;
	} else {
		result = false;
	}

	if (unk0->unk14->unk4) {
		dVar9 = MsWrap<f32>(
		    dVar9, unk0->unk14->unk0->mParametrization[1],
		    unk0->unk14->unk0
		        ->mParametrization[unk0->unk14->unk0->mPointNum - 2]);
	}

	unk14 = dVar9;
	return result;
}
