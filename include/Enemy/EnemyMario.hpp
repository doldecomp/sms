#ifndef ENEMY_ENEMY_MARIO_HPP
#define ENEMY_ENEMY_MARIO_HPP

#include <Player/Mario.hpp>
#include "Strategic/HitActor.hpp"

class TEMario;

class TEnemyMario : public TMario {
public:
	enum EMDoing {
		EM_DOING_TRAMPLED          = 0xD,
		EM_DOING_DOWN_WAIT_TO_TALK = 0xF,
		EM_DOING_RUN_AWAY          = 0x10,
		EM_DOING_GOAL              = 0x16,
		EM_DOING_REACHED_GATE      = 0x18,
	};

	class TSettingParams : public TParams {
	public:
		TSettingParams(const char*);
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
	void emWaiting();
	void tryTake();
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
	BOOL isDispPencil() const;
	BOOL canJumpToNode() const;

	bool canControl() const
	{
		if (mEMDoing == 0xB || mEMDoing == 0xC || mEMDoing == 0x11) {
			return true;
		}
		return false;
	}

	bool isGoal() const { return mGoalFlags & 1; }
	bool isReachedToGate() const { return mEMDoing == EM_DOING_REACHED_GATE; }
	bool isDownWaitingToTalk() const
	{
		return mEMDoing == EM_DOING_DOWN_WAIT_TO_TALK;
	}

public:
	u16 mGoalFlags;      // 0x4290
	u16 mEMDoing;        // 0x4292
	char unk4294[0xC];
	TEMario* mEMario;    // 0x42A0
	s32 mEMDoingTimer;   // 0x42A4
	char unk42A8[0x8];
	f32 mAttackRange;    // 0x42B0
	char unk42B4[0x4];
	s16 mTrampleCount;   // 0x42B8
	char unk42BA[0x22];
	u32 mEMFlags;        // 0x42DC
	JGeometry::TVec3<f32> mDisappearPosition; // 0x42E0
};

#endif
