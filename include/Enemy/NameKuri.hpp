#ifndef ENEMY_NAME_KURI_HPP
#define ENEMY_NAME_KURI_HPP

#include <Strategic/Nerve.hpp>
#include <Enemy/SmallEnemy.hpp>

class TLiveActor;

class TNameKuriManager : public TSmallEnemyManager {
public:
	TNameKuriManager(const char*);
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual void createModelData();
	virtual TSmallEnemy* createEnemyInstance();
	virtual void initSetEnemies();

	static u32 mExplosionSpeed;
	static u32 mStopMinScaleFrame;
};

DECLARE_NERVE(TNerveNameKuriLand, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttack, TLiveActor);
DECLARE_NERVE(TNerveNameKuriJumpAttackPrepare, TLiveActor);
DECLARE_NERVE(TNerveNameKuriExplosion, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDiffuse, TLiveActor);
DECLARE_NERVE(TNerveNameKuriDrawPollute, TLiveActor);
DECLARE_NERVE(TNerveNKFollowMario, TLiveActor);

#endif
