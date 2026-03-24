#ifndef MOVE_BG_MAP_OBJ_MARE_HPP
#define MOVE_BG_MAP_OBJ_MARE_HPP

#include <MoveBG/MapObjBase.hpp>

// TODO: mark virtual methods as such

struct TBGWallCheckRecord;

class TCogwheelScale : public TMapObjBase {
public:
	u32 touchWater(THitActor*);
	BOOL receiveMessage(THitActor* sender, u32 message);
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
	TCogwheel(const char* name = "天秤");
};

class TMapObjElasticCode : public TMapObjBase {
public:
	void draw() const;
	void control();
	void initMapObj();
	TMapObjElasticCode(const char* name = "ゴムひも");
};

class TMapObjGrowTree : public TMapObjBase {
public:
	void getGrowHeightFromRate(float) const;
	void updateHeight();
	u32 touchWater(THitActor*);
	void control();
	void loadAfter();
	void initMapObj();
	TMapObjGrowTree(const char* name = "もやしの木");
};

class TWireBell : public TMapObjBase {
public:
	void initDraw() const;
	void draw() const;
	void control();
	void loadAfter();
	TWireBell(const char* name = "ワイヤー鈴（紫）");
};

class TMapObjPuncher : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	void control();
	void load(JSUMemoryInputStream&);
	TMapObjPuncher(const char* name = "パンチャー");
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
	TMuddyBoat(const char* name = "どろの船");
};

class TMareFall : public TMapObjBase {
public:
	void calc();
	void load(JSUMemoryInputStream&);
	TMareFall(const char* name = "マーレ滝");
};

class TMareCork : public TMapObjBase {
public:
	void loadAfter();
	void moveObject();
	void calcRootMatrix();
	MtxPtr getTakingMtx();
	void drawObject(JDrama::TGraphics*);
	TMareCork(const char* name = "マーレコルク");
};

class TMareEventPoint : public THitActor {
public:
	BOOL receiveMessage(THitActor* sender, u32 message);
	void load(JSUMemoryInputStream&);
	TMareEventPoint(const char* name = "イベントポイント");
};

#endif
