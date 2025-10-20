#ifndef MOVE_BG_MAP_OBJ_CORONA_HPP
#define MOVE_BG_MAP_OBJ_CORONA_HPP

#include <MoveBG/MapObjBase.hpp>

class TBathtub : public TMapObjBase {
public:
	TBathtub(const char*);
	~TBathtub();

	void loadAfter();
	void hipdrop(const JGeometry::TVec3<float>&);
	void quake(const JGeometry::TVec3<float>&);
	u8 getNumGripsDead() const;
	void tumble(float, float);
	MtxPtr getTakingMtx();
	MtxPtr getSubmarineMtxInDemo();
	MtxPtr getPeachMtxInDemo();
	MtxPtr getKoopaJrMtxInDemo();
	BOOL receiveMessage(THitActor* sender, u32 message);
	Mtx* getRootJointMtx() const;
	void perform(unsigned long, JDrama::TGraphics*);
	void control();
	void calcBathtubData();
	void setupCollisions_();
	void removeCollisions_(); // Unused
	void startDemo();
	bool allowsTumble() const;
	void calcRootMatrix();
	bool getNearGrip(const JGeometry::TVec3<float>&, float, float*) const;
	u8 getNextJuncture(const JGeometry::TVec3<float>&,
	                   const JGeometry::TVec3<float>&) const;
	bool getNextGrip(const JGeometry::TVec3<float>&,
	                 const JGeometry::TVec3<float>&, float, float*) const;
	void updatePosture_();
	void load(JSUMemoryInputStream&);
	u8 getNumKillerLaunchable() const;
	bool isKillerAttackable() const;
	u8 getNumKillerBurstable() const;
	bool isBreaking() const;                                  // Unused
	bool isKillerLaunchable() const;                          // Unused
	void showMessage(unsigned long);                          // Unused
	u8 getNearJuncture(const JGeometry::TVec3<float>&) const; // Unused
	MtxPtr getKoopaMtxInDemo();                               // Unused
	MtxPtr getWaterMtx(int);                                  // Unused
	MtxPtr getShineEffectMtx();                               // Unused
	MtxPtr getShineMtx();                                     // Unused
	void liftMario(const JGeometry::TVec3<float>&);           // Unused
	void trample(const JGeometry::TVec3<float>&);             // Unused

public:
	// Will start at 0x138
};

#endif
