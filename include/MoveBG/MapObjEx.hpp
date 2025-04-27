#ifndef MOVE_BG_MAP_OBJ_EX_HPP
#define MOVE_BG_MAP_OBJ_EX_HPP

#include <MoveBG/MapObjHide.hpp>

// TODO: mark virtual methods as such

class TMapObjNail : public THideObjBase {
public:
	u32 receiveMessage(THitActor*, unsigned long);
	TMapObjNail(const char*);
};

class TJointCoin : public TMapObjBase {
public:
	void control();
	void makeObj(const char*, unsigned short);
	void makeObjFromJointName(const char*, unsigned short);
	void loadAfter();
	void nameIsObj(const char*);
	void initMapObj();
	TJointCoin(const char*);
};

class TMapObjSteam : public THideObjBase {
public:
	void control();
	void load(JSUMemoryInputStream&);
	TMapObjSteam();
};

#endif
