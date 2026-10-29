#ifndef ANIMAL_BEE_HIVE_HPP
#define ANIMAL_BEE_HIVE_HPP

#include <Animal/fishoid.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <Strategic/Nerve.hpp>

class TMapObjBase;
class TBeeHive;

class TBeeHiveParams : public TSpineEnemyParams {
public:
	TBeeHiveParams(const char*);

	/* 0xA8 */ TParamRT<s32> mGiveupTimer;
	/* 0xBC */ TParamRT<f32> mGiveupRange;
	/* 0xD0 */ TParamRT<s32> mDecrimentTimer;
	/* 0xE4 */ TParamRT<f32> mRebound;
	/* 0xF8 */ TParamRT<f32> mDecay;
	/* 0x10C */ TParamRT<f32> mAngleMaxAdd;
	/* 0x120 */ TParamRT<f32> mShakePower;
	/* 0x134 */ TParamRT<f32> mFallAngularVel;
	/* 0x148 */ TParamRT<f32> mSearchRange;
};

class TBee : public TRealoidActor {
public:
	TBee(MActor*, TBeeHive*);

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init();

	void behaveToEat();

private:
	/* 0xA8 */ TBeeHive* mHive;
};

class TBeeHive : public TRealoid {
public:
	TBeeHive(const char* name = "ハチの巣とハチ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void bind();
	virtual void reset();
	virtual TRealoidActor* createRealoidActor(MActor*);

	bool isNotBreakOnceYet() const;
	bool isWaiting() const;
	bool isFalling() const;
	JGeometry::TVec3<f32> getCenterOfGravity() const;
	void setShakePower(const JGeometry::TVec3<f32>&);
	bool isMissMario() const;
	bool doScaling();
	void prepareReset();
	void prepareVanish();
	void prepareBreak();
	void dropCoin();
	void appearAllBees();
	void decrimentBee();
	void disappearBee(int);
	void appearBee(int);
	bool doFall();
	void prepareFall();
	bool doWait();
	void prepareWait();
	void controlSound();
	void controlCollision();
	void loadCoin(JSUMemoryInputStream&);
	void receiveMessageFromChild(TBee*);
	void setBoidParamOnMarioWaterIn();
	void setBoidParamOnWaiting();
	void setBoidParamOnAttacking();
	void setBoidBaseParams();
	void initBoids();
	void initCollision();

	// fabricated
	TBeeHiveParams* getSaveParam2() const
	{
		return (TBeeHiveParams*)getSaveParam();
	}

private:
	/* 0x158 */ JGeometry::TQuat4<f32> mSwingTargetRot;
	/* 0x168 */ JGeometry::TQuat4<f32> mSwingRot;
	/* 0x178 */ JGeometry::TQuat4<f32> mBaseRot;
	/* 0x188 */ f32 mSwingAngle;
	/* 0x18C */ f32 mSwingAngVel;
	/* 0x190 */ f32 mSwingAngleMax;
	/* 0x194 */ JGeometry::TVec3<f32> mInitialPosition;
	/* 0x1A0 */ JGeometry::TVec3<f32> mBeeSoundPos;
	/* 0x1AC */ TMapObjBase* mNestItem;
	/* 0x1B0 */ int mHitCheckBeeIdx;
	/* 0x1B4 */ int mActiveBeeNum;
	/* 0x1B8 */ TMapObjBase** mBeeCoins;
	/* 0x1BC */ int mEatenBeeNum;
};

class TBeeHiveManager : public TEnemyManager {
public:
	TBeeHiveManager(const char* name = "ハチの巣マネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

DECLARE_NERVE(TNerveBeeHiveWait, TLiveActor);
DECLARE_NERVE(TNerveBeeHiveFall, TLiveActor);
DECLARE_NERVE(TNerveBeeHiveBreak, TLiveActor);
DECLARE_NERVE(TNerveBeeHiveAttack, TLiveActor);
DECLARE_NERVE(TNerveBeeHiveMarioWaterIn, TLiveActor);
DECLARE_NERVE(TNerveBeeHiveReset, TLiveActor);

#endif
