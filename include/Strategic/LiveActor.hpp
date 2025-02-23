#ifndef STRATEGIC_LIVE_ACTOR_HPP
#define STRATEGIC_LIVE_ACTOR_HPP

#include <Strategic/TakeActor.hpp>
#include <Strategic/LiveManager.hpp>

// TODO: where should this live?
struct TLodAnmIndex;
class MActor;
class TMActorKeeper;
class MAnmSound;
class JKRFileLoader;
class TBGCheckData;
class TLodAnm;

class TLiveActor : public TTakeActor {
public:
	virtual void receiveMessage(THitActor*, u32);
	virtual MtxPtr getTakingMtx();
	virtual void belongToGround() const;
	virtual void getRootJointMtx() const;
	virtual void init(TLiveManager*);
	virtual void calcRootMatrix();
	virtual void setGroundCollision();
	virtual void control();
	virtual void bind();
	virtual void moveObject();
	virtual void requestShadow();
	virtual void drawObject(JDrama::TGraphics*);
	virtual void performOnlyDraw(u32, JDrama::TGraphics*);
	virtual void getShadowType();
	virtual void kill();
	virtual void getGravityY() const;
	virtual void hasMapCollision() const;
	virtual void getFocalPoint() const;
	virtual void updateAnmSound();
	virtual void getBasNameTable() const;

	void stopAnmSound();
	void setCurAnmSound();
	void setAnmSound(const char*);
	void initAnmSound();
	void getJointTransByIndex(int, JGeometry::TVec3<f32>*) const;
	void getJointTransByName(const char*, JGeometry::TVec3<f32>*) const;
	void calcVelocityToJumpToY(const JGeometry::TVec3<f32>&, f32, f32) const;
	void calcVelocityToJumpToXZ(const JGeometry::TVec3<f32>&, f32, f32) const;
	void perform(u32, JDrama::TGraphics*);
	void load(JSUMemoryInputStream&);
	void initLodAnm(const TLodAnmIndex*, int, f32);
	void getModel() const;
	void calcRideMomentum();
	void calcRidePos();
	~TLiveActor();
	TLiveActor(const char*);
	void getMActor() const;

public:
	/* 0x70 */ TLiveManager* unk70;
	/* 0x74 */ MActor* unk74;
	/* 0x78 */ TMActorKeeper* unk78;
	/* 0x7C */ u16 unk7C;
	/* 0x80 */ MAnmSound* unk80;
	/* 0x84 */ JKRFileLoader* unk84;
	/* 0x88 */ void* unk88;
	/* 0x8C */ void* unk8C;
	/* 0x90 */ void* unk90;
	/* 0x94 */ float unk94;
	/* 0x98 */ float unk98;
	/* 0x9C */ float unk9C;
	/* 0xA0 */ float unkA0;
	/* 0xA4 */ float unkA4;
	/* 0xA8 */ float unkA8;
	/* 0xAC */ float unkAC;
	/* 0xB0 */ float unkB0;
	/* 0xB4 */ float unkB4;
	/* 0xB8 */ float unkB8;
	/* 0xBC */ float unkBC;
	/* 0xC0 */ float unkC0;
	/* 0xC4 */ TBGCheckData* unkC4;
	/* 0xC8 */ float unkC8;
	/* 0xCC */ float unkCC;
	/* 0xD0 */ TLodAnm* unkD0;
	/* 0xD4 */ THitActor* unkD4;
	/* 0xD8 */ float unkD8;
	/* 0xDC */ float unkDC;
	/* 0xE0 */ float unkE0;
	/* 0xE4 */ float unkE4;
	/* 0xE8 */ u8 unkE8;
	/* 0xEC */ void* unkEC;
	/* 0xF0 */ u32 unkF0;
};

#endif
