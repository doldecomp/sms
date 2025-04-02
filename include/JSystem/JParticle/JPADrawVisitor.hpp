#ifndef JPA_DRAW_VISITOR_HPP
#define JPA_DRAW_VISITOR_HPP

#include <JSystem/JGeometry.hpp>
#include <JSystem/JSupport/JSUList.hpp>

class JPADrawClipBoard;
class JPABaseEmitter;
class JPABaseShape;
class JPAExtraShape;
class JPASweepShape;
class JPAExTexShape;
class JPADraw;
class JPABaseParticle;
class JPABaseEmitter;
class JPATextureResource;

void dirTypeVel(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&);
void dirTypePos(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&);
void dirTypePosInv(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&);
void dirTypeEmtrDir(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&);
void dirTypePrevPtcl(JPABaseParticle*, JPABaseEmitter*, JGeometry::TVec3<f32>&);

void rotTypeY(f32, f32, Mtx&);
void rotTypeX(f32, f32, Mtx&);
void rotTypeZ(f32, f32, Mtx&);
void rotTypeXYZ(f32, f32, Mtx&);
void rotTypeYJiggle(f32, f32, Mtx&);

struct JPADrawContext {
	static JPADrawClipBoard* pcb;

	// fabricated
	u16 getTexIdx(u8 i) { return mTexIndices[i]; }

	JPABaseEmitter* mBaseEmitter;
	JPABaseShape* mBaseShape;
	JPAExtraShape* mExtraShape;
	JPASweepShape* mSweepShape;
	JPAExTexShape* mExTexShape;
	JPADraw* unk14;
	JSUList<JPABaseParticle>* unk18;
	JPATextureResource* mTexResource;
	u16* mTexIndices;
};

// ================ Exec callbacks ================

class JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecEmitterVisitor() { }
	virtual void exec(const JPADrawContext*) = 0;
};

class JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecParticleVisitor() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*) = 0;
};

