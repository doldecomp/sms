#ifndef ENEMY_IGAIGA_HPP
#define ENEMY_IGAIGA_HPP

#include <Enemy/EnemyAttachment.hpp>
#include <Enemy/WalkerEnemy.hpp>

class TRollEnemySaveLoadParams {
public:
	TRollEnemySaveLoadParams(const char*);
};

class TRollEnemy : public TWalkerEnemy {
public:
	TRollEnemy(const char*);

	virtual void reset();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);
	virtual void setAfterDeadEffect();
	virtual void walkBehavior(int, f32);
	virtual void flagJump();
	virtual void isReachedToGoalXZ();
	virtual void bound();
	virtual bool isRolling();
	virtual void rollSE();
	virtual void boundSE();
};

class TIgaigaPolluteModelManager : public TEnemyPolluteModelManager {
public:
	TIgaigaPolluteModelManager(const char*);

	virtual void init(TLiveActor*);
};

class TIgaigaPolluteModel : public TEnemyPolluteModel {
public:
	TIgaigaPolluteModel(TLiveActor*, int, SDLModelData*, const char*);

	virtual void setAnm();
};

class TIgaigaManager : public TSmallEnemyManager {
public:
	TIgaigaManager(const char*);

	void load(JSUMemoryInputStream&);
	void createModelData();
	TSmallEnemy* createEnemyInstance();
	void initSetEnemies();
	void perform(u32 cue, JDrama::TGraphics* graphics);
	void requestPolluteModel(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);
};

class TIgaiga : public TRollEnemy {
public:
	TIgaiga(const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual void walkBehavior(int, f32);
	virtual void isReachedToGoalXZ();
	virtual void bound();
	virtual bool isRolling();
	virtual void rollSE();
	virtual void boundSE();

	void rollMove();
	void waterExplosion();
	void shoot(JGeometry::TVec3<f32>&);
};

DECLARE_NERVE(TNerveIgaigaRollOnGraph, TLiveActor);
DECLARE_NERVE(TNerveIgaigaWaterHit, TLiveActor);
DECLARE_NERVE(TNerveIgaigaShootFromCannon, TLiveActor);

class TGorogoroPolluteModelManager : public TEnemyPolluteModelManager {
public:
	TGorogoroPolluteModelManager(const char*);

	virtual void init(TLiveActor*);
};

class TGorogoroPolluteModel : public TEnemyPolluteModel {
public:
	TGorogoroPolluteModel(TLiveActor*, int, SDLModelData*, const char*);

	virtual void setAnm();
};

class TGorogoroManager {
public:
	TGorogoroManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual void createEnemyInstance();
	virtual void initSetEnemies();

	void inArea(const JGeometry::TVec3<f32>&);
	void requestPolluteModel(JGeometry::TVec3<f32>&, JGeometry::TVec3<f32>&);
};

class TGorogoro : public TRollEnemy {
public:
	TGorogoro(const char*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void kill();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual void walkBehavior(int, f32);
	virtual void flagJump();
	virtual void bound();
	virtual bool isRolling();
	virtual void rollSE();
	virtual void boundSE();

	void setGenerateGraphIdx(int);
	void generateByGateKeeper(const JGeometry::TVec3<f32>&,
	                          const JGeometry::TVec3<f32>&);
};

DECLARE_NERVE(TNerveGorogoroRollOnGraph, TLiveActor);
DECLARE_NERVE(TNerveGorogoroDie, TLiveActor);

#endif
