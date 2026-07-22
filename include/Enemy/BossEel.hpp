#ifndef ENEMY_BOSS_EEL_HPP
#define ENEMY_BOSS_EEL_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class MActor;
class SDLModelData;
class TSharedParts;
class TBossEel;
class TBossEelSaveParams;
class TBEelTears;
class TBossEelTearsRecoverCollision;

class TBEelTearsSaveLoadParams : public TSpineEnemyParams {
public:
	TBEelTearsSaveLoadParams(const char*);

public:
	/* 0x0A8 */ TParamRT<f32> mSLTearsUpSpeed;
	/* 0x0BC */ TParamRT<f32> mSLTearsDamageUpSpeed;
	/* 0x0D0 */ TParamRT<f32> mSLTearsLiveHeight;
	/* 0x0E4 */ TParamRT<s32> mSLTearsSplitNum;
	/* 0x0F8 */ TParamRT<s32> mSLTearsDamageRadius;
	/* 0x10C */ TParamRT<s32> mSLTearsDamageHeight;
	/* 0x120 */ TParamRT<s32> mSLTearsAttackRadius;
	/* 0x134 */ TParamRT<s32> mSLTearsAttackHeight;
	/* 0x148 */ TParamRT<s32> mSLTearsDropDamageRadius;
	/* 0x15C */ TParamRT<s32> mSLTearsDropDamageHeight;
	/* 0x170 */ TParamRT<s32> mSLTearsDropAttackRadius;
	/* 0x184 */ TParamRT<s32> mSLTearsDropAttackHeight;
	/* 0x198 */ TParamRT<f32> mSLHighPolyDistY;
	/* 0x1AC */ TParamRT<f32> mSLHitAnmFrameRate;
	/* 0x1C0 */ TParamRT<f32> mSLBodyScaleLow;
	/* 0x1D4 */ TParamRT<f32> mSLBodyScaleHigh;
	/* 0x1E8 */ TParamRT<f32> mSLTearsDropScaleLow;
	/* 0x1FC */ TParamRT<f32> mSLTearsDropScaleHigh;
	/* 0x210 */ f32 mBodyScaleLow;
	/* 0x214 */ f32 mBodyScaleHigh;
	/* 0x218 */ f32 mTearsDropScaleLow;
	/* 0x21C */ f32 mTearsDropScaleHigh;
};

class TBEelTearsDrop : public THitActor {
public:
	TBEelTearsDrop(TBEelTears*, int, SDLModelData*, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message)
	{
		return THitActor::receiveMessage(sender, message);
	}

	void generate(JGeometry::TVec3<f32>&);

public:
	/* 0x68 */ TSharedParts* mSharedParts;
	/* 0x6C */ bool mActive;
	/* 0x70 */ f32 mRiseSpeed;
	/* 0x74 */ TBEelTears* mOwner;
};

class TBEelTearsManager : public TEnemyManager {
public:
	TBEelTearsManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createEnemies(int);

	void splitTears(JGeometry::TVec3<f32>&);
	TBEelTearsSaveLoadParams* getSaveParam() const
	{
		return static_cast<TBEelTearsSaveLoadParams*>(unk38);
	}

public:
	/* 0x54 */ TBEelTearsDrop* mTearsDrops[30];
};

