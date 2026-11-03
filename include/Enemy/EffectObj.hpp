#ifndef ENEMY_EFFECT_OBJ_HPP
#define ENEMY_EFFECT_OBJ_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JGadget/std-list.hpp>

class TSimpleEffect : public JDrama::TActor {
public:
	TSimpleEffect(const char* name)
	    : JDrama::TActor(name)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void emitEffect() = 0;

	MtxPtr getEffectMtx() { return mEffectMtx; }

public:
	/* 0x44 */ u32 unk44;
	/* 0x48 */ Mtx mEffectMtx;
};

class TEffectPinnaFunsui : public TSimpleEffect {
public:
	TEffectPinnaFunsui(const char* name = "ピンナ噴水エフェクト")
	    : TSimpleEffect(name)
	{
	}

	virtual void loadAfter();
	virtual void emitEffect();
};

class TEffectBiancoFunsui : public TSimpleEffect {
public:
	TEffectBiancoFunsui(const char* name = "ビアンコ噴水エフェクト")
	    : TSimpleEffect(name)
	{
	}

	virtual void loadAfter();
	virtual void emitEffect();
};

class TEffectObjBase : public THitActor {
public:
	enum {
		STATE_INACTIVE     = 0,
		STATE_READY        = 1,
		STATE_ACTIVE       = 2,
		STATE_EXTINGUISHED = 3,
	};

	TEffectObjBase(const char* name = "<EffectObjBase>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void moveObject();

	void forceKill();

	f32 getFireScale() const { return mFireStrength / mMaxFireStrength; }

public:
	/* 0x68 */ int mState;
	/* 0x6C */ f32 mMaxFireStrength;
	/* 0x70 */ f32 mFireStrength;
	/* 0x74 */ u32 mActiveFrameCount;
};

class TEffectObjManager;
extern TEffectObjManager* gpEffectObjManager;

class TEffectObjManager : public JDrama::TViewObj {
public:
	TEffectObjManager(const char* name = "<EffectObjManager>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void generateEffect(Vec);
	void addListEffectObj(TEffectObjBase*);

public:
	enum { EFFECT_OBJ_POOL_SIZE = 5 };

	/* 0x10 */ u32 mRegisteredEffectCount;
	/* 0x14 */ JGadget::TList<TEffectObjBase*> mEffectObjList;
	/* 0x24 */ TEffectObjBase** mEffectObjPool;
};

class TEffectModel : public TSpineEnemy {
public:
	TEffectModel(const char* name)
	    : TSpineEnemy(name)
	{
	}

	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();
	virtual void control() { }
	virtual void moveObject();
	virtual void reset();
};

class TEffectModelManager : public TEnemyManager {
public:
	TEffectModelManager(const char* name)
	    : TEnemyManager(name)
	{
	}
};

class TEffectColumWaterManager : public TEffectModelManager {
public:
	TEffectColumWaterManager(const char* name = "エフェクト水柱マネージャー");
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectColumWater : public TEffectModel {
public:
	TEffectColumWater(const char* name = "エフェクト水柱");
	virtual void init(TLiveManager* manager);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

class TEffectBombColumWaterManager : public TEffectModelManager {
public:
	TEffectBombColumWaterManager(const char* name
	                             = "エフェクト爆発水柱マネージャー");
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectBombColumWater : public TEffectModel {
public:
	TEffectBombColumWater(const char* name = "エフェクト爆発水柱");
	virtual void init(TLiveManager* manager);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

class TEffectColumSandManager : public TEffectModelManager {
public:
	TEffectColumSandManager(const char* name = "エフェクト砂柱マネージャー");
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectColumSand : public TEffectModel {
public:
	TEffectColumSand(const char* name = "エフェクト砂柱");
	virtual void init(TLiveManager* manager);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

class TEffectExplosionManager : public TEffectModelManager {
public:
	TEffectExplosionManager(const char* name = "エフェクト爆発マネージャー");
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectExplosion : public TEffectModel {
public:
	TEffectExplosion(const char* name = "エフェクト爆発");
	virtual void init(TLiveManager* manager);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

#endif
