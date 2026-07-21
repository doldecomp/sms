#ifndef ENEMY_EMARIO_HPP
#define ENEMY_EMARIO_HPP

#include <Enemy/EnemyManager.hpp>
#include <Enemy/Enemy.hpp>

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
	u32 getInitialState() const { return mInitialState; }
	u32 getReplayIndexDolpic() const { return mReplayIndexDolpic; }
	u32 getReplayIndexPad2() const { return mReplayIndexPad2; }
	u32 getReplayIndexPad3() const { return mReplayIndexPad3; }

	// fabricated and fake
	static f32 vecDist(const JGeometry::TVec3<f32>& a,
	                   const JGeometry::TVec3<f32>& b)
	{
		return JGeometry::TVec3<f32>(a - b).length();
	}

private:
	TEnemyMario* mEnemyMario; // 0x150
	u32 mInitialState;
	u32 mReplayIndexDolpic;
	u32 mReplayIndexPad2;
	u32 mReplayIndexPad3;
};

class TEMarioManager : public TEnemyManager {
	TEMarioManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
