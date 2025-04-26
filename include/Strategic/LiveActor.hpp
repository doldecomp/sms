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
class TMapCollisionManager;

class TLiveActor : public TTakeActor {
public:
	virtual u32 receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual bool belongToGround() const;
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
	                                            f32, f32) const;
	void calcVelocityToJumpToXZ(const JGeometry::TVec3<f32>&, f32, f32) const;
	void perform(u32, JDrama::TGraphics*);
	void load(JSUMemoryInputStream&);
	void initLodAnm(const TLodAnmIndex*, int, f32);
	J3DModel* getModel() const;
	void calcRideMomentum();
	void calcRidePos();
	~TLiveActor();
	TLiveActor(const char*);

	MActor* getMActor() const { return unk74; }

	static f32 mVelocityMinY;

	// fabricated
	const TBGCheckData* getUnkC4() const { return unkC4; }
	// TODO: which one is real?
	bool checkLiveFlag(u32 flag) const { return mLiveFlag & flag; }
	bool checkLiveFlag2(u32 flag) const { return mLiveFlag & flag ? 1 : 0; }
	void offLiveFlag(u32 flag) { mLiveFlag &= ~flag; }
	void onLiveFlag(u32 flag) { mLiveFlag |= flag; }
	const TMActorKeeper* getActorKeeper() const { return unk78; }
	TMActorKeeper* getActorKeeper() { return unk78; }
	MActor* getUnk74() { return unk74; }
	TMapCollisionManager* getMapCollisionManager() { return unkEC; }

public:
	/* 0x70 */ TLiveManager* unk70;
	/* 0x74 */ MActor* unk74;
	/* 0x78 */ TMActorKeeper* unk78;
	/* 0x7C */ s16 unk7C;
	/* 0x80 */ MAnmSound* unk80;
	/* 0x84 */ const char* unk84;
	/* 0x88 */ void* unk88;
	/* 0x8C */ TSpineBase<TLiveActor>* unk8C;
	/* 0x90 */ void* unk90;
	/* 0x94 */ JGeometry::TVec3<f32> unk94;
	/* 0xA0 */ JGeometry::TVec3<f32> unkA0;
	/* 0xAC */ JGeometry::TVec3<f32> unkAC;
	/* 0xB8 */ float unkB8;
	/* 0xBC */ float unkBC;
	/* 0xC0 */ float unkC0;
	/* 0xC4 */ const TBGCheckData* unkC4;
	/* 0xC8 */ float unkC8;
	/* 0xCC */ float unkCC;
	/* 0xD0 */ TLodAnm* unkD0;
	/* 0xD4 */ TLiveActor* unkD4;
	/* 0xD8 */ Vec unkD8;
	/* 0xE4 */ float unkE4;
	/* 0xE8 */ u8 unkE8;
	/* 0xEC */ TMapCollisionManager* unkEC;
	/* 0xF0 */ u32 mLiveFlag;
};

#endif
