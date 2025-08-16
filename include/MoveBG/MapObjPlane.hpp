#ifndef MOVE_BG_MAP_OBJ_PLANE_HPP
#define MOVE_BG_MAP_OBJ_PLANE_HPP

#include <Strategic/LiveActor.hpp>

struct ResTIMG;
class TMapCheckGroundPlane;

class TMapObjPlane : public TLiveActor {
public:
	TMapObjPlane(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual BOOL hasMapCollision() const { return TRUE; }

	void initDraw();
	void draw();
	f32 getTexPos(f32 v) const;
	void updateCheckData(int x, int z);
	void calcNrm(int x, int z);
	void movement();
	void depress(f32, f32, f32);
	void weather();
	void makeMountain();

	f32& heightAt(int x, int z) { return mHeightMap[x + z * mExtents]; }
	const JGeometry::TVec3<f32>& normalAt(int x, int z)
	{
		return mNormalMap[x + z * mExtents];
	}

	static f32 mHipDropDownRate;
	static f32 mWaterDownRate;
	static f32 mMudDownRate;
	static f32 mTexScale;
	static f32 mWeatherRate;
	static u8 mAmbColor;

public:
	/* 0xF4 */ f32 unkF4;
	/* 0xF8 */ f32 unkF8;
	/* 0xFC */ f32 unkFC;
	/* 0x100 */ int mExtents;
	/* 0x104 */ f32* mHeightMap;
	/* 0x108 */ JGeometry::TVec3<f32>* mNormalMap;
	/* 0x10C */ TMapCheckGroundPlane* mCollision;
	/* 0x110 */ u32 unk110;
	/* 0x114 */ ResTIMG* mAlbedo;
	/* 0x118 */ u8* unk118;
	/* 0x11C */ u16 unk11C;
};

class TRockPlane : TMapObjPlane {
public:
	TRockPlane(const char*);

	virtual void load(JSUMemoryInputStream&);
};

class TSandPlane : TMapObjPlane {
public:
	TSandPlane(const char*);

	virtual void load(JSUMemoryInputStream&);
};

#endif
