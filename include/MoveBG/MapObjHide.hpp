#ifndef MOVE_BG_MAP_OBJ_HIDE_HPP
#define MOVE_BG_MAP_OBJ_HIDE_HPP

#include <MoveBG/MapObjBase.hpp>

class THideObjBase : public TMapObjBase {
public:
	THideObjBase(const char*);

	virtual void load(JSUMemoryInputStream&);
	virtual void loadAfter();
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void appearObj(float);
	virtual void appearObjFromPoint(const JGeometry::TVec3<float>&);
	virtual void emitEffect();

public:
	/* 0x138 */ TMapObjBase* unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ u32 unk144;
	/* 0x148 */ u32 unk148;
	/* 0x14C */ u8 unk14C;
};

class TBreakHideObj : public THideObjBase {
public:
	TBreakHideObj();
	virtual u32 receiveMessage(THitActor*, u32);
	virtual void control();
	virtual void kill();
	virtual void initMapObj();
};

#endif
