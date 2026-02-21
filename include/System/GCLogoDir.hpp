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
	    , unk10(tex)
	    , unk14(133, 170, 509, 274)
	    , unk24(0, 70, 255, 255)
	{
	}

	virtual void perform(u32, JDrama::TGraphics*);

public:
	/* 0x10 */ JUTTexture* unk10;
	/* 0x14 */ JUTRect unk14;
	/* 0x24 */ JUtility::TColor unk24;
};

class TGCLogoDir : public JDrama::TDirector {
public:
	TGCLogoDir();

	virtual ~TGCLogoDir();
	virtual int direct();

	void setup(JDrama::TDisplay*, TMarioGamePad*);
	bool direct_nlogo();
	bool direct_dolby();

public:
	/* 0x18 */ int mOverallState;
	/* 0x1C */ int mState;
	/* 0x20 */ TNintendo2D* unk20;
	/* 0x24 */ int mLogoShowTimer;
	/* 0x28 */ f32 mRefreshRate;
	/* 0x2C */ TMarioGamePad* mGamePad;
	/* 0x30 */ JUTTexture* unk30;
	/* 0x34 */ JUTTexture* unk34;
	/* 0x38 */ char unk38[0x4];
	/* 0x3C */ u32 unk3C;
	/* 0x40 */ TProgSelect* mProgSelect;
	/* 0x44 */ s16 unk44;
};

#endif
