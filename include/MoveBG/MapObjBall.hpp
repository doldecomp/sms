#ifndef MOVE_BG_MAP_OBJ_BALL_HPP
#define MOVE_BG_MAP_OBJ_BALL_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TWaterEmitInfo;

class TMapObjBall : public TMapObjGeneral {
public:
	TMapObjBall(const char* name = "ボール");

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
	virtual u32 touchWater(THitActor*);
	virtual void makeObjDefault();
	virtual f32 getDepthAtFloating() { return mDepthAtFloating; }
	virtual void hold(TTakeActor*);
	virtual void put();
	virtual void touchGround(JGeometry::TVec3<f32>*);
	virtual void checkWallCollision(JGeometry::TVec3<f32>*);
	virtual void touchWall(JGeometry::TVec3<f32>*, TBGWallCheckRecord*);
	virtual void touchRoof(JGeometry::TVec3<f32>*);
	virtual void rebound(JGeometry::TVec3<f32>*);
	virtual void touchWaterSurface();
	virtual void touchPollution();
	virtual void kicked();
	virtual void calcCurrentMtx();

	void boundByActor(THitActor*);

	// Per-ball-kind physics tunables, all written by initMapObj from a
	// switch on mActorType. Names follow usage where it is unambiguous.
	/* 0x148 */ f32 unk148;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ f32 unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ f32 unk180;
	/* 0x184 */ f32 unk184;
	/* 0x188 */ f32 unk188;
	/// How deep the ball sits when floating on water.
	/* 0x18C */ f32 mDepthAtFloating;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ int unk194;
};

class TResetFruit : public TMapObjBall {
public:
	TResetFruit(const char* name = "無限フルーツ");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
	virtual u32 touchWater(THitActor*);
	virtual u32 getLivingTime() const { return mFruitLivingTime; }
	virtual void appearing();
	virtual void breaking();
	virtual void waitingToAppear();
	virtual void hold(TTakeActor*);
	virtual void thrown();
	virtual void checkGroundCollision(JGeometry::TVec3<f32>*);
	virtual void touchGround(JGeometry::TVec3<f32>*);
	virtual void touchWaterSurface();
	virtual void touchPollution();
	virtual void kicked();

	void killByTimer(int);
	void rotting();
	void waitEffect();
	void living();
	void pick(THitActor*);
	void makeObjLiving();
	void makeObjWaitingToAppear();

	// The map records all six of these as globals, not members.
	static u32 mFruitLivingTime;
	static f32 mScaleUpSpeed;
	static f32 mRottingScaleSpeed;
	static f32 mBreakingScaleSpeed;
	static u32 mFruitWaitTimeToAppear;
	static GXColorS10 mRottenColor;

	/* 0x198 */ f32 unk198;
	/* 0x19C */ GXColorS10 unk19C;
	/* 0x1A4 */ u8 unk1A4;
};

class TRandomFruit : public TResetFruit {
public:
	TRandomFruit(const char* name = "ランダムフルーツ");
	virtual void initMapObj();

	// initMapObj picks one of four fruit model names into here and points
	// unkF4 at it.
	/* 0x1A8 */ char mModelName[0x20];
};

class TCoverFruit : public TMapObjBase {
public:
	TCoverFruit(const char* name = "フタのフルーツ")
	    : TMapObjBase(name)
	{
	}
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void calcRootMatrix();
};

class TBigWatermelon : public TMapObjBall {
public:
	TBigWatermelon(const char* name = "お化けスイカ");

	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void kill();
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
	virtual void appearing();
	virtual void touchGround(JGeometry::TVec3<f32>*);
	virtual void checkWallCollision(JGeometry::TVec3<f32>*);
	virtual void touchWall(JGeometry::TVec3<f32>*, TBGWallCheckRecord*);
	virtual void rebound(JGeometry::TVec3<f32>*);
	virtual void touchWaterSurface();

	void startEvent();

	/* 0x198 */ TWaterEmitInfo* unk198;
	/* 0x19C */ int unk19C;
	/* 0x1A0 */ f32 unk1A0;
};

#endif
