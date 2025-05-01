#ifndef MOVE_BG_MAP_OBJ_BASE_HPP
#define MOVE_BG_MAP_OBJ_BASE_HPP

#include <Strategic/LiveActor.hpp>

class J3DJoint;
class J3DMaterialTable;
class TMapCollisionStatic;
class TMapCollisionMove;
class TMapCollisionWarp;
class TJointObj;
class JPABaseEmitter;

// fabricated
struct TMapObjAnimData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ const char* unk4;
	/* 0x8 */ u8 unk8;
	/* 0xC */ const char* unkC;
	/* 0x10 */ const char* unk10;
};

// the only real name we have, everything else is fabricated
struct TMapObjAnimDataInfo {
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ const TMapObjAnimData* unk4;
};

struct TMapObjHitDataTable {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
};

// fabricated
struct TMapObjHitInfo {
	/* 0x0 */ int unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ const TMapObjHitDataTable* unkC;
};

// fabricated
struct TMapObjCollisionData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u16 unk4;
};

// fabricated
struct TMapObjCollisionInfo {
	/* 0x0 */ u16 unk0;
	/* 0x2 */ u16 unk2;
	/* 0x4 */ const TMapObjCollisionData* unk4;
};

// fabricated
struct TMapObjSoundData {
	/* 0x0 */ u32 unk0[10];
};

// fabricated
struct TMapObjSoundInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ const TMapObjSoundData* unk4;
};

// fabricated
struct TMapObjPhysicalData {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
};

// fabricated
struct TMapObjPhysicalInfo {
	/* 0x0 */ u32 unk0;
	/* 0x4 */ TMapObjPhysicalData* unk4;
	/* 0x8 */ u32 unk8;
};

// fabricated
struct TMapObjSinkData {
	/* 0x0 */ f32 unk0;
	/* 0x4 */ f32 unk4;
};

class J3DModelData;

// fabricated
struct TMapObjHoldData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ const char* unk4;
	/* 0x8 */ J3DModelData* unk8;
	/* 0xC */ J3DModel* unkC;
	/* 0x10 */ MtxPtr unk10;
};

class J3DAnmTransform;
class J3DFrameCtrl;

// fabricated
struct TMapObjMoveData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ J3DAnmTransform* unk4;
	/* 0x8 */ J3DFrameCtrl* unk8;
};

// fabricated
struct TMapObjData {
	/* 0x0 */ const char* unk0;
	/* 0x4 */ u32 unk4;
	/* 0x8 */ const char* unk8;
	/* 0xC */ const char* unkC;
	/* 0x10 */ const TMapObjAnimDataInfo* mAnim;
	/* 0x14 */ const TMapObjHitInfo* mHit;
	/* 0x18 */ const TMapObjCollisionInfo* mCollision;
	/* 0x1C */ const TMapObjSoundInfo* mSound;
	/* 0x20 */ const TMapObjPhysicalInfo* mPhysical;
	/* 0x24 */ const TMapObjSinkData* mSink;
	/* 0x28 */ TMapObjHoldData* mHold;
	/* 0x2C */ TMapObjMoveData* mMove;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ u32 unk34; // TODO: these are flags
	/* 0x38 */ u32 unk38;
};

class TMapObjBase : public TLiveActor {
public:
	TMapObjBase(const char*);

	virtual ~TMapObjBase() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void ensureTakeSituation();
	virtual void getRadiusAtY(f32) const;
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 getShadowType();
	virtual void kill();
	virtual void appear();
	virtual void makeObjAppeared();
	virtual void makeObjDead();
	virtual void changeObjSRT(const JGeometry::TVec3<f32>&,
	                          const JGeometry::TVec3<f32>&,
	                          const JGeometry::TVec3<f32>&);
	virtual void changeObjMtx(MtxPtr);
	virtual void updateObjMtx();
	virtual void setUpCurrentMapCollision();
	virtual void setObjHitData(u16);
	virtual void setModelMtx(MtxPtr);
	virtual void initMapObj();
	virtual void loadBeforeInit(JSUMemoryInputStream&);
	virtual void initMapCollisionData();
	virtual void makeMActors();
	virtual u32 getSDLModelFlag() const;
	virtual void checkIllegalAttr() const;
	virtual void calc();
	virtual void draw() const;
	virtual void dead();
	virtual void touchActor(THitActor*);
	virtual void touchPlayer(THitActor*);
	virtual u32 touchWater(THitActor*);
	virtual void touchEnemy(THitActor*);
	virtual void touchBoss(THitActor*);
	virtual void makeObjDefault();
	virtual u16 getHitObjNumMax();
	virtual void getDepthAtFloating();

