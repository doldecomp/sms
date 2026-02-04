#ifndef ENEMY_EFFECT_OBJ_HPP
#define ENEMY_EFFECT_OBJ_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

// rogue includes needed for matching sinit & bss
#include <MSound/MSSetSound.hpp>
#include <MSound/MSoundBGM.hpp>

class TSimpleEffect : public JDrama::TActor {
public:
	TSimpleEffect(const char* name);

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void emitEffect() = 0;

	MtxPtr getUnk48() { return unk48; }

public:
	/* 0x44 */ u32 unk44;
	/* 0x48 */ Mtx unk48;
};

class TEffectPinnaFunsui : public TSimpleEffect {
public:
	TEffectPinnaFunsui(const char*);

	virtual void loadAfter();
	virtual void emitEffect();
};

class TEffectBiancoFunsui : public TSimpleEffect {
public:
	TEffectBiancoFunsui(const char*);

	virtual void loadAfter();
	virtual void emitEffect();
};

class TEffectObjBase : public THitActor {
public:
	TEffectObjBase(const char* name = "<EffectObjBase>");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init();
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void moveObject();

	void forceKill();

public:
	/* 0x68 */ int unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ u32 unk74;
};

class TEffectObjManager;
extern TEffectObjManager* gpEffectObjManager;

class TEffectObjManager : public JDrama::TViewObj {
public:
	TEffectObjManager(const char* name = "<EffectObjManager>");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void generateEffect(Vec);
	void addListEffectObj(TEffectObjBase*);

	// TODO: why are there 5 always existing "special" effect objects?
	enum { COOL_EFFECT_OBJ_NUM = 5 };

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ JGadget::TList<TEffectObjBase*> unk14;
	/* 0x24 */ TEffectObjBase** unk24;
};

class TEffectModel : public TSpineEnemy {
public:
	TEffectModel(const char* name)
	    : TSpineEnemy(name)
	{
	}

	virtual void init(TLiveManager*);
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
	TEffectColumWaterManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectColumWater : public TEffectModel {
public:
	TEffectColumWater(const char* name = "エフェクト水柱");
	virtual void init(TLiveManager*);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

class TEffectBombColumWaterManager : public TEffectModelManager {
public:
	TEffectBombColumWaterManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectBombColumWater : public TEffectModel {
public:
	TEffectBombColumWater(const char* name = "エフェクト爆発水柱");
	virtual void init(TLiveManager*);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

class TEffectColumSandManager : public TEffectModelManager {
public:
	TEffectColumSandManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectColumSand : public TEffectModel {
public:
	TEffectColumSand(const char* name = "エフェクト砂柱");
	virtual void init(TLiveManager*);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

class TEffectExplosionManager : public TEffectModelManager {
public:
	TEffectExplosionManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TEffectModel* createEnemyInstance();
};

class TEffectExplosion : public TEffectModel {
public:
	TEffectExplosion(const char* name = "エフェクト爆発");
	virtual void init(TLiveManager*);
	virtual void reset();

	void generate(JGeometry::TVec3<f32>& position,
	              JGeometry::TVec3<f32>& scale);
};

#endif
