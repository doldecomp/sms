#ifndef ENEMY_BOSS_WANWAN_HPP
#define ENEMY_BOSS_WANWAN_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class TBossWanwanMtxCalc;
class TBWLeash;
class TBWPicket;
class TBWHit;

class TBossWanwan : public TSpineEnemy {
public:
	TBossWanwan(const char*);
	virtual ~TBossWanwan() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void control();
	virtual void kill();

	void emitEffects();
	void slideToCurPathNode(f32, f32);
	void shakeCamera(int);
	void changeBck(int);
	bool isBurning();
	bool isHeadPulled();
	bool isMarioInSight();
	bool isTailBurning();
	void releasePicket();
	void reverseNextGraphNode();
	void rollNextGraphNode();
	void showMessage(u32);
	void startGoldBrk();
	void takeBath();

public:
	/* 0x150 */ TBossWanwanMtxCalc* unk150;
	/* 0x154 */ TBWLeash* unk154;
	/* 0x158 */ TBWPicket* unk158;
	/* 0x15C */ JGeometry::TVec3<f32> unk15C;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ s32 unk16C;
	/* 0x170 */ TBWHit* unk170;
	/* 0x174 */ TBWHit* unk174;
	/* 0x178 */ f32 unk178;
	/* 0x17C */ s32 unk17C;
	/* 0x180 */ s32 unk180;
	/* 0x184 */ s32 unk184;
	/* 0x188 */ s32 unk188;
	/* 0x18C */ u8 unk18C;
	/* 0x18D */ u8 unk18D;
	/* 0x190 */ s32 unk190;
	/* 0x194 */ u8 unk194;
	/* 0x195 */ u8 unk195;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ s32 unk19C;
	/* 0x1A0 */ u8 unk1A0;
	/* 0x1A4 */ JGeometry::TVec3<f32> unk1A4;
	/* 0x1B0 */ s32 unk1B0;
	/* 0x1B4 */ s16 unk1B4;
};

class TBossWanwanManager : public TEnemyManager {
public:
	TBossWanwanManager(const char*);
	virtual ~TBossWanwanManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	void initJParticle();
};

DECLARE_NERVE(TNerveBWGraphWander, TLiveActor);
DECLARE_NERVE(TNerveBWRoll, TLiveActor);
DECLARE_NERVE(TNerveBWBark, TLiveActor);
DECLARE_NERVE(TNerveBWJump, TLiveActor);
DECLARE_NERVE(TNerveBWStun, TLiveActor);
DECLARE_NERVE(TNerveBWWakeup, TLiveActor);
DECLARE_NERVE(TNerveBWJumpToBath, TLiveActor);
DECLARE_NERVE(TNerveBWDie, TLiveActor);
DECLARE_NERVE(TNerveBWJumpAway, TLiveActor);
DECLARE_NERVE(TNerveBWShake, TLiveActor);
DECLARE_NERVE(TNerveBWFall, TLiveActor);

#endif
