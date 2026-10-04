#include <Strategic/HitActor.hpp>
#include <math.h>

void THitActor::calcEntryRadius()
{
	f32 rad;
	if (mAttackRadius > mDamageRadius)
		rad = mAttackRadius;
	else
		rad = mDamageRadius;

	f32 height;
	if (mAttackHeight > mDamageHeight)
		height = mAttackHeight;
	else
		height = mDamageHeight;

	f32 dist = rad * rad + height * height;
	if (dist > 0.0f) {
		volatile f32 y = dist * __frsqrte(dist);
		dist           = y;
		mEntryRadius   = 1.4142135f * dist;
	} else {
		mEntryRadius = 0.0f;
	}
}

void THitActor::perform(u32 cue, JDrama::TGraphics* graphics)
{
	JDrama::TActor::perform(cue, graphics);
}

void THitActor::initHitActor(u32 actor_type, u16 max_collisions, int hit_flags,
                             f32 attack_radius, f32 attack_height,
                             f32 damage_radius, f32 damage_height)
{
	mActorType   = actor_type;
	mColCapacity = max_collisions;
	mCollisions  = new THitActor*[mColCapacity];

	for (int i = 0; i < mColCapacity; ++i)
		mCollisions[i] = nullptr;

	onHitFlag(hit_flags);

	mAttackRadius = attack_radius;
	mAttackHeight = attack_height;
	mDamageRadius = damage_radius;
	mDamageHeight = damage_height;

	calcEntryRadius();
}

THitActor::THitActor(const char* name)
    : JDrama::TActor(name)
    , mCollisions(nullptr)
    , mColCount(0)
    , mColCapacity(0)
    , mActorType(0)
    , mAttackRadius(0.0f)
    , mAttackHeight(0.0f)
    , mDamageRadius(0.0f)
    , mDamageHeight(0.0f)
    , mEntryRadius(0.0f)
    , mHitFlags(0)
{
}
