#ifndef MOVE_BG_MAP_OBJ_GENERAL_HPP
#define MOVE_BG_MAP_OBJ_GENERAL_HPP

#include <MoveBG/MapObjBase.hpp>

struct TBGWallCheckRecord;

class TMapObjGeneral : public TMapObjBase {
public:
	void initPhysicalData();

	TMapObjGeneral(const char*);

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void ensureTakeSituation();
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual void kill();
	virtual void appear();
	virtual void initMapObj();
	virtual void checkIllegalAttr() const;
	virtual void touchPlayer(THitActor*);
	virtual u32 touchWater(THitActor*);
	virtual u32 getLivingTime() const { }
	virtual int getFlushTime() const { }
	virtual bool isPollutedGround(const JGeometry::TVec3<f32>&) const;
	virtual void work();
	virtual void appearing();
	virtual void appeared();
	virtual void breaking();
	virtual void sinking();
	virtual void holding();
	virtual void waitingToRecover();
	virtual void recovering();
	virtual void waitingToAppear();
	virtual void touchingPlayer();
	virtual void touchingWater();
	virtual void hold(TTakeActor*);
	virtual void put();
	virtual void thrown();
	virtual void sink();
	virtual void recover();
	virtual void waitToAppear(s32);
	virtual void makeObjBuried();
	virtual void makeObjRecovered();
	virtual void receiveMessageFromPlayer();
	virtual void calcVelocity();
	virtual void checkGroundCollision(JGeometry::TVec3<f32>*);
	virtual void touchGround(JGeometry::TVec3<f32>*);
	virtual void checkWallCollision(JGeometry::TVec3<f32>*);
	virtual void touchWall(JGeometry::TVec3<f32>*, TBGWallCheckRecord*);
	virtual void checkRoofCollision(JGeometry::TVec3<f32>*);
	virtual void touchRoof(JGeometry::TVec3<f32>*);

	static struct TMapObjSoundData mDefaultSound;

	static u32 mNormalLivingTime;
	static u32 mNormalFlushTime;
	static int mNormalFlushInterval;
	static u32 mNormalWaitToAppearTime;
	static f32 mNormalAppearingScaleUp;
	static f32 mNormalThrowSpeedRate;

public:
	/* 0x138 */ const TBGCheckData* unk138; // TODO: maybe not const
	/* 0x13C */ const TBGCheckData* unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
};

#endif
