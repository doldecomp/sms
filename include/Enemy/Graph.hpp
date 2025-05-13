#ifndef ENEMY_GRAPH_HPP
#define ENEMY_GRAPH_HPP

#include <JSystem/JGeometry/JGVec3.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

class TGraphWeb;
class TSplinePath;

struct TRailNode {
	u32 getFlags() const { return mFlags; }

	/* 0x0 */ S16Vec mPosition;
	/* 0x6 */ s16 mConnectionNum;
	/* 0x8 */ u32 mFlags;
	/* 0xC */ u16 mPitch;
	/* 0xE */ u16 mYaw;
	/* 0x10 */ u16 mRoll;
	/* 0x12 */ s16 mSpeed;
	/* 0x14 */ u16 mConnections[8];
	/* 0x24 */ f32 mPeriods[8];
};

class TGraphNode {
public:
	TGraphNode();
	void getPoint(Vec*) const;

	// fabricated
	TRailNode* getRailNode() { return unk0; }
	const TRailNode* getRailNode() const { return unk0; }
	f32 getUnk8() { return unk8; }
	void setUnk8(f32 v) { unk8 = v; }
	void incUnk4() { ++unk4; }
	int getUnk4() { return unk4; }

public:
	/* 0x0 */ TRailNode* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ s16 unkC;
};

class TSplineRail {
public:
	TSplineRail(const TGraphWeb*);
	f32 wrapT(f32);
	f32 getNthT(int);
	JGeometry::TVec3<f32> getPosition(f32);
	void getPosAndRot(f32, JGeometry::TVec3<f32>*, JGeometry::TVec3<f32>*);

public:
	/* 0x4 */ BOOL unk4;
	/* 0x0 */ TSplinePath* unk0;
};

class TGraphWeb {
public:
	/* 0x0 */ TGraphNode* unk0;
	/* 0x4 */ TRailNode* unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ const char* unkC;
	/* 0x10 */ int unk10;
	/* 0x14 */ TSplineRail* unk14;
	/* 0x18 */ // vt

public:
	TGraphWeb(TRailNode*, const char*, int);

	virtual ~TGraphWeb();

	int filterRailNode(u32, const TRailNode*, const TRailNode*,
	                   TRailNode*) const;
	void translateNodes(TRailNode*);
	void getAimToGoalNextIndex(int, int, u32, f32) const;
	int getShortestNextIndex(int, int, u32) const;
	int getRandomNextIndex(int, int, u32) const;
	int getEscapeFromMarioIndex(int, int, const JGeometry::TVec3<f32>&,
	                            u32) const;
	int getAimToDirNextIndex(int, int, const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&, u32) const;
	void getRandomButDirLimited(int, int, const JGeometry::TVec3<f32>&,
	                            const JGeometry::TVec3<f32>&, f32, u32) const;
	void getEscapeDirLimited(int, int, const JGeometry::TVec3<f32>&,
	                         const JGeometry::TVec3<f32>&, f32, u32) const;
	int findNearestNodeIndex(const JGeometry::TVec3<f32>&, u32) const;
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
	BOOL startIsEnd() const;
	JGeometry::TVec3<f32> indexToPoint(int) const;
	void perform(u32, JDrama::TGraphics*);
	BOOL isDummy() const;
	JGeometry::TVec3<f32>
	getNearestPosOnGraphLink(const JGeometry::TVec3<f32>&) const;
	int getNeighborNodeIndexByFlag(int, int, u32) const;
	void getDesignatedNodeIndex(u32, int, f32) const;

	// fabricated
	TGraphNode& getGraphNode(int i) { return unk0[i]; }
	TGraphNode& getCurrentNode() { return unk0[unk10]; }
	TSplineRail* getSplineRail() { return unk14; }
};

class TGraphGroup {
public:
	TGraphGroup(void*);
	~TGraphGroup();
	void initGraphGroup();
	TGraphWeb* getGraphByName(const char*);
	void perform(u32, JDrama::TGraphics*);

	TRailNode* getNode(int i)
	{
		return (TRailNode*)((u8*)unk0 + unk0[i].mRailNodesOffset);
	}
	const char* getNodeName(int i)
	{
		return (const char*)((u8*)unk0 + unk0[i].mNameOffset);
	}

	// fabricated
	struct GraphDesc {
		/* 0x0 */ int mNodeNum;
		/* 0x4 */ u32 mNameOffset;
		/* 0x8 */ u32 mRailNodesOffset;
	};

public:
	/* 0x0 */ GraphDesc* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ TGraphWeb** unk8;
	/* 0xC */ TGraphWeb* unkC;
};

class TGraphTracer {
public:
	TGraphTracer();
	void setParamFromGraph();
	void setTo(int node_idx);
	int moveTo(int node_idx);
	f32 calcSplineSpeed(float);
	bool traceSpline(float);
	void getCurGraphIndex() const;
	void getGraph() const;

	// fabricated
	int getCurrentIndex() { return mCurrIdx; }
	TGraphNode& getCurrent() { return unk0->getGraphNode(mCurrIdx); }
	int getPrevIndex() { return mPrevIdx; }
	JGeometry::TVec3<f32> getCurrentPos()
	{
		return unk0->indexToPoint(mCurrIdx);
	}
	void moveToShortestNext()
	{
		moveTo(unk0->getShortestNextIndex(getCurrentIndex(), getPrevIndex(),
		                                  0xffffffff));
	}
	void setToNearest(const JGeometry::TVec3<f32>& pos)
	{
		setTo(unk0->findNearestNodeIndex(pos, 0xffffffff));
	}

public:
	/* 0x0 */ TGraphWeb* unk0;
	/* 0x4 */ int mCurrIdx;
	/* 0x8 */ int mPrevIdx;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
};

#endif
