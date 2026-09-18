#ifndef MOVE_BG_MAP_OBJ_MARE_HPP
#define MOVE_BG_MAP_OBJ_MARE_HPP

#include <MoveBG/MapObjBase.hpp>

class TCannon;
class TCogwheel;
class TMareEventDepressWall;
struct TBGWallCheckRecord;

/// The two buckets hanging off TCogwheel's rope: the plate ("cogwheel_plate",
/// mIsUpper == true) and the pot ("cogwheel_pot", mIsUpper == false).
class TCogwheelScale : public TMapObjBase {
public:
	virtual ~TCogwheelScale() { }
	virtual u32 touchWater(THitActor*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void touchPlayer(THitActor*);
	virtual void control();

	TCogwheelScale(const char*);

	static f32 mWaterLeakSpeed;

public:
	/* 0x138 */ f32 mWeight;
	/* 0x13C */ f32 mMarioWeight;
	/* 0x140 */ f32 mWaterAmount;
	/* 0x144 */ f32 mWaterAmountMax;
	/* 0x148 */ f32 mRiderWeight;
	/* 0x14C */ f32 mHeadAttackRate;
	/* 0x150 */ f32 mPushSpeed;
	/* 0x154 */ u8 mIsUpper;
	/* 0x158 */ TCogwheel* mCogwheel;
};

/// The Noki Bay "balance" (天秤): a wheel with a rope over it carrying a plate
/// on one side and a pot on the other.
class TCogwheel : public TMapObjBase {
public:
	virtual ~TCogwheel() { }
	virtual void initMapObj();
	virtual void control();
	virtual void calc();
	virtual void draw() const;

	void initDraw() const;
	void rebound();

	TCogwheel(const char* name = "天秤");

	static f32 mRopeWidthX;
	static f32 mRopeWidthZ;
	static f32 mTexPosRate;
	static f32 mMinSpeed;

public:
	/* 0x138 */ f32 mSpeed;
	/* 0x13C */ f32 mPlateRopeLength;
	/* 0x140 */ f32 mAccelRate;
	/* 0x144 */ f32 mSpeedDecay;
	/* 0x148 */ f32 mReboundRate;
	/* 0x14C */ f32 mRopeLength;
	/* 0x150 */ TCogwheelScale* mPlate;
	/* 0x154 */ JGeometry::TVec3<f32> mPlateRopePos;
	/* 0x160 */ f32 mUpperMargin;
	/* 0x164 */ TCogwheelScale* mPot;
	/* 0x168 */ JGeometry::TVec3<f32> mPotRopePos;
	/* 0x174 */ f32 mLowerMargin;
};

/// The rubber cord (ゴムひも) a held object dangles from in Noki Bay.
class TMapObjElasticCode : public TMapObjBase {
public:
	virtual ~TMapObjElasticCode() { }
	virtual void initMapObj();
	virtual void control();
	virtual void draw() const;

	TMapObjElasticCode(const char* name = "ゴムひも")
	    : TMapObjBase(name)
	    , mHoldWeight(0.0f)
	    , mSpringRate(0.0f)
	    , mSpeedDecay(0.0f)
	{
	}

public:
	/* 0x138 */ f32 mHoldWeight;
	/* 0x13C */ f32 mSpringRate;
	/* 0x140 */ f32 mSpeedDecay;
};

/// The bean sprout tree (もやしの木) that grows when sprayed.
class TMapObjGrowTree : public TMapObjBase {
public:
	virtual ~TMapObjGrowTree() { }
	virtual void initMapObj();
	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void updateHeight();
	f32 getGrowHeightFromRate(f32) const;

	TMapObjGrowTree(const char* name = "もやしの木");

public:
	/* 0x138 */ f32 mGrowHeight;
	/* 0x13C */ f32 mGrowSpeed;
	/* 0x140 */ f32 mShrinkSpeed;
	/* 0x144 */ int mGrownTime;
	/* 0x148 */ f32 mInitialHeight;
};

/// The purple bell (ワイヤー鈴（紫）) hanging from a Noki Bay wire.
class TWireBell : public TMapObjBase {
public:
	virtual ~TWireBell() { }
	virtual void loadAfter();
	virtual void control();
	virtual void draw() const;

