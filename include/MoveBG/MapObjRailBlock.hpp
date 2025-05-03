#ifndef MOVE_BG_MAP_OBJ_RAIL_BLOCK_HPP
#define MOVE_BG_MAP_OBJ_RAIL_BLOCK_HPP

#include <MoveBG/MapObjBase.hpp>

class TGraphWeb;

class TRailMapObj : public TMapObjBase {
public:
	TRailMapObj(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 getShadowType();
	virtual void initMapObj();
	virtual void calcRecycle();
	virtual void resetPosition();
	virtual void readRailFlag();

	void initGraphTracer(TGraphWeb*);
	void resetStep(f32);
	void moveToNextNode(f32);
	void checkMarioRiding();
};

class TNormalLift : public TRailMapObj {
public:
	TNormalLift(const char*);
	void resetPosition();
	void load(JSUMemoryInputStream&);
	void readRailFlag();
	void initMapObj();
	void control();
	void setGroundCollision();
	void perform(u32, JDrama::TGraphics*);
};

class TRailBlock : public TRailMapObj {
public:
	TRailBlock(const char*);
	void initMapObj();
	Mtx* getRootJointMtx() const;
	void calcRootMatrix();
	void control();
};

class TRollBlock : public TMapObjBase {
public:
	TRollBlock(const char*);
	void load(JSUMemoryInputStream&);
	void setGroundCollision();
	Mtx* getRootJointMtx() const;
	void calcRootMatrix();
	void control();
};

class TWoodBlock : public TNormalLift {
public:
	TWoodBlock(const char*);
	void calcRecycle();
	void load(JSUMemoryInputStream&);
};

#endif
