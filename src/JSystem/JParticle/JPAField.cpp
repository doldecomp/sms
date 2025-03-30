#include <JSystem/JParticle/JPAField.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JParticle/JPADataBlock.hpp>
#include <JSystem/JParticle/JPAMath.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JSupport/JSUMemoryInputStream.hpp>
#include <JSystem/JMath.hpp>

static JMath::TRandom_fast_ FieldRand(0);

JPABaseField::JPABaseField()
    : unk0(this)
{
	unk54 = 0;
	unk20 = 0.0f;
	unk1C = 0.0f;
	unk18 = 0.0f;

	unk7C.x = unk7C.y = unk7C.z = 0.0f;

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
bool JPABaseField::checkMaxDistance(JGeometry::TVec3<float>& param_1,
                                    JGeometry::TVec3<float>& param_2)
{
	JGeometry::TVec3<f32> diff;
	bool result = false;
	diff.sub(param_1, param_2);
	diff.mul(diff);
	if (unk88 <= diff.x + diff.y + diff.z)
		result = true;
	return result;
}

f32 JPABaseField::calcFieldFadeScale(float param_1)
{
	bool bVar2 = false;
	f32 dVar3  = 1.0f;
	if (checkFlag(0x8) && param_1 < unk48) {
		dVar3 = 0.0f;
		bVar2 = true;
	}

	if (checkFlag(0x10) && unk4C <= param_1) {
		dVar3 = 0.0f;
		bVar2 = true;
	}

	if (!bVar2) {
		if (checkFlag(0x40) && param_1 >= unk44) {
			f32 diff = unk4C - unk44;
			if (diff > 0.0f)
				dVar3 = __fres(diff) * (unk4C - param_1);
		} else if (checkFlag(0x20) && param_1 < unk40) {
			f32 diff = unk40 - unk48;
			if (diff > 0.0f)
				dVar3 = __fres(diff) * (param_1 - unk48);
		}
	}

	return dVar3;
}

void JPABaseField::calcFieldVelocity(JPAParticle* particle)
{
	JGeometry::TVec3<f32> local_14 = unk7C;
	if (particle->checkFlag(4)) {
		// TODO: getter fpr unk38
		f32 scale = calcFieldFadeScale(particle->unk48);
		if (scale <= 0.0f) {
			local_14.set(0.0f, 0.0f, 0.0f);
		} else if (scale < 1.0f)
			local_14.scale(scale);
	}

	switch (unk52) {
	case 0:
		particle->unk94.add(local_14);
		break;
	case 1:
		particle->addBaseVelVec(local_14);
		break;
	case 2:
		particle->unk88.add(local_14);
		break;
	}
}

void JPABaseField::affect(JPAParticle* particle)
{
	calcFieldVelocity(particle);
}
void JPABaseField::loadFieldBlock(JPADataBlock* block)
{
	JSUMemoryInputStream stream(block->unk4, *(u32*)((u8*)block->unk4 + 4));

	stream.skip(0xC);
	stream.read(&unk50, 1);
	stream.read(&unk51, 1);
	stream.read(&unk52, 1);
	stream.read(&unk53, 1);
	stream.read(&unk54, 1);
	stream.skip(0x2);
	stream.read(&unk10, 4);
	stream.read(&unk14, 4);
	stream.read(&unk3C, 4);
	stream.read(&unk18, 4);
	stream.read(&unk1C, 4);
	stream.read(&unk20, 4);
	stream.read(&unk24, 4);
	stream.read(&unk28, 4);
	stream.read(&unk2C, 4);
	stream.read(&unk30, 4);
	stream.read(&unk34, 4);
	stream.read(&unk38, 4);

	unk40 = JPAConvertFixToFloat(stream.readS16());
	unk44 = JPAConvertFixToFloat(stream.readS16());
	unk48 = JPAConvertFixToFloat(stream.readS16());
	unk4C = JPAConvertFixToFloat(stream.readS16());
}

JPAGravityField::JPAGravityField() { unk50 = 0; }
JPAGravityField::~JPAGravityField() { }
void JPAGravityField::set()
{
	JPAEmitterInfo* info           = JPAGetEmitterInfoPtr();
	JGeometry::TVec3<f32> local_18 = info->unk24;
	if (!checkFlag(0x2))
		PSMTXMultVec(info->unk6C.mMtx, local_18.toVec(), local_18.toVec());
	unk7C.scale(unk10, local_18);
}

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
JPABaseField* JPAFieldManager::setField(u8) { }
void JPAFieldManager::setGravityField() { }
void JPAFieldManager::setAirField() { }
void JPAFieldManager::setMagnetField() { }
void JPAFieldManager::setNewtonField() { }
void JPAFieldManager::setVortexField() { }
void JPAFieldManager::setConvectionField() { }
void JPAFieldManager::setRandomField() { }
void JPAFieldManager::setDragField() { }
