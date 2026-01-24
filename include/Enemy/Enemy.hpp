#ifndef ENEMY_ENEMY_HPP
#define ENEMY_ENEMY_HPP

#include <Strategic/LiveActor.hpp>
#include <Strategic/SolidStack.hpp>
#include <Enemy/PathNode.hpp>

class TGraphTracer;
class TSpineEnemyParams;
class TEnemyManager;
extern size_t gpMarioAddress;

// TODO: this definitely has a better place to live
// I took it from walkerEnemy.cpp
static inline f32 vecdist(const JGeometry::TVec3<f32>& a,
                          const JGeometry::TVec3<f32>& b)
{
	JGeometry::TVec3<f32> tmp = a;
	tmp.sub(b);
	return tmp.length();
}

class TSpineEnemy : public TLiveActor {
public:
	TSpineEnemy(const char*);
	~TSpineEnemy();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void reset();
	virtual void resetToPosition(const JGeometry::TVec3<f32>& position);
	virtual void resetSRTV(const JGeometry::TVec3<f32>& position,
	                       const JGeometry::TVec3<f32>& rotation,
	                       const JGeometry::TVec3<f32>& scaling,
	                       const JGeometry::TVec3<f32>& velocity);
	virtual TSpineEnemyParams* getSaveParam() const;
	virtual f32 getPhaseShift() const { return 0.0f; }
	virtual BOOL isReachedToGoal() const { }

	void calcEnemyRootMatrix();
	f32 calcMinimumTurnRadius(f32, f32) const;
	f32 calcTurnSpeedToReach(f32, f32) const;
	void updateSquareToMario();
	BOOL isInSight(const JGeometry::TVec3<f32>& pos, f32 length, f32 angle,
	               f32 aware) const;
	void setGoalPathFromGraph();
	void goToInitialVisibleNode(f32, f32);
	void goToInitialGraphNodeCheckY(f32);
	int goToShortestNextGraphNode();
	int jumpToNextGraphNode();
	void goToRandomNextGraphNode();
	void goToRandomEscapeGraphNode();
	void goToExclusiveNextGraphNode();
	void goToDirectedNextGraphNode(const JGeometry::TVec3<f32>&);
	void goToDirLimitedNextGraphNode(f32);
	void updateStayCount(f32);
	bool turnToCurPathNode(f32);
	void walkToCurPathNode(f32 march_speed, f32 turn_speed, f32);
	void zigzagToCurPathNode(f32 march_speed, f32 turn_speed, f32 cycle,
	                         f32 angle);
	void doShortCut();
	void searchNearestBrother() const;
	f32 getCurAnmFrameNo(int) const;
	BOOL checkCurAnmEnd(int) const;

	// fabricated
	f32 getDistToMarioSquared() const { return mDistToMarioSquared; }
	f32 getMarchSpeed() const { return mMarchSpeed; }
	f32 getTurnSpeed() const { return mTurnSpeed; }
	u8 getHitPoints() const { return mHitPoints; }
	f32 getHeadHeight() const { return mBodyScale * mHeadHeight; }
	f32 getWallRadius() const { return mBodyScale * mWallRadius; }
	f32 getBodyRadius() const { return mBodyScale * mBodyRadius; }
	f32 getBodyScale() const { return mBodyScale; }

	// fabricated
	void setGoalPathMario()
	{
		TPathNode node((THitActor*)gpMarioAddress);

		// the hell
		if (gpMarioAddress) {
			node.unk4.set(*(f32*)(gpMarioAddress + 0x10),
			              *(f32*)(gpMarioAddress + 0x14),
			              *(f32*)(gpMarioAddress + 0x18));
		}

		unkF4  = node;
		unk104 = node;

		unk114.clear();
	}
	void setGoalPath(const TPathNode& point)
	{
		unkF4  = point;
		unk104 = point;

		unk114.clear();
	}
	void switchNextGoalPath()
	{
		if (!unk114.empty())
			unkF4 = unk114.pop();
	}

	void decHitPoints()
	{
		if (mHitPoints > 0)
			mHitPoints -= 1;
	}

	const TPathNode& getUnkF4() const { return unkF4; }
	const TPathNode& getUnk104() const { return unk104; }
	TGraphTracer* getTracer() { return unk124; }

public:
	/* 0xF4 */ TPathNode unkF4;
	/* 0x104 */ TPathNode unk104;
	/* 0x114 */ TSolidStack<TPathNode> unk114;
	/* 0x124 */ TGraphTracer* unk124;
	/* 0x128 */ s16 unk128;
	/* 0x12C */ f32 unk12C;
	/* 0x130 */ s8 unk130;
	/* 0x134 */ f32 mDistToMarioSquared;
	/* 0x138 */ const TBGCheckData* unk138; // TODO: type is a wild guess
	/* 0x13C */ u8 mHitPoints;
	/* 0x140 */ f32 mMarchSpeed;
	/* 0x144 */ f32 mTurnSpeed;
	/* 0x148 */ f32 mBodyScale;
	/* 0x14C */ f32 mWallRadius;
};

#endif
