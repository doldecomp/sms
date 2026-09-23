#ifndef ENEMY_KOOPA_HPP
#define ENEMY_KOOPA_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Spine.hpp>
#include <System/ParamInst.hpp>

// koopa_model.bmd's .bck slots. The names come from koopa_bastable (in
// Koopa.cpp), whose entries are the matching .bas paths; slots 1 and 13 have no
// .bas and are filled in alphabetically. TLimitKoopaManager loads the same
// .bmd, so limitkoopa.cpp indexes the same slots -- note that its .prm names do
// not line up with the .bas names: its "tumble" nerve plays HIPDROP and its
// hip-drop start plays FIRE_START.
enum {
	KOOPA_ANM_DOWN       = 0,
	KOOPA_ANM_DOWN_WAIT  = 1,
	KOOPA_ANM_FALL       = 2,
	KOOPA_ANM_FIRE_END   = 3,
	KOOPA_ANM_FIRE_LOOP  = 4,
	KOOPA_ANM_FIRE_START = 5,
	KOOPA_ANM_FIRST      = 6,
	KOOPA_ANM_GETUP      = 7,
	KOOPA_ANM_HIPDROP    = 8,
	KOOPA_ANM_STAGGER    = 9,
	KOOPA_ANM_TURN_L     = 0xA,
	KOOPA_ANM_TURN_R     = 0xB,
	KOOPA_ANM_WAIT       = 0xC,
	KOOPA_ANM_UNK13      = 0xD,
	KOOPA_ANM_WATERHIT   = 0xE,
};

class TKoopa;
class J3DNode;

// The Corona Mountain bathtub Bowser. Parameter names come from
// /enemy/koopa.prm's key strings, which are plain camelCase here rather than
// the mSL* convention the small-enemy parameter classes use.
//
// limitkoopa.prm is a cut-down copy of this file with four extra hip-drop
// parameters in front, which is why TLimitKoopaParams looks so similar.
class TKoopaParams : public TSpineEnemyParams {
public:
	// The map has no symbol for this constructor anywhere: it is inlined
	// whole into TKoopaManager::load, which is why it lives in the class body.
	TKoopaParams(const char* prm)
	    : TSpineEnemyParams(prm)
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

	/* 0x0A8 */ TParamRT<f32> turnSpeed;
	/* 0x0BC */ TParamRT<f32> turnAnim;
	/* 0x0D0 */ TParamRT<f32> waitStep;
	/* 0x0E4 */ TParamRT<f32> downStep;
	/* 0x0F8 */ TParamRT<f32> attackRadius;
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

// One of Bowser's hit boxes. Unlike TLimitKoopaParts these derive straight
// from THitActor (the vtable is 0xAC long and carries TActor::load, not
// TLiveActor's), and the class introduces two virtuals of its own: control()
// and the pure attack_().
class TKoopaParts : public THitActor {
public:
	TKoopaParts(const char* name, u32 actor_type, TKoopa* owner, f32 radius);
	virtual ~TKoopaParts() { }

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void control() { }
	virtual void attack_(THitActor*) = 0;

	void remove();
	void set(const JGeometry::TVec3<f32>& position, f32 radius, f32 height);

	/* 0x68 */ TKoopa* mOwner;
};

class TKoopaBody : public TKoopaParts {
public:
	TKoopaBody(TKoopa* owner);
	virtual ~TKoopaBody() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

class TKoopaHead : public TKoopaParts {
public:
	TKoopaHead(TKoopa* owner);
	virtual ~TKoopaHead() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

class TKoopaHand : public TKoopaParts {
public:
	TKoopaHand(TKoopa* owner);
	virtual ~TKoopaHand() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void attack_(THitActor*);
};

// One segment of Bowser's fire breath: it walks along mDirection from
// mStartPos until mLength passes mLengthMax, then removes itself.
class TKoopaFlame : public TKoopaParts {
public:
	TKoopaFlame(TKoopa* owner);
	virtual ~TKoopaFlame() { }

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void attack_(THitActor*);

	void fire(const JGeometry::TVec3<f32>& position,
	          const JGeometry::TVec3<f32>& direction, f32 speed, f32 length_max,
	          f32 radius, f32 height);

	// UNUSED in the map, so none of these can be weak: they are defined
	// out of line in Koopa.cpp at their map positions and inlined from there.
	void resetFlame();
	f32 getLength() const;
	bool isAlive() const;

