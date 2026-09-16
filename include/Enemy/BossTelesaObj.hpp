#ifndef ENEMY_BOSSTELESAOBJ_HPP
#define ENEMY_BOSSTELESAOBJ_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjSirena.hpp>
#include <Strategic/HitActor.hpp>

// Class hierarchy recovered from each destructor's vtable chain. Only the
// destructors are reconstructed; no other member is written yet. The
// constructors are fabricated -- the original supplies each object's own name.

class TBossTelesa : public TSpineEnemy {
public:
	TBossTelesa()
	    : TSpineEnemy("<TBossTelesa>")
	{
	}

	virtual ~TBossTelesa();
};

class TBossTelesaBody : public THitActor {
public:
	TBossTelesaBody()
	    : THitActor("<TBossTelesaBody>")
	{
	}

	virtual ~TBossTelesaBody();
};

class TBossTelesaKillSmallEnemy : public THitActor {
public:
	TBossTelesaKillSmallEnemy()
	    : THitActor("<TBossTelesaKillSmallEnemy>")
	{
	}

	virtual ~TBossTelesaKillSmallEnemy();
};

class TBossTelesaManager : public TEnemyManager {
public:
	TBossTelesaManager()
	    : TEnemyManager("<TBossTelesaManager>")
	{
	}

	virtual ~TBossTelesaManager();
};

class TBossTelesaTongue : public THitActor {
public:
	TBossTelesaTongue()
	    : THitActor("<TBossTelesaTongue>")
	{
	}

	virtual ~TBossTelesaTongue();
};

class TBubble : public TWalkerEnemy {
public:
	TBubble()
	    : TWalkerEnemy("<TBubble>")
	{
	}

	virtual ~TBubble();
};

class TBubbleManager : public TSmallEnemyManager {
public:
	TBubbleManager()
	    : TSmallEnemyManager("<TBubbleManager>")
	{
	}

	virtual ~TBubbleManager();
};

class TTelesaSlot : public TSlotDrum {
public:
	TTelesaSlot()
	    : TSlotDrum("<TTelesaSlot>")
	{
	}

	virtual ~TTelesaSlot();
};

#endif
