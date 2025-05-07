#ifndef MOVE_BG_MAP_OBJ_RAIL_BLOCK_HPP
#define MOVE_BG_MAP_OBJ_RAIL_BLOCK_HPP

#include <MoveBG/MapObjBase.hpp>

class TGraphWeb;
class TGraphTracer;

class TRailMapObj : public TMapObjBase {
public:
	TRailMapObj(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 getShadowType();
	virtual void initMapObj();
	virtual BOOL calcRecycle();
	virtual void resetPosition();
	virtual void readRailFlag();

	void initGraphTracer(TGraphWeb*);
	void resetStep(f32);
	BOOL moveToNextNode(f32);
	bool checkMarioRiding();

	// fabricated
	void onRailFlag(u32 flag) { unk140 |= flag; }
	void offRailFlag(u32 flag) { unk140 &= ~flag; }
	bool checkRailFlag(u32 flag) { return unk140 & flag ? TRUE : FALSE; }
	f32 getUnk144() const { return unk144; }

public:
	/* 0x138 */ TGraphTracer* unk138;
	/* 0x13C */ int unk13C;
	/* 0x140 */ u32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ s8 unk148;
	/* 0x14A */ volatile s16 unk14A; // TODO: fake or real?
	/* 0x14C */ s8 unk14C;
};

class TNormalLift : public TRailMapObj {
public:
	TNormalLift(const char*);
	void resetPosition();
	void load(JSUMemoryInputStream&);
	void readRailFlag();
	void initMapObj();
	void control();
	void setGroundCollision();
	void perform(u32, JDrama::TGraphics*);

public:
	/* 0x150 */ u16 unk150;
	/* 0x152 */ u16 unk152;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ s8 unk158;
};

class TRailBlock : public TRailMapObj {
public:
	TRailBlock(const char*);
	void initMapObj();
	Mtx* getRootJointMtx() const;
	void calcRootMatrix();
	void control();

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ JGeometry::TVec3<f32> unk15C;
	/* 0x168 */ JGeometry::TVec3<f32> unk168;
	/* 0x174 */ Mtx unk174;
};

class TRollBlock : public TMapObjBase {
public:
	TRollBlock(const char*);
	void load(JSUMemoryInputStream&);
	void setGroundCollision();
	Mtx* getRootJointMtx() const;
	void calcRootMatrix();
	void control();

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
};

class TWoodBlock : public TNormalLift {
public:
	TWoodBlock(const char*);
	BOOL calcRecycle();
	void load(JSUMemoryInputStream&);

public:
	/* 0x15C */ GXColorS10 unk15C;
	/* 0x164 */ GXColorS10 unk164;
};

#endif