class TBEelTears : public TSpineEnemy {
public:
	TBEelTears(const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void setMActorAndKeeper();

	void deadEffect();
	void setRecoverTears();
	void setBubble();

public:
	/* 0x150 */ JGeometry::TVec3<f32> mInitialPosition;
	/* 0x15C */ TBEelTearsSaveLoadParams* mTearsParams;
	/* 0x160 */ bool mHighPoly;
	/* 0x164 */ s32 mStateTimer;
	/* 0x168 */ const TBGCheckData* mWaterSurface;
	/* 0x16C */ TBossEelTearsRecoverCollision* mRecoverCollision;
};

class TOilBall : public TBEelTears {
public:
	TOilBall(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void reset();
};

class TBossEelSaveParams : public TSpineEnemyParams {
public:
	TBossEelSaveParams();

public:
	/* 0x0A8 */ TParamRT<f32> mSLInitTransYOffset;
	/* 0x0BC */ TParamRT<f32> mSLAppearMoveDistY;
	/* 0x0D0 */ TParamRT<f32> mSLBodyScale;
	/* 0x0E4 */ TParamRT<f32> mSLViewClipFar;
	/* 0x0F8 */ TParamRT<f32> mSLViewClipRadius;
	/* 0x10C */ TParamRT<f32> mSLBodyToHeadDistance;
	/* 0x120 */ TParamRT<f32> mSLBodyAttackRadius;
	/* 0x134 */ TParamRT<f32> mSLBodyAttackHeight;
	/* 0x148 */ TParamRT<f32> mSLBodyDamageRadius;
	/* 0x15C */ TParamRT<f32> mSLBodyDamageHeight;
	/* 0x170 */ TParamRT<f32> mSLHeadAttackRadius;
	/* 0x184 */ TParamRT<f32> mSLHeadAttackHeight;
	/* 0x198 */ TParamRT<f32> mSLHeadDamageRadius;
	/* 0x1AC */ TParamRT<f32> mSLHeadDamageHeight;
	/* 0x1C0 */ TParamRT<f32> mSLToothAttackRadius;
	/* 0x1D4 */ TParamRT<f32> mSLToothAttackHeight;
	/* 0x1E8 */ TParamRT<f32> mSLToothDamageRadius;
	/* 0x1FC */ TParamRT<f32> mSLToothDamageHeight;
	/* 0x210 */ TParamRT<f32> mSLSpinAccel;
	/* 0x224 */ TParamRT<f32> mSLSpinMaxSpeed;
	/* 0x238 */ TParamRT<f32> mSLToothUpSpeed;
	/* 0x24C */ TParamRT<f32> mSLToothLiveHeight;
	/* 0x260 */ TParamRT<s32> mSLToothMaxHitPoint;
	/* 0x274 */ TParamRT<s32> mSLGenTearsTime;
	/* 0x288 */ TParamRT<f32> mSLVortexAttackRadius;
	/* 0x29C */ TParamRT<f32> mSLVortexAttackHeight;
	/* 0x2B0 */ TParamRT<f32> mSLVortexDamageRadius;
	/* 0x2C4 */ TParamRT<f32> mSLVortexDamageHeight;
	/* 0x2D8 */ TParamRT<s32> mSLVortexLiveTimer;
	/* 0x2EC */ TParamRT<f32> mSLVortexScaleXZ;
	/* 0x300 */ TParamRT<f32> mSLVortexScaleY;
	/* 0x314 */ TParamRT<f32> mSLMouthOpenFrame;
	/* 0x328 */ TParamRT<s32> mSLMouthOpenInterval;
	/* 0x33C */ TParamRT<f32> mSLCanEatFrame;
	/* 0x350 */ TParamRT<f32> mSLBreathInPower;
};

class TBossEelManager : public TEnemyManager {
public:
	TBossEelManager(const char*);

	virtual void loadAfter();
	virtual void createModelData();
	virtual void clipEnemies(JDrama::TGraphics* graphics);
};

class TBossEelTooth : public THitActor {
public:
	TBossEelTooth(u8, TBossEel*, const char*, SDLModelData*, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void updateTremble();
	void changeToothAlpha(u8);
};

class TBossEelVortex : public THitActor {
public:
	TBossEelVortex(TBossEel*, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	void reset();
};

class TBossEelEye : public THitActor {
public:
	TBossEelEye(const TLiveActor*, int, SDLModelData*, u32, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	void setBckAnm(int);
};

class TBossEelHeartCoin : public THitActor {
public:
	TBossEelHeartCoin(const TLiveActor*, int, SDLModelData*, u32, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	void generate(JGeometry::TVec3<f32>&);
};

class TBossEelCollision : public THitActor {
public:
	TBossEelCollision(MtxPtr collisionMtx, const char* name)
	    : THitActor(name)
	    , mCollisionMtx(collisionMtx)
	    , mUnk6C(0.0f)
	    , mUnk70(0.0f)
	    , mUnk74(0.0f)
	    , mUnk78(0.0f)
	    , mOwner(nullptr)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void initCollision();
	virtual void behaveToMario();

public:
	/* 0x68 */ MtxPtr mCollisionMtx;
	/* 0x6C */ f32 mUnk6C;
	/* 0x70 */ f32 mUnk70;
	/* 0x74 */ f32 mUnk74;
	/* 0x78 */ f32 mUnk78;
	/* 0x7C */ TBossEel* mOwner;
};

class TBossEelBodyCollision : public TBossEelCollision {
public:
	TBossEelBodyCollision(MtxPtr, const char*);

