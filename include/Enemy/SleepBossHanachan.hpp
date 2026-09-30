#ifndef ENEMY_SLEEPBOSSHANACHAN_HPP
#define ENEMY_SLEEPBOSSHANACHAN_HPP

#include <Enemy/DemoBossHanachan.hpp>

class TMirrorActor;

class TSleepBossHanachan : public TDemoBossHanachan {
public:
	TSleepBossHanachan(const char* name = "?");

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;

	void startFall(f32, f32, f32);

public:
	/* 0x150 */ JGeometry::TVec3<f32> mShinePosition;
	/* 0x15C */ TMirrorActor* unk15C;
};

class TSleepBossHanachanManager : public TDemoBossHanachanManager {
public:
	TSleepBossHanachanManager(const char* name = "?");

	virtual void createModelData();
};

#endif
