#ifndef JDR_EFB_CTRL_HPP
#define JDR_EFB_CTRL_HPP

#include "JSystem/ResTIMG.hpp"
#include <JSystem/JDrama/JDRFlag.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <JSystem/JDrama/JDRSize.hpp>
#include <dolphin/gx.h>

// EFB stands for "Embedded Frame Buffer" -- the on-chip GPU framebuffer.
// These classes provide something like a "render pass" interface to the ECB,
// allowing for setting up various EFB-related states and then finally copying
// it back to main memory as a framebuffer that can be presented (EfbCtrlDisp)
// or as a texture that can then be sampled (EfbCtrlTex).

namespace JDrama {

class TEfbCtrl : public TViewObj {
public:
	TEfbCtrl(const char* name, TFlagT<u16> flag)
	    : TViewObj(name)
	    , unk20(flag)
	{
	}

	virtual void perform(u32 cue, TGraphics* graphics);

	void setSrcRect(const TRect&);

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

	virtual void perform(u32 cue, TGraphics* graphics);

	void setSrcRect(const GXRenderModeObj&);
};

class TEfbCtrlTex : public TEfbCtrl {
public:
	TEfbCtrlTex(const char* = "<EfbCtrlTex>", TFlagT<u16> = 0);

	virtual ~TEfbCtrlTex() { }
	virtual void perform(u32 cue, TGraphics* graphics);

	void setTexAttb(const GXTexObj&);

	void setDstSize(const JDrama::TSize& size) { mSize = size; }
	void setImgPtr(void* ptr) { mImagePtr = ptr; }
	void setTexFmt(GXTexFmt fmt) { mTexFmt = fmt; }

public:
	/* 0x24 */ TSize mSize;
	/* 0x2C */ void* mImagePtr;
	/* 0x30 */ GXTexFmt mTexFmt;
	/* 0x34 */ GXFBClamp mFbClamp;
	/* 0x38 */ JUtility::TColor mClearColor;
	/* 0x3C */ u32 mClearZ;
	/* 0x40 */ u8 (*mSamplePattern)[2];
	/* 0x44 */ u8* mVFilter;
};

}; // namespace JDrama

#endif
