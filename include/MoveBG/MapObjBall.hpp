#ifndef MOVE_BG_MAP_OBJ_BALL_HPP
#define MOVE_BG_MAP_OBJ_BALL_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMapObjBall : public TMapObjGeneral {
public:
	TMapObjBall(const char*);

	virtual u32 receiveMessage(THitActor*, u32);
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
};

class TResetFruit : public TMapObjBall {
public:
	TResetFruit(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual void touchActor(THitActor*);
	virtual u32 touchWater(THitActor*);
	virtual u32 getLivingTime() const;
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

	u32 mFruitLivingTime;
	u32 mScaleUpSpeed;
	u32 mRottingScaleSpeed;
	u32 mBreakingScaleSpeed;
	u32 mFruitWaitTimeToAppear;
	u32 mRottenColor;
};

class TRandomFruit : public TResetFruit {
public:
	TRandomFruit(const char*);
	virtual void initMapObj();
};

class TCoverFruit : public TMapObjBase {
public:
	TCoverFruit();
	virtual void loadAfter();
	virtual u32 receiveMessage(THitActor*, u32);

	void calcRootMatrix();
};

class TBigWatermelon : public TMapObjBall {
public:
	TBigWatermelon(const char*);

	virtual void loadAfter();
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control() { }
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
};

#endif
