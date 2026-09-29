#ifndef ENEMY_KAZEKUN_HPP
#define ENEMY_KAZEKUN_HPP

#include <Enemy/SmallEnemy.hpp>
#include <JSystem/JGeometry/JGQuat4.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class THitActor;
class TLiveManager;

DECLARE_NERVE(TNerveKazekunHitWater, TLiveActor);

DECLARE_NERVE(TNerveKazekunDisappear, TLiveActor);

DECLARE_NERVE(TNerveKazekunWait, TLiveActor);

DECLARE_NERVE(TNerveKazekunSearch, TLiveActor);

DECLARE_NERVE(TNerveKazekunAttack, TLiveActor);

DECLARE_NERVE(TNerveKazekunPreAttack, TLiveActor);

DECLARE_NERVE(TNerveKazekunTurn, TLiveActor);

DECLARE_NERVE(TNerveKazekunAppear, TLiveActor);

class TKazekunManager : public TSmallEnemyManager {
public:
	TKazekunManager(const char* name = "かぜくんマネージャ");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
};

class TKazekunParams : public TSmallEnemyParams {
public:
	TKazekunParams(const char*);

	/* 0x2D4 */ TParamRT<f32> mAppearDist;
	/* 0x2E8 */ TParamRT<f32> mAroundDist;
	/* 0x2FC */ TParamRT<f32> mAroundSpeed;
	/* 0x310 */ TParamRT<s32> mAroundTime;
	/* 0x324 */ TParamRT<f32> mAttackSpeed;
	/* 0x338 */ TParamRT<f32> mAirFric;
	/* 0x34C */ TParamRT<s32> mResetTime;
	/* 0x360 */ TParamRT<s32> mResetTimeHitting;
	/* 0x374 */ TParamRT<s32> mPoseTime;
	/* 0x388 */ TParamRT<f32> mDicideTiming;
	/* 0x39C */ TParamRT<f32> mTurnOffsetY;
	/* 0x3B0 */ TParamRT<f32> mLostOffsetYUp;
	/* 0x3C4 */ TParamRT<f32> mLostOffsetYDown;
	/* 0x3D8 */ TParamRT<f32> mPoseSpeed;
	/* 0x3EC */ TParamRT<f32> mPoseOmegaRate;
};

class TKazekun : public TSmallEnemy {
public:
	TKazekun(const char* name = "かぜくん");

	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual bool isCollidMove(THitActor*);

	void setVisible(bool);
	void doAttack(bool);
	bool doAttackPose(bool);
	static void getAroundQuat(JGeometry::TQuat4<f32>&,
	                          const JGeometry::TVec3<f32>&, f32);
	f32 getAroundRate(const JGeometry::TVec3<f32>&) const;
	void flyAroundMario();
	void changeBck(const char*);
	bool isGiveUpAround() const;
	void updateEffect();
	void emitAppearEffect();
	bool hasWind() const;
	bool isDamage() const;
	bool isHitWater() const;
	void initParticle();
	void initCollision();

public:
	/* 0x194 */ JGeometry::TVec3<f32> unk194;
	/* 0x1A0 */ JGeometry::TQuat4<f32> unk1A0;
	/* 0x1B0 */ s32 unk1B0;
	/* 0x1B4 */ char unk1B4[0x20];
};

#endif // ENEMY_KAZEKUN_HPP
