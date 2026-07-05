#ifndef MOVE_BG_MAP_OBJ_CORONA_HPP
#define MOVE_BG_MAP_OBJ_CORONA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <Map/BathWaterManager.hpp>

class MActorAnmData;
class TBathtubGrip;
class TBathtubParams;
class MActor;

class TBathtub : public TMapObjBase {
public:
	TBathtub(const char* name = "バスタブ");

	void loadAfter();
	void hipdrop(const JGeometry::TVec3<f32>&);
	void quake(const JGeometry::TVec3<f32>&);
	u8 getNumGripsDead() const;
	void tumble(f32, f32);
	MtxPtr getTakingMtx();
	MtxPtr getSubmarineMtxInDemo();
	MtxPtr getPeachMtxInDemo();
	MtxPtr getKoopaJrMtxInDemo();
	BOOL receiveMessage(THitActor* sender, u32 message);
	Mtx* getRootJointMtx() const;
	void perform(u32, JDrama::TGraphics*);
	void control();
	void calcBathtubData();
	void setupCollisions_();
	void removeCollisions_(); // Unused
	void startDemo();
	bool allowsTumble() const;
	void calcRootMatrix();
	bool getNearGrip(const JGeometry::TVec3<f32>&, f32, f32*) const;
	u8 getNextJuncture(const JGeometry::TVec3<f32>&,
	                   const JGeometry::TVec3<f32>&) const;
	u8 getNextGrip(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               f32, f32*) const;
	void updatePosture_();
	void load(JSUMemoryInputStream&);
	u8 getNumKillerLaunchable() const;
	bool isKillerAttackable() const;
	u8 getNumKillerBurstable() const;
	bool isBreaking() const;                                // Unused
	bool isKillerLaunchable() const;                        // Unused
	void showMessage(u32);                                  // Unused
	u8 getNearJuncture(const JGeometry::TVec3<f32>&) const; // Unused
	MtxPtr getKoopaMtxInDemo();                             // Unused
	MtxPtr getWaterMtx(s32);                                // Unused
	MtxPtr getShineEffectMtx();                             // Unused
	MtxPtr getShineMtx();                                   // Unused
	void liftMario(const JGeometry::TVec3<f32>&);           // Unused
	void trample(const JGeometry::TVec3<f32>&);             // Unused

	const TBathtubData& getBathtubData() const { return mBathtubData; }

public:
	/* 0x138 */ MActorAnmData* unk138;
	/* 0x13C */ f32 unk13C[5];
	/* 0x150 */ f32 unk150[5];
	/* 0x164 */ TMapCollisionMove** unk164;
	/* 0x168 */ TBathtubGrip** unk168;
	/* 0x16C */ TBathtubParams* unk16C;
	/* 0x170 */ TBathtubData mBathtubData;
	/* 0x1D8 */ f32 unk1D8;
	/* 0x1DC */ f32 unk1DC;
	/* 0x1E0 */ f32 unk1E0;
	/* 0x1E4 */ f32 unk1E4;
	/* 0x1E8 */ f32 unk1E8;
	/* 0x1EC */ f32 unk1EC;
	/* 0x1F0 */ f32 unk1F0;
	/* 0x1F4 */ JGeometry::TVec3<f32> unk1F4;
	/* 0x200 */ JGeometry::TVec3<f32> unk200;
	/* 0x20C */ u8 unk20C[0x30];
	/* 0x23C */ f32 unk23C;
	/* 0x240 */ f32 unk240;
	/* 0x244 */ f32 unk244;
	/* 0x248 */ int unk248;
	/* 0x24C */ int unk24C;
	/* 0x250 */ int unk250;
	/* 0x254 */ u32 unk254;
	/* 0x258 */ u32 unk258;
	/* 0x25C */ u32 unk25C;
	/* 0x260 */ int mMarioJntIdx;
	/* 0x264 */ int mStarJntIdx;
	/* 0x268 */ int mShineBodyJntIdx;
	/* 0x26C */ int mSubmarineJntIdx;
	/* 0x270 */ int mDuckJntIdx;
	/* 0x274 */ int mJuniorJntIdx;
	/* 0x278 */ int mKoopaJntIdx;
	/* 0x27C */ int mWater4JntIdx;
	/* 0x280 */ int mWater5JntIdx;
	/* 0x284 */ int mWater1JntIdx;
	/* 0x288 */ int mWater2JntIdx;
	/* 0x28C */ int mWater3JntIdx;
	/* 0x290 */ int unk290;
	/* 0x294 */ int unk294;
	/* 0x298 */ u8 unk298;
	/* 0x29A */ u8 unk29A;
	/* 0x29C */ MActor* unk29C;
	/* 0x2A0 */ u32 unk2A0;
};

#endif
