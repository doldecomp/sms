#ifndef MARIO_UTIL_SHADOW_UTIL_HPP
#define MARIO_UTIL_SHADOW_UTIL_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JGadget/std-list.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

class THitActor;
class J3DModel;
class J3DModelData;
class SDLModelData;
class TCircleShadowRequest;

class TCircleShadowRequest {
public:
	TCircleShadowRequest()
	    : unkC(0.0f)
	    , unk10(0.0f)
	    , unk14(0.0f)
	    , unk18(0.0f)
	    , unk1C(0)
	    , unk1D(1)
	    , unk20(0)
	{
		unk0.set(0.0f, 0.0f, 0.0f);
	}

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ u8 unk1C;
	/* 0x1D */ u8 unk1D;
	/* 0x20 */ u32 unk20;
};

class TAlphaShadowQuad {
public:
	TAlphaShadowQuad()
	    : unk0(0.01f)
	    , unk64(nullptr)
	    , unk68(nullptr)
	    , unk6C(nullptr)
	{
	}

	void reset();

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ Mtx unk4;
	/* 0x34 */ Vec unk34[4];
	/* 0x64 */ Vec* unk64;
	/* 0x68 */ TCircleShadowRequest* unk68;
	/* 0x6C */ TAlphaShadowQuad* unk6C;
};

class TAlphaShadowBlendQuad {
public:
	TAlphaShadowBlendQuad()
	{
		unk0.set(-1.0f, -1.0f, -1.0f);
		unkC.set(1.0f, 1.0f, 1.0f);
		unk18 = 0;
		unk1C = nullptr;
	}

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ JGeometry::TVec3<f32> unkC;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ TAlphaShadowBlendQuad* unk1C;
};

class TAlphaShadowQuadAry {
public:
	TAlphaShadowQuadAry()
	    : unk4(nullptr)
	    , unk8(nullptr)
	    , unkC(nullptr)
	    , unk10(nullptr)
	{
	}

public:
	/* 0x0 */ u32 unk0;
	/* 0x4 */ TAlphaShadowQuad* unk4;
	/* 0x8 */ TAlphaShadowQuad* unk8;
	/* 0xC */ TAlphaShadowBlendQuad* unkC;
	/* 0x10 */ TAlphaShadowBlendQuad* unk10;
};

class TSquareShadowInfo {
public:
	TSquareShadowInfo();

public:
	/* 0x0 */ Vec unk0[5];
};

class TModelShadowInfo {
public:
	TModelShadowInfo();

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ u8 unkC;
	/* 0xD */ u8 unkD;
	/* 0x10 */ f32 unk10;
};

class TModelShadow {
public:
	TModelShadow(SDLModelData*, void*, int);

	void update();
	void calc(int, JDrama::TGraphics*);
	void draw(int, JDrama::TGraphics*);

public:
	/* 0x0 */ SDLModelData* unk0;
	/* 0x4 */ void* unk4;
	/* 0x8 */ int unk8;
};

class TMBindShadowBody;

class TMBindShadowParts {
public:
	TMBindShadowParts(J3DModel*, u8, TMBindShadowBody*, f32);

	void calc(f32);

public:
	/* 0x0 */ f32 unk0;
	/* 0x4 */ TMBindShadowBody* unk4;
	/* 0x8 */ const char* unk8;
	/* 0xC */ MtxPtr unkC;
	/* 0x10 */ MtxPtr unk10;
	/* 0x14 */ bool unk14;
	/* 0x15 */ bool unk15;
	/* 0x16 */ bool unk16;
};

class TMBindShadowBody {
public:
	TMBindShadowBody(THitActor*, J3DModel*, f32);

	bool isUseThisJoint(int);
	bool isCircleJoint(int);
	bool isBodyJoint(int);
	void entryDrawShadow();
	void calc();

public:
	/* 0x0 */ TMBindShadowParts** unk0;
	/* 0x4 */ THitActor* unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ const char* unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
};

class TMBindShadowManager;

extern TMBindShadowManager* gpBindShadowManager;

class TMBindShadowManager : public JDrama::TViewObj {
public:
	TMBindShadowManager(const char* name = "<TMBindShadowManager>");

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void reset();
	void initEntry(TMBindShadowBody*);
	void drawShadowVolume(bool, TAlphaShadowQuad*);
	void drawShadowGD(u32, JDrama::TGraphics*);
	void drawShadow(u32, JDrama::TGraphics*);
	void request(const TCircleShadowRequest&, u32);
	void forceRequest(const TCircleShadowRequest&, u32);
	void calcVtx();

public:
	static f32 mJoinDist;
	static f32 mSquareShadowHeight;
	static f32 mTreeScale;
	static f32 mYScalePlus;
	static u8 mTestSw;
	static u8 mDLSw;

public:
	/* 0x10 */ TCircleShadowRequest* unk10;
	/* 0x14 */ int unk14;
	/* 0x18 */ TAlphaShadowQuad* unk18;
	/* 0x1C */ TAlphaShadowQuadAry* unk1C;
	/* 0x20 */ int unk20;
	/* 0x24 */ TAlphaShadowBlendQuad* unk24;
	/* 0x28 */ TSquareShadowInfo* unk28;
	/* 0x2C */ int unk2C;
	/* 0x30 */ JGeometry::TVec3<f32> unk30;
	/* 0x3C */ SDLModelData** unk3C;
	/* 0x40 */ u16 unk40;
	/* 0x44 */ u32 unk44;
	/* 0x48 */ u8 unk48;
	/* 0x49 */ u8 unk49;
	/* 0x4C */ JGadget::TList<TMBindShadowBody*> unk4C;
	/* 0x5C */ GXColor unk5C;
	/* 0x60 */ f32 unk60;
	/* 0x64 */ u8 unk64;
	/* 0x65 */ bool unk65;
	/* 0x68 */ f32 unk68;
	/* 0x6C */ f32 unk6C;
	/* 0x70 */ TModelShadowInfo* unk70;
};

#endif
