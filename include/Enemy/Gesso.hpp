#ifndef ENEMY_GESSO_HPP
#define ENEMY_GESSO_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/WalkerEnemy.hpp>

class TGessoSaveLoadParams : public TWalkerEnemyParams {
public:
	TGessoSaveLoadParams(const char*);
};

class TGessoPolluteModelManager : public TEnemyPolluteModelManager {
public:
	virtual void init(TLiveActor*);
};

class TGessoPolluteModel : public TEnemyPolluteModel {
public:
	TGessoPolluteModel();

	virtual void setAnm();
};

class TGessoManager;
extern TGessoManager* gpCurGesso;

class TGessoManager : public TSmallEnemyManager {
public:
	TGessoManager(const char* name = "ゲッソーマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual void initSetEnemies();

	void requestPolluteModel(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);
};

class TGesso : public TWalkerEnemy {
public:
	TGesso(const char* name = "ゲッソー");

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void genRandomItem();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm() { }
	virtual void attackToMario();
	virtual void initAttacker(THitActor*);
	virtual void setAfterDeadEffect();
	virtual void doKeepDistance();
	virtual void behaveToFindMario();
	virtual void walkBehavior(int, f32);
	virtual void isResignationAttack();

	void polluteBehavior();
	void setPolluteGoal();
	void pollute();
	void isUseBodyCallBack() const;
	void rollCheck();
	void rollEnd();
	void modifyRotate();
	void fallEnd();
	void turnIn();
	void turning();
	void turnOut();
	void checkDropInWater();

	static u8 mBodyJntIndex;
	static u8 mMouthJntIndex;
	static f32 mBodyRotSpeed;
	static f32 mPollRange;
	static f32 mThroughHoseiDistY;
	static f32 mAngTestY;
};

class TSurfGesso : public TGesso {
public:
	TSurfGesso(const char* name = "サーフゲッソー");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual bool isFindMario(f32) { return false; }
};

class TLandGesso : public TGesso {
public:
	TLandGesso(const char* name = "平地ゲッソー");

	virtual void load(JSUMemoryInputStream&);
};

class TGessoPolluteObj : public TEnemyAttachment {
public:
	TGessoPolluteObj(const char* name);

	virtual void calcRootMatrix();
	virtual void loadInit(TSpineEnemy*, const char*);
	virtual void rebirth();
	virtual void sendMessage();
	virtual void set();
	virtual f32 getNowGravity();
	virtual void pollute();
};

DECLARE_NERVE(TNerveGessoStay, TLiveActor);
DECLARE_NERVE(TNerveGessoFreeze, TLiveActor);
DECLARE_NERVE(TNerveGessoPunch, TLiveActor);
DECLARE_NERVE(TNerveGessoPollute, TLiveActor);
DECLARE_NERVE(TNerveGessoFall, TLiveActor);
DECLARE_NERVE(TNerveGessoRolling, TLiveActor);
DECLARE_NERVE(TNerveGessoFindMario, TLiveActor);
DECLARE_NERVE(TNerveGessoLand, TLiveActor);
DECLARE_NERVE(TNerveGessoTurn, TLiveActor);

#endif
