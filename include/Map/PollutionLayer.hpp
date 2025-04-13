#ifndef MAP_POLLUTION_LAYER_HPP
#define MAP_POLLUTION_LAYER_HPP

#include <Map/JointModel.hpp>
#include <Map/PollutionPos.hpp>

class TPollutionLayerInfo { };
struct ResTIMG;

class TPollutionLayer : public TJointModel {
public:
	TPollutionLayer();

	virtual TJointObj* newJointObj() const;
	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 getJ3DModelDataFlag() const;
	virtual void getPlaneType() const;
	virtual f32 getTexPosS(f32) const;
	virtual f32 getTexPosT(f32) const;
	virtual void initLayerInfo(const TPollutionLayerInfo*);
	virtual void getTexResource(const char*);
	virtual void stamp(u16, f32, f32, f32, f32);
	virtual void stampModel(J3DModel*);
	virtual void isPolluted(f32, f32, f32) const;
	virtual void isInArea(f32, f32, f32) const;
	virtual void isInAreaSize(f32, f32, f32, f32) const;

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
	/* 0x54 */ void* unk54;
	/* 0x58 */ ResTIMG* unk58;
	/* 0x5C */ TPollutionPos unk5C;
	/* 0x80 */ void* unk80;
	/* 0x84 */ u8 unk84;
	/* 0x85 */ u8 unk85;
	/* 0x86 */ char unk86[0x6];
	/* 0x8C */ u32 unk8C;
	/* 0x90 */ u32 unk90;
	/* 0x94 */ u32 unk94;
	/* 0x98 */ Vec* unk98;
	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ u32 unkA8;
};

class TPollutionLayerWallBase : public TPollutionLayer {
	TPollutionLayerWallBase();
	virtual f32 getTexPosT(f32) const;
};

class TPollutionLayerWallPlusZ : public TPollutionLayerWallBase {
public:
	virtual void isInAreaSize(f32, f32, f32, f32) const;
	virtual void isInArea(f32, f32, f32) const;
};

class TPollutionLayerWallMinusZ : public TPollutionLayerWallPlusZ {
public:
	virtual f32 getTexPosS(f32) const;
	virtual void getPlaneType() const;
};

class TPollutionLayerWallPlusX : public TPollutionLayerWallBase {
public:
	virtual void isInAreaSize(f32, f32, f32, f32) const;
	virtual void isInArea(f32, f32, f32) const;
};

class TPollutionLayerWallMinusX : public TPollutionLayerWallPlusX {
public:
	virtual f32 getTexPosS(f32) const;
	virtual void getPlaneType() const;
};

#endif
