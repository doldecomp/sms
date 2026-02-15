#ifndef STRATEGIC_LIVE_ACTOR_HPP
#define STRATEGIC_LIVE_ACTOR_HPP

#include <Strategic/TakeActor.hpp>
#include <Strategic/LiveManager.hpp>
#include <Strategic/Nerve.hpp>

// TODO: where should this live?
struct TLodAnmIndex;
class MActor;
class TMActorKeeper;
class MAnmSound;
class JKRFileLoader;
class TBGCheckData;
class TLodAnm;
class J3DModel;
class TBinder;
class TMapCollisionManager;

enum TLiveFlagBits {
	LIVE_FLAG_DEAD        = 0x1,
	LIVE_FLAG_HIDDEN      = 0x2,
	LIVE_FLAG_CLIPPED_OUT = 0x4,
	LIVE_FLAG_UNK8        = 0x8,
	LIVE_FLAG_UNK10       = 0x10,
	LIVE_FLAG_UNK40       = 0x40,
	LIVE_FLAG_AIRBORNE    = 0x80,
	LIVE_FLAG_UNK100      = 0x100,
	LIVE_FLAG_UNK200      = 0x200,
	LIVE_FLAG_UNK400      = 0x400,
	LIVE_FLAG_UNK800      = 0x800,
	LIVE_FLAG_UNK1000     = 0x1000,
	LIVE_FLAG_UNK2000     = 0x2000,
	LIVE_FLAG_UNK4000     = 0x4000,
	LIVE_FLAG_UNK8000     = 0x8000,
	LIVE_FLAG_UNK10000    = 0x10000,
	LIVE_FLAG_UNK20000    = 0x20000,
	LIVE_FLAG_UNK40000    = 0x40000,
	LIVE_FLAG_UNK100000   = 0x100000,
};

class TLiveActor : public TTakeActor {
public:
	TLiveActor(const char* name = "活動オブジェクト基底型");

	virtual ~TLiveActor();
	virtual BOOL receiveMessage(THitActor* sender, u32 message);
	virtual MtxPtr getTakingMtx();
	virtual BOOL belongToGround() const;
	virtual Mtx* getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void control();
	virtual void bind();
	virtual void moveObject();
	virtual void requestShadow();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void performOnlyDraw(u32, JDrama::TGraphics*);
	virtual u32 getShadowType();
	virtual void kill();
	virtual f32 getGravityY() const;
	virtual BOOL hasMapCollision() const;
	virtual Vec getFocalPoint() const;
	virtual void updateAnmSound();
	virtual const char** getBasNameTable() const;

	void stopAnmSound();
	void setCurAnmSound();
	void setAnmSound(const char* path);
	void initAnmSound();
	int getJointTransByIndex(int, JGeometry::TVec3<f32>*) const;
	void getJointTransByName(const char*, JGeometry::TVec3<f32>*) const;
	JGeometry::TVec3<f32> calcVelocityToJumpToY(const JGeometry::TVec3<f32>&,
	                                            f32 speed, f32 gravity) const;
	void calcVelocityToJumpToXZ(const JGeometry::TVec3<f32>&, f32, f32) const;
	void perform(u32, JDrama::TGraphics*);
	void load(JSUMemoryInputStream&);
	void initLodAnm(const TLodAnmIndex*, int, f32);
	J3DModel* getModel() const;
	void calcRideMomentum();
	void calcRidePos();

	MActor* getMActor() const { return mMActor; }

	static f32 mVelocityMinY;

	// fabricated
	const TBGCheckData* getGroundPlane() const { return mGroundPlane; }
	f32 getGroundHeight() const { return mGroundHeight; }
	// TODO: which one is real?
	bool checkLiveFlag(u32 flag) const { return mLiveFlag & flag; }
	bool checkLiveFlag2(u32 flag) const { return mLiveFlag & flag ? 1 : 0; }
	bool isAirborne() const
	{
		return checkLiveFlag(LIVE_FLAG_AIRBORNE) ? 1 : 0;
	}
	void offLiveFlag(u32 flag) { mLiveFlag &= ~flag; }
	void onLiveFlag(u32 flag) { mLiveFlag |= flag; }
	const TMActorKeeper* getActorKeeper() const { return mMActorKeeper; }
	TMActorKeeper* getActorKeeper() { return mMActorKeeper; }
	TLiveManager* getManager() { return mManager; }
	s16 getInstanceIndex() const { return mInstanceIndex; }
	MAnmSound* getAnmSound() { return mAnmSound; }
	TMapCollisionManager* getMapCollisionManager()
	{
		return mMapCollisionManager;
	}
	void getNextFramePosition(JGeometry::TVec3<f32>& result)
	{
		result = mPosition;
		result.add(mLinearVelocity);
		// It's not clear why we copy mVelocity to a local variable first,
		// but it matches what TWireBinder::bind does.
		// We can revisit later if needed.
		JGeometry::TVec3<f32> velocity = mVelocity;
		result.add(velocity);
	}
	const JGeometry::TVec3<f32>& getVelocity() const { return mVelocity; }
	void setVelocity(const JGeometry::TVec3<f32>& v) { mVelocity = v; }
	void setLinearVelocity(const JGeometry::TVec3<f32>& v)
	{
		mLinearVelocity = v;
	}

public:
	/* 0x70 */ TLiveManager* mManager;
	/* 0x74 */ MActor* mMActor;
	/* 0x78 */ TMActorKeeper* mMActorKeeper;
	/* 0x7C */ s16 mInstanceIndex;
	/* 0x80 */ MAnmSound* mAnmSound;
	/* 0x84 */ const char* mAnmSoundPath;
	/* 0x88 */ TBinder* mBinder;
	/* 0x8C */ TSpineBase<TLiveActor>* mSpine;
	/* 0x90 */ void* unk90;
	// TODO: Analyze mLinearVelocity vs mVelocity some more
	// and decide on better names
	/* 0x94 */ JGeometry::TVec3<f32> mLinearVelocity;
	/* 0xA0 */ JGeometry::TVec3<f32> mAngularVelocity;
	/* 0xAC */ JGeometry::TVec3<f32> mVelocity;
	/* 0xB8 */ f32 mScaledBodyRadius;
	/* 0xBC */ f32 mBodyRadius;
	/* 0xC0 */ f32 mHeadHeight;
	/* 0xC4 */ const TBGCheckData* mGroundPlane;
	/* 0xC8 */ f32 mGroundHeight;
	/* 0xCC */ f32 mGravity;
	/* 0xD0 */ TLodAnm* unkD0;
	/* 0xD4 */ const TLiveActor* mGroundActor;
	/* 0xD8 */ JGeometry::TVec3<f32> mRidePos;
	/* 0xE4 */ f32 mGroundActorYaw;
	/* 0xE8 */ s8 unkE8; // riding mode?
	/* 0xEC */ TMapCollisionManager* mMapCollisionManager;
	/* 0xF0 */ u32 mLiveFlag; // LiveFlagBits
};

#endif
