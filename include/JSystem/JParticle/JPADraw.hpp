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
	~JPADrawClipBoard() { }

public:
	/* 0x0 */ JPADrawSetupTev unk0;
	/* 0x4 */ f32 unk4;
	/* 0x8 */ f32 unk8;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ JGeometry::TVec2<f32> unk14[4];
	/* 0x34 */ Mtx44* unk34;
	/* 0x38 */ Mtx unk38;
	/* 0x68 */ Mtx unk68;
	/* 0x98 */ u8 unk98;
	/* 0x99 */ u8 unk99;
	/* 0x9A */ u8 unk9A;
	/* 0x9B */ u8 unk9B;
	/* 0x9C */ u8 unk9C;
	/* 0x9D */ u8 unk9D;
	/* 0x9E */ u8 unk9E;
	/* 0x9F */ u8 unk9F;
	/* 0xA0 */ void (*unkA0)(JPABaseParticle*, JPABaseEmitter*,
	                         JGeometry::TVec3<f32>&);
	/* 0xA4 */ void (*unkA4)(f32, f32, Mtx&);
	/* 0xA8 */ f32 unkA8;
	/* 0xAC */ u16 unkAC;
};

class JPADrawVisitorContainer {
public:
	~JPADrawVisitorContainer() { }

public:
	/* 0x0 */ JPADrawExecGenPrjMtx unk0;
	/* 0x4 */ JPADrawExecGenPrjTexMtx unk4;
	/* 0x8 */ JPADrawExecGenTexMtx0 unk8;
	/* 0xC */ JPADrawExecGenIdtMtx unkC;
	/* 0x10 */ JPADrawExecSetTexMtx unk10;
	/* 0x18 */ JPADrawExecSetPointSize unk18;
	/* 0x1C */ JPADrawExecSetLineWidth unk1C;
	/* 0x20 */ JPADrawExecRegisterColorEmitterPE unk20;
	/* 0x24 */ JPADrawExecRegisterColorEmitterP unk24;
	/* 0x28 */ JPADrawExecRegisterColorEmitterE unk28;
	/* 0x2C */ JPADrawExecRegisterColorChildPE unk2C;
	/* 0x30 */ JPADrawExecRegisterPrmColorAnm unk30;
	/* 0x34 */ JPADrawExecRegisterPrmAlphaAnm unk34;
	/* 0x38 */ JPADrawExecRegisterEnvColorAnm unk38;
	/* 0x3C */ JPADrawExecRegisterPrmCEnv unk3C;
	/* 0x40 */ JPADrawExecRegisterPrmAEnv unk40;
	/* 0x44 */ JPADrawExecLoadDefaultTexture unk44;
	/* 0x48 */ JPADrawExecLoadTexture unk48;
	/* 0x50 */ JPADrawExecLoadExTex unk50;
	/* 0x54 */ JPADrawExecBillBoard unk54;
	/* 0x58 */ JPADrawExecRotBillBoard unk58;
	/* 0x5C */ JPADrawExecYBillBoard unk5C;
	/* 0x60 */ JPADrawExecRotYBillBoard unk60;
	/* 0x64 */ JPADrawExecDirectional unk64;
	/* 0x68 */ JPADrawExecRotDirectional unk68;
	/* 0x6C */ JPADrawExecDirectionalCross unk6C;
	/* 0x70 */ JPADrawExecRotDirectionalCross unk70;
	/* 0x74 */ JPADrawExecDirBillBoard unk74;
	/* 0x78 */ JPADrawExecRotation unk78;
	/* 0x7C */ JPADrawExecRotationCross unk7C;
	/* 0x80 */ JPADrawExecPoint unk80;
	/* 0x84 */ JPADrawExecLine unk84;
	/* 0x88 */ JPADrawExecStripe unk88;
	/* 0x8C */ JPADrawExecStripeCross unk8C;
	/* 0x90 */ JPADrawExecCallBack unk90;
	/* 0x98 */ JPADrawCalcScaleX unk98;
	/* 0x9C */ JPADrawCalcScaleXBySpeed unk9C;
	/* 0xA0 */ JPADrawCalcScaleY unkA0;
	/* 0xA4 */ JPADrawCalcScaleYBySpeed unkA4;
	/* 0xA8 */ JPADrawCalcScaleCopyX2Y unkA8;
	/* 0xAC */ JPADrawCalcScaleAnmTimingNormal unkAC;
	/* 0xB0 */ JPADrawCalcScaleAnmTimingRepeatX unkB0;
	/* 0xB4 */ JPADrawCalcScaleAnmTimingRepeatY unkB4;
	/* 0xB8 */ JPADrawCalcScaleAnmTimingReverseX unkB8;
	/* 0xBC */ JPADrawCalcScaleAnmTimingReverseY unkBC;
	/* 0xC0 */ JPADrawCalcColorPrm unkC0;
	/* 0xC8 */ JPADrawCalcColorEnv unkC8;
	/* 0xD0 */ JPADrawCalcColorCopyFromEmitter unkD0;
	/* 0xD4 */ JPADrawCalcColorAnmFrameNormal unkD4;
	/* 0xDC */ JPADrawCalcColorAnmFrameRepeat unkDC;
	/* 0xE4 */ JPADrawCalcColorAnmFrameReverse unkE4;
	/* 0xEC */ JPADrawCalcColorAnmFrameMerge unkEC;
	/* 0xF4 */ JPADrawCalcColorAnmFrameRandom unkF4;
	/* 0xFC */ JPADrawCalcAlpha unkFC;
	/* 0x100 */ JPADrawCalcAlphaFlickNrmSin unk100;
	/* 0x104 */ JPADrawCalcAlphaFlickAddSin unk104;
	/* 0x108 */ JPADrawCalcAlphaFlickMultSin unk108;
	/* 0x10C */ JPADrawCalcTextureAnmIndexNormal unk10C;
	/* 0x114 */ JPADrawCalcTextureAnmIndexRepeat unk114;
	/* 0x11C */ JPADrawCalcTextureAnmIndexReverse unk11C;
	/* 0x124 */ JPADrawCalcTextureAnmIndexMerge unk124;
	/* 0x12C */ JPADrawCalcTextureAnmIndexRandom unk12C;
	/* 0x134 */ JPADrawCalcChildAlphaOut unk134;
	/* 0x138 */ JPADrawCalcChildScaleOut unk138;
};

