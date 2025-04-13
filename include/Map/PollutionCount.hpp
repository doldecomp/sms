#ifndef MAP_POLLUTION_COUNT_HPP
#define MAP_POLLUTION_COUNT_HPP

#include <System/DrawSyncCallback.hpp>

class TPollutionObj;
class TPollutionLayer;
class J3DModel;
class J3DDrawBuffer;
class ResTIMG;

class TPollutionCounterBase : public TDrawSyncCallback {
public:
	TPollutionCounterBase();

	void setCallback(int) const;
	void drawSyncCallback(u16);
	void initCounters(int);

	virtual void getTokenNo(int) const   = 0;
	virtual void getCounterNo(u32) const = 0;

public:
	/* 0x4 */ u32 unk4;
	/* 0x8 */ u32 unk8;
	/* 0xC */ u32** unkC;
	/* 0x10 */ u32* unk10;
};

class TPollutionCounterObj : public TPollutionCounterBase {
public:
	TPollutionCounterObj();

	virtual void getTokenNo(int) const;
	virtual void getCounterNo(u32) const;

	void draw(int) const;
	void countObjDegree() const;
	void registerPollutionObj(TPollutionObj*, u32*);
	void init(int);

public:
	/* 0x14 */ TPollutionObj** unk14;
};

class TPollutionLayerTaskInfo {
public:
	TPollutionLayerTaskInfo();

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ TPollutionObj* unk4;
};

class TPollutionModelTaskInfo {
public:
	TPollutionModelTaskInfo();

public:
	/* 0x0 */ u8 unk0;
	/* 0x4 */ J3DModel* unk4;
};

class TPollutionJointObjTaskInfo {
public:
	TPollutionJointObjTaskInfo();
};

class TPollutionTexStamp {
	void pushTask(u8, u16, u16, u16, short);
	void registerTexStamp(u16, u16, ResTIMG*);
	TPollutionTexStamp();
};

class TPollutionRevivalTexStamp {
public:
	void registerTex(int, short, short, short, short, int, ResTIMG*);
	TPollutionRevivalTexStamp();
};

class TPollutionCounterLayer : public TPollutionCounterBase {
public:
	TPollutionCounterLayer();

	virtual void getTokenNo(int) const;
	virtual void getCounterNo(u32) const;

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
	void registerRevivalTexStamp(int, short, short, short, short, int,
	                             ResTIMG*);
	void registerTexStamp(u16, u16, ResTIMG*);
	void registerLayer(const TPollutionLayer*, u32*);
	void stampIsCleanType(u16) const;
	void onLayer(int);
	void offLayer(int);
	void init(int, u16, u16);

public:
	/* 0x14 */ TPollutionLayer** unk14;
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
