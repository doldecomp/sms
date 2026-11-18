#ifndef GC2D_SCRN_FADER_HPP
#define GC2D_SCRN_FADER_HPP

#include <GC2D/hx_wiper.h>
#include <JSystem/JUtility/JUTColor.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

class TSMSFader : public JDrama::TViewObj {
public:
	enum EFadeStatus {
		FADE_STATUS_FULLY_FADED_OUT = 0,
		FADE_STATUS_FULLY_FADED_IN  = 1,
		FADE_STATUS_FADING_IN       = 2,
		FADE_STATUS_FADING_OUT      = 3,
	};

	enum EWipeType {
		WIPE_DUMMY              = 0,
		WIPE_CIRCLE_IN          = 1,
		WIPE_CIRCLE_OUT         = 2,
		WIPE_CORNER_CIRCLES_IN  = 3,
		WIPE_CORNER_CIRCLES_OUT = 4,
		WIPE_TWISTING_TILES_IN  = 5,
		WIPE_TWISTING_TILES_OUT = 6,
		WIPE_SPIRAL_IN          = 7,
		WIPE_SPIRAL_OUT         = 8,
		WIPE_CURVED_SWEEP_IN    = 9,
		WIPE_CURVED_SWEEP_OUT   = 10,
		WIPE_DOOR_OUT           = 11,
		WIPE_LOGO               = 12,
		WIPE_GAME_OVER          = 13,
		WIPE_FADE_IN            = 14,
		WIPE_FADE_OUT           = 15,
		WIPE_BOX_OPEN           = 16,
		WIPE_BOX_CLOSE          = 17,
		WIPE_NONE               = 18,
	};

	struct WipeRequest {
		/* 0x0 */ u32 mWipeType;
		/* 0x4 */ f32 mDurationSeconds;
		/* 0x8 */ f32 mDelaySeconds;
	};

	TSMSFader(JUtility::TColor color, f32 frame_rate, const char* name);

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void update();
	virtual void draw(const JDrama::TRect&);

	void updateRequest();
	void updateDelay();
	void updateFadeinout();
	void drawWipe(const JDrama::TRect&);
	void drawFadeinout(const JDrama::TRect&);
	void setupGraphicsFadeinout();
	void requestWipe(WipeRequest* request);
	void startWipe(u32 wipe_type, f32 duration_seconds, f32 delay_seconds);
	void startFadein(int duration_frames);
	void startFadeinT(f32 duration_seconds);
	void startFadeout(int duration_frames);
	void startFadeoutT(f32 duration_seconds);
	void setColor(JUtility::TColor color);
	void setDisplaySize(int width, int height);
	void setFadeStatus(EFadeStatus);
	void getGameOverBtiResource();
	void getMmarkBtiResource();
	void getLogoBtiResource();

	bool isFullyFadedIn() const
	{
		return mFadeStatus == FADE_STATUS_FULLY_FADED_IN;
	}
	bool isFullyFadedOut() const
	{
		return mFadeStatus == FADE_STATUS_FULLY_FADED_OUT;
	}
	f32 getRate() const { return mFrameRate; }

public:
	/* 0x10 */ u16 mFadeDurationFrames;
	/* 0x12 */ u16 mFadeFrame;
	/* 0x14 */ f32 mFrameRate;
	/* 0x18 */ JUtility::TColor mFadeColor;
	/* 0x1C */ bool mUseBoxWipe;
	/* 0x20 */ TSMSFader::EFadeStatus mFadeStatus;
	/* 0x24 */ WipeRequest mPendingWipeRequest;
	/* 0x30 */ int mWipeType;
	/* 0x34 */ f32 unk34;
};

class TSmplFader : public TSMSFader {
public:
	TSmplFader(JUtility::TColor color = JUtility::TColor(0, 0, 0, 0),
	           f32 frame_rate = 60.0f, const char* name = "<ScrnFader>")
	    : TSMSFader(color, frame_rate, name)
	{
	}
};

class TShineFader : public TSmplFader {
public:
	TShineFader(JUtility::TColor color = JUtility::TColor(0, 0, 0, 0),
	            f32 frame_rate = 60.0f, const char* name = "<ShineFader>")
	    : TSmplFader(color, frame_rate, name)
	    , mFadeDelayFrames(0)
	{
		mFadeStatus = TSMSFader::FADE_STATUS_FULLY_FADED_IN;
	}

	virtual void load(JSUMemoryInputStream& stream);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	virtual void update();

	bool registFadeout(u16 duration_frames, u16 delay_frames);

	/* 0x38 */ u16 mFadeDelayFrames;
};

#endif