	void initAndRegister(const char*);
	void moveByBck();
	void startAnim(u16);
	void startBck(const char*);
	void startControlAnim(u16);
	void stopAnim();
	bool animIsFinished() const;
	bool hasAnim(u16) const;
	bool hasModelOrAnimData(u16) const;
	void startSound(u16);
	void soundBas(u32, f32, f32);
	void setUpMapCollision(u16);
	void removeMapCollision();
	void sleep();
	void awake();
	void initActorData();
	void initModelData();
	MActor* initMActor(const char*, const char*, u32);
	void initBckMoveData();
	void initObjCollisionData();
	void initHoldData();
	void initEventData();
	void initUnique();
	void setMatTableTex(J3DMaterialTable*);
	void setMatTable(J3DMaterialTable*);
	JPABaseEmitter* emitAndRotate(long, unsigned char,
	                              const JGeometry::TVec3<f32>*) const;
	JPABaseEmitter* emitAndScale(long, unsigned char,
	                             const JGeometry::TVec3<f32>*,
	                             const JGeometry::TVec3<f32>&) const;
	JPABaseEmitter* emitAndBindScale(long, unsigned char,
	                                 const JGeometry::TVec3<f32>*,
	                                 const JGeometry::TVec3<f32>&) const;
	JPABaseEmitter* emitAndScale(long, unsigned char,
	                             const JGeometry::TVec3<f32>*) const;
	JPABaseEmitter* emitAndRotateScale(long, unsigned char,
	                                   const JGeometry::TVec3<f32>*) const;
	JPABaseEmitter* emitAndSRT(long, unsigned char,
	                           const JGeometry::TVec3<f32>*,
	                           const JGeometry::TVec3<f32>&,
	                           const JGeometry::TVec3<f32>&);
	void emitColumnWater();
	void marioHipAttack() const;
	bool marioHeadAttack() const;
	bool marioIsOn() const;
	static bool marioIsOn(const TLiveActor*);
	void actorIsOn(TLiveActor*) const;
	void sendMsgToAll(u32);
	void sendMsg(u32, u32);
	void waterHitPlane(THitActor*);
	void getWaterPos(THitActor*);
	void getWaterSpeed(THitActor*);
	static void getWaterPlane(THitActor*);
	void getWaterID(THitActor*);
	f32 getDistance(const JGeometry::TVec3<f32>&) const;
	f32 getDistanceXZ(const JGeometry::TVec3<f32>&) const;
	f32 getRotYFromAxisZ(const JGeometry::TVec3<f32>&) const;
	f32 getRotYFromAxisX(const JGeometry::TVec3<f32>&) const;
	void makeVecToLocalX(f32, JGeometry::TVec3<f32>*) const;
	void makeVecToLocalZ(f32, JGeometry::TVec3<f32>*) const;
	void getNormalVecFromTarget(f32, f32, f32, JGeometry::TVec3<f32>*) const;
	void getNormalVecFromOffset(f32, f32, f32, JGeometry::TVec3<f32>*);
	void getNormalVecFromTargetXZ(f32, f32, JGeometry::TVec3<f32>*) const;
	void getNormalVecFromOffsetXZ(f32, f32, JGeometry::TVec3<f32>*);
	void rotateVecByAxisY(JGeometry::TVec3<f32>*, f32);
	void getVerticalVecFromOffsetXZ(f32, f32, JGeometry::TVec3<f32>*);
	void getVerticalVecToTargetXZ(f32, f32, JGeometry::TVec3<f32>*) const;
	void calcReflectingVelocity(const TBGCheckData*, f32,
	                            JGeometry::TVec3<f32>*) const;
	void makeObjMtxRotByAxis(const JGeometry::TVec3<f32>&, f32, MtxPtr) const;
	void makeMtxRotByAxis(const JGeometry::TVec3<f32>&, f32, MtxPtr);
	static void concatOnlyRotFromRight(MtxPtr, MtxPtr, MtxPtr);
	static void concatOnlyRotFromLeft(MtxPtr, MtxPtr, MtxPtr);
	void setRootMtxTrans();
	void makeRootMtxTrans(MtxPtr);
	void updateRootMtxTrans();
	void setRootMtxRotX();
	void makeRootMtxRotX(MtxPtr);
	void setRootMtxRotY();
	void makeRootMtxRotY(MtxPtr);
	void setRootMtxRotZ();
	void makeRootMtxRotZ(MtxPtr);
	static void makeLowerStr(const char*, char*);
	static void moveJoint(J3DJoint*, f32, f32, f32);
	static f32 getJointTransX(J3DJoint*);
	static f32 getJointTransY(J3DJoint*);
	static f32 getJointTransZ(J3DJoint*);
	static void setJointTrans(J3DJoint*, f32, f32, f32);
	static void setJointTransX(J3DJoint*, f32);
	static void setJointTransY(J3DJoint*, f32);
	static void setJointTransZ(J3DJoint*, f32);
	static f32 getJointRotateX(J3DJoint*);
	static f32 getJointRotateY(J3DJoint*);
	static f32 getJointRotateZ(J3DJoint*);
	void setJointRotate(J3DJoint*, short, short, short);
	void setJointRotateX(J3DJoint*, short);
	void setJointRotateY(J3DJoint*, short);
	void setJointRotateZ(J3DJoint*, short);
	void rotateJointX(J3DJoint*, f32);
	void rotateJointY(J3DJoint*, f32);
	void rotateJointZ(J3DJoint*, f32);
	static f32 getJointScaleX(J3DJoint*);
	static f32 getJointScaleY(J3DJoint*);
	static f32 getJointScaleZ(J3DJoint*);
	static void setJointScale(J3DJoint*, f32, f32, f32);
	static void setJointScaleX(J3DJoint*, f32);
	static void setJointScaleY(J3DJoint*, f32);
	static void setJointScaleZ(J3DJoint*, f32);
	void calcMap();
	void getMapModel();
	void getMapModelData();
	void getMapMActor();
	static TJointObj* getBuildingJointObj(int);
	static J3DJoint* getBuildingJoint(int);

