#ifndef ENEMY_FRUITS_BOAT_HPP
#define ENEMY_FRUITS_BOAT_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>

class J3DFrameCtrl;
class J3DAnmTransformKey;

class TFruitsBoat : public TSpineEnemy {
public:
	TFruitsBoat(const char* name = "フルーツの船");

	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void moveObject();
	virtual void requestShadow();

	void rowToCurPathNode(f32);
	int getBoatType() const;
	void traceBckTrack();
	void setJumpReaction();
	int setBckTrack(const char*);

public:
	/* 0x150 */ s16 unk150;
	/* 0x154 */ f32 unk154;
	/* 0x158 */ f32 unk158;
	/* 0x15C */ J3DAnmTransformKey* unk15C;
	/* 0x160 */ J3DFrameCtrl* unk160;
	/* 0x164 */ JGeometry::TVec3<f32> unk164;
	/* 0x170 */ f32 unk170;
	/* 0x174 */ f32 unk174;
};

class TFruitsBoatParams : public TSpineEnemyParams {
public:
	TFruitsBoatParams(const char* path);

	/* 0xA8 */ TParamRT<f32> mSLMoveSpeed;
	/* 0xBC */ TParamRT<f32> mSLRotSpeed;
	/* 0xD0 */ TParamRT<f32> mSLBckMoveSpeed;
};

class TFruitsBoatManager : public TEnemyManager {
public:
	TFruitsBoatManager(int, const char* name = "フルーツの船マネージャー");

	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	/* 0x54 */ int unk54;
};

DECLARE_NERVE(TNerveFruitsBoatBckTrace, TLiveActor);
DECLARE_NERVE(TNerveFruitsBoatGraphWander, TLiveActor);

#endif
