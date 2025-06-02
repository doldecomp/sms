#ifndef ENEMY_CONDUCTOR_HPP
#define ENEMY_CONDUCTOR_HPP

#include <System/ParamInst.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class TLiveManager;
class TEnemyManager;
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
	void getManagerByName(const char*);
	void polluterExterminated();
	void isBossDefeated();
	void conduct();
	void maskNFlagOfChildren(int, u32);
	void makeEnemyAppear(const JGeometry::TVec3<f32>&, const char*, int, int);
	void makeOneEnemyAppear(const JGeometry::TVec3<f32>&, const char*, int);
	void killEnemiesWithin(const JGeometry::TVec3<f32>&, f32);
	void genEnemyFromPollution();
	void clipAloneActors(JDrama::TGraphics*);
	void clipGenerators(JDrama::TGraphics*);

	virtual JDrama::TNameRef* searchF(u16, const char*);
	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ JGadget::TList_pointer_void unk10; // TODO: list of what?
	/* 0x20 */ JGadget::TList_pointer_void unk20;
	/* 0x30 */ JGadget::TList_pointer_void unk30;
	/* 0x40 */ JGadget::TList_pointer_void unk40;
	/* 0x50 */ JGadget::TList_pointer_void unk50;
	/* 0x60 */ JGadget::TList_pointer_void unk60;
	/* 0x70 */ JGadget::TList_pointer_void unk70;
	/* 0x80 */ TGraphGroup* unk80;
	/* 0x84 */ const char* unk84;
	/* 0x88 */ u32 unk88;
	/* 0x8C */ TParamRT<f32> mEnemyFarClip;
	/* 0xA0 */ TParamRT<f32> mGenerateRadiusMax;
	/* 0xB4 */ TParamRT<f32> mGenerateRadiusMin;
	/* 0xC8 */ TParamRT<s32> mGenerateTime;
	/* 0xDC */ TParamRT<f32> mGenerateProp;
	/* 0xF0 */ u32 unkF0;
	/* 0xF4 */ TNpcParams* unkF4;
	/* 0xF8 */ TViewObj* unkF8; // TODO: some kind of a manager
	/* 0xFC */ u32 unkFC;
};

#endif
