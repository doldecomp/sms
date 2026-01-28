#ifndef POI_HANA_HPP
#define POI_HANA_HPP

#include <Enemy/WalkerEnemy.hpp>

class TPoihanaSaveLoadParams : public TWalkerEnemyParams {
public:
	TPoihanaSaveLoadParams(const char* path);

	/* 0x32c */ TParamRT<f32> mSLThrowSpeed;
	/* 0x340 */ TParamRT<f32> mSLBackThrowVal;
	/* 0x354 */ TParamRT<s32> mSLSleepFrame;
	/* 0x368 */ TParamRT<s32> mSLWakeFrame;
	/* 0x390 */ TParamRT<f32> mSLTrapJumpMinSpY;
	/* 0x37c */ TParamRT<f32> mSLTrapJumpMaxSpY;
	/* 0x3a4 */ TParamRT<f32> mSLTrapJumpMaxSpXZ;
	/* 0x3b8 */ TParamRT<f32> mSLTrapJumpMinSpXZ;
	/* 0x3cc */ TParamRT<f32> mSLTrapJumpGravity;
};

class TPoiHanaManager : public TSmallEnemyManager {
public:
	TPoiHanaManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

class TPoiHana;

class TPoiHanaCollision : public THitActor {
public:
	TPoiHanaCollision(const char* name = "ポイハナコリジョン") { }

	virtual BOOL receiveMessage(THitActor*, u32);

	void kill();
	void checkHit();

public:
	/* 0x68 */ TPoiHana* unk68;
};

class TPoiHana : public TWalkerEnemy {
public:
	TPoiHana(const char* name = "ポイハナ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void moveObject();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genEventCoin();
	virtual void generateItem();
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setFreezeAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual BOOL isInhibitedForceMove();
	virtual bool doKeepDistance();
	virtual void walkBehavior(int, f32);

	bool isOnTrap();
	void resetHeadHeight();
	bool isMoving();

	static u8 mMouthJntIndex;
	static u8 mSleepVersion;
	static u8 mBodyJntIndex;

public:
	/* 0x194 */ bool unk194;
	/* 0x195 */ bool unk195;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ TPoihanaSaveLoadParams* unk19C;
	/* 0x1A0 */ bool unk1A0;
	/* 0x1A4 */ int mGoToSleepTimer;
	/* 0x1A8 */ bool unk1A8;
	/* 0x1A9 */ bool mIsTrapped;
	/* 0x1AC */ int mThrowTimer;
	/* 0x1B0 */ JGeometry::TVec3<f32> mCurrentFlungVelocity;
	/* 0x1BC */ TPoiHanaCollision* unk1BC;
	/* 0x1C0 */ GXColorS10 unk1C0;
	/* 0x1C8 */ bool unk1C8;
};

class TPoiHanaRed : public TPoiHana {
public:
	TPoiHanaRed(const char* name = "ポイハナ赤")
	    : TPoiHana(name)
	{
	}

	virtual void init(TLiveManager*);
	virtual void attackToMario();
};

class TSleepPoiHana : public TPoiHana {
public:
	TSleepPoiHana(const char* name = "居眠りポイハナ");

	virtual void load(JSUMemoryInputStream&);
};

DECLARE_NERVE(TNervePoihanaSleep, TLiveActor);
DECLARE_NERVE(TNervePoihanaFreeze, TLiveActor);
DECLARE_NERVE(TNervePoihanaThrow, TLiveActor);
DECLARE_NERVE(TNervePoihanaTrapped, TLiveActor);

#endif
