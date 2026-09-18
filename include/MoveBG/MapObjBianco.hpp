#ifndef MOVEBG_MAPOBJBIANCO_HPP
#define MOVEBG_MAPOBJBIANCO_HPP

#include <JSystem/JAudio/JAInterface/JAISound.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjFloat.hpp>
#include <MoveBG/MapObjTurn.hpp>

class TBGCheckData;
class TBiancoBell;
class TLampSeesawMain;
class TMapObjMessenger;
class TTrembleModelEffect;
struct TBGWallCheckRecord;

/// The big windmill on the Bianco Hills bridge, carrying four blocks around
/// its rim.
class TBigWindmill : public TMapObjBase {
public:
	virtual ~TBigWindmill() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void control();

	TBigWindmill(const char* name = "大風車")
	    : TMapObjBase(name)
	{
		mSoundHandle = nullptr;
	}

public:
	/* 0x138 */ TMapObjBase* mBlocks[4];
	/* 0x148 */ JAISoundHandle mSoundHandle;
};

/// The Piranha Plant root that shakes the ground it sits on.
class TMapObjRootPakkun : public TMapObjBase {
public:
	virtual ~TMapObjRootPakkun() { }
	virtual void initMapObj();
	virtual void drawObject(JDrama::TGraphics*);

	TMapObjRootPakkun(const char* name = "根元パックン")
	    : TMapObjBase(name)
	{
		mTrembleEffect = nullptr;
	}

	static f32 mTremblePower;
	static f32 mTrembleAccel;
	static f32 mTrembleBrake;
	static int mTrembleTime;

public:
	/* 0x138 */ TTrembleModelEffect* mTrembleEffect;
};

/// The horizontal water wheels beside the Bianco Hills stream.
class TBiancoWatermill : public TMapObjBase {
public:
	virtual ~TBiancoWatermill() { }
	virtual void initMapObj();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	void turn(const JGeometry::TVec3<f32>&, const TBGCheckData*, f32);
	void turnByEnemy(THitActor*, const TBGCheckData*);

	TBiancoWatermill(const char* name = "水車");

	static f32 mRotAccel;
	static f32 mEnemyRotAccel;
	static f32 mRotSpeedDownRate;
	static f32 mRotSpeedMax;
	static f32 mRotSpeedMin;

public:
	/* 0x138 */ f32 mRotSpeed;
	/* 0x13C */ JAISoundHandle mSoundHandle;
};

/// The vertical water wheel that drives the turning bridge.
class TBiancoWatermillVertical : public TMapObjBase {
public:
	virtual ~TBiancoWatermillVertical() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	TBiancoWatermillVertical(const char* name = "縦水車");

	static f32 mRotAccel;
	static f32 mRotSpeedDownRate;
	static f32 mRotSpeedMax;
	static f32 mBridgeRotRate;

public:
	/* 0x138 */ f32 mRotSpeed;
	/* 0x13C */ f32 mRotSpeedTarget;
	/* 0x140 */ TMapObjBase* mBridge;
	/* 0x144 */ u8 mNeedGroundUpdate;
	/* 0x148 */ JAISoundHandle mSoundHandle;
	/* 0x14C */ JAISoundHandle mBridgeSoundHandle;
};

/// The small windmills on the Bianco Hills roofs, spun by the hose.
class TBiancoMiniWindmill : public THideObjBase {
public:
	virtual ~TBiancoMiniWindmill() { }
	virtual void initMapObj();
	virtual void control();
	virtual void calc();
	virtual u32 touchWater(THitActor*);

	TBiancoMiniWindmill(const char* name = "小風車");

	static f32 mRotWaterAccel;
	static f32 mFriction;
	static f32 mRotSpeedMax;

public:
	/* 0x150 */ f32 mAngle;
	/* 0x154 */ f32 mRotSpeed;
	/* 0x158 */ f32 mIdleRotSpeed;
	/* 0x15C */ TMapObjMessenger* mMessenger;
	/* 0x160 */ JAISoundHandle mSoundHandle;
};

/// The leaf boat floating on the Bianco Hills stream.
class TLeafBoat : public TMapObjBase {
public:
	virtual ~TLeafBoat() { }
	virtual void control();
	virtual void bind();
	virtual void initMapObj();
	virtual void calc();
	virtual void touchActor(THitActor*);

