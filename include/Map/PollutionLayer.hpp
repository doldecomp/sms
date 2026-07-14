#ifndef MAP_POLLUTION_LAYER_HPP
#define MAP_POLLUTION_LAYER_HPP

#include <Map/JointModel.hpp>
#include <Map/PollutionPos.hpp>
#include <Map/PollutionObj.hpp>
#include <JSystem/J3D/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3D/J3DGraphBase/J3DTexture.hpp>
#include <JSystem/J3D/J3DGraphLoader/J3DModelLoaderFlags.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

struct ResTIMG;

enum EPollutionType {
	POLLUTION_TYPE_SINK       = 0,
	POLLUTION_TYPE_FIRE       = 1,
	POLLUTION_TYPE_SLIP       = 2,
	POLLUTION_TYPE_GLASS_WALL = 3,
	POLLUTION_TYPE_ELECTRIC   = 4,
	POLLUTION_TYPE_INSTAKILL  = 5,
	POLLUTION_TYPE_SAFE       = 6,
	POLLUTION_TYPE_UNK7       = 7,

	// Both act as "not polluted" I think
	POLLUTION_TYPE_UNK8  = 8,
	POLLUTION_TYPE_UNK10 = 10,
};

/// Serialized layer data from on-disk assets
struct TPollutionLayerInfo {
	/* 0x0 */ u16 mPollutionType;
	/* 0x2 */ u16 mFlags;
	/* 0x4 */ u16 mPlaneType;
	/* 0x8 */ f32 mVerticalOffset;
	/* 0xC */ f32 mTexelSize;

	// World-space bounds that get shuffled into min/max x/y/z
	// depending on the plane type
	/* 0x10 */ f32 mLeft;
	/* 0x14 */ f32 mTop;
	/* 0x18 */ f32 mRight;
	/* 0x1C */ f32 mBottom;

	/* 0x20 */ u16 mLog2Width;
	/* 0x22 */ u16 mLog2Height;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ u8* mHeightMap;
};

/**
 * @brief An in-world "pollutable region" object.
 * @details An axis-aligned rectangular region of the map where goop can be
 * placed or cleared. Can be horizontal, vertical, or even cover water, see
 * derived classes. The pollution can be painted or cleared on this layer as
 * if it was a canvas, offscreen GX rendering is used for it. Sound/particle
 * effects are spawned in the polluted region automatically.
 */
class TPollutionLayer : public TJointModel {
public:
	TPollutionLayer();

	virtual TJointObj* newJointObj() const { return new TPollutionObj; }
	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual u32 getJ3DModelDataFlag() const
	{
		return J3DMLF_MaterialPEFull | J3DMLF_MaterialUseIndirect
		       | J3DMLF_UseUniqueMaterials | (2 << J3DMLF_TevStageNumShift);
	}
	virtual int getPlaneType() const { return 0; }
	virtual int getTexPosS(f32 param_1) const
	{
		return mPos.worldToTexSize(param_1 - mMinX);
	}
	virtual int getTexPosT(f32 param_1) const
	{
		return mPos.worldToTexSize(param_1 - mMinZ);
	}
	virtual void initLayerInfo(const TPollutionLayerInfo*);
	virtual ResTIMG* getTexResource(const char*)
	{
		return getModelData()->getTexture()->mResources;
	}
	virtual void stamp(u16, f32 x, f32 y, f32 z, f32 range);
	virtual void stampModel(J3DModel*);
	virtual bool isPolluted(f32 x, f32 y, f32 z) const;
	virtual bool isInArea(f32 x, f32 y, f32 z) const
	{
		if (x < mMinX || mMaxX < x || z < mMinZ || mMaxZ < z)
			return false;
		return true;
	}
	virtual bool isInAreaSize(f32 x, f32 y, f32 z, f32 s) const
	{
		if (x < mMinX - s || mMaxX + s < x || z < mMinZ - s || mMaxZ + s < z)
			return false;
		return true;
	}
	void action();
	void fire();
	void glassWall();
	void electric();
	void spread();
	void changeEffectScale(const JGeometry::TVec3<f32>&, f32);
	bool getPollutedPos(f32, JGeometry::TVec3<f32>*);
	bool getPollutedPosNear(f32, JGeometry::TVec3<f32>*);
	void changeType(u16);
	void initPollutionTex(const char*);
	void initTex(const char*);
	void initTexImage(const char*);
	void subtractFromYMap(f32, f32, f32) const;
	bool isPolluted(int, int, f32) const;
	void isProhibit(f32, f32, f32) const;
	void cleaned(f32, f32, f32, f32);
	void appearItem(f32, f32, f32);

	static f32 mAreaMinRate;
	static f32 mSpreadArea;
	static s32 mSpreadFrequency;
	static f32 mFireArea;
	static s32 mFireEffectWaitTime;
	static f32 mThunderArea;
	static u32 mThunderScaleRate;
	static f32 mGlassWallArea;
	static u32 mGlassWallScaleRate;
	static s32 mGlassWallEffectTime;
	static int mEffectTime;

