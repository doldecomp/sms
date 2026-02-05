#ifndef COASTER_KILLER
#define COASTER_KILLER

#include "Enemy/SmallEnemy.hpp"
#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JGeometry/JGVec4.hpp>

class TCoasterEnemyParams : public TWalkerEnemyParams {
public:
	TCoasterEnemyParams(const char* path)
	    : TWalkerEnemyParams(path)
	    , PARAM_INIT(mSLCoasterSpeedInOrder, 20.0f)
	    , PARAM_INIT(mSLCoasterSpeedReverse, 20.0f)
	{
		TParams::load(mPrmPath);
		mSLCoasterSpeedInOrder.value = 20.0f;
		mSLCoasterSpeedReverse.value = 16.0f;
	}

	/* 0x338 */ TParamRT<f32> mSLCoasterSpeedInOrder;
	/* 0x34C */ TParamRT<f32> mSLCoasterSpeedReverse;
};

class TCoasterKillerSaveLoadParams : public TCoasterEnemyParams {
public:
	TCoasterKillerSaveLoadParams(const char* path)
	    : TCoasterEnemyParams(path)
	    , PARAM_INIT(mSLBombRange, 300.0f)
	{
		TParams::load(mPrmPath);
	}

	/* 0x360 */ TParamRT<f32> mSLBombRange;
};

class TCoasterEnemy : public TWalkerEnemy {
public:
	TCoasterEnemy(const char* name)
	    : TWalkerEnemy(name)
	    , mPathIdx(0) { };
	virtual ~TCoasterEnemy() { }

	virtual void calcRootMatrix();
	virtual void makeCoasterGoalPath();
	virtual void moveCoaster();
	virtual float getGravityY() const;
	virtual void setNormalFlyAnm();
	virtual void setWalkAnm();
	virtual void moveObject();
	virtual void init(TLiveManager* manager);
	virtual void bind();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void reset();

	TCoasterEnemyParams* getSaveParam2() const
	{
		return (TCoasterEnemyParams*)getSaveParam();
	}
	int getPathIdx() const { return mPathIdx; }
	int getPathDir() const { return mPathDir; }
	const JGeometry::TQuat4<f32>& getQuat() const { return mQuat; }

	/* 0x194 */ int mPathIdx;
	/* 0x198 */ int mPathDir;
	/* 0x19C */ JGeometry::TQuat4<f32> mQuat;
};

DECLARE_NERVE(TNerveCoasterEnemyWander, TLiveActor);

class TCoasterKillerManager : public TSmallEnemyManager {
public:
	TCoasterKillerManager(const char* name = "コースターキラーマネージャー");
	virtual ~TCoasterKillerManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	/* 0x60 */ u8 unk60;
};

class TCoasterKiller : public TCoasterEnemy {
public:
	TCoasterKiller(const char* name = "コースターキラー");
	virtual ~TCoasterKiller() { }

	virtual void init(TLiveManager* manager);
	virtual void reset();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void setMActorAndKeeper();
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);
	virtual void behaveToWater(THitActor*);
	virtual const char** getBasNameTable() const;
	virtual void setNormalFlyAnm();
	virtual void setDeadAnm();

	TCoasterKillerSaveLoadParams* getSaveParam2() const
	{
		return (TCoasterKillerSaveLoadParams*)getSaveParam();
	}
	float get1AC() const { return unk1AC; }
	GXColorS10& getBodyColor() { return mBodyColor; }
	GXColorS10& getEyesColor() { return mEyesColor; }
	GXColorS10& getNoseColor() { return mNoseColor; }
	GXColorS10& getBaseColor() { return mBaseColor; }
	TCoasterKillerManager* getManager()
	{
		return (TCoasterKillerManager*)mManager;
	}

	/* 0x1AC */ float unk1AC;
	/* 0x1B0 */ GXColorS10 mBodyColor;
	/* 0x1B8 */ GXColorS10 mNoseColor;
	/* 0x1C0 */ GXColorS10 mEyesColor;
	/* 0x1C8 */ GXColorS10 mBaseColor;
	/* 0x1D0 */ TPosition3f mParticlePos;
};

DECLARE_NERVE(TNerveCoasterKillerExplosion, TLiveActor);

#endif
