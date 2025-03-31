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

	unk18.x = unk18.y = unk18.z = 0.0f;

	unk7C.x = unk7C.y = unk7C.z = 0.0f;

	unk24.x = 0.0f;
	unk24.y = -1.0f;
	unk24.z = 0.0f;

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
	JSUMemoryInputStream streamImpl(block->unk4, *(u32*)((u8*)block->unk4 + 4));
	JSUInputStream& stream = streamImpl; // TODO: fakematch?

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
	stream.read(&unk18.x, 4);
	stream.read(&unk18.y, 4);
	stream.read(&unk18.z, 4);
	stream.read(&unk24.x, 4);
	stream.read(&unk24.y, 4);
	stream.read(&unk24.z, 4);
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
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	JGeometry::TVec3<f32> local_18;
	local_18.set(unk24.x, unk24.y, unk24.z);
	if (!checkFlag(0x2))
		MTXMultVec(info->unk6C, &local_18, &local_18);
	unk7C.scale(unk10, local_18);
}

JPAAirField::JPAAirField() { unk50 = 1; }
void JPAAirField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	unk70                = unk24;
	if (!checkFlag(0x2)) {
		MTXMultVec(info->unk6C, &unk70, &unk70);
	}
	unk7C.scale(unk10, unk70);
	if (checkFlag(0x1)) {
		unk64.x = JMASCos(unk30 * 32768.0f);
		if (!checkFlag(0x2)) {
			MTXMultVec(info->unk6C.mMtx, &unk18, &unk58);
		} else {
			unk58 = unk18;
		}
	}
}
void JPAAirField::affect(JPAParticle* particle)
{
	if (checkFlag(0x1)) {
		JGeometry::TVec3<f32> diff;
		if (!checkFlag(0x2))
			diff.sub(particle->unk20, unk58);
		else
			diff.sub(particle->unk2C, unk58);

		diff.normalize();
		if (unk64.x <= unk70.dot(diff))
			calcFieldVelocity(particle);
	} else {
		calcFieldVelocity(particle);
	}

	if (checkFlag(0x4)) {
		JGeometry::TVec3<f32> vec;
		particle->getBaseVelVec(vec);
		f32 len = vec.length();
		if (len > unk14) {
			vec.scale(unk14 / len);
			particle->setBaseVelVec(vec);
		}
	}
}

JPAMagnetField::JPAMagnetField() { unk50 = 2; }
JPAMagnetField::~JPAMagnetField() { }
void JPAMagnetField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	if (!checkFlag(0x2)) {
		MTXMultVec(info->unk6C.mMtx, &unk18, &unk58);
	} else {
		unk58 = unk18;
	}
}
void JPAMagnetField::affect(JPAParticle* particle)
{
	if (!checkFlag(0x2))
		unk7C.sub(unk58, particle->unk20);
	else
		unk7C.sub(unk58, particle->unk2C);

	unk7C.setLength(unk10);
	calcFieldVelocity(particle);
}

JPANewtonField::JPANewtonField() { unk50 = 3; }
JPANewtonField::~JPANewtonField() { }
void JPANewtonField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	if (!checkFlag(0x2)) {
		MTXMultVec(info->unk6C.mMtx, &unk18, &unk58);
	} else {
		unk58 = unk18;
	}
	unk34 = unk30 * unk30;
}
void JPANewtonField::affect(JPAParticle* particle)
{
	if (!checkFlag(0x2))
		unk7C.sub(unk58, particle->unk20);
	else
		unk7C.sub(unk58, particle->unk2C);

	if (checkFlag(0x100)) {
		unk7C.setLength(unk10);
	} else {
		f32 lsq   = unk7C.squared();
		f32 fVar2 = lsq > unk34 ? (unk34 * 10.0f) / lsq : 10;
		unk7C.setLength(unk10 * fVar2);
	}
	calcFieldVelocity(particle);
}

