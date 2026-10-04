#ifndef ENEMY_BATHTUBPEACH_HPP
#define ENEMY_BATHTUBPEACH_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/BathtubBinder.hpp>
#include <Enemy/EnemyManager.hpp>

class TBathtubPeachParams;

class TBathtubPeach : public TSpineEnemy {
public:
	TBathtubPeach(const char*);
	virtual ~TBathtubPeach() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void changeAnm(int, int, f32);
	void faceTo(const JGeometry::TVec3<f32>&, f32);
	void goTo(const JGeometry::TVec3<f32>&);
	TBathtubPeachParams* getParam() const;

public:
	/* 0x150 */ TBathtubBinder unk150;
};

class TBathtubPeachManager : public TEnemyManager {
public:
	TBathtubPeachManager(const char*);

	virtual ~TBathtubPeachManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