	virtual void initCollision();
};

class TBossEelAwaCollision : public TBossEelCollision {
public:
	TBossEelAwaCollision(MtxPtr, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void initCollision();
	virtual void behaveToMario();
};

class TBossEelBarrierCollision : public TBossEelCollision {
public:
	TBossEelBarrierCollision(MtxPtr, const char*);

	virtual void initCollision();
	virtual void behaveToMario();
};

class TBossEelTearsRecoverCollision : public TBossEelCollision {
public:
	TBossEelTearsRecoverCollision(MtxPtr collisionMtx, const char* name)
	    : TBossEelCollision(collisionMtx, name)
	    , mRecovering(false)
	    , mColliding(false)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void initCollision();
	virtual void behaveToMario();

public:
	/* 0x80 */ bool mRecovering;
	/* 0x81 */ bool mColliding;
};

class TBossEel : public TSpineEnemy {
public:
	TBossEel(const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual MtxPtr getTakingMtx();
	virtual const char** getBasNameTable() const;
	virtual BOOL hasMapCollision() const;

	void startMoguCamera();
	BOOL isInBossEelMoguDemo();
	void quickBack();
	BOOL isValidToothDamage();
	void deadCheck();
	void setBckAnm(int);
	void collideToMario();
	void invalidateAllCollision();
	void generateVortex();
	void forceShedTears(bool);
	void shedTears(MtxPtr);
	BOOL isEyeBlurOn();
	BOOL canEatMario();
	void forceEat();
	void updateTearsCnt();
	void calcAndSetCollisionCubeBite_();
	void generateBubble(JGeometry::TVec3<f32>&);
};

void ExecSpinNerve_Sub(TBossEel*);
void ExecBackNerve_Sub(TSpineBase<TLiveActor>*, f32);
void hoseiDiveCameraCallback(u32, u32);

DECLARE_NERVE(TNerveBEelTearsGenerate, TLiveActor);
DECLARE_NERVE(TNerveBEelTearsMoveUp, TLiveActor);
DECLARE_NERVE(TNerveBEelTearsWaterHit, TLiveActor);
DECLARE_NERVE(TNerveBEelTearsMarioRecover, TLiveActor);
DECLARE_NERVE(TNerveBEelTearsSplit, TLiveActor);
DECLARE_NERVE(TNerveOilBallStay, TLiveActor);
DECLARE_NERVE(TNerveBossEelWaitAppear, TLiveActor);
DECLARE_NERVE(TNerveBossEelFirstSpin, TLiveActor);
DECLARE_NERVE(TNerveBossEelSecondSpin, TLiveActor);
DECLARE_NERVE(TNerveBossEelAppear, TLiveActor);
DECLARE_NERVE(TNerveBossEelOutWait, TLiveActor);
DECLARE_NERVE(TNerveBossEelSlowBack, TLiveActor);
DECLARE_NERVE(TNerveBossEelQuickBack, TLiveActor);
DECLARE_NERVE(TNerveBossEelEat, TLiveActor);
DECLARE_NERVE(TNerveBossEelDie, TLiveActor);
DECLARE_NERVE(TNerveBossEelMouthOpenWait, TLiveActor);
DECLARE_NERVE(TNerveBossEelSleepOnBottom, TLiveActor);

#endif
