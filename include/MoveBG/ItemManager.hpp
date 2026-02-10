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
	TItemManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	BOOL hasMapCollision() const { return false; }

	TCoin* newAndRegisterCoinReal();
	static TCoin* newAndRegisterCoin(u32);
	TShine* makeShineAppearWithDemoOffset(const char*, const char*, f32, f32,
	                                      f32);
	TShine* makeShineAppearWithDemo(const char*, const char*, f32, f32, f32);
	TShine* makeShineAppearWithTimeOffset(const char*, int, f32, f32, f32, int,
	                                      int, int);
	TShine* makeShineAppearWithTime(const char*, int, f32, f32, f32, int, int,
	                                int);
	void resetNozzleBoxesModel(int);

public:
	/* 0x40 */ TMtx34f unk40;
	/* 0x70 */ f32 unk70;
	/* 0x74 */ f32 unk74;
	/* 0x78 */ TCoinEmpty* unk78;
};

#endif
