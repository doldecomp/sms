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
	virtual ~TBathtub();

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void control();

	void hipdrop(const JGeometry::TVec3<f32>&);
	void quake(const JGeometry::TVec3<f32>&);
	int getNumGripsDead() const;
	void tumble(f32, f32);
	MtxPtr getTakingMtx();
	MtxPtr getSubmarineMtxInDemo();
	MtxPtr getPeachMtxInDemo();
	MtxPtr getKoopaJrMtxInDemo();
	void calcBathtubData();
	void setupCollisions_();
	void removeCollisions_(); // Unused
	void startDemo();
	bool allowsTumble() const;
	bool getNearGrip(const JGeometry::TVec3<f32>&, f32, f32*) const;
	// f32, not u8: it returns unk13C[index] in f1, and TKoopa::mTargetDir is
	// an f32.
	f32 getNextJuncture(const JGeometry::TVec3<f32>&,
	                    const JGeometry::TVec3<f32>&) const;
	// u8, not BOOL: the three TKoopa callers keep the result in a u8 local and
	// test it twice, and retail reloads it as a byte both times.
	u8 getNextGrip(const JGeometry::TVec3<f32>&, const JGeometry::TVec3<f32>&,
	               f32, f32*) const;
	void updatePosture_();
	int getNumKillerLaunchable() const;
	bool isKillerAttackable() const;
	int getNumKillerBurstable() const;
	bool isBreaking() const;                                // Unused
	bool isKillerLaunchable() const;                        // Unused
	void showMessage(u32);                                  // Unused
	f32 getNearJuncture(const JGeometry::TVec3<f32>&) const; // Unused
	MtxPtr getKoopaMtxInDemo();                             // Unused
	MtxPtr getWaterMtx(int);                                // Unused
	MtxPtr getShineEffectMtx();                             // Unused
	MtxPtr getShineMtx();                                   // Unused
	void liftMario(const JGeometry::TVec3<f32>&);           // Unused
	void trample(const JGeometry::TVec3<f32>&);             // Unused

	const TBathtubData& getBathtubData() const { return mBathtubData; }

public:
	// fabricated: header round 20 accessor candidates
	TBathtubParams* getUnk16C() const { return unk16C; }

	/* 0x138 */ MActorAnmData* unk138;
	/* 0x13C */ f32 unk13C[5];
	/* 0x150 */ f32 unk150[5];
	/* 0x164 */ TMapCollisionMove** unk164;
	/* 0x168 */ TBathtubGrip** unk168;
	/* 0x16C */ TBathtubParams* unk16C;
	/* 0x170 */ TBathtubData mBathtubData;
	/// Bathtub tilt, as a unit quaternion.
	/* 0x1D8 */ JGeometry::TQuat4<f32> mQuat;
	/// Angular velocity that integrates into mQuat each frame.
	/* 0x1E8 */ JGeometry::TVec3<f32> mAngleVel;
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
	/// The five water-spout joints, indexed by grip, hence the shuffled
	/// order they are filled in: water4, water5, water1, water2, water3.
	/* 0x27C */ int mWaterJntIdx[5];
	/* 0x290 */ int unk290;
	/* 0x294 */ int unk294;
	/* 0x298 */ u8 unk298;
	/* 0x299 */ u8 unk299;
	/* 0x29A */ u8 unk29A;
	/* 0x29C */ MActor* unk29C;
	/* 0x2A0 */ u32 unk2A0;
};

class TBathtubGripPartsHard;
class TBathtubGripPartsFragile;

class TBathtubGrip : public TMapObjBase {
public:
	TBathtubGrip(TBathtub*, f32, MActorAnmData*, const char*);
	virtual ~TBathtubGrip() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void control();
	virtual void kill();

	void reset(); // Unused
	bool isCracking() const; // Unused
	void startCrack(); // Unused
	void startBreak(int, int, f32); // Unused
	bool marioIsOn() const; // Unused
	void removeCollisions_(); // Unused
	void setupCollisions_(); // Unused

public:
	/* 0x138 */ JGeometry::TVec3<f32> unk138[2];
	/* 0x150 */ TMapCollisionMove* mFragileCollisions[5];
	/* 0x164 */ TMapCollisionMove* mHardCollisions[17];
	/* 0x1A8 */ TBathtubGripPartsFragile* mFragileParts[5];
	/* 0x1BC */ TBathtubGripPartsHard* mHardParts[17];
	/* 0x200 */ int mJointIndices[17];
	/* 0x244 */ TBathtub* mBathtub;
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

class TBathtubGripParts : public TLiveActor {
public:
	TBathtubGripParts(const char*, int, TBathtubGrip*);
	virtual ~TBathtubGripParts() { }
	virtual Mtx* getRootJointMtx() const;

public:
	/* 0xF4 */ TBathtubGrip* mGrip;
	/* 0xF8 */ int mIndex;
};

class TBathtubGripPartsFragile : public TBathtubGripParts {
public:
	TBathtubGripPartsFragile(int, TBathtubGrip*);
	virtual ~TBathtubGripPartsFragile() { }
	virtual BOOL receiveMessage(THitActor*, u32);
};

class TBathtubGripPartsHard : public TBathtubGripParts {
public:
	TBathtubGripPartsHard(int, TBathtubGrip*);
	virtual ~TBathtubGripPartsHard() { }
	virtual BOOL receiveMessage(THitActor*, u32);
};

class TBathtubParams : public TParams {
public:
	TBathtubParams();

	/* 0x8 */ TParamRT<u8> resetGrip;
	/* 0x1C */ TParamRT<s32> trampleRelease;
	/* 0x30 */ TParamRT<s32> trampleRecover;
	/* 0x44 */ TParamRT<s32> quakeRelease;
	/* 0x58 */ TParamRT<s32> quakeRecover;
	/* 0x6C */ TParamRT<s32> hipdropRelease;
	/* 0x80 */ TParamRT<s32> hipdropRecover;
	/* 0x94 */ TParamRT<s32> breakCount0;
	/* 0xA8 */ TParamRT<s32> breakCount1;
	/* 0xBC */ TParamRT<s32> breakCount2;
	/* 0xD0 */ TParamRT<s32> breakCount3;
	/* 0xE4 */ TParamRT<s32> launchStopCount;
	/* 0xF8 */ TParamRT<f32> animSpeed0;
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

#endif
