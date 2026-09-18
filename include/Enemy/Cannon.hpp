#ifndef ENEMY_CANNON_HPP
#define ENEMY_CANNON_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/CannonNerve.hpp>
#include <Strategic/SharedParts.hpp>

class TCannon;
class TBombHei;
class TChorobei;
class TMapCollisionMove;
class MAnmSound;
class SDLModelData;

// Names and defaults are the ones PARAM_INIT stringified into .rodata.
class TCannonSaveLoadParams : public TSmallEnemyParams {
public:
	TCannonSaveLoadParams(const char* prm);

	f32 getSLHideDist() const { return mSLHideDist.get(); }
	f32 getSLBombDist() const { return mSLBombDist.get(); }
	f32 getSLKillerDist() const { return mSLKillerDist.get(); }
	s32 getSLBombInterval() const { return mSLBombInterval.get(); }
	s32 getSLKillerInterval() const { return mSLKillerInterval.get(); }
	s32 getSLShootInterval() const { return mSLShootInterval.get(); }
	f32 getSLChorobeiAttackRadius() const { return mSLChorobeiAttackRadius.get(); }
	f32 getSLChorobeiAttackHeight() const { return mSLChorobeiAttackHeight.get(); }
	f32 getSLChorobeiDamageRadius() const { return mSLChorobeiDamageRadius.get(); }
	f32 getSLChorobeiDamageHeight() const { return mSLChorobeiDamageHeight.get(); }
	f32 getSLKillerTransYOffset() const { return mSLKillerTransYOffset.get(); }
	f32 getSLBombHeiGenerateRate() const { return mSLBombHeiGenerateRate.get(); }
	f32 getSLThrowXZSpeed() const { return mSLThrowXZSpeed.get(); }

	/* 0x2D4 */ TParamRT<f32> mSLHideDist;
	/* 0x2E8 */ TParamRT<f32> mSLBombDist;
	/* 0x2FC */ TParamRT<f32> mSLKillerDist;
	/* 0x310 */ TParamRT<s32> mSLBombInterval;
	/* 0x324 */ TParamRT<s32> mSLKillerInterval;
	/* 0x338 */ TParamRT<s32> mSLShootInterval;
	/* 0x34C */ TParamRT<f32> mSLChorobeiAttackRadius;
	/* 0x360 */ TParamRT<f32> mSLChorobeiAttackHeight;
	/* 0x374 */ TParamRT<f32> mSLChorobeiDamageRadius;
	/* 0x388 */ TParamRT<f32> mSLChorobeiDamageHeight;
	/* 0x39C */ TParamRT<f32> mSLKillerTransYOffset;
	/* 0x3B0 */ TParamRT<f32> mSLBombHeiGenerateRate;
	/* 0x3C4 */ TParamRT<f32> mSLThrowXZSpeed;
};

// One of the three barrels ("砲身").
class TCannonDom : public TSharedParts {
public:
	TCannonDom(TLiveActor*, int, SDLModelData*, u32, const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void setBckAnm(int);

	/* 0x1C */ MAnmSound* mAnmSound;
	/* 0x20 */ const char* mAnmSoundName;
	/* 0x24 */ u8 mIsHidden;
	/* 0x28 */ f32 mPitch;
	/* 0x2C */ f32 mRoll;
	/* 0x30 */ f32 mSwingPhase;
};

// The chorobei ("チョロベー") riding the cannon.
class TChorobei : public THitActor {
public:
	TChorobei(TCannon*, int, const char*);

	virtual ~TChorobei() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void checkHit();
	void setBckAnm(int);
	bool isUpEnd();
	bool isDownEnd();

	/* 0x68 */ TCannon* mCannon;
	/* 0x6C */ TSharedParts* mParts;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ MAnmSound* mAnmSound;
	/* 0x78 */ const char* mAnmSoundName;
	/* 0x7C */ f32 mHeightOffset;
};

class TCannon : public TSmallEnemy {
public:
	TCannon(const char* name);

	virtual ~TCannon() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual bool isCollidMove(THitActor*) { return false; }
	virtual BOOL isInhibitedForceMove() { return TRUE; }
	// A misspelt twin of isHitValid that sits at the end of the vtable.
	virtual bool isHitVallid(u32) { return false; }

	void startChorobeiShout();
	void gateOpen();
	void killShootAct();
	bool isObject();
	void turnToGoal();
	void startMarioDemo();
	void startDemo();
	void deadCannon();
	void setKillerGoalPoint();
	void damage();
	void endKillerShoot();
	void killerShoot();
	void updateAttachPos();

	// fabricated
	TChorobei* getChorobei() const { return mChorobei; }
	void hitHead(TBombHei*);
	void bombScaleUp();
	void bombShoot();
	void bombSet();
	void entryObjCollision();
	void calcObjCollision();

	// fabricated
	TCannonSaveLoadParams* getSaveParams() const { return mSaveParams; }

	static u8 mChorobeiJntIdx;
	static u8 mChorobeiHandJntIdx;
	static f32 mVelocityRate;
	static f32 mSearchRate;

	/* 0x194 */ JGeometry::TVec3<f32> mAttachOffset;
	/* 0x1A0 */ TBombHei* mAttachedBomb;
	/* 0x1A4 */ TBombHei* mHeldBomb;
	/* 0x1A8 */ TChorobei* mChorobei;
	/* 0x1AC */ TCannonDom* mDoms[3];
	/* 0x1B8 */ TCannonDom* mSingleDom;
	/* 0x1BC */ TSharedParts* mMarioParts;
	/* 0x1C0 */ TMapCollisionMove* mDomCollisions[3];
	/* 0x1CC */ u8 unk1CC[0x1E0 - 0x1CC];
	/* 0x1E0 */ MtxPtr mShootMtx;
	/* 0x1E4 */ TPosition3f mTakingMtx;
	/* 0x214 */ int mDomIdx;
	/* 0x218 */ u32 unk218;
	/* 0x21C */ u8 mBombThrown;
	/* 0x220 */ f32 mBombScale;
	/* 0x224 */ f32 mDomRoll[3];
	/* 0x230 */ u8 mStage;
	/* 0x234 */ f32 mInitialYaw;
	/* 0x238 */ u8 mMarioAnmPlaying;
	/* 0x239 */ u8 mAimAtMario;
	/* 0x23C */ JGeometry::TVec3<f32> mInitialPos;
	/* 0x248 */ JGeometry::TVec3<f32> mKillerGoal;
	/* 0x254 */ TSpineEnemy* mMareGate;
	/* 0x258 */ TMapCollisionMove* mObjCollision;
	/* 0x25C */ JGeometry::TVec3<f32> mObjVertices[4];
	/* 0x28C */ TCannonSaveLoadParams* mSaveParams;
	/* 0x290 */ u8 mShootMode;
	/* 0x294 */ JGeometry::TVec3<f32> mEffectPos;
	/* 0x2A0 */ JGeometry::TVec3<f32> mDemoCamPos;
	/* 0x2AC */ f32 mPrevYaw;
	/* 0x2B0 */ TMapCollisionMove* mFutaCollision;
};

class TCannonManager : public TSmallEnemyManager {
public:
	TCannonManager(const char* name);

	virtual ~TCannonManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

#endif
