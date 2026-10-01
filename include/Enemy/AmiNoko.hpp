#ifndef ENEMY_AMINOKO_HPP
#define ENEMY_AMINOKO_HPP

#include <Enemy/SmallEnemy.hpp>
#include <Enemy/WalkerEnemy.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class TBGCheckData;
class TLiveManager;
class TAmiHit;

DECLARE_NERVE(TNerveAmiNokoFreeze, TLiveActor);

DECLARE_NERVE(TNerveAmiNokoDie, TLiveActor);

DECLARE_NERVE(TNerveAmiNokoAttack, TLiveActor);

DECLARE_NERVE(TNerveAmiNokoTurn, TLiveActor);

DECLARE_NERVE(TNerveAmiNokoWalkOnFence, TLiveActor);

class TAmiNokoSaveLoadParams : public TWalkerEnemyParams {
public:
	TAmiNokoSaveLoadParams(const char*);

	/* 0x32C */ TParamRT<f32> mSLElecRange;
	/* 0x340 */ TParamRT<f32> mSLMtxRotSpeed;
};

class TAmiNoko : public TWalkerEnemy {
public:
	TAmiNoko(const char* name = "アミノコ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void bind();
	virtual f32 getGravityY() const;
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual void behaveToWater(THitActor*);
	virtual void setWalkAnm();
	virtual void attackToMario();
	virtual void setMActorAndKeeper();
	virtual bool isHitValid(u32);
	virtual bool isCollidMove(THitActor*) { return false; }

	enum {
		PLANE_UNK0 = 0,
		PLANE_UNK1 = 1,
		PLANE_UNK2 = 2,
	};

	bool isDeadByWall();
	void creepToCurPathNode(f32);
#ifdef VERSION_GMSP01
	void emitEffects();
	void calcDirection();
#endif

public:
	/* 0x194 */ const TBGCheckData* unk194;
	/* 0x198 */ s32 unk198;
	/* 0x19C */ JGeometry::TVec3<f32> unk19C;
	/* 0x1A8 */ JGeometry::TVec3<f32> unk1A8;
	/* 0x1B4 */ JGeometry::TVec3<f32> unk1B4;
	/* 0x1C0 */ JGeometry::TVec3<f32> unk1C0;
	/* 0x1CC */ Mtx unk1CC;
	/* 0x1FC */ JGeometry::TVec3<f32> unk1FC;
#ifdef VERSION_GMSP01
	/* 0x208 */ TAmiNokoSaveLoadParams* unk20C;
	/* 0x20C */ u8 unk210;
	/* 0x210 */ TAmiHit* unk208;
#else
	/* 0x208 */ TAmiHit* unk208;
	/* 0x20C */ TAmiNokoSaveLoadParams* unk20C;
	/* 0x210 */ u8 unk210;
#endif
};

class TAmiHit : public THitActor {
public:
	TAmiHit(TAmiNoko*, const char* name = "アミノコ当り判定");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);

public:
	/* 0x68 */ TAmiNoko* unk68;
};

class TAmiNokoManager : public TSmallEnemyManager {
public:
	TAmiNokoManager(const char* name = "アミノコマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();
};

#endif // ENEMY_AMINOKO_HPP
