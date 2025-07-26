#include <Enemy/Graph.hpp>
#include <Enemy/Spline.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <Player/MarioAccess.hpp>
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

TSplineRail::TSplineRail(const TGraphWeb* graph)
    : unk0(nullptr)
    , unk4(0)
{
	if (graph->startIsEnd()) {
		unk4 = 1;
		unk0 = new TSplinePath(graph->getNodeNum() + 3);

		unk0->setPoint(0, graph->getLastGraphNode().getPoint());

		for (int i = 0; i < graph->getNodeNum(); ++i)
			unk0->setPoint(i + 1, graph->getGraphNode(i).getPoint());

		int lastest = graph->getNodeNum() + 1;
		unk0->setPoint(lastest, graph->getGraphNode(0).getPoint());

		unk0->setPoint(graph->getNodeNum() + 2,
		               graph->getGraphNode(1).getPoint());
	} else {
		unk0 = new TSplinePath(graph->getNodeNum());

		for (int i = 0; i < graph->getNodeNum(); ++i)
			unk0->setPoint(i, graph->getGraphNode(i).getPoint());
	}

	unk0->getPoint(0.0f);

	char trash[0x50]; // TODO: skill issue, need more inlines
}

f32 TSplineRail::wrapT(f32 param_1)
{
	if (unk4)
		param_1 = MsWrap<f32>(param_1, unk0->getNthT(1),
		                      unk0->getNthT(unk0->getPointNum() - 2));

	return param_1;
}

