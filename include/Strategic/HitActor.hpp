#ifndef HIT_ACTOR_HPP
#define HIT_ACTOR_HPP

#include <JSystem/JDrama/JDRActor.hpp>

class THitActor : public JDrama::TActor {
public:
	THitActor** collisions;
	u16 colCount;
	u16 colCapacity;
	u32 actorType; // ActorType

	f32 attackRadius; // 0x50
	f32 attackHeight;

	f32 damageRadius;
	f32 damageHeight;

	f32 entryRadius;

	u32 _064;

	THitActor(const char*);

	virtual ~THitActor() { }

	virtual void perform(u32, JDrama::TGraphics*);
	virtual void receiveMessage(THitActor*, u32);

	// TODO: Should be at offset 0xb8 in the vtable, currently is not
	virtual MtxPtr uknownVirtFunc() = 0;

	void initHitActor(u32, u16, int, f32, f32, f32, f32);
	float calcEntryRadius();
};

#endif
