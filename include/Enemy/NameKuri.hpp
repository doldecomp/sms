#ifndef ENEMY_NAME_KURI_HPP
#define ENEMY_NAME_KURI_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/Launcher.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JParticle/JPACallback.hpp>

class TLiveActor;
class JPABaseEmitter;
class JPABaseParticle;
class TSharedParts;

class TNameKuriLauncher : public TLauncher {
public:
	TNameKuriLauncher(const char*);

	virtual void stateLaunch();
};

class TNameKuriSaveLoadParams : public TWalkerEnemyParams {
public:
	TNameKuriSaveLoadParams(const char* path);

	/* 0x32C */ TParamRT<s32> mSLAliveTime;
	/* 0x340 */ TParamRT<f32> mSLLandHeight;
	/* 0x354 */ TParamRT<f32> mSLJumpAttackGravity;
	/* 0x368 */ TParamRT<f32> mSLJumpAttackSp;
	/* 0x37C */ TParamRT<f32> mSLJumpAttackTurnSp;
	/* 0x390 */ TParamRT<s32> mSLJumpPrepareFrame;
	/* 0x3A4 */ TParamRT<f32> mSLJumpMaxFrame;
	/* 0x3B8 */ TParamRT<f32> mSLJumpMaxAngle;
	/* 0x3CC */ TParamRT<f32> mSLJumpMaxScale;
	/* 0x3E0 */ TParamRT<s32> mSLColorChangeRate;
	/* 0x3F4 */ TParamRT<f32> mSLJumpAttackRadius;
	/* 0x408 */ TParamRT<f32> mSLJumpAttackAngle;
};

class TNameKuriManager : public TSmallEnemyManager {
public:
	TNameKuriManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	static f32 mExplosionSpeed;
	static int mStopMinScaleFrame;

public:
	/* 0x60 */ int unk60;
};

class TNameKuri;

class TNameIndParCallback
    : public JPACallBackBase2<JPABaseEmitter*, JPABaseParticle*> {
public:
	TNameIndParCallback(TNameKuri*);
	void execute(JPABaseEmitter*, JPABaseParticle*);
	void draw(JPABaseEmitter*, JPABaseParticle*);

public:
	/* 0x4 */ TNameKuri* mOwner;
};

class TNameKuri : public TWalkerEnemy {
public:
	TNameKuri(const char* name = "ナメクリ");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual void setGenerateAnm();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setMeltAnm();
	virtual void setWaitAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*);
	virtual void endHitWaterJump();
	virtual void setBckAnm(int);
	virtual void setAfterDeadEffect();
	virtual void behaveToFindMario();

	bool isAttackJump() const;
	bool isHitWaterJump() const;
	bool canJumpAttack() const;

public:
	/* 0x194 */ int unk194;
	/* 0x198 */ u8 unk198;
	/* 0x19C */ TNameIndParCallback unk19C;
	/* 0x1A4 */ TNameKuriSaveLoadParams* unk1A4;
	/* 0x1A8 */ bool unk1A8;
	/* 0x1AC */ f32 unk1AC;
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ f32 unk1B8;
	/* 0x1BC */ GXColorS10 unk1BC;
	/* 0x1C4 */ GXColorS10 unk1C4;
	/* 0x1CC */ TSharedParts* unk1CC;
};

class TDiffusionNameKuriManager : public TNameKuriManager {
public:
	TDiffusionNameKuriManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual TSmallEnemy* createEnemyInstance();
};

class TDiffusionNameKuri : public TNameKuri {
public:
	TDiffusionNameKuri(const char* name = "拡散ナメクリ")
	    : TNameKuri(name)
	{
	}
};

DECLARE_NERVE(TNerveNameKuriLand, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttack, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor);
DECLARE_NERVE(TNerveNameKuriExplosion, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDiffuse, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDrawPollute, TLiveActor);
DECLARE_NERVE(TNerveNKFollowMario, TLiveActor);

#endif
