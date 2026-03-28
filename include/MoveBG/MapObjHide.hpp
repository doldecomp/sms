#ifndef MOVE_BG_MAP_OBJ_HIDE_HPP
#define MOVE_BG_MAP_OBJ_HIDE_HPP

#include <MoveBG/MapObjBase.hpp>

class THideObjBase : public TMapObjBase {
public:
	THideObjBase(const char* name = "隠しオブジェ");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual void appearObj(f32);
	virtual void appearObjFromPoint(const JGeometry::TVec3<f32>&);
	virtual void emitEffect();

public:
	/* 0x138 */ TMapObjBase* unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ char* unk144;
	/* 0x148 */ s32 unk148;
	/* 0x14C */ bool unk14C;
};

class THideObj : public THideObjBase {
public:
	THideObj(const char* name = "隠しオブジェ");

	virtual void touchPlayer(THitActor*);
};

class TWaterHitHideObj : public THideObjBase {
public:
	TWaterHitHideObj(const char* name = "水ヒットオブジェ");
	virtual void load(JSUMemoryInputStream&);
	virtual u32 touchWater(THitActor*);
};

class TFruitHitHideObj : public THideObjBase {
public:
	TFruitHitHideObj(const char* name = "水ヒットオブジェ");
	virtual void load(JSUMemoryInputStream&);
	virtual void touchActor(THitActor*);
	virtual void touchFruit(THitActor*);
};

class TFruitBasket : public TFruitHitHideObj {
public:
	TFruitBasket(const char* name = "バスケット");
	virtual void loadAfter();
	virtual void touchFruit(THitActor*);
	virtual void countFruit(THitActor*);

public:
	/* 0x150 */ u32 unk150;
};

class TWaterHitPictureHideObj : public THideObjBase {
public:
	TWaterHitPictureHideObj(const char* name = "オブジェ出現の絵");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void control();
	virtual u32 touchWater(THitActor*);
	virtual void touchActor(THitActor*);
	virtual const JGeometry::TVec3<f32>& getObjAppearPos() { return mPosition; }
	virtual void afterFinishedAnim();
	virtual void forward(f32);

public:
	/* 0x150 */ bool unk150; // animation direction
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ f32 unk15C;
	/* 0x160 */ f32 unk160;
	/* 0x164 */ f32 unk164;
	/* 0x168 */ f32 unk168;
	/* 0x16C */ GXColorS10 unk16C;
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

public:
};
#endif
