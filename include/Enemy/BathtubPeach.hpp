#ifndef ENEMY_BATHTUBPEACH_HPP
#define ENEMY_BATHTUBPEACH_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/BathtubBinder.hpp>

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TBathtubPeachManager::load stores into each TParamRT. The
// constructor is defined in the class body because the map records no symbol
// for it at all -- not even an UNUSED one -- so it was weak and inlined away.
class TBathtubPeachParams : public TSpineEnemyParams {
public:
	TBathtubPeachParams(const char* prm)
	    : TSpineEnemyParams(prm)
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

// The rubber-duck Peach ("ahiru_peach") that paddles around the bathtub at the
// top of Corona Mountain. She keeps a fixed angular distance from Mario around
// the tub's centre, riding the bath water on her own TBathtubBinder, and is
// parented straight to the tub's demo matrix while the ending cutscene plays.
class TBathtubPeach : public TSpineEnemy {
public:
	TBathtubPeach(const char* name);

	// Declared in vtable order; TBathtubPeach adds no new slots to
	// TSpineEnemy.
	virtual ~TBathtubPeach() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void goTo(const JGeometry::TVec3<f32>& goal);
	void faceTo(const JGeometry::TVec3<f32>& target, f32 turn_speed);
	void changeAnm(int bck, int btp, f32 rate);
	TBathtubPeachParams* getParam() const;

	// One inline level above getParam(), which is what puts
	// TEnemyManager::getSaveParam() past its depth allowance at goTo's
	// setLength argument: the ROM reads speed twice there, inlined for the
	// comparison and through a real bl getSaveParam for the argument, and
	// emits the weak getSaveParam__13TEnemyManagerCFv the map lists as a
	// duplicate in this TU. Always inlined, so the map records no symbol.
	f32 getSpeed() const { return getParam()->speed.get(); }

public:
	/* 0x150 */ TBathtubBinder mBathtubBinder;
};

class TBathtubPeachManager : public TEnemyManager {
public:
	TBathtubPeachManager(const char* name);

	virtual ~TBathtubPeachManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
