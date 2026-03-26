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
	TFerrisWheel(const char* name = "観覧車");
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
	TViking(const char* name = "バイキング");
};

class TPinnaShell : public THitActor {
public:
	void opened();
	BOOL receiveMessage(THitActor* sender, u32 message);
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
	TShellCup(const char* name = "シェルカップ");
};

class TMerrygoround : public TMapObjBase {
public:
	void control();
	void draw() const;
	void initMapObj();
	TMerrygoround(const char* name = "メリーゴーランド");
};

class TChangeStageMerrygoround : public TMapObjChangeStage {
public:
	void touchPlayer(THitActor*);
	void calc();

	TChangeStageMerrygoround()
	    : TMapObjChangeStage("ステージ切り替え（メリーゴーランド用）")
	    , unk13C(0)
	{
	}

public:
	/* 0x13C */ u8 unk13C;
};

class TBalloonKoopaJr : public TMapObjGeneral {
public:
	void touchActor(THitActor*);
	void kill();
	void load(JSUMemoryInputStream&);
	TBalloonKoopaJr(const char* name = "風船（クッパＪｒ）");
};

class TPinnaEntrance : public TMapObjBase {
public:
	void loadAfter();
	TPinnaEntrance(const char* name = "ピンナ入り口");
};

class TWaterRecoverObj : public TMapObjBase {
public:
	void touchPlayer(THitActor*);
	TWaterRecoverObj(const char* name = "水回復オブジェ");
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
	TAmiKing(const char* name = "アミキング");
};

class TPinnaCoaster : public TMapObjBase {
public:
	void control();
	void initMapObj();
	TPinnaCoaster(const char* name = "コースター");
};

class TMerryPole : public TMapObjBase {
public:
	virtual Mtx* getRootJointMtx() const { return (Mtx*)unk138.mMtx; }

	TMerryPole()
	    : TMapObjBase("メリーゴーランド用ポール")
	{
		unk138.identity();
	}

public:
	/* 0x138 */ TMtx34f unk138;
};

#endif
