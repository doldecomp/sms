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
	TMapCollisionManager(u16 max_entries, const char* folder,
	                     const TLiveActor* owner);
	void init(const char* col_file, u16 flags, const char* folder);
	void createCollision(const char*, u8);
	void getFileName(const char*, char*);
	void changeCollision(u32);

	// fabricated
	TMapCollisionBase* getActiveCollision() const { return mActiveEntry; }
	void clearOwnerActor() { mOwnerActor = nullptr; }
	void removeActiveCollision()
	{
		if (mActiveEntry)
			mActiveEntry->remove();
	}
	void moveActiveCollisionTrans(const JGeometry::TVec3<f32>& trans)
	{
		if (mActiveEntry)
			mActiveEntry->moveTrans(trans);
	}
	void moveActiveCollisionMtx(MtxPtr mtx)
	{
		if (mActiveEntry)
			mActiveEntry->moveMtx(mtx);
	}
	void moveActiveCollisionSRT(const JGeometry::TVec3<f32>& trans,
	                            const JGeometry::TVec3<f32>& rot,
	                            const JGeometry::TVec3<f32>& scale)
	{
		if (mActiveEntry)
			mActiveEntry->moveSRT(trans, rot, scale);
	}

	void setUpActiveCollisionTRS(const JGeometry::TVec3<f32>& trans,
	                             const JGeometry::TVec3<f32>& rot,
	                             const JGeometry::TVec3<f32>& scale)
	{
		Mtx mtx;
		MsMtxSetTRS(mtx, trans.x, trans.y, trans.z, rot.x, rot.y, rot.z,
		            scale.x, scale.y, scale.z);
		mActiveEntry->setUpMtx(mtx);
	}

private:
	/* 0x0 */ TMapCollisionBase** mEntries;
	/* 0x4 */ u16 mMaxEntries;
	/* 0x6 */ u16 mEntryNum;
	/* 0x8 */ TMapCollisionBase* mActiveEntry;
	/* 0xC */ const char* mFolder;
	/* 0x10 */ const TLiveActor* mOwnerActor;
	/* 0x14 */ u16 unk14;
};

#endif
