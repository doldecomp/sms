#ifndef ENEMY_ENEMY_HPP
#define ENEMY_ENEMY_HPP

#include <Strategic/LiveActor.hpp>
#include <Enemy/SolidStack.hpp>
#include <Enemy/PathNode.hpp>

class TGraphTracer;

class TSpineEnemy : public TLiveActor {
public:
	TSpineEnemy(const char*);
	~TSpineEnemy();
	void init(TLiveManager*);
	void reset();
	void load(JSUMemoryInputStream&);
	void calcEnemyRootMatrix();
	void calcRootMatrix();
	void getSaveParam() const;
	void resetToPosition(const JGeometry::TVec3<f32>&);
	void resetSRTV(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&);
	void calcMinimumTurnRadius(f32, f32) const;
	void calcTurnSpeedToReach(f32, f32) const;
	void updateSquareToMario();
	void receiveMessage(THitActor*, u32);
	void isInSight(const JGeometry::TVec3<f32>&, f32, f32, f32) const;
	void setGoalPathFromGraph();
	void goToInitialVisibleNode(f32, f32);
	void goToInitialGraphNodeCheckY(f32);
	void goToShortestNextGraphNode();
	void jumpToNextGraphNode();
	void goToRandomNextGraphNode();
	void goToRandomEscapeGraphNode();
	void goToExclusiveNextGraphNode();
	void goToDirectedNextGraphNode(const JGeometry::TVec3<f32>&);
	void goToDirLimitedNextGraphNode(f32);
	void updateStayCount(f32);
	void turnToCurPathNode(f32);
	void walkToCurPathNode(f32, f32, f32);
	void zigzagToCurPathNode(f32, f32, f32, f32);
	void doShortCut();
	void searchNearestBrother() const;
	void getCurAnmFrameNo(int) const;
	void checkCurAnmEnd(int) const;
	void perform(u32, JDrama::TGraphics*);
	void isReachedToGoal() const;
	void getPhaseShift() const;

public:
	/* 0xF4 */ u32 unkF4;
	/* 0xF8 */ Vec unkF8;
	/* 0x104 */ u32 unk104;
	/* 0x108 */ Vec unk108;
	/* 0x114 */ TSolidStack<TPathNode> unk114;
	/* 0x124 */ TGraphTracer* unk124;
	/* 0x128 */ u16 unk128;
	/* 0x12C */ f32 unk12C;
	/* 0x130 */ u8 unk130;
	/* 0x134 */ f32 unk134;
	/* 0x138 */ u32 unk138;
	/* 0x13C */ u8 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
};

#endif
