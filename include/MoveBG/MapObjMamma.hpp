#ifndef MOVE_BG_MAP_OBJ_MAMMA_HPP
#define MOVE_BG_MAP_OBJ_MAMMA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBall.hpp>
#include <MoveBG/MapObjEx.hpp>

class J3DJoint;
class TJointModel;
class TJointObj;
class TMapCollisionMove;
class TMapObjFlag;
class JPABaseEmitter;

class TSandLeaf : public TMapObjBase {
public:
	TSandLeaf(const char* name = "すなやまの芽")
	    : TMapObjBase(name)
	    , unk138(nullptr)
	{
	}

	virtual void control();
	virtual u32 touchWater(THitActor*);

public:
	/* 0x138 */ class TSandBase* unk138;
};

class TSandBase : public TMapObjBase {
public:
	TSandBase(const char*);

	virtual void grow() = 0;
	virtual bool withering();

	bool isDown() const;

	enum {
		STATE_WITHERING = 0x2,
		STATE_WITHERED  = 0x3,
	};

	static s32 mWitherTime;
	static f32 mScaleMin;

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ s32 unk140;
	/* 0x144 */ TMapObjBase* unk144;
};

class TSandLeafBase : public TSandBase {
public:
	TSandLeafBase(const char* name = "すなやまの芽の土台")
	    : TSandBase(name)
	{
	}

	virtual void control();
	virtual void initMapObj();
	virtual void grow();

	enum {
		STATE_GROWN     = 0x4,
		STATE_SPROUTING = 0x5,
	};
};

class TSandBomb : public TSandLeaf {
public:
	TSandBomb()
	    : TSandLeaf("すなやま爆弾")
	    , unk13C(0)
	    , unk140(0)
	{
	}

	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual u32 getSDLModelFlag() const;
	virtual u32 touchWater(THitActor*);

public:
	/* 0x13C */ u32 unk13C;
	/* 0x140 */ u8 unk140;
};

class TSandBombBase : public TSandBase {
public:
	TSandBombBase(const char* name = "すなやま爆弾の土台");

	virtual void loadAfter();
	virtual void control();
	virtual void initMapObj();
	virtual void grow();
	virtual void waitBeforeExplode();
	virtual void explode();
	virtual void exploding();
	virtual void expanded();
	virtual void withered();
	virtual TMapObjBase* findTriggerActor();

	enum {
		STATE_GROW                = 0x5,
		STATE_WAIT_BEFORE_EXPLODE = 0x6,
		STATE_EXPLODING           = 0x7,
		STATE_EXPANDED            = 0x8,
	};

	bool isFootHandOrStairs() const
	{
		return isActorType(0x400000CE) ? true : false;
	}

	static f32 mFiringFrameSpeed;
	static f32 mFiringFrameDownSpeed;
	static f32 mExplodeFrameSpeed;
	static f32 mMarioJumpRate;
	static s32 mExlodingRumbleTime;

public:
	/* 0x148 */ s32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
};

class TSandCastle : public TSandBombBase {
public:
	TSandCastle(const char* name = "砂の城");

	virtual void loadAfter();
	virtual void calcRootMatrix();
	virtual void initMapObj();
	virtual bool withering();
	virtual void waitBeforeExplode();
	virtual void explode();
	virtual void expanded();
	virtual TMapObjBase* findTriggerActor();

	static f32 mCollisionRate;

public:
	/* 0x158 */ TMapObjBase* unk158;
	/* 0x15C */ u8 unk15C;
};

class TShiningStone;

class TLeanMirror : public TMapObjBase {
public:
	TLeanMirror(const char* name = "ぐらぐら鏡");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual u32 getSDLModelFlag() const;
	virtual void draw() const;
	virtual void touchPlayer(THitActor*);
	virtual void touchEnemy(THitActor*);

	bool enemyIsOn() const;
	void updateSpeedVec(const JGeometry::TVec3<f32>&, f32);
	void calcCurrentMtx(MtxPtr);
	void release();
	void controlGoTarget();
	void controlShake();