	void initDraw() const;

	TWireBell(const char* name = "ワイヤー鈴（紫）");

public:
	/* 0x138 */ int mWireNo;
	/* 0x13C */ f32 mHangLength;
	/* 0x140 */ f32 mRopeWidthX;
	/* 0x144 */ f32 mRopeWidthZ;
	/* 0x148 */ f32 mTexPosRate;
	/* 0x14C */ JGeometry::TVec3<f32> mWirePos;
};

/// The puncher (パンチャー) that throws Mario when he walks into it.
class TMapObjPuncher : public TMapObjBase {
public:
	virtual ~TMapObjPuncher() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void control();
	virtual void touchPlayer(THitActor*);

	TMapObjPuncher(const char* name = "パンチャー")
	    : TMapObjBase(name)
	{
		mThrowSpeed = 0.0f;
	}

public:
	/* 0x138 */ f32 mThrowSpeed;
};

/// The mud boat (どろの船) Mario rides across Noki Bay, pushed by his nozzle.
class TMuddyBoat : public TMapObjBase {
public:
	virtual ~TMuddyBoat() { }
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual void kill();
	virtual void initMapObj();
	virtual u32 getSDLModelFlag() const;
	virtual void calc();

	void moveByWater();
	void touchWall(JGeometry::TVec3<f32>*, const TBGWallCheckRecord&);
	void bindToWall(const JGeometry::TVec3<f32>&, f32,
	                JGeometry::TVec3<f32>*);

	TMuddyBoat(const char* name = "どろの船");

public:
	/* 0x138 */ f32 mAccelForward;
	/* 0x13C */ f32 mAccelBackward;
	/* 0x140 */ f32 mSpeed;
	/* 0x144 */ f32 mSpeedDecay;
	/* 0x148 */ f32 mTurnAccel;
	/* 0x14C */ f32 mTurnSpeed;
	/* 0x150 */ f32 mTurnDecay;
	/* 0x154 */ f32 mWallRadiusCenter;
	/* 0x158 */ f32 mWallRadiusFront;
	/* 0x15C */ f32 mWallRadiusBack;
	/* 0x160 */ f32 mFrontOffset;
	/* 0x164 */ f32 mBackOffset;
	/* 0x168 */ int mRespawnTime;
	/* 0x16C */ int unk16C;
	/* 0x170 */ JGeometry::TVec3<f32> mEffectPos;
	// Set by initMapObj but never read in this TU.
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ f32 unk180;
	/* 0x184 */ f32 unk184;
};

/// The Noki Bay waterfall (マーレ滝): sound and spray only.
class TMareFall : public TMapObjBase {
public:
	virtual ~TMareFall() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void calc();

	TMareFall(const char* name = "マーレ滝")
	    : TMapObjBase(name)
	{
	}
};

/// The cork (マーレコルク) plugging the Noki Bay bottle; blown out by the
/// cannon, which is what releases the boss shine.
class TMareCork : public TMapObjBase {
public:
	virtual ~TMareCork() { }
	virtual void loadAfter();
	virtual MtxPtr getTakingMtx();
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void drawObject(JDrama::TGraphics*);

	TMareCork(const char* name = "マーレコルク")
	    : TMapObjBase(name)
	{
		mCannon     = nullptr;
		mIsBlownOut = 0;
	}

public:
	/* 0x138 */ TCannon* mCannon;
	/* 0x13C */ JGeometry::TVec3<f32> mEffectPos;
	/* 0x148 */ JGeometry::TVec3<f32> mSoundPos;
	/* 0x154 */ u8 mIsBlownOut;
};

/// The event trigger (イベントポイント) on the Noki Bay wall that starts the
/// wall-sinking event once enough of the wall has been cleaned.
class TMareEventPoint : public THitActor {
public:
	virtual ~TMareEventPoint() { }
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	TMareEventPoint(const char* name = "イベントポイント")
	    : THitActor(name)
	{
		mDepressWall = nullptr;
	}

public:
	/* 0x68 */ TMareEventDepressWall* mDepressWall;
};

#endif
