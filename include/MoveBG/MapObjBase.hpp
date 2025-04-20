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
	virtual void getSDLModelFlag() const;
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
	virtual void getHitObjNumMax();
	virtual void getDepthAtFloating();

	void initAndRegister(const char*);
	void moveByBck();
	void startAnim(u16);
	void startBck(const char*);
	void startControlAnim(u16);
	void stopAnim();
	void animIsFinished() const;
	void hasAnim(u16) const;
	void hasModelOrAnimData(u16) const;
	void startSound(u16);
	void soundBas(u32, f32, f32);
	void setUpMapCollision(u16);
	void removeMapCollision();
	void sleep();
	void awake();
	void initActorData();
	void initModelData();
	void initMActor(const char*, const char*, u32);
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
	void marioHeadAttack() const;
	void marioIsOn() const;
	void marioIsOn(const TLiveActor*);
	void actorIsOn(TLiveActor*) const;
	void sendMsgToAll(u32);
	void sendMsg(u32, u32);
	void waterHitPlane(THitActor*);
	void getWaterPos(THitActor*);
	void getWaterSpeed(THitActor*);
	void getWaterPlane(THitActor*);
	void getWaterID(THitActor*);
	void getDistance(const JGeometry::TVec3<f32>&) const;
	void getDistanceXZ(const JGeometry::TVec3<f32>&) const;
	void getRotYFromAxisZ(const JGeometry::TVec3<f32>&) const;
	void getRotYFromAxisX(const JGeometry::TVec3<f32>&) const;
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
	void concatOnlyRotFromRight(MtxPtr, MtxPtr, MtxPtr);
	void concatOnlyRotFromLeft(MtxPtr, MtxPtr, MtxPtr);
	void setRootMtxTrans();
	void makeRootMtxTrans(MtxPtr);
	void updateRootMtxTrans();
	void setRootMtxRotX();
	void makeRootMtxRotX(MtxPtr);
	void setRootMtxRotY();
	void makeRootMtxRotY(MtxPtr);
	void setRootMtxRotZ();
	void makeRootMtxRotZ(MtxPtr);
	void makeLowerStr(const char*, char*);
	static void moveJoint(J3DJoint*, f32, f32, f32);
	static f32 getJointTransX(J3DJoint*);
	static f32 getJointTransY(J3DJoint*);
	static f32 getJointTransZ(J3DJoint*);
	void setJointTrans(J3DJoint*, f32, f32, f32);
	void setJointTransX(J3DJoint*, f32);
	static void setJointTransY(J3DJoint*, f32);
	void setJointTransZ(J3DJoint*, f32);
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
	void setJointScale(J3DJoint*, f32, f32, f32);
	void setJointScaleX(J3DJoint*, f32);
	void setJointScaleY(J3DJoint*, f32);
	void setJointScaleZ(J3DJoint*, f32);
	void calcMap();
	void getMapModel();
	void getMapModelData();
	void getMapMActor();
	TJointObj* getBuildingJointObj(int);
	J3DJoint* getBuildingJoint(int);

	static TMapCollisionStatic* newAndInitBuildingCollisionStatic(int,
	                                                              TLiveActor*);
	static TMapCollisionMove* newAndInitBuildingCollisionMove(int, TLiveActor*);
	static TMapCollisionWarp* newAndInitBuildingCollisionWarp(int, TLiveActor*);

	void joinToGroup(const char*, THitActor*);
	static void startAllAnim(MActor*, const char*);
	void initPacketMatColor(J3DModel*, _GXTevRegID, const _GXColorS10*);
	void isFruit(THitActor*);
	void isCoin(THitActor*);
	void throwObjFromPointWithRot(TMapObjBase*, const JGeometry::TVec3<f32>&,
	                              const JGeometry::TVec3<f32>&, f32, f32);
	void throwObjToFrontFromPoint(TMapObjBase*, const JGeometry::TVec3<f32>&,
	                              f32, f32) const;
	void throwObjToFront(TMapObjBase*, f32, f32, f32) const;
	void throwObjToOverhead(TMapObjBase*, f32, f32) const;
	void checkOnManhole();
	void loadHideObjInfo(JSUMemoryInputStream&, long*, f32*, f32*, long*);
	void isDemo();
	void isHideObj(THitActor*);
	void getObjCollisionHeightOffset() const;

public:
	/* 0xF4 */ const char* unkF4;
	/* 0xF8 */ u32 unkF8;
	/* 0xFC */ u16 unkFC;
	/* 0xFE */ u16 unkFE;
	/* 0x100 */ u16 unk100;
	/* 0x102 */ u16 unk102;
	/* 0x104 */ u32 unk104;
	/* 0x108 */ f32 unk108;
	/* 0x10C */ f32 unk10C;
	/* 0x110 */ f32 unk110;
	/* 0x114 */ f32 unk114;
	/* 0x118 */ f32 unk118;
	/* 0x11C */ f32 unk11C;
	/* 0x120 */ f32 unk120;
	/* 0x124 */ f32 unk124;
	/* 0x128 */ f32 unk128;
	/* 0x12C */ f32 unk12C;
	/* 0x130 */ void* unk130;
	/* 0x134 */ u32 unk134;
};

#endif
