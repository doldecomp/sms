#include <Enemy/Graph.hpp>
#include <Enemy/Spline.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Map/MapData.hpp>
#include <Map/Map.hpp>
#include <stdlib.h>
#include <types.h>

static void dummy(Vec* v)
{
	static Vec v1  = { 1.0f, 1.0f, 1.0f };
	static Vec v2  = { 1.0f, 1.0f, 1.0f };
	static int a[] = { 0, 2, 1, 3 };
}

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
	v->x = unk0->mPosition.x;
	v->y = unk0->mPosition.y;
	v->z = unk0->mPosition.z;
}

TSplineRail::TSplineRail(const TGraphWeb* param_1)
    : unk0(nullptr)
    , unk4(0)
{
	// TODO: need an S16Vec -> Vec inline of some sort...
	if (param_1->startIsEnd()) {
		unk4 = 1;
		unk0 = new TSplinePath(param_1->unk8 + 3);

		TRailNode* lastNode = param_1->unk0[param_1->unk8 - 1].unk0;
		JGeometry::TVec3<f32> lastV;
		lastV.set(lastNode->mPosition.x, lastNode->mPosition.y,
		          lastNode->mPosition.z);
		unk0->setPoint(0, lastV);

		for (int i = 0; i < param_1->unk8; ++i) {
			TRailNode* node = param_1->unk0[i + 1].unk0;
			JGeometry::TVec3<f32> v;
			v.set(node->mPosition.x, node->mPosition.y, node->mPosition.z);
			unk0->setPoint(i + 1, v);
		}

		TRailNode* firstNode = param_1->unk0[0].unk0;
		JGeometry::TVec3<f32> firstV;
		firstV.set(firstNode->mPosition.x, firstNode->mPosition.y,
		           firstNode->mPosition.z);
		unk0->setPoint(param_1->unk8 + 1, firstV);

		TRailNode* firstNode2 = param_1->unk0[1].unk0;
		JGeometry::TVec3<f32> firstV2;
		firstV2.set(firstNode2->mPosition.x, firstNode2->mPosition.y,
		            firstNode2->mPosition.z);
		unk0->setPoint(param_1->unk8 + 2, firstV2);

	} else {
		unk0 = new TSplinePath(param_1->unk8);

		for (int i = 0; i < param_1->unk8; ++i) {
			TRailNode* node = param_1->unk0[i].unk0;
			JGeometry::TVec3<f32> v;
			v.set(node->mPosition.x, node->mPosition.y, node->mPosition.z);
			unk0->setPoint(i, v);
		}
	}

	unk0->getPoint(0.0f);
}

f32 TSplineRail::wrapT(f32 param_1)
{
	if (unk4)
		param_1
		    = MsWrap<f32>(param_1, getNthT(1), getNthT(unk0->mPointNum - 2));

	return param_1;
}

// TODO: size is wrong, wtf was this originally...
f32 TSplineRail::getNthT(int n) { return unk0->mParametrization[n]; }

JGeometry::TVec3<f32> TSplineRail::getPosition(f32 t)
{
	return unk0->getPoint(wrapT(t));
}

void TSplineRail::getPosAndRot(f32 t, JGeometry::TVec3<f32>* out_pos,
                               JGeometry::TVec3<f32>* out_rot)
{
	t      = wrapT(t);
	f32 dt = 0.01f;

	JGeometry::TVec3<f32> point;
	JGeometry::TVec3<f32> dir;

	for (;;) {
		if (t + dt > 1.0f)
			t = 1.0f - dt;

		point = getPosition(t);
		dir   = getPosition(t + dt);

		dir.sub(dir, point);

		if (!dir.isZero())
			break;

		dt *= 2.0f;
		if (dt > 1.0f) {
			*out_pos   = point;
			out_rot->x = 0.0f;
			out_rot->y = 0.0f;
			out_rot->z = 0.0f;
			return;
		}
	}

	*out_pos = point;
	*out_rot = MsGetRotFromZaxis(dir);
}