	enum {
		STATE_GO_TARGET  = 0x2,
		STATE_LIGHT_DEMO = 0x3,
		STATE_DONE       = 0x4,
	};

	static s32 mGoTargetTime;
	static s32 mDemoWaitTime;
	static s32 mDemoLightTime;

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
	/* 0x14C */ JGeometry::TVec3<f32> unk14C;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ f32 unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ TShiningStone* unk17C;
	/* 0x180 */ JGeometry::TVec3<f32> unk180;
	/* 0x18C */ JGeometry::TVec3<f32> unk18C;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ u8 unk1AC;
	/* 0x1AE */ s16 unk1AE;
};

class TShiningStone : public THitActor {
public:
	TShiningStone(const char* name = "太陽石");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void putOnLight(TLiveActor*);
	void endDemo();

public:
	/* 0x68 */ MActor** unk68;
	/* 0x6C */ MActor* unk6C;
	/* 0x70 */ u8 unk70;
	/* 0x71 */ u8 unk71;
	/* 0x72 */ u8 unk72;
	/* 0x73 */ u8 unk73;
	/* 0x74 */ s32 unk74;
	/* 0x78 */ JPABaseEmitter* unk78;
	/* 0x7C */ f32 unk7C;
};

class TMammaBlockRotate : public TMapObjBase {
public:
	TMammaBlockRotate(const char* name = "太陽の塔ブロック");

	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);

	enum {
		STATE_MAP_GO   = 0x2,
		STATE_MAP_WAIT = 0x3,
		STATE_MAP_BACK = 0x4,
	};

	static f32 mRotSpeed;
	static f32 mRotReturnSpeed;
	static f32 mRotEnd;
	static f32 mMapGoSpeed;
	static f32 mMapBackSpeed;
	static s32 mWaitTime;

public:
	/* 0x138 */ TJointModel* unk138;
	/* 0x13C */ TJointObj* unk13C;
	/* 0x140 */ TJointObj* unk140;
	/* 0x144 */ TMapCollisionMove* unk144;
	/* 0x148 */ TMapCollisionMove* unk148;
};

class TMammaYacht : public TMapObjBase {
public:
	TMammaYacht(const char* name = "砂の城")
	    : TMapObjBase(name)
	{
	}

	virtual void control();
	virtual void initMapObj();

public:
	/* 0x138 */ TMapObjFlag* unk138;
};

class TSandBird : public TJointCoin {
public:
	TSandBird(const char* name = "おおすな鳥");

	virtual void control();
	virtual void initMapObj();
	virtual bool nameIsObj(const char*);
	virtual TMapObjBase* makeObjFromJointName(const char*, u16);

public:
	/* 0x148 */ u32 unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ u8 unk150;
	/* 0x151 */ u8 unk151;
};

class TWatermelon : public TMapObjBall {
public:
	virtual void control();
};

class TGoalWatermelon : public TMapObjBase {
public:
	TGoalWatermelon(const char* name = "スイカゴール");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void control();
	virtual void touchActor(THitActor*);

	enum {
		STATE_SHRINK = 0x2,
		STATE_SHINE  = 0x3,
	};

public:
	/* 0x138 */ TMapObjBase* unk138;
	/* 0x13C */ TMapObjBase* unk13C;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
};

class TMammaMirrorMapOperator : public JDrama::TViewObj {
public:
	TMammaMirrorMapOperator(const char* name = "鏡内地形操作");

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void show(int);
	void hide(int);

public:
	/* 0x10 */ J3DJoint* unk10[8];
	/* 0x30 */ JGeometry::TVec3<f32> unk30[8];
	/* 0x90 */ f32 unk90[8];
	/* 0xB0 */ u8 unkB0[8];
	/* 0xB8 */ JGeometry::TVec3<f32> unkB8[3];
};

class TSandEgg : public TMapObjBase {
public:
	TSandEgg(const char* name = "すなのたまご")
	    : TMapObjBase(name)
	{
	}

	virtual u32 getSDLModelFlag() const;
};

#endif
