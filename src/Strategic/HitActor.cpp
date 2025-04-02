#include <Strategic/HitActor.hpp>
#include <math.h>

f32 THitActor::calcEntryRadius()
{
	float rad;
	if (mAttackRadius > mDamageRadius)
		rad = mAttackRadius;
	else
		rad = mDamageRadius;

	float height;
	if (mAttackHeight > mDamageHeight)
		height = mAttackHeight;
	else
		height = mDamageHeight;

	float height2 = height * height;
	float rad2    = rad * rad + height2;

	if (rad2 > 0.0f) {
		// TODO: some kind of a fast sqrt function?
		volatile f32 f = rad2 * __frsqrte(rad2);
		mEntryRadius   = 1.4142135f * f;
	} else {
		mEntryRadius = 0.0f;
	}

	return height2;
}

void THitActor::perform(u32 param_1, JDrama::TGraphics* param_2)
{
	JDrama::TActor::perform(param_1, param_2);
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
