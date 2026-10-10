#ifndef ENEMY_DEMOBOSSHANACHAN_HPP
#define ENEMY_DEMOBOSSHANACHAN_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <System/BaseParam.hpp>
#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <dolphin/types.h>

class THitActor;
class TLiveManager;
class TDemoBossHanachan;
class TDemoBossHanachanManager;
class TDemoBossHanachanSaveParams;

class TDemoBossHanachan : public TSpineEnemy {
public:
	TDemoBossHanachan(const char* name = "?")
	    : TSpineEnemy(name)
	{
	}
	virtual BOOL receiveMessage(THitActor*, u32);

	void initBase(TLiveManager*, u32);
};

class TDemoBossHanachanSaveParams : public TParams {
public:
	TDemoBossHanachanSaveParams(const char*);

public:
	/* 0x8  */ TParamRT<f32> mSLViewClipFar;
	/* 0x1C */ TParamRT<f32> mSLViewClipRadius;
};

class TDemoBossHanachanManager : public TEnemyManager {
public:
	TDemoBossHanachanManager(const char* name, const char* prm_path)
	    : TEnemyManager(name)
	    , mSaveParams(new TDemoBossHanachanSaveParams(prm_path))
	{
	}
	virtual void clipEnemies(JDrama::TGraphics*);

public:
	/* 0x54 */ TDemoBossHanachanSaveParams* mSaveParams;
};

#endif // ENEMY_DEMOBOSSHANACHAN_HPP
