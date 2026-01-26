#ifndef ENEMY_TAMA_NOKO_HPP
#define ENEMY_TAMA_NOKO_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/SharedParts.hpp>

class TTamaNokoFlower : public TSharedParts {
public:
	TTamaNokoFlower(const TLiveActor*, int, SDLModelData*, u32, const char*);

	virtual void perform(u32, JDrama::TGraphics*);

	void setBckAnm(int);

public:
	/* 0x1C */ u8 unk1C;
	/* 0x20 */ Vec unk20;
	/* 0x2C */ MAnmSound* unk2C;
	/* 0x30 */ const char* unk30;
	/* 0x34 */ u8 unk34;
	/* 0x35 */ u8 unk35;
};

class TTamaNokoSaveLoadParams : public TWalkerEnemyParams {
public:
	TTamaNokoSaveLoadParams(const char* path);

	f32 getJumpAttackSp() const { return mSLJumpAttackSp.get(); }

	/* 0x32C */ TParamRT<s32> mSLPickUpTime;
	/* 0x340 */ TParamRT<f32> mSLJumpAttackDist;
	/* 0x354 */ TParamRT<f32> mSLThrownVY;
	/* 0x368 */ TParamRT<f32> mSLThrownRateXZ;
	/* 0x37C */ TParamRT<f32> mSLThrownGravityY;
	/* 0x390 */ TParamRT<f32> mSLJumpAttackSp;
	/* 0x3A4 */ TParamRT<f32> mSLAttackGravityY;
	/* 0x3B8 */ TParamRT<s32> mSLSinkTime;
	/* 0x3CC */ TParamRT<s32> mSLWakeUpTimer;
};

class TTamaNokoManager : public TSmallEnemyManager {
public:
	TTamaNokoManager(const char*);

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

class TTamaNoko : public TWalkerEnemy {
public:
	TTamaNoko(const char* name = "タマノコ");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void requestShadow();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual BOOL isReachedToGoal() const;
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice() { return false; }
	virtual void behaveToTaken(THitActor*);
	virtual void behaveToRelease();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual void forceKill() { }
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void setAfterDeadEffect();
	virtual bool doKeepDistance();
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, f32);

	void landEffect();
	void forceWakeUp();
	void forceSleep();

	static int mStopOnAirTimeMax;
	static int mRollOnAirTimeMax;
	static f32 mRollOnAirAngle;
	static f32 mDropVelocityY;

	// fabricated
	TTamaNokoSaveLoadParams* getSaveParams2() const
	{
		return (TTamaNokoSaveLoadParams*)getSaveParam();
	}

public:
	/* 0x194 */ bool mIsDoingJumpAttack;
	/* 0x195 */ bool mIsSunkInGround;
	/* 0x198 */ TTamaNokoSaveLoadParams* unk198;
	/* 0x19C */ TTamaNokoFlower* unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ JGeometry::TVec3<f32> unk1AC;
	/* 0x1B8 */ bool unk1B8;
	/* 0x1B9 */ bool unk1B9;
	/* 0x1BC */ int mAirAttackTimer;
	/* 0x1C0 */ int mWakeUpTimer;
};

DECLARE_NERVE(TNerveTamaNokoSleep, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoAttack, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoDown, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoPickUp, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoThrown, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoSink, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoHitWater, TLiveActor);
DECLARE_NERVE(TNerveTamaNokoWait, TLiveActor);

#endif
