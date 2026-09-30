#ifndef MOVE_BG_MAP_OBJ_CORONA_HPP
#define MOVE_BG_MAP_OBJ_CORONA_HPP

#include <MoveBG/MapObjBase.hpp>
#include <Map/BathWaterManager.hpp>

class MActorAnmData;
class TBathtubGrip;
class MActor;

class TBathtubParams : public TParams {
public:
	TBathtubParams();

public:
	/* 0x008 */ TParamRT<u8> resetGrip;
	/* 0x01C */ TParamRT<s32> trampleRelease;
	/* 0x030 */ TParamRT<s32> trampleRecover;
	/* 0x044 */ TParamRT<s32> quakeRelease;
	/* 0x058 */ TParamRT<s32> quakeRecover;
	/* 0x06C */ TParamRT<s32> hipdropRelease;
	/* 0x080 */ TParamRT<s32> hipdropRecover;
	/* 0x094 */ TParamRT<s32> breakCount0;
	/* 0x0A8 */ TParamRT<s32> breakCount1;
	/* 0x0BC */ TParamRT<s32> breakCount2;
	/* 0x0D0 */ TParamRT<s32> breakCount3;
	/* 0x0E4 */ TParamRT<s32> launchStopCount;
	/* 0x0F8 */ TParamRT<f32> animSpeed0;
	/* 0x10C */ TParamRT<f32> animSpeed1;
	/* 0x120 */ TParamRT<f32> animSpeed2;
	/* 0x134 */ TParamRT<f32> animSpeed3;
	/* 0x148 */ TParamRT<f32> animSpeed4;
	/* 0x15C */ TParamRT<f32> shake;
	/* 0x170 */ TParamRT<f32> watermark;
	/* 0x184 */ TParamRT<f32> maxAngle;
	/* 0x198 */ TParamRT<f32> angleVelDamp;
	/* 0x1AC */ TParamRT<f32> rebound;
	/* 0x1C0 */ TParamRT<f32> shakeDamp;
	/* 0x1D4 */ TParamRT<f32> marioWeight;
	/* 0x1E8 */ TParamRT<f32> marioDropWeight;
	/* 0x1FC */ TParamRT<f32> outerHeight;
};

class TBathtub : public TMapObjBase {
public:
	TBathtub(const char* name = "バスタブ");
	virtual ~TBathtub();

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void control();

	void hipdrop(const JGeometry::TVec3<f32>&);
	void quake(const JGeometry::TVec3<f32>&);
	int getNumGripsDead() const;
	void tumble(f32, f32);
	MtxPtr getSubmarineMtxInDemo();
	MtxPtr getPeachMtxInDemo();
	MtxPtr getKoopaJrMtxInDemo();
	void calcBathtubData();
	void setupCollisions_();
	void removeCollisions_(); // Unused
	void startDemo();
	bool allowsTumble() const;
	bool getNearGrip(const JGeometry::TVec3<f32>&, f32, f32*) const;
	f32 getNextJuncture(const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec3<f32>&) const;
	bool getNextGrip(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	                 f32, f32*) const;
	void updatePosture_();
	int getNumKillerLaunchable() const;
	bool isKillerAttackable() const;
	int getNumKillerBurstable() const;
	bool isBreaking() const;                                // Unused
	bool isKillerLaunchable() const;                        // Unused
	void showMessage(u32);                                  // Unused
	u8 getNearJuncture(const JGeometry::TVec3<f32>&) const; // Unused
	MtxPtr getKoopaMtxInDemo();                             // Unused
	MtxPtr getWaterMtx(int);                                // Unused
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
	/* 0x1D8 */ JGeometry::TQuat4<f32> unk1D8;
	/* 0x1E8 */ JGeometry::TVec3<f32> unk1E8;
	/* 0x1F4 */ JGeometry::TVec3<f32> unk1F4;
	/* 0x200 */ JGeometry::TVec3<f32> unk200;
	/* 0x20C */ u8 unk20C[0x30];
	/* 0x23C */ f32 unk23C;
	/* 0x240 */ f32 unk240;
	/* 0x244 */ f32 unk244;
	/* 0x248 */ int unk248;
	/* 0x24C */ int unk24C;
	/* 0x250 */ int unk250;
	/* 0x254 */ int unk254;
	/* 0x258 */ int unk258;
	/* 0x25C */ int unk25C;
	/* 0x260 */ int mMarioJntIdx;
	/* 0x264 */ int mStarJntIdx;
	/* 0x268 */ int mShineBodyJntIdx;
	/* 0x26C */ int mSubmarineJntIdx;
	/* 0x270 */ int mDuckJntIdx;
	/* 0x274 */ int mJuniorJntIdx;
	/* 0x278 */ int mKoopaJntIdx;
	/* 0x27C */ int mWaterJntIdx[5];
	/* 0x290 */ int unk290;
	/* 0x294 */ int unk294;
	/* 0x298 */ u8 unk298;
	/* 0x299 */ u8 unk299;
	/* 0x29A */ u8 unk29A;
	/* 0x29C */ MActor* unk29C;
	/* 0x2A0 */ u32 unk2A0;
};

class TBathtubGripParts : public TLiveActor {
public:
	TBathtubGripParts(const char*, int, TBathtubGrip*); // Unused

	virtual Mtx* getRootJointMtx() const;

public:
	/* 0xF4 */ TBathtubGrip* unkF4;
	/* 0xF8 */ int unkF8;
};

class TBathtubGripPartsHard : public TBathtubGripParts {
public:
	TBathtubGripPartsHard(int, TBathtubGrip*); // Unused

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
};

class TBathtubGripPartsFragile : public TBathtubGripParts {
public:
	TBathtubGripPartsFragile(int, TBathtubGrip*); // Unused

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
};

class TBathtubGrip : public TMapObjBase {
public:
	TBathtubGrip(TBathtub*, f32, MActorAnmData*,
	             const char* = "壊れかけのバスタブの取っ手");

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void control();
	virtual void kill();

	void setupCollisions_();        // Unused
	void removeCollisions_();       // Unused
	void reset();                   // Unused
	bool marioIsOn() const;         // Unused
	void startBreak(int, int, f32); // Unused
	void startCrack();              // Unused
	bool isCracking() const;        // Unused

public:
	/* 0x138 */ JGeometry::TVec3<f32> unk138[2];
	/* 0x150 */ TMapCollisionMove* unk150[5];
	/* 0x164 */ TMapCollisionMove* unk164[17];
	/* 0x1A8 */ TBathtubGripPartsFragile* unk1A8[5];
	/* 0x1BC */ TBathtubGripPartsHard* unk1BC[17];
	/* 0x200 */ int unk200[17];
	/* 0x244 */ TBathtub* unk244;
	/* 0x248 */ u8 unk248;
	/* 0x249 */ u8 unk249;
	/* 0x24A */ u8 unk24A;
	/* 0x24B */ u8 unk24B;
	/* 0x24C */ f32 unk24C;
	/* 0x250 */ f32 unk250;
	/* 0x254 */ int unk254;
	/* 0x258 */ int unk258;
	/* 0x25C */ MActor* unk25C;
	/* 0x260 */ u8 unk260;
};

#endif
