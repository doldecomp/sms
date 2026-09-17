#ifndef MOVE_BG_MAP_OBJ_RICCO_HPP
#define MOVE_BG_MAP_OBJ_RICCO_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/Item.hpp>

// TODO: mark virtual methods as such

class TCraneRotY : public TMapObjBase {
public:
	void calc();
	void control();
	void load(JSUMemoryInputStream&);
	TCraneRotY(const char* name = "Ｙ軸回転クレーン");

	static u32 mWaitTime;
};

class TCraneUpDown : public TMapObjBase {
public:
	void control();
	void initMapObj();
	TCraneUpDown(const char* name = "上下クレーン");

	static f32 mRotSpeed;
	static u32 mWaitTime;
};

class TCraneCargo : public TLeanBlock {
public:
	void control();
	void calc();
	TCraneCargo()
	    : TLeanBlock("クレーン積み荷")
	{
	}
};

class TRiccoWatermill : public TMapObjBase {
public:
	u32 touchWater(THitActor*);
	void control();
	void calc();
	void loadAfter();
	TRiccoWatermill(const char* name = "リコ水車");

	static f32 mRotAccel;
	static f32 mRotSpeedMaxUp;
	static f32 mRotSpeedMaxDown;
	static f32 mRotDown;
	static f32 mSubmarineMoveRate;
	static f32 mSubmarineMaxTransY;
	static f32 mSubmarineBottomTransY;
	static u32 mWaitTime;
};

class TSurfGesoObj : public TItem {
public:
	void initMapObj();
	TSurfGesoObj(const char* name = "イカサーフィン");
};

class TFruitSwitch : public TMapObjBase {
public:
	void pullUp();
	void pushDown();
	BOOL receiveMessage(THitActor* sender, u32 message);
	TFruitSwitch(const char* name = "フルーツスイッチ");
};

class TFruitLauncher : public TMapObjBase {
public:
	void appearFruit() const;
	void fireObj();
	void loadAfter();
	TFruitLauncher(const char* name = "フルーツ発射口");

	static f32 mObjSpeedXZ;
	static f32 mObjSpeedY;
	static u32 mFruitLiveTime;
};

#endif