JPAVortexField::JPAVortexField()
{
	unk24.x = 0.0f;
	unk24.y = 1.0f;
	unk24.z = 0.0f;
	unk50   = 4;
}
JPAVortexField::~JPAVortexField() { }
void JPAVortexField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	MTXMultVec(info->unkCC, &unk24, &unk58);
	unk30 = unk18.z * unk18.z;
	unk34 = 1.0f / unk30;
}
void JPAVortexField::affect(JPAParticle* particle)
{
	JGeometry::TVec3<f32> thing  = particle->unk20;
	JGeometry::TVec3<f32> thing2 = unk58;

	f32 dot = thing.dot(thing2);
	thing2.scale(dot);
	thing.sub(thing2);

	f32 fVar1 = thing.squared();
	if (thing.squared() > unk30)
		fVar1 = unk30;
	fVar1 *= unk34;
	fVar1 = (1.0f - fVar1) * unk10 + fVar1 * unk14;
	thing.setLength(1.0f);
	unk7C.cross(unk58, thing);
	unk7C.scale(fVar1);
	calcFieldVelocity(particle);
}
bool JPAVortexField::checkMaxDistance(JGeometry::TVec3<float>&,
                                      JGeometry::TVec3<float>&)
{
	return false;
}

JPAConvectionField::JPAConvectionField()
{
	unk50 = 7;

	unk24.x = 0.0f;
	unk24.y = 1.0f;
	unk24.z = 0.0f;

	unk18.x = 1.0f;
	unk18.y = 0.0f;
	unk18.z = 0.0f;
}
JPAConvectionField::~JPAConvectionField() { }
void JPAConvectionField::set()
{
	JPAEmitterInfo* info = JPAGetEmitterInfoPtr();
	JGeometry::TVec3<f32> prod;
	JGeometry::TVec3<f32> localUnk18 = unk18;
	JGeometry::TVec3<f32> localUnk24 = unk24;
	prod.cross(localUnk24, localUnk18);
	unk18.cross(localUnk24, prod);
	unk18.normalize();

	MTXMultVec(info->unkCC, &unk18, &unk58);
	MTXMultVec(info->unkCC, &unk24, &unk64);
	MTXMultVec(info->unkCC, &prod, &unk70);

	unk58.normalize();
	unk64.normalize();
	unk70.normalize();
}
void JPAConvectionField::affect(JPAParticle* particle)
{
	bool bVar13                 = false;
	bool bVar12                 = false;
	JGeometry::TVec3<f32> thing = particle->unk20;
	if (unk64.x == 0.0f && unk64.y == 1.0f) {
		bVar12 = true;
	}
	if (bVar12 && unk64.z == 0.0f) {
		bVar13 = true;
	}
	JGeometry::TVec3<f32> thing2;
	if (bVar13) {
		thing2.set(0.0f, 0.0f, 0.0f);
	} else {
		JGeometry::TVec3<f32> a = unk58;
		JGeometry::TVec3<f32> b = unk70;
		f32 fVar11              = a.dot(thing);
		f32 fVar10              = b.dot(thing);
		a.scale(fVar11);
		b.scale(fVar10);
		thing2.add(a, b);
	}
	thing2.setLength(thing2, unk30);
	thing.sub(thing2);
	JGeometry::TVec3<f32> thing3;
	thing3.cross(thing2, unk64);
	unk7C.cross(thing3, thing);
	unk7C.setLength(unk10);
	if (unk34 != 0.0f) {
		JGeometry::TVec3<f32> thing4;
		thing4.setLength(thing, unk34);
		unk7C.add(thing4);
	}
	calcFieldVelocity(particle);
}
bool JPAConvectionField::checkMaxDistance(JGeometry::TVec3<float>&,
                                          JGeometry::TVec3<float>&)
{
	return false;
}

JPARandomField::JPARandomField() { unk50 = 5; }
JPARandomField::~JPARandomField() { }
void JPARandomField::affect(JPAParticle* particle)
{
	bool bVar3 = false;
	if ((int)particle->unk44 == 0) {
		bVar3 = true;
	} else {
		if (unk53 != 0) {
			if ((int)particle->unk44 % unk53 == 0) {
				bVar3 = true;
			}
		}
	}

	if (bVar3) {
		unk7C.set(FieldRand.get_ufloat_1(), FieldRand.get_ufloat_1(),
		          FieldRand.get_ufloat_1());
		unk7C.scale(unk10);
		calcFieldVelocity(particle);
	}
}

