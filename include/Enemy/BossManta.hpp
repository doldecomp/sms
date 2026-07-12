#ifndef ENEMY_BOSSMANTA_HPP
#define ENEMY_BOSSMANTA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class TBossManta : public TSpineEnemy {
public:
	TBossManta(const char*);

	virtual void init(TLiveManager*);
	virtual void control();
	virtual void moveObject();
	virtual void calcRootMatrix();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void reset();
	virtual BOOL receiveMessage(THitActor*, u32);

	void updateAttractor();
	f32 getPolluteRadius();
	void initNthGeneration(int);
	BOOL collidedWithWater();
	void getIntoGraphVec(JGeometry::TVec3<f32>*);

	static int sCenterJointIndex;
	static f32 sScale[];

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ s32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ f32 unk16C;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ f32 unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ JGeometry::TVec3<f32> unk17C;
	/* 0x188 */ s32 unk188;
	/* 0x18C */ s32 unk18C;
	/* 0x190 */ f32 unk190;
	/* 0x194 */ f32 unk194;
	/* 0x198 */ f32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ s32 unk1A0;
};

class TBossMantaAdditionalCollision : public THitActor {
public:
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
};

class TBossMantaAdditionalCollisionSet {
public:
	TBossMantaAdditionalCollisionSet();
	void update(u32, JDrama::TGraphics*);
	void adapt(TBossManta*);

public:
	/* 0x0 */ THitActor* unk0;
	/* 0x4 */ u8 pad4[8];
	/* 0xC */ TBossManta* unkC;
};

class TBossMantaParams : public TSpineEnemyParams {
public:
	TBossMantaParams(const char*);

public:
	/* 0xA8 */ TParamRT<f32> mSLPolluteRadius;
	/* 0xBC */ TParamRT<int> mSLDamageEffectNum;
	/* 0xD0 */ TParamRT<f32> mSLAppearDemoInitialZ;
	/* 0xE4 */ TParamRT<f32> mSLAppearDemoWalkSpeed;
	/* 0xF8 */ TParamRT<int> mSLMantaRed;
	/* 0x10C */ TParamRT<int> mSLMantaGreen;
	/* 0x120 */ TParamRT<int> mSLMantaBlue;
	/* 0x134 */ TParamRT<int> mSLMantaAlpha;
	/* 0x148 */ TParamRT<int> mSLAngryMantaRed;
	/* 0x15C */ TParamRT<int> mSLAngryMantaGreen;
	/* 0x170 */ TParamRT<int> mSLAngryMantaBlue;
	/* 0x184 */ TParamRT<int> mSLAngryMantaAlpha;
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
		void update();
		TBossMantaManager* unk0;
		u8 unk4;
	};
	class TMantaBattleState {
	public:
		void update();
		TBossMantaManager* unk0;
		u8 unk4;
	};

	TBossMantaManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void createModelData();

	void spawn(int, const JGeometry::TVec3<f32>&);
	virtual void createEnemies(int);
	void setupEfbAlpha(JDrama::TGraphics*);
	void updateMantaEscape();
	void drawMantaShadow(JDrama::TGraphics*);
	virtual TSpineEnemy* createEnemyInstance();

public:
	/* 0x54 */ TBossMantaAdditionalCollisionSet* mCollisionSets[8];
	/* 0x74 */ JGeometry::TVec3<f32>* unk74;
	/* 0x78 */ JGeometry::TVec3<f32>* unk78;
	/* 0x7C */ u8* unk7C;
	/* 0x80 */ s32 unk80;
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
