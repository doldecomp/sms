#ifndef MAP_MAP_HPP
#define MAP_MAP_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <Map/MapModel.hpp>

class TBGCheckData;
struct TBGWallCheckRecord;
class TMapCollisionData;
class TMapModelManager;
class TMapWarp;
class TMapXlu;
class TMap;

extern TMap* gpMap;

class TMap : public JDrama::TViewObj {
public:
	~TMap();
	TMap(const char*);

	void load(JSUMemoryInputStream&);
	void loadAfter();
	void perform(u32, JDrama::TGraphics*);
	void changeModel(s16) const;

	f32 checkGround(f32 x, f32 y, f32 z, const TBGCheckData**) const;
	f32 checkGround(const JGeometry::TVec3<f32>& pos,
	                const TBGCheckData**) const;
	f32 checkGroundExactY(const JGeometry::TVec3<f32>& pos,
	                      const TBGCheckData**) const;
	f32 checkGroundExactY(f32 x, f32 y, f32 z, const TBGCheckData**) const;
	f32 checkGroundIgnoreWaterSurface(const JGeometry::TVec3<f32>& pos,
	                                  const TBGCheckData**) const;
	f32 checkGroundIgnoreWaterSurface(f32 x, f32 y, f32 z,
	                                  const TBGCheckData**) const;
	f32 checkGroundIgnoreWaterThrough(f32 x, f32 y, f32 z,
	                                  const TBGCheckData**) const;

	f32 checkRoof(const JGeometry::TVec3<f32>& pos, const TBGCheckData**) const;
	f32 checkRoof(f32 x, f32 y, f32 z, const TBGCheckData**) const;
	f32 checkRoofIgnoreWaterThrough(f32 x, f32 y, f32 z,
	                                const TBGCheckData**) const;

	bool isTouchedWallsAndMoveXZ(TBGWallCheckRecord*) const;
	bool isTouchedOneWallAndMoveXZ(f32* x, f32 y, f32* z, f32) const;

	bool isTouchedOneWall(f32 x, f32 y, f32 z, f32) const;
	bool isTouchedOneWall(const JGeometry::TVec3<f32>& pos, f32) const;

	void intersectLine(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&, bool,
	                   JGeometry::TVec3<f32>*) const;
	bool isInArea(f32, f32) const;

	static TBGCheckData* getIllegalCheckData();

	void update();
	void updateMonte();
	void updateDelfino();
	void draw(u32, JDrama::TGraphics*) const;

	// fabricated
	TMapModelManager* getModelManager() { return mModelManager; }
	// idk whether it's actually the root tho
	TJointModel* getRootJointModel() { return mModelManager->getJointModel(0); }

public:
	/* 0x10 */ TMapCollisionData* mCollisionData;
	/* 0x14 */ TMapModelManager* mModelManager;
	/* 0x18 */ TMapWarp* mWarp;
	/* 0x1C */ TMapXlu* mXlu;
	/* 0x20 */ u8 unk20;
};

#endif
