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

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual TRealoidActor* createRealoidActor(MActor*) = 0;

	void clipBoids(JDrama::TGraphics*);
	void loadDefault(JSUMemoryInputStream&, const char*, int);

	TRealoidActor* getRealoid(int idx) { return unk154[idx]; }

public:
	/* 0x150 */ TBoidLeader* unk150;
	/* 0x154 */ TRealoidActor** unk154;
};

class TRealoidActor : public TTakeActor {
public:
	TRealoidActor(MActor*);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual MtxPtr getTakingMtx();
	virtual void init() = 0;

	void checkHitActors();
	void calcRootMatrix(TBoid*);
	void calcRootMatrixOnTaking();

	void onFlag(int flag) { unk74 |= flag; }
	void offFlag(int flag) { unk74 &= ~flag; }

public:
	/* 0x70 */ MActor* unk70;
	/* 0x74 */ u32 unk74;
	/* 0x78 */ TPosition3f unk78;
};

class TFish : public TRealoidActor {
public:
	TFish(MActor* actor)
	    : TRealoidActor(actor)
	{
	}

	virtual void init();
};

class TFishoid : public TRealoid {
public:
	TFishoid(int type, const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual TRealoidActor* createRealoidActor(MActor*);

	void initBoids();
	void performItem(u32, JDrama::TGraphics*);
	void loadItem(JSUMemoryInputStream&);

public:
	/* 0x158 */ int mType;
	/* 0x15C */ TMapObjBase* unk15C;
};

class TFishoidManager : public TEnemyManager {
public:
	TFishoidManager(const char* name = "回遊魚マネージャー");

	virtual ~TFishoidManager();
	virtual void createModelData();
};

#endif
