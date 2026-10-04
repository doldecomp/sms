#ifndef JPA_DRAW_HPP
#define JPA_DRAW_HPP

#include <dolphin/mtx.h>
#include <JSystem/ResTIMG.hpp>
#include <dolphin/gx/GXEnum.h>
#include <JSystem/JParticle/JPADrawVisitor.hpp>
#include <JSystem/JParticle/JPADrawSetupTev.hpp>
#include <dolphin/gx/GXStruct.h>

class JPABaseEmitter;
class JPATextureResource;
class JPABaseParticle;

class JPADrawClipBoard {
public:
	JPADrawClipBoard() { }
	~JPADrawClipBoard() { }

public:
	typedef void (*DirTypeFunc)(JPABaseParticle*, JPABaseEmitter*,
	                            JGeometry::TVec3<f32>&);
	typedef void (*RotTypeFunc)(f32, f32, Mtx&);

	/* 0x0 */ JPADrawSetupTev mSetupTev;
	/* 0x4 */ JGeometry::TVec2<f32> unk4;
	/* 0xC */ JGeometry::TVec2<f32> unkC;
	/* 0x14 */ JGeometry::TVec2<f32> mTexCoords[4];
	/* 0x34 */ MtxPtr mViewMtx;
	/* 0x38 */ Mtx unk38;
	/* 0x68 */ Mtx unk68;
	/* 0x98 */ GXColor mPrmColor;
	/* 0x9C */ GXColor mEnvColor;
	/* 0xA0 */ DirTypeFunc mDirTypeFunc;
	/* 0xA4 */ RotTypeFunc mRotTypeFunc;
	/* 0xA8 */ f32 mScaleAnmTimer;
	/* 0xAC */ s16 mColorAnmFrame;
};

