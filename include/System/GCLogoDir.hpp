#ifndef SYSTEM_GC_LOGO_DIR_HPP
#define SYSTEM_GC_LOGO_DIR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;
class JUTTexture;
class TProgSelect;

class TNintendo2D : public JDrama::TViewObj {
public:
	TNintendo2D(JUTTexture* tex, const char* name = "Nintendo2D")
	    : JDrama::TViewObj(name)
	    , mLogoTex(tex)
	    , mRect(133, 170, 509, 274)
	    , mColor(0, 70, 255, 255)
	{
	}

	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

public:
	/* 0x10 */ JUTTexture* mLogoTex;
	/* 0x14 */ JUTRect mRect;
	/* 0x24 */ JUtility::TColor mColor;
};

class TGCLogoDir : public JDrama::TDirector {
public:
	enum {
		STATE_WAIT_FADE_IN     = 0,
		STATE_SHOW_LOGO        = 1,
		STATE_FADE_OUT         = 2,
		STATE_ASK_PROGRESSIVE  = 3,
		STATE_SHOW_PROG_RESULT = 4,
	};

	enum {
		OVERALL_STATE_NLOGO = 0,
		OVERALL_STATE_DOLBY = 1,
	};

	TGCLogoDir();

	virtual ~TGCLogoDir();
	virtual int direct();

	void setup(JDrama::TDisplay*, TMarioGamePad*);
	bool direct_nlogo();
	bool direct_dolby();

public:
	/* 0x18 */ int mOverallState;
	/* 0x1C */ int mState;
	/* 0x20 */ TNintendo2D* mLogoView;
	/* 0x24 */ int mLogoShowTimer;
	/* 0x28 */ f32 mRefreshRate;
	/* 0x2C */ TMarioGamePad* mGamePad;
	/* 0x30 */ JUTTexture* mDolbyTexture;
	/* 0x34 */ JUTTexture* mNintendoTexture;
	/* 0x38 */ char unk38[0x4];
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ TProgSelect* mProgSelect;
	/* 0x44 */ s16 mBHoldTimer;
};

#endif
