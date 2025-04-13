#ifndef MAP_POLLUTION_LAYER_HPP
#define MAP_POLLUTION_LAYER_HPP

#include <Map/JointModel.hpp>
#include <Map/PollutionPos.hpp>
#include <Map/PollutionObj.hpp>

class TPollutionLayerInfo {
public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ char unk4[0x4];
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
	virtual u32 getJ3DModelDataFlag() const;
	virtual void getPlaneType() const;
	virtual int getTexPosS(f32) const;
	virtual int getTexPosT(f32) const;
	virtual void initLayerInfo(const TPollutionLayerInfo*);
	virtual ResTIMG* getTexResource(const char*);
	virtual void stamp(u16, f32, f32, f32, f32);
	virtual void stampModel(J3DModel*);
	virtual void isPolluted(f32, f32, f32) const;
	virtual void isInArea(f32, f32, f32) const;
	virtual void isInAreaSize(f32, f32, f32, f32) const { }

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

	static u32 mAreaMinRate;
	static u32 mSpreadArea;
	static u32 mSpreadFrequency;
	static u32 mFireArea;
	static u32 mFireEffectWaitTime;
	static u32 mThunderArea;
	static u32 mThunderScaleRate;
	static u32 mGlassWallArea;
	static u32 mGlassWallScaleRate;
	static u32 mGlassWallEffectTime;
	static u32 mEffectTime;

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
	TPollutionLayerWallBase();
	virtual int getTexPosT(f32) const;
};

class TPollutionLayerWallPlusX : public TPollutionLayerWallBase {
public:
	virtual void getPlaneType() const { }
	virtual int getTexPosS(f32) const { }
	virtual void isInArea(f32, f32, f32) const;
	virtual void isInAreaSize(f32, f32, f32, f32) const;

	void initLayerInfo(const TPollutionLayerInfo*);
	void stamp(u16, f32, f32, f32, f32);
};

class TPollutionLayerWallMinusX : public TPollutionLayerWallPlusX {
public:
	virtual void getPlaneType() const;
	virtual int getTexPosS(f32) const;
};

class TPollutionLayerWallPlusZ : public TPollutionLayerWallBase {
public:
	virtual void getPlaneType() const { }
	virtual void isInArea(f32, f32, f32) const;
	virtual void isInAreaSize(f32, f32, f32, f32) const;

	void initLayerInfo(const TPollutionLayerInfo*);
	void stamp(u16, f32, f32, f32, f32);
};

class TPollutionLayerWallMinusZ : public TPollutionLayerWallPlusZ {
public:
	virtual void getPlaneType() const;
	virtual int getTexPosS(f32) const;
};

class TPollutionLayerWave : public TPollutionLayer {
public:
	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual void getPlaneType() const { }
	virtual ResTIMG* getTexResource(const char*);

	void draw() const;
	void initGX() const;

	static u32 mInterval;
	static u32 mAlpha;
};

#endif
