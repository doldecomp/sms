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
	    : unk4(0)
	    , unk8(0)
	    , unkC(nullptr)
	    , unk10(nullptr)
	{
	}

	void setCallback(int) const;
	void drawSyncCallback(u16);
	void initCounters(int);

	virtual int getTokenNo(int) const   = 0;
	virtual u32 getCounterNo(u32) const = 0;

public:
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ u32** unkC;
	/* 0x10 */ u32* unk10;
};

class TPollutionCounterObj : public TPollutionCounterBase {
public:
	TPollutionCounterObj();

	virtual int getTokenNo(int param_1) const { return param_1 + 0x92; }
	virtual u32 getCounterNo(u32 param_1) const { return param_1 - 0x92; }

	void draw(int) const;
	void countObjDegree() const;
	void registerPollutionObj(TPollutionObj*, u32*);
	void init(int);

public:
	/* 0x14 */ TPollutionObj** unk14;
};

class TPollutionLayerTaskInfo {
public:
	TPollutionLayerTaskInfo()
	    : unk0(0)
	    , unk2(0)
	    , unk4(0)
	    , unk6(0)
	    , unk8(0)
	{
	}

public:
	/* 0x0 */ u8 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ u16 unk4;
	/* 0x6 */ u16 unk6;
	/* 0x8 */ s16 unk8;
};

class TPollutionModelTaskInfo {
public:
	TPollutionModelTaskInfo()
	    : unk0(0)
	    , unk4(nullptr)
	{
	}

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ J3DModel* unk4;
};

class TPollutionJointObjTaskInfo {
public:
	TPollutionJointObjTaskInfo()
	    : unk0(0)
	    , unk1(0)
	    , unk4(nullptr)
	{
	}

public:
	/* 0x0 */ u8 unk0;
	/* 0x1 */ u8 unk1;
	/* 0x4 */ TPollutionObj* unk4;
};

class TPollutionTexStamp {
public:
	void pushTask(u8, u16, u16, u16, short);
	void registerTexStamp(u16, u16, ResTIMG*);
	TPollutionTexStamp();

public:
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ ResTIMG* unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ int unkC;
	/* 0x10 */ TPollutionLayerTaskInfo* unk10;
};

class TPollutionRevivalTexStamp {
public:
	void registerTex(int, short, short, short, short, int, ResTIMG*);
	TPollutionRevivalTexStamp();

public:
	/* 0x0 */ int unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ s16 unk8;
	/* 0xA */ s16 unkA;
	/* 0xC */ s16 unkC;
	/* 0xE */ s16 unkE;
	/* 0x10 */ ResTIMG* unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ int unk18;
};

class TPollutionCounterLayer : public TPollutionCounterBase {
public:
	TPollutionCounterLayer();

	virtual int getTokenNo(int param_1) const { return param_1 + 0x7E; }
	virtual u32 getCounterNo(u32 param_1) const { return param_1 - 0x7E; }

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

public:
	/* 0x14 */ const TPollutionLayer** unk14;
	/* 0x18 */ u16 unk18;
	/* 0x1A */ u16 unk1A;
	/* 0x1C */ TPollutionTexStamp* unk1C;
	/* 0x20 */ u16 unk20;
	/* 0x22 */ u16 unk22;
	/* 0x24 */ TPollutionRevivalTexStamp* unk24;
	/* 0x28 */ u16 unk28;
	/* 0x2C */ J3DDrawBuffer** unk2C;
	/* 0x30 */ u16* unk30;
	/* 0x34 */ TPollutionModelTaskInfo unk34[20];
	/* 0xD4 */ u16 unkD4;
	/* 0xD8 */ TPollutionJointObjTaskInfo unkD8[20];
	/* 0x178 */ u8* unk178;
};

#endif
