#ifndef ENEMY_BATHTUB_PEACH_HPP
#define ENEMY_BATHTUB_PEACH_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/BathtubBinder.hpp>
#include <System/ParamInst.hpp>

// fabricated name: the class is never emitted, its constructor only appears
// inlined into TBathtubPeachManager::load. Member names come from the .prm
// strings.
class TBathtubPeachParams : public TSpineEnemyParams {
public:
	TBathtubPeachParams(const char* path)
	    : TSpineEnemyParams(path)
	    , PARAM_INIT(turnSpeed, 8.0f)
	    , PARAM_INIT(turnSpeed2, 1.0f)
	    , PARAM_INIT(speed, 16.0f)
	    , PARAM_INIT(angle, 72.0f)
	    , PARAM_INIT(range, 100.0f)
	    , PARAM_INIT(radius, 2200.0f)
	{
		TParams::load(mPrmPath);
	}

	/* 0xA8 */ TParamRT<f32> turnSpeed;
	/* 0xBC */ TParamRT<f32> turnSpeed2;
	/* 0xD0 */ TParamRT<f32> speed;
	/* 0xE4 */ TParamRT<f32> angle;
	/* 0xF8 */ TParamRT<f32> range;
	/* 0x10C */ TParamRT<f32> radius;
};

class TBathtubPeachManager : public TEnemyManager {
public:
	TBathtubPeachManager(const char* name = "バスタブピーチマネージャー");
	virtual ~TBathtubPeachManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

// Peach floating in the bathtub during the final fight with Bowser.
class TBathtubPeach : public TSpineEnemy {
public:
	TBathtubPeach(const char* name = "バスタブピーチ");
	virtual ~TBathtubPeach() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	const TBathtubPeachParams* getParam() const;
	void changeAnm(int bck, int btp, f32 speed);
	void faceTo(const JGeometry::TVec3<f32>& pos, f32 speed);
	void goTo(const JGeometry::TVec3<f32>& pos);

public:
	/* 0x150 */ TBathtubBinder mBathtubBinder;
};

#endif
