#ifndef ANIMAL_BOID_HPP
#define ANIMAL_BOID_HPP

#include <Enemy/Graph.hpp>
#include <Enemy/PathNode.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry.hpp>

class TBoid;

class TBoidLeader : public JDrama::TViewObj {
public:
	TBoidLeader(int, const char*);

	virtual ~TBoidLeader();
	virtual void perform(u32, JDrama::TGraphics*);

	JGeometry::TVec3<f32> calcForces(const TBoid*) const;
	JGeometry::TVec3<f32> calcGoalForce(const JGeometry::TVec3<f32>&) const;
	void setGraph(TGraphWeb*, const JGeometry::TVec3<f32>&);
	void calcBoids();

public:
	/* 0x10 */ int mNumBoids;
	/* 0x14 */ TBoid* mBoids;
	/* 0x18 */ TGraphTracer* unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ TPathNode unk38;
	/* 0x48 */ f32 unk48;
	/* 0x4C */ JGeometry::TVec3<f32> unk4C;
	/* 0x58 */ s32 unk58;
	/* 0x5C */ TPathNode unk5C;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ f32 unk74;
	/* 0x78 */ f32 unk78;
	/* 0x7C */ f32 unk7C;
};

class TBoid {
public:
	TBoid();

public:
	/* 0x00 */ JGeometry::TVec3<f32> unk0;
	/* 0x0C */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ JGeometry::TVec3<f32> unk18;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
	/* 0x30 */ JGeometry::TVec3<f32> unk30;
	/* 0x3C */ JGeometry::TVec3<f32> unk3C;
	/* 0x48 */ s32 unk48;
	/* 0x4C */ f32 unk4C;
};

#endif
