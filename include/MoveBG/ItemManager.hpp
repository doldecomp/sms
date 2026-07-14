#ifndef MOVE_BG_ITEM_MANAGER_HPP
#define MOVE_BG_ITEM_MANAGER_HPP

#include <MoveBG/MapObjManager.hpp>

class TShine;
class TCoin;
class TCoinEmpty;
class TItemManager;

extern TItemManager* gpItemManager;

class TItemManager : public TMapObjBaseManager {
public:
	TItemManager(const char* name = "アイテムマネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);

	BOOL hasMapCollision() const { return false; }

	TCoin* newAndRegisterCoinReal();
	static TCoin* newAndRegisterCoin(u32);
	TShine* makeShineAppearWithDemoOffset(const char* shine_name,
	                                      const char* demo_name, f32 offset_x,
	                                      f32 offset_y, f32 offset_z);
	TShine* makeShineAppearWithDemo(const char* shine_name,
	                                const char* demo_name, f32 x, f32 y, f32 z);
	TShine* makeShineAppearWithTimeOffset(const char*, int, f32, f32, f32, int,
	                                      int, int);
	TShine* makeShineAppearWithTime(const char*, int, f32, f32, f32, int, int,
	                                int);
	void resetNozzleBoxesModel(int nozzle_type);

public:
	/* 0x40 */ TMtx34f unk40;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ f32 unk74;
	/* 0x78 */ TCoinEmpty* unk78;
};

#endif
