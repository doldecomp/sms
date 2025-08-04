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

enum LiveFlagBits {
	LIVE_FLAG_AIRBORNE = 0x80,
};

class TLiveActor : public TTakeActor {
public:
	virtual BOOL receiveMessage(THitActor*, u32);
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
	void setAnmSound(const char*);
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
	~TLiveActor();
	TLiveActor(const char*);

	MActor* getMActor() const { return mMActor; }

	static f32 mVelocityMinY;

	// fabricated
	const TBGCheckData* getGroundPlane() const { return mGroundPlane; }
	// TODO: which one is real?
	bool checkLiveFlag(u32 flag) const { return mLiveFlag & flag; }
	bool checkLiveFlag2(u32 flag) const { return mLiveFlag & flag ? 1 : 0; }
	void offLiveFlag(u32 flag) { mLiveFlag &= ~flag; }
	void onLiveFlag(u32 flag) { mLiveFlag |= flag; }
	const TMActorKeeper* getActorKeeper() const { return mMActorKeeper; }
	TMActorKeeper* getActorKeeper() { return mMActorKeeper; }
	TLiveManager* getManager() { return mManager; }
	s16 getUnk7C() const { return unk7C; }
	MAnmSound* getUnk80() { return unk80; }
	TMapCollisionManager* getMapCollisionManager() { return unkEC; }
	const JGeometry::TVec3<f32>& getVelocity() const { return mVelocity; }
	void setVelocity(const JGeometry::TVec3<f32>& v) { mVelocity = v; }

public:
	/* 0x70 */ TLiveManager* mManager;
	/* 0x74 */ MActor* mMActor;
	/* 0x78 */ TMActorKeeper* mMActorKeeper;
	/* 0x7C */ s16 unk7C;
	/* 0x80 */ MAnmSound* unk80;
	/* 0x84 */ const char* unk84;
	/* 0x88 */ TBinder* unk88;
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
	/* 0xCC */ f32 unkCC;
	/* 0xD0 */ TLodAnm* unkD0;
	/* 0xD4 */ const TLiveActor* mGroundActor;
	/* 0xD8 */ JGeometry::TVec3<f32> mRidePos;
	/* 0xE4 */ f32 mGroundActorYaw;
	/* 0xE8 */ s8 unkE8;
	/* 0xEC */ TMapCollisionManager* unkEC;
	/* 0xF0 */ u32 mLiveFlag; // LiveFlagBits
};

#endif
