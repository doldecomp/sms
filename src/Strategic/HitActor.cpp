#include <Strategic/HitActor.hpp>
#include <math.h>

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
    , unk64(0)
{
}

float THitActor::initHitActor(u32 param_1, u16 param_2, int param_3,
                              float attack_radius, float attack_height,
                              float damage_radius, float damage_height)
{
	mActorType   = param_1;
	mColCapacity = param_2;
	mCollisions  = new THitActor*[mColCapacity];

	for (int i = 0; i < mColCapacity; ++i)
		mCollisions[i] = nullptr;

	unk64 |= param_3;

	mAttackRadius = attack_radius;
	mAttackHeight = attack_height;
	mDamageRadius = damage_radius;
	mDamageHeight = damage_height;

	return calcEntryRadius();
}

void THitActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	JDrama::TActor::perform(param_1, param_2);
}

f32 THitActor::calcEntryRadius()
{
	float rad = mDamageRadius > mAttackRadius ? mDamageRadius : mAttackRadius;
	float height
	    = mDamageHeight > mAttackHeight ? mDamageHeight : mAttackHeight;

	float height2 = height * height;
	float rad2    = rad * rad + height2;

	// floats, how do they work
	if (rad2 > 0.0f)
		mEntryRadius = 1.414214f * __frsqrte(rad2);
	else
		mEntryRadius = 0.0f;

	return height2;
}
