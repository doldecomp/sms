#ifndef ENEMY_DEMO_BOSS_HANACHAN_HPP
#define ENEMY_DEMO_BOSS_HANACHAN_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TDemoBossHanachanSaveParams;

class TDemoBossHanachan : public TSpineEnemy {
public:
	TDemoBossHanachan(const char* name)
	    : TSpineEnemy(name)
	{
	}

	virtual ~TDemoBossHanachan() { }
	virtual BOOL receiveMessage(THitActor*, u32);

	void initBase(TLiveManager*, u32);
};

class TDemoBossHanachanManager : public TEnemyManager {
public:
	virtual ~TDemoBossHanachanManager() { }
	virtual void clipEnemies(JDrama::TGraphics*);

public:
	/* 0x54 */ TDemoBossHanachanSaveParams* mSaveParams;
};

class TDemoBossHanachanSaveParams : public TParams {
public:
	TDemoBossHanachanSaveParams(const char*);

	/* 0x08 */ TParamRT<f32> mSLViewClipFar;
	/* 0x1C */ TParamRT<f32> mSLViewClipRadius;
};

#endif