static TRailNode grDummyRail[3] = {
	{ { 0, 0, 0 }, 1, 0, 0, 0, 0, 0, { 1 }, { 1.0f } },
	{ { 0, 0, 0 }, 1, 0, 0, 0, 0, 0, { 0 }, { 1.0f } },
	{},
};

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
		while (param_1[i].mConnectionNum > 0)
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

int TGraphWeb::filterRailNode(u32 param_1, const TRailNode* param_2,
                              const TRailNode* param_3,
                              TRailNode* param_4) const
{
	if (param_1 == 0xffffffff) {
		if (param_4 != param_2)
			*param_4 = *param_2;
		return param_2->mConnectionNum;
	}

	TRailNode node = *param_2;

	int filteredCons = 0;
	for (int j = 0; j < param_2->mConnectionNum; ++j) {
		const TRailNode& other = param_3[param_2->mConnections[j]];
		if (other.mFlags == 0 || (other.mFlags & param_1)) {
			node.mConnections[filteredCons] = param_2->mConnections[j];
			node.mPeriods[filteredCons]     = param_2->mPeriods[j];
			++filteredCons;
		}
	}
	node.mConnectionNum = filteredCons;

	*param_4 = node;

	return filteredCons;
}

void TGraphWeb::translateNodes(TRailNode* param_1)
{
	for (int i = 0; i < unk8; ++i) {
		if (param_1[i].mFlags & 0x8) {
			TRailNode node = param_1[i];
			int conns      = 0;
			for (int j = 0; j < param_1[i].mConnectionNum; ++j) {
				if (!(param_1[param_1[i].mConnections[j]].mFlags & 0x4)) {
					node.mConnections[conns] = param_1[i].mConnections[j];
					node.mPeriods[conns]     = param_1[i].mPeriods[j];
					++conns;
				}
			}
			node.mConnectionNum = conns;
			param_1[i]          = node;
		}

		if (param_1[i].mFlags & 0x200) {
			TRailNode node = param_1[i];
			int conns      = 0;
			for (int j = 0; j < param_1[i].mConnectionNum; ++j) {
				if (param_1[i].mConnections[j] >= i) {
					node.mConnections[conns] = param_1[i].mConnections[j];
					node.mPeriods[conns]     = param_1[i].mPeriods[j];
					++conns;
				}
			}
			node.mConnectionNum = conns;
			param_1[i]          = node;
		}
	}
}

void TGraphWeb::getAimToGoalNextIndex(int, int, u32, f32) const { }

int TGraphWeb::getShortestNextIndex(int param_1, int param_2, u32 param_3) const
{
	TGraphNode* graphNode = &unk0[param_1];

	TRailNode tmp;
	TRailNode* railNode;
	if (param_3 == 0xffffffff) {
		railNode = graphNode->getRailNode();
	} else {
		filterRailNode(param_3, graphNode->getRailNode(), unk4, &tmp);
		railNode = &tmp;
	}

	if (railNode->mConnectionNum == 0)
		return param_1;

	if (railNode->mConnectionNum == 1)
		return railNode->mConnections[0];

	if (railNode->mConnectionNum == 2 && param_2 >= 0) {
		if (railNode->mConnections[0] != param_2)
			return railNode->mConnections[0];
		else
			return railNode->mConnections[1];
	}

	JGeometry::TVec3<f32> railNodePos;
	graphNode->getPoint(&railNodePos);

	int result  = -1;
	f32 minDist = 0.0f;
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> pos;
		unk0[railNode->mConnections[i]].getPoint(&pos);
		pos.sub(railNodePos);
		f32 dist = pos.squared();
		if (result < 0 || minDist > dist) {
			minDist = dist;
			result  = railNode->mConnections[i];
		}
	}

	return result;
}

