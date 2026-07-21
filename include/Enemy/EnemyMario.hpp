#ifndef ENEMY_ENEMY_MARIO_HPP
#define ENEMY_ENEMY_MARIO_HPP

#include <Player/Mario.hpp>
#include "Strategic/HitActor.hpp"

class TEMario;
class TMarioInputReplay;

class TEnemyMario : public TMario {
public:
	struct TReplayLink {
		u8 mNodeIndex;
		u8 mReplayIndex;
	};

	enum EMGoalFlag {
		EM_GOAL_FLAG_REACHED         = 0x1,
		EM_GOAL_FLAG_DISP_PENCIL     = 0x2,
		EM_GOAL_FLAG_FORCE_WATER_HIT = 0x10,
		EM_GOAL_FLAG_ENFORCE_TAKE    = 0x20,
		EM_GOAL_FLAG_DISP_HP_METER   = 0x80,
	};

	enum EMDoing {
		EM_DOING_WAITING                 = 0x0,
		EM_DOING_GET_CLOSER              = 0x1,
		EM_DOING_JUMPING                 = 0x2,
		EM_DOING_RUN_AWAY_FROM_MARIO     = 0x3,
		EM_DOING_TURNING                 = 0x4,
		EM_DOING_WALK_AROUND             = 0x5,
		EM_DOING_WALK_GRAPH              = 0x6,
		EM_DOING_HIDDEN                  = 0x7,
		EM_DOING_APPEAR                  = 0x8,
		EM_DOING_DISAPPEAR               = 0x9,
		EM_DOING_DISAPPEAR_TO_GATE       = 0xA,
		EM_DOING_REPLAY                  = 0xB,
		EM_DOING_SLEEPING                = 0xC,
		EM_DOING_TRAMPLED                = 0xD,
		EM_DOING_DOWN_ANIMATION          = 0xE,
		EM_DOING_DOWN_WAIT_TO_TALK       = 0xF,
		EM_DOING_RUN_AWAY                = 0x10,
		EM_DOING_REPLAY_RUN_AWAY_TO_GATE = 0x11,
		EM_DOING_KEEP_STAY               = 0x12,
		EM_DOING_DRAW_STAMP              = 0x13,
		EM_DOING_WAITING_TO_INVITE_MARIO = 0x14,
		EM_DOING_REPLAY_TO_GATE          = 0x15,
		EM_DOING_GOAL                    = 0x16,
		EM_DOING_GATE_DRAWING            = 0x17,
		EM_DOING_REACHED_GATE            = 0x18,
		EM_DOING_REPLAY_TO_GOAL          = 0x19,
		EM_DOING_REPLAY_WAITING          = 0x1A,
		EM_DOING_GET_PAD                 = 0x1B,
	};

	class TSettingParams : public TParams {
	public:
		TSettingParams(const char* path)
		    : TParams(path)
		    , PARAM_INIT(mSearchDist, 1000.0f)
		    , PARAM_INIT(mSearchHeight, 300.0f)
		    , PARAM_INIT(mWaterCtMax, 64)
		    , PARAM_INIT(mStopFlag, 1)
		    , PARAM_INIT(mStampFlag, 1)
		    , PARAM_INIT(mRandomFlag, 1)
		    , PARAM_INIT(mCarryFlag, 0)
		    , PARAM_INIT(mInvincibleFlag, 0)
		    , PARAM_INIT(mRandomPow, 1.0f)
		    , PARAM_INIT(mDownTime, 1200)
		    , PARAM_INIT(mPolluteFlag, 0)
		    , PARAM_INIT(mPolluteSize, 160.0f)
		{
			TParams::load(mPrmPath);
		}

		TParamRT<f32> mSearchDist;
		TParamRT<f32> mSearchHeight;
		TParamRT<s16> mWaterCtMax;
		TParamRT<u8> mStopFlag;
		TParamRT<u8> mStampFlag;
		TParamRT<u8> mRandomFlag;
		TParamRT<u8> mCarryFlag;
		TParamRT<u8> mInvincibleFlag;
		TParamRT<f32> mRandomPow;
		TParamRT<s16> mDownTime;
		TParamRT<u8> mPolluteFlag;
		TParamRT<f32> mPolluteSize;
	};

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void initValues();
	virtual void checkReturn();
	virtual void checkController(JDrama::TGraphics* graphics);
	virtual void playerControl(JDrama::TGraphics* graphics);
	virtual void initModel();
	virtual void damageExec(THitActor* hittingActor, int damage,
	                        int damageAnimType, int waterEmit,
	                        f32 knockbackSpeed, int rumbleFrames,
	                        f32 pollutionAmount, s16 invincibilityFrames);

