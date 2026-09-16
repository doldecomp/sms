#ifndef MOVE_BG_MAP_OBJ_RICCO_HPP
#define MOVE_BG_MAP_OBJ_RICCO_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjBlock.hpp>
#include <MoveBG/Item.hpp>

// TODO: mark virtual methods as such

class TCraneRotY : public TMapObjBase {
public:
	virtual ~TCraneRotY();
	void calc();
	void control();
	void load(JSUMemoryInputStream&);
	TCraneRotY(const char* name = "Ｙ軸回転クレーン");
};

class TCraneUpDown : public TMapObjBase {
public:
	virtual ~TCraneUpDown();
	void control();
	void initMapObj();
	TCraneUpDown(const char* name = "上下クレーン");
};

class TCraneCargo : public TLeanBlock {
public:
	virtual ~TCraneCargo();
	void control();
	void calc();
	TCraneCargo()
	    : TLeanBlock("クレーン積み荷")
	{
	}
};

class TRiccoWatermill : public TMapObjBase {
public:
	virtual ~TRiccoWatermill();
	u32 touchWater(THitActor*);
	void control();
	void calc();
	void loadAfter();
	TRiccoWatermill(const char* name = "リコ水車");
};

class TSurfGesoObj : public TItem {
public:
	virtual ~TSurfGesoObj();
	void initMapObj();
	TSurfGesoObj(const char* name = "イカサーフィン");
};

class TFruitSwitch : public TMapObjBase {
public:
	virtual ~TFruitSwitch();
	void pullUp();
	void pushDown();
	BOOL receiveMessage(THitActor* sender, u32 message);
	TFruitSwitch(const char* name = "フルーツスイッチ");
};

class TFruitLauncher : public TMapObjBase {
public:
	virtual ~TFruitLauncher();
	void appearFruit() const;
	void fireObj();
	void loadAfter();
	TFruitLauncher(const char* name = "フルーツ発射口");
};

#endif
