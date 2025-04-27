#ifndef MOVE_BG_MAP_OBJ_ITEM_2_HPP
#define MOVE_BG_MAP_OBJ_ITEM_2_HPP

#include <MoveBG/MapObjBase.hpp>

// TODO: mark virtual methods as such

class TMushroom1up : public TMapObjBase {
public:
	TMushroom1up(int, const char*);
	void touchPlayer(THitActor*);
	void makeObjAppeared();
	void initMapObj();
	void load(JSUMemoryInputStream&);
	void control();
	void perform(unsigned long, JDrama::TGraphics*);
};

class TJumpBase : public TMapObjBase {
public:
	TJumpBase(const char*);
	void initMapObj();
	void ensureTakeSituation();
	u32 receiveMessage(THitActor*, unsigned long);
	Mtx* getRootJointMtx() const;
	void calcRootMatrix();
	void control();
};

#endif