	void drawHPMeter(MtxPtr);
	void reachGoal();
	u8 thinkTrample();
	void hitWater(THitActor*);
	void consider();
	void considerAfter();
	void emGetPad();
	void emGateDrawing();
	void startGateDrawing();
	void emKeepStay();
	void emWaitingMario();
	void emReplayToGoal();
	void emReplayRunAwayToGate();
	void emWaitingToInviteMario();
	void emDrawStamp();
	void emEnforceTake();
	void decideDoingAfterCarry();
	void emReplayRunAway();
	void emRunAwayToNearestNode();
	void runAwayMoveEffect();
	void findRunAwayNearestNode();
	void startRunAway();
	void emDownAnimation();
	void emPreDownAnimation();
	void emReplayJumpToNearestNode();
	void emReplayWaiting();
	void emReplayWaitingToReplayJumpToNearestNode();
	void emReplay();
	void emDisappearToGate();
	void emDisappear();
	void startDisappear(u16);
	void emAppear();
	void emHide();
	void emTurning();
	void emWalkGraph();
	void emWalkAround();
	void emGetCloser();
	void emJumping();
	void emRunAway();
	virtual void emWaiting();
	u8 tryTake();
	void changeEMWalkGraph();
	void changeEMJumping();
	void changeEMDoing(u16);
	void startMonteReplay(u32);
	void resetReplayStatus();
	void setStickToAngle(s16, f32);
	void setStickAgainstMario();
	f32 getStickPower();
	void kill();
	void initEnemyValues();
	bool isDispPencil() const
	{
		if (mGoalFlags & EM_GOAL_FLAG_DISP_PENCIL) {
			return true;
		}
		return false;
	}
	bool isForceWaterHit() const
	{
		if (mGoalFlags & EM_GOAL_FLAG_FORCE_WATER_HIT) {
			return true;
		}
		return false;
	}
	BOOL canJumpToNode() const;

	bool canControl() const
	{
		if (mEMDoing == 0xB || mEMDoing == 0xC || mEMDoing == 0x11) {
			return true;
		}
		return false;
	}

	bool isGoal() const { return mGoalFlags & EM_GOAL_FLAG_REACHED; }
	bool isDispStamp() const
	{
		if (mEMDoing == EM_DOING_DRAW_STAMP
		    && mSettingParams->mStampFlag.get() == 1) {
			return true;
		}
		return false;
	}
	bool isEnforceTake() const
	{
		if (mGoalFlags & EM_GOAL_FLAG_ENFORCE_TAKE) {
			return true;
		}
		return false;
	}
	bool isReachedToGate() const { return mEMDoing == EM_DOING_REACHED_GATE; }
	bool isDownWaitingToTalk() const
	{
		return mEMDoing == EM_DOING_DOWN_WAIT_TO_TALK;
	}

public:
	u16 mGoalFlags;    // 0x4290
	u16 mEMDoing;      // 0x4292
	s16 mWaterCounter; // 0x4294
	s16 mAngleToMario; // 0x4296
	s16 mTargetAngle;  // 0x4298
	char unk429A[2];
	f32 mDistanceToMario; // 0x429C
	TEMario* mEMario;     // 0x42A0
	u32 mEMDoingTimer;    // 0x42A4
	s32 mReplayIndex;     // 0x42A8
	f32 mTrembleStrength;
	f32 mAttackRange;                         // 0x42B0
	s16 mWaterEffectTimer;                    // 0x42B4
	s16 mWaterEffectTimerMax;                 // 0x42B6
	s16 mTrampleCount;                        // 0x42B8
	s16 mWaterHitTimer;                       // 0x42BA
	f32 mReplayJumpSpeed;                     // 0x42BC
	JGeometry::TVec3<f32> mReferencePosition; // 0x42C0
	s16 mRunAwayNodeIndex;                    // 0x42CC
	char unk42CE[0x2];
	f32 mRunAwaySpeed; // 0x42D0
	u8 mPadIndex;
	char unk42D5[0x7];
	J3DModel* mSpecialModel;                  // 0x42DC
	JGeometry::TVec3<f32> mDisappearPosition; // 0x42E0
	J3DModel* mPencilModel;                   // 0x42EC
	MActor* mStampActor;                      // 0x42F0
	f32 mPencilScale;
	TMarioInputReplay** mInputReplays;        // 0x42F8
	TMarioInputReplay** mRunAwayInputReplays; // 0x42FC
	TMarioInputReplay* mGateReplay;           // 0x4300
	TReplayLink (*mReplayLinks)[3];           // 0x4304
	char unk4308[0x4];
	TSettingParams* mSettingParams; // 0x430C
};

#endif
