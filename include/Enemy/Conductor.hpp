#ifndef ENEMY_CONDUCTOR_HPP
#define ENEMY_CONDUCTOR_HPP

#include <System/ParamInst.hpp>
#include <System/Params.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class TLiveManager;
class TEnemyManager;
class TSpineEnemy;
class TLiveActor;
class TGenerator;
class TAreaCylinderManager;
class SDLModelData;
class TStageEnemyInfoTable;
class TGraphWeb;
class TGraphGroup;
class TNpcParams;

class TConductor;

extern TConductor* gpConductor;

class TConductor : public JDrama::TViewObj {
public:
	class TCondParams : public TParams {
	public:
		TCondParams();

		// fabricated
		f32 getEnemyFarClip() const { return mEnemyFarClip.get(); }

		/* 0x0C */ TParamRT<f32> mEnemyFarClip;
		/* 0x10 */ TParamRT<f32> mGenerateRadiusMax;
		/* 0x14 */ TParamRT<f32> mGenerateRadiusMin;
		/* 0x18 */ TParamRT<s32> mGenerateTime;
		/* 0x1C */ TParamRT<f32> mGenerateProp;
	};

	TConductor();
	~TConductor();
	void makeGraphGroup(void*);
	void initGraphGroup();
	void registerManager(TLiveManager*);
	void registerEnemyManager(TEnemyManager*);
	void registerAloneActor(TLiveActor*);
	void registerOtherObj(JDrama::TViewObj*);
	void registerGenerator(TGenerator*);
	void registerAreaCylinderManager(TAreaCylinderManager*);
	void registerSDLModelData(SDLModelData*);
	void registerEnemyInfoTable(TStageEnemyInfoTable*);
	void init();
	TGraphWeb* getGraphByName(const char*);
	TLiveManager* getManagerByName(const char*);
	void polluterExterminated();
	BOOL isBossDefeated();
	void conduct();
	void maskNFlagOfChildren(int, u32);
	int makeEnemyAppear(const JGeometry::TVec3<f32>& position,
	                    const char* manager_name, int count, int);
	TSpineEnemy* makeOneEnemyAppear(const JGeometry::TVec3<f32>& position,
	                                const char* manager_name, int);
	void killEnemiesWithin(const JGeometry::TVec3<f32>& position, f32 radius);
	void genEnemyFromPollution();
	void clipAloneActors(JDrama::TGraphics*);
	void clipGenerators(JDrama::TGraphics*);

	virtual JDrama::TNameRef* searchF(u16, const char*);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	const TCondParams& getCondParams() const { return mCondParams; }
	const TNpcParams* getNpcParams() const { return mNpcParams; }

protected:
	/* 0x10 */ JGadget::TList<TLiveManager*> mLiveManagers;
	/* 0x20 */ JGadget::TList<TEnemyManager*> mEnemyManagers;
	/* 0x30 */ JGadget::TList<TLiveActor*> mAloneActors;
	/* 0x40 */ JGadget::TList<JDrama::TViewObj*> mOtherObjects;
	/* 0x50 */ JGadget::TList<TAreaCylinderManager*> mAreaCylinderManagers;
	/* 0x60 */ JGadget::TList<TGenerator*> mGenerators;
	/* 0x70 */ JGadget::TList<SDLModelData*> mSDLModelDatas;
	/* 0x80 */ TGraphGroup* mGraphGroup;
	/* 0x84 */ TCondParams mCondParams;
	/* 0xF0 */ TStageEnemyInfoTable* mStageEnemyInfoTable;
	/* 0xF4 */ TNpcParams* mNpcParams;
	/* 0xF8 */ TAreaCylinderManager* mPollutionSpawnAreaManager;
	/* 0xFC */ BOOL mPollutionSpawningEnabled;
};

#endif
