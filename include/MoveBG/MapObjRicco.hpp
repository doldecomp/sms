#ifndef MOVE_BG_MAP_OBJ_RICCO_HPP
#define MOVE_BG_MAP_OBJ_RICCO_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/Item.hpp>

class TCraneCargo;
class TFruitLauncher;

/// The crane that swings around Y between its loaded angle and that angle plus
/// the range read out of the map, waiting mWaitTime frames at each end.
class TCraneRotY : public TMapObjBase {
public:
	virtual ~TCraneRotY() { }

	virtual void calc();
	virtual void control();
	virtual void load(JSUMemoryInputStream&);

	TCraneRotY(const char* name = "Ｙ軸回転クレーン")
	    : TMapObjBase(name)
	    , mBaseRotY(0.0f)
	    , mRotYMin(0.0f)
	    , mRotYMax(0.0f)
	    , mRotSpeed(0.0f)
	    , mSoundId(0)
	{
	}

public:
	enum {
		STATE_TURNING_UP    = 0x0,
		STATE_WAIT_AT_START = 0x1,
		STATE_TURNING_DOWN  = 0x2,
		STATE_WAIT_AT_END   = 0x3,
	};

	static int mWaitTime;

	/// The angle the object was placed at; both limits are relative to it.
	/* 0x138 */ f32 mBaseRotY;
	/* 0x13C */ f32 mRotYMin;
	/* 0x140 */ f32 mRotYMax;
	/* 0x144 */ f32 mRotSpeed;
	/* 0x148 */ u32 mSoundId;
};

/// The crane that tips about X, carrying a TCraneCargo on its arm.
class TCraneUpDown : public TMapObjBase {
public:
	virtual ~TCraneUpDown() { }

	virtual void control();
	virtual void initMapObj();

	TCraneUpDown(const char* name = "上下クレーン")
	    : TMapObjBase(name)
	    , mCargo(nullptr)
	    , mSoundId(0)
	{
	}

public:
	enum {
		STATE_TIPPING_UP    = 0x0,
		STATE_WAIT_AT_START = 0x1,
		STATE_TIPPING_DOWN  = 0x2,
		STATE_WAIT_AT_END   = 0x3,
	};

	static f32 mRotSpeed;
	static int mWaitTime;

	/* 0x138 */ TCraneCargo* mCargo;
	/* 0x13C */ u32 mSoundId;
	/* 0x140 */ f32 mRotXMax;
	/* 0x144 */ f32 mRotXMin;
};

class TCraneCargo : public TLeanBlock {
public:
	virtual ~TCraneCargo() { }

	virtual void control();
	virtual void calc();

	TCraneCargo()
	    : TLeanBlock("クレーン積み荷")
	{
	}
};

/// The water wheel that raises and lowers the Ricco Harbour submarine. Water
/// hitting it spins it up; it coasts down again on its own.
class TRiccoWatermill : public TMapObjBase {
public:
	virtual ~TRiccoWatermill() { }

	virtual u32 touchWater(THitActor*);
	virtual void control();
	virtual void calc();
	virtual void loadAfter();
	TRiccoWatermill(const char* name = "リコ水車");

public:
	enum {
		/// Submarine parked at the bottom, wheel idle.
		STATE_DOWN = 0x1,
		/// Wheel spinning up, submarine rising.
		STATE_RISING = 0x2,
		/// Coasting back down with the coin already handed out.
		STATE_SINKING_DONE = 0x3,
		/// Coasting back down before the coin was handed out.
		STATE_SINKING = 0x4,
		/// Submarine surfaced and locked for mWaitTime frames.
		STATE_SURFACED = 0x5,
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

	/* 0x138 */ f32 mRotSpeed;
	/* 0x13C */ TMapObjBase* mSubmarine;
	/// Frames left before the wheel starts slowing down again.
	/* 0x140 */ int mWaterHitTimer;
	/* 0x144 */ bool mCoinThrown;
	/* 0x148 */ TMapObjBase* mBlueCoin;
	/* 0x14C */ JAISoundHandle mWheelSound;
	/* 0x150 */ JAISoundHandle mCraneSound;
	/* 0x154 */ JAISoundHandle mSubmarineSound;
};

/// One of the three surfing squids. Same model with a different TEV colour per
/// instance.
class TSurfGesoObj : public TItem {
public:
	virtual ~TSurfGesoObj() { }

	virtual void initMapObj();

	TSurfGesoObj(const char* name = "イカサーフィン")
	    : TItem(name)
	{
	}

public:
	/* 0x154 */ GXColorS10 mBodyColor;
};

/// The switch on top of a fruit tank. Pressing it fires the launcher.
class TFruitSwitch : public TMapObjBase {
public:
	virtual ~TFruitSwitch() { }

	void pullUp();
	void pushDown();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	TFruitSwitch(const char* name = "フルーツスイッチ")
	    : TMapObjBase(name)
	    , mLauncher(nullptr)
	{
	}

public:
	/* 0x138 */ TFruitLauncher* mLauncher;
};

/// The fruit tank's muzzle. Alternates between the two switches and spits a
/// random fruit out of the one that was not just used.
class TFruitLauncher : public TMapObjBase {
public:
	virtual ~TFruitLauncher() { }

	TMapObjBase* appearFruit() const;
	void fireObj();
	virtual void loadAfter();

	TFruitLauncher(const char* name = "フルーツ発射口")
	    : TMapObjBase(name)
	{
	}

public:
	static f32 mObjSpeedXZ;
	static f32 mObjSpeedY;
	static int mFruitLiveTime;

	/* 0x138 */ TFruitSwitch* mSwitches[2];
	/* 0x140 */ int mCurrentSwitch;
};

#endif
