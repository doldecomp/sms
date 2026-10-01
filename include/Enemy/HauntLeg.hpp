#ifndef ENEMY_HAUNTLEG_HPP
#define ENEMY_HAUNTLEG_HPP

#include <Enemy/WalkerEnemy.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class TLiveManager;
class THauntedObject;

DECLARE_NERVE(TNerveHauntLegHaunt, TLiveActor);

class THauntLeg : public TWalkerEnemy {
public:
	THauntLeg(const char* = "ハントレッグ");

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
	virtual bool isCollidMove(THitActor*);

	bool isUseCallBack();

public:
	/* 0x194 */ THauntedObject* unk194;
	/* 0x198 */ u8 unk198;
	/* 0x199 */ u8 unk199;
	/* 0x19C */ TTakeActor* unk19C;
	/* 0x1A0 */ JGeometry::TVec3<f32> unk1A0;
	/* 0x1AC */ f32 unk1AC;
};

class THauntedObject : public THitActor {
public:
	THauntedObject()
	    : THitActor("ハントオブジェクト")
	{
	}
	virtual BOOL receiveMessage(THitActor*, u32);

	void kill();
	void checkHit();

public:
	/* 0x68 */ THauntLeg* unk68;
};

class THauntLegManager : public TSmallEnemyManager {
public:
	THauntLegManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
	virtual void initSetEnemies();
};

#endif // ENEMY_HAUNTLEG_HPP
