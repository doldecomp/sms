#ifndef ENEMY_KAZEKUNNERVE_HPP
#define ENEMY_KAZEKUNNERVE_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Strategic/Nerve.hpp>
#include <JSystem/JGeometry.hpp>

class TLiveActor;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TKazekunParams::TKazekunParams stores into each TParamRT.
class TKazekunParams : public TSmallEnemyParams {
public:
	TKazekunParams(const char* prm);

	f32 getAppearDist() const { return mAppearDist.get(); }
	f32 getAroundDist() const { return mAroundDist.get(); }
	f32 getAroundSpeed() const { return mAroundSpeed.get(); }
	s32 getAroundTime() const { return mAroundTime.get(); }
	f32 getAttackSpeed() const { return mAttackSpeed.get(); }
	f32 getAirFric() const { return mAirFric.get(); }
	s32 getResetTime() const { return mResetTime.get(); }
	s32 getResetTimeHitting() const { return mResetTimeHitting.get(); }
	s32 getPoseTime() const { return mPoseTime.get(); }
	f32 getDicideTiming() const { return mDicideTiming.get(); }
	f32 getTurnOffsetY() const { return mTurnOffsetY.get(); }
	f32 getLostOffsetYUp() const { return mLostOffsetYUp.get(); }
	f32 getLostOffsetYDown() const { return mLostOffsetYDown.get(); }
	f32 getPoseSpeed() const { return mPoseSpeed.get(); }
	f32 getPoseOmegaRate() const { return mPoseOmegaRate.get(); }

	/* 0x2D4 */ TParamRT<f32> mAppearDist;
	/* 0x2E8 */ TParamRT<f32> mAroundDist;
	/* 0x2FC */ TParamRT<f32> mAroundSpeed;
	/* 0x310 */ TParamRT<s32> mAroundTime;
	/* 0x324 */ TParamRT<f32> mAttackSpeed;
	/* 0x338 */ TParamRT<f32> mAirFric;
	/* 0x34C */ TParamRT<s32> mResetTime;
	/* 0x360 */ TParamRT<s32> mResetTimeHitting;
	/* 0x374 */ TParamRT<s32> mPoseTime;
	/* 0x388 */ TParamRT<f32> mDicideTiming;
	/* 0x39C */ TParamRT<f32> mTurnOffsetY;
	/* 0x3B0 */ TParamRT<f32> mLostOffsetYUp;
	/* 0x3C4 */ TParamRT<f32> mLostOffsetYDown;
	/* 0x3D8 */ TParamRT<f32> mPoseSpeed;
	/* 0x3EC */ TParamRT<f32> mPoseOmegaRate;
};

// The wind spirit of Pinna Park's beach: it pops out of nowhere, circles Mario
// at a fixed radius while blowing him around, then charges him head on. Its
// facing is kept as a quaternion rather than a Euler angle because it banks
// while it flies.
class TKazekun : public TSmallEnemy {
public:
	TKazekun(const char* name);

	// Declared in vtable order; TKazekun adds no new slots to TSmallEnemy.
	virtual ~TKazekun() { }
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);

	void setVisible(bool);
	void doAttack(bool);
	bool doAttackPose(bool);
	void getAroundQuat(JGeometry::TQuat4<f32>&,
	                   const JGeometry::TVec3<f32>&, f32);
	f32 getAroundRate(const JGeometry::TVec3<f32>&) const;
	void flyAroundMario();
	void changeBck(const char*);
	bool isGiveUpAround() const;
	void updateEffect();
	void emitAppearEffect();
	bool hasWind() const;
	bool isDamage() const;
	bool isHitWater() const;
	void initParticle();
	void initCollision();

	// fabricated
	JGeometry::TVec3<f32> getHomePosition() const { return mHomePosition; }

	// fabricated
	TKazekunParams* getSaveParams() const
	{
		return (TKazekunParams*)getSaveParam();
	}

	// Where the wind spirit was placed; it circles around this point and
	// gives up once Mario climbs too far above or below it.
	/* 0x194 */ JGeometry::TVec3<f32> mHomePosition;
	// Facing, as a unit quaternion.
	/* 0x1A0 */ JGeometry::TQuat4<f32> mQuat;
	// How many frames to stay gone before searching for Mario again.
	/* 0x1B0 */ int mSleepTime;
	// TODO: unknown, 0x20 bytes. The only evidence is MarNameRefGen_Enemy's
	// `new 0x1d4` in front of __ct__8TKazekunFPCc; the constructor stores
	// nothing past mSleepTime and neither Kazekun.o nor MarNameRefGen_Enemy.o
	// (the only two objects that mention TKazekun) loads or stores anything
	// between 0x1b4 and 0x1d3, so the code that used these went away with
	// doAttack/getAroundQuat and the rest of the UNUSED list. Split it up as
	// soon as one access turns up.
	/* 0x1B4 */ u8 unk1B4[0x20];
};

class TKazekunManager : public TSmallEnemyManager {
public:
	TKazekunManager(const char* name);

	virtual ~TKazekunManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

DECLARE_NERVE(TNerveKazekunAppear, TLiveActor)
DECLARE_NERVE(TNerveKazekunAttack, TLiveActor)
DECLARE_NERVE(TNerveKazekunDisappear, TLiveActor)
DECLARE_NERVE(TNerveKazekunHitWater, TLiveActor)
DECLARE_NERVE(TNerveKazekunPreAttack, TLiveActor)
DECLARE_NERVE(TNerveKazekunSearch, TLiveActor)
DECLARE_NERVE(TNerveKazekunTurn, TLiveActor)
DECLARE_NERVE(TNerveKazekunWait, TLiveActor)

#endif
