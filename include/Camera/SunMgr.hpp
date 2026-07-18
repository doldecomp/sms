#ifndef CAMERA_SUN_MGR_HPP
#define CAMERA_SUN_MGR_HPP

#include <JSystem/JDrama/JDRViewObj.hpp>
#include <System/DrawSyncCallback.hpp>

class TSunMgr;

extern TSunMgr* gpSunMgr;

class TSunMgr : public JDrama::TViewObj, public TDrawSyncCallback {
public:
	TSunMgr(const char* name = "<TSunMgr>");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(unsigned long, JDrama::TGraphics*);
	virtual void drawSyncCallback(unsigned short);

	int getAddColor() const;

	// fabricated
	u8 getUnk1CAlpha() { return unk1C.a; }

	bool isThing() const { return unk15 & 2; }

public:
	/* 0x14 */ u8 unk14;
	/* 0x15 */ u8 unk15;
	/* 0x16 */ char unk16[2];
	/* 0x18 */ JUtility::TColor unk18;
	/* 0x1C */ JUtility::TColor unk1C;
	/* 0x20 */ f32 unk20;
	/* 0x24 */ JGeometry::TVec3<f32> unk24;
};

#endif
