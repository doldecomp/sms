#ifndef ENEMY_HAUNT_LEG_HPP
#define ENEMY_HAUNT_LEG_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>

// An object a haunting leg has possessed. Forwards a kill to its holder.
class THauntedObject : public TTakeActor {
public:
	THauntedObject(const char* name)
	    : TTakeActor(name)
	{
	}

	virtual ~THauntedObject();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
};

class THauntLeg : public TWalkerEnemy {
public:
	THauntLeg(const char* name);

	virtual ~THauntLeg();
	virtual void init(TLiveManager*);
	virtual void setMActorAndKeeper();
	virtual void reset();
	virtual void calcRootMatrix();
	virtual void setGenerateAnm();
	virtual void setWaitAnm();
	virtual void setWalkAnm();
	virtual void setRunAnm();
	virtual void setDeadAnm();
	virtual void attackToMario();
	virtual BOOL isCollidMove(THitActor*);
	virtual const char** getBasNameTable() const;

	MtxPtr getTakingMtx();

	/* 0x194 */ THauntedObject* unk194;
	/* 0x198 */ u8 unk198;
	/* 0x199 */ u8 unk199;
	/* 0x19C */ THitActor* unk19C;
	/* 0x1A0 */ u8 unk1A0[0x1B0 - 0x1A0];
};

class THauntLegManager : public TSmallEnemyManager {
public:
	THauntLegManager(const char* name);

	virtual ~THauntLegManager();
	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();
	virtual TLiveActor* createEnemyInstance();
	virtual void initSetEnemies();
};

DECLARE_NERVE(TNerveHauntLegHaunt, TLiveActor)

extern THauntLeg* gpCurHauntLeg;

#endif