class JPADrawVisitorContainer {
public:
	JPADrawVisitorContainer() { }
	~JPADrawVisitorContainer() { }

public:
	// clang-format off
	/* 0x0 */ JPADrawExecGenPrjMtx mDrawExecGenPrjMtx;
	/* 0x4 */ JPADrawExecGenPrjTexMtx mDrawExecGenPrjTexMtx;
	/* 0x8 */ JPADrawExecGenTexMtx0 mDrawExecGenTexMtx0;
	/* 0xC */ JPADrawExecGenIdtMtx mDrawExecGenIdtMtx;
	/* 0x10 */ JPADrawExecSetTexMtx mDrawExecSetTexMtx;
	/* 0x18 */ JPADrawExecSetPointSize mDrawExecSetPointSize;
	/* 0x1C */ JPADrawExecSetLineWidth mDrawExecSetLineWidth;
	/* 0x20 */ JPADrawExecRegisterColorEmitterPE mDrawExecRegisterColorEmitterPE;
	/* 0x24 */ JPADrawExecRegisterColorEmitterP mDrawExecRegisterColorEmitterP;
	/* 0x28 */ JPADrawExecRegisterColorEmitterE mDrawExecRegisterColorEmitterE;
	/* 0x2C */ JPADrawExecRegisterColorChildPE mDrawExecRegisterColorChildPE;
	/* 0x30 */ JPADrawExecRegisterPrmColorAnm mDrawExecRegisterPrmColorAnm;
	/* 0x34 */ JPADrawExecRegisterPrmAlphaAnm mDrawExecRegisterPrmAlphaAnm;
	/* 0x38 */ JPADrawExecRegisterEnvColorAnm mDrawExecRegisterEnvColorAnm;
	/* 0x3C */ JPADrawExecRegisterPrmCEnv mDrawExecRegisterPrmCEnv;
	/* 0x40 */ JPADrawExecRegisterPrmAEnv mDrawExecRegisterPrmAEnv;
	/* 0x44 */ JPADrawExecLoadDefaultTexture mDrawExecLoadDefaultTexture;
	/* 0x48 */ JPADrawExecLoadTexture mDrawExecLoadTexture;
	/* 0x50 */ JPADrawExecLoadExTex mDrawExecLoadExTex;
	/* 0x54 */ JPADrawExecBillBoard mDrawExecBillBoard;
	/* 0x58 */ JPADrawExecRotBillBoard mDrawExecRotBillBoard;
	/* 0x5C */ JPADrawExecYBillBoard mDrawExecYBillBoard;
	/* 0x60 */ JPADrawExecRotYBillBoard mDrawExecRotYBillBoard;
	/* 0x64 */ JPADrawExecDirectional mDrawExecDirectional;
	/* 0x68 */ JPADrawExecRotDirectional mDrawExecRotDirectional;
	/* 0x6C */ JPADrawExecDirectionalCross mDrawExecDirectionalCross;
	/* 0x70 */ JPADrawExecRotDirectionalCross mDrawExecRotDirectionalCross;
	/* 0x74 */ JPADrawExecDirBillBoard mDrawExecDirBillBoard;
	/* 0x78 */ JPADrawExecRotation mDrawExecRotation;
	/* 0x7C */ JPADrawExecRotationCross mDrawExecRotationCross;
	/* 0x80 */ JPADrawExecPoint mDrawExecPoint;
	/* 0x84 */ JPADrawExecLine mDrawExecLine;
	/* 0x88 */ JPADrawExecStripe mDrawExecStripe;
	/* 0x8C */ JPADrawExecStripeCross mDrawExecStripeCross;
	/* 0x90 */ JPADrawExecCallBack mDrawExecCallBack;
	/* 0x98 */ JPADrawCalcScaleX mDrawCalcScaleX;
	/* 0x9C */ JPADrawCalcScaleXBySpeed mDrawCalcScaleXBySpeed;
	/* 0xA0 */ JPADrawCalcScaleY mDrawCalcScaleY;
	/* 0xA4 */ JPADrawCalcScaleYBySpeed mDrawCalcScaleYBySpeed;
	/* 0xA8 */ JPADrawCalcScaleCopyX2Y mDrawCalcScaleCopyX2Y;
	/* 0xAC */ JPADrawCalcScaleAnmTimingNormal mDrawCalcScaleAnmTimingNormal;
	/* 0xB0 */ JPADrawCalcScaleAnmTimingRepeatX mDrawCalcScaleAnmTimingRepeatX;
	/* 0xB4 */ JPADrawCalcScaleAnmTimingRepeatY mDrawCalcScaleAnmTimingRepeatY;
	/* 0xB8 */ JPADrawCalcScaleAnmTimingReverseX mDrawCalcScaleAnmTimingReverseX;
	/* 0xBC */ JPADrawCalcScaleAnmTimingReverseY mDrawCalcScaleAnmTimingReverseY;
	/* 0xC0 */ JPADrawCalcColorPrm mDrawCalcColorPrm;
	/* 0xC8 */ JPADrawCalcColorEnv mDrawCalcColorEnv;
	/* 0xD0 */ JPADrawCalcColorCopyFromEmitter mDrawCalcColorCopyFromEmitter;
	/* 0xD4 */ JPADrawCalcColorAnmFrameNormal mDrawCalcColorAnmFrameNormal;
	/* 0xDC */ JPADrawCalcColorAnmFrameRepeat mDrawCalcColorAnmFrameRepeat;
	/* 0xE4 */ JPADrawCalcColorAnmFrameReverse mDrawCalcColorAnmFrameReverse;
	/* 0xEC */ JPADrawCalcColorAnmFrameMerge mDrawCalcColorAnmFrameMerge;
	/* 0xF4 */ JPADrawCalcColorAnmFrameRandom mDrawCalcColorAnmFrameRandom;
	/* 0xFC */ JPADrawCalcAlpha mDrawCalcAlpha;
	/* 0x100 */ JPADrawCalcAlphaFlickNrmSin mDrawCalcAlphaFlickNrmSin;
	/* 0x104 */ JPADrawCalcAlphaFlickAddSin mDrawCalcAlphaFlickAddSin;
	/* 0x108 */ JPADrawCalcAlphaFlickMultSin mDrawCalcAlphaFlickMultSin;
	/* 0x10C */ JPADrawCalcTextureAnmIndexNormal mDrawCalcTextureAnmIndexNormal;
	/* 0x114 */ JPADrawCalcTextureAnmIndexRepeat mDrawCalcTextureAnmIndexRepeat;
	/* 0x11C */ JPADrawCalcTextureAnmIndexReverse mDrawCalcTextureAnmIndexReverse;
	/* 0x124 */ JPADrawCalcTextureAnmIndexMerge mDrawCalcTextureAnmIndexMerge;
	/* 0x12C */ JPADrawCalcTextureAnmIndexRandom mDrawCalcTextureAnmIndexRandom;
	/* 0x134 */ JPADrawCalcChildAlphaOut mDrawCalcChildAlphaOut;
	/* 0x138 */ JPADrawCalcChildScaleOut mDrawCalcChildScaleOut;
	// clang-format on
};

