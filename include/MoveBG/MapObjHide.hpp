#ifndef MOVE_BG_MAP_OBJ_HIDE_HPP
#define MOVE_BG_MAP_OBJ_HIDE_HPP

#include <MoveBG/MapObjBase.hpp>

/**
 * @brief Base class for map objects that have something else hidden inside of
 * them, for a VERY broad definition of "hidden inside".
 */
class THideObjBase : public TMapObjBase {
public:
	THideObjBase(const char* name = "隠しオブジェ");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void appearObj(f32 y_offset);
	virtual void appearObjFromPoint(const JGeometry::TVec3<f32>& point);
	virtual void emitEffect();

public:
	/* 0x138 */ TMapObjBase* mHiddenObj;
	/* 0x13C */ f32 mAppearSpeed;
	/* 0x140 */ f32 mAppearYSpeed;
	/* 0x144 */ char* mHiddenShineDemoName;
	/* 0x148 */ s32 unk148;
	/* 0x14C */ bool mAllowReveal;
};

class THideObj : public THideObjBase {
public:
	THideObj(const char* name = "隠しオブジェ");

	virtual void touchPlayer(THitActor*);
};

/// Reveals the hidden object upon being sprayed by water
class TWaterHitHideObj : public THideObjBase {
public:
	TWaterHitHideObj(const char* name = "水ヒットオブジェ");
	virtual void load(JSUMemoryInputStream&);
	virtual u32 touchWater(THitActor*);
};

/// Reveals the hidden object upon touching a fruit
class TFruitHitHideObj : public THideObjBase {
public:
	TFruitHitHideObj(const char* name = "水ヒットオブジェ");
	virtual void load(JSUMemoryInputStream&);
	virtual void touchActor(THitActor*);
	virtual void touchFruit(THitActor*);
};

/// Reveals the hidden object upon having a fruit put inside of it
class TFruitBasket : public TFruitHitHideObj {
public:
	TFruitBasket(const char* name = "バスケット");
	virtual void loadAfter();
	virtual void touchFruit(THitActor*);
	virtual void countFruit(THitActor*);

public:
	/* 0x150 */ u32 unk150;
};

/// Special basket for events that counts the fruits put inside of it
class TFruitBasketEvent : public TFruitBasket {
public:
	TFruitBasketEvent(const char* name = "バスケット（イベント用）");
	virtual void countFruit(THitActor*);

	int getFruitNum(int param_1) const;
	void reset();

public:
	/* 0x154 */ u32 unk154[5];
};

/// Reveals the hidden object upon being hip-dropped by Mario
class THipDropHideObj : public THideObjBase {
public:
	THipDropHideObj(const char* name = "ヒップドロップオブジェ");
	virtual void touchPlayer(THitActor*);
};

/**
 * @brief A graffiti that reveals the object upon being sprayed by water for
 * some time.
 * @details Has customizable color. The graffiti becomes more or less (depending
 * on mReverse) transparent as it is being sprayed.
 */
class TWaterHitPictureHideObj : public THideObjBase {
public:
	TWaterHitPictureHideObj(const char* name = "オブジェ出現の絵");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void touchActor(THitActor*);
	virtual u32 touchWater(THitActor*);
	virtual const JGeometry::TVec3<f32>& getObjAppearPos() { return mPosition; }
	virtual void afterFinishedAnim();
	virtual void forward(f32);

public:
	enum {
		STATE_HIT_BY_BARREL = 2,
		STATE_FINISHED      = 3,
	};

	/* 0x150 */ bool mReverse;
	/* 0x154 */ f32 mSprayProgressSpeed;
	/* 0x158 */ f32 mBarrelProgressSpeed;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 mProgress;
	/* 0x164 */ f32 mMinProgress;
	/* 0x168 */ f32 mMaxProgress;
	/* 0x16C */ GXColorS10 mColor;
};

class THideObjPictureTwin : public TWaterHitPictureHideObj {
public:
	THideObjPictureTwin(const char* name = "ふたご落書き");

	virtual void loadAfter();
	virtual void initMapObj();

	virtual const JGeometry::TVec3<f32>& getObjAppearPos()
	{
		return unk174->mPosition;
	}
	virtual void afterFinishedAnim();

public:
	THideObjPictureTwin* unk174;
	char unk178[0x19];
};

class TBreakHideObj : public THideObjBase {
public:
	TBreakHideObj(const char* name = "壊れる隠しオブジェ")
	    : THideObjBase(name)
	{
	}

	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void control();
	virtual void kill();
	virtual void initMapObj();
};

class TWoodBox : public TBreakHideObj {
public:
	TWoodBox(const char* name = "木箱");

	virtual void loadAfter();
	virtual void kill();

	// Fabricated
	void fabricatedGroundKillCheck(f32, f32);
};

#endif
