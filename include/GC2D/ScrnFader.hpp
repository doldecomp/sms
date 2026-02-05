#ifndef GC2D_SCRN_FADER_HPP
#define GC2D_SCRN_FADER_HPP

#include <GC2D/hx_wiper.h>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class TSMSFader : public JDrama::TViewObj {
public:
	enum EFadeStatus {
		FADE_STATUS_UNK0 = 0,
		FADE_STATUS_UNK1 = 1,
		FADE_STATUS_UNK2 = 2,
		FADE_STATUS_UNK3 = 3,
	};

	// TODO: maybe this is from the hx_wipe.h header/libary type thing?
	enum {
		UNK30_UNK_12 = 12,
		UNK30_UNK_13 = 13,
		UNK30_UNK_14 = 14,
		UNK30_UNK_15 = 15,
		UNK30_UNK_16 = 16,
		UNK30_UNK_17 = 17,
		UNK30_UNK_18 = 18,
	};

	struct WipeRequest {
		/* 0x0 */ u32 unk0;
		/* 0x4 */ f32 unk4;
		/* 0x8 */ f32 unk8;
	};

	TSMSFader(JUtility::TColor, f32, const char* name);

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void update();
	virtual void draw(const JDrama::TRect&);

	void updateRequest();
	void updateDelay();
	void updateFadeinout();
	void drawWipe(const JDrama::TRect&);
	void drawFadeinout(const JDrama::TRect&);
	void setupGraphicsFadeinout();
	void requestWipe(WipeRequest*);
	void startWipe(u32, f32, f32);
	void startFadein(int);
	void startFadeinT(f32);
	void startFadeout(int);
	void startFadeoutT(f32);
	void setColor(JUtility::TColor);
	void setDisplaySize(int, int);
	void setFadeStatus(EFadeStatus);
	void getGameOverBtiResource();
	void getMmarkBtiResource();
	void getLogoBtiResource();

public:
	/* 0x10 */ u16 unk10;
	/* 0x12 */ u16 unk12;
	/* 0x14 */ f32 mRate;
	/* 0x18 */ JUtility::TColor mFadeColor;
	/* 0x1C */ bool unk1C;
	/* 0x20 */ TSMSFader::EFadeStatus mFadeStatus;
	/* 0x24 */ WipeRequest mWipeRequest;
	/* 0x30 */ int unk30;
	/* 0x34 */ f32 unk34;
};

class TSmplFader : public TSMSFader {
public:
	TSmplFader(f32 param_1, const char* name = "<ScrnFader>")
	    : TSMSFader(JUtility::TColor(0, 0, 0, 0), param_1, name)
	{
	}
};

class TShineFader : public TSmplFader {
public:
	TShineFader(f32 param_1, const char* name = "<ShineFader>")
	    : TSmplFader(param_1, name)
	{
	}

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	virtual void update();

	void registFadeout(u16, u16);
};

#endif
