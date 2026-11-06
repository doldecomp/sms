#ifndef MAP_MAP_EVENT_DOLPIC_HPP
#define MAP_MAP_EVENT_DOLPIC_HPP

#include <Map/MapEvent.hpp>

class TMapObjBase;
class TMapCollisionMove;
class TMapCollisionWarp;

class TDolpicEventBiancoGate : public TMapEvent {
public:
	TDolpicEventBiancoGate(const char* name = "イベント（ビアンコゲート）");

	virtual void loadAfter();
	virtual bool isFinishedAll() const;
	virtual bool watch();
	virtual bool control();

private:
	/* 0x20 */ TMapObjBase* mKingStatue;
	/* 0x24 */ f32 mKingStatueRiseSpeed;
};

class TDolpicEventRiccoMammaGate : public TMapEvent {
public:
	TDolpicEventRiccoMammaGate(const char* name
	                           = "イベント（リコ、マンマゲート）");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual bool isFinishedAll() const;
	virtual bool watch();
	virtual bool control();

	void rising();

public:
	/* 0x20 */ J3DJoint* mEventBuildingJoint;
	/* 0x24 */ TMapCollisionMove* mRisingBuildingCollision;
	/* 0x28 */ TMapCollisionWarp* mRisenBuildingCollision;
	/* 0x2C */ int mTriggerFlag;
	/* 0x30 */ bool mBuildingAlreadyRevealed;
	/* 0x34 */ f32 mScaleUpSpeed;
	/* 0x38 */ int mDemoDuration;
	/* 0x3C */ int mRiseStartDelay;
	/* 0x40 */ int mRiseEndDelay;
	/* 0x44 */ int mDemoTimer;
	/* 0x48 */ JGeometry::TVec3<f32> mEffectPosition;
	/* 0x54 */ JGeometry::TVec3<f32> mMarioStartPos;
	/* 0x60 */ f32 mMarioStartAngle;
};

#endif
