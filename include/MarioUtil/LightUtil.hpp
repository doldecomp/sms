#ifndef MARIO_UTIL_LIGHT_UTIL_HPP
#define MARIO_UTIL_LIGHT_UTIL_HPP

#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

class J3DDrawBuffer;

class TLightWithDBSet;
class TLightMario;

namespace JDrama {
class TAmbAry;
class TLightAry;
} // namespace JDrama

enum {
	LIGHT_TYPE_PLAYER    = 0,
	LIGHT_TYPE_OBJECT    = 1,
	LIGHT_TYPE_MAPOBJECT = 2,
	LIGHT_TYPE_INDIRECT  = 3,

	LIGHT_TYPE_COUNT,
	LIGHT_TYPE_FIRST = LIGHT_TYPE_PLAYER,
};

class TLightWithDBSetManager : public JDrama::TViewObj {
public:
	TLightWithDBSetManager(const char*);

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	void calcLightBorder();
	GXColor getEffectLightColor() const;
	void setEffectLight(const JDrama::TGraphics*, GXLightObj*);
	Vec* getLightPos() const;
	void makeDrawBuffer();
	void addChildGroupObj(JDrama::TViewObjPtrListT<JDrama::TViewObj>*);

	// fabricated
	TLightWithDBSet* getLightSet(int i) { return mLightSets[i]; }

	// TODO: GXColor vs JUtility::TColor?
	void setEffectLightColor(const GXColor& color)
	{
		mEffectLightColor.r = color.r;
		mEffectLightColor.g = color.g;
		mEffectLightColor.b = color.b;
		mEffectLightColor.a = color.a;

		unk54 = true;
	}

	// TODO: both seem to be real inlines... why?
	void setEffectLightPos(const JGeometry::TVec3<f32>& pos)
	{
		mEffectLightPos.set(pos);
		unk54 = true;
	}
	void setEffectLightPos2(f32 x, f32 y, f32 z)
	{
		mEffectLightPos.set(x, y, z);
	}

public:
	/* 0x10 */ TLightMario* mMarioLight;
	/* 0x14 */ TLightWithDBSet** mLightSets;
	/* 0x18 */ GXColor mEffectLightColor;
	/* 0x1C */ JGeometry::TVec3<f32> mEffectLightPos;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
	/* 0x30 */ f32 unk30;
	/* 0x34 */ f32 unk34;
	/* 0x38 */ f32 unk38;
	/* 0x3C */ f32 unk3C;
	/* 0x40 */ f32 unk40;
	/* 0x44 */ f32 unk44;
	/* 0x48 */ Vec unk48;
	/* 0x54 */ u8 unk54;
	/* 0x55 */ u8 unk55;
};

extern TLightWithDBSetManager* gpLightManager;

class TLightCommon : public JDrama::TViewObj {
public:
	TLightCommon(const char* = "<TLightCommon>");

	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual GXColor getLightColor(int) const;
	virtual GXColor getAmbColor(int) const;
	virtual Vec* getLightPosition(int);
	virtual void setLight(const JDrama::TGraphics*, int);

	static JDrama::TAmbAry* mAmbAry;
	static JDrama::TLightAry* mLightAry;
	static Vec* mLightPos;

public:
	/* 0x10 */ f32 mShininess;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ f32 unk18;
	/* 0x1C */ f32 unk1C;
	/* 0x20 */ int mAmbIndex;
	/* 0x24 */ int mLightIndex;
	/* 0x28 */ u8 unk28;
	/* 0x29 */ GXColor unk29[2];
	/* 0x31 */ GXColor unk31[4];
	/* 0x41 */ u8 unk41;
	/* 0x42 */ char pad42[2];
	/* 0x44 */ JGeometry::TVec3<f32> unk44[4];
};

class TLightDrawBuffer : public JDrama::TViewObj {
public:
	TLightDrawBuffer(int, u32, const char*);

	virtual ~TLightDrawBuffer() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void setLight(TLightCommon* light)
	{
		mLight = light;
		mLight->loadAfter();
	}

	JDrama::TDrawBufObj* getOpaDbo() { return mOpaDrawBufferObject; }
	JDrama::TDrawBufObj* getXluDbo() { return mXluDrawBufferObject; }

public:
	/* 0x10 */ TLightCommon* mLight;
	/* 0x14 */ JDrama::TDrawBufObj* mOpaDrawBufferObject;
	/* 0x18 */ JDrama::TDrawBufObj* mXluDrawBufferObject;
	/* 0x1C */ char unk1C[0x32];
	/* 0x4E */ char unk4E[0x32];
	/* 0x80 */ int unk80;
};

class TLightWithDBSet : public JDrama::TViewObj {
public:
	TLightWithDBSet(int, const char*);

	virtual ~TLightWithDBSet() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void makeDrawBuffer() = 0;

	int getAmbIndex(const char*);
	int getLightIndex(const char*);
	void getLightDrawBuffer(int);
	void getXluDrawBuffer(int);
	void getOpaDrawBuffer(int);
	void resetLightDrawBuffer();
	void changeLightDrawBuffer(int);
	void addChildGroupObj(JDrama::TViewObjPtrListT<JDrama::TViewObj>*);

	bool isEnabled() const { return unk20; }
	void enable() { unk20 = true; }

protected:
	/* 0x10 */ TLightDrawBuffer** unk10;
	/* 0x14 */ J3DDrawBuffer* unk14;
	/* 0x18 */ J3DDrawBuffer* unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ bool unk20;
};

class TIndirectLightWithDBSet : public TLightWithDBSet {
public:
	TIndirectLightWithDBSet();

	virtual ~TIndirectLightWithDBSet() { }
	virtual void makeDrawBuffer();
};

class TMapObjectLightWithDBSet : public TLightWithDBSet {
public:
	TMapObjectLightWithDBSet();

	virtual ~TMapObjectLightWithDBSet() { }
	virtual void makeDrawBuffer();
};

class TObjectLightWithDBSet : public TLightWithDBSet {
public:
	TObjectLightWithDBSet();

	virtual ~TObjectLightWithDBSet() { }
	virtual void makeDrawBuffer();
};

class TPlayerLightWithDBSet : public TLightWithDBSet {
public:
	TPlayerLightWithDBSet();

	virtual ~TPlayerLightWithDBSet() { }
	virtual void makeDrawBuffer();
};

class TLightMario : public TLightCommon {
public:
	TLightMario(const char* name = "<TLightMario>")
	    : TLightCommon(name)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual GXColor getLightColor(int) const;
	virtual GXColor getAmbColor(int) const;
	virtual void setLight(const JDrama::TGraphics*, int);
};

class TLightShadow : public TLightCommon {
public:
	TLightShadow(const char* name = "<TLightShadow>")
	    : TLightCommon(name)
	{
	}

	virtual ~TLightShadow() { }
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
};

#endif
