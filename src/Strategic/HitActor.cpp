#include <Strategic/HitActor.hpp>
#include <math.h>

// fabricated: a refinement-free square root, the same idiom as JPASqrtf()
// minus its zero guard. The call has to be an inline rather than the body
// written out in place: only then does MWCC leave f0 free for the sqrt(2)
// literal and give the frsqrte chain f1, and only then does the expansion
// reserve the extra stack slot below the volatile at 0x30(r1).
static inline f32 HitActorFastSqrt(f32 x)
{
	volatile f32 f = x * __frsqrte(x);
	return f;
}

/**
 * @brief Recomputes the radius of the sphere this actor occupies in the hit
 * check grid, from whichever of its attack and damage volumes is larger.
 *
 * Called by every setter that touches one of the four hit params, so it has to
 * be cheap: the square root is one frsqrte estimate with no Newton refinement.
 */
void THitActor::calcEntryRadius()
{
	// The reads go through the getters: each expansion reserves a dead 4-byte
	// temporary, and the eight of them are what puts the volatile below at
	// 0x30(r1) instead of 0xc(r1).
	f32 rad;
	if (getAttackRadius() > getDamageRadius())
		rad = getAttackRadius();
	else
		rad = getDamageRadius();

	f32 height;
	if (getAttackHeight() > getDamageHeight())
		height = getAttackHeight();
	else
		height = getDamageHeight();

	f32 height2 = height * height;
	f32 rad2    = rad * rad + height2;

	if (rad2 > 0.0f) {
		f32 diag     = HitActorFastSqrt(rad2);
		mEntryRadius = 1.4142135f * diag;
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
