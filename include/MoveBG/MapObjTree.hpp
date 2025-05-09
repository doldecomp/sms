#ifndef MOVE_BG_MAP_OBJ_TREE_HPP
#define MOVE_BG_MAP_OBJ_TREE_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMapObjLeaf {
	TMapObjLeaf();
};

class TMapObjTree : public TMapObjGeneral {
public:
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void getRadiusAtY(float) const;
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	void controlLeaf(int);
	void initEach();
	TMapObjTree(const char*);

	static f32 mNearMiddle;
	static f32 mMiddleFar;
	static f32 mBananaTreeJumpPower;
};

class TMapObjTreeScale : public TMapObjGeneral {
public:
	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void startScaleUp();
	void beSmall();
	TMapObjTreeScale(const char*);

	static f32 mScaleSpeedY;
	static f32 mStatusChangeScaleY;
	static f32 mScaleSpeedXZ;
	static f32 mScaleMin;
};

#endif