JPADragField::JPADragField() { unk50 = 6; }
JPADragField::~JPADragField() { }
void JPADragField::affect(JPAParticle* particle)
{
	if (!particle->checkFlag(0x4)) {
		if ((int)unk44 == 0) {
			f32 rnd = unk14 * (FieldRand.get_ufloat_1() - 0.5f) + unk10;
			if (rnd > 1.0f)
				rnd = 1.0f;
			unk7C.y = rnd;
		}
		f32 scale = calcFieldFadeScale(particle->unk48);
		unk7C.z *= -(scale * (1.0f - particle->getDragForce()) - 1.0f);
	} else {
		unk7C.z *= particle->getDragForce();
	}
}

JPAFieldManager::JPAFieldManager() { }
JPAFieldManager::~JPAFieldManager() { }

void JPAFieldManager::deleteField(JPABaseField* field)
{
	unk0.remove(field->getLinkBufferPtr());
	unkC->prepend(field->getLinkBufferPtr());
}

void JPAFieldManager::deleteAllField()
{
	if (unk0.getNumLinks() == 0)
		return;

	JSULink<JPABaseField>* link = unk0.getFirst();
	while (link) {
		JSULink<JPABaseField>* next = link->getNext();
		JPABaseField* field         = link->getObject();
		deleteField(field);
		link = next;
	}
}

void JPAFieldManager::calcFieldParams()
{
	if (unk0.getNumLinks() == 0)
		return;

	JSULink<JPABaseField>* link = unk0.getFirst();
	while (link) {
		JSULink<JPABaseField>* next = link->getNext();
		JPABaseField* field         = link->getObject();

		if (field->checkFlag(0x80))
			field->unk88 = field->unk3C * field->unk3C;
		field->set();

		link = next;
	}
}

void JPAFieldManager::affectField(JPAParticle* particle)
{
	JGeometry::TVec3<f32>& particlePos = particle->unk2C;
	JSUListIterator<JPABaseField> it;
	JPABaseField* field;

	for (it = unk0.getFirst(); it != unk0.getEnd(); ++it) {
		field = it.getObject();
		if (!field->checkFlag(0x80)) {
			field->affect(particle);
		} else if (!field->checkMaxDistance(particlePos, field->getUnk18())) {
			field->affect(particle);
		}
	}
}

JPABaseField* JPAFieldManager::setField(u8 param_1)
{
	JPABaseField* result = nullptr;
	switch (param_1) {
	case 0:
		result = setGravityField();
		break;
	case 1:
		result = setAirField();
		break;
	case 2:
		result = setMagnetField();
		break;
	case 3:
		result = setNewtonField();
		break;
	case 4:
		result = setVortexField();
		break;
	case 5:
		result = setRandomField();
		break;
	case 6:
		result = setDragField();
		break;
	case 7:
		result = setConvectionField();
		break;
	}
	return result;
}

JPAGravityField* JPAFieldManager::setGravityField()
{
	JPAGravityField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPAGravityField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPAGravityField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAAirField* JPAFieldManager::setAirField()
{
	JPAAirField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPAAirField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPAAirField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAMagnetField* JPAFieldManager::setMagnetField()
{
	JPAMagnetField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPAMagnetField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPAMagnetField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPANewtonField* JPAFieldManager::setNewtonField()
{
	JPANewtonField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPANewtonField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPANewtonField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAVortexField* JPAFieldManager::setVortexField()
{
	JPAVortexField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPAVortexField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPAVortexField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPAConvectionField* JPAFieldManager::setConvectionField()
{
	JPAConvectionField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPAConvectionField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPAConvectionField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPARandomField* JPAFieldManager::setRandomField()
{
	JPARandomField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPARandomField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPARandomField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}

JPADragField* JPAFieldManager::setDragField()
{
	JPADragField* result = nullptr;

	if (unkC->getNumLinks()) {
		result = (JPADragField*)unkC->getFirst()->getObject();
		unkC->remove(result->getLinkBufferPtr());
		new (result) JPADragField;
		unk0.append(result->getLinkBufferPtr());
	}

	return result;
}
