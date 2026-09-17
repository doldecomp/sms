#ifndef ENEMY_FRUITSBOAT_HPP
#define ENEMY_FRUITSBOAT_HPP

#include <Enemy/Enemy.hpp>
#include <Enemy/EnemyManager.hpp>
#include <Strategic/Nerve.hpp>
#include <dolphin/mtx.h>

class J3DAnmTransform;
class J3DFrameCtrl;

// Names and defaults are the ones PARAM_INIT stringified into .rodata and the
// constants TFruitsBoatManager::load stores into each TParamRT.
class TFruitsBoatParams : public TSpineEnemyParams {
public:
	TFruitsBoatParams(const char* prm);

	f32 getSLMoveSpeed() const { return mSLMoveSpeed.get(); }
	f32 getSLRotSpeed() const { return mSLRotSpeed.get(); }
	f32 getSLBckMoveSpeed() const { return mSLBckMoveSpeed.get(); }

	/* 0xA8 */ TParamRT<f32> mSLMoveSpeed;
	/* 0xBC */ TParamRT<f32> mSLRotSpeed;
	/* 0xD0 */ TParamRT<f32> mSLBckMoveSpeed;
};

// The ship that ferries Mario around Ricco Harbour and Noki Bay. It rides the
// TMapObjWave surface (pitching with the wave height under its bow and stern)
// and rolls about a horizontal axis while Mario walks on its deck. It either
// follows a graph/spline rail or, when the named graph does not exist, replays
// a .bck track by hand (see setBckTrack()).
class TFruitsBoat : public TSpineEnemy {
public:
	enum {
		// Set while the boat must not row: either the current rail node says
		// "stop here" or the deck animation is being switched.
		LIVE_FLAG_UNK10000 = 0x10000,
	};

	TFruitsBoat(const char* name);

	virtual ~TFruitsBoat() { }
	virtual void load(JSUMemoryInputStream&);
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void moveObject();
	virtual void requestShadow();

	int setBckTrack(const char* name);
	void setJumpReaction();
	void traceBckTrack();
	int getBoatType() const;
	void rowToCurPathNode(f32 turn_speed);

	// fabricated
	TFruitsBoatParams* getSaveParams() const
	{
		return (TFruitsBoatParams*)getSaveParam();
	}

	/* 0x150 */ s16 mReversed;
	/* 0x154 */ f32 mShadowRadiusX;
	/* 0x158 */ f32 mShadowRadiusZ;
	/* 0x15C */ J3DAnmTransform* mBckTrack;
	/* 0x160 */ J3DFrameCtrl* mBckTrackCtrl;
	// Horizontal axis the hull rolls about, steered towards the direction
	// Mario is standing off-centre.
	/* 0x164 */ JGeometry::TVec3<f32> mRollAxis;
	/* 0x170 */ f32 mRollAngle;
	/* 0x174 */ f32 mRollSpeed;
};

class TFruitsBoatManager : public TEnemyManager {
public:
	TFruitsBoatManager(int boat_type, const char* name);

	virtual ~TFruitsBoatManager() { }
	virtual void load(JSUMemoryInputStream&);
	virtual void createModelData();
	virtual TSpineEnemy* createEnemyInstance();

	// fabricated
	int getBoatType() const { return mBoatType; }

	/* 0x54 */ int mBoatType;
};

class TLiveActor;

DECLARE_NERVE(TNerveFruitsBoatBckTrace, TLiveActor)
DECLARE_NERVE(TNerveFruitsBoatGraphWander, TLiveActor)

#endif
