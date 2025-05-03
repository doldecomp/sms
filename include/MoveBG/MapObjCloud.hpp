#ifndef MOVE_BG_MAP_OBJ_CLOUD_HPP
#define MOVE_BG_MAP_OBJ_CLOUD_HPP

#include <MoveBG/MapObjRailBlock.hpp>

class TRideCloud : public TRailMapObj {
	TRideCloud(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 getShadowType();
	virtual void initMapObj();
	virtual u32 touchWater(THitActor*);
};

#endif
