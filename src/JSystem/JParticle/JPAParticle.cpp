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
	unk44            = 0.0f;
	unk50            = nullptr;
}

void JPABaseParticle::incTimer()
{
	if (unk10 & 1)
		unk10 &= ~1;
	else
		unk44 += 1.0f;

	if (unk44 >= unk4C) {
		unk48 = 1.0f;
		unk10 |= 0x80;
	} else {
		unk48 = unk44 / unk4C;
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
	mAcceleration.z   = 0.0f;
	mAcceleration.y   = 0.0f;
	mAcceleration.x   = 0.0f;
}

void JPAParticle::setVelocity()
{
	JPAGetEmitterInfoPtr();
	mVelocity.add(mAcceleration);
	unk38.x
	    = mCurrentDragForce * (mBaseVelocity.x + mVelocity.x) * mDynamicsWeight;
	unk38.y
	    = mCurrentDragForce * (mBaseVelocity.y + mVelocity.y) * mDynamicsWeight;
	unk38.z
	    = mCurrentDragForce * (mBaseVelocity.z + mVelocity.z) * mDynamicsWeight;
}

void JPAParticle::calcVelocity()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	mVelocity.x = mVelocity.y = mVelocity.z = 0.0f;
	if (checkFlag(0x20))
		unk14 = info->unk24;

	if (unk78 != 0.0f)
		mBaseVelocity.scaleAdd(unk78, mBaseVelocity, unk68);

	if (!checkFlag(0x40))
		info->unk4->affectField(this);

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
	mLocalPosition.add(unk38);
	initGlobalPosition();
}

void JPABaseParticle::getCurrentPosition(JGeometry::TVec3<f32>& result)
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	result.x = info->unkC.x * (mLocalPosition.x + unk38.x) + unk14.x;
	result.y = info->unkC.y * (mLocalPosition.y + unk38.y) + unk14.y;
	result.z = info->unkC.z * (mLocalPosition.z + unk38.z) + unk14.z;
}

void JPABaseParticle::getCurrentPositionX() { }
void JPABaseParticle::getCurrentPositionY() { }
void JPABaseParticle::getCurrentPositionZ() { }

bool JPAParticle::checkCreateChildParticle()
{
	bool result          = false;
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	if (info->unk0->getUnk172()) {
		f32 time;
		if (unk4C > 1.0f)
			time = unk44 / (unk4C - 1.0f);
		else
			time = 1.0f;

		JPASweepShape* sweepShape
		    = info->unk0->getEmitterDataBlockInfoPtr()->getSweepShape();
		if (time >= sweepShape->getTiming()) {
			if (sweepShape->getStep() > 0) {
				f32 frame = unk44;
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
