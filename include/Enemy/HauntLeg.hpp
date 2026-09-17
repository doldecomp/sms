#ifndef ENEMY_HAUNT_LEG_HPP
#define ENEMY_HAUNT_LEG_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/Nerve.hpp>

class J3DNode;

// An object a haunting leg has possessed. Forwards a kill to its holder.
class THauntedObject : public THitActor {
public:
	THauntedObject(const char* name)
	    : THitActor(name)
	{
	}

	virtual ~THauntedObject() { }
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	void kill();
	void checkHit();

	/* 0x68 */ TLiveActor* mHaunter;
};

class THauntLeg : public TWalkerEnemy {
public:
	// UNUSED in the map (0x5c), so it is defined out of line in the .cpp:
	// an UNUSED symbol is never weak and therefore never an in-class body.
	THauntLeg(const char* name);

	virtual ~THauntLeg() { }
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void setGenerateAnm();
	virtual void setWalkAnm();
	virtual void setDeadAnm();
	virtual void setWaitAnm();
	virtual void setRunAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual BOOL isCollidMove(THitActor*);

	bool isUseCallBack();

	/* 0x194 */ THauntedObject* unk194;
	/* 0x198 */ u8 unk198;
	/* 0x199 */ u8 unk199;
	/* 0x19C */ THitActor* unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> mJumpVelocity;
	/* 0x1AC */ f32 mSpinAngle;
};

class THauntLegManager : public TSmallEnemyManager {
public:
	THauntLegManager(const char* name);

	virtual ~THauntLegManager() { }
	virtual void load(JSUMemoryInputStream& stream);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

DECLARE_NERVE(TNerveHauntLegHaunt, TLiveActor)

extern THauntLeg* gpCurHauntLeg;

#endif
