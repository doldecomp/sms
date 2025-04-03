#ifndef MAP_MAP_HPP
#define MAP_MAP_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TBGCheckData;
struct TBGWallCheckRecord;

class TMapCollisionData;
class TMapModelManager;
class TMapWarp;
class TMapXlu;

class TMap : public JDrama::TViewObj {
public:
	~TMap();
	TMap(const char*);

	void load(JSUMemoryInputStream&);
	void loadAfter();
	void perform(u32, JDrama::TGraphics*);
	void changeModel(s16) const;
	f32 checkGround(f32, f32, f32, const TBGCheckData**) const;
	void checkGround(const JGeometry::TVec3<f32>&, const TBGCheckData**) const;
	void checkGroundExactY(const JGeometry::TVec3<f32>&,
	                       const TBGCheckData**) const;
	void checkGroundExactY(f32, f32, f32, const TBGCheckData**) const;
	void checkGroundIgnoreWaterSurface(const JGeometry::TVec3<f32>&,
	                                   const TBGCheckData**) const;
	void checkGroundIgnoreWaterSurface(f32, f32, f32,
	                                   const TBGCheckData**) const;
	void checkGroundIgnoreWaterThrough(f32, f32, f32,
	                                   const TBGCheckData**) const;
	void checkRoof(const JGeometry::TVec3<f32>&, const TBGCheckData**) const;
	void checkRoof(f32, f32, f32, const TBGCheckData**) const;
	void checkRoofIgnoreWaterThrough(f32, f32, f32, const TBGCheckData**) const;
	void isTouchedWallsAndMoveXZ(TBGWallCheckRecord*) const;
	void isTouchedOneWallAndMoveXZ(f32*, f32, f32*, f32) const;
	void isTouchedOneWall(f32, f32, f32, f32) const;
	void isTouchedOneWall(const JGeometry::TVec3<f32>&, f32) const;
	void intersectLine(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&, bool,
	                   JGeometry::TVec3<f32>*) const;
	void isInArea(f32, f32) const;

	static TBGCheckData* getIllegalCheckData();

	void update();
	void updateMonte();
	void updateDelfino();
	void draw(u32, JDrama::TGraphics*) const;

public:
	/* 0x10 */ TMapCollisionData* mCollisionData;
	/* 0x14 */ TMapModelManager* mModelManager;
	/* 0x18 */ TMapWarp* mWarp;
	/* 0x1C */ TMapXlu* mXlu;
	/* 0x20 */ u32 unk20;
};

extern TMap* gpMap;

#endif
