#ifndef MOVE_BG_MAP_OBJ_BLOCK_HPP
#define MOVE_BG_MAP_OBJ_BLOCK_HPP

#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjGeneral.hpp>

// TODO: mark virtual methods as such

class TBreakableBlock : public TMapObjGeneral {
public:
	TBreakableBlock()
	    : TMapObjGeneral("壊れるブロック")
	{
	}
	void touchPlayer(THitActor*);
};

class TSandBlock : public TMapObjBase {
public:
	TSandBlock();
	void initMapObj();
	void control();
	void touchPlayer(THitActor*);
};

class TLeanBlock : public TMapObjBase {
public:
	TLeanBlock(const char*);

	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual void calcDefaultMtx();

	void calcLeanMtx(MtxPtr);

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ JGeometry::TVec3<f32> unk14C;
	/* 0x158 */ JGeometry::TVec3<f32> unk158;
	/* 0x164 */ Mtx unk164;
};

class TIceBlock : public TMapObjBase {
public:
	TIceBlock();
	void initMapObj();
	void calc();
	void control();

	u32 touchWater(THitActor*);
	u32 getSDLModelFlag() const;
};

class TBrickBlock : public THideObjBase {
public:
	TBrickBlock();
	void initMapObj();
	u32 receiveMessage(THitActor*, u32);
	void kill();
};

class TJuiceBlock : public TMapObjBase {
public:
	TJuiceBlock(const char* name = "ジュースブロック")
	    : TMapObjBase(name)
	{
		unk14C = 0;
		unk140.set(1.0f, 1.0f, 1.0f);
	}

	void kill();
	void moveObject();
	void initMapObj();

public:
	/* 0x138 */ char unk138[0x8];
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
	/* 0x14C */ u32 unk14C;
};

class TTelesaBlock : public TJuiceBlock {
public:
	TTelesaBlock()
	    : TJuiceBlock("テレサブロック")
	{
	}

	void setGroundCollision();
	void perform(u32, JDrama::TGraphics*);
	void initMapObj();
};

class TSuperHipDropBlock : public TBreakHideObj {
public:
	void loadAfter();
	u32 receiveMessage(THitActor*, u32);
};

#endif
