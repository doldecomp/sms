#ifndef JDR_LIGHTING_HPP
#define JDR_LIGHTING_HPP

#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JStage/JSGLight.hpp>
#include <JSystem/JDrama/JDRPlacement.hpp>
#include <JSystem/JStage/JSGAmbientLight.hpp>
#include <dolphin/gx/GXLighting.h>

namespace JDrama {

class TLightMap : public TViewObj {
public:
	class TLightInfo {
	public:
		TLightInfo()
		    : unk0(0)
		    , unk4(nullptr)
		{
		}

	public:
		u32 unk0;
		TViewObj* unk4;
	};

	TLightMap()
	    : TViewObj("<LightMap>")
	    , mLightInfoCount(0)
	    , mLightInfos(nullptr)
	{
	}

	virtual ~TLightMap() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, TGraphics*);

public:
	/* 0x10 */ s32 mLightInfoCount;
	/* 0x14 */ TLightInfo* mLightInfos;
}; // size 0x18

class TLight : public TPlacement, public JStage::TLight {
public:
	TLight(const char* name = "<Light>")
	    : TPlacement(name)
	{
		GXInitLightAttn(&unk24, 1.875f, 0.0f, 0.0f, 1.875f, 0.0f, 0.0f);
		GXInitLightColor(&unk24, JUtility::TColor(0xff, 0xff, 0xff, 0xff));
	}

	virtual ~TLight() { }

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, TGraphics*);

	void correct(TGraphics*) const;

	virtual JStage::TELight JSGGetLightType() const;
	virtual void JSGSetLightType(JStage::TELight);

	virtual void JSGGetPosition(Vec*) const;
	virtual void JSGSetPosition(const Vec&);

	virtual GXColor JSGGetColor() const;
	virtual void JSGSetColor(GXColor);

public:
	/* 0x24 */ GXLightObj unk24;
	/* 0x64 */ JStage::TELight mLightType;
};

class TIdxLight : public TLight {
public:
	TIdxLight()
	    : TLight("<IdxLight>")
	    , unk68(0)
	{
	}

	virtual ~TIdxLight() { }

public:
	/* 0x68 */ u32 unk68;
};

class TLightAry : public TViewObj {
public:
	TLightAry()
	    : TViewObj("<LightAry>")
	{
	}

	virtual ~TLightAry() { }
	virtual void load(JSUMemoryInputStream&);
	virtual TNameRef* searchF(u16, const char*);
	virtual void perform(u32, TGraphics*);

	void setLightNum(long);

public:
	/* 0x10 */ TIdxLight* mLights;
	/* 0x14 */ s32 mLightCount;
};

class TAmbColor : public TViewObj, public JStage::TAmbientLight {
public:
	TAmbColor()
	    : TViewObj("<AmbColor>")
	    , mColor(0x4C, 0x4C, 0x4C, 0xFF)
	{
	}

	virtual ~TAmbColor() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, TGraphics*);
	virtual GXColor JSGGetColor() const;
	virtual void JSGSetColor(GXColor color);

public:
	/* 0x14 */ JUtility::TColor mColor;
};

class TAmbAry : public TViewObj {
public:
	TAmbAry()
	    : TViewObj("<AmbAry>")
	{
	}

	virtual ~TAmbAry() { }
	virtual void load(JSUMemoryInputStream&);
	virtual TNameRef* searchF(u16, const char*);
	virtual void perform(u32, TGraphics*) { }

	void setAmbNum(long);

public:
	/* 0x10 */ TAmbColor* mAmbColors;
	/* 0x14 */ s32 mAmbColorCount;
};

} // namespace JDrama

#endif