class JPADraw {
public:
	/* 0x00 */ JPADrawExecEmitterVisitor* mpExecEmtrVis[1];
	/* 0x04 */ JPADrawExecEmitterVisitor* mpExecEmtrPVis[5];
	/* 0x18 */ JPADrawExecEmitterVisitor* mpExecEmtrCVis[3];
	/* 0x24 */ JPADrawCalcEmitterVisitor* mpCalcEmtrVis[4];
	/* 0x34 */ JPADrawExecParticleVisitor* mpExecPtclVis[5];
	/* 0x48 */ JPADrawCalcParticleVisitor* mpCalcPtclVis[10];
	/* 0x70 */ JPADrawExecParticleVisitor* mpExecChldVis[4];
	/* 0x80 */ JPADrawCalcParticleVisitor* mpCalcChldVis[2];
	/* 0x88 */ u8 execEmtrVisNum;
	/* 0x89 */ u8 execEmtrPVisNum;
	/* 0x8A */ u8 execEmtrCVisNum;
	/* 0x8B */ u8 calcEmtrVisNum;
	/* 0x8C */ u8 execPtclVisNum;
	/* 0x8D */ u8 calcPtclVisNum;
	/* 0x8E */ u8 execChldVisNum;
	/* 0x8F */ u8 calcChldVisNum;
	/* 0x90 */ JPADrawContext mDrawCtx;
	/* 0xB4 */ f32 mScaleOut;
	/* 0xB8 */ GXColor mPrmColor;
	/* 0xBC */ GXColor mEnvColor;
	/* 0xC0 */ u16 mTexIdx;
	/* 0xC2 */ u8 unkC2;

public:
	struct JPADrawVisitorDefFlags {
		/* 0x00 */ BOOL mbIsEnableDrawParent;
		/* 0x04 */ BOOL mbHasPrmAnm;
		/* 0x08 */ BOOL mbHasEnvAnm;
		/* 0x0C */ BOOL mbIsStripe;
		/* 0x10 */ BOOL mbIsPointOrLine;
		/* 0x14 */ BOOL mbIsEnableAlpha;
	};

	static JPADrawVisitorContainer vc;
	static JPADrawClipBoard cb;

	void setKeyScl(f32 value) { mScaleOut = value; }

	BOOL initialize(JPABaseEmitter*, JPATextureResource*);
	void draw(MtxPtr view_mtx);
	void calc();
	void calcParticle(JPABaseParticle*);
	void calcChild(JPABaseParticle*);
	void initParticle(JPABaseParticle*);
	void initChild(JPABaseParticle*, JPABaseParticle*);
	const ResTIMG* swapImage(const ResTIMG*, s16);
	BOOL loadTexture(u8, GXTexMapID);
	void setDrawExecVisitorsBeforeCB(const JPADraw::JPADrawVisitorDefFlags&);
	void setDrawExecVisitorsAfterCB(const JPADraw::JPADrawVisitorDefFlags&);
	void setDrawCalcVisitors(const JPADraw::JPADrawVisitorDefFlags&);
	void setParticleClipBoard();
	void setChildClipBoard();
	void drawParticle();
	void drawChild();
	void zDraw();
	void zDrawParticle();
	void zDrawChild();
	s16 getMainTextureID(u8);
	s16 getIndTextureID();
	s16 getIndSubTextureID();
	s16 getSecondTextureID();
	void loadYBBMtx(MtxPtr);
};

#endif
