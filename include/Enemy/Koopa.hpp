#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JDrama/JDRNameRefGen.hpp>
#include <MoveBG/MapObjCorona.hpp>
#include <Player/MarioAccess.hpp>

#include <JSystem/JParticle/JPAEmitter.hpp>
#include <System/EmitterViewObj.hpp>

class TKoopaFlame;
class TKoopaHand;
class TKoopaHead;
class TKoopaBody;

class TKoopaParams : public TSpineEnemyParams {
public:
	// Fabricated
	TKoopaParams(const char* path)
	    : TSpineEnemyParams(path)
	    , PARAM_INIT(turnSpeed, 1.6f)
	    , PARAM_INIT(turnAnim, 3.7f)
	    , PARAM_INIT(waitStep, 600.0f)
	    , PARAM_INIT(downStep, 1000.0f)
	    , PARAM_INIT(attackRadius, 800.0f)
	    , PARAM_INIT(attackHeight, 1000.0f)
	    , PARAM_INIT(focusRange, 2.0f)
	    , PARAM_INIT(waitRange, 12.0f)
	    , PARAM_INIT(fireSpeed, 3.5f)
	    , PARAM_INIT(tumbleWeight, 8.3f)
	    , PARAM_INIT(tumbleSpeed, 2.0f)
	    , PARAM_INIT(tumbleStartFrame, 95.0f)
	    , PARAM_INIT(tumbleEndFrame, 160.0f)
	    , PARAM_INIT(waitSpeed, 2.0f)
	    , PARAM_INIT(staggerSpeed, 2.0f)
	    , PARAM_INIT(downSpeed, 2.0f)
	    , PARAM_INIT(flameVelocity, 35.0f)
	    , PARAM_INIT(flameScale, 1.0f)
	    , PARAM_INIT(flameCount, 500)
	    , PARAM_INIT(flameFocusStartStep, 25)
	    , PARAM_INIT(flameFocusEndStep, 500)
	    , PARAM_INIT(flameRadius, 300.0f)
	    , PARAM_INIT(flameHeight, 1000.0f)
	    , PARAM_INIT(headRadius, 400.0f)
	    , PARAM_INIT(waterhitSpeed, 2.0f)
	    , PARAM_INIT(flameOverStart, 1.0f)
	    , PARAM_INIT(flameNeckRange, 17.0f)
	    , PARAM_INIT(flameNeckDownRate, 0.3f)
	    , PARAM_INIT(flameJump, 80.0f)
	    , PARAM_INIT(fallSpeed, 2.0f)
	    , PARAM_INIT(marioEstimationFire, 20.0f)
	    , PARAM_INIT(marioEstimationWait, 10.0f)
	{
		TParams::load(mPrmPath);
	}

