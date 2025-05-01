#ifndef MOVE_BG_MAP_OBJ_TUMBLE_POLE_HPP
#define MOVE_BG_MAP_OBJ_TUMBLE_POLE_HPP

#include <MoveBG/MapObjBase.hpp>

class TMapObjTumblePole : public TMapObjBase {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void draw();

	void fallDown();
	void movement();
	void init(float, float, float);

	static JUtility::TColor mColorUpper;
	static JUtility::TColor mColorLower;

	TMapObjTumblePole(const char*);
};

class TMapObjTumblePoleManager : public JDrama::TViewObj {
public:
	virtual void loadAfter();

	void initDraw();

	TMapObjTumblePoleManager(const char*);
};

#endif
