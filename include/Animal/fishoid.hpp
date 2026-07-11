#ifndef ANIMAL_FISHOID_HPP
#define ANIMAL_FISHOID_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/TakeActor.hpp>
#include <dolphin/mtx.h>

class MActor;
class TBoid;
class TBoidLeader;
class TRealoidActor;
class TMapObjBase;

class TRealoid : public TSpineEnemy {
public:
	TRealoid(const char*);

	virtual ~TRealoid();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TRealoidActor* createRealoidActor(MActor*) = 0;

	void clipBoids(JDrama::TGraphics*);
	void loadDefault(JSUMemoryInputStream&, const char*, int);

public:
	/* 0x150 */ TBoidLeader* unk150;
	/* 0x154 */ TRealoidActor** unk154;
};

class TRealoidActor : public TTakeActor {
public:
	TRealoidActor(MActor*);

	virtual ~TRealoidActor();
	virtual MtxPtr getTakingMtx();
	virtual void perform(u32, JDrama::TGraphics*);

	void checkHitActors();
	void calcRootMatrix(TBoid*);

public:
	/* 0x70 */ MActor* unk70;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ Mtx unk78;
};

class TFish : public TRealoidActor {
public:
	TFish(MActor* actor)
	    : TRealoidActor(actor)
	{
	}

	virtual ~TFish();

	void init();
};

class TFishoid : public TRealoid {
public:
	TFishoid(int, const char*);

	virtual ~TFishoid();
	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TRealoidActor* createRealoidActor(MActor*);

public:
	/* 0x158 */ int unk158;
	/* 0x15C */ TMapObjBase* unk15C;
};

class TFishoidManager : public TEnemyManager {
public:
	TFishoidManager(const char*);

	virtual ~TFishoidManager();
	virtual void createModelData();
};

#endif
