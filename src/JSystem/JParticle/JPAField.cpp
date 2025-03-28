#include <JSystem/JParticle/JPAField.hpp>
#include <JSystem/JMath.hpp>

static JMath::TRandom_fast_ FieldRand(0);

JPABaseField::JPABaseField()
    : unk0(this)
{
	unk54 = 0;
	unk20 = 0.0f;
	unk1C = 0.0f;
	unk18 = 0.0f;
	unk84 = 0.0f;
	unk80 = 0.0f;
	unk7C = 0.0f;
	unk24 = 0.0f;
	unk28 = -1.0f;
	unk2C = 0.0f;
	unk10 = 1.0f;
	unk14 = 0.0f;
	unk40 = 0.0f;
	unk44 = 1.0f;
	unk48 = 0.0f;
	unk4C = 1.0f;
	unk3C = 0.0f;
	unk52 = 0;
}
JPABaseField::~JPABaseField() { }
void JPABaseField::set() { }
void JPABaseField::calcMaxDistance() { }
bool JPABaseField::checkMaxDistance(JGeometry::TVec3<float>&,
                                    JGeometry::TVec3<float>&)
{
}
void JPABaseField::calcFieldFadeScale(float) { }
void JPABaseField::calcFieldVelocity(JPAParticle*) { }
void JPABaseField::affect(JPAParticle*) { }
void JPABaseField::loadFieldBlock(JPADataBlock*) { }

JPAGravityField::JPAGravityField() { unk50 = 0; }
JPAGravityField::~JPAGravityField() { }
void JPAGravityField::set() { }

JPAAirField::JPAAirField() { unk50 = 1; }
void JPAAirField::set() { }
void JPAAirField::affect(JPAParticle*) { }

JPAMagnetField::JPAMagnetField() { unk50 = 2; }
JPAMagnetField::~JPAMagnetField() { }
void JPAMagnetField::set() { }
void JPAMagnetField::affect(JPAParticle*) { }

JPANewtonField::JPANewtonField() { unk50 = 3; }
JPANewtonField::~JPANewtonField() { }
void JPANewtonField::set() { }
void JPANewtonField::affect(JPAParticle*) { }

JPAVortexField::JPAVortexField()
{
	unk24 = 0.0f;
	unk28 = 1.0f;
	unk2C = 0.0f;
	unk50 = 4;
}
JPAVortexField::~JPAVortexField() { }
void JPAVortexField::set() { }
void JPAVortexField::affect(JPAParticle*) { }
bool JPAVortexField::checkMaxDistance(JGeometry::TVec3<float>&,
                                      JGeometry::TVec3<float>&)
{
	return false;
}

JPAConvectionField::JPAConvectionField()
{
	unk50 = 7;
	unk24 = 0.0f;
	unk28 = 1.0f;
	unk2C = 0.0f;
	unk18 = 1.0f;
	unk1C = 0.0f;
	unk20 = 0.0f;
}
JPAConvectionField::~JPAConvectionField() { }
void JPAConvectionField::set() { }
void JPAConvectionField::affect(JPAParticle*) { }
bool JPAConvectionField::checkMaxDistance(JGeometry::TVec3<float>&,
                                          JGeometry::TVec3<float>&)
{
	return false;
}

JPARandomField::JPARandomField() { unk50 = 5; }
JPARandomField::~JPARandomField() { }
void JPARandomField::affect(JPAParticle*) { }

JPADragField::JPADragField() { unk50 = 6; }
JPADragField::~JPADragField() { }
void JPADragField::affect(JPAParticle*) { }

JPAFieldManager::JPAFieldManager() { }
JPAFieldManager::~JPAFieldManager() { }
void JPAFieldManager::deleteField(JPABaseField*) { }
void JPAFieldManager::deleteAllField() { }
void JPAFieldManager::calcFieldParams() { }
void JPAFieldManager::affectField(JPAParticle*) { }
void JPAFieldManager::setField(unsigned char) { }
void JPAFieldManager::setGravityField() { }
void JPAFieldManager::setAirField() { }
void JPAFieldManager::setMagnetField() { }
void JPAFieldManager::setNewtonField() { }
void JPAFieldManager::setVortexField() { }
void JPAFieldManager::setConvectionField() { }
void JPAFieldManager::setRandomField() { }
void JPAFieldManager::setDragField() { }
