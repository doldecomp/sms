#ifndef TELESA_HPP
#define TELESA_HPP

#include <Enemy/WalkerEnemy.hpp>

class SDLModelData;
class TSharedParts;

class TTelesaSaveLoadParams : public TWalkerEnemyParams {
public:
	TTelesaSaveLoadParams(const char* path);

	/* 0x32C */ TParamRT<s32> mSLFadeInTime;
	/* 0x340 */ TParamRT<s32> mSLAppearTime;
	/* 0x354 */ TParamRT<s32> mSLFadeOutTime;
	/* 0x368 */ TParamRT<f32> mSLFlyHeightMax;
	/* 0x37C */ TParamRT<f32> mSLFlyHeightMin;
	/* 0x390 */ TParamRT<f32> mSLFlyAmplitudeMax;
	/* 0x3A4 */ TParamRT<f32> mSLFlyAmplitudeMin;
	/* 0x3B8 */ TParamRT<f32> mSLFlyFrequencyMax;
	/* 0x3CC */ TParamRT<f32> mSLFlyFrequencyMin;
	/* 0x3E0 */ TParamRT<f32> mSLTelesaGravityY;
	/* 0x3F4 */ TParamRT<s32> mSLGenAttackerTime;
	/* 0x408 */ TParamRT<f32> mSLTelesaAttackGravityY;
	/* 0x41C */ TParamRT<f32> mSLTelesaPowerByWater;
	/* 0x430 */ TParamRT<s32> mSLLoopAppearTime;
	/* 0x444 */ TParamRT<s32> mSLLoopHideTime;

	// TODO: random interval class?
	/* 0x458 */ f32 unk458;
	/* 0x45C */ f32 unk45C;
	/* 0x460 */ f32 unk460;
	/* 0x464 */ f32 unk464;
	/* 0x468 */ f32 unk468;
	/* 0x46C */ f32 unk46C;
};

class TTelesa;

class TTelesaManager : public TSmallEnemyManager {
public:
	TTelesaManager(const char* name);
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual void setFlagOutOfCube();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void createEnemies(int);

	void telesaForceKill();
	void generatePetBottle(TTelesa*);

public:
	/* 0x60 */ u8 unk60;
	/* 0x61 */ u8 unk61;
	/* 0x64 */ SDLModelData* mModokiTelesaModel;
};

class TTelesa : public TWalkerEnemy {
public:
	TTelesa(const char* name = "テレサ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual void moveObject();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual BOOL isReachedToGoal() const;
	virtual void setBehavior();
	virtual void behaveToWater(THitActor*);
	virtual bool changeByJuice();
	virtual int getChangeBlockTime() { return 3000; }
	virtual void scalingChangeActor();
	virtual void changeOut();
	virtual void setWalkAnm();
	virtual void setWaitAnm();
	virtual void attackToMario();
	virtual void forceKill();
	virtual void setMActorAndKeeper();
	virtual void initAttacker(THitActor*);
	virtual bool isCollidMove(THitActor*);
	virtual BOOL isInhibitedForceMove() { return true; }
	virtual void decHpByWater(THitActor*) { }
	virtual bool isFindMario(f32) { return false; }
	virtual void walkBehavior(int, f32);

	void setFlyParam(f32);
	void reduceFlyForce();
	bool isFlying();
	bool isResetTransY();
	void initItemAttacker(THitActor*);
	void setAttacker();
	bool resetBaseGround();
	void setAttackPoint();
	void setFirstAttackPoint();
	void setTypeNormal();
	void setTypeLoop();
	void setTypeCanSee();
	void changeTevKColor();

	static f32 mFlyAngMax;

public:
	/* 0x194 */ TTelesaSaveLoadParams* unk194;
	/* 0x198 */ f32 mCurrentFlyHeight; // offset from ground
	/* 0x19C */ f32 mTargetFlyHeight;  // offset from ground
	/* 0x1A0 */ f32 mFlyBobOffsetY;    // offset from mCurrentFlyHeight
	/* 0x1A4 */ f32 mFlyBobAmplitude;
	/* 0x1A8 */ f32 mFlyBobPhase;
	/* 0x1AC */ f32 mFlyBobFrequency;
	/* 0x1B0 */ f32 mDampenedGroundHeight;
	/* 0x1B4 */ f32 mGroundHeightDampeningSpeed;
	/* 0x1B8 */ u8 unk1B8;
	enum { TELESA_TYPE_NORMAL, TELESA_TYPE_LOOP, TELESA_TYPE_CAN_SEE };
	/* 0x1BC */ int mTelesaType;
	/* 0x1C0 */ int mFadeLoopTimer; // used for waiting to fade in/out
	/* 0x1C4 */ TSharedParts* mImitatedBmd;
	/* 0x1C8 */ u8 unk1C8;
	/* 0x1C9 */ GXColor mTelesaFadeColor;
	/* 0x1CE */ GXColorS10 mTelesaBaseColor;
	/* 0x1D8 */ int mFadeTimer; // used for fading in/out
	enum {
		FADE_STATE_INVISIBLE,
		FADE_STATE_FADE_IN,
		FADE_STATE_FADE_OUT,
		FADE_STATE_VISIBLE
	};
	/* 0x1DC */ int mFadeState;
};

class TSeeTelesa : public TTelesa {
public:
	TSeeTelesa(const char* name);
	void load(JSUMemoryInputStream&);
};

class TLoopTelesa : public TTelesa {
public:
	TLoopTelesa(const char* name);
	void load(JSUMemoryInputStream&);
};

class TBoxTelesa : public TTelesa {
public:
	TBoxTelesa(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void reset();
	virtual bool isEatenByYosshi() { return false; }
	virtual bool isHitValid(u32);
};

class TMarioModokiTelesa : public TTelesa {
public:
	TMarioModokiTelesa(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);

	void imitateAnm();

public:
	enum { IMITATION_INDEX_NOT_IMITATING = 0 };
	/* 0x1E0 */ int mImitationIndex;
};

DECLARE_NERVE(TNerveTelesaImitate, TLiveActor);
DECLARE_NERVE(TNerveTelesaDie, TLiveActor);
DECLARE_NERVE(TNerveTelesaFreeze, TLiveActor);
DECLARE_NERVE(TNerveTelesaAttackMario, TLiveActor);
DECLARE_NERVE(TNerveTelesaTrample, TLiveActor);

class TKageMarioModokiManager : public TSmallEnemyManager {
public:
	TKageMarioModokiManager(const char* name)
	    : TSmallEnemyManager(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
};

class TKageMarioModoki : public TWalkerEnemy {
public:
	TKageMarioModoki(const char* name = "カゲマリオモドキ")
	    : TWalkerEnemy(name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void init(TLiveManager*);
};

DECLARE_NERVE(TNerveKageMarioModokiWait, TLiveActor);

#endif
