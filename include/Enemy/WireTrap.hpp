#ifndef ENEMY_WIRE_TRAP_HPP
#define ENEMY_WIRE_TRAP_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/WireBinder.hpp>
#include <Strategic/Spine.hpp>
#include <System/ParamInst.hpp>
#include <dolphin/gx/GXStruct.h>

// The electric balls that slide along tightrope wires (TMapWire) and shock
// Mario. Member names come from the .prm strings.
class TWireTrapParams : public TSpineEnemyParams {
public:
	TWireTrapParams(const char* path);

	/* 0xA8 */ TParamRT<f32> mInWaterPowerRate;
	/* 0xBC */ TParamRT<s32> mScaleTimerMax;
	/* 0xD0 */ TParamRT<s32> mGoTimerMax;
};

class TWireTrapManager : public TEnemyManager {
public:
	TWireTrapManager(const char* name = "電線トラップマネージャー");
	virtual ~TWireTrapManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

// How a trap moves along its wire, picked by the scene file.
enum {
	WIRE_TRAP_MODE_RETURN = 0, // back and forth between the wire's two ends
	WIRE_TRAP_MODE_ONEWAY = 1, // to one end, then vanish and reappear
	WIRE_TRAP_MODE_SEARCH = 2, // only while Mario is on a wire, towards him
};

class TWireTrap : public TSpineEnemy {
public:
	TWireTrap(const char* name = "電線トラップ");
	virtual ~TWireTrap() { }

	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();

	void initCollision();
	void initParticle();
	void initWire();
	void initThisColor(const GXColorS10* color);
	void behaveHitWater(THitActor* sender);
	void behaveHitWireTrap(TWireTrap* other,
	                       const JGeometry::TVec3<f32>& momentum,
	                       const JGeometry::TVec3<f32>& other_momentum);
	JGeometry::TVec3<f32> calcMomentum();
	f32 getWaterPow() const;
	bool doReturnMove();
	bool doOnewayMove();
	bool doSearchMove();
	bool doScaleUp();
	bool doScaleDown();
	void doResetToEdge();
	void checkHitActors();
	void updateCollision();
	void emitEffects();
	void setMoveMode(int mode);
	JGeometry::TVec3<f32> getDirAtWirePos() const;
	const JGeometry::TVec3<f32>& getWireDir() const;
	bool isReflect() const;
	static const TNerveBase<TLiveActor>* getNerveFromMode(int mode);
	f32 getRangePosInWire() const;
	TWireBinder* getWireBinder();
	const TWireBinder* getWireBinder() const;
	bool isStartWire() const;
	bool isEndWire() const;

	// fabricated: both timers are loaded again right after being checked,
	// which is what an inline accessor in the check does.
	s32 getHitTimer() const { return mHitTimer; }
	s32 getWaterTimer() const { return mWaterTimer; }

	// fabricated
	const TWireTrapParams* getParam() const
	{
		return (const TWireTrapParams*)getSaveParam();
	}

public:
	/* 0x150 */ u8 unk150[0x10]; // TODO: never touched in this file
	/* 0x160 */ s32 unk160;
	/* 0x164 */ s32 mWaterTimer;
	/* 0x168 */ f32 mWaterPow;
	/* 0x16C */ s32 mHitTimer;
	/* 0x170 */ f32 mMoveDir;
	/* 0x174 */ s32 mWaitTime;
	/* 0x178 */ f32 mAppearScale;
	/* 0x17C */ f32 mMoveSpeed;
	/* 0x180 */ s16 mMoveMode;
};

DECLARE_NERVE(TNerveWireTrapReturnMove, TLiveActor);
DECLARE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor);
DECLARE_NERVE(TNerveWireTrapOnewayMove, TLiveActor);
DECLARE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor);
DECLARE_NERVE(TNerveWireTrapSearch, TLiveActor);
DECLARE_NERVE(TNerveWireTrapWait, TLiveActor);
DECLARE_NERVE(TNerveWireTrapGoWait, TLiveActor);

#endif
