#ifndef MOVE_BG_MAP_OBJ_ITEM_2_HPP
#define MOVE_BG_MAP_OBJ_ITEM_2_HPP

#include <MoveBG/MapObjBase.hpp>

class TMushroom1up : public TMapObjBase {
public:
	TMushroom1up(int, const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32, JDrama::TGraphics*);
	virtual void control();
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ u8 unk138;
	/* 0x139 */ u8 unk139;
	/* 0x13A */ u8 unk13A;
	/* 0x13C */ int unk13C;
};

class TJumpBase : public TMapObjBase {
public:
	TJumpBase(const char*);

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void ensureTakeSituation();
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void control();
	virtual void initMapObj();
};

#endif
