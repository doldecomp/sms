#ifndef MOVE_BG_ITEM_MANAGER_HPP
#define MOVE_BG_ITEM_MANAGER_HPP

#include <MoveBG/MapObjManager.hpp>

class TItemManager;

extern TItemManager* gpItemManager;

class TItemManager : public TMapObjBaseManager {
public:
	~TItemManager();
	void hasMapCollision() const;
	TItemManager(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual void perform(u32, JDrama::TGraphics*);

	void newAndRegisterCoinReal();
	void newAndRegisterCoin(u32);
	void makeShineAppearWithDemoOffset(const char*, const char*, f32, f32, f32);
	void makeShineAppearWithDemo(const char*, const char*, f32, f32, f32);
	void makeShineAppearWithTimeOffset(const char*, int, f32, f32, f32, int,
	                                   int, int);
	void makeShineAppearWithTime(const char*, int, f32, f32, f32, int, int,
	                             int);
	void resetNozzleBoxesModel(int);
};

#endif
