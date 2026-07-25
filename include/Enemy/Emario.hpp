#ifndef ENEMY_EMARIO_HPP
#define ENEMY_EMARIO_HPP

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>
#include <Enemy/Graph.hpp>

class TEnemyMario;

class TEMario : public TSpineEnemy {
public:
	TEMario(const char* name);

	virtual void load(JSUMemoryInputStream& stream);
	virtual void loadAfter();
	virtual void init(TLiveManager* liveManager);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void kill();

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	BOOL isGoal();
	BOOL isReachedToGate() const;
	BOOL isDownWaitingToTalk() const;
	void startRunAway();
	void startMonteReplay(u32 param1);
	void startGateDrawing();
	void forceDisappear();

public:
	/* 0x150 */ TEnemyMario* mEnemyMario;
	/* 0x154 */ u32 mInitialState;
	/* 0x158 */ u32 unk158;
	/* 0x15C */ u32 unk15C;
	/* 0x160 */ u32 unk160;
};

class TEMarioManager : public TEnemyManager {
	TEMarioManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