	/* 0x6C */ JGeometry::TVec3<f32> mStartPos;
	/* 0x78 */ JGeometry::TVec3<f32> mDirection;
	/* 0x84 */ f32 mSpeed;
	/* 0x88 */ f32 mLengthMax;
	/* 0x8C */ f32 mLength;
	/* 0x90 */ f32 mRadius;
	/* 0x94 */ f32 mHeight;
};

// The wrap in TKoopa::turnBody, the TUtil<f32>::mod twin of KoopaNerve.hpp's
// KoopaWrapDirection: this level is what makes the Turn nerves `bl` the weak
// mod copy the map records for Koopa.cpp instead of expanding it.
static inline f32 KoopaModDirection(f32 t, f32 l, f32 r)
{
	return l + JGeometry::TUtil<f32>::mod((r - l) + (t - l), r - l);
}

class TKoopa : public TSpineEnemy {
public:
	TKoopa(const char* name = "クッパ");
	virtual ~TKoopa() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void updateAnmSound();
	virtual const char** getBasNameTable() const;
	virtual void reset();

	int checkMarioWhichSide();
	MtxPtr getHeadMtx() const;
	BOOL getAnmEnd() const;
	void fall();
	bool canTumble() const;
	bool passesAnmFrame(f32 frame) const;
	f32 getAnmFrameNext() const;
	f32 getAnmFrame() const;
	f32 getTargetDir(const JGeometry::TVec3<f32>& target) const;
	BOOL isTumbling() const;
	void stagger(bool force);
	bool getShowered();
	// Both are bool, not BOOL: TBathtub::getNumKillerBurstable and
	// getNumKillerLaunchable mask the result with clrlwi. after the bl.
	bool effectsTumble() const;
	void getDown();
	bool allowsLaunch() const;
	f32 getNeckFocus() const;
	bool isProvoking() const;
	bool isFlaming() const;
	f32 getFlameDirRate() const;
	void setIgnoreMario(long ignore);
	BOOL ignoresMario() const;
	bool isBreathing() const;
	void laugh();
	BOOL endsAnm() const;
	int getAnmIndex() const;
	void changeAnm(int bck_index, int btp_index, f32 rate);
	void setUpHitActors();
	void resetFlame_();
	void breathFlame();
	void stopFlame();
	f32 getFlameDirDegree() const;

	// UNUSED, 0xc in the map, so it lives in the .cpp.
	TKoopaParams* getParam() const;

	// TNerveKoopaTurnL and TNerveKoopaTurnR both expand this: the redundant
	// second `delta > 0` test in each of them is the inlined body's own.
	// It has no map symbol of its own, so it was a header inline.
	// TODO: the ROM also `bl`s TEnemyManager::getSaveParam() for both
	// turnAnim reads here and for the Turn nerves' clamped-argument
	// turnSpeed read (the condition's read stays inline), and its frame is
	// 0x1a8 against our 0xd0; named locals in getParam or in the wrap only
	// grow the frame, and another level above turnBody takes it out of line.
	bool turnBody(f32 delta)
	{
		if (delta > 0.0f)
			changeAnm(KOOPA_ANM_TURN_R, 0, delta * getParam()->turnAnim.get());
		else
			changeAnm(KOOPA_ANM_TURN_L, 0,
			          -delta * getParam()->turnAnim.get());
		mRotation.y = KoopaModDirection(mRotation.y + delta, -180.0f, 180.0f);
		return true;
	}

	/* 0x150 */ f32 mTargetDir;
	/* 0x154 */ bool mTurnsLeft;
	/* 0x155 */ bool mLaughPending;
	/* 0x158 */ JGeometry::TVec3<f32> mAnmSoundPos;
	/* 0x164 */ TKoopaFlame* mFlames[10];
	/* 0x18C */ TKoopaParts* mHands[2];
	/* 0x194 */ TKoopaParts* mHead;
	/* 0x198 */ TKoopaParts* mBody;
	/* 0x19C */ int mWaitTimer;
	/* 0x1A0 */ int mHeadJntIndex;
	/* 0x1A4 */ int mNeckJntIndex;
	/* 0x1A8 */ int mAgoJntIndex;
	// TODO: nothing in this TU reads 0x1AC-0x1B7. setIgnoreMario(long) is a
	// two-instruction store and ignoresMario() an eight-instruction load, so
	// one word of the gap is the ignore flag; the class is 0x1BC long
	// (MarNameRefGen_BossEnemy allocates it), so the other two words exist but
	// are unidentified.
	/* 0x1AC */ long mIgnoreMario;
	/* 0x1B0 */ f32 unk1B0;
	/* 0x1B4 */ f32 unk1B4;
	/* 0x1B8 */ f32 mNeckFocus;
};

class TKoopaManager : public TEnemyManager {
public:
	TKoopaManager(const char* name = "クッパマネージャー");
	virtual ~TKoopaManager() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
