#ifndef MOVE_BG_POOL_HPP
#define MOVE_BG_POOL_HPP

#include <MoveBG/MapObjBase.hpp>
#include <MoveBG/MapObjManager.hpp>

class TPool : public TMapObjBase {
public:
	virtual void load(JSUMemoryInputStream&);
	virtual void makeObjAppeared();
	virtual void draw() const;

	bool subWaterLevel();
	void makePoolUnlimited();

	TPool(const char*);

	// fabricated

	f32 getWaterLevel() const { return mPosition.y + unk13C; }

public:
	/* 0x138 */ u32 unk138;
	/* 0x13C */ f32 unk13C;
	/* 0x140 */ f32 unk140;
	/* 0x144 */ f32 unk144;
	/* 0x148 */ JUtility::TColor unk148;
};

class TPoolManager;

extern TPoolManager* gpPoolManager;

class TPoolManager : public TMapObjBaseManager {
public:
	TPoolManager(const char*);
	bool subWaterLevel(const TBGCheckData*);
	f32 getWaterLevel(const TBGCheckData*) const;
};

#endif