int TGraphWeb::getRandomNextIndex(int param_1, int param_2, u32 param_3) const
{
	TGraphNode* graphNode = &unk0[param_1];

	TRailNode tmp;
	TRailNode* railNode;
	if (param_3 == 0xffffffff) {
		railNode = graphNode->getRailNode();
	} else {
		filterRailNode(param_3, graphNode->getRailNode(), unk4, &tmp);
		railNode = &tmp;
	}

	if (railNode->mConnectionNum == 0)
		return param_1;

	if (railNode->mConnectionNum == 1)
		return railNode->mConnections[0];

	if (railNode->mConnectionNum == 2 && param_2 >= 0) {
		if (railNode->mConnections[0] != param_2)
			return railNode->mConnections[0];
		else
			return railNode->mConnections[1];
	}

	int rnd = (rand() * (1.0f / RAND_MAX)) * railNode->mConnectionNum;
	int result;
	if (railNode->mConnections[rnd] == param_2
	    && rnd == railNode->mConnectionNum - 1)
		result = 0;
	else
		result = rnd;

	return railNode->mConnections[result];
}

int TGraphWeb::getEscapeFromMarioIndex(int, int, const JGeometry::TVec3<f32>&,
                                       u32) const
{
}

int TGraphWeb::getAimToDirNextIndex(int param_1, int param_2,
                                    const JGeometry::TVec3<f32>& param_3,
                                    const JGeometry::TVec3<f32>& param_4,
                                    u32 param_5) const
{
	TGraphNode* graphNode = &unk0[param_1];

	TRailNode tmp;
	TRailNode* railNode;
	if (param_5 == 0xffffffff) {
		railNode = graphNode->getRailNode();
	} else {
		filterRailNode(param_5, graphNode->getRailNode(), unk4, &tmp);
		railNode = &tmp;
	}

	if (railNode->mConnectionNum == 0)
		return param_1;

	if (railNode->mConnectionNum == 1)
		return railNode->mConnections[0];

	if (railNode->mConnectionNum == 2 && param_2 >= 0) {
		if (railNode->mConnections[0] != param_2)
			return railNode->mConnections[0];
		else
			return railNode->mConnections[1];
	}

	JGeometry::TVec3<f32> local_a4 = param_3;
	MsVECNormalize(&local_a4, &local_a4);

	int result = -1;
	f32 maxCos = 0.0f;
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> local_b0;
		unk0[railNode->mConnections[i]].getPoint(&local_b0);
		local_b0.sub(param_4);
		MsVECNormalize(&local_b0, &local_b0);

		f32 cos = local_b0.dot(local_a4);
		if (result < 0 || cos > maxCos) {
			maxCos = cos;
			result = railNode->mConnections[i];
		}
	}

	return result;
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

int TGraphWeb::findNearestNodeIndex(const JGeometry::TVec3<f32>& param_1,
                                    u32 param_2) const
{
	int i;
	int result  = -1;
	f32 minDist = -1.0f;
	for (i = 0; i < unk8; ++i) {
		if (param_2 == -1 || !(unk0[i].unk0->mFlags & param_2)) {
			JGeometry::TVec3<f32> pos;
			unk0[i].getPoint(&pos);
			pos.sub(param_1);
			f32 dist = pos.squared();
			if (minDist < 0.0f || dist < minDist) {
				minDist = dist;
				result  = i;
			}
		}
	}
	return result;
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

#pragma dont_inline on
void TGraphWeb::calcGraphDirection(int n)
{
	TGraphNode& graphNode = getGraphNode(n);
	TRailNode* railNode   = graphNode.getRailNode();
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		f32 totalPeriod = railNode->mPeriods[i] + graphNode.getUnk8();
		u32 conn        = railNode->mConnections[i];

		// are we trying to find shortest paths in a graph here and failing
		// miserably at it or what?
		TGraphNode& otherGraphNode = getGraphNode(conn);
		if (otherGraphNode.getUnk4() == 0
		    || otherGraphNode.getUnk8() > totalPeriod) {
			otherGraphNode.incUnk4();
			otherGraphNode.setUnk8(totalPeriod);
			calcGraphDirection(conn);
		}
	}
}
#pragma dont_inline off

