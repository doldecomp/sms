#ifndef ENEMY_WIRE_TRAP_HPP
#define ENEMY_WIRE_TRAP_HPP

#include "JSystem/JGeometry/JGVec3.hpp"
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <M3DUtil/M3UJoint.hpp>
#include <Strategic/Spine.hpp>
#include <Strategic/Nerve.hpp>

class TWireBinder;

DECLARE_NERVE(TNerveWireTrapReturnMove, TLiveActor);
DECLARE_NERVE(TNerveWireTrapOnewayMove, TLiveActor);
DECLARE_NERVE(TNerveWireTrapOnewayMoveStart, TLiveActor);
DECLARE_NERVE(TNerveWireTrapOnewayMoveEnd, TLiveActor);
DECLARE_NERVE(TNerveWireTrapSearch, TLiveActor);
DECLARE_NERVE(TNerveWireTrapWait, TLiveActor);
DECLARE_NERVE(TNerveWireTrapGoWait, TLiveActor);

class TWireTrapParams : public TSpineEnemyParams {
public:
	TWireTrapParams(const char* path);

	/* 0xA8 */ TParamRT<f32> mInWaterPowerRate;
	/* 0xBC */ TParamRT<s32> mScaleTimerMax;
	/* 0xD0 */ TParamRT<s32> mGoTimerMax;
};

class TWireTrap : public TSpineEnemy {
public:
	enum TWireMode {
		WIRETRAP_MODE_RETURN_MOVE,
		WIRETRAP_MODE_ONEWAY_MOVE,
		WIRETRAP_MODE_SEARCH,
	};
	TWireTrap(const char* name = "電線トラップ");
	virtual ~TWireTrap() { }

	virtual void init(TLiveManager* manager);
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();

	void initCollision();                        // UNUSED
	void initParticle();                         // UNUSED
	void initWire();                             // UNUSED
	void initThisColor(const GXColorS10* color); // UNUSED

	void behaveHitWater(THitActor*); // UNUSED
	void kill();
	void behaveHitWireTrap(TWireTrap* otherTrap,
	                       const JGeometry::TVec3<float>& dir,
	                       const JGeometry::TVec3<float>& otherDir); // UNUSED
	void moveObject();
	const JGeometry::TVec3<f32> calcMomentum(); // UNUSED
	f32 getWaterPow() const;                    // UNUSED
	bool doReturnMove();                        // UNUSED
	bool doOnewayMove();                        // UNUSED
	bool doSearchMove();                        // UNUSED
	bool doScaleUp();                           // UNUSED
	bool doScaleDown();                         // UNUSED
	void doResetToEdge();                       // UNUSED
	void checkHitActors();
	void updateCollision();                              // UNUSED
	void emitEffects();                                  // UNUSED
	void setMoveMode(int);                               // UNUSED
	const JGeometry::TVec3<f32> getDirAtWirePos() const; // UNUSED
	const JGeometry::TVec3<f32>& getWireDir() const;
	bool isReflect() const; // UNUSED
	static const TNerveBase<TLiveActor>* getNerveFromMode(int mode);
	f32 getRangePosInWire() const; // UNUSED
	TWireBinder* getWireBinder();  // UNUSED
	TWireBinder* getWireBinder() const;
	bool isStartWire() const; // UNUSED
	bool isEndWire() const;   // UNUSED

	// fabricated
	TWireTrapParams* getDataParams() const
	{
		return (TWireTrapParams*)getSaveParam();
	}

public:
	/* 0x150 */ s32 unk150;
	/* 0x154 */ s32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ s32 unk160;
	/* 0x164 */ s32 mHitCooldownTimer;
	/* 0x168 */ f32 mHitCooldownSpeed;
	/* 0x16C */ s32 unk16C; // cooldowntimer?
	/* 0x170 */ f32 mWireDirectionMultiplier;
	/* 0x174 */ s32 mTargetValue;
	/* 0x178 */ f32 mModelScaleMultiplier;
	/* 0x17C */ f32 unk17C;
	/* 0x180 */ s16 mInitialMode;
};

class TWireTrapManager : public TEnemyManager {
public:
	TWireTrapManager(const char* name = "電線トラップマネージャー");
	virtual ~TWireTrapManager() { }

	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();
};

#endif