	// fabricated
	const ResTIMG* getPollutionImage() const { return mPollutionImage; }
	int getUnk48() const { return unk48; }
	int getPollutionType() const { return mPollutionType; }
	u32 getPollutionDegree() const { return mCounter; }
	TPollutionObj* getObj(int i) { return (TPollutionObj*)getChild(i); }
	TPollutionPos& getPos() { return mPos; }
	int getTexWidth() { return mPos.getWidth(); }
	int getTexHeight() { return mPos.getHeight(); }
	f32 getTexelSize() { return mPos.getTexelSize(); }
	f32 getWorldPosX(int param_1) const
	{
		return mMinX + mPos.texToWorldSize(param_1);
	}
	f32 getWorldPosZ(int param_1) const
	{
		return mMinZ + mPos.texToWorldSize(param_1);
	}
	int worldToDepth(f32 d) const { return mPos.worldToDepth(d); }
	void startDecay() { mFlags |= FLAG_DECAY; }
	void stopDecay() { mFlags &= ~FLAG_DECAY; }

public:
	enum {
		/// If on, the pollution will spread when Mario is nearby
		FLAG_CAN_SPREAD = 0x1,
		/// If on, the pollution will rapidly decay and disappear
		FLAG_DECAY = 0x2,
	};

	/* 0x30 */ u16 mPollutionType;
	/* 0x32 */ u16 mFlags;
	/* 0x34 */ u32 mCounter;
	/* 0x38 */ f32 mMinX;
	/* 0x3C */ f32 mMaxX;
	/* 0x40 */ f32 mMinZ;
	/* 0x44 */ f32 mMaxZ;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ s32 mSpreadTimer;
	/* 0x50 */ u8 mPollutedThreshold;
	/* 0x54 */ u8* mPollutionMap;
	/* 0x58 */ ResTIMG* mPollutionImage;
	/* 0x5C */ TPollutionPos mPos;
	/* 0x80 */ u8* mPollutionBmp;
	/* 0x84 */ u8 mPerFrameChangeDelta;
	/* 0x85 */ u8 mPerFrameChangeThreshold;
	/* 0x88 */ u32 unk88;

	/* 0x8C */ s32 mEffectTimer;
	/* 0x90 */ s32 mCurEffectPosIndex;
	/* 0x94 */ s32 mEffectPositionsCapacity;
	/* 0x98 */ JGeometry::TVec3<f32>* mEffectPositions;

	/* 0x9C */ u32 unk9C;
	/* 0xA0 */ u32 unkA0;
	/* 0xA4 */ u32 unkA4;
	/* 0xA8 */ s32 unkA8;
};

class TPollutionLayerWallBase : public TPollutionLayer {
public:
	TPollutionLayerWallBase();
	virtual int getTexPosT(f32 param_1) const
	{
		return mPos.worldToTexSize(mMaxY - param_1);
	}

public:
	/* 0xAC */ f32 mMinY;
	/* 0xB0 */ f32 mMaxY;
};

class TPollutionLayerWallPlusX : public TPollutionLayerWallBase {
public:
	virtual int getPlaneType() const { return 2; }
	virtual int getTexPosS(f32 v) const
	{
		return mPos.worldToTexSize(mMaxZ - v);
	}
	virtual bool isInArea(f32 x, f32 y, f32 z) const
	{
		if (z < mMinZ || mMaxZ < z || y < mMinY || mMaxY < y)
			return 0;
		return 1;
	}
	virtual bool isInAreaSize(f32 x, f32 y, f32 z, f32 s) const
	{
		if (z < mMinZ - s || mMaxZ + s < z || y < mMinY - s || mMaxY + s < y)
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
		return mPos.worldToTexSize(param_1 - mMinZ);
	}
};

class TPollutionLayerWallPlusZ : public TPollutionLayerWallBase {
public:
	virtual int getPlaneType() const { return 4; }
	virtual bool isInArea(f32 x, f32 y, f32 z) const
	{
		if (x < mMinX || mMaxX < x || y < mMinY || mMaxY < y)
			return 0;
		return 1;
	}
	virtual bool isInAreaSize(f32 x, f32 y, f32 z, f32 s) const
	{
		if (x < mMinX - s || mMaxX + s < x || y < mMinY - s || mMaxY + s < y)
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
		return mPos.worldToTexSize(mMaxX - param_1);
	}
};

class TPollutionLayerWave : public TPollutionLayer {
public:
	virtual void initJointModel(TJointModelManager*, const char*,
	                            MActorAnmData*);
	virtual void perform(unsigned long cue, JDrama::TGraphics* graphics);
	virtual int getPlaneType() const { return 6; }
	virtual ResTIMG* getTexResource(const char*);

	void draw() const;
	void initGX() const;

	static f32 mInterval;
	static u8 mAlpha;
};

#endif
