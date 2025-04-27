#ifndef MOVE_BG_MAP_OBJ_MARE_HPP
#define MOVE_BG_MAP_OBJ_MARE_HPP

#include <MoveBG/MapObjBase.hpp>

// TODO: mark virtual methods as such

struct TBGWallCheckRecord;

class TCogwheelScale : public TMapObjBase {
public:
	u32 touchWater(THitActor*);
	u32 receiveMessage(THitActor*, u32);
	void touchPlayer(THitActor*);
	void control();
	TCogwheelScale(const char*);
};

class TCogwheel : public TMapObjBase {
public:
	void initDraw() const;
	void draw() const;
	void rebound();
	void calc();
	void control();
	void initMapObj();
	TCogwheel(const char*);
};

class TMapObjElasticCode : public TMapObjBase {
public:
	void draw() const;
	void control();
	void initMapObj();
	TMapObjElasticCode();
};

class TMapObjGrowTree : public TMapObjBase {
public:
	void getGrowHeightFromRate(float) const;
	void updateHeight();
	u32 touchWater(THitActor*);
	void control();
	void loadAfter();
	void initMapObj();
	TMapObjGrowTree(const char*);
};

class TWireBell : public TMapObjBase {
public:
	void initDraw() const;
	void draw() const;
	void control();
	void loadAfter();
	TWireBell(const char*);
};

class TMapObjPuncher : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	void control();
	void load(JSUMemoryInputStream&);
	TMapObjPuncher();
};

class TMuddyBoat : public TMapObjBase {
public:
	void moveByWater();
	void calcRootMatrix();
	void kill();
	void touchWall(JGeometry::TVec3<float>*, const TBGWallCheckRecord&);
	void bindToWall(const JGeometry::TVec3<float>&, float,
	                JGeometry::TVec3<float>*);
	void bind();
	void control();
	void calc();
	u32 getSDLModelFlag() const;
	void initMapObj();
	TMuddyBoat(const char*);
};

class TMareFall : public TMapObjBase {
public:
	void calc();
	void load(JSUMemoryInputStream&);
	TMareFall();
};

class TMareCork : public TMapObjBase {
public:
	void loadAfter();
	void moveObject();
	void calcRootMatrix();
	MtxPtr getTakingMtx();
	void drawObject(JDrama::TGraphics*);
	TMareCork();
};

class TMareEventPoint : public THitActor {
public:
	u32 receiveMessage(THitActor*, u32);
	void load(JSUMemoryInputStream&);
	TMareEventPoint();
};

#endif
