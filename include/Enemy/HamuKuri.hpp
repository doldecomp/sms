#ifndef HAMU_KURI_HPP
#define HAMU_KURI_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Enemy/Launcher.hpp>
#include <Strategic/SharedParts.hpp>

class TMapObjBase;

class THamuKuriLauncher : public TLauncher {
public:
	THamuKuriLauncher(const char* name)
	    : TLauncher(name)
	{
	}

	virtual void drawObject(JDrama::TGraphics*);
	virtual void stateLaunch();
};

// ============= params =============

class THamuKuriSaveLoadParams : public TWalkerEnemyParams {
public:
	THamuKuriSaveLoadParams(const char* path);

	s32 getCrashBonusNum() const { return mSLCrashBonusNum.get(); }
	s32 getSerialCrashFrame() const { return mSLSerialCrashFrame.get(); }

	/* 0x32C */ TParamRT<f32> mSLWaterCoeff;
	/* 0x340 */ TParamRT<f32> mSLWaterAttackCoeff;
	/* 0x354 */ TParamRT<f32> mSLFirstVelocityY;
	/* 0x368 */ TParamRT<f32> mSLVelocityRate;
	/* 0x37C */ TParamRT<s32> mSLBoundNum;
	/* 0x390 */ TParamRT<s32> mSLSearchActorTimer;
	/* 0x3A4 */ TParamRT<f32> mSLCanSearchDist;
	/* 0x3B8 */ TParamRT<s32> mSLJitabataTimer;
	/* 0x3CC */ TParamRT<f32> mSLFirstKickVelocityY;
	/* 0x3E0 */ TParamRT<s32> mSLFlyTimer;
	/* 0x3F4 */ TParamRT<s32> mSLTrampleBonusNum;
	/* 0x408 */ TParamRT<s32> mSLCrashBonusNum;
	/* 0x41C */ TParamRT<s32> mSLSerialCrashFrame;
	/* 0x430 */ TParamRT<s32> mSLKyoroTimer;
};

class THaneHamuKuriSaveLoadParams : public THamuKuriSaveLoadParams {
public:
	THaneHamuKuriSaveLoadParams(const char* path);

	/* 0x444 */ TParamRT<f32> mSLNormalJumpVy;
	/* 0x458 */ TParamRT<f32> mSLAttackJumpVy;
	/* 0x46C */ TParamRT<f32> mSLFlyBaseHeight;
	/* 0x480 */ TParamRT<f32> mSLFlyBaseAmplitude;
	/* 0x494 */ TParamRT<f32> mSLFlyBaseFrequency;
};

class TBossDangoHamuKuriSaveLoadParams : public THamuKuriSaveLoadParams {
public:
	TBossDangoHamuKuriSaveLoadParams(const char* path);

public:
	/* 0x444 */ TParamRT<s32> mSLNumArray;
};

class TFireHamuKuriSaveLoadParams : public THamuKuriSaveLoadParams {
public:
	TFireHamuKuriSaveLoadParams(const char* path);

public:
	/* 0x444 */ TParamRT<s32> mSLRecoverTimer;
};

// ============= managers =============

class THamuKuri;

class THamuKuriManager : public TSmallEnemyManager {
public:
	THamuKuriManager(const char* = "ハムクリマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual void createAnmData();
	virtual TSpineEnemy* createEnemyInstance();

	void setSearchHamuKuri();
	void requestSerialKill(THamuKuri*);
	void checkSerialKill();

	static bool mSearchActSw;

	// fabricated
	int getUnk6C() const { return unk6C; }
	void setUnk6C(int v) { unk6C = v; }

public:
	/* 0x60 */ u32 unk60;
	/* 0x64 */ TMapObjBase** unk64;
	/* 0x68 */ int unk68;
	/* 0x6C */ int unk6C;
	/* 0x70 */ THamuKuri* unk70;
};

class THaneHamuKuriManager : public THamuKuriManager {
public:
	THaneHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void createAnmData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TDoroHige;

class TDoroHaneKuriManager : public THaneHamuKuriManager {
public:
	TDoroHaneKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	void createHige();

public:
	/* 0x74 */ TDoroHige* unk74;
};

class TDangoHamuKuriManager : public THamuKuriManager {
public:
	TDangoHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelDataArray(const TModelDataLoadEntry*);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

class TBossDangoHamuKuriManager : public TDangoHamuKuriManager {
public:
	TBossDangoHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

class TFireHamuKuriManager : public THamuKuriManager {
public:
	TFireHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

class TDoroHaneKuri;

class TDoroHige : public TSharedParts {
public:
	TDoroHige(const TLiveActor* param_1, int param_2, SDLModelData* param_3,
	          const char* name = "ひげ")
	    : TSharedParts(param_1, param_2, param_3, 3, name)
	    , unk1C((THamuKuri*)param_1)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);

