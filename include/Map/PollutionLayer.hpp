#ifndef MAP_POLLUTION_LAYER_HPP
#define MAP_POLLUTION_LAYER_HPP

#include <Map/JointModel.hpp>
#include <Map/PollutionPos.hpp>
#include <Map/PollutionObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>

struct TPollutionLayerInfo {
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ u16 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ u16 unk20;
	/* 0x22 */ u16 unk22;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u8* unk28;
};
struct ResTIMG;

class TPollutionLayer : public TJointModel {
public:
	TPollutionLayer();

	virtual TJointObj* newJointObj() const { return new TPollutionObj; }
	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 getJ3DModelDataFlag() const { return 0x11220000; }
	virtual int getPlaneType() const;
	virtual int getTexPosS(f32) const;
	virtual int getTexPosT(f32) const;
	virtual void initLayerInfo(const TPollutionLayerInfo*);
	virtual ResTIMG* getTexResource(const char*)
	{
		return getModelData()->getTexture()->mResources;
	}
	virtual void stamp(u16, f32, f32, f32, f32);
	virtual void stampModel(J3DModel*);
	virtual bool isPolluted(f32, f32, f32) const;
	virtual bool isInArea(f32, f32, f32) const;
	virtual bool isInAreaSize(f32, f32, f32, f32) const { }

	void action();
	void fire();
	void glassWall();
	void electric();
	void spread();
	void changeEffectScale(const JGeometry::TVec3<f32>&, f32);
	void getPollutedPos(f32, JGeometry::TVec3<f32>*);
	void getPollutedPosNear(f32, JGeometry::TVec3<f32>*);
	void changeType(u16);
	void initPollutionTex(const char*);
	void initTex(const char*);
	void initTexImage(const char*);
	void subtractFromYMap(f32, f32, f32) const;
	void isPolluted(int, int, f32) const;
	void isProhibit(f32, f32, f32) const;
	void cleaned(f32, f32, f32, f32);
	void appearItem(f32, f32, f32);

	static f32 mAreaMinRate;
	static f32 mSpreadArea;
	static u32 mSpreadFrequency;
	static f32 mFireArea;
	static u32 mFireEffectWaitTime;
	static f32 mThunderArea;
	static u32 mThunderScaleRate;
	static f32 mGlassWallArea;
	static u32 mGlassWallScaleRate;
	static u32 mGlassWallEffectTime;
	static u32 mEffectTime;

	// fabricated
	const ResTIMG* getUnk58() const { return unk58; }
	u32 getUnk48() const { return unk48; }
	u16 getPollutionType() const { return unk30; }
	u32 getPollutionDegree() const { return unk34; }
	TPollutionObj* getObj(int i) { return (TPollutionObj*)getChild(i); }
	void onUnk32(u32 flag) { unk32 |= flag; }

public:
	/* 0x30 */ u16 unk30;
	/* 0x32 */ u16 unk32;
	/* 0x34 */ u32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ u32 unk4C;
	/* 0x50 */ u8 unk50;
	/* 0x54 */ u8* unk54;
	/* 0x58 */ ResTIMG* unk58;
	/* 0x5C */ TPollutionPos unk5C;
	/* 0x80 */ u8* unk80;
	/* 0x84 */ u8 unk84;
	/* 0x85 */ u8 unk85;
	/* 0x88 */ u32 unk88;
	/* 0x8C */ u32 unk8C;
	/* 0x90 */ u32 unk90;
	/* 0x94 */ u32 unk94;
	/* 0x98 */ JGeometry::TVec3<f32>* unk98;
	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ u32 unkA8;
};

class TPollutionLayerWallBase : public TPollutionLayer {
public:
	TPollutionLayerWallBase();
	virtual int getTexPosT(f32 param_1) const
	{
		return unk5C.worldToTexSize(unkB0 - param_1);
	}

public:
	/* 0xAC */ f32 unkAC;
	/* 0xB0 */ f32 unkB0;
};

class TPollutionLayerWallPlusX : public TPollutionLayerWallBase {
public:
	virtual int getPlaneType() const { }
	virtual int getTexPosS(f32) const { }
	virtual bool isInArea(f32 x, f32 y, f32 z) const
	{
		if (z < unk40 || unk44 < z || y < unkAC || unkB0 < y)
			return 0;
		return 1;
	}
	virtual bool isInAreaSize(f32 x, f32 y, f32 z, f32 s) const
	{
		if (z < unk40 - s || unk44 + s < z || y < unkAC - s || unkB0 + s < y)
			return 0;
		return 1;
	}

	void initLayerInfo(const TPollutionLayerInfo*);
	void stamp(u16, f32, f32, f32, f32);
};

class TPollutionLayerWallMinusX : public TPollutionLayerWallPlusX {
public:
	virtual int getPlaneType() const { return 3; }
	virtual int getTexPosS(f32 param_1) const
	{
		return unk5C.worldToTexSize(param_1 - unk40);
	}
};

class TPollutionLayerWallPlusZ : public TPollutionLayerWallBase {
public:
	virtual int getPlaneType() const { }
	virtual bool isInArea(f32 x, f32 y, f32 z) const
	{
		if (x < unk38 || unk3C < x || y < unkAC || unkB0 < y)
			return 0;
		return 1;
	}
	virtual bool isInAreaSize(f32 x, f32 y, f32 z, f32 s) const
	{
		if (x < unk38 - s || unk3C + s < x || y < unkAC - s || unkB0 + s < y)
			return 0;
		return 1;
	}

	void initLayerInfo(const TPollutionLayerInfo*);
	void stamp(u16, f32, f32, f32, f32);
};

class TPollutionLayerWallMinusZ : public TPollutionLayerWallPlusZ {
public:
	virtual int getPlaneType() const { return 5; }
	virtual int getTexPosS(f32 param_1) const
	{
		return unk5C.worldToTexSize(unk3C - param_1);
	}
};

class TPollutionLayerWave : public TPollutionLayer {
public:
	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual int getPlaneType() const { }
	virtual ResTIMG* getTexResource(const char*);

	void draw() const;
	void initGX() const;

	static u32 mInterval;
	static u32 mAlpha;
};

#endif