// TODO: you'd think this is correct, but the size is wrong...
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

	char trash2[0xC];

	for (;;) {
		if (t + dt > 1.0f)
			t = 1.0f - dt;

		point = getPosition(t);
		dir   = getPosition(t + dt);

		dir -= point;

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

	if (param_3 <= 0) {
		int i = 0;
		while (param_1[i].mConnectionNum > 0)
			++i;
		unk8 = i;
	} else {
		unk8 = param_3;
	}

	unk0 = new TGraphNode[unk8];

	for (int i = 0; i < unk8; ++i)
		getGraphNode(i).unk0 = &param_1[i];

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
		TRailNode& nodeRef = param_1[i];

		if (nodeRef.mFlags & 0x8) {
			TRailNode node = nodeRef;
			int conns      = 0;
			for (int j = 0; j < param_1[i].mConnectionNum; ++j) {
				if (!(param_1[param_1[i].mConnections[j]].mFlags & 0x4)) {
					node.mConnections[conns] = param_1[i].mConnections[j];
					node.mPeriods[conns]     = param_1[i].mPeriods[j];
					++conns;
				}
			}
			node.mConnectionNum = conns;
			nodeRef             = node;
		}

		if (nodeRef.mFlags & 0x200) {
			TRailNode node = nodeRef;
			int conns      = 0;
			for (int j = 0; j < param_1[i].mConnectionNum; ++j) {
				if (param_1[i].mConnections[j] >= i) {
					node.mConnections[conns] = param_1[i].mConnections[j];
					node.mPeriods[conns]     = param_1[i].mPeriods[j];
					++conns;
				}
			}
			node.mConnectionNum = conns;
			nodeRef             = node;
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

static inline f32 randf() { return rand() * (1.f / (RAND_MAX + 1)); }

int TGraphWeb::getRandomNextIndex(int param_1, int param_2, u32 param_3) const
{
	const TGraphNode* graphNode = &getGraphNode(param_1);

	TRailNode tmp;
	const TRailNode* railNode;
	if (param_3 == 0xffffffff) {
		railNode = graphNode->getRailNode();
	} else {
		filterRailNode(param_3, graphNode->getRailNode(), unk4, &tmp);
		railNode = &tmp;
	}

	s16 num = railNode->mConnectionNum;
	if (num == 0)
		return param_1;

	if (num == 1)
		return railNode->mConnections[0];

	if (num == 2 && param_2 >= 0) {
		if (railNode->mConnections[0] != param_2)
			return railNode->mConnections[0];
		else
			return railNode->mConnections[1];
	}

	int rnd = randf() * num;

	int result = rnd;

	if (railNode->mConnections[rnd] == param_2) {
		if (rnd == railNode->mConnectionNum - 1)
			result = 0;
		else
			result = railNode->mConnectionNum - 1;
	}

	return railNode->mConnections[result];
}

int TGraphWeb::getEscapeFromMarioIndex(int param_1, int param_2,
                                       const JGeometry::TVec3<f32>& param_3,
                                       u32 param_4) const
{
	const TGraphNode& node = getGraphNode(param_1);
	TRailNode fakeNode;
	const TRailNode* railNode;
	if (param_4 == -1) {
		railNode = node.getRailNode();
	} else {
		filterRailNode(param_4, node.getRailNode(), unk4, &fakeNode);
		railNode = &fakeNode;
	}

	if (!railNode->mConnectionNum)
		return param_1;

	if (railNode->mConnectionNum == 1)
		return railNode->mConnections[0];

	if (railNode->mConnectionNum == 2 && param_2 >= 0) {
		if (railNode->mConnections[0] != param_2)
			return railNode->mConnections[0];
		else
			return railNode->mConnections[1];
	}

	JGeometry::TVec3<f32> local_a0(gpMarioPos->x, gpMarioPos->y, gpMarioPos->z);
	local_a0 -= param_3;
	MsVECNormalize(&local_a0, &local_a0);

	int result = -1;
	f32 fVar1  = 0.0f;
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> local_ac;
		getGraphNode(railNode->mConnections[i]).getPoint(local_ac);
		local_ac -= param_3;
		MsVECNormalize(&local_ac, &local_ac);

		f32 fVar3 = local_ac.dot(local_a0);
		if (result < 0 || fVar3 < fVar1) {
			fVar1  = fVar3;
			result = railNode->mConnections[i];
		}
	}

	return result;
}

int TGraphWeb::getAimToDirNextIndex(int param_1, int param_2,
                                    const JGeometry::TVec3<f32>& param_3,
                                    const JGeometry::TVec3<f32>& param_4,
                                    u32 param_5) const
{
	const TGraphNode& graphNode = getGraphNode(param_1);

	TRailNode tmp;
	const TRailNode* railNode;
	if (param_5 == 0xffffffff) {
		railNode = graphNode.getRailNode();
	} else {
		filterRailNode(param_5, graphNode.getRailNode(), unk4, &tmp);
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

int TGraphWeb::getRandomButDirLimited(int param_1, int param_2,
                                      const JGeometry::TVec3<f32>& param_3,
                                      const JGeometry::TVec3<f32>& param_4,
                                      f32 param_5, u32 param_6) const
{
	const TGraphNode& graphNode = getGraphNode(param_1);

	TRailNode tmp;
	const TRailNode* railNode;
	if (param_6 == 0xffffffff) {
		railNode = graphNode.getRailNode();
	} else {
		filterRailNode(param_6, graphNode.getRailNode(), unk4, &tmp);
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

	JGeometry::TVec3<f32> local_cc = param_3;
	MsVECNormalize(&local_cc, &local_cc);

	int result;
	int iVar13 = 0;
	result     = -1;

	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> local_d8;
		getGraphNode(railNode->mConnections[i]).getPoint(&local_d8);
		local_d8 -= param_4;
		MsVECNormalize(&local_d8, &local_d8);

		JGeometry::TVec3<f32> local_f4;
		local_f4.cross(local_cc, local_d8);
		f32 angle = abs(matan(local_cc.dot(local_d8), MsVECMag2(&local_f4))
		                * (360.0f / 65536.0f));
		if (abs(angle) < param_5) {
			if (iVar13 == 0) {
				result = railNode->mConnections[i];
			} else {
				if (randf() < 1.0f / (iVar13 + 1))
					result = railNode->mConnections[i];
			}
			++iVar13;
		}
	}

	if (result > 0)
		return result;

	result     = -1;
	f32 maxCos = 0.0f;
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> local_e4;
		unk0[railNode->mConnections[i]].getPoint(&local_e4);
		local_e4.sub(param_4);
		MsVECNormalize(&local_e4, &local_e4);

		f32 cos = local_e4.dot(local_cc);
		if (result < 0 || cos > maxCos) {
			maxCos = cos;
			result = railNode->mConnections[i];
		}
	}

	return result;
}

int TGraphWeb::getEscapeDirLimited(int param_1, int param_2,
                                   const JGeometry::TVec3<f32>& param_3,
                                   const JGeometry::TVec3<f32>& param_4,
                                   f32 param_5, u32 param_6) const
{
	const TGraphNode& graphNode = getGraphNode(param_1);

	TRailNode tmp;
	const TRailNode* railNode;
	if (param_6 == 0xffffffff) {
		railNode = graphNode.getRailNode();
	} else {
		filterRailNode(param_6, graphNode.getRailNode(), unk4, &tmp);
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

	JGeometry::TVec3<f32> local_c0 = param_3;
	JGeometry::TVec3<f32> local_cc = SMS_GetMarioPos();
	MsVECNormalize(&local_c0, &local_c0);
	local_cc -= param_4;
	MsVECNormalize(&local_cc, &local_cc);

	int result;
	result = -1;
	f32 unaff_f29;

	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> local_d8;
		getGraphNode(railNode->mConnections[i]).getPoint(&local_d8);
		local_d8 -= param_4;
		MsVECNormalize(&local_d8, &local_d8);

		JGeometry::TVec3<f32> local_f4;
		local_f4.cross(local_c0, local_d8);
		f32 angle = abs(matan(local_c0.dot(local_d8), MsVECMag2(&local_f4))
		                * (360.0f / 65536.0f));
		if (abs(angle) < param_5) {
			if (result < 0) {
				result    = railNode->mConnections[i];
				unaff_f29 = local_d8.dot(local_cc);
			} else {
				f32 cos = local_d8.dot(local_cc);
				if (cos < unaff_f29) {
					result    = railNode->mConnections[i];
					unaff_f29 = cos;
				}
			}
		}
	}

	if (result >= 0)
		return result;

	result = -1;
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		if (param_2 == railNode->mConnections[i])
			continue;

		JGeometry::TVec3<f32> local_e4;
		unk0[railNode->mConnections[i]].getPoint(&local_e4);
		local_e4.sub(param_4);
		MsVECNormalize(&local_e4, &local_e4);

		f32 cos = local_e4.dot(local_c0);
		if (result < 0 || cos < unaff_f29) {
			unaff_f29 = cos;
			result    = railNode->mConnections[i];
		}
	}

	return result;
}

int TGraphWeb::findNearestNodeIndex(const JGeometry::TVec3<f32>& param_1,
                                    u32 param_2) const
{
	int i;
	int result  = -1;
	f32 minDist = -1.0f;
	for (i = 0; i < unk8; ++i) {
		if (param_2 != 0xffffffff && getGraphNode(i).checkFlag(param_2))
			continue;

		JGeometry::TVec3<f32> pos;
		getGraphNode(i).getPoint(&pos);
		pos -= param_1;
		f32 dist = pos.squared();
		if (minDist < 0.0f || dist < minDist) {
			minDist = dist;
			result  = i;
		}
	}
	return result;
}

int TGraphWeb::findFarthestNodeIndex(const JGeometry::TVec3<f32>&, u32) const {
}

int TGraphWeb::findNearestVisibleIndex(const JGeometry::TVec3<f32>& param_1,
                                       f32 param_2, f32 param_3, f32 param_4,
                                       u32 param_5) const
{
	int i;
	int result;

	f32 minDist = -1.0f;
	result      = -1;
	for (i = 0; i < unk8; ++i) {
		if (param_5 != 0xffffffff && getGraphNode(i).checkFlag(param_5))
			continue;
		JGeometry::TVec3<f32> point;
		getGraphNode(i).getPoint(&point);
		if (MsIsInSight(param_1, param_2, point, param_3, param_4, 0.0f)) {
			point -= param_1;
			f32 len = point.squared();
			if (minDist < 0.0f || len < minDist) {
				minDist = len;
				result  = i;
			}
		}
	}

	return result;
}

int TGraphWeb::findNearestNodeIndexCheckY(const JGeometry::TVec3<f32>&, f32,
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
	if (getFirstGraphNode().unk0->mConnectionNum > 2
	    || (getFirstGraphNode().unk0->mConnections[0] != getNodeNum() - 1
	        && getFirstGraphNode().unk0->mConnections[1] != getNodeNum() - 1))
		return false;

	if (getLastGraphNode().unk0->mConnectionNum > 2
	    || (getLastGraphNode().unk0->mConnections[0] != 0
	        && getLastGraphNode().unk0->mConnections[1] != 0))
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
	bool bVar9 = true;

	JGeometry::TVec3<f32> local_48;
	f32 min;
	for (int i = 0; i < unk8; ++i) {
		const TGraphNode& node = getGraphNode(i);
		JGeometry::TVec3<f32> point;
		node.getPoint(&point);
		const TRailNode* railNode = node.getRailNode();
		for (int i = 0; i < railNode->mConnectionNum; ++i) {
			int conn               = railNode->mConnections[i];
			const TGraphNode& node = getGraphNode(conn);
			JGeometry::TVec3<f32> point2;
			node.getPoint(&point2);
			point2 -= point;

			f32 fVar4 = MsClamp((param_1.dot(point2) - point.dot(point2))
			                        / point2.squared(),
			                    0.0f, 1.0f);
			JGeometry::TVec3<f32> thing;
			thing.scaleAdd(fVar4, point2, point);
			thing.sub(param_1);
			f32 dVar18 = thing.squared();
			if (bVar9 || dVar18 < min) {
				local_48.scaleAdd(fVar4, point2, point);
				bVar9 = false;
				min   = dVar18;
			}
		}
	}

	return local_48;
}

int TGraphWeb::getNeighborNodeIndexByFlag(int param_1, int param_2,
                                          u32 param_3) const
{
	int goodConnectionNum = 0;
	int goodConnections[8];

	const TRailNode* railNode = getGraphNode(param_1).getRailNode();
	for (int i = 0; i < railNode->mConnectionNum; ++i) {
		int conn = railNode->mConnections[i];
		if (conn != param_2 && getGraphNode(conn).checkFlag(param_3)) {
			goodConnections[goodConnectionNum] = conn;
			++goodConnectionNum;
		}
	}

	if (goodConnectionNum == 0)
		return -1;

	return goodConnections[(int)(randf() * goodConnectionNum)];
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

	JGeometry::TVec3<f32> v1;
	unk0->unk0[mCurrIdx].getPoint(&v1);
	JGeometry::TVec3<f32> v2;
	unk0->unk0[mPrevIdx].getPoint(&v2);

	JGeometry::TVec3<f32> diff = v1;
	diff -= v2;
	f32 fVar13 = VECMag(&diff);

	f32 fVar1;
	f32 fVar2;
	if (unk0->getSplineRail()->isUnk4() && mPrevIdx == unk0->unk8 - 1
	    && mCurrIdx == 0) {
		fVar1 = unk0->getSplineRail()->getNthT(0);
		fVar2 = unk0->getSplineRail()->getNthT(1);
	} else if (unk0->getSplineRail()->isUnk4() && mPrevIdx == 0
	           && mCurrIdx == unk0->unk8 - 1) {
		fVar1 = unk0->getSplineRail()->getNthT(mPrevIdx + 1);
		fVar2 = unk0->getSplineRail()->getNthT(mPrevIdx);
	} else {
		u32 uVar10 = mPrevIdx;
		if (unk0->getSplineRail()->isUnk4())
			uVar10 += 1;
		fVar1 = unk0->getSplineRail()->getNthT(uVar10);

		u32 uVar7 = mCurrIdx;
		if (unk0->getSplineRail()->isUnk4())
			uVar7 += 1;
		fVar2 = unk0->getSplineRail()->getNthT(uVar7);
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
		dVar10 = unk0->unk14->getNthT(mPrevIdx + 1);
	} else if (unk0->unk14->unk4 && mPrevIdx == 0
	           && mCurrIdx == unk0->unk8 - 1) {
		dVar10 = unk0->unk14->getNthT(mPrevIdx);
	} else {
		u32 uVar7 = mCurrIdx;
		if (unk0->unk14->unk4)
			uVar7 += 1;
		dVar10 = unk0->unk14->getNthT(uVar7);
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
