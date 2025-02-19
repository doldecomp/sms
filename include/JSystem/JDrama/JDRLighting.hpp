#ifndef JDR_LIGHTING_HPP
#define JDR_LIGHTING_HPP

#include <JSystem/JStage/JSGLight.hpp>
#include <JSystem/JDrama/JDRPlacement.hpp>
#include <JSystem/JStage/JSGAmbientLight.hpp>

namespace JDrama {

class TLightMap : public TViewObj {
public:
	class TLightInfo {
	public:
		TLightInfo();
		~TLightInfo();

	public:
		u32 unk0;
		u32 unk4;
	};

	TLightMap()
	    : TViewObj("<LightMap>")
	    , unk10(0)
	    , unk14(nullptr)
	{
	}

	virtual ~TLightMap();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, TGraphics*);

public:
	/* 0x10 */ u32 unk10;
	/* 0x14 */ TLightInfo* unk14;
}; // size 0x18

class TLight : public TPlacement, public JStage::TLight {
public:
	TLight(const char* name = "<Light>")
	    : TPlacement(name)
	{
	}

	virtual ~TLight();

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void correct(JDrama::TGraphics*) const;

	virtual JStage::TELight JSGGetLightType() const;
	virtual void JSGSetLightType(JStage::TELight);

	virtual void JSGGetPosition(Vec*) const;
	virtual void JSGSetPosition(const Vec&);

	virtual GXColor JSGGetColor() const;
	virtual void JSGSetColor(GXColor);
};

class TIdxLight : public TLight {
public:
	TIdxLight()
	    : TLight("<IdxLight>")
	{
	}

	virtual ~TIdxLight();
};

class TLightAry : public TViewObj {
public:
	TLightAry()
	    : TViewObj("<LightAry>")
	{
	}

	virtual ~TLightAry();
	virtual void load(JSUMemoryInputStream&);
	virtual TNameRef* searchF(u16, const char*);
	virtual void setLightNum(long);
	virtual void perform(u32, JDrama::TGraphics*);
};

class TAmbColor : public TViewObj, public JStage::TAmbientLight {
public:
	TAmbColor()
	    : TViewObj("<AmbColor>")
	{
	}

	virtual ~TAmbColor();
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual GXColor JSGGetColor() const;
	virtual void JSGSetColor(GXColor);
};

class TAmbAry : public TViewObj {
public:
	TAmbAry()
	    : TViewObj("<AmbAry>")
	{
	}

	virtual ~TAmbAry();
	virtual void load(JSUMemoryInputStream&);
	virtual TNameRef* searchF(u16, const char*);
	virtual void perform(u32, JDrama::TGraphics*);

	void setAmbNum(long);
};

} // namespace JDrama

#endif
