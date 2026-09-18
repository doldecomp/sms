#ifndef MOVE_BG_MAP_OBJ_EX_HPP
#define MOVE_BG_MAP_OBJ_EX_HPP

#include <MoveBG/MapObjHide.hpp>

class TMapObjNail : public THideObjBase {
public:
	virtual BOOL receiveMessage(THitActor* sender, u32 message);

	TMapObjNail(const char* name = "くぎ");

	static f32 mDownHeight;

	// TODO: name inferred, not from the map: the counter is bumped once per
	// hip-drop and the nail stops sinking at 3, one mDownHeight step each
	// time. Reading it through the accessor is what gives receiveMessage its
	// frame (0x28; the raw member is 0x20).
	int getDownCount() const { return mDownCount; }

public:
	/* 0x150 */ s32 mDownCount;
};

class TJointCoin : public TMapObjBase {
public:
	virtual void loadAfter();
	virtual void control();
	virtual void initMapObj();

	virtual bool nameIsObj(const char*);
	virtual TMapObjBase* makeObjFromJointName(const char*, u16);
	virtual TMapObjBase* makeObj(const char*, u16);

	TJointCoin(const char* name = "ジョイントコイン");

	// TODO: names inferred, not from the map. control() needs both of these
	// reads to go through accessors for its frame (0x50; either one alone, or
	// neither, gives 0x48).
	int getObjNum() const { return unk13C; }
	u16 getJointIndex(int i) const { return unk144[i]; }

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

	TMapObjSteam(const char* name = "水蒸気")
	    : THideObjBase(name)
	{
	}
};

#endif
