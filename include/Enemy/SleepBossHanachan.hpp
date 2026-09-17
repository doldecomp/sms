#ifndef ENEMY_SLEEP_BOSS_HANACHAN_HPP
#define ENEMY_SLEEP_BOSS_HANACHAN_HPP

#include <Enemy/DemoBossHanachan.hpp>
#include <Strategic/Nerve.hpp>

class TMirrorActor;

class TSleepBossHanachan : public TDemoBossHanachan {
public:
	TSleepBossHanachan(const char* name)
	    : TDemoBossHanachan(name)
	    , mShinePosition(0.0f, 0.0f, 0.0f)
	    , mMirrorActor(nullptr)
	{
	}

	virtual ~TSleepBossHanachan() { }
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;

	void startFall(f32, f32, f32);

public:
	/* 0x150 */ JGeometry::TVec3<f32> mShinePosition;
	/* 0x15C */ TMirrorActor* mMirrorActor;
};

class TSleepBossHanachanManager : public TDemoBossHanachanManager {
public:
	TSleepBossHanachanManager(const char* name)
	    : TDemoBossHanachanManager(name, "/enemy/sleepBossHanachan.prm")
	{
	}

	virtual ~TSleepBossHanachanManager() { }
	virtual void createModelData();
};

DECLARE_NERVE(TNerveSBH_SleepContinue, TLiveActor);
DECLARE_NERVE(TNerveSBH_Fall, TLiveActor);

#endif
