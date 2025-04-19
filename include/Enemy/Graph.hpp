#ifndef ENEMY_GRAPH_HPP
#define ENEMY_GRAPH_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

class TGraphWeb;

class TGraphNode {
public:
	TGraphNode();
	void getPoint(Vec*) const;

public:
	/* 0x0 */ S16Vec* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ s16 unkC;
};

class TRailNode {
public:
};

class TSplineRail {
public:
	TSplineRail(const TGraphWeb*);
	void wrapT(f32);
	void getNthT(int);
	void getPosition(f32);
	void getPosAndRot(f32, JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*);
};

class TGraphWeb {
public:
	/* 0x0 */ TGraphNode* unk0;
	/* 0x4 */ TRailNode* unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ const char* unkC;
	/* 0x10 */ u32 unk10;
	/* 0x14 */ TSplineRail* unk14;
	/* 0x18 */ // vt

public:
	TGraphWeb(TRailNode*, const char*, int);

	virtual ~TGraphWeb();

	void filterRailNode(u32, const TRailNode*, const TRailNode*,
	                    TRailNode*) const;
	void translateNodes(TRailNode*);
	void getAimToGoalNextIndex(int, int, u32, f32) const;
	void getShortestNextIndex(int, int, u32) const;
	void getRandomNextIndex(int, int, u32) const;
	void getEscapeFromMarioIndex(int, int, const JGeometry::TVec3<f32>&,
	                             u32) const;
	void getAimToDirNextIndex(int, int, const JGeometry::TVec3<f32>&,
	                          const JGeometry::TVec3<f32>&, u32) const;
	void getRandomButDirLimited(int, int, const JGeometry::TVec3<f32>&,
	                            const JGeometry::TVec3<f32>&, f32, u32) const;
	void getEscapeDirLimited(int, int, const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&, f32, u32) const;
	void findNearestNodeIndex(const JGeometry::TVec3<f32>&, u32) const;
	void findFarthestNodeIndex(const JGeometry::TVec3<f32>&, u32) const;
	void findNearestVisibleIndex(const JGeometry::TVec3<f32>&, f32, f32, f32,
	                             u32) const;
	void findNearestNodeIndexCheckY(const JGeometry::TVec3<f32>&, f32,
	                                u32) const;
	void getNodeIndexInXZRange(const JGeometry::TVec3<f32>&, f32, u32) const;
	void calcGraphDirection(int);
	void initGoalIndex(const Vec&);
	void attachToGround();
	void isOnePath() const;
	void startIsEnd() const;
	void indexToPoint(int) const;
	void perform(u32, JDrama::TGraphics*);
	BOOL isDummy() const;
	void getNearestPosOnGraphLink(const JGeometry::TVec3<f32>&) const;
	void getNeighborNodeIndexByFlag(int, int, u32) const;
	void getDesignatedNodeIndex(u32, int, f32) const;
};

class TGraphGroup {
public:
	TGraphGroup(void*);
	~TGraphGroup();
	void initGraphGroup();
	void getGraphByName(const char*);
	void perform(u32, JDrama::TGraphics*);
};

class TGraphTracer {
public:
	TGraphTracer();
	void setParamFromGraph();
	void setTo(int);
	void moveTo(int);
	void calcSplineSpeed(float);
	void traceSpline(float);
	void getCurGraphIndex() const;
	void getGraph() const;
};

#endif
