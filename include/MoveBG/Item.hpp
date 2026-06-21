#ifndef MOVE_BG_ITEM_HPP
#define MOVE_BG_ITEM_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMirrorActor;

class TItem : public TMapObjGeneral {
public:
	TItem(const char* name = "アイテム");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();
	virtual void appear();
	virtual void initMapObj();
	virtual void calc();
	virtual void touchPlayer(THitActor*);
	virtual void appearing();
	virtual void appeared();
	virtual void killByTimer(int);
	virtual void taken(THitActor*);

	static f32 mAppearedScaleSpeed;

	void setContainer(THitActor* container) { mContainer = container; }

public:
	/* 0x148 */ THitActor* mContainer;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ u32 unk150;
};

class TCoin : public TItem {
public:
	TCoin(const char* name = "コイン");

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void appear();
	virtual void makeObjAppeared();
	virtual void makeObjDead();
	virtual void initMapObj();
	virtual void taken(THitActor*);

	void appearWithoutSound();

public:
	/* 0x154 */ TMirrorActor* unk154;
};

class TFlowerCoin : public TCoin {
public:
	TFlowerCoin(const char* name = "コイン(フラワー用)");

	virtual void load(JSUMemoryInputStream&);

public:
	/* 0x158 */ u32 unk158;
};

class TCoinEmpty : public TCoin {
public:
	TCoinEmpty(const char* name = "空コイン");

	virtual void kill();
	virtual void appear();
	virtual void makeObjAppeared();

	void warning();
};

class TCoinRed : public TCoin {
public:
	TCoinRed(const char* name = "赤コイン");
	virtual void taken(THitActor*);
};

class TCoinBlue : public TCoin {
public:
	TCoinBlue(const char* name = "青コイン");

	virtual void load(JSUMemoryInputStream&);
	virtual void makeObjAppeared();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void taken(THitActor*);
};

class TShine : public TItem {
public:
	TShine(const char* name = "シャイン");

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void kill();
	virtual void initMapObj();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void makeMActors();
	virtual void calc();
	virtual void touchPlayer(THitActor*);

	void appearWithDemo(const char*);
	void appearSimple(int);
	static s32 appearWithTimeCallback(u32, u32);
	void appearWithTime(int, int, int, int);
	void movingDown();
	void movingUp();
	void movingCircle();

	static int mPromiLife[4];
	static f32 mSenkoRate[4];
	static f32 mKiraRate[4];
	static f32 mBowRate[4];
	static f32 mCircleRateY;
	static f32 mUpSpeed;
	static f32 mSpeedDownRate;

	enum {
		STATE_UNKB          = 0xB,
		STATE_MOVING_UP     = 0xC,
		STATE_MOVING_CIRCLE = 0xD,
		STATE_MOVING_DOWN   = 0xE,
		STATE_UNKF          = 0xF,
		STATE_UNK10         = 0x10,
		STATE_UNK11         = 0x11,
		STATE_UNK12         = 0x12,
	};

public:
	/* 0x154 */ u32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ int unk168;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ int unk170;
	/* 0x174 */ int unk174;
	/* 0x178 */ int unk178;
	/* 0x17C */ JGeometry::TVec3<f32> unk17C;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ u32 unk18C;
	/* 0x190 */ u8 unk190;
	/* 0x194 */ JPABaseEmitter* unk194;
	/* 0x198 */ JPABaseEmitter* unk198;
	/* 0x19C */ JPABaseEmitter* unk19C;
	/* 0x1A0 */ JPABaseEmitter* unk1A0;
	/* 0x1A4 */ u8 unk1A4;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
	/* 0x1B4 */ u8 unk1B4;
};

class MActor;

class TEggYoshi : public TMapObjGeneral {
public:
	TEggYoshi(const char* name = "ヨッシーの卵");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void touchActor(THitActor*);
	virtual u16 getHitObjNumMax() { return 10; }

	void startFruit();
	void touchFruit(THitActor*);
	void startBalloonAnim();
	void decideRandomLoveFruit();

	// fabricated
	THitActor* getFruit() { return unk150; }

public:
	/* 0x148 */ MActor* unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ THitActor* unk150;
};

class TItemNozzle : public TItem {
public:
	TItemNozzle()
	    : TItem("ノズル")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual void appearing();
	virtual void put();
};

class TNozzleBox : public TMapObjGeneral {
public:
	TNozzleBox(const char* name = "ノズルボックス");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void touchPlayer(THitActor*);
	virtual void breaking();

	void makeModelInvalid();
	void makeModelValid();

public:
	/* 0x148 */ int mContainedNozzleType;
	/* 0x14C */ TItemNozzle* mContainedNozzleItem;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ const char* mContainedNozzleName;
	/* 0x15C */ bool unk15C;
	/* 0x15E */ GXColorS10 unk15E;
	/* 0x166 */ bool unk166;
};

#endif