	void touchWall(JGeometry::TVec3<f32>*, TBGWallCheckRecord*);

	TLeafBoat(const char* name = "葉っぱボート");

public:
	/* 0x138 */ f32 mEnemyBounce;
	/* 0x13C */ f32 mActorBounce;
	/* 0x140 */ f32 mWallBounce;
	/* 0x144 */ f32 mWaterPushRate;
	/* 0x148 */ f32 mFriction;
	/* 0x14C */ f32 unk14C;
	/* 0x150 */ f32 mRiderWeight;
	/* 0x154 */ f32 mHipAttackPower;
	/* 0x158 */ f32 mSpringRate;
	/* 0x15C */ f32 mVerticalDecay;
	/* 0x160 */ int mEffectTimer;
	/* 0x164 */ JGeometry::TVec3<f32> mEffectPos;
};

/// The rotten leaf boat: it sinks and fades once Mario steps on it.
class TLeafBoatRotten : public TLeafBoat {
public:
	virtual ~TLeafBoatRotten() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void control();

	TLeafBoatRotten(const char* name = "腐った葉っぱボート");

	static f32 mAlphaDownSpeed;
	static f32 mCollisionRemoveAlpha;
	static int mBoatFlushTime;
	static int mBoatFlushInterval;
	static GXColorS10 mRottenColor;

public:
	/* 0x170 */ int mRottenTime;
	/* 0x174 */ f32 mAlpha;
	/* 0x178 */ GXColorS10 mColor;
};

/// The lamp seesaw's counterweight end; the main end does the moving.
class TLampSeesaw : public TMapObjBase {
public:
	virtual ~TLampSeesaw() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void touchPlayer(THitActor*);
	virtual void pushDown(f32) { }

	TLampSeesaw(const char* name = "ランプシーソー");

public:
	/* 0x138 */ TLampSeesawMain* mPartner;
	/* 0x13C */ f32 mLowerLimitY;
	/* 0x140 */ f32 mPushSpeed;
};

class TLampSeesawMain : public TLampSeesaw {
public:
	virtual ~TLampSeesawMain() { }
	virtual void loadAfter();
	virtual void control();
	virtual void touchPlayer(THitActor*);
	virtual void pushDown(f32);

	void move();

	TLampSeesawMain(const char* name = "ランプシーソー（主）");

public:
	/* 0x144 */ f32 mSpeed;
	/* 0x148 */ f32 mSpeedDecay;
	/* 0x14C */ f32 mReboundRate;
	/* 0x150 */ f32 mMinSpeed;
};

/// One of the three Bianco Hills bells.
class TBiancoBell : public TMapObjBase {
public:
	virtual ~TBiancoBell() { }
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);
	virtual u32 touchWater(THitActor*);

	void ringSingle();
	void ring();
	void stopToRing();

	TBiancoBell(const char* name = "ビアンコの鐘");

public:
	/* 0x138 */ u16 mAnmIndex;
	/* 0x13A */ u8 mRingsAloud;
};

/// The water wheel that rings the three bells and coughs up coins.
class TBellWatermill : public TMapObjTurn {
public:
	virtual ~TBellWatermill() { }
	virtual void loadAfter();
	virtual void control();
	virtual u32 touchWater(THitActor*);

	TBellWatermill(const char* name = "鐘水車");

public:
	/* 0x16C */ f32 mRotSpeedMax;
	/* 0x170 */ f32 mHeight;
	/* 0x174 */ f32 mHeightMax;
	/* 0x178 */ f32 mRiseSpeed;
	/* 0x17C */ f32 mMinRiseSpeed;
	/* 0x180 */ f32 mRiseAccel;
	/* 0x184 */ f32 mGravity;
	/* 0x188 */ f32 mReboundRate;
	/* 0x18C */ f32 unk18C;
	/* 0x190 */ u8 mSprayedThisFrame;
	/* 0x194 */ TBiancoBell* mBells[3];
	/* 0x1A0 */ u8 mShouldPayCoins;
	/* 0x1A4 */ JAISoundHandle mSoundHandle;
};

/// The floating log in the Bianco Hills lake; it pushes a swimming Mario out.
class TWoodLog : public TMapObjFloatOnSea {
public:
	virtual ~TWoodLog() { }
	virtual void control();

	TWoodLog(const char* name = "丸太")
	    : TMapObjFloatOnSea(name)
	{
	}
};

#endif