	static TMapCollisionStatic* newAndInitBuildingCollisionStatic(int,
	                                                              TLiveActor*);
	static TMapCollisionMove* newAndInitBuildingCollisionMove(int, TLiveActor*);
	static TMapCollisionWarp* newAndInitBuildingCollisionWarp(int, TLiveActor*);

	static void joinToGroup(const char*, THitActor*);
	static void startAllAnim(MActor*, const char*);
	static void initPacketMatColor(J3DModel*, GXTevRegID, const GXColorS10*);
	static bool isFruit(THitActor*);
	static bool isCoin(THitActor*);
	static void throwObjFromPointWithRot(TMapObjBase*,
	                                     const JGeometry::TVec3<f32>&,
	                                     const JGeometry::TVec3<f32>&, f32,
	                                     f32);
	void throwObjToFrontFromPoint(TMapObjBase*, const JGeometry::TVec3<f32>&,
	                              f32, f32) const;
	void throwObjToFront(TMapObjBase*, f32, f32, f32) const;
	void throwObjToOverhead(TMapObjBase*, f32, f32) const;
	void checkOnManhole();
	static void loadHideObjInfo(JSUMemoryInputStream&, long*, f32*, f32*,
	                            long*);
	static bool isDemo();
	static bool isHideObj(THitActor*);
	void getObjCollisionHeightOffset() const { }

	// fabricated
	bool checkMapObjFlag(u32 flag) const { return unkF8 & flag; }
	void onMapObjFlag(u32 flag) { unkF8 |= flag; }
	void offMapObjFlag(u32 flag) { unkF8 &= ~flag; }
	TMapObjData* getMapObjData() { return unk130; }
	bool isState(u32 v) { return mState == v ? true : false; }
	bool isUnk104Positive() { return unk104 > 0 ? true : false; }
	int getUnk104() { return unk104; }
	u32 getUnk134() { return unk134; }
	void setUnk134(u32 v) { unk134 = v; }
	const char* getUnkF4() { return unkF4; }

public:
	/* 0xF4 */ const char* unkF4;
	/* 0xF8 */ u32 unkF8;
	/* 0xFC */ u16 mState;
	/* 0xFE */ u16 unkFE;
	/* 0x100 */ u16 unk100;
	/* 0x102 */ u16 unk102;
	/* 0x104 */ int unk104;
	/* 0x108 */ f32 unk108;
	/* 0x10C */ JGeometry::TVec3<f32> unk10C;
	/* 0x118 */ JGeometry::TVec3<f32> unk118;
	/* 0x124 */ JGeometry::TVec3<f32> unk124;
	/* 0x130 */ TMapObjData* unk130;
	/* 0x134 */ u32 unk134;
};

#endif
