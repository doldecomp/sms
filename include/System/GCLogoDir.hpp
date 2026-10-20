#ifndef SYSTEM_GC_LOGO_DIR_HPP
#define SYSTEM_GC_LOGO_DIR_HPP

#include <JSystem/JDrama/JDRDirector.hpp>
#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/FlagManager.hpp>

namespace JDrama {
class TDisplay;
};

class TMarioGamePad;
class JUTTexture;
class TProgSelect;

#ifdef VERSION_GMSP01_NX
JUTRect toWideScreen(s32 x1, s32 y1, s32 x2, s32 y2);
#endif

class TNintendo2D : public JDrama::TViewObj {
public:
	TNintendo2D(JUTTexture* tex, const char* name = "Nintendo2D")
	    : JDrama::TViewObj(name)
	    , mLogoTex(tex)
#ifdef VERSION_GMSP01_NX
	    , mRect(toWideScreen(133, 170, 509, 274))
#else
	    , mRect(133, 170, 509, 274)
#endif
#if defined(VERSION_GMSJ01) || defined(VERSION_GMSJ01_01)
	    , mColor(0, 70, 255, 255)
#elif !defined(VERSION_GMSP01_NX)
	    , mColor(220, 0, 0, 255)
#endif
	{
#ifdef VERSION_GMSP01_NX
		if (TFlagManager::getInstance()->getFlag(MSF_LANGUAGE) == 5) {
			mColor = JUtility::TColor(0, 70, 255, 255);
		} else {
			mColor = JUtility::TColor(220, 0, 0, 255);
		}
#endif
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