void TGraphWeb::initGoalIndex(const Vec& param_1)
{
	unk10 = findNearestNodeIndex(param_1, 0xffffffff);
	for (int i = 0; i < unk8; ++i)
		getGraphNode(i).unk4 = 0;
	getCurrentNode().unk4 += 1;
	getCurrentNode().unk8 = 0.0f;
	calcGraphDirection(unk10);
}

void TGraphWeb::attachToGround()
{
	for (int j = 0; j < unk8; ++j) {
		TRailNode* railNode = &unk4[j];
		if (railNode->mFlags & 0x10) {
			const TBGCheckData* checkData;
			JGeometry::TVec3<f32> pos;
			pos.set(railNode->mPosition.x, railNode->mPosition.y,
			        railNode->mPosition.z);
			pos.y = gpMap->checkGround(pos, &checkData);
			if (!checkData->checkFlag2(0x10))
				railNode->mPosition.y = pos.y;
		}
	}
}

void TGraphWeb::isOnePath() const { }

BOOL TGraphWeb::startIsEnd() const
{
	TRailNode* node = unk0->unk0;
	if (node->mConnectionNum > 2
	    || (node->mConnections[0] != unk8 - 1
	        && node->mConnections[1] != unk8 - 1))
		return false;
	if (node->mConnectionNum > 2
	    || (node->mConnections[0] != 0 && node->mConnections[1] != 0))
		return false;

	return true;
}

JGeometry::TVec3<f32> TGraphWeb::indexToPoint(int param_1) const
{
	JGeometry::TVec3<f32> result;
	const TGraphNode& graphNode = unk0[param_1];
	const TRailNode* railNode   = graphNode.getRailNode();
	result.x                    = railNode->mPosition.x;
	result.y                    = railNode->mPosition.y;
	result.z                    = railNode->mPosition.z;
	return result;
}

void TGraphWeb::perform(u32, JDrama::TGraphics*) { }

BOOL TGraphWeb::isDummy() const
{
	if (unk4 == grDummyRail)
		return true;
	return false;
}

JGeometry::TVec3<f32>
TGraphWeb::getNearestPosOnGraphLink(const JGeometry::TVec3<f32>& param_1) const
{
}

int TGraphWeb::getNeighborNodeIndexByFlag(int param_1, int param_2,
                                          u32 param_3) const
{
	int goodConnectionNum = 0;
	int goodConnections[8];

	TRailNode* railNode = unk0[param_1].getRailNode();
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		int conn = railNode->mConnections[i];
		if (conn != param_2 && (unk0[conn].unk0->mFlags & param_3)) {
			goodConnections[goodConnectionNum] = conn;
			++goodConnectionNum;
		}
	}

	if (goodConnectionNum == 0)
		return -1;

	return goodConnections[(int)(rand() * (1.0f / RAND_MAX)
	                             * goodConnectionNum)];
}

void TGraphWeb::getDesignatedNodeIndex(u32, int, f32) const { }

TGraphGroup::TGraphGroup(void* param_1)
    : unk0((GraphDesc*)param_1)
    , unk4(0)
    , unk8(nullptr)
{
	if (unk0 == nullptr) {
		unkC = new TGraphWeb(nullptr, "<nullrail>", -1);
		return;
	}

	unk4 = 0;
	while (unk0[unk4].mNodeNum != 0)
		++unk4;
	if (unk4 > 0) {
		unk8 = new TGraphWeb*[unk4];
		for (int i = 0; i < unk4; ++i) {
			unk8[i]
			    = new TGraphWeb(getNode(i), getNodeName(i), unk0[i].mNodeNum);
		}
	}

	unkC = new TGraphWeb(nullptr, "<nullrail>", -1);
}

TGraphGroup::~TGraphGroup() { }

