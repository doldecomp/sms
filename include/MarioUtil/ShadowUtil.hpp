#ifndef MARIO_UTIL_SHADOW_UTIL_HPP
#define MARIO_UTIL_SHADOW_UTIL_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class THitActor;
class J3DModel;

class TCircleShadowRequest {
public:
	TCircleShadowRequest()
	    : unkC(0.0f)
	    , unk10(0.0f)
	    , unk14(0.0f)
	    , unk18(0.0f)
	    , unk1C(0)
	    , unk1D(1)
	    , unk20(0.0f)
	{
		unk0.zero();
	}

public:
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC */ f32 unkC;
	/* 0x10 */ f32 unk10;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ u8 unk1C;
	/* 0x1D */ u8 unk1D;
	/* 0x20 */ f32 unk20;
};

class TMBindShadowBody {
public:
	TMBindShadowBody(THitActor*, J3DModel*, float);
	bool isUseThisJoint(int);
	bool isCircleJoint(int);
	bool isBodyJoint(int);
	void entryDrawShadow();
	void calc();
};

class TAlphaShadowQuad;

class TMBindShadowManager;

extern TMBindShadowManager* gpBindShadowManager;

class TMBindShadowManager : JDrama::TViewObj {
public:
	TMBindShadowManager(const char*);
	void load(JSUMemoryInputStream&);
	void reset();
	void initEntry(TMBindShadowBody*);
	void perform(unsigned long, JDrama::TGraphics*);
	void drawShadowVolume(bool, TAlphaShadowQuad*);
	void drawShadowGD(unsigned long, JDrama::TGraphics*);
	void drawShadow(unsigned long, JDrama::TGraphics*);
	void request(const TCircleShadowRequest&, unsigned long);
	void forceRequest(const TCircleShadowRequest&, unsigned long);
	void calcVtx();
	~TMBindShadowManager();
};

#endif
