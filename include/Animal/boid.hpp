#ifndef ANIMAL_BOID_HPP
#define ANIMAL_BOID_HPP

#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>

class TBoid {
public:
	TBoid();

public:
	/* 0x00 */ JGeometry::TVec3<f32> mPosition;
	/* 0x0C */ JGeometry::TVec3<f32> mRotation;
	/* 0x18 */ JGeometry::TVec3<f32> mHeading;
	/* 0x24 */ JGeometry::TVec3<f32> mSeparationForce;
	/* 0x30 */ JGeometry::TVec3<f32> mAlignmentForce;
	/* 0x3C */ JGeometry::TVec3<f32> mCohesionForce;
	/* 0x48 */ s32 mNeighbors;
	/* 0x4C */ f32 mSpeed;
};

class TBoidLeader : public JDrama::TViewObj {
public:
	TBoidLeader(int, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	JGeometry::TVec3<f32> calcForces(const TBoid*) const;
	JGeometry::TVec3<f32> calcGoalForce(const JGeometry::TVec3<f32>&) const;
	void setGraph(TGraphWeb*, const JGeometry::TVec3<f32>&);
	void calcBoids();
	void updateGoal();

	void setUnk38(const JGeometry::TVec3<f32>& v) { mGoal = v; }
	TBoid* getBoid(int idx) { return &mBoids[idx]; }
	int getBoidNum() const { return mNumBoids; }

	void setFleeTarget(const TPathNode& target) { mFleeTarget = target; }

public:
	enum {
		FLAG_SIMULATE       = 1,
		FLAG_USE_GRAPH_GOAL = 4,
	};

	/* 0x10 */ int mNumBoids;
	/* 0x14 */ TBoid* mBoids;
	/* 0x18 */ TGraphTracer* mTracer;
	/* 0x1C */ u32 mFlags;
	/* 0x20 */ f32 mBaseSpeed;
	/* 0x24 */ f32 mNeighborRadius;
	/* 0x28 */ f32 mYawSpeed;
	/* 0x2C */ f32 mPitchSpeed;
	/* 0x30 */ f32 mMaxPitch;
	/* 0x34 */ f32 mAlignmentStrength;
	/* 0x38 */ TPathNode mGoal;
	/* 0x48 */ f32 mGoalStrength;
	/* 0x4C */ JGeometry::TVec3<f32> mGoalOffset;
	/* 0x58 */ s32 unk58;
	/* 0x5C */ TPathNode mFleeTarget;
	/* 0x6C */ f32 mFleeRadius;
	/* 0x70 */ f32 mFleeStrength;
	/* 0x74 */ JGeometry::TVec3<f32> mGraphGoal;
};

#endif
