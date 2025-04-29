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
	void initMapObj();
	void calcDefaultMtx();
	void calcLeanMtx(MtxPtr);
	void control();
	void touchPlayer(THitActor*);
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
