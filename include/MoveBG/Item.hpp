#ifndef MOVE_BG_ITEM_HPP
#define MOVE_BG_ITEM_HPP

#include <MoveBG/MapObjGeneral.hpp>

class TMirrorActor;

class TItem : public TMapObjGeneral {
public:
	TItem(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
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

public:
	/* 0x148 */ THitActor* unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ u32 unk150;
};

class TCoin : public TItem {
public:
	TCoin(const char*);

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
	TFlowerCoin();

	virtual void load(JSUMemoryInputStream&);

public:
	/* 0x158 */ u32 unk158;
};

class TCoinEmpty : public TCoin {
public:
	TCoinEmpty(const char*);

	virtual void kill();
	virtual void appear();
	virtual void makeObjAppeared();

	void warning();
};

class TCoinRed : public TCoin {
public:
	TCoinRed(const char*);
	virtual void taken(THitActor*);
};

class TCoinBlue : public TCoin {
public:
	TCoinBlue(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void makeObjAppeared();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void taken(THitActor*);
};

class TShine : public TItem {
public:
	TShine(const char*);

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void kill();
	virtual void initMapObj();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void makeMActors();
	virtual void calc();
	virtual void touchPlayer(THitActor*);

	void appearWithDemo(const char*);
	void appearSimple(int);
	void appearWithTimeCallback(u32, u32);
	void appearWithTime(int, int, int, int);
	void movingDown();
	void movingUp();
	void movingCircle();

	static u32 mPromiLife;
	static u32 mSenkoRate;
	static u32 mKiraRate;
	static u32 mBowRate;
	static u32 mCircleRateY;
	static u32 mUpSpeed;
	static u32 mSpeedDownRate;
	static u32 mSpeedDownTime;

public:
	/* 0x154 */ u32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ u32 unk168;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ u32 unk170;
	/* 0x174 */ u32 unk174;
	/* 0x178 */ u32 unk178;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ f32 unk180;
	/* 0x184 */ f32 unk184;
	/* 0x188 */ f32 unk188;
	/* 0x18C */ u32 unk18C;
	/* 0x190 */ u8 unk190;
	/* 0x194 */ u32 unk194;
	/* 0x198 */ u32 unk198;
	/* 0x19C */ u32 unk19C;
	/* 0x1A0 */ u32 unk1A0;
	/* 0x1A4 */ char unk1A4[0x4];
	/* 0x1A8 */ f32 unk1A8;
	/* 0x1AC */ f32 unk1AC;
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ u8 unk1B4;
};

class TEggYoshi : public TMapObjGeneral {
public:
	TEggYoshi(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void touchActor(THitActor*);
	virtual u16 getHitObjNumMax() { return 10; }

	void startFruit();
	void touchFruit(THitActor*);
	void startBalloonAnim();
	void decideRandomLoveFruit();
};

class TItemNozzle : public TItem {
public:
	TItemNozzle()
	    : TItem("ノズル")
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual void appearing();
	virtual void put();
};

class TNozzleBox : public TMapObjGeneral {
public:
	TNozzleBox(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void touchPlayer(THitActor*);
	virtual void breaking();

	void makeModelInvalid();
	void makeModelValid();

public:
	/* 0x148 */ int unk148;
	/* 0x14C */ u32 unk14C;
	/* 0x150 */ f32 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ u32 unk158;
	/* 0x15C */ u8 unk15C;
	/* 0x15E */ u16 unk15E;
	/* 0x160 */ u16 unk160;
	/* 0x162 */ u16 unk162;
	/* 0x164 */ u16 unk164;
	/* 0x166 */ u8 unk166;
};

#endif
