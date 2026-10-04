#ifndef MOVE_BG_MAP_OBJ_ITEM_2_HPP
#define MOVE_BG_MAP_OBJ_ITEM_2_HPP

#include <MoveBG/MapObjBase.hpp>

class TMushroom1up : public TMapObjBase {
public:
	TMushroom1up(int unk, const char* name = "１ＵＰキノコ");

	virtual void load(JSUMemoryInputStream&);
	virtual void perform(u32 cue, JDrama::TGraphics* graphics);
	virtual void control();
	virtual void makeObjAppeared();
	virtual void initMapObj();
	virtual void touchPlayer(THitActor*);

public:
	/* 0x138 */ s8 unk138;
	/* 0x139 */ s8 unk139;
	/* 0x13A */ s8 unk13A;
	/* 0x13C */ int unk13C;
};

class TJumpBase : public TMapObjBase {
public:
	TJumpBase(const char* name = "移動ジャンプ台");

	virtual BOOL receiveMessage(THitActor*, u32);
	virtual void ensureTakeSituation();
	virtual Mtx* getRootJointMtx() const;
	virtual void calcRootMatrix();
	virtual void control();
	virtual void initMapObj();

public:
	/* 0x138 */ s8 unk138;
	/* 0x13C */ u32 unk13C;
};

#endif
