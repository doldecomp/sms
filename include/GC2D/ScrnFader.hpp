#ifndef GC2D_SCRN_FADER_HPP
#define GC2D_SCRN_FADER_HPP

#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JDrama/JDRGraphics.hpp>

class JSUMemoryInputStream;

class TSMSFader {
public:
	enum EFadeStatus {};

	struct WipeRequest { };

	TSMSFader(JUtility::TColor, f32, const char*);
	void perform(u32, JDrama::TGraphics*);
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
	void load(JSUMemoryInputStream&);
	void setDisplaySize(int, int);
	void setFadeStatus(TSMSFader::EFadeStatus);
	void getGameOverBtiResource();
	void getMmarkBtiResource();
	void getLogoBtiResource();
	~TSMSFader();
};

#endif