	void setOwner(THamuKuri* hamu)
	{
		unk1C = hamu;
		unk10 = (TLiveActor*)hamu;
	}

public:
	/* 0x1C */ const THamuKuri* unk1C;
};

class TDoroHamuKuriManager : public THamuKuriManager {
public:
	TDoroHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	void createHige();

public:
	/* 0x74 */ TDoroHige* unk74;
};

// ============= instances =============

class THamuKuri : public TWalkerEnemy {
public:
	THamuKuri(const char* name = "ハムクリ");

	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void bind();
	virtual void moveObject();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genRandomItem();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void setGenerateAnm();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual void initAttacker(THitActor*);
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void endHitWaterJump();
	virtual void setBckAnm(int);
	virtual void setAfterDeadEffect();
	virtual bool isFindMario(f32);
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, f32);
	virtual bool isResignationAttack();
	virtual void setRollAnm();
	virtual void setCrashAnm();
	virtual bool canDoJitabata() { return !isAirborne(); }
	virtual void onHaveCap() { unk198 = true; }

	void releaseCap();
	void setSearchActor(THitActor*);
	bool isGiveUpSearchActor();
	void jumpToSearchActor();
	void canGoForSearchActor();
	void changeCapHolder();
	void selectCapHolder();
	void makeCapFly(TMapObjBase*);
	void setWallDeadEffect();
	void setAppearAnm();
	void isAttackToHam();
	void isSerialWallDie();
	void forceRoll(JGeometry::TVec3<f32>, bool);

	static f32 mCapGravityY;
	static f32 mCapSpeed;
	static f32 mVGenerateGravityY;
	static f32 mLandAnmFrameNum;

	// fabricated
	THamuKuriManager* getManager() { return (THamuKuriManager*)mManager; }
	bool isUnk198() const { return unk198 ? true : false; }

public:
	/* 0x194 */ f32 unk194;
	/* 0x198 */ u8 unk198;
	/* 0x19C */ int unk19C;
	/* 0x1A0 */ u8 unk1A0;
	/* 0x1A1 */ u8 unk1A1;
	/* 0x1A2 */ u8 unk1A2;
	/* 0x1A3 */ u8 unk1A3;
	/* 0x1A4 */ u8 unk1A4;
	/* 0x1A8 */ int unk1A8;
	/* 0x1AC */ u8 unk1AC;
	/* 0x1B0 */ Mtx unk1B0;
	/* 0x1E0 */ int unk1E0;
	/* 0x1E4 */ JGeometry::TVec3<f32> unk1E4;
	/* 0x1F0 */ u8 unk1F0;
	/* 0x1F4 */ THamuKuriSaveLoadParams* unk1F4;
	/* 0x1F8 */ THitActor* unk1F8;
	/* 0x1FC */ GXColor unk1FC;
	/* 0x200 */ JGeometry::TVec3<f32> unk200;
};

class THaneHamuKuri : public THamuKuri {
public:
	THaneHamuKuri(const char* name = "はねハムクリ");

	virtual void init(TLiveManager*);
	virtual void bind();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual BOOL isReachedToGoal() const;
	virtual void setBehavior() { }
	virtual void behaveToWater(THitActor*);
	virtual void setGenerateAnm() { }
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void setBckAnm(int param_1) { TSmallEnemy::setBckAnm(param_1); }
	virtual void walkBehavior(int, f32);
	virtual void setRollAnm();
	virtual void setCrashAnm();
	virtual bool canDoJitabata() { return false; }

	void resetFlyParam();

	static bool mBoundFly;

public:
	/* 0x20C */ f32 unk20C;
	/* 0x210 */ f32 unk210;
	/* 0x214 */ f32 unk214;
	/* 0x218 */ f32 unk218;
	/* 0x21C */ f32 unk21C;
	/* 0x220 */ Vec unk220;
	/* 0x22C */ THaneHamuKuriSaveLoadParams* unk22C;
	/* 0x230 */ f32 unk230;
	/* 0x234 */ f32 unk234;
};

class TDoroHaneKuri : public THaneHamuKuri {
public:
	TDoroHaneKuri(const char* = "ドロハネクリ");

	virtual void init(TLiveManager*);
	virtual void reset();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);

