#ifndef MOVE_BG_MAP_OBJ_RICCO_HPP
#define MOVE_BG_MAP_OBJ_RICCO_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/Item.hpp>

class JAISound;

class TCraneRotY : public TMapObjBase {
public:
	TCraneRotY(const char* name = "Ｙ軸回転クレーン")
	    : TMapObjBase(name)
	    , unk138(0.0f)
	    , unk13C(0.0f)
	    , unk140(0.0f)
	    , unk144(0.0f)
	    , unk148(0)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual void calc();

	enum {
		STATE_ROTATE_TO_MAX = 0x0,
		STATE_WAIT_AT_MIN   = 0x1,
		STATE_ROTATE_TO_MIN = 0x2,
		STATE_WAIT_AT_MAX   = 0x3,
	};

	static int mWaitTime;

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ u32 unk148;
};

class TCraneUpDown : public TMapObjBase {
public:
	TCraneUpDown(const char* name = "上下クレーン")
	    : TMapObjBase(name)
	    , unk138(nullptr)
	    , unk13C(0)
	{
	}

	virtual void control();
	virtual void initMapObj();

	enum {
		STATE_ROTATE_TO_MAX = 0x0,
		STATE_WAIT_AT_MIN   = 0x1,
		STATE_ROTATE_TO_MIN = 0x2,
		STATE_WAIT_AT_MAX   = 0x3,
	};

	static f32 mRotSpeed;
	static int mWaitTime;

public:
	/* 0x138 */ TMapObjBase* unk138;
	/* 0x13C */ u32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
};

class TCraneCargo : public TLeanBlock {
public:
	TCraneCargo(const char* name = "クレーン積み荷")
	    : TLeanBlock(name)
	{
	}

	virtual void control();
	virtual void calc();
};

class TRiccoWatermill : public TMapObjBase {
public:
	TRiccoWatermill(const char* name = "リコ水車");

	virtual void loadAfter();
	virtual void control();
	virtual void calc();
	virtual u32 touchWater(THitActor*);

	enum {
		STATE_RISE            = 0x2,
		STATE_SINK_TO_SURFACE = 0x3,
		STATE_SINK_TO_BOTTOM  = 0x4,
		STATE_STAY_AT_SURFACE = 0x5,
	};

	static f32 mRotAccel;
	static f32 mRotSpeedMaxUp;
	static f32 mRotSpeedMaxDown;
	static f32 mRotDown;
	static f32 mSubmarineMoveRate;
	static f32 mSubmarineMaxTransY;
	static f32 mSubmarineBottomTransY;
	static int mWaitTime;
	static f32 mSubmarineSurfaceTransY;

public:
	/* 0x138 */ f32 unk138;
	/* 0x13C */ TMapObjBase* unk13C;
	/* 0x140 */ int unk140;
	/* 0x144 */ bool unk144;
	/* 0x148 */ TMapObjBase* unk148;
	/* 0x14C */ JAISound* unk14C[3];
};

class TSurfGesoObj : public TItem {
public:
	TSurfGesoObj(const char* name = "イカサーフィン")
	    : TItem(name)
	{
	}

	virtual void initMapObj();

public:
	/* 0x154 */ GXColorS10 unk154;
};

class TFruitLauncher;

class TFruitSwitch : public TMapObjBase {
public:
	TFruitSwitch(const char* name = "フルーツスイッチ")
	    : TMapObjBase(name)
	    , unk138(nullptr)
	{
	}

	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void pushDown();
	void pullUp();

public:
	/* 0x138 */ TFruitLauncher* unk138;
};

class TFruitLauncher : public TMapObjBase {
public:
	TFruitLauncher(const char* name = "フルーツ発射口")
	    : TMapObjBase(name)
	{
	}

	virtual void loadAfter();

	TMapObjBase* appearFruit() const;
	void fireObj();

	static f32 mObjSpeedXZ;
	static f32 mObjSpeedY;
	static int mFruitLiveTime;

public:
	/* 0x138 */ TFruitSwitch* unk138[2];
	/* 0x140 */ int unk140;
};

#endif
