#ifndef ENEMY_ENEMY_MANAGER_HPP
#define ENEMY_ENEMY_MANAGER_HPP

#include <Strategic/LiveManager.hpp>
#include <System/Params.hpp>
#include <System/ParamInst.hpp>

class J3DModelData;
class TSpineEnemy;
class MActor;

class TSpineEnemyParams : public TParams {
public:
	TSpineEnemyParams(const char*);

	/* 0x8 */ TParamRT<f32> mSLHeadHeight;
	/* 0x1C */ TParamRT<f32> mSLBodyRadius;
	/* 0x30 */ TParamRT<f32> mSLWallRadius;
	/* 0x44 */ TParamRT<f32> mSLClipRadius;
	/* 0x58 */ TParamRT<f32> mSLFarClip;
	/* 0x6C */ TParamRT<u8> mSLHitPointMax;
	/* 0x80 */ TParamRT<u8> mSLInstanceNum;
	/* 0x94 */ TParamRT<u8> mSLActiveEnemyNum;
};

class TSharedMActorSet {
public:
	TSharedMActorSet()
	    : unk0(nullptr)
	    , unk4(0)
	    , unk8(0)
	{
	}

	void init(MActorAnmData*, J3DModelData*, const char*, int);
	void calcAnm();
	void setScale(const JGeometry::TVec3<float>&);

	// fabricated
	MActor* getMActor(int idx) { return unk0[idx % unk4]; }

public:
	/* 0x0 */ MActor** unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
};

class TEnemyManager : public TLiveManager {
public:
	TEnemyManager(const char*);

	virtual ~TEnemyManager();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual TSpineEnemy* createEnemyInstance();
	virtual void clipEnemies(JDrama::TGraphics*);
	virtual void restoreDrawBuffer(u32) { }
	virtual void createEnemies(int);
	virtual void changeDrawBuffer(u32) { }

	void createEnemy();
	void createSharedMActorSet(const char**);
	TSharedMActorSet* getSharedMActorSet(int);
	void setSharedFlags();
	void updateAnmSoundShared();
	void copyFromShared();
	void performShared(u32, JDrama::TGraphics*);
	void getNearestEnemy(const JGeometry::TVec3<float>&);
	void getDeadEnemy();
	TSpineEnemy* getFarOutEnemy();
	void killChildren();
	void killChildrenWithin(const JGeometry::TVec3<float>&, float);
	void killOtherEnemies();
	int countLivingEnemy() const;
	void createCopyAnmMtx(int);
	bool copyAnmMtx(TSpineEnemy*);

	TSpineEnemyParams* getSaveParam() const { return unk38; }
	TSpineEnemy* getObj(int i) { return (TSpineEnemy*)TLiveManager::getObj(i); }

	static bool mIsCopyAnmMtx;

public:
	/* 0x38 */ TSpineEnemyParams* unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ TSharedMActorSet* unk40;
	/* 0x44 */ int unk44;
	/* 0x48 */ u32 unk48;
	/* 0x4C */ s32 unk4C;
	/* 0x50 */ u32 unk50;
};

#endif
