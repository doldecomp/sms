#ifndef JDR_EFB_CTRL_HPP
#define JDR_EFB_CTRL_HPP

#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <dolphin/gx.h>

namespace JDrama {

class TEfbCtrl : public TViewObj {
public:
	TEfbCtrl(const char* name, TFlagT<u16> flag)
	    : TViewObj(name)
	    , unk20(flag)
	{
	}

	virtual ~TEfbCtrl() { }
	virtual void perform(u32, TGraphics*);

	void setSrcRect(const TRect&);

	// TODO: these should be inlines on TFlagT I guess
	bool checkFlag8() const { return (unk20.mValue & 0x8) != 0; }
	bool checkFlag10() const { return (unk20.mValue & 0x10) != 0; }
	bool checkFlag20() const { return (unk20.mValue & 0x20) != 0; }
	bool checkFlag100() const { return !!(unk20.mValue & 0x100); }
	bool checkFlag200() const { return !!(unk20.mValue & 0x200); }
	bool checkFlag400() const { return !!(unk20.mValue & 0x400); }
	bool checkFlag800() const { return (unk20.mValue & 0x800) != 0; }

public:
	/* 0x10 */ TRect unk10;
	/* 0x20 */ TFlagT<u16> unk20;
};

class TEfbCtrlDisp : public TEfbCtrl {
public:
	TEfbCtrlDisp(const char* name = "<EfbCtrlDisp>", TFlagT<u16> flag = 0)
	    : TEfbCtrl(name, flag)
	{
	}

	virtual ~TEfbCtrlDisp() { }
	virtual void perform(u32, TGraphics*);

	void setSrcRect(const GXRenderModeObj&);
};

class TEfbCtrlTex : public TEfbCtrl {
public:
	TEfbCtrlTex(const char*, TFlagT<u16> = 0);

	virtual ~TEfbCtrlTex() { }
	virtual void perform(u32, TGraphics*);

	void setTexAttb(const GXTexObj&);

public:
	/* 0x24 */ u32 mWidth;
	/* 0x28 */ u32 mHeight;
	/* 0x2C */ void* mImagePtr;
	/* 0x30 */ GXTexFmt mTexFmt;
	/* 0x34 */ GXFBClamp mFbClamp;
	/* 0x38 */ JUtility::TColor unk38;
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ u8** unk40;
	/* 0x44 */ u8* unk44;
};

}; // namespace JDrama

#endif
