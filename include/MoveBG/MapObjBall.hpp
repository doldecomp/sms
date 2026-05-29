#ifndef MOVE_BG_MAP_OBJ_BALL_HPP
#define MOVE_BG_MAP_OBJ_BALL_HPP

#include <MoveBG/MapObjGeneral.hpp>
#include <Player/ModelWaterManager.hpp>

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
	virtual void getDepthAtFloating() { }
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

	inline void kick();

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
	/* 0x18C */ f32 unk18C;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ s32 unk194;
};

class TResetFruit : public TMapObjBall {
public:
	TResetFruit(const char* name = "無限フルーツ");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
	virtual u32 touchWater(THitActor*);
	// This definition is being occluded by TMapObjGeneral::getLivingTime for
	// some reason
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

	void touchKillSurface();

	inline void hideTouchActor(THitActor*);
	inline void unknownInline();

	static u32 mFruitWaitTimeToAppear;
	static f32 mScaleUpSpeed;
	static u32 mFruitLivingTime;
	static f32 mBreakingScaleSpeed;

	f32 unk198;
	GXColorS10 mFruitColor;
	u8 unk1A4;
};

class TRandomFruit : public TResetFruit {
public:
	TRandomFruit(const char* name = "ランダムフルーツ");
	virtual void initMapObj();

	char mFruitName[0x20];
};

class TCoverFruit : public TMapObjBase {
public:
	TCoverFruit(const char* name = "フタのフルーツ");
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

	TWaterEmitInfo* unk198;
	s32 unk19C;
	f32 unk1A0;
};

#endif
