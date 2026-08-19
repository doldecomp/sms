#ifndef ENEMY_DEMO_BOSS_HANACHAN_BASE_HPP
#define ENEMY_DEMO_BOSS_HANACHAN_BASE_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TDemoBossHanachanSaveParams : public TParams {
public:
	TDemoBossHanachanSaveParams(const char* prm);

public:
	/* 0x08 */ TParamRT<f32> mSLViewClipFar;
	/* 0x1C */ TParamRT<f32> mSLViewClipRadius;
};

class TDemoBossHanachan : public TSpineEnemy {
public:
	TDemoBossHanachan(const char* name)
	    : TSpineEnemy(name)
	{
	}

	virtual ~TDemoBossHanachan() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void initBase(TLiveManager* manager, u32 model_flags);
};

class TDemoBossHanachanManager : public TEnemyManager {
public:
	TDemoBossHanachanManager(const char* name)
	    : TEnemyManager(name)
	{
	}

	virtual ~TDemoBossHanachanManager() { }
	virtual void clipEnemies(JDrama::TGraphics* graphics);

public:
	/* 0x54 */ TDemoBossHanachanSaveParams* mSaveParams;
};

#endif
