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

	virtual void perform(u32, JDrama::TGraphics*);

	BOOL isGoal();
	BOOL isReachedToGate() const;
	BOOL isDownWaitingToTalk() const;
	void startRunAway();
	void startMonteReplay(u32 param1);
	void startGateDrawing();
	void forceDisappear();

	// fabricated and fake
	static f32 vecDist(const JGeometry::TVec3<f32>& a,
	                   const JGeometry::TVec3<f32>& b)
	{
		return JGeometry::TVec3<f32>(a - b).length();
	}

private:
	TEnemyMario* mEnemyMario; // 0x150
	u32 unk154;
	u32 unk158;
	u32 unk15C;
	u32 unk160;
};

class TEMarioManager : public TEnemyManager {
	TEMarioManager(const char* name);

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
};

#endif