void TGraphGroup::initGraphGroup()
{
	for (int i = 0; i < unk4; ++i) {
		if (unk8[i]->unk10 >= 0)
			continue;

		unk8[i]->initGoalIndex(JGeometry::TVec3<f32>(0.0f, 0.0f, 0.0f));
		unk8[i]->attachToGround();
	}
}

TGraphWeb* TGraphGroup::getGraphByName(const char* name)
{
	if (!name)
		return unkC;

	for (int i = 0; i < unk4; ++i)
		if (strcmp(name, getNodeName(i)) == 0)
			return unk8[i];

	return unkC;
}

void TGraphGroup::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	for (int i = 0; i < unk4; ++i)
		unk8[i]->perform(param_1, param_2);
}

TGraphTracer::TGraphTracer()
    : unk0(nullptr)
    , mCurrIdx(-1)
    , mPrevIdx(-1)
    , unkC(100.0f)
    , unk10(1.0f)
    , unk14(0.0f)
{
}

void TGraphTracer::setParamFromGraph()
{
	if (mCurrIdx >= 0) {
		unk10 = (u16)unk0->unk0[mCurrIdx].unk0->mPitch * (1.0f / 65535.0f);
	} else {
		unk10 = 0.0f;
	}

	if (mPrevIdx >= 0)
		unkC = (u16)unk0->unk0[mPrevIdx].unk0->mYaw * 0.01f;
}

void TGraphTracer::setTo(int node_idx)
{
	mPrevIdx = -1;
	mCurrIdx = node_idx;
	setParamFromGraph();
}

int TGraphTracer::moveTo(int node_idx)
{
	if (node_idx < 0)
		return node_idx;

	mPrevIdx = mCurrIdx;
	mCurrIdx = node_idx;
	setParamFromGraph();
	return node_idx;
}

f32 TGraphTracer::calcSplineSpeed(f32 param_1)
{
	if (unk0->unk14 == nullptr)
		return 0.001f;

	if (mPrevIdx < 0)
		return 0.001f;

	Vec v1;
	unk0->unk0[mCurrIdx].getPoint(&v1);
	Vec v2;
	unk0->unk0[mPrevIdx].getPoint(&v2);

	JGeometry::TVec3<f32> diff;
	diff.x     = v1.x - v2.x;
	diff.y     = v1.y - v2.y;
	diff.z     = v1.z - v2.z;
	f32 fVar13 = VECMag(&diff);

	f32 fVar1;
	f32 fVar2;
	if (unk0->unk14->unk4 && mPrevIdx == unk0->unk8 - 1 && mCurrIdx == 0) {
		fVar1 = unk0->unk14->unk0->mParametrization[0];
		fVar2 = unk0->unk14->unk0->mParametrization[1];
	} else if (unk0->unk14->unk4 && mPrevIdx == 0
	           && mCurrIdx == unk0->unk8 - 1) {
		fVar1 = unk0->unk14->unk0->mParametrization[mPrevIdx + 1];
		fVar2 = unk0->unk14->unk0->mParametrization[mPrevIdx];
	} else {
		u32 uVar10 = mPrevIdx;
		if (unk0->unk14->unk4)
			uVar10 += 1;
		fVar1     = unk0->unk14->unk0->mParametrization[uVar10];
		u32 uVar7 = mCurrIdx;
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
	if (unk0->unk14->unk4 && mPrevIdx == unk0->unk8 - 1 && mCurrIdx == 0) {
		dVar10 = unk0->unk14->unk0->mParametrization[mPrevIdx + 1];
	} else if (unk0->unk14->unk4 && mPrevIdx == 0
	           && mCurrIdx == unk0->unk8 - 1) {
		dVar10 = unk0->unk14->unk0->mParametrization[mPrevIdx];
	} else {
		u32 uVar7 = mCurrIdx;
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

	unk14 = unk0->unk14->wrapT(dVar9);
	return result;
}
