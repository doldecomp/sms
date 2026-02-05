#ifndef MARIO_UTIL_LIGHT_UTIL_HPP
#define MARIO_UTIL_LIGHT_UTIL_HPP

#include <JSystem/JDrama/JDRDrawBufObj.hpp>
#include <JSystem/JDrama/JDRViewObjPtrList.hpp>

class J3DDrawBuffer;

class TLightWithDBSet;
class TLightMario;

class TLightWithDBSetManager : public JDrama::TViewObj {
public:
	TLightWithDBSetManager(const char*);

	virtual ~TLightWithDBSetManager() { }
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void calcLightBorder();
	void getEffectLightColor() const;
	void setEffectLight(const JDrama::TGraphics*, GXLightObj*);
	void getLightPos() const;
	void makeDrawBuffer();
	void addChildGroupObj(
	    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);

	// fabricated
	TLightWithDBSet* getUnk14(int i) { return unk14[i]; }

public:
	/* 0x10 */ TLightMario* unk10;
	/* 0x14 */ TLightWithDBSet** unk14;
	/* 0x18 */ GXColor unk18;
	/* 0x1C */ u32 unk1C;
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ float unk28;
	/* 0x2C */ float unk2C;
	/* 0x30 */ float unk30;
	/* 0x34 */ float unk34;
	/* 0x38 */ float unk38;
	/* 0x3C */ float unk3C;
	/* 0x40 */ float unk40;
	/* 0x44 */ float unk44;
	/* 0x48 */ Vec unk48;
	/* 0x54 */ u8 unk54;
	/* 0x55 */ u8 unk55;
};

extern TLightWithDBSetManager* gpLightManager;

class TLightCommon : public JDrama::TViewObj {
public:
	TLightCommon(const char* = "<TLightCommon>");

	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void getLightColor(int) const;
	virtual void getAmbColor(int) const;
	virtual void getLightPosition(int);
	virtual void setLight(const JDrama::TGraphics*, int);
};

class TLightDrawBuffer : public JDrama::TViewObj {
public:
	TLightDrawBuffer(int, u32, const char*);

	virtual ~TLightDrawBuffer() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void setLight(TLightCommon*) { }

public:
	/* 0x10 */ TLightCommon* unk10;
	/* 0x14 */ JDrama::TDrawBufObj* unk14;
	/* 0x18 */ JDrama::TDrawBufObj* unk18;
	/* 0x1C */ char unk1C[0x64];
	/* 0x80 */ int unk80;
};

class TLightWithDBSet : public JDrama::TViewObj {
public:
	TLightWithDBSet(int, const char*);

	virtual ~TLightWithDBSet() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void makeDrawBuffer() = 0;

	void getAmbIndex(const char*);
	void getLightIndex(const char*);
	void getLightDrawBuffer(int);
	void getXluDrawBuffer(int);
	void getOpaDrawBuffer(int);
	void resetLightDrawBuffer();
	void changeLightDrawBuffer(int);
	void addChildGroupObj(
	    JDrama::TViewObjPtrListT<JDrama::TViewObj, JDrama::TViewObj>*);

public:
	/* 0x10 */ TLightDrawBuffer** unk10;
	/* 0x14 */ J3DDrawBuffer* unk14;
	/* 0x18 */ J3DDrawBuffer* unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ u8 unk20;
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

	virtual ~TLightMario() { }
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void setLight(const JDrama::TGraphics*, int);

	void getAmbColor(int) const;
	void getLightColor(int) const;
};

class TLightShadow : public TLightCommon {
public:
	TLightShadow(const char* name = "<TLightShadow>")
	    : TLightCommon(name)
	{
	}

	virtual ~TLightShadow() { }
	virtual void perform(u32, JDrama::TGraphics*);
};

#endif
