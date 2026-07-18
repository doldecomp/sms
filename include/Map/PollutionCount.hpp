#ifndef MAP_POLLUTION_COUNT_HPP
#define MAP_POLLUTION_COUNT_HPP

#include <System/DrawSyncCallback.hpp>
#include <types.h>

class TPollutionObj;
class TPollutionLayer;
class J3DModel;
class J3DDrawBuffer;
class ResTIMG;

class TPollutionCounterBase : public TDrawSyncCallback {
public:
	TPollutionCounterBase()
	    : mCounterCapacity(0)
	    , mCounterNum(0)
	    , mCounters(nullptr)
	    , mPolygonCount(nullptr)
	{
	}

	void setCallback(int) const;
	void drawSyncCallback(u16);
	void initCounters(int);

	virtual int getTokenNo(int) const   = 0;
	virtual u16 getCounterNo(u32) const = 0;

public:
	/* 0x4 */ int mCounterCapacity;
	/* 0x8 */ int mCounterNum;
	/* 0xC */ u32** mCounters;
	/* 0x10 */ u32* mPolygonCount;
};

class TPollutionCounterObj : public TPollutionCounterBase {
public:
	TPollutionCounterObj();

	virtual int getTokenNo(int param_1) const { return param_1 + 0x92; }
	virtual u16 getCounterNo(u32 param_1) const { return param_1 - 0x92; }

	void draw(int index) const;
	void countObjDegree() const;
	void registerPollutionObj(TPollutionObj* obj, u32* counter_ptr);
	void init(int max_counters);

public:
	/* 0x14 */ TPollutionObj** mObjects;
};

class TPollutionLayerTaskInfo {
public:
	TPollutionLayerTaskInfo()
	    : mLayerIdx(0)
	    , mX(0)
	    , mZ(0)
	    , mSize(0)
	    , mY(0)
	{
	}

public:
	/* 0x0 */ u8 mLayerIdx;
	/* 0x2 */ u16 mX;
	/* 0x4 */ u16 mZ;
	/* 0x6 */ u16 mSize;
	/* 0x8 */ s16 mY;
};

class TPollutionModelTaskInfo {
public:
	TPollutionModelTaskInfo()
	    : mLayerIdx(0)
	    , mModel(nullptr)
	{
	}

public:
	/* 0x0 */ u8 mLayerIdx;
	/* 0x4 */ J3DModel* mModel;
};

class TPollutionJointObjTaskInfo {
public:
	TPollutionJointObjTaskInfo()
	    : unk0(0)
	    , mLayerIdx(0)
	    , mJointObj(nullptr)
	{
	}

public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 mLayerIdx;
	/* 0x4 */ TPollutionObj* mJointObj;
};

/**
 * @brief A "brush" that the pollution texture can be painted with.
 * @details Can both add and clean up pollution.
 */
class TPollutionTexStamp {
public:
	TPollutionTexStamp();

	void pushTask(u8 layer_idx, u16 size, u16 x, u16 z, s16 y);
	void registerTexStamp(u16, u16, ResTIMG*);

public:
	/* 0x0 */ u16 mStampType; //< 0 is clear, 1 is pollute
	/* 0x2 */ u16 unk2;
	/* 0x4 */ ResTIMG* mStampShapeTex;
	/* 0x8 */ int mTaskNum;
	/* 0xC */ int mTaskCapacity;
	/* 0x10 */ TPollutionLayerTaskInfo* mTaskQueue;
};

class TPollutionRevivalTexStamp {
public:
	void registerTex(int, short, short, short, short, int, ResTIMG*);
	TPollutionRevivalTexStamp();

	bool isThing() const { return unk0 == 0 ? true : false; }

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ int mTargetLayer;
	/* 0x8 */ s16 mLeft;
	/* 0xA */ s16 mTop;
	/* 0xC */ s16 mWidth;
	/* 0xE */ s16 mHeight;
	/* 0x10 */ ResTIMG* mStampShapeTex;
	/* 0x14 */ int mStampTimer;
	/* 0x18 */ int mStampInterval;
};

class TPollutionCounterLayer : public TPollutionCounterBase {
public:
	TPollutionCounterLayer();

	virtual int getTokenNo(int param_1) const { return param_1 + 0x7E; }
	virtual u16 getCounterNo(u32 param_1) const { return param_1 - 0x7E; }

	void drawPollutionLayer(int) const;
	void drawJointObjStamp(int) const;
	void drawTexStamp(int) const;
	void drawRevivalTexStamp(int) const;
	void cleanProhibitArea(int) const;
	void drawModelStamp(int);
	void countTexDegree(int);
	void resetTask();
	void setTevColorInByStampType(u16) const;
	void pushJointObjStampTask(u8, u8, TPollutionObj*);
	void calcViewMtx();
	void pushModelStampTask(u8, J3DModel*);
	int registerRevivalTexStamp(int, short, short, short, short, int, ResTIMG*);
	int registerTexStamp(u16, u16, ResTIMG*);
	void registerLayer(const TPollutionLayer*, u32*);
	bool stampIsCleanType(u16) const;
	void onLayer(int);
	void offLayer(int);
	void init(int, u16, u16);

	void pushStampTask(u16 stamp_type, u8 layer_idx, u16 size, u16 x, u16 z,
	                   s16 y)
	{
		mTexStamps[stamp_type].pushTask(layer_idx, size, x, z, y);
	}

public:
	/* 0x14 */ const TPollutionLayer** mLayers;
	/* 0x18 */ u16 mTexStampCapacity;
	/* 0x1A */ u16 mTexStampNum;
	/* 0x1C */ TPollutionTexStamp* mTexStamps;
	/* 0x20 */ u16 mRevivalTexStampCapacity;
	/* 0x22 */ u16 mRevivalTexStampNum;
	/* 0x24 */ TPollutionRevivalTexStamp* mRevivalTexStamps;
	/* 0x28 */ u16 mModelStampTaskNum;
	/* 0x2C */ J3DDrawBuffer** mModelStampDrawBuffers;
	/* 0x30 */ u16* mLayerModelStampTaskNum;
	/* 0x34 */ TPollutionModelTaskInfo mModelStampTaskQueue[20];
	/* 0xD4 */ u16 mJointObjStampTaskNum;
	/* 0xD8 */ TPollutionJointObjTaskInfo mJointObjStampTaskQueue[20];
	/* 0x178 */ bool* mIsLayerEnabled;
};

#endif
