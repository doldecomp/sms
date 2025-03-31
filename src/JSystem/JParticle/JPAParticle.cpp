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
	unk10   = 0;
	unk20.z = 0.0f;
	unk20.y = 0.0f;
	unk20.x = 0.0f;
	unk44   = 0.0f;
	unk50   = nullptr;
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

	unk78   = 0.0f;
	unk80   = 1.0f;
	unk84   = 1.0f;
	unk94.z = 0.0f;
	unk94.y = 0.0f;
	unk94.x = 0.0f;
}

void JPAParticle::setVelocity()
{
	JPAGetEmitterInfoPtr();
	unk88.add(unk94);
	unk38.x = unk84 * (unk5C.x + unk88.x) * unk74;
	unk38.y = unk84 * (unk5C.y + unk88.y) * unk74;
	unk38.z = unk84 * (unk5C.z + unk88.z) * unk74;
}

void JPAParticle::calcVelocity()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	unk88.x = unk88.y = unk88.z = 0.0f;
	if (checkFlag(0x20))
		unk14 = info->unk24;

	if (unk78 != 0.0f)
		unk5C.scaleAdd(unk78, unk5C, unk68);

	if (!checkFlag(0x40))
		info->unk4->affectField(this);

	if (unk7C < 1.0f)
		unk5C.scale(unk7C);

	setVelocity();
}

void JPABaseParticle::initGlobalPosition()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	unk2C.x = unk20.x * info->unkC.x + unk14.x;
	unk2C.y = unk20.y * info->unkC.y + unk14.y;
	unk2C.z = unk20.z * info->unkC.z + unk14.z;
}

void JPABaseParticle::calcGlobalPosition()
{
	unk20.add(unk38);
	initGlobalPosition();
}

void JPABaseParticle::getCurrentPosition(JGeometry::TVec3<f32>& result)
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	result.x = info->unkC.x * (unk20.x + unk38.x) + unk14.x;
	result.y = info->unkC.y * (unk20.y + unk38.y) + unk14.y;
	result.z = info->unkC.z * (unk20.z + unk38.z) + unk14.z;
}

void JPABaseParticle::getCurrentPositionX() { }
void JPABaseParticle::getCurrentPositionY() { }
void JPABaseParticle::getCurrentPositionZ() { }

bool JPAParticle::checkCreateChildParticle()
{
	bool result          = false;
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();

	if (info->unk0->unk172) {
		f32 fVar3;
		if (unk4C > 1.0f)
			fVar3 = unk44 / (unk4C - 1.0f);
		else
			fVar3 = 1.0f;

		JPASweepShape* sweepShape = info->unk0->unk118->unkC;
		if (fVar3 >= sweepShape->unk18) {
			if ((int)sweepShape->unk47 > 0) {
				if ((int)unk44 % (sweepShape->unk47 + 1) == 0)
					result = true;
			} else {
				result = true;
			}
		}
	}

	return result;
}
