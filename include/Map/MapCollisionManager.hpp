#ifndef MAP_MAP_COLLISION_MANAGER_HPP
#define MAP_MAP_COLLISION_MANAGER_HPP

#include <dolphin/types.h>

class TLiveActor;
class TMapCollisionBase;

class TMapCollisionManager {
public:
	TMapCollisionManager(u16, const char*, const TLiveActor*);
	void init(const char*, u16, const char*);
	void createCollision(const char*, u8);
	void getFileName(const char*, char*);
	void changeCollision(u32);

	// fabricated
	TMapCollisionBase* getUnk8() { return unk8; }

public:
	/* 0x0 */ TMapCollisionBase** mEntries;
	/* 0x4 */ u16 mMaxEntries;
	/* 0x6 */ u16 mEntryNum;
	/* 0x8 */ TMapCollisionBase* unk8;
	/* 0xC */ const char* mFolder;
	/* 0x10 */ const TLiveActor* unk10;
	/* 0x14 */ u16 unk14;
};

#endif
