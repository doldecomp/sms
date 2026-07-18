#ifndef MAP_MAP_COLLISION_MANAGER_HPP
#define MAP_MAP_COLLISION_MANAGER_HPP

#include <Map/MapCollisionEntry.hpp>
#include <JSystem/JGeometry/JGVec3.hpp>
#include <MarioUtil/MathUtil.hpp>
#include <dolphin/types.h>

class TLiveActor;
class TMapCollisionBase;

class TMapCollisionManager {
public:
	TMapCollisionManager(u16, const char*, const TLiveActor*);
	void init(const char* file, u16, const char* path);
	void createCollision(const char*, u8);
	void getFileName(const char*, char*);
	void changeCollision(u32);

	// fabricated
	TMapCollisionBase* getUnk8() { return unk8; }

	void setUpUnk8TRS(const JGeometry::TVec3<f32>& trans,
	                  const JGeometry::TVec3<f32>& rot,
	                  const JGeometry::TVec3<f32>& scale)
	{
		Mtx mtx;
		MsMtxSetTRS(mtx, trans.x, trans.y, trans.z, rot.x, rot.y, rot.z,
		            scale.x, scale.y, scale.z);
		unk8->setUpMtx(mtx);
	}

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