	// fabricated
	f32 getTurnSpeed() const { return turnSpeed.get(); }
	f32 getTurnAnim() const { return turnAnim.get(); }
	f32 getWaitStep() const { return waitStep.get(); }
	f32 getDownStep() const { return downStep.get(); }
	f32 getAttackRadius() const { return attackRadius.get(); }
	f32 getAttackHeight() const { return attackHeight.get(); }
	f32 getFocusRange() const { return focusRange.get(); }
	f32 getWaitRange() const { return waitRange.get(); }
	f32 getFireSpeed() const { return fireSpeed.get(); }
	f32 getTumbleWeight() const { return tumbleWeight.get(); }
	f32 getTumbleSpeed() const { return tumbleSpeed.get(); }
	f32 getTumbleStartFrame() const { return tumbleStartFrame.get(); }
	f32 getTumbleEndFrame() const { return tumbleEndFrame.get(); }
	f32 getWaitSpeed() const { return waitSpeed.get(); }
	f32 getStaggerSpeed() const { return staggerSpeed.get(); }
	f32 getDownSpeed() const { return downSpeed.get(); }
	f32 getFlameVelocity() const { return flameVelocity.get(); }
	f32 getFlameScale() const { return flameScale.get(); }
	s32 getFlameCount() const { return flameCount.get(); }
	s32 getFlameFocusStartStep() const { return flameFocusStartStep.get(); }
	s32 getFlameFocusEndStep() const { return flameFocusEndStep.get(); }
	f32 getFlameRadius() const { return flameRadius.get(); }
	f32 getFlameHeight() const { return flameHeight.get(); }
	f32 getHeadRadius() const { return headRadius.get(); }
	f32 getWaterhitSpeed() const { return waterhitSpeed.get(); }
	f32 getFlameOverStart() const { return flameOverStart.get(); }
	f32 getFlameNeckRange() const { return flameNeckRange.get(); }
	f32 getFlameNeckDownRate() const { return flameNeckDownRate.get(); }
	f32 getFlameJump() const { return flameJump.get(); }
	f32 getFallSpeed() const { return fallSpeed.get(); }
	f32 getMarioEstimationFire() const { return marioEstimationFire.get(); }
	f32 getMarioEstimationWait() const { return marioEstimationWait.get(); }

private:
	/* 0xA8 */ TParamRT<f32> turnSpeed;
	/* 0xBC */ TParamRT<f32> turnAnim;
	/* 0xD0 */ TParamRT<f32> waitStep;
	/* 0xE4 */ TParamRT<f32> downStep;
	/* 0xF8 */ TParamRT<f32> attackRadius;
	/* 0x10C */ TParamRT<f32> attackHeight;
	/* 0x120 */ TParamRT<f32> focusRange;
	/* 0x134 */ TParamRT<f32> waitRange;
	/* 0x148 */ TParamRT<f32> fireSpeed;
	/* 0x15C */ TParamRT<f32> tumbleWeight;
	/* 0x170 */ TParamRT<f32> tumbleSpeed;
	/* 0x184 */ TParamRT<f32> tumbleStartFrame;
	/* 0x198 */ TParamRT<f32> tumbleEndFrame;
	/* 0x1AC */ TParamRT<f32> waitSpeed;
	/* 0x1C0 */ TParamRT<f32> staggerSpeed;
	/* 0x1D4 */ TParamRT<f32> downSpeed;
	/* 0x1E8 */ TParamRT<f32> flameVelocity;
	/* 0x1FC */ TParamRT<f32> flameScale;
	/* 0x210 */ TParamRT<s32> flameCount;
	/* 0x224 */ TParamRT<s32> flameFocusStartStep;
	/* 0x238 */ TParamRT<s32> flameFocusEndStep;
	/* 0x24C */ TParamRT<f32> flameRadius;
	/* 0x260 */ TParamRT<f32> flameHeight;
	/* 0x274 */ TParamRT<f32> headRadius;
	/* 0x288 */ TParamRT<f32> waterhitSpeed;
	/* 0x29C */ TParamRT<f32> flameOverStart;
	/* 0x2B0 */ TParamRT<f32> flameNeckRange;
	/* 0x2C4 */ TParamRT<f32> flameNeckDownRate;
	/* 0x2D8 */ TParamRT<f32> flameJump;
	/* 0x2EC */ TParamRT<f32> fallSpeed;
	/* 0x300 */ TParamRT<f32> marioEstimationFire;
	/* 0x314 */ TParamRT<f32> marioEstimationWait;
};

enum TKoopaAnm {
	KOOPA_ANM_DOWN = 0,
	KOOPA_ANM_DOWN_LOOP,
	KOOPA_ANM_FALL,
	KOOPA_ANM_FIRE_END,
	KOOPA_ANM_FIRE_LOOP,
	KOOPA_ANM_FIRE_START,
	KOOPA_ANM_FIRST,
	KOOPA_ANM_GETUP,
	KOOPA_ANM_HIPDROP,
	KOOPA_ANM_STAGGER,
	KOOPA_ANM_TURN_L,
	KOOPA_ANM_TURN_R,
	KOOPA_ANM_WAIT,
	KOOPA_ANM_WAIT_LOOP,
	KOOPA_ANM_WATERHIT,
};

