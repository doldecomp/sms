#ifndef STRATEGIC_HIT_ACTOR_HPP
#define STRATEGIC_HIT_ACTOR_HPP

#include <JSystem/JDrama/JDRActor.hpp>

enum TActorTypeBits {
	ACTOR_TYPE_PLAYER = 0x80000000,
	ACTOR_TYPE_ENEMY  = 0x10000000,
	ACTOR_TYPE_BOSS   = 0x8000000,
};

class THitActor : public JDrama::TActor {
public:
	THitActor(const char* = "HitActor");

	virtual ~THitActor() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual u32 receiveMessage(THitActor*, u32);

	float initHitActor(u32, u16, int, f32, f32, f32, f32);
	float calcEntryRadius();

	// fabricated
	u32 getActorType() const { return mActorType; }
	bool checkActorType(u32 flag) const
	{
		return mActorType & flag ? true : false;
	}
	bool isActorType(u32 flag) const
	{
		return mActorType == flag ? true : false;
	}
	THitActor* getCollision(int i) { return mCollisions[i]; }
	u16 getColNum() { return mColCount; }
	bool checkHitFlag(u32 flag) { return unk64 & flag; }
	void onHitFlag(u32 flag) { unk64 |= flag; }
	void offHitFlag(u32 flag) { unk64 &= ~flag; }

public:
	/* 0x44 */ THitActor** mCollisions;
	/* 0x48 */ u16 mColCount;
	/* 0x4A */ u16 mColCapacity;
	/* 0x4C */ u32 mActorType;
	/* 0x50 */ float mAttackRadius;
	/* 0x54 */ float mAttackHeight;
	/* 0x58 */ float mDamageRadius;
	/* 0x5C */ float mDamageHeight;
	/* 0x60 */ float mEntryRadius;
	/* 0x64 */ u32 unk64;
};

#endif
