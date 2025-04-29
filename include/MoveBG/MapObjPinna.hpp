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
	virtual Mtx* getRootJointMtx() const { return (Mtx*)unk138.mMtx; }

	TMerryPole()
	    : TMapObjBase("メリーゴーランド用ポール")
	{
		// not TRotation3f::identity33 and not written in it's style...
		unk138.mMtx[1][0] = unk138.mMtx[2][0] = unk138.mMtx[0][1]
		    = unk138.mMtx[2][1] = unk138.mMtx[0][2] = unk138.mMtx[1][2]
		    = unk138.mMtx[0][3] = unk138.mMtx[1][3] = unk138.mMtx[2][3] = 0.0f;

		unk138.mMtx[0][0] = unk138.mMtx[1][1] = unk138.mMtx[2][2] = 1.0f;
	}

public:
	/* 0x138 */ TRotation3f unk138; // TODO: type likely wrong
};

#endif
