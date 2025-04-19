#ifndef STRATEGIC_HIT_ACTOR_HPP
#define STRATEGIC_HIT_ACTOR_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class THitActor : public JDrama::TActor {
public:
	THitActor(const char*);

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
