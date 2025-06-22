#ifndef MOVE_BG_MAP_OBJ_GRASS_HPP
#define MOVE_BG_MAP_OBJ_GRASS_HPP

#include <Strategic/HitActor.hpp>

class TMapObjGrassGroup : public THitActor {
public:
	virtual void load(JSUMemoryInputStream&);

	void drawNear() const;
	void drawFar() const;
	void calc();
	TMapObjGrassGroup();

public:
	/* 0x68 */ s32 unk68;
	/* 0x6C */ JGeometry::TVec3<f32>* unk6C;
	/* 0x70 */ JGeometry::TVec3<s16>* unk70;
	/* 0x74 */ s16* unk74;
	/* 0x78 */ s32 unk78;
};

class TMapObjGrassManager;

extern TMapObjGrassManager* gpMapObjGrassManager;

class TMapObjGrassManager : public JDrama::TViewObj {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void initDrawNear() const;
	void initDrawFar() const;
	void draw() const;
	void entryGrassGroup(TMapObjGrassGroup*, long);
	TMapObjGrassManager(const char*);

	static f32 mWidth;
	static f32 mSwingWidth;
	static f32 mSwingSpeed;
	static f32 mDistNear;
	static f32 mDistFar;
	static JUtility::TColor mColorUpper;
	static JUtility::TColor mColorLower;
	static JUtility::TColor mColorFar;
	static S16Vec mDrawVecS16;
	static Vec mDrawVec;

public:
	/* 0x10 */ s32 unk10;
	/* 0x14 */ TMapObjGrassGroup** unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ f32* unk20;
	/* 0x24 */ s16* unk24;
};

#endif
