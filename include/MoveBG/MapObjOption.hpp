#ifndef MOVE_BG_MAP_OBJ_OPTION_HPP
#define MOVE_BG_MAP_OBJ_OPTION_HPP

#include <MoveBG/MapObjBase.hpp>

class TFileLoadBlock : public TMapObjBase {
public:
	virtual void loadAfter();
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

	void makeBlockNoCard();
	void makeBlockNormal();
	void makeBlockRock();
	void pushed();
	TFileLoadBlock(const char*);

public:
	/* 0x138 */ u8 unk138;
	/* 0x13C */ TFileLoadBlock* unk13C;
	/* 0x140 */ TFileLoadBlock* unk140;
	/* 0x144 */ JGeometry::TVec3<f32> unk144;
};

class TMapObjOptionWall : public THitActor {
public:
	void onCollision();
	void offCollision();
	void init();
	TMapObjOptionWall(const char*);

public:
	/* 0x68 */ TMapCollisionWarp* unk68;
};

#endif
