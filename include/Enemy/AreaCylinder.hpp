#ifndef ENEMY_AREA_CYLINDER_HPP
#define ENEMY_AREA_CYLINDER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TAreaCylinder : public JDrama::TViewObj {
public:
	TAreaCylinder(const char*);
	void load(JSUMemoryInputStream&);
	void perform(u32, JDrama::TGraphics*);
};

class TAreaCylinderManager : public JDrama::TViewObj {
	TAreaCylinderManager(const char*);
	void registerCylinder(TAreaCylinder*);
	void contain(const JGeometry::TVec3<f32>&);
	void getCylinderContains(const JGeometry::TVec3<f32>&);
	void perform(u32, JDrama::TGraphics*);
};

#endif
