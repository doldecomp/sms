#ifndef ENEMY_SEAL_HPP
#define ENEMY_SEAL_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class THitActor;
class TLiveManager;

DECLARE_NERVE(TNerveSealDie, TLiveActor);

DECLARE_NERVE(TNerveSealSleep, TLiveActor);

DECLARE_NERVE(TNerveSealWait, TLiveActor);

class TSealManager : public TEnemyManager {
public:
	TSealManager(const char* name = "シールマネージャ");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();

	void initJParticle();
};

class TSeal : public TSpineEnemy {
public:
	TSeal(const char* name = "シール");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void init(TLiveManager* manager);
	virtual void calcRootMatrix();

public:
	/* 0x150 */ s32 unk150;
};

#endif // ENEMY_SEAL_HPP