class JPADraw {
public:
	/* 0x00 */ JPADrawExecEmitterVisitor* unk0[1];
	/* 0x04 */ JPADrawExecEmitterVisitor* unk4[5];
	/* 0x18 */ JPADrawExecEmitterVisitor* unk18[3];
	/* 0x24 */ JPADrawCalcEmitterVisitor* unk24[4];
	/* 0x34 */ JPADrawExecParticleVisitor* unk34[5];
	/* 0x48 */ JPADrawCalcParticleVisitor* unk48[10];
	/* 0x70 */ JPADrawExecParticleVisitor* unk70[4];
	/* 0x80 */ JPADrawCalcParticleVisitor* unk80[2];
	/* 0x88 */ u8 unk88;
	/* 0x89 */ u8 unk89;
	/* 0x8A */ u8 unk8A;
	/* 0x8B */ u8 unk8B;
	/* 0x8C */ u8 unk8C;
	/* 0x8D */ u8 unk8D;
	/* 0x8E */ u8 unk8E;
	/* 0x8F */ u8 unk8F;
	/* 0x90 */ JPADrawContext unk90;
	/* 0xB4 */ f32 unkB4;
	/* 0xB8 */ GXColor unkB8;
	/* 0xBC */ GXColor unkBC;
	/* 0xC0 */ u16 unkC0;
	/* 0xC2 */ u8 unkC2;

public:
	struct JPADrawVisitorDefFlags {
		/* 0x00 */ BOOL unk0;
		/* 0x04 */ BOOL unk4;
		/* 0x08 */ BOOL unk8;
		/* 0x0C */ BOOL unkC;
		/* 0x10 */ BOOL unk10;
		/* 0x14 */ BOOL unk14;
	};

	static JPADrawVisitorContainer vc;
	static JPADrawClipBoard cb;

	BOOL initialize(JPABaseEmitter*, JPATextureResource*);
	void draw(MtxPtr);
	void calc();
	void calcParticle(JPABaseParticle*);
	void calcChild(JPABaseParticle*);
	void initParticle(JPABaseParticle*);
	void initChild(JPABaseParticle*, JPABaseParticle*);
	const ResTIMG* swapImage(const ResTIMG*, short);
	void loadTexture(u8, GXTexMapID);
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
	u32 getMainTextureID(u8);
	void getIndTextureID();
	void getIndSubTextureID();
	void getSecondTextureID();
	void loadYBBMtx(MtxPtr);
};

#endif
