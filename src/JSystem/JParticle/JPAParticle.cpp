#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAResourceManager.hpp>
#include <JSystem/JParticle/JPASweepShape.hpp>

JPABaseParticle::JPABaseParticle()
    : unk0(this)
{
}

JPABaseParticle::~JPABaseParticle() { }

void JPABaseParticle::initBase()
{
	unk10            = 0;
	mLocalPosition.z = 0.0f;
	mLocalPosition.y = 0.0f;
	mLocalPosition.x = 0.0f;
	mAge             = 0.0f;
	unk50            = nullptr;
}

void JPABaseParticle::incTimer()
{
	if (unk10 & FLAG_JUST_BORN)
		unk10 &= ~FLAG_JUST_BORN;
	else
		mAge += 1.0f;

	if (mAge >= mLifetime) {
		mLifeProgress = 1.0f;
		unk10 |= FLAG_DEAD;
	} else {
		mLifeProgress = mAge / mLifetime;
	}
}

void JPABaseParticle::executeParticleCallBack(JPABaseEmitter* emitter)
{
	if (unk50)
		unk50->execute(emitter, this);
}

void JPABaseParticle::drawParticleCallBack(JPABaseEmitter* emitter)
{
	if (unk50)
		unk50->draw(emitter, this);
}

JPAParticle::JPAParticle() { }

JPAParticle::~JPAParticle() { }

void JPAParticle::init()
{
	initBase();

	unk78             = 0.0f;
	mDragForce        = 1.0f;
	mCurrentDragForce = 1.0f;
	mFieldAcceleration.zero();
}

void JPAParticle::setVelocity()
{
	JPAGetEmitterInfoPtr();
	mFieldVelocity.add(mFieldAcceleration);

	// Don't try thinking about the physical units here too hard.
	// The original developers likely didn't either.
	mVelocity.x = mCurrentDragForce * (mBaseVelocity.x + mFieldVelocity.x)
	              * mDynamicsWeight;
	mVelocity.y = mCurrentDragForce * (mBaseVelocity.y + mFieldVelocity.y)
	              * mDynamicsWeight;
	mVelocity.z = mCurrentDragForce * (mBaseVelocity.z + mFieldVelocity.z)
	              * mDynamicsWeight;
}

void JPAParticle::calcVelocity()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	mFieldVelocity.zero();
	if (checkStatus(JPABaseParticle::FLAG_FOLLOW_EMITTER))
		unk14.set(info->unk24);

	if (unk78 != 0.0f)
		mBaseVelocity.scaleAdd(unk78, mBaseVelocity, unk68);

	if (!checkStatus(JPABaseParticle::FLAG_IGNORE_FIELDS))
		info->mCurrentFieldManager->affectField(this);

	if (mAirResistance < 1.0f)
		mBaseVelocity.scale(mAirResistance);

	setVelocity();
}

void JPABaseParticle::initGlobalPosition()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	mGlobalPosition.x = mLocalPosition.x * info->unkC.x + unk14.x;
	mGlobalPosition.y = mLocalPosition.y * info->unkC.y + unk14.y;
	mGlobalPosition.z = mLocalPosition.z * info->unkC.z + unk14.z;
}

void JPABaseParticle::calcGlobalPosition()
{
	mLocalPosition.add(mVelocity);
	initGlobalPosition();
}

void JPABaseParticle::getCurrentPosition(JGeometry::TVec3<f32>& result)
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	result.x = info->unkC.x * (mLocalPosition.x + mVelocity.x) + unk14.x;
	result.y = info->unkC.y * (mLocalPosition.y + mVelocity.y) + unk14.y;
	result.z = info->unkC.z * (mLocalPosition.z + mVelocity.z) + unk14.z;
}

void JPABaseParticle::getCurrentPositionX() { }
void JPABaseParticle::getCurrentPositionY() { }
void JPABaseParticle::getCurrentPositionZ() { }

bool JPAParticle::checkCreateChildParticle()
{
	bool result          = false;
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	if (info->mCurrentEmitter->getUnk172()) {
		f32 time;
		if (mLifetime > 1.0f)
			time = mAge / (mLifetime - 1.0f);
		else
			time = 1.0f;

		JPASweepShape* sweepShape
		    = info->mCurrentEmitter->getEmitterDataBlockInfoPtr()
		          ->getSweepShape();
		if (time >= sweepShape->getTiming()) {
			if (sweepShape->getStep() > 0) {
				f32 frame = mAge;
				s32 step  = sweepShape->getStep() + 1;
				if ((int)frame % step == 0)
					result = true;
			} else {
				result = true;
			}
		}
	}

	return result;
}
