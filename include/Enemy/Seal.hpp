#ifndef ENEMY_SEAL_HPP
#define ENEMY_SEAL_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class TSeal : public TSpineEnemy {
public:
	TSeal(const char* name);

	virtual void init(TLiveManager*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void calcRootMatrix();

	// Two levels here are receiveMessage's missing 16 bytes of frame.
	TSpineBase<TLiveActor>* getSpine() { return mSpine; }

	/* 0x150 */ int unk150;
};

class TSealManager : public TEnemyManager {
public:
	TSealManager(const char* name);

	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();

	void initJParticle();
};

DECLARE_NERVE(TNerveSealSleep, TLiveActor)
DECLARE_NERVE(TNerveSealWait, TLiveActor)
DECLARE_NERVE(TNerveSealDie, TLiveActor)

#endif
