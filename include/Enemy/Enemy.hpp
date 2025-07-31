#ifndef ENEMY_ENEMY_HPP
#define ENEMY_ENEMY_HPP

#include <Strategic/LiveActor.hpp>
#include <Strategic/SolidStack.hpp>
#include <Enemy/PathNode.hpp>

class TGraphTracer;
class TSpineEnemyParams;
extern size_t gpMarioAddress;

class TSpineEnemy : public TLiveActor {
public:
	TSpineEnemy(const char*);
	~TSpineEnemy();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void reset();
	virtual void resetToPosition(const JGeometry::TVec3<f32>&);
	virtual void resetSRTV(const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&,
	                       const JGeometry::TVec3<f32>&);
	virtual TSpineEnemyParams* getSaveParam() const;
	virtual void getPhaseShift() const;
	virtual BOOL isReachedToGoal() const { }

	void calcEnemyRootMatrix();
	f32 calcMinimumTurnRadius(f32, f32) const;
	void calcTurnSpeedToReach(f32, f32) const;
	void updateSquareToMario();
	BOOL isInSight(const JGeometry::TVec3<f32>& pos, f32 length, f32 angle,
	               f32 aware) const;
	void setGoalPathFromGraph();
	void goToInitialVisibleNode(f32, f32);
	void goToInitialGraphNodeCheckY(f32);
	void goToShortestNextGraphNode();
	int jumpToNextGraphNode();
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
	f32 getCurAnmFrameNo(int) const;
	BOOL checkCurAnmEnd(int) const;

	// fabricated
	f32 getUnk134() const { return unk134; }
	f32 getMarchSpeed() const { return mMarchSpeed; }
	f32 getTurnSpeed() const { return mTurnSpeed; }
	u8 getHitPoints() const { return mHitPoints; }
	f32 getSomething() const { return mBodyScale * unkC0; }

	// fabricated
	void setGoalPathMario()
	{
		// what
		volatile size_t mario = gpMarioAddress;
		JGeometry::TVec3<f32> marioPos(0.0f, 0.0f, 0.0f);

		// the hell
		if (gpMarioAddress) {
			marioPos.set(*(f32*)(gpMarioAddress + 0x10),
			             *(f32*)(gpMarioAddress + 0x14),
			             *(f32*)(gpMarioAddress + 0x18));
		}

		// is happening here
		unkF4  = (THitActor*)mario;
		unkF8  = marioPos;
		unk104 = (THitActor*)mario;
		unk108 = marioPos;

		unk114.clear();
	}
	f32 getBodyScale() const { return mBodyScale; }
	void decHitPoints()
	{
		if (mHitPoints > 0)
			mHitPoints -= 1;
	}

public:
	/* 0xF4 */ THitActor* unkF4; // TODO: type is a wild guess
	/* 0xF8 */ JGeometry::TVec3<f32> unkF8;

	// TODO: these two are a struct according to TNerveHino2JumpIn::execute
	/* 0x104 */ THitActor* unk104; // TODO: type is a wild guess
	/* 0x108 */ JGeometry::TVec3<f32> unk108;

	/* 0x114 */ TSolidStack<TPathNode> unk114;
	/* 0x124 */ TGraphTracer* unk124;
	/* 0x128 */ u16 unk128;
	/* 0x12C */ f32 unk12C;
	/* 0x130 */ s8 unk130;
	/* 0x134 */ f32 unk134;
	/* 0x138 */ const TBGCheckData* unk138; // TODO: type is a wild guess
	/* 0x13C */ u8 mHitPoints;
	/* 0x140 */ f32 mMarchSpeed;
	/* 0x144 */ f32 mTurnSpeed;
	/* 0x148 */ f32 mBodyScale;
	/* 0x14C */ f32 unk14C;
};

#endif
