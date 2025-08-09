#ifndef RICCOHOOK_H
#define RICCOHOOK_H

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <Strategic/Strategy.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>

class TRiccoHook;

class TRiccoHookParams : public TSpineEnemyParams {
public:
	TRiccoHookParams(const char* path);

	/* 0xA8 */ TParamRT<f32> mSLHitHeight;
	/* 0xBC */ TParamRT<f32> mSLHitRadius;
	/* 0xD0 */ TParamRT<f32> mSLHangRadius;
	/* 0xE4 */ TParamRT<f32> mSLMoveSpeed;
};

class TRiccoHookManager : public TEnemyManager {
public:
    TRiccoHookManager(const char* name);

    virtual void load(JSUMemoryInputStream&);
    virtual void perform(u32, JDrama::TGraphics*);
    virtual TSpineEnemy* createEnemyInstance();
    virtual void createModelData();

    TRiccoHook* getObj(int i) { return (TRiccoHook*)TObjManager::getObj(i); }
};

class THookTake;

class TRiccoHook : public TSpineEnemy {
public:
    TRiccoHook(const char* name = "フック");

    virtual void init(TLiveManager*);
    virtual void kill();
    virtual BOOL receiveMessage(THitActor*, u32);
    virtual void perform(u32, JDrama::TGraphics*);

    TRiccoHookParams* getSaveLoadParam() const
	{
		return (TRiccoHookParams*)getSaveParam();
	}

public:
    THookTake* mHookTake;
    int mTimer;
};

class THookTake : public TTakeActor {
public:
    THookTake(TRiccoHook* owner, const char* name = "フックつかみ")
        : TTakeActor(name)
        , mOwner(owner)
    {
        initHitActor(0x400000BB, 1, -0x80000000,
            mOwner->getSaveLoadParam()->mSLHitRadius.get(),
            mOwner->getSaveLoadParam()->mSLHitHeight.get(),
            mOwner->getSaveLoadParam()->mSLHitRadius.get(),
            mOwner->getSaveLoadParam()->mSLHitHeight.get());

        JDrama::TNameRefGen::search<TIdxGroupObj>("オブジェクトグループ")->getChildren().push_back(this);
    }

    virtual ~THookTake() { }

    virtual void perform(u32, JDrama::TGraphics*);
    virtual BOOL receiveMessage(THitActor*, u32);
    virtual MtxPtr getTakingMtx();
    virtual f32 getRadiusAtY(f32 y) const;

public:
    TRiccoHook* mOwner;
};

DECLARE_NERVE(TNerveRHGraphWander, TLiveActor);

#endif /* RICCOHOOK_H */