class TKoopaManager : public TEnemyManager {
public:
	TKoopaManager(const char* name);
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	// Fabricated
	TKoopaParams* getSaveParam()
	{
		return (TKoopaParams*)TEnemyManager::getSaveParam();
	}
};

class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char* name);
	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();
	virtual void updateAnmSound();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	void fall();
	void stagger(bool force);
	bool getShowered();
	bool effectsTumble() const;
	void getDown();
	bool allowsLaunch() const;
	f32 getTargetDir(const JGeometry::TVec3<f32>& target) const;
	f32 getNeckFocus() const;
	bool isFlaming() const;
	f32 getFlameDirRate() const;
	f32 getFlameDirDegree() const;
	void changeAnm(int bckIndex, int btpIndex, f32 speed);
	void setUpHitActors();

	// Unused
	TKoopaParams* getParam() const;
	// Unused
	void checkMarioWhichSide();
	// Unused
	MtxPtr getHeadMtx() const;
	// Unused
	f32 getAnmEnd() const;
	// Unused
	bool canTumble() const;
	// Unused
	bool passesAnmFrame(f32 frame) const;
	// Unused
	f32 getAnmFrameNext() const;
	// Unused
	f32 getAnmFrame() const;
	// Unused
	bool isTumbling() const;
	// Unused
	bool isProvoking() const;
	// Unused
	void setIgnoreMario(long time);
	// Unused
	bool ignoresMario() const;
	// Unused
	bool isBreathing() const;
	// Unused
	void laugh();
	// Unused
	bool endsAnm() const;
	// Unused
	int getAnmIndex() const;
	// Unused
	void resetFlame_();
	// Unused
	void breathFlame();

	// fabricated and fake
	void emitMtxParticle(s32 id, MtxPtr mtx, const JGeometry::TVec3<f32>& scale)
	{
		JPABaseEmitter* emitter = gpMarioParticleManager->emitAndBindToMtxPtr(
		    id, mtx, TMarioParticleManager::EFFECT_LOOP_NORMAL, this);
		if (emitter)
			emitter->setGlobalScale(scale);
	}

	// Unused
	void stopFlame();

	// fabricated
	void turn(f32 amount)
	{
		if (amount > 0.0f)
			changeAnm(KOOPA_ANM_TURN_R, 0, amount * getParam()->getTurnAnim());
		else
			changeAnm(KOOPA_ANM_TURN_L, 0, -amount * getParam()->getTurnAnim());
		mRotation.y
		    = -180.0f
		      + std::fmodf(360.0f + (mRotation.y + amount - -180.0f), 360.0f);
	}

	// fabricated
	bool turnToTarget(bool right)
	{
		f32 diff
		    = -180.0f
		      + std::fmodf(360.0f + (unk150 - mRotation.y - -180.0f), 360.0f);
		bool turned;
		if (right) {
			if (diff > getParam()->getTurnSpeed()) {
				turn(getParam()->getTurnSpeed());
				turned = true;
			} else if (diff > 0.0f) {
				turn(diff);
				turned = true;
			} else {
				turned = false;
			}
		} else {
			if (diff < -getParam()->getTurnSpeed()) {
				turn(-getParam()->getTurnSpeed());
				turned = true;
			} else if (diff < 0.0f) {
				turn(diff);
				turned = true;
			} else {
				turned = false;
			}
		}
		return turned;
	}

	// fabricated
	int getTargetSide() const;

	// fabricated
	bool updateTarget()
	{
		TBathtub* bathtub = (TBathtub*)JDrama::TNameRefGen::search("バスタブ");
		JGeometry::TVec3<f32> velocity;
		velocity.scale(getParam()->getMarioEstimationWait(),
		               JGeometry::TVec3<f32>(SMS_GetMarioSpeedX(),
		                                     SMS_GetMarioSpeedY(),
		                                     SMS_GetMarioSpeedZ()));
		bool nearGrip = bathtub->getNextGrip(
		    *gpMarioPos, velocity, getParam()->getWaitRange(), &unk150);
		if (!nearGrip) {
			velocity.scale(getParam()->getMarioEstimationFire(),
			               JGeometry::TVec3<f32>(SMS_GetMarioSpeedX(),
			                                     SMS_GetMarioSpeedY(),
			                                     SMS_GetMarioSpeedZ()));
			unk150 = bathtub->getNextJuncture(*gpMarioPos, velocity);
		}
		return nearGrip;
	}

	// fabricated
	f32 getUnk150() const { return unk150; }
	// fabricated
	bool getUnk155() const { return unk155; }
	// Fabricated
	void setUnk155(bool val) { unk155 = val; }

