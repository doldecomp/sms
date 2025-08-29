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
};

class THaneHamuKuriSaveLoadParams : public THamuKuriSaveLoadParams {
public:
	THaneHamuKuriSaveLoadParams(const char* path);
};

class TBossDangoHamuKuriSaveLoadParams : public THamuKuriSaveLoadParams {
public:
	TBossDangoHamuKuriSaveLoadParams(const char* path);
};

class TFireHamuKuriSaveLoadParams : public THamuKuriSaveLoadParams {
public:
	TFireHamuKuriSaveLoadParams(const char* path);
};

// ============= managers =============

class THamuKuri;

class THamuKuriManager : public TSmallEnemyManager {
public:
	THamuKuriManager(const char*);

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
};

class THaneHamuKuriManager : public THamuKuriManager {
public:
	THaneHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void createAnmData();
	virtual TSpineEnemy* createEnemyInstance();
};

class TDoroHaneKuriManager : public THaneHamuKuriManager {
public:
	TDoroHaneKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	void createHige();
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

class TDoroHige : public TSharedParts {
public:
	TDoroHige(const TLiveActor* param_1, SDLModelData* param_2,
	          const char* name = "ひげ")
	    : TSharedParts(param_1, 5, param_2, 3, name)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);
};

class TDoroHamuKuriManager : public THamuKuriManager {
public:
	TDoroHamuKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	virtual void createHige();
};

// ============= instances =============

class THamuKuri : public TWalkerEnemy {
public:
	THamuKuri(const char*);

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
	virtual bool canDoJitabata() { }
	virtual void onHaveCap();

	void releaseCap();
	void setSearchActor(THitActor*);
	void isGiveUpSearchActor();
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
};

class THaneHamuKuri : public THamuKuri {
public:
	THaneHamuKuri(const char*);

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
	virtual void setBckAnm(int);
	virtual void walkBehavior(int, f32);
	virtual void setRollAnm();
	virtual void setCrashAnm();
	virtual bool canDoJitabata() { }

	void resetFlyParam();

	static bool mBoundFly;
};

class TDoroHaneKuri : public THaneHamuKuri {
public:
	TDoroHaneKuri(const char*);

	virtual void init(TLiveManager*);
	virtual void reset();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isCollidMove(THitActor*);

	void onHaveCap();
};

class THaneHamuKuri2 : public THaneHamuKuri {
public:
	THaneHamuKuri2(const char*);

	virtual void reset();
	virtual BOOL isReachedToGoal() const;
	virtual void behaveToWater(THitActor*) { }
	virtual void forceKill() { }
	virtual bool isHitValid(u32) { }
	virtual void sendAttackMsgToMario();
	virtual void walkBehavior(int, f32);
};

class TDangoHamuKuri : public THamuKuri {
public:
	TDangoHamuKuri(const char*);

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
};

class TBossDangoHamuKuri : public TDangoHamuKuri {
public:
	TBossDangoHamuKuri(const char*);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void moveObject();
	virtual void reset();
	virtual void genEventCoin();
	virtual bool changeByJuice() { }
	virtual void setGenerateAnm();
	virtual bool isFindMario(f32) { }

	void isDead();
	void generateBody();
	void isNowAttack();
	void isNowGenerate();
};

class TFireHamuKuri : public THamuKuri {
public:
	TFireHamuKuri(const char*);

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual void sendAttackMsgToMario();
	virtual void setBckAnm(int) { }
	virtual void walkBehavior(int, f32);

	void recoverFire();
	void genFire();
	void dieFire();
	void changeTevColor();
};

class TDoroHamuKuri : public THamuKuri {
public:
	TDoroHamuKuri(const char*);

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
