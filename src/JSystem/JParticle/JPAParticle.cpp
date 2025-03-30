#include <JSystem/JParticle/JPAParticle.hpp>

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

void JPAParticle::setVelocity() { }
void JPAParticle::calcVelocity() { }
void JPABaseParticle::initGlobalPosition() { }
void JPABaseParticle::calcGlobalPosition() { }
void JPABaseParticle::getCurrentPosition(JGeometry::TVec3<float>&) { }
void JPABaseParticle::getCurrentPositionX() { }
void JPABaseParticle::getCurrentPositionY() { }
void JPABaseParticle::getCurrentPositionZ() { }
bool JPAParticle::checkCreateChildParticle() { }
