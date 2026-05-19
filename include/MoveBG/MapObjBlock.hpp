#ifndef MOVE_BG_MAP_OBJ_BLOCK_HPP
#define MOVE_BG_MAP_OBJ_BLOCK_HPP

#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjGeneral.hpp>

#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>

class TSmallEnemy;

class TBreakableBlock : public TMapObjGeneral {
public:
	TBreakableBlock()
	    : TMapObjGeneral("壊れるブロック")
	{
	}
	virtual void touchPlayer(THitActor*);
};

class TSandBlock : public TMapObjBase {
public:
	// Fabricated
	enum EState {
		STATE_WAITING   = 1,
		STATE_RESTORING = 2,
		STATE_TOUCHED   = 3,
		STATE_FALLING   = 4,
		STATE_GONE      = 5
	};

	TSandBlock(const char* name = "砂ブロック");
	virtual void initMapObj();
	virtual void control();
	virtual void touchPlayer(THitActor*);

	static u32 mWaitTimeToFall;
	static f32 mSandScaleUp;
	static f32 mSandScaleDown;
	static f32 mSandScaleMin;
	static u32 mSandWaitTime;
};

class TLeanBlock : public TMapObjBase {
public:
	TLeanBlock(const char*);

	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual void calcDefaultMtx();

	void calcLeanMtx(MtxPtr);

	// Fabricated
	JGeometry::SMatrix34C<f32>& getDefaultMtx() { return mDefaultMtx; }

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
	/* 0x14C */ JGeometry::TVec3<f32> unk14C;
	/* 0x158 */ JGeometry::TVec3<f32> unk158;
	/* 0x164 */ JGeometry::SMatrix34C<f32> mDefaultMtx;
};

class TIceBlock : public TMapObjBase {
public:
	TIceBlock(const char* name = "アイスブロック");
	virtual void initMapObj();
	virtual void calc();
	virtual void control();

	u32 touchWater(THitActor*);
	u32 getSDLModelFlag() const;

	static f32 mMeltSpeedWater;
	static f32 mMeltSpeedAuto;
	static f32 mAutoMeltScale;
};

class TBrickBlock : public THideObjBase {
public:
	TBrickBlock(const char* name = "レンガブロック");
	virtual void initMapObj();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void kill();
};

class TJuiceBlock : public TMapObjBase {
public:
	TJuiceBlock(const char* name = "ジュースブロック")
	    : TMapObjBase(name)
	{
		unk14C = 0;
		unk140.set(1.0f, 1.0f, 1.0f);
	}

	virtual void kill();
	virtual void moveObject();
	virtual void initMapObj();

	JGeometry::TVec3<f32>& getUnk140() { return unk140; }

public:
	/* 0x138 */ GXColorS10 unk138;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
	/* 0x14C */ TSmallEnemy* unk14C;
};

class TTelesaBlock : public TJuiceBlock {
public:
	TTelesaBlock()
	    : TJuiceBlock("テレサブロック")
	{
	}

	virtual void setGroundCollision();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void initMapObj();
};

class TSuperHipDropBlock : public TBreakHideObj {
public:
	TSuperHipDropBlock(const char* name = "スーパーヒップドロップブロック");
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x150 */ bool mMonteBlockBroken;
};

#endif
