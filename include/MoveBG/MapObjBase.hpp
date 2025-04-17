#ifndef MOVE_BG_MAP_OBJ_BASE_HPP
#define MOVE_BG_MAP_OBJ_BASE_HPP

#include <Strategic/LiveActor.hpp>

class J3DJoint;
class J3DMaterialTable;
class TMapCollisionStatic;
class TMapCollisionMove;
class TMapCollisionWarp;

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
	virtual void getRadiusAtY(float) const;
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void control();
	virtual u32 getShadowType();
	virtual void kill();
	virtual void appear();
	virtual void makeObjAppeared();
	virtual void makeObjDead();
	virtual void changeObjSRT(const JGeometry::TVec3<float>&,
	                          const JGeometry::TVec3<float>&,
	                          const JGeometry::TVec3<float>&);
	virtual void changeObjMtx(float (*)[4]);
	virtual void updateObjMtx();
	virtual void setUpCurrentMapCollision();
	virtual void setObjHitData(u16);
	virtual void setModelMtx(float (*)[4]);
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
	virtual void touchWater(THitActor*);
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
	void soundBas(u32, float, float);
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
	void emitAndRotate(long, unsigned char,
	                   const JGeometry::TVec3<float>*) const;
	void emitAndScale(long, unsigned char, const JGeometry::TVec3<float>*,
	                  const JGeometry::TVec3<float>&) const;
	void emitAndBindScale(long, unsigned char, const JGeometry::TVec3<float>*,
	                      const JGeometry::TVec3<float>&) const;
	void emitAndScale(long, unsigned char,
	                  const JGeometry::TVec3<float>*) const;
	void emitAndRotateScale(long, unsigned char,
	                        const JGeometry::TVec3<float>*) const;
	void emitAndSRT(long, unsigned char, const JGeometry::TVec3<float>*,
	                const JGeometry::TVec3<float>&,
	                const JGeometry::TVec3<float>&);
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
	void getDistance(const JGeometry::TVec3<float>&) const;
	void getDistanceXZ(const JGeometry::TVec3<float>&) const;
	void getRotYFromAxisZ(const JGeometry::TVec3<float>&) const;
	void getRotYFromAxisX(const JGeometry::TVec3<float>&) const;
	void makeVecToLocalX(float, JGeometry::TVec3<float>*) const;
	void makeVecToLocalZ(float, JGeometry::TVec3<float>*) const;
	void getNormalVecFromTarget(float, float, float,
	                            JGeometry::TVec3<float>*) const;
	void getNormalVecFromOffset(float, float, float, JGeometry::TVec3<float>*);
	void getNormalVecFromTargetXZ(float, float, JGeometry::TVec3<float>*) const;
	void getNormalVecFromOffsetXZ(float, float, JGeometry::TVec3<float>*);
	void rotateVecByAxisY(JGeometry::TVec3<float>*, float);
	void getVerticalVecFromOffsetXZ(float, float, JGeometry::TVec3<float>*);
	void getVerticalVecToTargetXZ(float, float, JGeometry::TVec3<float>*) const;
	void calcReflectingVelocity(const TBGCheckData*, float,
	                            JGeometry::TVec3<float>*) const;
	void makeObjMtxRotByAxis(const JGeometry::TVec3<float>&, float,
	                         float (*)[4]) const;
	void makeMtxRotByAxis(const JGeometry::TVec3<float>&, float, float (*)[4]);
	void concatOnlyRotFromRight(float (*)[4], float (*)[4], float (*)[4]);
	void concatOnlyRotFromLeft(float (*)[4], float (*)[4], float (*)[4]);
	void setRootMtxTrans();
	void makeRootMtxTrans(float (*)[4]);
	void updateRootMtxTrans();
	void setRootMtxRotX();
	void makeRootMtxRotX(float (*)[4]);
	void setRootMtxRotY();
	void makeRootMtxRotY(float (*)[4]);
	void setRootMtxRotZ();
	void makeRootMtxRotZ(float (*)[4]);
	void makeLowerStr(const char*, char*);
	static void moveJoint(J3DJoint*, float, float, float);
	void getJointTransX(J3DJoint*);
	void getJointTransY(J3DJoint*);
	void getJointTransZ(J3DJoint*);
	void setJointTrans(J3DJoint*, float, float, float);
	void setJointTransX(J3DJoint*, float);
	static void setJointTransY(J3DJoint*, float);
	void setJointTransZ(J3DJoint*, float);
	void getJointRotateX(J3DJoint*);
	void getJointRotateY(J3DJoint*);
	void getJointRotateZ(J3DJoint*);
	void setJointRotate(J3DJoint*, short, short, short);
	void setJointRotateX(J3DJoint*, short);
	void setJointRotateY(J3DJoint*, short);
	void setJointRotateZ(J3DJoint*, short);
	void rotateJointX(J3DJoint*, float);
	void rotateJointY(J3DJoint*, float);
	void rotateJointZ(J3DJoint*, float);
	void getJointScaleX(J3DJoint*);
	void getJointScaleY(J3DJoint*);
	void getJointScaleZ(J3DJoint*);
	void setJointScale(J3DJoint*, float, float, float);
	void setJointScaleX(J3DJoint*, float);
	void setJointScaleY(J3DJoint*, float);
	void setJointScaleZ(J3DJoint*, float);
	void calcMap();
	void getMapModel();
	void getMapModelData();
	void getMapMActor();
	void getBuildingJointObj(int);
	void getBuildingJoint(int);

	static TMapCollisionStatic* newAndInitBuildingCollisionStatic(int,
	                                                              TLiveActor*);
	static TMapCollisionMove* newAndInitBuildingCollisionMove(int, TLiveActor*);
	static TMapCollisionWarp* newAndInitBuildingCollisionWarp(int, TLiveActor*);

	void joinToGroup(const char*, THitActor*);
	void startAllAnim(MActor*, const char*);
	void initPacketMatColor(J3DModel*, _GXTevRegID, const _GXColorS10*);
	void isFruit(THitActor*);
	void isCoin(THitActor*);
	void throwObjFromPointWithRot(TMapObjBase*, const JGeometry::TVec3<float>&,
	                              const JGeometry::TVec3<float>&, float, float);
	void throwObjToFrontFromPoint(TMapObjBase*, const JGeometry::TVec3<float>&,
	                              float, float) const;
	void throwObjToFront(TMapObjBase*, float, float, float) const;
	void throwObjToOverhead(TMapObjBase*, float, float) const;
	void checkOnManhole();
	void loadHideObjInfo(JSUMemoryInputStream&, long*, float*, float*, long*);
	void isDemo();
	void isHideObj(THitActor*);
	void getObjCollisionHeightOffset() const;
};

#endif
