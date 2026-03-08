#ifndef STRATEGIC_HIT_ACTOR_HPP
#define STRATEGIC_HIT_ACTOR_HPP

#include <JSystem/JDrama/JDRActor.hpp>

enum TActorTypeBits {
	ACTOR_TYPE_UNK4000000  = 0x4000000,
	ACTOR_TYPE_BOSS        = 0x8000000,
	ACTOR_TYPE_ENEMY       = 0x10000000,
	ACTOR_TYPE_UNK40000000 = 0x40000000,
	ACTOR_TYPE_PLAYER      = 0x80000000,

	ACTOR_TYPE_MASK = 0xFFFF0000,
};

enum THitMessageType {
	HIT_MESSAGE_TRAMPLE          = 0,
	HIT_MESSAGE_HIP_DROP         = 1,
	HIT_MESSAGE_UNK2             = 2,
	HIT_MESSAGE_UNK3             = 3,
	HIT_MESSAGE_TAKE             = 4,
	HIT_MESSAGE_UNK5             = 5,
	HIT_MESSAGE_UNK6             = 6,
	HIT_MESSAGE_UNK7             = 7,
	HIT_MESSAGE_UNK8             = 8,
	HIT_MESSAGE_UNKA             = 0xA,
	HIT_MESSAGE_UNKB             = 0xB,
	HIT_MESSAGE_PUNCH            = 0xC,
	HIT_MESSAGE_UNKD             = 0xD,
	HIT_MESSAGE_ATTACK           = 0xE, // TODO: attack -> touch?!
	HIT_MESSAGE_SPRAYED_BY_WATER = 0xF,
	HIT_MESSAGE_UNK10            = 0x10,
};

enum THitFlagBits {
	HIT_FLAG_NO_COLLISION = 0x1,
	HIT_FLAG_UNK2         = 0x2,
	HIT_FLAG_UNK4         = 0x4,
	HIT_FLAG_UNK8000000   = 0x8000000,
	HIT_FLAG_UNK10000000  = 0x10000000,
	HIT_FLAG_UNK40000000  = 0x40000000,
};

class THitActor : public JDrama::TActor {
public:
	THitActor(const char* = "HitActor");

	virtual ~THitActor() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual BOOL receiveMessage(THitActor* sender, u32 message)
	{
		return false;
	}

	float initHitActor(u32, u16, int, f32 attack_radius, f32 attack_height,
	                   f32 damage_radius, f32 damage_height);
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
	bool checkHitFlag2(u32 flag) { return unk64 & flag ? true : false; }
	void onHitFlag(u32 flag) { unk64 |= flag; }
	void offHitFlag(u32 flag) { unk64 &= ~flag; }
	f32 getAttackRadius() const { return mAttackRadius; }
	f32 getAttackHeight() const { return mAttackHeight; }
	f32 getDamageRadius() const { return mDamageRadius; }
	f32 getDamageHeight() const { return mDamageHeight; }

	void setAttackRadius(f32 v)
	{
		mAttackRadius = v;
		calcEntryRadius();
	}
	void setAttackHeight(f32 v)
	{
		mAttackHeight = v;
		calcEntryRadius();
	}
	void setDamageRadius(f32 v)
	{
		mDamageRadius = v;
		calcEntryRadius();
	}
	void setDamageHeight(f32 v)
	{
		mDamageHeight = v;
		calcEntryRadius();
	}
	void setHitParams(f32 attack_radius, f32 attack_height, f32 damage_radius,
	                  f32 damage_height)
	{
		mAttackRadius = attack_radius;
		mAttackHeight = attack_height;
		mDamageRadius = damage_radius;
		mDamageHeight = damage_height;
		calcEntryRadius();
	}
	f32 getEntryRadius() const { return mEntryRadius; }

public:
	/* 0x44 */ THitActor** mCollisions;
	/* 0x48 */ u16 mColCount;
	/* 0x4A */ u16 mColCapacity;
	/* 0x4C */ u32 mActorType;
	/* 0x50 */ f32 mAttackRadius;
	/* 0x54 */ f32 mAttackHeight;
	/* 0x58 */ f32 mDamageRadius;
	/* 0x5C */ f32 mDamageHeight;
	/* 0x60 */ f32 mEntryRadius;
	/* 0x64 */ u32 unk64;
};

#endif
