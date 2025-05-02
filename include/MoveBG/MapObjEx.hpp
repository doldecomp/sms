#ifndef MOVE_BG_MAP_OBJ_EX_HPP
#define MOVE_BG_MAP_OBJ_EX_HPP

#include <MoveBG/MapObjHide.hpp>

class TMapObjNail : public THideObjBase {
public:
	virtual u32 receiveMessage(THitActor*, u32);

	TMapObjNail(const char*);

	static f32 mDownHeight;

public:
	/* 0x150 */ s32 unk150;
};

class TJointCoin : public TMapObjBase {
public:
	virtual void loadAfter();
	virtual void control();
	virtual void initMapObj();

	virtual bool nameIsObj(const char*);
	virtual TMapObjBase* makeObjFromJointName(const char*, u16);
	virtual TMapObjBase* makeObj(const char*, u16);

	TJointCoin(const char*);

public:
	/* 0x138 */ MActor* unk138;
	/* 0x13C */ s32 unk13C;
	/* 0x140 */ TMapObjBase** unk140;
	/* 0x144 */ u16* unk144;
};

class TMapObjSteam : public THideObjBase {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void control();

	TMapObjSteam();
};

#endif
