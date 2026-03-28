#ifndef PLAYER_SPLASH_MANAGER_HPP
#define PLAYER_SPLASH_MANAGER_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>

class TSplashManager;
class JUTTexture;
class TDLColorTexQuad;

extern TSplashManager* gpSplashManager;

struct TWaterSplash {
	/* 0x0 */ JGeometry::TVec3<f32> unk0;
	/* 0xC*/ f32 unkC;
	/* 0x10 */ u8 unk10;
	/* 0x11 */ u8 unk11;
};

class TSplashManager : public JDrama::TViewObj {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);

	void newSplash(JGeometry::TVec3<f32>, f32);
	void move();
	void makeDL(JDrama::TGraphics*) const;
	void draw() const;

public:
	/* 0x10 */ u16 unk10;
	/* 0x14 */ JUTTexture* unk14;
	/* 0x18 */ JSULink<TWaterSplash>* unk18[64];
	/* 0x118 */ JSUList<TWaterSplash> unk118;
	/* 0x124 */ JSUList<TWaterSplash> unk124;
	/* 0x130 */ TWaterSplash unk130[64];
	/* 0x630 */ f32 unk630;
	/* 0x634 */ f32 unk634;
	/* 0x638 */ f32 unk638;
	/* 0x63C */ GXColor unk63C;
	/* 0x640 */ TDLColorTexQuad* unk640;
	/* 0x644 */ f32 unk644;
	/* 0x648 */ u8 unk648;
};

#endif
