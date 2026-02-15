#ifndef ENEMY_CONDUCTOR_HPP
#define ENEMY_CONDUCTOR_HPP

#include <System/ParamInst.hpp>
#include <System/Params.hpp>
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
	int makeEnemyAppear(const JGeometry::TVec3<f32>&, const char*, int, int);
	TSpineEnemy* makeOneEnemyAppear(const JGeometry::TVec3<f32>&, const char*,
	                                int);
	void killEnemiesWithin(const JGeometry::TVec3<f32>&, f32);
	void genEnemyFromPollution();
	void clipAloneActors(JDrama::TGraphics*);
	void clipGenerators(JDrama::TGraphics*);

	virtual JDrama::TNameRef* searchF(u16, const char*);
	virtual void perform(u32, JDrama::TGraphics*);

	const TCondParams& getCondParams() const { return unk84; }

public:
	/* 0x10 */ JGadget::TList<TLiveManager*> unk10;
	/* 0x20 */ JGadget::TList<TEnemyManager*> unk20;
	/* 0x30 */ JGadget::TList<TLiveActor*> unk30;
	/* 0x40 */ JGadget::TList<JDrama::TViewObj*> unk40;
	/* 0x50 */ JGadget::TList<TAreaCylinderManager*> unk50;
	/* 0x60 */ JGadget::TList<TGenerator*> unk60;
	/* 0x70 */ JGadget::TList<SDLModelData*> unk70;
	/* 0x80 */ TGraphGroup* unk80;
	/* 0x84 */ TCondParams unk84;
	/* 0xF0 */ TStageEnemyInfoTable* unkF0;
	/* 0xF4 */ TNpcParams* unkF4;
	/* 0xF8 */ TAreaCylinderManager* unkF8;
	/* 0xFC */ int unkFC;
};

#endif
