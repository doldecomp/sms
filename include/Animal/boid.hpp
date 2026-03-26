#ifndef ANIMAL_BOID
#define ANIMAL_BOID

#include <macros.h>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Enemy/Graph.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <MarioUtil/RandomUtil.hpp>

class TBoid {
public:
	TBoid();

private:
	// many unused, can we just use padding later?
	/* 0x00 */ float unk00;
	/* 0x04 */ float unk04;
	/* 0x08 */ float unk08;
	/* 0x0C */ float unk0C;
	/* 0x10 */ float unk10;
	/* 0x14 */ float unk14;
	/* 0x18 */ float unk18;
	/* 0x1C */ float unk1C;
	/* 0x20 */ float unk20;
	/* 0x24 */ float unk24;
	/* 0x28 */ float unk28;
	/* 0x2C */ float unk2C;
	/* 0x30 */ float unk30;
	/* 0x34 */ float unk34;
	/* 0x38 */ float unk38;
	/* 0x3C */ float unk3C;
	/* 0x40 */ float unk40;
	/* 0x44 */ float unk44;
	/* 0x48 */ float unk48;
	/* 0x4C */ float unk4C;
};

class TBoidLeader : public JDrama::TNameRef, public JDrama::TViewObj {
public:
	TBoidLeader(int boid_count, const char* name);
	~TBoidLeader();
	int calcForces(const TBoid* boid) const;
	int calcGoalForce(const JGeometry::TVec3<float>& goal_position) const;
	void perform(unsigned long some_parameter, JDrama::TGraphics* graphics);
	TBoidLeader* setGraph(TGraphWeb* graph,
	                      const JGeometry::TVec3<float>& graph_position);
	void calcBoids();

private:
	/* 0x00 */ int m_boid_count;
	/* 0x04 */ TBoid* m_boid_array;
	/* 0x08 */ TGraphTracer* unk18;
	/* 0x0C */ int unk1C;
	/* 0x18 */ float unk20;
	/* 0x18 */ float unk24;
	/* 0x18 */ float unk28;
	/* 0x18 */ float unk2C;
	/* 0x18 */ float unk30;
	/* 0x18 */ float unk34;
	/* 0x18 */ int unk38;
	/* 0x18 */ float unk3C;
	/* 0x18 */ float unk40;
	/* 0x18 */ float unk44;
	/* 0x18 */ float unk48;
	/* 0x18 */ float unk4C;
	/* 0x18 */ float unk50;
	/* 0x18 */ float unk54;
	/* 0x18 */ int unk58;
	/* 0x18 */ int unk5C;
	/* 0x18 */ float unk60; // JGeometry::TVec3<float> ?
	/* 0x18 */ float unk64;
	/* 0x18 */ float unk68;
	/* 0x18 */ float unk6C;
	/* 0x18 */ float unk70;
	/* 0x18 */ float unk74;
	/* 0x18 */ float unk78;
	/* 0x18 */ float unk7C;
};

#endif // ANIMAL_BOID
