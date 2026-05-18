#ifndef MOVE_BG_MAP_OBJ_BLOCK_HPP
#define MOVE_BG_MAP_OBJ_BLOCK_HPP

#include <MoveBG/MapObjHide.hpp>
#include <MoveBG/MapObjGeneral.hpp>

#include <MarioUtil/MathUtil.hpp>
#include <MSound/MSound.hpp>

// TODO: mark virtual methods as such
class TSmallEnemy;

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
	// Fabricated
	typedef enum ESandBlockStates {
		Waiting   = 1,
		Restoring = 2,
		Touched   = 3,
		Falling   = 4,
		Gone      = 5
	};

	TSandBlock(const char* name = "砂ブロック");
	void initMapObj();
	void control();
	void touchPlayer(THitActor*);

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
	void initMapObj();
	void calc();
	void control();

	u32 touchWater(THitActor*);
	u32 getSDLModelFlag() const;

	static f32 mMeltSpeedWater;
	static f32 mMeltSpeedAuto;
	static f32 mAutoMeltScale;
};

class TBrickBlock : public THideObjBase {
public:
	TBrickBlock(const char* name = "レンガブロック");
	void initMapObj();
	BOOL receiveMessage(THitActor* sender, u32 message);
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

	void setGroundCollision();
	void perform(u32, JDrama::TGraphics*);
	void initMapObj();
};

class TSuperHipDropBlock : public TBreakHideObj {
public:
	TSuperHipDropBlock(const char* name = "スーパーヒップドロップブロック");
	void loadAfter();
	BOOL receiveMessage(THitActor* sender, u32 message);

public:
	/* 0x150 */ bool mMonteBlockBroken;
};

#endif
