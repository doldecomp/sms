#ifndef GC2D_SCRN_FADER_HPP
#define GC2D_SCRN_FADER_HPP

#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class TSMSFader : public JDrama::TViewObj {
public:
	enum EFadeStatus {};

	struct WipeRequest { };

	TSMSFader(JUtility::TColor, f32, const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void update();
	void updateRequest();
	void updateDelay();
	void updateFadeinout();
	void draw(const JDrama::TRect&);
	void drawWipe(const JDrama::TRect&);
	void drawFadeinout(const JDrama::TRect&);
	void setupGraphicsFadeinout();
	void requestWipe(TSMSFader::WipeRequest*);
	void startWipe(u32, f32, f32);
	void startFadein(int);
	void startFadeinT(f32);
	void startFadeout(int);
	void startFadeoutT(f32);
	void setColor(JUtility::TColor);
	void setDisplaySize(int, int);
	void setFadeStatus(TSMSFader::EFadeStatus);
	void getGameOverBtiResource();
	void getMmarkBtiResource();
	void getLogoBtiResource();

	~TSMSFader();

public:
	/* 0x10 */ u16 unk10;
	/* 0x12 */ u16 unk12;
	/* 0x14 */ f32 unk14;
	/* 0x18 */ JUtility::TColor unk18;
	/* 0x1C */ char unk1C[0x4];
	/* 0x20 */ u32 unk20;
	/* 0x24 */ u32 unk24;
	/* 0x28 */ char unk28[0x8];
	/* 0x30 */ u32 unk30;
	/* 0x34 */ f32 unk34;
};

#endif
