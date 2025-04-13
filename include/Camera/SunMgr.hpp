#ifndef CAMERA_SUN_MGR_HPP
#define CAMERA_SUN_MGR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/DrawSyncCallback.hpp>

class TSunMgr;

extern TSunMgr* gpSunMgr;

class TSunMgr : public JDrama::TViewObj, public TDrawSyncCallback {
public:
	TSunMgr(const char*);
	void load(JSUMemoryInputStream&);
	void perform(unsigned long, JDrama::TGraphics*);
	void getAddColor() const;
	void drawSyncCallback(unsigned short);
	~TSunMgr();

  // fabricated
	u8 getUnk1CAlpha() { return unk1C.a; }

public:
	/* 0x14 */ u8 unk14;
	/* 0x15 */ u8 unk15;
	/* 0x18 */ GXColor unk18;
	/* 0x1C */ GXColor unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ f32 unk24;
	/* 0x28 */ f32 unk28;
	/* 0x2C */ f32 unk2C;
};

#endif
