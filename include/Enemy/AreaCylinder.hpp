#ifndef ENEMY_AREA_CYLINDER_HPP
#define ENEMY_AREA_CYLINDER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TAreaCylinder : public JDrama::TViewObj {
public:
	TAreaCylinder(const char* name = "<TAreaCylinder>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
};

class TAreaCylinderManager : public JDrama::TViewObj {
public:
	TAreaCylinderManager(const char* name);

	virtual void perform(u32, JDrama::TGraphics*);

	void registerCylinder(TAreaCylinder*);
	void contain(const JGeometry::TVec3<f32>&);
	TAreaCylinder* getCylinderContains(const JGeometry::TVec3<f32>&);
};

#endif
