#ifndef ENEMY_ENEMY_HPP
#define ENEMY_ENEMY_HPP

#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <Strategic/LiveActor.hpp>
#include <Strategic/SolidStack.hpp>
#include <Enemy/PathNode.hpp>

class TGraphTracer;
class TSpineEnemyParams;
class TEnemyManager;
extern void* gpMarioAddress;

class TSpineEnemyParams : public TParams {
public:
	TSpineEnemyParams(const char*);

	/* 0x8 */ TParamRT<f32> mSLHeadHeight;
	/* 0x1C */ TParamRT<f32> mSLBodyRadius;
	/* 0x30 */ TParamRT<f32> mSLWallRadius;
	/* 0x44 */ TParamRT<f32> mSLClipRadius;
	/* 0x58 */ TParamRT<f32> mSLFarClip;
	/* 0x6C */ TParamRT<u8> mSLHitPointMax;
	/* 0x80 */ TParamRT<u8> mSLInstanceNum;
	/* 0x94 */ TParamRT<u8> mSLActiveEnemyNum;
};

class TSpineEnemy : public TLiveActor {
public:
	TSpineEnemy(const char*);
	~TSpineEnemy();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
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
	// The copy-and-subtract distance helper. The map has no symbol for it,
	// so retail had it as an inline that expanded everywhere; it is kept in
	// class here because the only site is an in-class virtual. It is the
	// level that reaches the map's out-of-line `TVec3<f32>::sub`, `::dot`
	// and `TUtil<f32>::sqrt` at a copy-and-subtract distance test, which
	// `TVec3::distance()` cannot do (`isReachedToGoal` 67.46 -> 99.74,
	// TFishoid's weak copy 98.79 -> 99.93 in header round 22; writing the
	// three statements into `isReachedToGoal` itself instead loses the level
	// and scores 60.80). The same body is parked TU-locally as
	// AnimalNerve.cpp's `calcDist`, emario's `EMarioCalcDist`,
	// elecNokonoko's `ElecDistTo` and TabePuku's `TabePukuLength`; see the
	// note on `TVec3::distance()` in JGVec3.hpp for why that member cannot
	// carry it.
	static f32 calcDist(JGeometry::TVec3<f32> a,
	                    const JGeometry::TVec3<f32>& b)
	{
		a.sub(b);
		return JGeometry::TUtil<f32>::sqrt(a.squared());
	}

	// The named goal reference is retail's: without it the by-value copy of
	// `a` lands 4 bytes high (0x24 instead of 0x20 in an otherwise exact 0x30
	// frame) -- a consumed reference binding is 4 bytes of low region, which
	// is exactly what frame-gaps.md batch 142's two-region ladder predicts,
	// and it is the spelling AnimalNerve.cpp's two `calcDist` sites already
	// use for the same body (`const TVec3<f32>& goalPos = ...getPoint();`).
	// Header round 26: fishoid's weak copy 99.74 -> 100. Measured as +0 or
	// worse: a named `f32 dist` or `f32 length`/`f32 sq` result (+8, the copy
	// stays high), an explicit `TVec3<f32> d = a;` with both parameters by
	// reference (+8 more), `b` by pointer, the by-value parameter declared
	// second, `a.dot(a)` for `a.squared()`, a non-static `calcDist(TVec3)`
	// against `mPosition`, and `100.0f > calcDist(...)`.
	virtual BOOL isReachedToGoal() const
	{
		const JGeometry::TVec3<f32>& goal = unk104.getPoint();
		return calcDist(goal, mPosition) < 100.0f ? TRUE : FALSE;
	}

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
	BOOL turnToCurPathNode(f32);
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
	// u8, not int: TStayPakkun::setBehavior divides by it with divwu,
	// PakkunRootCallback compares against it after a clrlwi and
	// TEffectEnemy::perform divides by it, all of which need the narrowed
	// return type -- an `int` return costs those three their exact match
	// (effectEnemy unlinks) for the same bosstelesa gain this spelling gets
	// for free (header round 38).
	// The constant is a plain `1`, not `(u8)1`: that makes the ternary an
	// int expression which MWCC narrows once on the way out, and that single
	// post-merge `clrlwi` is what the sites reading the result into a `u8`
	// local want -- TNerveBossTelesaFreeze::execute 98.5 -> 99.7,
	// TNerveBossTelesaHideWait +1.1, TNerveBossTelesaPrepareSlot +0.3, with
	// TFireWanwan::receiveMessage, TStayPakkun::setBehavior,
	// PakkunRootCallback and TEffectEnemy::perform all still exact.
	// Priced: TFireWanwan::moveObject 76.2 -> 75.6 and TBossManta::init
	// 99.07 -> 99.01, both already nonmatching, both wanting the narrowing
	// one statement later than we put it.
	u8 getMaxHitPoints() const
	{
		return getSaveParam() ? getSaveParam()->mSLHitPointMax.get() : 1;
	}

	// fabricated TODO: remove
	// The Mario pointer is bound into a named local rather than cast inside
	// the argument: that pointer binding is worth 4 bytes of low region (see
	// frame-gaps.md, batch 142's return-type table) and is what seven callers
	// tree-wide want -- TGesso::behaveToFindMario, TNerveHaneHamuKuriUpWait,
	// TPakkun::load, TNervePakkunShoot, TNerveFireWanwanAttack,
	// TNerveBombHeiAttack and TNervePopoWait all go byte-exact with it and 4
	// short without it (header round 28).
	void setGoalPathMario()
	{
		THitActor* mario = (THitActor*)gpMarioAddress;
		setGoalPath(mario);
	}
	void setGoalPath(const TPathNode& point)
	{
		unkF4  = point;
		unk104 = point;

		unk114.clear();
	}
	// The popped node is bound to a const reference, not assigned straight
	// out of the call: that consumed reference binding is 4 bytes of low
	// region (frame-gaps.md, header round 26), which is what
	// TNerveBPFlyPivot::execute wants (99.77 -> 100). The by-value form
	// (`TPathNode next = unk114.pop();`) is far worse -- it adds the whole
	// 16-byte copy and costs five functions including this one's beneficiary
	// (BPFlyPivot 99.77 -> 81.16, TNerveWalkerEscape 99.86 -> 90.77) -- and
	// the unbound form leaves BPFlyPivot 4 short. Header round 29.
	void switchNextGoalPath()
	{
		if (!unk114.empty()) {
			const TPathNode& next = unk114.pop();
			unkF4                 = next;
		}
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
