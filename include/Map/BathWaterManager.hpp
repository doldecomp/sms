#ifndef MAP_BATH_WATER_MANAGER_HPP
#define MAP_BATH_WATER_MANAGER_HPP

#include <Strategic/HitActor.hpp>
#include <JSystem/JMath.hpp>

class TBathtubData {
public:
	void getGravityDir(f32) const;
	void getPos(int, int, f32) const;
};

class TBathWaterParams {
public:
	TBathWaterParams(const char*);
};

class TBathWaterGlobalParams {
public:
	TBathWaterGlobalParams();
};

class TBathWater : public THitActor {
public:
	class TDrop {
	public:
		TDrop();
		void calcBathtub(const TBathtubData&, f32, const JGeometry::TVec3<f32>&,
		                 const JGeometry::TVec3<f32>&, int&,
		                 JGeometry::TVec3<f32>&);
	};
};

class TBathWaterRenderer {
public:
	virtual void prerender(JDrama::TGraphics*, const TBathtubData&,
	                       TBathWater**, TBathWaterParams**, int)
	    = 0;
	virtual void render(JDrama::TGraphics*, const TBathtubData&, TBathWater**,
	                    TBathWaterParams**, int)
	    = 0;
	virtual f32 getHeight(f32, f32) const = 0;
};

class TBathWaterFlatRenderer : public TBathWaterRenderer {
public:
	virtual void prerender(JDrama::TGraphics*, const TBathtubData&,
	                       TBathWater**, TBathWaterParams**, int);
	virtual void render(JDrama::TGraphics*, const TBathtubData&, TBathWater**,
	                    TBathWaterParams**, int);
	virtual f32 getHeight(f32, f32) const;
};

class TBathWaterMeshRenderer : public TBathWaterRenderer {
public:
	void clearEFB(GXColor, u16, u16, bool);
	void makeHeightMap(f32);
	void makeNormalMap();
	void calcCoord();
	void clearHeightMap();

	virtual void prerender(JDrama::TGraphics*, const TBathtubData&,
	                       TBathWater**, TBathWaterParams**, int);
	virtual void render(JDrama::TGraphics*, const TBathtubData&, TBathWater**,
	                    TBathWaterParams**, int);
	virtual f32 getHeight(f32, f32) const;
};

class TBathWaterManager;

class TBathWaterPreprocessor : public JDrama::TViewObj {
public:
	TBathWaterPreprocessor(TBathWaterManager*);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ TBathWaterManager* unk10;
};

class TBathWaterManager : public JDrama::TViewObj {
public:
	TBathWaterManager();
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void wave(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&, f32, f32) const;
	void initializeIfYet_();
	void preprocess(JDrama::TGraphics*);
	void getWaterHeight(f32, f32) const;
	void throwMario(f32);

public:
	/* 0x10 */ JMath::TRandom_fast_ unk10;
	/* 0x14 */ TBathWaterParams** unk14;
	/* 0x18 */ TBathWaterGlobalParams* unk18;
	/* 0x1C */ u8 unk1C;
	/* 0x20 */ TBathWater** unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ TBathWaterRenderer* unk28;
	/* 0x2C */ TBathWaterRenderer* unk2C;
	/* 0x30 */ TBathWaterRenderer* unk30;
	/* 0x34 */ TBathWaterPreprocessor unk34;
};

#endif