class JPADrawExecCallBack : public JPADrawExecEmitterVisitor,
                            public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecCallBack() { }
	virtual void exec(const JPADrawContext*);
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecStripeCross : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecStripeCross() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecStripe : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecStripe() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecLine : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecLine() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecPoint : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecPoint() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRotationCross : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRotationCross() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRotation : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRotation() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecDirBillBoard : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecDirBillBoard() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRotDirectionalCross : public JPADrawExecParticleVisitor {
public:
	~JPADrawExecRotDirectionalCross() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecDirectionalCross : public JPADrawExecParticleVisitor {
public:
	~JPADrawExecDirectionalCross() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRotDirectional : public JPADrawExecParticleVisitor {
public:
	~JPADrawExecRotDirectional() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecDirectional : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecDirectional() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRotYBillBoard : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRotYBillBoard() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecYBillBoard : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecYBillBoard() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRotBillBoard : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRotBillBoard() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecBillBoard : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecBillBoard() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecLoadExTex : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecLoadExTex() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecLoadTexture : public JPADrawExecEmitterVisitor,
                               public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecLoadTexture() { }
	virtual void exec(const JPADrawContext*);
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecLoadDefaultTexture : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecLoadDefaultTexture() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecRegisterPrmAEnv : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRegisterPrmAEnv() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRegisterPrmCEnv : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRegisterPrmCEnv() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRegisterEnvColorAnm : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRegisterEnvColorAnm() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRegisterPrmAlphaAnm : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRegisterPrmAlphaAnm() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRegisterPrmColorAnm : public JPADrawExecParticleVisitor {
public:
	virtual ~JPADrawExecRegisterPrmColorAnm() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecRegisterColorChildPE : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecRegisterColorChildPE() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecRegisterColorEmitterE : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecRegisterColorEmitterE() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecRegisterColorEmitterP : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecRegisterColorEmitterP() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecRegisterColorEmitterPE : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecRegisterColorEmitterPE() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecSetLineWidth : public JPADrawExecParticleVisitor {
public:
	~JPADrawExecSetLineWidth() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecSetPointSize : public JPADrawExecParticleVisitor {
public:
	~JPADrawExecSetPointSize() { }
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecSetTexMtx : public JPADrawExecEmitterVisitor,
                             public JPADrawExecParticleVisitor {
public:
	~JPADrawExecSetTexMtx() { }
	virtual void exec(const JPADrawContext*);
	virtual void exec(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawExecGenIdtMtx : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecGenIdtMtx() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecGenTexMtx0 : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecGenTexMtx0() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecGenPrjTexMtx : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecGenPrjTexMtx() { }
	virtual void exec(const JPADrawContext*);
};

class JPADrawExecGenPrjMtx : public JPADrawExecEmitterVisitor {
public:
	virtual ~JPADrawExecGenPrjMtx() { }
	virtual void exec(const JPADrawContext*);
};

// ===================================================
// ================ Particle visitors ================
// ===================================================

class JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcParticleVisitor() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*) = 0;
};

class JPADrawCalcEmitterVisitor {
public:
	virtual ~JPADrawCalcEmitterVisitor() { }
	virtual void calc(const JPADrawContext*) = 0;
};

class JPADrawCalcChildScaleOut : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcChildScaleOut() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcChildAlphaOut : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcChildAlphaOut() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcTextureAnmIndexRandom : public JPADrawCalcEmitterVisitor,
                                         public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcTextureAnmIndexRandom() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcTextureAnmIndexMerge : public JPADrawCalcEmitterVisitor,
                                        public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcTextureAnmIndexMerge() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcTextureAnmIndexReverse : public JPADrawCalcEmitterVisitor,
                                          public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcTextureAnmIndexReverse() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcTextureAnmIndexRepeat : public JPADrawCalcEmitterVisitor,
                                         public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcTextureAnmIndexRepeat() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcTextureAnmIndexNormal : public JPADrawCalcEmitterVisitor,
                                         public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcTextureAnmIndexNormal() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcAlphaFlickMultSin : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcAlphaFlickMultSin() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcAlphaFlickAddSin : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcAlphaFlickAddSin() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcAlphaFlickNrmSin : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcAlphaFlickNrmSin() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcAlpha : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcAlpha() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorAnmFrameRandom : public JPADrawCalcEmitterVisitor,
                                       public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorAnmFrameRandom() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorAnmFrameMerge : public JPADrawCalcEmitterVisitor,
                                      public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorAnmFrameMerge() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorAnmFrameReverse : public JPADrawCalcEmitterVisitor,
                                        public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorAnmFrameReverse() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorAnmFrameRepeat : public JPADrawCalcEmitterVisitor,
                                       public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorAnmFrameRepeat() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorAnmFrameNormal : public JPADrawCalcEmitterVisitor,
                                       public JPADrawCalcParticleVisitor {
public:
	~JPADrawCalcColorAnmFrameNormal() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorCopyFromEmitter : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorCopyFromEmitter() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorEnv : public JPADrawCalcEmitterVisitor,
                            public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorEnv() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcColorPrm : public JPADrawCalcEmitterVisitor,
                            public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcColorPrm() { }
	virtual void calc(const JPADrawContext*);
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleAnmTimingReverseY : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleAnmTimingReverseY() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleAnmTimingReverseX : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleAnmTimingReverseX() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleAnmTimingRepeatY : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleAnmTimingRepeatY() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleAnmTimingRepeatX : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleAnmTimingRepeatX() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleAnmTimingNormal : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleAnmTimingNormal() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleCopyX2Y : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleCopyX2Y() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleYBySpeed : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleYBySpeed() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleY : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleY() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleXBySpeed : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleXBySpeed() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

class JPADrawCalcScaleX : public JPADrawCalcParticleVisitor {
public:
	virtual ~JPADrawCalcScaleX() { }
	virtual void calc(const JPADrawContext*, JPABaseParticle*);
};

#endif
