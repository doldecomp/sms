#ifndef ENEMY_BATHTUB_PEACH_HPP
#define ENEMY_BATHTUB_PEACH_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/BathtubBinder.hpp>

// TODO: guessed name
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

class TBathtubPeach : public TSpineEnemy {
public:
	TBathtubPeach(const char* name = "バスタブピーチ");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	TBathtubPeachParams* getParam() const;
	void changeAnm(int, int, f32);
	void faceTo(const JGeometry::TVec3<f32>&, f32);
	void goTo(const JGeometry::TVec3<f32>&);

public:
	/* 0x150 */ TBathtubBinder unk150;
};

class TBathtubPeachManager : public TEnemyManager {
public:
	TBathtubPeachManager(const char* name = "バスタブピーチマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
