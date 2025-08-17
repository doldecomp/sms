#ifndef MOVE_BG_WOOD_BARREL_HPP
#define MOVE_BG_WOOD_BARREL_HPP

#include <MoveBG/MapObjGeneral.hpp>
#include <Player/ModelWaterManager.hpp>

class TWoodBarrel : public TMapObjGeneral {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void kill();
	virtual void appear();
	virtual void touchActor(THitActor*);
	virtual void touchPlayer(THitActor*);
	virtual int getFlushTime() const { return mOilBarrelFlushTime; }
	virtual void appeared();
	virtual void breaking();
	virtual void hold(TTakeActor*);
	virtual void put();
	virtual void touchGround(JGeometry::TVec3<float>*);
	virtual void touchWall(JGeometry::TVec3<float>*, TBGWallCheckRecord*);

	TWoodBarrel(const char*);

	static s32 mWaitAppearTime;
	static int mBreakTime;
	static int mOilBarrelFlushTime;

public:
	/* 0x148 */ TWaterEmitInfo* unk148;
};

#endif
