#ifndef GC2D_OPTION_HPP
#define GC2D_OPTION_HPP

#include <dolphin/types.h>
#include <JSystem/J3D/J3DGraphAnimator/J3DAnimation.hpp>
#include <JSystem/JUtility/JUTRect.hpp>

class J2DPane;
class J2DScreen;
class J2DOrthoGraph;
class J2DSetScreen;
class TExPane;
class JAISound;

class TArrowControl {
public:
	void update();
	void updateAlpha();
	void updateScale();
	void calcMoveX(int) const;

public:
	/* 0x0 */ J2DPane* unk0;
	/* 0x4 */ JUTRect unk4;
	/* 0x14 */ u8 unk14;
	/* 0x18 */ u32 unk18;
};

class TBalloonControl {
public:
	TBalloonControl(int);

	void add(TExPane*);
	void setupAnm();
	void startAnm();
	void stopAnm();
	void update();

public:
	struct UnknownBalloonControlStruct {
		/* 0x0 */ TExPane* unk0;
		/* 0x4 */ u8 unk4;
	};

	/* 0x0 */ UnknownBalloonControlStruct* unk0;
	/* 0x4 */ int unk4;
	/* 0x8 */ int unk8;
	/* 0xC */ J3DFrameCtrl unkC;
	/* 0x20 */ u32 unk20;
};

class TPaneScalingControl {
public:
	TPaneScalingControl(J2DPane*);

	void setupAnm(f32, f32);
	void startAnm();
	void stopAnm();
	void update();

public:
	/* 0x0 */ J2DPane* unk0;
	/* 0x4 */ J3DFrameCtrl unk4;
	/* 0x18 */ JUTRect unk18;
	/* 0x28 */ f32 unk28;
};

class TPatternAnmControl {
public:
	struct TAnmChunk { };

	TPatternAnmControl(J2DScreen*);
	void set(const TAnmChunk*, int);
	void setupAnm();
	void update();
	void show();
	void hide();

public:
	/* 0x0 */ J2DScreen* unk0;
	/* 0x4 */ J3DFrameCtrl unk4;
	/* 0x18 */ TAnmChunk* unk18;
	/* 0x1C */ int unk1C;
	/* 0x20 */ TAnmChunk* unk20;
	/* 0x24 */ f32 unk24;
};

class TToggleControl {
public:
	TToggleControl(J2DScreen*);
	void setupToggle(const u32*, int);
	void toggle();
	void getNumber() const;
	void setNumber(int);
};

class TOptionRumbleUnit {
public:
	enum RumbleType { };
	enum State { };

	TOptionRumbleUnit(J2DScreen*);
	void update();
	void checkRumble();
	void toggle();
	void adjust();
	void adjustView();
	void show();
	void hide();
	void deactivate(bool);
	void activate();
	void setValue(RumbleType);
	void setState(State);
	void setInfluencedAlphaRecursive(J2DPane*, bool);

public:
	/* 0x0 */ J2DScreen* unk0;
	/* 0x4 */ TExPane* unk4;
	/* 0x8 */ u8 unk8;
	/* 0xC */ TBalloonControl* unkC;
	/* 0x10 */ TPaneScalingControl* unk10;
	/* 0x14 */ TPatternAnmControl* unk14;
	/* 0x18 */ TPatternAnmControl* unk18;
	/* 0x1C */ TToggleControl* unk1C;
	/* 0x20 */ State unk20;
	/* 0x24 */ bool unk24;
};

namespace { // huh? maybe TOptionSoundUnit was inside the cpp?
template <class T> class ArrayWrapper {
	ArrayWrapper() { }

	void begin() const { }
};
} // namespace

class TOptionSoundUnit {
public:
	enum SoundType { };
	enum State { };

	TOptionSoundUnit(J2DScreen*);
	void initMonoAnm();
	void initSteleoAnm();
	void initSurroundAnm();
	void update();
	void updatePatternAnm();
	void foreachPatternAnm(ArrayWrapper<TPatternAnmControl*>&,
	                       void (TPatternAnmControl::*)());
	void toggle();
	void adjust();
	void show();
	void hide();
	void deactivate(bool);
	void activate();
	void setValue(int);
	void getValue() const;
	void stopSound();
	void flagToType(int);
	void typeToFlag(SoundType);
	void setState(State);
	void adjustView();
	void adjustSound();
	void setInfluencedAlphaRecursive(J2DPane*, bool);

	int cSoundSettings;
	int cFlagInfos;

public:
	/* 0x0 */ J2DScreen* unk0;
	/* 0x4 */ TExPane* unk4;
	/* 0x8 */ u8 unk8;
	/* 0xC */ TBalloonControl* unkc;
	/* 0x10 */ TPaneScalingControl* unk10;
	/* 0x14 */ TToggleControl* unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ J3DFrameCtrl unk1c;
	/* 0x30 */ JAISound* unk30;
	/* 0x34 */ TPatternAnmControl* unk34[2];
	/* 0x3C */ TPatternAnmControl* unk3c[3];
	/* 0x48 */ TPatternAnmControl* unk48[5];
	/* 0x5C */ TPatternAnmControl** unk5c;
	/* 0x60 */ u32 unk60;
	/* 0x64 */ TPatternAnmControl** unk64;
	/* 0x68 */ u32 unk68;
	/* 0x6C */ TPatternAnmControl** unk6c;
	/* 0x70 */ u32 unk70;
};

class TOptionControl {
public:
	enum SelectType { };

	void load();
	void loadSetting();
	void movementCommon();
	void draw(J2DOrthoGraph*);
	void movementCard2Option();
	void movementOption();
	void movementOption2Card();
	void setType(TOptionControl::SelectType, bool);
	void toggleCurType();
	void checkInput();
	void writeValue();
	void isChangedSetting() const;
	void resetChangedSetting();

public:
	/* 0x0 */ J2DSetScreen* unk0;
	/* 0x4 */ TArrowControl* unk4;
	/* 0x8 */ TOptionRumbleUnit* unk8;
	/* 0xC */ TOptionSoundUnit* unkC;
	/* 0x10 */ SelectType unk10;
	/* 0x14 */ u8 unk14;
	/* 0x18 */ u32 unk18;
	/* 0x1C */ u32 unk1C;
};

#endif