public:
	/* 0x150 */ f32 unk150;
	/* 0x154 */ bool unk154;
	/* 0x155 */ bool unk155;
	/* 0x158 */ JGeometry::TVec3<f32> unk158;
	/* 0x164 */ TKoopaFlame* unk164[10];
	/* 0x18C */ TKoopaHand* unk18C[2];
	/* 0x194 */ TKoopaHead* unk194;
	/* 0x198 */ TKoopaBody* unk198;
	/* 0x19C */ long unk19C;
	/* 0x1A0 */ int unk1A0;
	/* 0x1A4 */ int unk1A4;
	/* 0x1A8 */ int unk1A8;
	/* 0x1AC */ u8 unk1AC[0xC];
	/* 0x1B8 */ f32 unk1B8;
};

class TKoopaParts : public THitActor {
public:
	TKoopaParts(const char* name, u32 actorType, TKoopa* koopa, f32 radius);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void control() { }
	virtual void attack_(THitActor* target) = 0;
	// Unused
	void remove();
	// Unused
	void set(const JGeometry::TVec3<f32>& pos, f32 radius, f32 height);

public:
	/* 0x68 */ TKoopa* unk68;
};

class TKoopaFlame : public TKoopaParts {
public:
	// Unused
	TKoopaFlame(TKoopa* koopa);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void attack_(THitActor* target);
	// Unused
	void fire(const JGeometry::TVec3<f32>& pos,
	          const JGeometry::TVec3<f32>& dir, f32 speed, f32 length,
	          f32 radius, f32 height);
	// Unused
	void resetFlame();
	// Unused
	f32 getLength() const;
	// Unused
	bool isAlive() const;

public:
	/* 0x6C */ JGeometry::TVec3<f32> unk6C;
	/* 0x78 */ JGeometry::TVec3<f32> unk78;
	/* 0x84 */ f32 unk84;
	/* 0x88 */ f32 unk88;
	/* 0x8C */ f32 unk8C;
	/* 0x90 */ f32 unk90;
	/* 0x94 */ f32 unk94;
};

class TKoopaHand : public TKoopaParts {
public:
	// Unused
	TKoopaHand(TKoopa* koopa);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor* target);
};

class TKoopaHead : public TKoopaParts {
public:
	// Unused
	TKoopaHead(TKoopa* koopa);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor* target);
};

class TKoopaBody : public TKoopaParts {
public:
	// Unused
	TKoopaBody(TKoopa* koopa);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor* target);
};

class TNerveKoopaTurn : public TNerveBase<TLiveActor> {
public:
};

class TNerveKoopaTurnR : public TNerveKoopaTurn {
public:
	static const TNerveKoopaTurnR& theNerve();
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
};

class TNerveKoopaTurnL : public TNerveKoopaTurn {
public:
	static const TNerveKoopaTurnL& theNerve();
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
};

class TNerveKoopaTumble : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaTumble& theNerve();
};

class TNerveKoopaProvoke : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaProvoke& theNerve();
};

class TNerveKoopaWait : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaWait& theNerve();
};

class TNerveKoopaFlame : public TNerveKoopaTurn {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaFlame& theNerve();
};

class TNerveKoopaGetDown : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaGetDown& theNerve();
};

class TNerveKoopaStagger : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaStagger& theNerve();
};

class TNerveKoopaFall : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaFall& theNerve();
};

class TNerveKoopaGetShowered : public TNerveBase<TLiveActor> {
public:
	virtual BOOL execute(TSpineBase<TLiveActor>* spine) const;
	static const TNerveKoopaGetShowered& theNerve();
};

#endif
