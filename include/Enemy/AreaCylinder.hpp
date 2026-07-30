#ifndef ENEMY_AREA_CYLINDER_HPP
#define ENEMY_AREA_CYLINDER_HPP

#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>

class TAreaCylinder : public JDrama::TViewObj {
public:
	TAreaCylinder(const char* name = "<TAreaCylinder>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	// fabricated
	BOOL contain(const JGeometry::TVec3<f32>& pos) const
	{
		if (pos.y < mPos.y || mPos.y + mHeight < pos.y)
			return FALSE;

		return (pos.x - mPos.x) * (pos.x - mPos.x)
		           + (pos.z - mPos.z) * (pos.z - mPos.z)
		       <= mRadius * mRadius;
	}

public:
	/* 0x10 */ JGeometry::TVec3<f32> mPos;
	/* 0x1C */ f32 mRadius;
	/* 0x20 */ f32 mHeight;
	/* 0x24 */ f32 mProbability;
};

class TAreaCylinderManager : public JDrama::TViewObj {
public:
	TAreaCylinderManager(const char* name);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void registerCylinder(TAreaCylinder*);
	BOOL contain(const JGeometry::TVec3<f32>&);
	TAreaCylinder* getCylinderContains(const JGeometry::TVec3<f32>&);

public:
	/* 0x10 */ JGadget::TList<TAreaCylinder*> mList;
};

#endif
