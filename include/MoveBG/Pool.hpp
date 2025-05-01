#ifndef MOVE_BG_POOL_HPP
#define MOVE_BG_POOL_HPP

#include <MoveBG/MapObjBase.hpp>

class TPool : public TMapObjBase {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void makeObjAppeared();
	virtual void draw() const;

	void subWaterLevel();
	void makePoolUnlimited();

	TPool(const char*);
};

#endif
