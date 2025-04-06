#ifndef ENEMY_CONDUCTOR_HPP
#define ENEMY_CONDUCTOR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TLiveManager;
class TEnemyManager;
class TLiveActor;
class TGenerator;
class TAreaCylinderManager;
class SDLModelData;
class TStageEnemyInfoTable;

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
	void getGraphByName(const char*);
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
};

#endif
