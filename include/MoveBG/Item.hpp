#ifndef MOVE_BG_ITEM_HPP
#define MOVE_BG_ITEM_HPP

#include <stdint.h>
#include <MoveBG/MapObjGeneral.hpp>

class TMirrorActor;

class TItem : public TMapObjGeneral {
public:
	TItem(const char* name = "アイテム");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();
	virtual void appear();
	virtual void initMapObj();
	virtual void calc();
	virtual void touchPlayer(THitActor*);
	virtual void appearing();
	virtual void appeared();
	virtual void killByTimer(int);
	virtual void taken(THitActor*);

	static f32 mAppearedScaleSpeed;

	void setContainer(THitActor* container) { mContainer = container; }

public:
	/* 0x148 */ THitActor* mContainer;
	/* 0x14C */ u32 mDisappearAfterFrames;
	/* 0x150 */ u32 mUntouchableFrames;
};

class TCoin : public TItem {
public:
	TCoin(const char* name = "コイン");

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void appear();
	virtual void makeObjAppeared();
	virtual void makeObjDead();
	virtual void initMapObj();
	virtual void taken(THitActor*);

	void appearWithoutSound();

private:
	/* 0x154 */ TMirrorActor* mMirrorActor;
};

class TFlowerCoin : public TCoin {
public:
	TFlowerCoin(const char* name = "コイン(フラワー用)")
	    : TCoin(name)
	    , mCoinUnitIndex(0)
	{
	}

	virtual void load(JSUMemoryInputStream&);

public:
	/* 0x158 */ s32 mCoinUnitIndex;
};

class TCoinEmpty : public TCoin {
public:
	TCoinEmpty(const char* name = "空コイン");

	virtual void kill();
	virtual void appear();
	virtual void makeObjAppeared();

	void warning();
};

class TCoinRed : public TCoin {
public:
	TCoinRed(const char* name = "赤コイン");
	virtual void taken(THitActor*);

	const JGeometry::TVec3<f32>& getUnk158() { return unk158; }

public:
	/* 0x158 */ JGeometry::TVec3<f32> unk158;
};

class TCoinBlue : public TCoin {
public:
	TCoinBlue(const char* name = "青コイン");

	virtual void load(JSUMemoryInputStream&);
	virtual void makeObjAppeared();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void taken(THitActor*);
};

class TShine : public TItem {
public:
	TShine(const char* name = "シャイン");

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void kill();
	virtual void initMapObj();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void makeMActors();
	virtual void calc();
	virtual void touchPlayer(THitActor*);

	void appearWithDemo(const char*);
	void appearSimple(int);
	static s32 appearWithTimeCallback(uintptr_t, u32);
	void appearWithTime(int time, int appear_wait_time, int move_up_time,
	                    int float_time);
	void movingDown();
	void movingUp();
	void movingCircle();

	static int mPromiLife[4];
	static f32 mSenkoRate[4];
	static f32 mKiraRate[4];
	static f32 mBowRate[4];
	static f32 mCircleRateY;
	static f32 mUpSpeed;
	static f32 mSpeedDownRate;

	enum {
		STATE_APPEAR_WAIT   = 0xB,
		STATE_MOVING_UP     = 0xC,
		STATE_MOVING_CIRCLE = 0xD,
		STATE_MOVING_DOWN   = 0xE,
		STATE_FLOAT         = 0xF,
		STATE_GOT           = 0x10,
		STATE_WAIT          = 0x11,
		STATE_QUICKLY_WAIT  = 0x12,
	};

	enum {
		APPEAR_TYPE_NORMAL  = 0,
		APPEAR_TYPE_DEAD    = 1,
		APPEAR_TYPE_QUICKLY = 2,
		APPEAR_TYPE_SIMPLE  = 3,
	};

public:
	/* 0x154 */ u32 mAppearType;
	/* 0x158 */ f32 mCircleAngle;
	/* 0x15C */ f32 mCircleRadius;
	/* 0x160 */ f32 mCircleHeight;
	/* 0x164 */ f32 mCircleStartY;
	/* 0x168 */ int mCircleTime;
	/* 0x16C */ f32 mRotateSpeed;
	/* 0x170 */ int mMoveUpTime;
	/* 0x174 */ int mAppearWaitTime;
	/* 0x178 */ int mFloatTime;
	/* 0x17C */ JGeometry::TVec3<f32> mCircleStep;
	/* 0x188 */ f32 mVelocityY;
	/* 0x18C */ u32 mDemoLength;
	/* 0x190 */ u8 mIsInside;
	/* 0x194 */ JPABaseEmitter* mPromiEmitter;
	/* 0x198 */ JPABaseEmitter* mSenkoEmitter;
	/* 0x19C */ JPABaseEmitter* mKiraEmitter;
	/* 0x1A0 */ JPABaseEmitter* mBowEmitter;
	/* 0x1A4 */ u8 mEffectEmitted;
	/* 0x1A8 */ JGeometry::TVec3<f32> mEffectScale;
	/* 0x1B4 */ u8 mIsEmpty;
};

class MActor;

class TEggYoshi : public TMapObjGeneral {
public:
	TEggYoshi(const char* name = "ヨッシーの卵");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void touchActor(THitActor*);
	virtual u16 getHitObjNumMax() { return 10; }

	void startFruit();
	void touchFruit(THitActor*);
	void startBalloonAnim();
	void decideRandomLoveFruit();

	// fabricated
	THitActor* getFruit() { return unk150; }

private:
	/* 0x148 */ MActor* unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ THitActor* unk150;
};

class TItemNozzle : public TItem {
public:
	TItemNozzle()
	    : TItem("ノズル")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual void appearing();
	virtual void put();
};

class TNozzleBox : public TMapObjGeneral {
public:
	TNozzleBox(const char* name = "ノズルボックス");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void touchPlayer(THitActor*);
	virtual void breaking();

	void makeModelInvalid();
	void makeModelValid();

public:
	/* 0x148 */ int mContainedNozzleType;
	/* 0x14C */ TItemNozzle* mContainedNozzleItem;
	/* 0x150 */ f32 mDropSpeed;
	/* 0x154 */ f32 mDropYSpeed;
	/* 0x158 */ const char* mContainedNozzleName;
	/* 0x15C */ bool mIsValid;
	/* 0x15E */ GXColorS10 mBoxColor;
	/* 0x166 */ bool mBalloonShown;
};

#endif
