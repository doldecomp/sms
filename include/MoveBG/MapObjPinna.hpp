#ifndef MOVE_BG_MAP_OBJ_PINNA_HPP
#define MOVE_BG_MAP_OBJ_PINNA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjTown.hpp>

// TODO: mark virtual methods as such

class TCoin;

class TFerrisWheel : public TMapObjBase {
public:
	void becomeCalmlyCallback(u32, u32);
	void control();
	void initMapObj();
	TFerrisWheel(const char*);
};

class THorizontalViking : public TMapObjBase {
public:
	void updateTrans();
	void moveNormal();
	void control();
	void reset();
	void initMapObj();
	THorizontalViking(const char*);
};

class TViking : public THorizontalViking {
public:
	void roll();
	void control();
	void reset();
	void loadAfter();
	void initMapObj();
	TViking(const char*);
};

class TPinnaShell : public THitActor {
public:
	void opened();
	u32 receiveMessage(THitActor*, u32);
	void control();
	TPinnaShell(const char*);
	TPinnaShell();
};

class TShellCup : public TMapObjBase {
public:
	void control();
	void attachCoin(TCoin*, int);
	void calcAfter();
	void perform(u32, JDrama::TGraphics*);
	void loadAfter();
	void initMapObj();
	TShellCup(const char*);
};

class TMerrygoround : public TMapObjBase {
public:
	void control();
	void draw() const;
	void initMapObj();
	TMerrygoround(const char*);
};

class TChangeStageMerrygoround : public TMapObjChangeStage {
public:
	void touchPlayer(THitActor*);
	void calc();
};

class TBalloonKoopaJr : public TMapObjGeneral {
public:
	void touchActor(THitActor*);
	void kill();
	void load(JSUMemoryInputStream&);
	TBalloonKoopaJr();
};

class TPinnaEntrance : public TMapObjBase {
public:
	void loadAfter();
	TPinnaEntrance();
};

class TWaterRecoverObj : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	TWaterRecoverObj();
};

class TAmiKing : public TMapObjBase {
public:
	u32 touchWater(THitActor*);
	void loadAfter();
	void initMapObj();
	void moveObject();
	void calcRootMatrix();
	void bind();
	void touchPlayer(THitActor*);
	TAmiKing();
};

class TPinnaCoaster : public TMapObjBase {
public:
	void control();
	void initMapObj();
	TPinnaCoaster(const char*);
};

class TMerryPole : public TMapObjBase {
public:
	Mtx* getRootJointMtx() const;
	TMerryPole();
};

#endif