	void onHaveCap()
	{
		unk198          = 1;
		TDoroHige* hige = ((TDoroHaneKuriManager*)mManager)->unk74;
		hige->unk1C     = this;
		hige->unk10     = this;
	}

public:
	/* 0x238 */ THaneHamuKuriSaveLoadParams* unk238;
};

class THaneHamuKuri2 : public THaneHamuKuri {
public:
	THaneHamuKuri2(const char*);

	virtual void reset();
	virtual BOOL isReachedToGoal() const;
	virtual void behaveToWater(THitActor*) { }
	virtual void forceKill() { }
	virtual bool isHitValid(u32) { return true; }
	virtual void sendAttackMsgToMario();
	virtual void walkBehavior(int, f32);
};

class TBossDangoHamuKuri;

class TDangoHamuKuri : public THamuKuri {
public:
	TDangoHamuKuri(const char* = "だんごハムクリ");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void updateAnmSound();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice();
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);

	void nerveInit();
	void swingBody();

	static bool mAttackSw;

public:
	/* 0x20C */ f32 unk20C;
	/* 0x210 */ f32 unk210;
	/* 0x214 */ f32 unk214;
	/* 0x218 */ f32 unk218;
	/* 0x21C */ f32 unk21C;
	/* 0x220 */ f32 unk220;
	/* 0x224 */ f32 unk224;
	/* 0x228 */ TDangoHamuKuri* mNext;
	/* 0x22C */ TDangoHamuKuri* mPrev;
	/* 0x230 */ u8 unk230;
	/* 0x234 */ TBossDangoHamuKuri* mBoss;
};

class TBossDangoHamuKuri : public TDangoHamuKuri {
public:
	TBossDangoHamuKuri(const char* = "ボスだんごハムクリ");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void reset();
	virtual void genEventCoin();
	virtual bool changeByJuice() { return false; }
	virtual void setGenerateAnm();
	virtual bool isFindMario(f32) { return 0; }

	bool isDead();
	void generateBody();
	void isNowAttack();
	void isNowGenerate();

public:
	/* 0x238 */ int unk238;
	/* 0x23C */ TBossDangoHamuKuriSaveLoadParams* unk23C;
};

class TFireHamuKuri : public THamuKuri {
public:
	TFireHamuKuri(const char* = "ヤキグリ");

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual void sendAttackMsgToMario();
	virtual void setBckAnm(int index) { TSmallEnemy::setBckAnm(index); }
	virtual void walkBehavior(int, f32);

	bool recoverFire();
	void genFire();
	void dieFire();
	void changeTevColor();

public:
	/* 0x20C */ TFireHamuKuriSaveLoadParams* unk20C;
	/* 0x210 */ u8 unk210;
	/* 0x214 */ int unk214;
	/* 0x218 */ int unk218;
	/* 0x21C */ GXColorS10 unk21C;
	/* 0x224 */ GXColorS10 unk224;
};

class TDoroHamuKuri : public THamuKuri {
public:
	TDoroHamuKuri(const char* = "どろハムクリ");

	virtual void init(TLiveManager*);
	virtual void kill();
	virtual void reset();
	virtual void setBehavior();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);
	virtual void onHaveCap();
};

// ============= nerves =============

DECLARE_NERVE(TNerveHamuKuriGoForSearchActor, TLiveActor);
DECLARE_NERVE(TNerveHamuKuriBoundFreeze, TLiveActor);
DECLARE_NERVE(TNerveHamuKuriWallDie, TLiveActor);
DECLARE_NERVE(TNerveHamuKuriLand, TLiveActor);
DECLARE_NERVE(TNerveHamuKuriJitabata, TLiveActor);
DECLARE_NERVE(TNerveDangoHamuKuriWait, TLiveActor);
DECLARE_NERVE(TNerveDangoHamuKuriAttack, TLiveActor);
DECLARE_NERVE(TNerveHaneHamuKuriUpWait, TLiveActor);
DECLARE_NERVE(TNerveHaneHamuKuriMoveOnGraph, TLiveActor);
DECLARE_NERVE(TNerveDoroHamuKuriRobCap, TLiveActor);
DECLARE_NERVE(TNerveFireHamuKuriRecover, TLiveActor);
DECLARE_NERVE(TNerveDoroHaneRise, TLiveActor);
DECLARE_NERVE(TNerveDoroHaneHitWater, TLiveActor);
DECLARE_NERVE(TNerveDoroHanePrepareAttack, TLiveActor);

#endif
