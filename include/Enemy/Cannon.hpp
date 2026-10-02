#ifndef ENEMY_CANNON_HPP
#define ENEMY_CANNON_HPP

#include <Enemy/SmallEnemy.hpp>
#include <JSystem/JDrama/JDRActor.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>
#include <JSystem/JDrama/JDRNameRef.hpp>
#include <JSystem/JGeometry.hpp>
#include <Strategic/Nerve.hpp>
#include <Strategic/HitActor.hpp>
#include <Strategic/SharedParts.hpp>
#include <Strategic/Spine.hpp>
#include <dolphin/types.h>

class JSUMemoryInputStream;
class SDLModelData;
class TBombHei;
class TLiveManager;
class TCannonDom;
class MAnmSound;
class TChorobei;
class TCannonSaveLoadParams;
class TMapObjBase;
class TMapCollisionMove;

DECLARE_NERVE(TNerveCannonObject, TLiveActor);

DECLARE_NERVE(TNerveCannonDamageDemo, TLiveActor);

DECLARE_NERVE(TNerveCannonDamage, TLiveActor);

DECLARE_NERVE(TNerveCannonSearch, TLiveActor);

DECLARE_NERVE(TNerveCannonClose, TLiveActor);

DECLARE_NERVE(TNerveCannonOpen, TLiveActor);

DECLARE_NERVE(TNerveCannonForceBombShoot, TLiveActor);

DECLARE_NERVE(TNerveCannonShoot, TLiveActor);

class TCannon : public TSmallEnemy {
public:
	TCannon(const char* name = "砲台");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void moveObject();
	virtual const char** getBasNameTable() const;
	virtual void reset();
	virtual bool isCollidMove(THitActor*) { return false; }
	virtual BOOL isInhibitedForceMove() { return TRUE; }
	virtual bool isHitVallid(u32) { return false; }

	void startChorobeiShout();
	void gateOpen();
	void killShootAct();
	bool isObject();
	void turnToGoal();
	void startMarioDemo();
	void startDemo();
	void deadCannon();
	void setKillerGoalPoint();
	void damage();
	void endKillerShoot();
	void killerShoot();
	void updateAttachPos();
	void hitHead(TBombHei*);
	void bombScaleUp();
	void bombShoot();
	void bombSet();
	void entryObjCollision();
	void calcObjCollision();
	static u8 mChorobeiJntIdx[1];
	static u8 mChorobeiHandJntIdx[1];
	static f32 mVelocityRate;
	static f32 mSearchRate;

public:
	/* 0x194 */ JGeometry::TVec3<f32> unk194;
	/* 0x1A0 */ TSmallEnemy* unk1A0;
	/* 0x1A4 */ TSpineEnemy* unk1A4;
	/* 0x1A8 */ TChorobei* unk1A8;
	/* 0x1AC */ TCannonDom* unk1AC[3];
	/* 0x1B8 */ TCannonDom* unk1B8;
	/* 0x1BC */ TSharedParts* unk1BC;
	/* 0x1C0 */ TMapCollisionMove* unk1C0[3];
	/* 0x1CC */ char unk1CC[0x14];
	/* 0x1E0 */ MtxPtr unk1E0;
	/* 0x1E4 */ TPosition3f unk1E4;
	/* 0x214 */ s32 unk214;
	/* 0x218 */ s32 unk218;
	/* 0x21C */ u8 unk21C;
	/* 0x21D */ char unk21D[0x3];
	/* 0x220 */ f32 unk220;
	/* 0x224 */ f32 unk224[3];
	/* 0x230 */ u8 unk230;
	/* 0x231 */ char unk231[0x3];
	/* 0x234 */ f32 unk234;
	/* 0x238 */ u8 unk238;
	/* 0x239 */ u8 unk239;
	/* 0x23A */ char unk23A[0x2];
	/* 0x23C */ JGeometry::TVec3<f32> unk23C;
	/* 0x248 */ JGeometry::TVec3<f32> unk248;
	/* 0x254 */ TMapObjBase* unk254;
	/* 0x258 */ TMapCollisionMove* unk258;
	/* 0x25C */ JGeometry::TVec3<f32> unk25C[4];
	/* 0x28C */ TCannonSaveLoadParams* unk28C;
	/* 0x290 */ u8 unk290;
	/* 0x291 */ char unk291[0x3];
	/* 0x294 */ JGeometry::TVec3<f32> unk294;
	/* 0x2A0 */ JGeometry::TVec3<f32> unk2A0;
	/* 0x2AC */ f32 unk2AC;
	/* 0x2B0 */ TMapCollisionMove* unk2B0;
};

class TCannonDom : public TSharedParts {
public:
	TCannonDom(TLiveActor*, int, SDLModelData*, u32, const char* name = "砲身");

	virtual void perform(u32, JDrama::TGraphics*);

	void setBckAnm(int);

public:
	/* 0x1C */ MAnmSound* unk1C;
	/* 0x20 */ const char* unk20;
	/* 0x24 */ u8 unk24;
	/* 0x25 */ char unk25[0x3];
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
};

class TChorobei : public THitActor {
public:
	TChorobei(TCannon*, int, const char* name = "チョロベー");

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);

	bool isDownEnd();
	bool isUpEnd();
	void checkHit();
	void setBckAnm(int);

public:
	/* 0x68 */ TCannon* unk68;
	/* 0x6C */ TSharedParts* unk6C;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ MAnmSound* unk74;
	/* 0x78 */ const char* unk78;
	/* 0x7C */ f32 unk7C;
};

class TCannonManager : public TSmallEnemyManager {
public:
	TCannonManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*) { }
};

class TCannonSaveLoadParams : public TSmallEnemyParams {
public:
	TCannonSaveLoadParams(const char*);

public:
	/* 0x2D4 */ TParamRT<f32> mSLHideDist;
	/* 0x2E8 */ TParamRT<f32> mSLBombDist;
	/* 0x2FC */ TParamRT<f32> mSLKillerDist;
	/* 0x310 */ TParamRT<s32> mSLBombInterval;
	/* 0x324 */ TParamRT<s32> mSLKillerInterval;
	/* 0x338 */ TParamRT<s32> mSLShootInterval;
	/* 0x34C */ TParamRT<f32> mSLChorobeiAttackRadius;
	/* 0x360 */ TParamRT<f32> mSLChorobeiAttackHeight;
	/* 0x374 */ TParamRT<f32> mSLChorobeiDamageRadius;
	/* 0x388 */ TParamRT<f32> mSLChorobeiDamageHeight;
	/* 0x39C */ TParamRT<f32> mSLKillerTransYOffset;
	/* 0x3B0 */ TParamRT<f32> mSLBombHeiGenerateRate;
	/* 0x3C4 */ TParamRT<f32> mSLThrowXZSpeed;
};

#endif // ENEMY_CANNON_HPP
