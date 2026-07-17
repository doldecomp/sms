#ifndef ENEMY_BOSSMANTA_HPP
#define ENEMY_BOSSMANTA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>
#include <JSystem/JUtility/JUTColor.hpp>

class TBossMantaParams;
class TBossMantaManager;

class TBossManta : public TSpineEnemy {
public:
	TBossManta(const char* name = "ボスマンタ");

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void moveObject();
	virtual void drawObject(JDrama::TGraphics*) { }
	virtual void reset() { }

	void updateAttractor();
	bool isPolluting();
	f32 getPolluteRadius();
	void initNthGeneration(int);
	bool collidedWithWater();
	bool getIntoGraphVec(JGeometry::TVec3<f32>*);

	bool isDamageable();
	void updateEpilogueFrame();
	f32 getEpilogueValue();
	void resetDamageAnimEpilogue();
	void startDamageAnimEpilogue();
	BOOL isSpawnState();
	void updateAnimBlend();
	void startDamageAnim();
	void startWalkAnim();
	void setCollision();
	f32 getTailAnimSpeed();

	// fabricated
	const TBossMantaParams* getSaveParams() const
	{
		return (const TBossMantaParams*)getSaveParam();
	}
	TBossMantaManager* getManager() { return (TBossMantaManager*)mManager; }

	static int sCenterJointIndex;
	static int sBodyJointIndex;
	static int sRwingJointIndex;
	static int sLwingJointIndex;
	static u8 sEscapeFromMario;
	static f32 sFrameRate[6];
	static f32 sScale[6];

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ s32 unk154;
	/* 0x158 */ JGeometry::TVec3<f32> unk158;
	/* 0x164 */ JGeometry::TVec3<f32> unk164;
	/* 0x170 */ JGeometry::TVec3<f32> unk170;
	/* 0x17C */ JGeometry::TVec3<f32> unk17C;
	/* 0x188 */ s32 unk188;
	/* 0x18C */ s32 mGeneration;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ s32 unk1A0;
	/* 0x1A4 */ s32 unk1A4;
};

class TBossMantaAdditionalCollision : public THitActor {
public:
	TBossMantaAdditionalCollision(const char* name);

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor*, u32);

	void setManta(TBossManta*);

private:
	/* 0x68 */ TBossManta* unk68;
};

class TBossMantaAdditionalCollisionSet {
public:
	TBossMantaAdditionalCollisionSet();
	void update(u32 cue, JDrama::TGraphics* graphics);
	void adapt(TBossManta*);

	bool isUsed();

public:
	/* 0x0 */ TBossMantaAdditionalCollision* unk0[3];
	/* 0xC */ TBossManta* unkC;
};

class TBossMantaParams : public TSpineEnemyParams {
public:
	TBossMantaParams(const char*);

public:
	/* 0xA8 */ TParamRT<f32> mSLPolluteRadius;
	/* 0xBC */ TParamRT<s32> mSLDamageEffectNum;
	/* 0xD0 */ TParamRT<f32> mSLAppearDemoInitialZ;
	/* 0xE4 */ TParamRT<f32> mSLAppearDemoWalkSpeed;
	/* 0xF8 */ TParamRT<s32> mSLMantaRed;
	/* 0x10C */ TParamRT<s32> mSLMantaGreen;
	/* 0x120 */ TParamRT<s32> mSLMantaBlue;
	/* 0x134 */ TParamRT<s32> mSLMantaAlpha;
	/* 0x148 */ TParamRT<s32> mSLAngryMantaRed;
	/* 0x15C */ TParamRT<s32> mSLAngryMantaGreen;
	/* 0x170 */ TParamRT<s32> mSLAngryMantaBlue;
	/* 0x184 */ TParamRT<s32> mSLAngryMantaAlpha;
	/* 0x198 */ TParamRT<f32> mSLAttractorPower;
	/* 0x1AC */ TParamRT<f32> mSLPusherPower;
	/* 0x1C0 */ TParamRT<f32> mSLEscapeLookPoint;
	/* 0x1D4 */ TParamRT<f32> mSLEscapeLookedPoint;
	/* 0x1E8 */ TParamRT<f32> mSLEscapeRegion;
};

class TBossMantaManager : public TEnemyManager {
public:
	class TMantaMessageState {
	public:
		TMantaMessageState(TBossMantaManager* owner)
		    : unk0(owner)
		    , unk4(0)
		{
		}

		void update();

	public:
		TBossMantaManager* unk0;
		s32 unk4;
	};
	class TMantaBattleState {
	public:
		TMantaBattleState(TBossMantaManager* owner)
		    : unk0(owner)
		    , mState(0)
		{
		}

		void update();

	public:
		TBossMantaManager* unk0;
		s32 mState;
	};

	TBossMantaManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void createEnemies(int);

	void spawn(int, const JGeometry::TVec3<f32>&);
	void setupEfbAlpha(JDrama::TGraphics*);
	void updateMantaEscape();
	void drawMantaShadow(JDrama::TGraphics*);

	void initAdditionalCollision();
	void createEnemy();
	const JUtility::TColor& getMantaColor();
	void loadEffects();
	void adaptAdditionalCollision(TBossManta*);

public:
	/* 0x54 */ TBossMantaAdditionalCollisionSet* mCollisionSets[8];
	/* 0x74 */ JGeometry::TVec3<f32>* unk74;
	/* 0x78 */ JGeometry::TVec3<f32>* unk78;
	/* 0x7C */ u8* unk7C;
	/* 0x80 */ JUtility::TColor unk80;
	/* 0x84 */ s32 unk84;
	/* 0x88 */ TMantaBattleState unk88;
	/* 0x90 */ TMantaMessageState unk90;
};

DECLARE_NERVE(TNerveMantaSpawn, TLiveActor);
DECLARE_NERVE(TNerveMantaMove, TLiveActor);
DECLARE_NERVE(TNerveMantaAppearDemo, TLiveActor);
DECLARE_NERVE(TNerveMantaDeath, TLiveActor);
DECLARE_NERVE(TNerveMantaHitWater, TLiveActor);

#endif
