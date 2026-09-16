#ifndef ENEMY_HANASAMBOOBJ_HPP
#define ENEMY_HANASAMBOOBJ_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <MoveBG/MapObjBase.hpp>
#include <Strategic/HitActor.hpp>

// Class hierarchy recovered from each destructor's vtable chain. Only the
// destructors are reconstructed; no other member is written yet. The
// constructors are fabricated -- the original supplies each object's own name.

class THanaSambo : public TSpineEnemy {
public:
	THanaSambo()
	    : TSpineEnemy("<THanaSambo>")
	{
	}

	virtual ~THanaSambo();
};

class THanaSamboHead : public THitActor {
public:
	THanaSamboHead()
	    : THitActor("<THanaSamboHead>")
	{
	}

	virtual ~THanaSamboHead();
};

class THanaSamboManager : public TSmallEnemyManager {
public:
	THanaSamboManager()
	    : TSmallEnemyManager("<THanaSamboManager>")
	{
	}

	virtual ~THanaSamboManager();
};

class TSamboFlower : public TSpineEnemy {
public:
	TSamboFlower()
	    : TSpineEnemy("<TSamboFlower>")
	{
	}

	virtual ~TSamboFlower();
};

class TSamboFlowerManager : public TEnemyManager {
public:
	TSamboFlowerManager()
	    : TEnemyManager("<TSamboFlowerManager>")
	{
	}

	virtual ~TSamboFlowerManager();
};

class TSamboHead : public TWalkerEnemy {
public:
	TSamboHead()
	    : TWalkerEnemy("<TSamboHead>")
	{
	}

	virtual ~TSamboHead();
};

class TSamboHeadManager : public TSmallEnemyManager {
public:
	TSamboHeadManager()
	    : TSmallEnemyManager("<TSamboHeadManager>")
	{
	}

	virtual ~TSamboHeadManager();
};

class TSamboLeaf : public JDrama::TViewObj {
public:
	TSamboLeaf()
	    : JDrama::TViewObj("<TSamboLeaf>")
	{
	}

	virtual ~TSamboLeaf();
	// fabricated: TViewObj::perform is pure virtual.
	virtual void perform(u32 cue, JDrama::TGraphics* graphics) { }
};

#endif
