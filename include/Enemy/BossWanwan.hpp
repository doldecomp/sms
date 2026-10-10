#ifndef ENEMY_BOSS_WANWAN_HPP
#define ENEMY_BOSS_WANWAN_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>

class TLiveActor;

// fabricated: declaration only, layout unknown
class TBossWanwan : public TSpineEnemy {
public:
	TBossWanwan(const char*);

public:
	// TODO: unknown layout, size from getNameRef_BossEnemy's operator new
	/* 0x150 */ u8 unk150[0x68];
};

// fabricated: declaration only, layout unknown beyond TEnemyManager
class TBossWanwanManager : public TEnemyManager {
public:
	TBossWanwanManager(const char*);
};

DECLARE_NERVE(TNerveBWGraphWander, TLiveActor);
DECLARE_NERVE(TNerveBWRoll, TLiveActor);
DECLARE_NERVE(TNerveBWBark, TLiveActor);
DECLARE_NERVE(TNerveBWJump, TLiveActor);
DECLARE_NERVE(TNerveBWStun, TLiveActor);
DECLARE_NERVE(TNerveBWWakeup, TLiveActor);
DECLARE_NERVE(TNerveBWJumpToBath, TLiveActor);
DECLARE_NERVE(TNerveBWDie, TLiveActor);
DECLARE_NERVE(TNerveBWJumpAway, TLiveActor);
DECLARE_NERVE(TNerveBWShake, TLiveActor);
DECLARE_NERVE(TNerveBWFall, TLiveActor);

#endif
