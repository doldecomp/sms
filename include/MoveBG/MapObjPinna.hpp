#ifndef MOVE_BG_MAP_OBJ_PINNA_HPP
#define MOVE_BG_MAP_OBJ_PINNA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjGeneral.hpp>
#include <MoveBG/MapObjTown.hpp>

class TCoin;

class TFerrisWheel : public TMapObjBase {
public:
	static s32 becomeCalmlyCallback(uintptr_t, u32);
	virtual void control();
	virtual void initMapObj();
	TFerrisWheel(const char* name = "観覧車");

	enum {
		STATE_UNK2 = 0x2,
	};

public:
	/* 0x138 */ s32 unk138;
	/* 0x13C */ TMapObjBase** unk13C;
	/* 0x140 */ f32 unk140;
};

class THorizontalViking : public TMapObjBase {
public:
	void updateTrans();
	void moveNormal();
	virtual void control();
	virtual void initMapObj();
	virtual void reset();
	THorizontalViking(const char*);

	enum {
		STATE_UNK2 = 0x2,
	};

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ f32 unk148;
};

class TViking : public THorizontalViking {
public:
	void roll();
	virtual void loadAfter();
	virtual void control();
	virtual void initMapObj();
	virtual void reset();
	TViking(const char* name = "バイキング");

	enum {
		MODE_UNK0 = 0,
		MODE_UNK1 = 1,
	};

	enum {
		STATE_UNK3 = 0x3,
		STATE_UNK4 = 0x4,
	};

public:
	/* 0x14C */ s32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
};

class TPinnaShell : public THitActor {
public:
	void opened();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	void control();
	TPinnaShell(const char* name = "シェル");

	enum {
		STATE_UNK0 = 0x0,
		STATE_UNK1 = 0x1,
		STATE_UNK2 = 0x2,
		STATE_UNK3 = 0x3,
	};

public:
	/* 0x68 */ s32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ MtxPtr unk74;
	/* 0x78 */ J3DJoint* unk78;
	/* 0x7C */ s32 unk7C;
	/* 0x80 */ TLiveActor* unk80;
	/* 0x84 */ TMapCollisionMove* unk84;
	/* 0x88 */ TDamageObj* unk88;
	/* 0x8C */ class TShellCup* unk8C;
};

class TShellCup : public TMapObjBase {
public:
	void attachCoin(TCoin*, int);
	void calcAfter();
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void control();
	virtual void initMapObj();
	TShellCup(const char* name = "シェルカップ");

public:
	/* 0x138 */ TPinnaShell unk138[6];
	/* 0x498 */ TCoin* unk498;
	/* 0x49C */ TCoin* unk49C;
	/* 0x4A0 */ TCoin* unk4A0;

	static f32 mOpenRotMax;
	static f32 mShellDamageRot;
	static f32 mWaterOpenAccel;
	static f32 mCloseAccel;
};

class TMerrygoround : public TMapObjBase {
public:
	virtual void control();
	virtual void initMapObj();
	virtual void draw() const;
	TMerrygoround(const char* name = "メリーゴーランド");

	static f32 mRotSpeed;

public:
	/* 0x138 */ TMapObjBase* unk138[2];
	/* 0x140 */ u16 unk140[2];
	/* 0x144 */ class TMerryPole* unk144[9];
	/* 0x168 */ TMapCollisionMove* unk168[9];
	/* 0x18C */ u16 unk18C[9];
	/* 0x1A0 */ TMapObjChangeStage* unk1A0;
	/* 0x1A4 */ u16 unk1A4;
};

class TChangeStageMerrygoround : public TMapObjChangeStage {
public:
	virtual void calc();
	virtual void touchPlayer(THitActor*);

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
	virtual void load(JSUMemoryInputStream&);
	virtual void kill();
	virtual void touchActor(THitActor*);
	TBalloonKoopaJr(const char* name = "風船（クッパＪｒ）")
	    : TMapObjGeneral(name)
	{
		unk148.zero();
	}

public:
	/* 0x148 */ JGeometry::TVec3<f32> unk148;
};

class TPinnaEntrance : public TMapObjBase {
public:
	virtual void loadAfter();
	TPinnaEntrance(const char* name = "ピンナ入り口")
	    : TMapObjBase(name)
	{
	}
};

class TWaterRecoverObj : public TMapObjBase {
public:
	virtual void touchPlayer(THitActor*);
	TWaterRecoverObj(const char* name = "水回復オブジェ")
	    : TMapObjBase(name)
	{
	}
};

class TAmiKing : public TMapObjBase {
public:
	virtual void loadAfter();
#ifdef VERSION_GMSP01
	virtual void calc();
#else
	virtual void calcRootMatrix();
#endif
	virtual void bind();
	virtual void moveObject();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual u32 touchWater(THitActor*) { return 1; }
	TAmiKing(const char* name = "アミキング")
	    : TMapObjBase(name)
	    , unk138(0)
	{
	}

public:
	/* 0x138 */ u8 unk138;
	/* 0x139 */ u8 unk139[3];
	/* 0x13C */ JGeometry::TVec3<f32> unk13C;
};

class TPinnaCoaster : public TMapObjBase {
public:
	virtual void control();
	virtual void initMapObj();
	TPinnaCoaster(const char* name = "コースター");

public:
	/* 0x138 */ MActor* unk138;
	/* 0x13C */ s32 unk13C;
	/* 0x140 */ JGeometry::TVec3<f32> unk140;
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
	/* 0x138 */ TPosition3f unk138;
};

#endif
